#define AnalysisWZ_cxx
#include "AnalysisWZ.h"


//==============================================================================
//
// Constants, enums and structs
//
//==============================================================================
const float E_MASS =  0.000511;  // [GeV]
const float M_MASS =  0.106;     // [GeV]
const float Z_MASS = 91.1876;    // [GeV]


const int nchannel = 4;

enum {
  eee,
  eem,
  mme,
  mmm
};

const TString schannel[nchannel] = {
  "eee",
  "eem",
  "mme",
  "mmm"
};


const int ncut = 3;

enum {
  Exactly3Leptons,
  HasZ,
  HasW
};

const TString scut[ncut] = {
  "Exactly3Leptons",
  "HasZ",
  "HasW"
};


enum {Muon, Electron};

enum {Loose, Tight};

struct Lepton
{
  int            index;
  int            flavor;  // Muon, Electron
  int            type;    // Loose, Tight
  float          charge;
  TLorentzVector v;
};


//==============================================================================
//
// Data members
//
//==============================================================================
std::vector<Lepton> AnalysisLeptons;
Lepton              WLepton;
Lepton              ZLepton1;
Lepton              ZLepton2;

float               luminosity;
float               mll;
int                 channel;
int                 nelectron;
ofstream            txt_summary;
ofstream            txt_mmm;
TFile*              root_output;
TString             directory;
TString             filename;

TH1F*               hcounter_e;
TH1F*               hcounter_m;
TH1F*               hcounter[nchannel][ncut];


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWZ::Loop()
{
  luminosity = 500;
  directory  = "test";
  filename   = "WZ13TeV";

  gSystem->mkdir(directory, kTRUE);

  root_output = new TFile(directory + "/" + filename + ".root", "recreate");

  txt_mmm.open(directory + "/" + filename + "_mmm.txt");


  // Initialize histograms
  //----------------------------------------------------------------------------
  hcounter_e = new TH1F("hcounter_e", "", 3, 0, 3);
  hcounter_m = new TH1F("hcounter_m", "", 3, 0, 3);

  for (int i=0; i<nchannel; i++) {
    for (int j=0; j<ncut; j++) {

      hcounter[i][j] = new TH1F("hcounter_" + schannel[i] + "_" + scut[j], "", 3, 0, 3);
    }
  }


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
    AnalysisLeptons.clear();

    int vsize = std_vector_lepton_pt->size();

    for (int i=0; i<vsize; i++) {

      if (!IsFiducialLepton(i)) continue;

      float pt  = std_vector_lepton_pt ->at(i);
      float eta = std_vector_lepton_eta->at(i);
      float phi = std_vector_lepton_phi->at(i);
      float id  = std_vector_lepton_id ->at(i);

      Lepton lep;
      
      lep.index  = i;
      lep.charge = id;
      lep.type   = Loose;
      
      float mass = -999;

      if (fabs(id) == 11)
	{
	  lep.flavor = Electron;
	  mass = E_MASS;
	}
      else if (fabs(id) == 13)
	{
	  lep.flavor = Muon;
	  mass = M_MASS;
	}

      if (!IsTightLepton(i))    continue;
      if (!IsIsolatedLepton(i)) continue;

      if (lep.flavor == Electron) hcounter_e->Fill(1);
      if (lep.flavor == Muon)     hcounter_m->Fill(1);

      lep.type = Tight;

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(pt, eta, phi, mass);
      
      lep.v = tlv;

      AnalysisLeptons.push_back(lep);
    }


    // Synchronization
    //--------------------------------------------------------------------------
    if ((run == 1 && lumi == 1355 && event == 135499) ||
        (run == 1 && lumi == 1593 && event == 159297) ||
        (run == 1 && lumi == 2101 && event ==  10040) ||
        (run == 1 && lumi ==  363 && event ==  36225) ||
        (run == 1 && lumi ==  404 && event ==  40309) ||
        (run == 1 && lumi ==  444 && event ==  44384))
      {
	printf("\n%u:%u:%u -- ", run, lumi, event);
	
	for (UInt_t i=0; i<AnalysisLeptons.size(); i++)
	  {
	    TString lepton_flavor = (AnalysisLeptons[i].flavor == Electron) ? "e" : "m";
	    printf("%s", lepton_flavor.Data());
	  }
	printf("\n");
      }
    
    
    // Require exactly three leptons
    //--------------------------------------------------------------------------
    if (AnalysisLeptons.size() != 3) continue;


    // Classify the channels
    //--------------------------------------------------------------------------
    nelectron = 0;

    for (int i=0; i<3; i++)
      {
	if (AnalysisLeptons[i].flavor == Electron) nelectron++;
      }

    channel = -1;

    if      (nelectron == 0) channel = mmm;
    else if (nelectron == 1) channel = mme;
    else if (nelectron == 2) channel = eem;
    else if (nelectron == 3) channel = eee;


    // Make Z and W candidates
    //--------------------------------------------------------------------------
    mll = 999;

    for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

      for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {
      
	if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

	if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0.) continue;

	float inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	if (fabs(inv_mass - Z_MASS) < fabs(mll - Z_MASS)) {

	  mll = inv_mass;

	  ZLepton1 = AnalysisLeptons[i];
	  ZLepton2 = AnalysisLeptons[j];
	  
	  for (UInt_t k=0; k<3; k++) {
	
	    if (k == i) continue;
	    if (k == j) continue;

	    WLepton = AnalysisLeptons[k];
	  }
	}
      }
    }


    // Fill histograms
    //--------------------------------------------------------------------------
    FillHistograms(channel, Exactly3Leptons);


    // Synchronization
    //--------------------------------------------------------------------------
    if (channel == mmm)
      {
	txt_mmm << Form("%u:%u:%u\n", run, lumi, event);
      }


    if (fabs(mll - Z_MASS) > 20.) continue;
    if (ZLepton1.v.Pt()    < 20.) continue;

    FillHistograms(channel, HasZ);

    if (WLepton.v.DeltaR(ZLepton1.v) < 0.1) continue;
    if (WLepton.v.DeltaR(ZLepton2.v) < 0.1) continue;
    if (WLepton.v.Pt()               < 20.) continue;
    if (pfType1Met                   < 30.) continue;

    FillHistograms(channel, HasW);
  }


  printf("\n\n");


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  txt_mmm.close();

  Summary();

  root_output->cd();

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();
}


//------------------------------------------------------------------------------
// IsFiducialLepton
//------------------------------------------------------------------------------
bool AnalysisWZ::IsFiducialLepton(int k)
{
  float pt  = std_vector_lepton_pt ->at(k);
  float eta = std_vector_lepton_eta->at(k);
  float id  = std_vector_lepton_id ->at(k);

  bool is_fiducial_lepton = false;

  if (fabs(id) == 13)
    {
      is_fiducial_lepton = (pt > 10. && fabs(eta) < 2.4);
    }
  else if (fabs(id) == 11)
    {
      is_fiducial_lepton = (pt > 10. && fabs(eta) < 2.5);
    }

  return is_fiducial_lepton;
}


//------------------------------------------------------------------------------
// IsTightLepton
//
// Devin is using
// egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-medium
// https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=13
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
      float aeta = fabs(std_vector_electron_scEta->at(k));

      if (aeta <= 1.479)
	{                                                   //V1         //V2
	  if (fabs(std_vector_electron_deltaEtaIn->at(k)) < 0.007641 &&  //0.008925 &&
	      fabs(std_vector_electron_deltaPhiIn->at(k)) < 0.032643 &&  //0.035973 &&
	      std_vector_electron_sigmaIetaIeta->at(k)    < 0.010399 &&  //0.009996 &&
	      std_vector_electron_HoE->at(k)              < 0.060662 &&  //0.050537 &&
	      fabs(std_vector_electron_d0->at(k))         < 0.011811 &&  //0.012235 &&
	      fabs(std_vector_electron_dz->at(k))         < 0.070775 &&  //0.042020 &&
	      fabs(std_vector_electron_ooEooP->at(k))     < 0.153897 &&  //0.091942 &&
	      !std_vector_electron_passConversion->at(k))  // Includes expectedMissingInnerHits
	    {
	      is_tight_lepton = true;
	    }
	}
      else if (aeta > 1.479 && aeta < 2.5)
	{                                                   //V1         //V2
	  if (fabs(std_vector_electron_deltaEtaIn->at(k)) < 0.009285 &&  //0.007429 &&
	      fabs(std_vector_electron_deltaPhiIn->at(k)) < 0.042447 &&  //0.067879 &&
	      std_vector_electron_sigmaIetaIeta->at(k)    < 0.029524 &&  //0.030135 &&
	      std_vector_electron_HoE->at(k)              < 0.104263 &&  //0.086782 &&
	      fabs(std_vector_electron_d0->at(k))         < 0.051682 &&  //0.036719 &&
	      fabs(std_vector_electron_dz->at(k))         < 0.180720 &&  //0.138142 &&
	      fabs(std_vector_electron_ooEooP->at(k))     < 0.137468 &&  //0.100683 &&
	      !std_vector_electron_passConversion->at(k))  // Includes expectedMissingInnerHits
	    {
	      is_tight_lepton = true;
	    }
	}
    }

  return is_tight_lepton;
}


//------------------------------------------------------------------------------
// IsIsolatedLepton
//------------------------------------------------------------------------------
bool AnalysisWZ::IsIsolatedLepton(int k)
{
  float pt = std_vector_lepton_pt->at(k);
  float id = std_vector_lepton_id->at(k);

  float isolation = 999;

  bool is_isolated_lepton = false;

  if (fabs(id) == 13)
    {
      isolation =
	std_vector_lepton_chargedHadronIso->at(k) +
	max(float(0.0),
	    float(std_vector_lepton_photonIso->at(k) +
		  std_vector_lepton_neutralHadronIso->at(k) -
		  0.5*std_vector_lepton_sumPUPt->at(k)));

      is_isolated_lepton = (isolation/pt < 0.12);
    }
  else if (fabs(id) == 11)
    {
      float aeta = fabs(std_vector_lepton_eta->at(k));
      
      float effective_area = -999;
      
      if      (aeta >  2.2)               effective_area = 0.2680;
      else if (aeta >= 2.0 && aeta < 2.2) effective_area = 0.1565;
      else if (aeta >= 1.3 && aeta < 2.0) effective_area = 0.1077;
      else if (aeta >= 0.8 && aeta < 1.3) effective_area = 0.1734;
      else if (aeta <  0.8)               effective_area = 0.1830;

      isolation =
	std_vector_lepton_chargedHadronIso->at(k) +
	max(float(0.0),
	    float(std_vector_lepton_photonIso->at(k) +
		  std_vector_lepton_neutralHadronIso->at(k) -
		  jetRho*effective_area));

      is_isolated_lepton = (isolation/pt < 0.15);
    }
  
  return is_isolated_lepton;
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(int ichannel, int icut)
{
  hcounter[ichannel][icut]->Fill(1);
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary()
{
  txt_summary.open(directory + "/" + filename + ".txt");

  txt_summary << Form("\n %39s results with %7.1f pb\n\n", filename.Data(), luminosity);

  txt_summary << Form(" number of tight electrons: %.0f\n", hcounter_e->Integral());
  txt_summary << Form(" number of tight muons:     %.0f\n", hcounter_m->Integral());

  txt_summary << Form("\n %19s %13s %13s %13s %13s\n",
		     " ",
		     schannel[0].Data(),
		     schannel[1].Data(),
		     schannel[2].Data(),
		     schannel[3].Data());

  for (int i=0; i<ncut; i++) {
      
    txt_summary << Form(" %19s", scut[i].Data());

    for (int j=0; j<nchannel; j++) {

      float integral = hcounter[j][i]->Integral();

      txt_summary << Form(" %13.0f", integral);
    }
      
    txt_summary << "\n";
  }

  txt_summary << "\n";

  txt_summary.close();
}
