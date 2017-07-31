#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "THStack.h"
#include "TString.h"
#include "TSystem.h"


// Constants
//------------------------------------------------------------------------------
const TString sll = "#font[12]{ll}";
const TString sm  = "E_{T}^{miss}";


// Data members
//------------------------------------------------------------------------------
TChain* latino_dt;
TChain* latino_mc;


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
//  root -l -b -q 'compareDataMC.C+("__wwSel")'
//
//    https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/python/variables/WWVar.C#L772
//
//    mth = sqrt(2 * ptll * met * (1 - cos(dphillmet)));
//
//------------------------------------------------------------------------------
void compareDataMC(TString skim = "")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir("png", kTRUE);

  TH1::SetDefaultSumw2();


  // Read files
  //----------------------------------------------------------------------------
  latino_dt = new TChain("latino", "latino");
  latino_mc = new TChain("latino", "latino");

  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016B_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016B-03Feb2017_ver2-v2.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016C_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016C-03Feb2017-v1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016D_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016D-03Feb2017-v1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016E_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016E-03Feb2017-v1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016F_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016F-03Feb2017-v1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016G_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016G-03Feb2017-v1__part0.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016G_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016G-03Feb2017-v1__part1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver2-v1__part0.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver2-v1__part1.root");
  latino_dt->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA" + skim + "/latino_DoubleEG_Run2016H-03Feb2017_ver3-v1.root");

  latino_mc->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC" + skim + "/latino_WWTo2L2Nu.root");


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
  TH1D* h_dt = new TH1D(name + "_dt", name, nbins, xmin, xmax);
  TH1D* h_mc = new TH1D(name + "_mc", name, nbins, xmin, xmax);

  TCanvas* canvas = new TCanvas(name, name);

  latino_dt->Draw(variable + ">>" + name + "_dt", supercut);
  latino_mc->Draw(variable + ">>" + name + "_mc", supercut);

  h_dt->Scale(1. / h_dt->Integral());
  h_mc->Scale(1. / h_mc->Integral());


  // Cosmetics
  //----------------------------------------------------------------------------
  h_dt->SetMarkerStyle(kFullCircle);


  // Draw
  //----------------------------------------------------------------------------
  THStack* hs = new THStack();

  hs->Add(h_mc, "hist");
  hs->Add(h_dt, "ep");

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
