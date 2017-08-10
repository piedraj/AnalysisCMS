#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "THStack.h"
#include "TString.h"
#include "TSystem.h"


// Constants
//------------------------------------------------------------------------------
const TString path = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/";
const TString sll  = "#font[12]{ll}";
const TString sm   = "E_{T}^{miss}";


// Data members
//------------------------------------------------------------------------------
TChain* chain1;
TChain* chain2;


// Functions
//------------------------------------------------------------------------------
void Draw(TString variable,
	  TCut    supercut,
	  TString name,
	  TString xtitle,
	  Int_t   nbins,
	  Float_t xmin,
	  Float_t xmax);


//------------------------------------------------------------------------------
//
//  root -l -b -q 'compareTrees.C+("__wwSel")'
//
//    https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/python/variables/WWVar.C#L772
//
//    mth = sqrt(2 * ptll * met * (1 - cos(dphillmet)));
//
//------------------------------------------------------------------------------
void compareTrees(TString skim = "__wwSel")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir("png", kTRUE);

  TH1::SetDefaultSumw2();


  // Read files
  //----------------------------------------------------------------------------
  chain1 = new TChain("latino", "latino");

  chain1->Add(path + "Apr2017_Run2016B_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016B-03Feb2017_ver2-v2.root");
  chain1->Add(path + "Apr2017_Run2016C_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016C-03Feb2017-v1.root");
  chain1->Add(path + "Apr2017_Run2016D_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016D-03Feb2017-v1.root");
  chain1->Add(path + "Apr2017_Run2016E_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016E-03Feb2017-v1.root");
  chain1->Add(path + "Apr2017_Run2016F_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016F-03Feb2017-v1.root");
  chain1->Add(path + "Apr2017_Run2016G_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016G-03Feb2017-v1__part0.root");
  chain1->Add(path + "Apr2017_Run2016G_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016G-03Feb2017-v1__part1.root");
  chain1->Add(path + "Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver2-v1__part0.root");
  chain1->Add(path + "Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver2-v1__part1.root");
  chain1->Add(path + "Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver3-v1.root");

  chain2 = new TChain("latino", "latino");

  chain2->Add(path + "Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC" + skim + "/latino_WWTo2L2Nu.root");


  // Draw
  //----------------------------------------------------------------------------
  Draw("std_vector_lepton_phi[0]", "mth > 60", "h_lep1phi_mth60", "leading lepton #phi", 60, -3.3, 3.3);
  Draw("std_vector_lepton_phi[0]", "mth < 60", "h_lep1phi",       "leading lepton #phi", 60, -3.3, 3.3);

  Draw("std_vector_jet_phi[0]", "mth > 60 && std_vector_jet_pt[0] > 20", "h_jet1phi_mth60", "leading jet #phi", 60, -3.3, 3.3);
  Draw("std_vector_jet_phi[0]", "mth < 60 && std_vector_jet_pt[0] > 20", "h_jet1phi",       "leading jet #phi", 60, -3.3, 3.3);

  Draw("dphillmet", "mth > 60", "h_dphillmet_mth60", "#Delta#phi(" + sll + "," + sm + ")", 60, 0, 3.3);
  Draw("dphillmet", "mth < 60", "h_dphillmet",       "#Delta#phi(" + sll + "," + sm + ")", 60, 0, 3.3);
}


//------------------------------------------------------------------------------
// Draw
//------------------------------------------------------------------------------
void Draw(TString variable,
	  TCut    supercut,
	  TString name,
	  TString xtitle,
	  Int_t   nbins,
	  Float_t xmin,
	  Float_t xmax)
{
  TH1D* hist1 = new TH1D(name + "1", name, nbins, xmin, xmax);
  TH1D* hist2 = new TH1D(name + "2", name, nbins, xmin, xmax);

  TCanvas* canvas = new TCanvas(name, name);

  chain1->Draw(variable + ">>" + name + "1", supercut);  // Faster (took 18 minutes on 2017/08/09 at lxplus)
  chain2->Draw(variable + ">>" + name + "2", supercut);

  //  chain1->Project(name + "1", variable, supercut);  // Slower (took 34 minutes on 2017/08/09 at lxplus)
  //  chain2->Project(name + "2", variable, supercut);

  hist1->Scale(1. / hist1->Integral());
  hist2->Scale(1. / hist2->Integral());


  // Cosmetics
  //----------------------------------------------------------------------------
  hist1->SetMarkerStyle(kFullCircle);


  // Draw
  //----------------------------------------------------------------------------
  THStack* hs = new THStack();

  hs->Add(hist2, "hist");
  hs->Add(hist1, "ep");

  hs->Draw("nostack");


  // Labels
  //----------------------------------------------------------------------------
  hs->GetXaxis()->SetTitle(xtitle);
  hs->GetXaxis()->SetTitleOffset(1.7);
  hs->SetTitle(supercut);


  // Save
  //----------------------------------------------------------------------------
  canvas->SaveAs("png/" + name + ".png");
}
