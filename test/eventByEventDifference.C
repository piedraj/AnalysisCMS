#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2.h"
#include "TString.h"
#include "TSystem.h"


const TString path = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC";


//------------------------------------------------------------------------------
//
//  root -l 'eventByEventDifference.C+("latino_ZZZ")'
//
//------------------------------------------------------------------------------
void eventByEventDifference(TString filename = "latino_ttDM0001pseudo00020")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");


  // Read inputs
  //----------------------------------------------------------------------------
  printf("\n Reading %s\n", filename.Data());

  TChain* chain    = new TChain("latino", "latino");
  TChain* chain_up = new TChain("latino", "latino");
  TChain* chain_do = new TChain("latino", "latino");

  chain   ->Add(path + "/"        + filename + ".root");
  chain_up->Add(path + "__METup/" + filename + ".root");
  chain_do->Add(path + "__METdo/" + filename + ".root");


  // Prepare tree branches
  //----------------------------------------------------------------------------
  vector<float> *std_vector_lepton_flavour = 0;
  vector<float> *std_vector_lepton_pt      = 0;

  float metPfType1;
  float mll;
  float njet;

  float metPfType1_up;
  float metPfType1_do;

  chain->SetBranchAddress("std_vector_lepton_flavour", &std_vector_lepton_flavour);
  chain->SetBranchAddress("std_vector_lepton_pt",      &std_vector_lepton_pt);

  chain->SetBranchAddress("metPfType1", &metPfType1);
  chain->SetBranchAddress("mll",        &mll);
  chain->SetBranchAddress("njet",       &njet);

  chain_up->SetBranchAddress("metPfType1", &metPfType1_up);
  chain_do->SetBranchAddress("metPfType1", &metPfType1_do);


  // Prepare histograms
  //----------------------------------------------------------------------------
  TString titles = filename + ";MET up - MET nominal [GeV];MET do - MET nominal [GeV]";

  TH2F* deltaMet_all = new TH2F("deltaMet_all", titles, 100, -25, 25, 100, -25, 25);
  TH2F* deltaMet_bad = new TH2F("deltaMet_bad", titles, 100, -25, 25, 100, -25, 25);


  // Loop
  //----------------------------------------------------------------------------
  Long64_t nentries    = chain   ->GetEntries();
  Long64_t nentries_up = chain_up->GetEntries();
  Long64_t nentries_do = chain_do->GetEntries();

  printf ("\n Loop over %lld events\n", nentries);

  if (nentries_up != nentries) printf("\n Warning: up file has %lld events\n", nentries_up);
  if (nentries_do != nentries) printf("\n Warning: do file has %lld events\n", nentries_do);

  for (Long64_t jentry=0; jentry<nentries; jentry++) {

    chain   ->GetEntry(jentry);
    chain_up->GetEntry(jentry);
    chain_do->GetEntry(jentry);


    // Selection
    //--------------------------------------------------------------------------
    if (std_vector_lepton_pt->at(0) < 25.) continue;

    if (std_vector_lepton_pt->at(1) < 20.) continue;

    if (std_vector_lepton_pt->at(2) > 10.) continue;

    if (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) > 0) continue;

    if (njet < 2) continue;

    if (mll < 20.) continue;

    if (fabs(mll - 91.188) < 15.) continue;

    if (metPfType1 < 50.) continue;


    // Fill histograms
    //--------------------------------------------------------------------------
    float deltaMet_up = metPfType1_up - metPfType1;
    float deltaMet_do = metPfType1_do - metPfType1;

    deltaMet_all->Fill(deltaMet_up, deltaMet_do);

    if (deltaMet_up * deltaMet_do <= 0) continue;

    deltaMet_bad->Fill(deltaMet_up, deltaMet_do);
  }


  // Print statistics
  //----------------------------------------------------------------------------
  float fraction_bad = 1e2 * deltaMet_bad->GetEntries() / deltaMet_all->GetEntries();

  printf("\n %.0f events have passed the selection\n", deltaMet_all->GetEntries());

  printf("\n %.2f%% of these events have MET (up-nominal)*(down-nominal) > 0\n\n", fraction_bad);


  // Draw all
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);

  c1->SetRightMargin(3.5 * c1->GetRightMargin());

  deltaMet_all->GetXaxis()->SetTitleOffset(1.5);
  deltaMet_all->GetYaxis()->SetTitleOffset(1.7);

  deltaMet_all->Draw("colz");

  c1->SaveAs(filename + "__deltaMet_all.png");


  // Draw bad
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);

  c2->SetRightMargin(3.5 * c2->GetRightMargin());

  deltaMet_bad->GetXaxis()->SetTitleOffset(1.5);
  deltaMet_bad->GetYaxis()->SetTitleOffset(1.7);

  deltaMet_bad->Draw("colz");

  c2->SaveAs(filename + "__deltaMet_bad.png");
}
