#define AnalysisWZ_cxx
#include "AnalysisWZ.h"


//==============================================================================
//
// Constants, enums and structs
//
//==============================================================================
const bool print_events = false;
const int  verbosity    = 1;

const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =  0.000511;  // [GeV]
const float MUON_MASS     =  0.106;     // [GeV]
const float TAU_MASS      =  1.777;     // [GeV]
const float Z_MASS        = 91.188;     // [GeV]


const int nchannel = 5;

enum {
  eee,
  eem,
  emm,
  mmm,
  all
};

const TString schannel[nchannel] = {
  "eee",
  "eem",
  "emm",
  "mmm",
  "all"
};


const int ncut = 5;

enum {
  Exactly3Leptons,
  HasZ,
  HasW,
  OneBJet,
  NoBJets
};

const TString scut[ncut] = {
  "Exactly3Leptons",
  "HasZ",
  "HasW",
  "OneBJet",
  "NoBJets"
};


enum {Loose, Tight, Gen};

struct Lepton
{
  int            index;
  int            type;  // Loose, Tight, Gen
  int            flavour;
  TLorentzVector v;
};


//==============================================================================
//
// Data members
//
//==============================================================================
bool                isMC;

std::vector<Lepton> AnalysisLeptons;
std::vector<Lepton> GenLeptons;
Lepton              WLepton;
Lepton              ZLepton1;
Lepton              ZLepton2;

float               luminosity;
float               event_weight;
float               mll;
int                 channel;
unsigned int        nelectron;
unsigned int        nlepton;
ofstream            txt_summary;
ofstream            txt_events_eee;
ofstream            txt_events_eem;
ofstream            txt_events_emm;
ofstream            txt_events_mmm;
TFile*              root_output;

TH1F*               h_gen_mZ;

TH1F*               h_counter_raw[nchannel][ncut];
TH1F*               h_counter_lum[nchannel][ncut];
TH1F*               h_invMass2Lep[nchannel][ncut];


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWZ::Loop(TString sample, float luminosity)
{
  isMC = true;

  if (sample.EqualTo("DoubleEG"))          isMC = false;
  if (sample.EqualTo("DoubleMuon"))        isMC = false;
  if (sample.EqualTo("DoubleMuonLowMass")) isMC = false;
  if (sample.EqualTo("MuonEG"))            isMC = false;
  if (sample.EqualTo("SingleElectron"))    isMC = false;
  if (sample.EqualTo("SingleMu"))          isMC = false;
  if (sample.EqualTo("SingleMuon"))        isMC = false;

  TH1::SetDefaultSumw2();

  gSystem->mkdir("rootfiles", kTRUE);
  gSystem->mkdir("txt",       kTRUE);

  root_output = new TFile("rootfiles/" + sample + ".root", "recreate");

  if (print_events)
    {
      txt_events_eee.open("txt/" + sample + "_eee.txt");
      txt_events_eem.open("txt/" + sample + "_eem.txt");
      txt_events_emm.open("txt/" + sample + "_emm.txt");
      txt_events_mmm.open("txt/" + sample + "_mmm.txt");
    }


  // Initialize histograms
  //----------------------------------------------------------------------------
  h_gen_mZ = new TH1F("h_gen_mZ", "", 400, 0, 200);

  for (int i=0; i<nchannel; i++) {
    for (int j=0; j<ncut; j++) {

      h_counter_raw[i][j] = new TH1F("h_counter_raw_" + schannel[i] + "_" + scut[j], "",   3, 0,   3);
      h_counter_lum[i][j] = new TH1F("h_counter_lum_" + schannel[i] + "_" + scut[j], "",   3, 0,   3);
      h_invMass2Lep[i][j] = new TH1F("h_invMass2Lep_" + schannel[i] + "_" + scut[j], "", 400, 0, 200);
    }
  }


  // Loop over events
  //----------------------------------------------------------------------------
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  if (verbosity > 0)
    {
      printf("\n");
      printf(" [%s]\n", sample.Data());
      printf(" Will run on %lld events\n", nentries);
      printf("\n");
    }
  
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    if (verbosity == 1 && jentry%10000 == 0) std::cout << "." << std::flush;

    event_weight = baseW * luminosity;

    ApplySignedWeight(sample);


    // Loop over GEN leptons
    //--------------------------------------------------------------------------
    if (isMC)
      {
	GenLeptons.clear();

	int vector_leptonGen_size = std_vector_leptonGen_pt->size();

	for (int i=0; i<vector_leptonGen_size; i++) {

	  float pt  = std_vector_leptonGen_pt ->at(i);
	  float eta = std_vector_leptonGen_eta->at(i);
	  float phi = std_vector_leptonGen_phi->at(i);

	  Lepton lep;

	  lep.index   = i;
	  lep.type    = Gen;
	  lep.flavour = std_vector_leptonGen_pid->at(i);
	  
	  float mass = -999;
	  
	  if      (abs(lep.flavour) == ELECTRON_FLAVOUR) mass = ELECTRON_MASS;
	  else if (abs(lep.flavour) == MUON_FLAVOUR)     mass = MUON_MASS;
	  else if (abs(lep.flavour) == TAU_FLAVOUR)	     mass = TAU_MASS;

	  if (fabs(std_vector_leptonGen_mpid->at(i)) != Z_FLAVOUR) continue;  // Require leptons from Z

	  TLorentzVector tlv;
	  
	  tlv.SetPtEtaPhiM(pt, eta, phi, mass);

	  lep.v = tlv;

	  GenLeptons.push_back(lep);
	}
	
	unsigned int ngenlepton = GenLeptons.size();


	// Make GEN Z mass
	//----------------------------------------------------------------------
	float mZ = 999;

	for (UInt_t i=0; i<ngenlepton; i++) {
	  
	  for (UInt_t j=i+1; j<ngenlepton; j++) {
	    
	    if (abs(GenLeptons[i].flavour) != abs(GenLeptons[j].flavour)) continue;
	    
	    if (GenLeptons[i].flavour * GenLeptons[j].flavour > 0) continue;
	    
	    float inv_mass = (GenLeptons[i].v + GenLeptons[j].v).M();
	    
	    if (fabs(inv_mass - Z_MASS) < fabs(mZ - Z_MASS)) {

	      mZ = inv_mass;
	    }
	  }
	}

	if (mZ < 999.) h_gen_mZ->Fill(mZ);
      }


    // Loop over leptons
    //--------------------------------------------------------------------------
    AnalysisLeptons.clear();

    int vector_lepton_size = std_vector_lepton_pt->size();

    for (int i=0; i<vector_lepton_size; i++) {

      if (!IsFiducialLepton(i)) continue;

      float pt  = std_vector_lepton_pt ->at(i);
      float eta = std_vector_lepton_eta->at(i);
      float phi = std_vector_lepton_phi->at(i);

      Lepton lep;
      
      lep.index   = i;
      lep.type    = Loose;
      lep.flavour = std_vector_lepton_flavour->at(i);
      
      float mass = -999;

      if      (abs(lep.flavour) == ELECTRON_FLAVOUR) mass = ELECTRON_MASS;
      else if (abs(lep.flavour) == MUON_FLAVOUR)     mass = MUON_MASS;

      if (!IsTightLepton(i))    continue;
      if (!IsIsolatedLepton(i)) continue;

      lep.type = Tight;

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(pt, eta, phi, mass);
      
      lep.v = tlv;

      AnalysisLeptons.push_back(lep);
    }

    nlepton = AnalysisLeptons.size();


    // For synchronization
    //--------------------------------------------------------------------------
    if (verbosity == 2)
      {
	if ((run == 1 && lumi == 1679 && event == 167809) ||
	    (run == 1 && lumi == 2101 && event ==  10040) ||
	    (run == 1 && lumi == 2174 && event ==  17347) ||
	    (run == 1 && lumi == 3106 && event == 110551) ||
	    (run == 1 && lumi == 3325 && event == 132450) ||
	    (run == 1 && lumi ==  404 && event ==  40309) ||
	    (run == 1 && lumi == 4984 && event ==  98324) ||
	    (run == 1 && lumi ==  742 && event ==  74108))
	  {
	    printf("%u:%u:%u -- %u leptons found ", run, lumi, event, nlepton);
	    
	    for (UInt_t i=0; i<nlepton; i++)
	      {
		TString lepton_flavour = (abs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR) ? "e" : "m";
		printf("%s", lepton_flavour.Data());
	      }
	    printf("\n");
	  }
      }
    
    
    // Require exactly three leptons
    //--------------------------------------------------------------------------
    if (nlepton != 3) continue;


    // Classify the channels
    //--------------------------------------------------------------------------
    nelectron = 0;

    for (int i=0; i<3; i++)
      {
	if (abs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR) nelectron++;
      }

    channel = -1;

    if      (nelectron == 3) channel = eee;
    else if (nelectron == 2) channel = eem;
    else if (nelectron == 1) channel = emm;
    else if (nelectron == 0) channel = mmm;


    // Make Z and W candidates
    //--------------------------------------------------------------------------
    mll = 999;

    for (UInt_t i=0; i<nlepton; i++) {

      for (UInt_t j=i+1; j<nlepton; j++) {
      
	if (abs(AnalysisLeptons[i].flavour) != abs(AnalysisLeptons[j].flavour)) continue;

	if (AnalysisLeptons[i].flavour * AnalysisLeptons[j].flavour > 0) continue;

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


    // Loop over jets
    //--------------------------------------------------------------------------
    int vector_jet_size = std_vector_jet_pt->size();

    int nbjet = 0;

    for (int i=0; i<vector_jet_size; i++) {

      float pt  = std_vector_jet_pt ->at(i);
      float eta = std_vector_jet_eta->at(i);
      float phi = std_vector_jet_phi->at(i);

      // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating
      // Loose  WP = 0.423
      // Medium WP = 0.814
      // Tight  WP = 0.941

      bool btag = (std_vector_jet_csvv2ivf->at(i) > 0.814);

      TLorentzVector jet;

      jet.SetPtEtaPhiM(pt, eta, phi, 0.0);
      
      if ((pt > 30.) &&
	  (fabs(eta) < 2.4) &&
	  (jet.DeltaR(ZLepton1.v) > 0.4) &&
	  (jet.DeltaR(ZLepton2.v) > 0.4) &&
	  (jet.DeltaR(WLepton.v)  > 0.4) &&
	  btag) nbjet++;  
    }


    // For synchronization
    //--------------------------------------------------------------------------
    if (print_events)
      {
	if (channel == eee) txt_events_eee << Form("%u:%u:%u\n", run, lumi, event);
	if (channel == eem) txt_events_eem << Form("%u:%u:%u\n", run, lumi, event);
	if (channel == emm) txt_events_emm << Form("%u:%u:%u\n", run, lumi, event);
	if (channel == mmm) txt_events_mmm << Form("%u:%u:%u\n", run, lumi, event);
      }


    // Fill histograms
    //--------------------------------------------------------------------------
    FillHistograms(channel, Exactly3Leptons);

    if (mll <  60.) continue;
    if (mll > 120.) continue;
    if (ZLepton1.v.Pt() < 20.) continue;

    FillHistograms(channel, HasZ);

    if (WLepton.v.DeltaR(ZLepton1.v) < 0.1) continue;
    if (WLepton.v.DeltaR(ZLepton2.v) < 0.1) continue;
    if (WLepton.v.Pt()               < 20.) continue;
    if (pfType1Met                   < 30.) continue;

    FillHistograms(channel, HasW);

    if (nbjet > 1) continue;

    FillHistograms(channel, OneBJet);

    if (nbjet > 0) continue;

    FillHistograms(channel, NoBJets);
  }


  if (verbosity >  0) printf("\n");
  if (verbosity == 1) printf("\n");


  // For synchronization
  //----------------------------------------------------------------------------
  if (print_events)
    {
      txt_events_eee.close();
      txt_events_eem.close();
      txt_events_emm.close();
      txt_events_mmm.close();
    }


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  txt_summary.open("txt/" + sample + ".txt");

  txt_summary << Form("\n%20s results with %.0f fb\n", sample.Data(), luminosity);

  Summary("11.0", "raw yields");
  Summary("11.2", "predicted yields");

  txt_summary.close();

  root_output->cd();

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();
}


//------------------------------------------------------------------------------
// IsFiducialLepton
//------------------------------------------------------------------------------
bool AnalysisWZ::IsFiducialLepton(int k)
{
  float pt      = std_vector_lepton_pt     ->at(k);
  float eta     = std_vector_lepton_eta    ->at(k);
  float flavour = std_vector_lepton_flavour->at(k);

  bool is_fiducial_lepton = false;

  if (fabs(flavour) == MUON_FLAVOUR)
    {
      is_fiducial_lepton = (pt > 10. && fabs(eta) < 2.4);
    }
  else if (fabs(flavour) == ELECTRON_FLAVOUR)
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
  float flavour = std_vector_lepton_flavour->at(k);

  bool is_tight_lepton = false;

  // Muon tight ID
  if (fabs(flavour) == MUON_FLAVOUR)
    {
      is_tight_lepton = std_vector_lepton_isTightMuon->at(k);
    }
  // Electron cut based medium ID
  else if (fabs(flavour) == ELECTRON_FLAVOUR)
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
  float pt      = std_vector_lepton_pt     ->at(k);
  float flavour = std_vector_lepton_flavour->at(k);

  float relative_isolation = -999;

  if (fabs(flavour) != MUON_FLAVOUR) return relative_isolation;

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
  float pt      = std_vector_lepton_pt     ->at(k);
  float flavour = std_vector_lepton_flavour->at(k);

  float relative_isolation = -999;

  if (fabs(flavour) != ELECTRON_FLAVOUR) return relative_isolation;

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
  float flavour = std_vector_lepton_flavour->at(k);

  bool is_isolated_lepton = false;

  if      (fabs(flavour) == ELECTRON_FLAVOUR) is_isolated_lepton = true;  //(ElectronIsolation(k) < 0.15);
  else if (fabs(flavour) == MUON_FLAVOUR)     is_isolated_lepton = (MuonIsolation(k)     < 0.12);
  
  return is_isolated_lepton;
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(int ichannel, int icut)
{
  h_counter_raw[ichannel][icut]->Fill(1);
  h_counter_lum[ichannel][icut]->Fill(1, event_weight);

  h_invMass2Lep[ichannel][icut]->Fill(mll, event_weight);

  if (ichannel != all) FillHistograms(all, icut);
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary(TString precision, TString title)
{
  txt_summary << Form("\n%20s", title.Data());

  for (int i=0; i<nchannel; i++) txt_summary << Form("%11s    %11s", schannel[i].Data(), " ");

  txt_summary << Form("\n---------------------\n");

  for (int i=0; i<ncut; i++) {
      
    txt_summary << Form("%20s", scut[i].Data());

    for (int j=0; j<nchannel; j++) {

      TH1F* h_counter = h_counter_raw[j][i];

      if (title.Contains("predicted")) h_counter = h_counter_lum[j][i];

      float yield = h_counter->Integral();
      float error = sqrt(h_counter->GetSumw2()->GetSum());

      TString yield_error = Form("%s%sf +- %s-%sf",
				 "%", precision.Data(),
				 "%", precision.Data());

      txt_summary << Form(yield_error.Data(), yield, error);
    }
      
    txt_summary << "\n";
  }
  
  txt_summary << "\n";
}


//------------------------------------------------------------------------------
// ApplySignedWeight
//------------------------------------------------------------------------------
void AnalysisWZ::ApplySignedWeight(TString sample)
{
  if (!isMC) return;

  float signed_weight = -999.;

  if (sample.EqualTo("WJetsToLNu"))          {signed_weight = 0.683938;}
  if (sample.EqualTo("DYJetsToLL_M-10to50")) {signed_weight = 0.727601;}
  if (sample.EqualTo("DYJetsToLL_M-50"))     {signed_weight = 0.66998;}
  if (sample.EqualTo("ZZTo2L2Q"))            {signed_weight = 0.631351;}
  if (sample.EqualTo("ST_t-channel"))        {signed_weight = 0.215648;}
  if (sample.EqualTo("TTJets"))              {signed_weight = 0.331658;}

  if (signed_weight < 0) return;

  if (GEN_weight_SM < 0) signed_weight *= -1.;

  event_weight *= signed_weight;

  return;
}
