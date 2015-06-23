#define AnalysisWZ_cxx
#include "AnalysisWZ.h"


//==============================================================================
//
// Constants, enums and structs
//
//==============================================================================
const int verbosity = 2;


const float E_MASS =  0.000511;  // [GeV]
const float M_MASS =  0.106;     // [GeV]
const float Z_MASS = 91.1876;    // [GeV]


const int nchannel = 4;

enum {
  eee,
  eem,
  emm,
  mmm
};

const TString schannel[nchannel] = {
  "eee",
  "eem",
  "emm",
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
ofstream            txt_events_eee;
ofstream            txt_events_eem;
ofstream            txt_events_emm;
ofstream            txt_events_mmm;
TFile*              root_output;
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
  filename   = "WZ13TeV";

  gSystem->mkdir("rootfiles", kTRUE);
  gSystem->mkdir("txt",       kTRUE);

  root_output = new TFile("rootfiles/" + filename + ".root", "recreate");

  txt_events_eee.open("txt/" + filename + "_eee.txt");
  txt_events_eem.open("txt/" + filename + "_eem.txt");
  txt_events_emm.open("txt/" + filename + "_emm.txt");
  txt_events_mmm.open("txt/" + filename + "_mmm.txt");


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

  if (verbosity > 0) printf("\n Will run on %lld events\n\n", nentries);

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    if (verbosity == 1 && jentry%10000 == 0) std::cout << "." << std::flush;


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
    if (verbosity == 2)
      {
	if ((run == 1 && lumi == 2101 && event ==  10040) ||
	    (run == 1 && lumi ==  404 && event ==  40309) ||
	    (run == 1 && lumi == 4984 && event ==  98324))
	  {
	    printf("%u:%u:%u -- ", run, lumi, event);
	    
	    for (UInt_t i=0; i<AnalysisLeptons.size(); i++)
	      {
		TString lepton_flavor = (AnalysisLeptons[i].flavor == Electron) ? "e" : "m";
		printf("%s", lepton_flavor.Data());
	      }
	    printf("\n");
	  }
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

    if      (nelectron == 3) channel = eee;
    else if (nelectron == 2) channel = eem;
    else if (nelectron == 1) channel = emm;
    else if (nelectron == 0) channel = mmm;


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
    if (channel == eee) txt_events_eee << Form("%u:%u:%u\n", run, lumi, event);
    if (channel == eem) txt_events_eem << Form("%u:%u:%u\n", run, lumi, event);
    if (channel == emm) txt_events_emm << Form("%u:%u:%u\n", run, lumi, event);
    if (channel == mmm) txt_events_mmm << Form("%u:%u:%u\n", run, lumi, event);


    if (fabs(mll - Z_MASS) > 20.) continue;
    if (ZLepton1.v.Pt()    < 20.) continue;

    FillHistograms(channel, HasZ);

    if (WLepton.v.DeltaR(ZLepton1.v) < 0.1) continue;
    if (WLepton.v.DeltaR(ZLepton2.v) < 0.1) continue;
    if (WLepton.v.Pt()               < 20.) continue;
    if (pfType1Met                   < 30.) continue;

    FillHistograms(channel, HasW);
  }


  if (verbosity >  0) printf("\n");
  if (verbosity == 1) printf("\n");


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  txt_events_eee.close();
  txt_events_eem.close();
  txt_events_emm.close();
  txt_events_mmm.close();

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
// https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
// egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium
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
      is_tight_lepton = std_vector_lepton_eleIdMedium->at(k);
    }

  return is_tight_lepton;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
float AnalysisWZ::MuonIsolation(int k)
{
  float pt = std_vector_lepton_pt->at(k);
  float id = std_vector_lepton_id->at(k);

  float relative_isolation = -999;

  if (fabs(id) != 13) return relative_isolation;

  relative_isolation =
    std_vector_lepton_chargedHadronIso->at(k) +
    max(float(0.0),
	float(std_vector_lepton_photonIso->at(k) +
	      std_vector_lepton_neutralHadronIso->at(k) -
	      0.5*std_vector_lepton_sumPUPt->at(k)));

  relative_isolation /= pt;
  
  return relative_isolation;
}


//------------------------------------------------------------------------------
// ElectronIsolation
//------------------------------------------------------------------------------
float AnalysisWZ::ElectronIsolation(int k)
{
  float pt = std_vector_lepton_pt->at(k);
  float id = std_vector_lepton_id->at(k);

  float relative_isolation = -999;

  if (fabs(id) != 11) return relative_isolation;

  relative_isolation =
    std_vector_lepton_chargedHadronIso->at(k) +
    max(float(0.0),
	float(std_vector_lepton_photonIso->at(k) +
	      std_vector_lepton_neutralHadronIso->at(k) -
	      jetRho*std_vector_electron_effectiveArea->at(k)));
  
  relative_isolation /= pt;
  
  return relative_isolation;
}


//------------------------------------------------------------------------------
// IsIsolatedLepton
//------------------------------------------------------------------------------
bool AnalysisWZ::IsIsolatedLepton(int k)
{
  float id = std_vector_lepton_id->at(k);

  bool is_isolated_lepton = false;

  if      (fabs(id) == 11) is_isolated_lepton = true;  //(ElectronIsolation(k) < 0.15);
  else if (fabs(id) == 13) is_isolated_lepton = (MuonIsolation(k)     < 0.12);
  
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
  txt_summary.open("txt/" + filename + ".txt");

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
