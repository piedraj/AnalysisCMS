#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include <fstream>
#include <iostream>
#include <vector>


const double   ELECTRON_MASS = 0.000511;  // [GeV]
const double   MUON_MASS     = 0.106;     // [GeV]
const Long64_t MAX_ENTRIES   = 2e5;


void PrintPercent(TString label,
		  int     counter1,
		  int     counter2,
		  int     denominator);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// This macro checks the latino script below [1] and estimates the fraction of
// fake events passing the genmatched selection.
//
// [1] https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/python/variables/genMatchVar.py
//
//------------------------------------------------------------------------------
//
// How to run this macro,
//
//    root -l -b -q checkGenMatch.C+
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void checkGenMatch(TString filename = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut__l2tight/latino_DYJetsToLL_M-50__part7.root")
{
  printf("\n Reading %s\n", filename.Data());

  TFile* file = TFile::Open(filename);

  TTree* tree = (TTree*)file->Get("latino");


  // Get the variables of interest
  //----------------------------------------------------------------------------
  float mll;

  vector<float> *std_vector_lepton_eta = 0;
  vector<float> *std_vector_lepton_flavour = 0;
  vector<float> *std_vector_lepton_genmatched = 0;
  vector<float> *std_vector_lepton_phi = 0;
  vector<float> *std_vector_lepton_pt = 0;

  vector<float> *std_vector_leptonGen_eta = 0;
  vector<float> *std_vector_leptonGen_isDirectPromptTauDecayProduct = 0;
  vector<float> *std_vector_leptonGen_isPrompt = 0;
  vector<float> *std_vector_leptonGen_MotherPID = 0;
  vector<float> *std_vector_leptonGen_phi = 0;
  vector<float> *std_vector_leptonGen_pid = 0;
  vector<float> *std_vector_leptonGen_pt = 0;
  vector<float> *std_vector_leptonGen_status = 0;

  tree->SetBranchAddress("mll", &mll);

  tree->SetBranchAddress("std_vector_lepton_eta",        &std_vector_lepton_eta);
  tree->SetBranchAddress("std_vector_lepton_flavour",    &std_vector_lepton_flavour);
  tree->SetBranchAddress("std_vector_lepton_genmatched", &std_vector_lepton_genmatched);
  tree->SetBranchAddress("std_vector_lepton_phi",        &std_vector_lepton_phi);
  tree->SetBranchAddress("std_vector_lepton_pt",         &std_vector_lepton_pt);

  tree->SetBranchAddress("std_vector_leptonGen_eta",                           &std_vector_leptonGen_eta);
  tree->SetBranchAddress("std_vector_leptonGen_isDirectPromptTauDecayProduct", &std_vector_leptonGen_isDirectPromptTauDecayProduct);
  tree->SetBranchAddress("std_vector_leptonGen_isPrompt",                      &std_vector_leptonGen_isPrompt);
  tree->SetBranchAddress("std_vector_leptonGen_MotherPID",                     &std_vector_leptonGen_MotherPID);
  tree->SetBranchAddress("std_vector_leptonGen_phi",                           &std_vector_leptonGen_phi);
  tree->SetBranchAddress("std_vector_leptonGen_pid",                           &std_vector_leptonGen_pid);
  tree->SetBranchAddress("std_vector_leptonGen_pt",                            &std_vector_leptonGen_pt);
  tree->SetBranchAddress("std_vector_leptonGen_status",                        &std_vector_leptonGen_status);

  int counter_genmatched = 0;

  int counter1_nonprompt          = 0;
  int counter1_mother_1_d         = 0;
  int counter1_mother_2_u         = 0;
  int counter1_mother_3_s         = 0;
  int counter1_mother_4_c         = 0;
  int counter1_mother_5_b         = 0;
  int counter1_mother_6_t         = 0;
  int counter1_mother_11_electron = 0;
  int counter1_mother_13_muon     = 0;
  int counter1_mother_15_tau      = 0;
  int counter1_mother_21_gluon    = 0;
  int counter1_mother_22_photon   = 0;
  int counter1_mother_23_Z        = 0;
  int counter1_mother_24_W        = 0;
  int counter1_mother_2212_proton = 0;
  int counter1_mother_other       = 0;

  int counter2_nonprompt          = 0;
  int counter2_mother_1_d         = 0;
  int counter2_mother_2_u         = 0;
  int counter2_mother_3_s         = 0;
  int counter2_mother_4_c         = 0;
  int counter2_mother_5_b         = 0;
  int counter2_mother_6_t         = 0;
  int counter2_mother_11_electron = 0;
  int counter2_mother_13_muon     = 0;
  int counter2_mother_15_tau      = 0;
  int counter2_mother_21_gluon    = 0;
  int counter2_mother_22_photon   = 0;
  int counter2_mother_23_Z        = 0;
  int counter2_mother_24_W        = 0;
  int counter2_mother_2212_proton = 0;
  int counter2_mother_other       = 0;


  // Loop over events
  //----------------------------------------------------------------------------
  Long64_t nentries = tree->GetEntries();

  if (MAX_ENTRIES > -1 && nentries > MAX_ENTRIES) nentries = MAX_ENTRIES;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    tree->GetEntry(jentry);

    //    if (mll > 75 && mll < 105) continue;  // Require to be outside of the Z-peak

    if (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) != -11*13) continue;  // Require different flavour

    if (std_vector_lepton_pt->at(0) < 25) continue;
    if (std_vector_lepton_pt->at(1) < 10) continue;

    if (abs(std_vector_lepton_flavour->at(0)) != 11 && abs(std_vector_lepton_flavour->at(0)) != 13) continue;
    if (abs(std_vector_lepton_flavour->at(1)) != 11 && abs(std_vector_lepton_flavour->at(1)) != 13) continue;

    if (std_vector_lepton_genmatched->at(0) < 1) continue;
    if (std_vector_lepton_genmatched->at(1) < 1) continue;

    counter_genmatched++;


    // Loop over 1st and 2nd RECO leptons
    //--------------------------------------------------------------------------
    UInt_t firstLeptonGen  = 999;
    UInt_t secondLeptonGen = 999;

    for (UInt_t i=0; i<2; i++) {

      float std_vector_lepton_mass = (abs(std_vector_lepton_flavour->at(i)) == 11) ? ELECTRON_MASS : MUON_MASS;

      TLorentzVector lepton_tlorentz;

      lepton_tlorentz.SetPtEtaPhiM(std_vector_lepton_pt->at(i),
				   std_vector_lepton_eta->at(i),
				   std_vector_lepton_phi->at(i),
				   std_vector_lepton_mass);


      // Loop over GEN leptons
      //------------------------------------------------------------------------
      float deltaRMin = 0.3;

      for (UInt_t j=0; j<std_vector_leptonGen_pt->size(); j++) {

	if (std_vector_leptonGen_pt->at(j) < 0) continue;

	if (std_vector_leptonGen_status->at(j) != 1) continue;

	if (abs(std_vector_leptonGen_pid->at(j)) != 11 && abs(std_vector_leptonGen_pid->at(j)) != 13) continue;

	if (std_vector_leptonGen_isPrompt->at(j) != 1 && std_vector_leptonGen_isDirectPromptTauDecayProduct->at(j) != 1) continue;

	float std_vector_leptonGen_mass = (abs(std_vector_leptonGen_pid->at(j)) == 11) ? ELECTRON_MASS : MUON_MASS;

	TLorentzVector leptonGen_tlorentz;

	leptonGen_tlorentz.SetPtEtaPhiM(std_vector_leptonGen_pt->at(j),
					std_vector_leptonGen_eta->at(j),
					std_vector_leptonGen_phi->at(j),
					std_vector_leptonGen_mass);


	// Get the 1st and 2nd GEN lepton index
	//----------------------------------------------------------------------
	if (lepton_tlorentz.DeltaR(leptonGen_tlorentz) < deltaRMin) {

	  if (i == 0) firstLeptonGen = j;

	  if (i == 1 && j == firstLeptonGen) continue;

	  if (i == 1) secondLeptonGen = j;

	  deltaRMin = lepton_tlorentz.DeltaR(leptonGen_tlorentz);
	}
      }
    }


    // Fill the 1st lepton counters
    //--------------------------------------------------------------------------
    if (firstLeptonGen < 999) {

      int lep1mpid = abs(std_vector_leptonGen_MotherPID->at(firstLeptonGen));

      if      (lep1mpid ==    1) ++counter1_mother_1_d;
      else if (lep1mpid ==    2) ++counter1_mother_2_u;
      else if (lep1mpid ==    3) ++counter1_mother_3_s;
      else if (lep1mpid ==    4) ++counter1_mother_4_c;
      else if (lep1mpid ==    5) ++counter1_mother_5_b;
      else if (lep1mpid ==    6) ++counter1_mother_6_t;
      else if (lep1mpid ==   11) ++counter1_mother_11_electron;
      else if (lep1mpid ==   13) ++counter1_mother_13_muon;
      else if (lep1mpid ==   15) ++counter1_mother_15_tau;
      else if (lep1mpid ==   21) ++counter1_mother_21_gluon;
      else if (lep1mpid ==   22) ++counter1_mother_22_photon;
      else if (lep1mpid ==   23) ++counter1_mother_23_Z;
      else if (lep1mpid ==   24) ++counter1_mother_24_W;
      else if (lep1mpid == 2212) ++counter1_mother_2212_proton;
      else {
	++counter1_mother_other;
	printf(" lep1mpid: %d\n", lep1mpid);
      }
    }
    else ++counter1_nonprompt;


    // Fill the 2nd lepton counters
    //--------------------------------------------------------------------------
    if (secondLeptonGen < 999) {

      int lep2mpid = abs(std_vector_leptonGen_MotherPID->at(secondLeptonGen));

      if      (lep2mpid ==    1) ++counter2_mother_1_d;
      else if (lep2mpid ==    2) ++counter2_mother_2_u;
      else if (lep2mpid ==    3) ++counter2_mother_3_s;
      else if (lep2mpid ==    4) ++counter2_mother_4_c;
      else if (lep2mpid ==    5) ++counter2_mother_5_b;
      else if (lep2mpid ==    6) ++counter2_mother_6_t;
      else if (lep2mpid ==   11) ++counter2_mother_11_electron;
      else if (lep2mpid ==   13) ++counter2_mother_13_muon;
      else if (lep2mpid ==   15) ++counter2_mother_15_tau;
      else if (lep2mpid ==   21) ++counter2_mother_21_gluon;
      else if (lep2mpid ==   22) ++counter2_mother_22_photon;
      else if (lep2mpid ==   23) ++counter2_mother_23_Z;
      else if (lep2mpid ==   24) ++counter2_mother_24_W;
      else if (lep2mpid == 2212) ++counter2_mother_2212_proton;
      else {
	++counter2_mother_other;
	printf(" lep2mpid: %d\n", lep2mpid);
      }
    }
    else ++counter2_nonprompt;
  }


  // Print the results
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" number of events genmatched: %d for %s\n\n", counter_genmatched, filename.Data());
  printf(" mother PID |    lep1 |    lep2\n");
  printf("------------+---------+---------\n");
  PrintPercent("non-prompt", counter1_nonprompt,          counter2_nonprompt,          counter_genmatched);
  PrintPercent("d",          counter1_mother_1_d,         counter2_mother_1_d,         counter_genmatched);
  PrintPercent("u",          counter1_mother_2_u,         counter2_mother_2_u,         counter_genmatched);
  PrintPercent("s",          counter1_mother_3_s,         counter2_mother_3_s,         counter_genmatched);
  PrintPercent("c",          counter1_mother_4_c,         counter2_mother_4_c,         counter_genmatched);
  PrintPercent("b",          counter1_mother_5_b,         counter2_mother_5_b,         counter_genmatched);
  PrintPercent("t",          counter1_mother_6_t,         counter2_mother_6_t,         counter_genmatched);
  PrintPercent("electron",   counter1_mother_11_electron, counter2_mother_11_electron, counter_genmatched);
  PrintPercent("muon",       counter1_mother_13_muon,     counter2_mother_13_muon,     counter_genmatched);
  PrintPercent("tau",        counter1_mother_15_tau,      counter2_mother_15_tau,      counter_genmatched);
  PrintPercent("gluon",      counter1_mother_21_gluon,    counter2_mother_21_gluon,    counter_genmatched);
  PrintPercent("photon",     counter1_mother_22_photon,   counter2_mother_22_photon,   counter_genmatched);
  PrintPercent("Z",          counter1_mother_23_Z,        counter2_mother_23_Z,        counter_genmatched);
  PrintPercent("W",          counter1_mother_24_W,        counter2_mother_24_W,        counter_genmatched);
  PrintPercent("proton",     counter1_mother_2212_proton, counter2_mother_2212_proton, counter_genmatched);
  PrintPercent("other",      counter1_mother_other,       counter2_mother_other,       counter_genmatched);
  printf("\n");
}


//------------------------------------------------------------------------------
// PrintPercent
//------------------------------------------------------------------------------
void PrintPercent(TString label,
		  int     counter1,
		  int     counter2,
		  int     denominator)
{
  float factor = 1e2 / denominator;

  if (factor * counter1 < 0.01 || factor * counter2 < 0.01) return;

  printf(" %-10s | %6.2f%% | % 6.2f%%\n", label.Data(), factor * counter1, factor * counter2);

  return;
}
