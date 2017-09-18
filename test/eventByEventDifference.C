#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2.h"
#include "TString.h"
#include "TSystem.h"


const TString path = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC";


//------------------------------------------------------------------------------
//
//  root -l eventByEventDifference.C
//
//------------------------------------------------------------------------------
void eventByEventDifference(TString filename = "ttDM0001pseudo00020")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  TChain* chain    = new TChain("latino", "latino");
  TChain* chain_up = new TChain("latino", "latino");
  TChain* chain_do = new TChain("latino", "latino");

  chain   ->Add(path + "/latino_"        + filename + ".root");
  chain_up->Add(path + "__METup/latino_" + filename + ".root");
  chain_do->Add(path + "__METdo/latino_" + filename + ".root");

  float metPfType1;
  float metPfType1_up;
  float metPfType1_do;

  chain   ->SetBranchAddress("metPfType1", &metPfType1);
  chain_up->SetBranchAddress("metPfType1", &metPfType1_up);
  chain_do->SetBranchAddress("metPfType1", &metPfType1_do);


  // Loop
  //----------------------------------------------------------------------------
  TH2F* deltaMet_all = new TH2F("deltaMet_all", ";MET up - MET nominal [GeV];MET do - MET nominal [GeV]", 100, -25, 25, 100, -25, 25);
  TH2F* deltaMet_bad = new TH2F("deltaMet_bad", ";MET up - MET nominal [GeV];MET do - MET nominal [GeV]", 100, -25, 25, 100, -25, 25);

  Long64_t nentries = chain->GetEntries();

  printf ("\n Loop over %lld entries\n\n", nentries);

  for (Long64_t jentry=0; jentry<nentries; jentry++) {

    chain   ->GetEntry(jentry);
    chain_up->GetEntry(jentry);
    chain_do->GetEntry(jentry);

    float deltaMet_up = metPfType1_up - metPfType1;
    float deltaMet_do = metPfType1_do - metPfType1;

    deltaMet_all->Fill(deltaMet_up, deltaMet_do);

    if (deltaMet_up * deltaMet_do <= 0) continue;

    deltaMet_bad->Fill(deltaMet_up, deltaMet_do);
  }


  // Draw all
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);

  c1->SetRightMargin(3.5 * c1->GetRightMargin());

  deltaMet_all->GetXaxis()->SetTitleOffset(1.5);
  deltaMet_all->GetYaxis()->SetTitleOffset(1.7);

  deltaMet_all->Draw("colz");

  c1->SaveAs("deltaMet_all.png");


  // Draw bad
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);

  c2->SetRightMargin(3.5 * c2->GetRightMargin());

  deltaMet_bad->GetXaxis()->SetTitleOffset(1.5);
  deltaMet_bad->GetYaxis()->SetTitleOffset(1.7);

  deltaMet_bad->Draw("colz");

  c2->SaveAs("deltaMet_bad.png");
}
