
// Constants and data members
//------------------------------------------------------------------------------
const TString _gridui = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/";
const TString _lxplus = "eos/cms/store/group/phys_higgs/cmshww/amassiro/HWW6p3/";


const Bool_t _savepdf = false;
const Bool_t _savepng = true;


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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void metFilters(TString dataset = "DoubleEG")
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  TChain* tree = new TChain("latino", "latino");

  tree->Add(_lxplus + "21Jun2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr__l2tight/latino_Run2016B_PromptReco_" + dataset + ".root");
  tree->Add(_lxplus + "05Jul2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr__l2tight/latino_Run2016B_PromptReco_" + dataset + ".root");
  tree->Add(_lxplus + "08Jul2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr__l2tight/latino_Run2016B_PromptReco_" + dataset + ".root");
  tree->Add(_lxplus + "08Jul2016_Run2016C_PromptReco/l2loose__hadd__EpTCorr__l2tight/latino_Run2016C_PromptReco_" + dataset + ".root");

  TH1D* before = new TH1D("before", "", 20, 0, 1000);
  TH1D* after  = new TH1D("after",  "", 20, 0, 1000);

  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);

  c1->SetLogy();

  tree->Draw("metPfType1>>before", "trigger");
  tree->Draw("metPfType1>>after",  "trigger * metFilter");

  before->SetFillColor(kGray);
  before->SetFillStyle(1001);
  before->SetLineColor(kGray);

  after->SetMarkerStyle(kFullCircle);

  before->Draw("hist");
  after ->Draw("ep,same");

  Float_t nbefore    = before->Integral(-1,-1);
  Float_t nafter     = after ->Integral(-1,-1);
  Float_t efficiency = 1e2*nafter/nbefore;


  // Legend
  //----------------------------------------------------------------------------
  before->GetXaxis()->SetTitleOffset(1.5);
  before->GetYaxis()->SetTitleOffset(1.8);

  before->SetXTitle("E_{T}^{miss} [GeV]");
  before->SetYTitle(Form("events / %.0f GeV", before->GetBinWidth(0)));

  DrawLatex(42, 0.190, 0.945, 0.045, 11, dataset);
  DrawLatex(42, 0.940, 0.945, 0.045, 31, "6.3 fb^{-1} (13 TeV)");

  DrawLegend(0.55, 0.83, before, "before filters", "f");
  DrawLegend(0.55, 0.77, after,  Form("after filters (%.2f%%)", efficiency), "ep");


  // Save
  //----------------------------------------------------------------------------
  c1->GetFrame()->DrawClone();

  if (_savepdf) gSystem->mkdir("pdf", kTRUE);
  if (_savepng) gSystem->mkdir("png", kTRUE);

  if (_savepdf) c1->SaveAs("pdf/metFilters_" + dataset + ".pdf");
  if (_savepng) c1->SaveAs("png/metFilters_" + dataset + ".png");
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
