#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include <fstream>
#include <iostream>
#include <vector>


const double ELECTRON_MASS = 0.000511;  // [GeV]
const double MUON_MASS     = 0.106;     // [GeV]


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
// Results reading the latino_TTTo2L2Nu__part7.root tree,
//
//    number of events genmatched       = 807582
//    number of events genpromptmatched = 806753
//    fraction of duplicates            = 0.103%
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void checkGenMatch(TString filename = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut__l2tight/latino_TTTo2L2Nu__part7.root")
{
  TFile* file = TFile::Open(filename);

  TTree* tree = (TTree*)file->Get("latino");


  // Get the variables of interest
  //----------------------------------------------------------------------------
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

  int counter_genmatched       = 0;  // Events currently matched in the latino framework
  int counter_genpromptmatched = 0;  // Events with more strict matching


  // Loop over events
  //----------------------------------------------------------------------------
  Long64_t nentries = tree->GetEntries();

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    tree->GetEntry(jentry);

    if (std_vector_lepton_pt->at(0) < 10) continue;
    if (std_vector_lepton_pt->at(1) < 10) continue;

    if (abs(std_vector_lepton_flavour->at(0)) != 11 && abs(std_vector_lepton_flavour->at(0)) != 13) continue;
    if (abs(std_vector_lepton_flavour->at(1)) != 11 && abs(std_vector_lepton_flavour->at(1)) != 13) continue;

    if (std_vector_lepton_genmatched->at(0) < 1) continue;
    if (std_vector_lepton_genmatched->at(1) < 1) continue;

    counter_genmatched++;


    // Loop over RECO leptons
    //--------------------------------------------------------------------------
    UInt_t firstLeptonGen  = 999;
    UInt_t secondLeptonGen = 999;

    Bool_t event_genpromptmatched = false;

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


	// Do the matching
	//----------------------------------------------------------------------
	if (lepton_tlorentz.DeltaR(leptonGen_tlorentz) < deltaRMin) {

	  if (i == 0) firstLeptonGen = j;

	  if (i == 1 && firstLeptonGen > 10) continue;

	  if (i == 1 && j == firstLeptonGen) continue;

	  if (i == 1) secondLeptonGen = j;

	  if (i == 1 && secondLeptonGen > 10) continue;

	  deltaRMin = lepton_tlorentz.DeltaR(leptonGen_tlorentz);

	  if (abs(std_vector_leptonGen_MotherPID->at(j)) == 11 ||
	      abs(std_vector_leptonGen_MotherPID->at(j)) == 13 ||
	      abs(std_vector_leptonGen_MotherPID->at(j)) == 15 ||
	      abs(std_vector_leptonGen_MotherPID->at(j)) == 23 ||
	      abs(std_vector_leptonGen_MotherPID->at(j)) == 24) {

	    event_genpromptmatched = true;
	  }
	}
      }
    }

    if (event_genpromptmatched) counter_genpromptmatched++;
  }


  // Print the results
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" number of events genmatched       = %d\n",     counter_genmatched);
  printf(" number of events genpromptmatched = %d\n",     counter_genpromptmatched);
  printf(" fraction of duplicates            = %.3f%%\n", 1e2 - 1e2*counter_genpromptmatched/counter_genmatched);
  printf("\n");
}
