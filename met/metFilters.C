#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TString.h"
#include "TSystem.h"


// Constants and data members
//------------------------------------------------------------------------------
Bool_t _ismc = true;

enum {B, C, D, E, F, G, H, nrun};

TString srun[nrun] = {"B", "C", "D", "E", "F", "G", "H"};


// Functions
//------------------------------------------------------------------------------
void     DrawLatex (Font_t      tfont,
		    Float_t     x,
		    Float_t     y,
		    Float_t     tsize,
		    Short_t     align,
		    const char* text,
		    Bool_t      setndc = true);

TLegend* DrawLegend(Float_t     x1,
		    Float_t     y1,
		    TH1*        hist,
		    TString     label,
		    TString     option  = "p",
		    Float_t     tsize   = 0.035,
		    Float_t     xoffset = 0.184,
		    Float_t     yoffset = 0.043);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// metFilters
//
//   https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#Moriond_2017
//   https://github.com/latinos/LatinoTrees/blob/master/AnalysisStep/python/skimEventProducer_cfi.py#L383-L392
//
//   Flag_HBHENoiseFilter                     #0
//   Flag_HBHENoiseIsoFilter                  #1
//   Flag_EcalDeadCellTriggerPrimitiveFilter  #2
//   Flag_goodVertices                        #3
//   Flag_eeBadScFilter                       #4
//   Flag_globalTightHalo2016Filter           #5
//   Flag_duplicateMuons                      #6 -> 0 is good // Giovanni's filter
//   Flag_badMuons                            #7 -> 0 is good // Giovanni's filter
//   Bad PF Muon Filter                       #8              // ICHEP additional filter
//   Bad Charged Hadrons                      #9              // ICHEP additional filter
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void metFilters(TString input = "NONE")
{
  if (input.EqualTo("NONE")) return;

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  if (input.EqualTo("SingleMuon"))     _ismc = false;
  if (input.EqualTo("SingleElectron")) _ismc = false;
  if (input.EqualTo("DoubleMuon"))     _ismc = false;
  if (input.EqualTo("DoubleEG"))       _ismc = false;
  if (input.EqualTo("MuonEG"))         _ismc = false;


  // Get the input files
  //----------------------------------------------------------------------------
  TChain* tree = new TChain("latino", "latino");

  if (_ismc)
    {
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut__l2tight/latino_" + input + "*.root");
    }
  else
    {
      for (int i=0; i<nrun; i++)
	tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_Run2016" + srun[i] + "_RemAOD/l2looseCut__hadd__EpTCorr__TrigMakerData__l2tight/latino_" + input + "_Run2016" + srun[i] + "-03Feb2017*.root");
    }

  printf("\n Reading %lld events from %s\n\n", tree->GetEntries(), input.Data());


  // Prepare the trigger for data
  //----------------------------------------------------------------------------
  TCut trigger = "1";

  if (!_ismc)
    {
      if (input.EqualTo("MuonEG"))         trigger = " trig_EleMu";
      if (input.EqualTo("DoubleMuon"))     trigger = "!trig_EleMu &&  trig_DbleMu";
      if (input.EqualTo("SingleMuon"))     trigger = "!trig_EleMu && !trig_DbleMu &&  trig_SnglMu";
      if (input.EqualTo("DoubleEG"))       trigger = "!trig_EleMu && !trig_DbleMu && !trig_SnglMu &&  trig_DbleEle";
      if (input.EqualTo("SingleElectron")) trigger = "!trig_EleMu && !trig_DbleMu && !trig_SnglMu && !trig_DbleEle && trig_SnglEle";
    }


  // Prepare the MET filters
  //----------------------------------------------------------------------------
  TCut commonFilters = "std_vector_trigger_special[0]*std_vector_trigger_special[1]*std_vector_trigger_special[2]*std_vector_trigger_special[3]*std_vector_trigger_special[5]";
  TCut dataFilters   = "std_vector_trigger_special[4]*!std_vector_trigger_special[6]*!std_vector_trigger_special[7]*std_vector_trigger_special[8]*std_vector_trigger_special[9]";
  TCut mcFiltersOld  = "std_vector_trigger_special[6]*std_vector_trigger_special[7]";  // Some MC were not produced with the latest skimEventProducer_cfi.py
  TCut mcFiltersNew  = "std_vector_trigger_special[8]*std_vector_trigger_special[9]";
  TCut latinoCheck   = "std_vector_trigger_special[8] == -2";

  TCut applyFilters;

  if (_ismc)
    {
      applyFilters = commonFilters && ((latinoCheck && mcFiltersOld) || (!latinoCheck && mcFiltersNew));
    }
  else
    {
      applyFilters = commonFilters && dataFilters;
    }


  // Draw
  //----------------------------------------------------------------------------
  TH1D* before = new TH1D("before", "", 20, 0, 1000);
  TH1D* after  = new TH1D("after",  "", 20, 0, 1000);

  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);

  c1->SetLogy();

  tree->Draw("metPfType1>>before", trigger);    
  tree->Draw("metPfType1>>after",  trigger && applyFilters);

  before->SetFillColor(kGray);
  before->SetFillStyle(1001);
  before->SetLineColor(kGray);

  after->SetMarkerStyle(kFullCircle);

  before->Draw("hist");
  after ->Draw("ep,same");

  Float_t nbefore    = before->Integral(-1,-1);
  Float_t nafter     = after ->Integral(-1,-1);
  Float_t efficiency = 1e2*nafter/nbefore;

  printf(" before MET filters: %.0f\n", nbefore);
  printf("  after MET filters: %.0f\n", nafter);
  printf("\n %.2f%%  %s\n\n", efficiency, input.Data());


  // Legend
  //----------------------------------------------------------------------------
  before->GetXaxis()->SetTitleOffset(1.5);
  before->GetYaxis()->SetTitleOffset(1.8);

  before->SetXTitle("E_{T}^{miss} [GeV]");
  before->SetYTitle(Form("events / %.0f GeV", before->GetBinWidth(0)));

  DrawLatex(42, 0.190, 0.945, 0.045, 11, input);

  if (_ismc) DrawLatex(42, 0.940, 0.945, 0.045, 31, "(13 TeV)");
  else       DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");

  DrawLegend(0.55, 0.83, before, "before filters", "f");
  DrawLegend(0.55, 0.77, after,  Form("after filters (%.2f%%)", efficiency), "ep");


  // Save
  //----------------------------------------------------------------------------
  c1->GetFrame()->DrawClone();

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);

  c1->SaveAs("pdf/metFilters_" + input + ".pdf");
  c1->SaveAs("png/metFilters_" + input + ".png");
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void DrawLatex(Font_t      tfont,
	       Float_t     x,
	       Float_t     y,
	       Float_t     tsize,
	       Short_t     align,
	       const char* text,
	       Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
TLegend* DrawLegend(Float_t x1,
		    Float_t y1,
		    TH1*    hist,
		    TString label,
		    TString option,
		    Float_t tsize,
		    Float_t xoffset,
		    Float_t yoffset)
{
  TLegend* legend = new TLegend(x1,
				y1,
				x1 + xoffset,
				y1 + yoffset);
  
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (tsize);

  TString final_label = Form(" %s", label.Data());

  legend->AddEntry(hist, final_label.Data(), option.Data());
  legend->Draw();

  return legend;
}
