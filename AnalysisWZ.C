#define AnalysisWZ_cxx
#include "AnalysisWZ.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>
#include <iostream>


const int n_channel = 4;

enum {
  EEE,
  EEM,
  MME,
  MMM
};

const TString s_channel[n_channel] = {
  "EEE",
  "EEM",
  "MME",
  "MMM"
};


const int n_cut = 3;

enum {
  Exactly3Leptons,
  HasZ,
  HasW
};

const TString s_cut[n_cut] = {
  "Exactly3Leptons",
  "HasZ",
  "HasW"
};


int n_event[n_channel][n_cut];


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWZ::Loop()
{
  for (int i=0; i<n_channel; i++)
    for (int j=0; j<n_cut; j++)
      n_event[i][j] = 0;


  // Loop over events
  //----------------------------------------------------------------------------
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  printf("\n Will run on %lld events\n\n", nentries);

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    if (jentry%10000 == 0) std::cout << "." << std::flush;


    // Loop over leptons
    //--------------------------------------------------------------------------
    int vsize = std_vector_lepton_pt->size();

    int n_electron = 0;
    int n_muon     = 0;

    for (int i=0; i<vsize; i++) {

      if (std_vector_lepton_pt->at(i) < 10) continue;

      float aeta = fabs(std_vector_lepton_eta->at(i));

      // Electrons
      if (fabs(std_vector_lepton_id->at(i)) == 11)
	{
	  if (aeta > 2.5)                  continue;
	  if (!IsTightLepton(i))           continue;
	  if (RelativeIsolation(i) > 0.15) continue;

	  n_electron++;
	}
      // Muons
      else if (fabs(std_vector_lepton_id->at(i)) == 13)
	{
	  if (aeta > 2.4)                  continue;
	  if (!IsTightLepton(i))           continue;
	  if (RelativeIsolation(i) > 0.12) continue;
	  
	  n_muon++;
	}
    }
    
    int n_lepton = n_electron + n_muon;

    if (n_lepton != 3) continue;

    if      (n_electron == 3) n_event[EEE][Exactly3Leptons]++;
    else if (n_electron == 2) n_event[EEM][Exactly3Leptons]++;
    else if (n_electron == 1) n_event[MME][Exactly3Leptons]++;
    else if (n_electron == 0) n_event[MMM][Exactly3Leptons]++;
  }


  // Summary
  //----------------------------------------------------------------------------
  printf("\n\n");
  for (int i=0; i<n_channel; i++)
    printf(" n_event[%s][%s]: %d\n",
	   s_channel[i].Data(),
	   s_cut[Exactly3Leptons].Data(),
	   n_event[i][Exactly3Leptons]);
  printf("\n");
}


//------------------------------------------------------------------------------
// RelativeIsolation
//------------------------------------------------------------------------------
float AnalysisWZ::RelativeIsolation(int k)
{
  float isolation = -999;

  if (fabs(std_vector_lepton_id->at(k)) == 13)
    {
      isolation = std_vector_lepton_chargedHadronIso->at(k) + max(float(0.0), float(std_vector_lepton_photonIso->at(k) + std_vector_lepton_neutralHadronIso->at(k) - 0.5*std_vector_lepton_sumPUPt->at(k)));
    }
  else if (fabs(std_vector_lepton_id->at(k)) == 11)
    {
      float aeta = fabs(std_vector_lepton_eta->at(k));

      float effective_area = -999;

      if      (aeta >  2.2)               effective_area = 0.2680;
      else if (aeta >= 2.0 && aeta < 2.2) effective_area = 0.1565;
      else if (aeta >= 1.3 && aeta < 2.0) effective_area = 0.1077;
      else if (aeta >= 0.8 && aeta < 1.3) effective_area = 0.1734;
      else if (aeta <  0.8)               effective_area = 0.1830;

      isolation = std_vector_lepton_chargedHadronIso->at(k) + max(float(0.0), float(std_vector_lepton_photonIso->at(k) + std_vector_lepton_neutralHadronIso->at(k) - jetRho*effective_area));
    }
  
  float relative_isolation = isolation / std_vector_lepton_pt->at(k);

  return relative_isolation;
}


//------------------------------------------------------------------------------
// IsTightLepton
//------------------------------------------------------------------------------
bool AnalysisWZ::IsTightLepton(int k)
{
  bool is_tight_lepton = false;

  // Muon tight ID
  if (fabs(std_vector_lepton_id->at(k)) == 13)
    {
      is_tight_lepton = std_vector_lepton_isTightMuon->at(k);
    }
  // Electron cut based medium ID
  else if (fabs(std_vector_lepton_id->at(k)) == 11)
    {
      float aeta = fabs(std_vector_lepton_eta->at(k));

      if (aeta <= 1.479)
	{
	  if (fabs(std_vector_deltaEtaIn->at(k)) < 0.008925 &&
	      fabs(std_vector_deltaPhiIn->at(k)) < 0.035973 &&
	      std_vector_sigmaIetaIeta->at(k)    < 0.009996 &&
	      std_vector_HoE->at(k)              < 0.050537)
	    {
	      is_tight_lepton = true;
	    }
	}
      else if (aeta > 1.479 && aeta < 2.5)
	{
	  if (fabs(std_vector_deltaEtaIn->at(k)) < 0.009277 &&
	      fabs(std_vector_deltaPhiIn->at(k)) < 0.094739 &&
	      std_vector_sigmaIetaIeta->at(k)    < 0.010331 &&
	      std_vector_HoE->at(k)              < 0.093068)
	    {
	      is_tight_lepton = true;
	    }
	}
    }

  return is_tight_lepton;
}
