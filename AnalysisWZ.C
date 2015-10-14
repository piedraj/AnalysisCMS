#define AnalysisWZ_cxx
#include "AnalysisWZ.h"
#include "Constants.h"


// verbosity = 0 (silent)  doesn't print anything
// verbosity > 0 (default) prints the input values and "." every <interval> events
// verbosity > 1 (debug)
const int verbosity = 1;
const int interval  = 10000;


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
std::vector<Lepton> AnalysisLeptons;
std::vector<Lepton> GenLeptons;
Lepton              WLepton;
Lepton              ZLepton1;
Lepton              ZLepton2;

TString             _sample;
bool                _ismc;
float               _event_weight;
float               _pt2l;
float               _m2l;
float               _m3l;
int                 _channel;
unsigned int        _nelectron;
unsigned int        _nlepton;
unsigned int        _ntight;
unsigned int        _njet;
unsigned int        _nbjet;

ofstream            txt_summary;
ofstream            txt_events_eee;
ofstream            txt_events_eem;
ofstream            txt_events_emm;
ofstream            txt_events_mmm;
TFile*              root_output;

TH1D*               h_gen_mZ;

TH1D*               h_counter_raw[nchannel][ncut];
TH1D*               h_counter_lum[nchannel][ncut];
TH1D*               h_m2l        [nchannel][ncut];
TH1D*               h_m3l        [nchannel][ncut];
TH1D*               h_njet       [nchannel][ncut];
TH1D*               h_nbjet      [nchannel][ncut];
TH1D*               h_nvtx       [nchannel][ncut];
TH1D*               h_pfType1Met [nchannel][ncut];


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWZ::Loop(TString filename,
		      TString era,
		      float   luminosity)
{
  GetSampleName(filename);

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  
  if (verbosity > 0)
    {
      printf("\n");
      printf("   filename: %s\n",        filename.Data());
      printf("     sample: %s\n",        _sample.Data());
      printf("        era: %s\n",        era.Data());
      printf(" luminosity: %.2f pb-1\n", 1e3*luminosity);
      printf("   nentries: %lld\n",      nentries);
      printf("\n");
    }


  // Additional settings
  //----------------------------------------------------------------------------
  gSystem->mkdir(Form("rootfiles/%s", era.Data()), kTRUE);
  gSystem->mkdir(Form("txt/%s",       era.Data()), kTRUE);

  root_output = new TFile("rootfiles/" + era + "/" + _sample + ".root", "recreate");

  if (_sample.EqualTo("WZTo3LNu") && era.EqualTo("25ns"))
    {
      txt_events_eee.open("txt/" + era + "/" + _sample + "_eee.txt");
      txt_events_eem.open("txt/" + era + "/" + _sample + "_eem.txt");
      txt_events_emm.open("txt/" + era + "/" + _sample + "_emm.txt");
      txt_events_mmm.open("txt/" + era + "/" + _sample + "_mmm.txt");
    }


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  h_gen_mZ = new TH1D("h_gen_mZ", "", 400, 0, 200);

  for (int i=0; i<nchannel; i++) {
    for (int j=0; j<ncut; j++) {

      h_counter_raw[i][j] = new TH1D("h_counter_raw_" + schannel[i] + "_" + scut[j], "",    3, 0,    3);
      h_counter_lum[i][j] = new TH1D("h_counter_lum_" + schannel[i] + "_" + scut[j], "",    3, 0,    3);
      h_m2l        [i][j] = new TH1D("h_m2l_"         + schannel[i] + "_" + scut[j], "",  400, 0,  200);
      h_m3l        [i][j] = new TH1D("h_m3l_"         + schannel[i] + "_" + scut[j], "", 4000, 0, 4000);
      h_njet       [i][j] = new TH1D("h_njet_"        + schannel[i] + "_" + scut[j], "",   10, 0,   10);
      h_nbjet      [i][j] = new TH1D("h_nbjet_"       + schannel[i] + "_" + scut[j], "",   10, 0,   10);
      h_nvtx       [i][j] = new TH1D("h_nvtx_"        + schannel[i] + "_" + scut[j], "",   50, 0,   50);
      h_pfType1Met [i][j] = new TH1D("h_pfType1Met_"  + schannel[i] + "_" + scut[j], "",  200, 0,  200);
    }
  }

  
  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    if (!trigger) continue;

    if (verbosity > 0 && jentry%interval == 0) std::cout << "." << std::flush;

    ApplyWeights(_sample, era, luminosity);


    // Loop over GEN leptons
    //--------------------------------------------------------------------------
    if (_ismc)
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
	  else if (abs(lep.flavour) == TAU_FLAVOUR)      mass = TAU_MASS;

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

      if (IsTightLepton(i) && IsIsolatedLepton(i)) lep.type = Tight;

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(pt, eta, phi, mass);
      
      lep.v = tlv;

      AnalysisLeptons.push_back(lep);
    }

    _nlepton = AnalysisLeptons.size();


    // Count the number of tight leptons
    //--------------------------------------------------------------------------
    _ntight = 0;

    for (int i=0; i<_nlepton; i++)
      {
	if (AnalysisLeptons[i].type == Tight) _ntight++;
      }


    // Count the number of electrons
    //--------------------------------------------------------------------------
    _nelectron = 0;

    for (int i=0; i<_nlepton; i++)
      {
	if (abs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR) _nelectron++;
      }


    // Define the channel
    //--------------------------------------------------------------------------
    _channel = -1;

    if (_nlepton == 2 && _ntight == 2)
      {
	if      (_nelectron == 2) _channel = ee;
	else if (_nelectron == 1) _channel = em;
	else if (_nelectron == 0) _channel = mm;
      }
    else if (_nlepton == 3 && _ntight == 3)
      {
	if      (_nelectron == 3) _channel = eee;
	else if (_nelectron == 2) _channel = eem;
	else if (_nelectron == 1) _channel = emm;
	else if (_nelectron == 0) _channel = mmm;
      }


    // From here we have at least two tight leptons
    //--------------------------------------------------------------------------
    if (_channel < 0) continue;


    // Make Z and W candidates
    //--------------------------------------------------------------------------
    if (_channel < eee)
      {
	_pt2l = (AnalysisLeptons[0].v + AnalysisLeptons[1].v).Pt();
	_m2l  = (AnalysisLeptons[0].v + AnalysisLeptons[1].v).M();
	_m3l  = 999;
      }
    else
      {
	_m2l = 999;

	for (UInt_t i=0; i<_nlepton; i++) {

	  for (UInt_t j=i+1; j<_nlepton; j++) {
      
	    if (abs(AnalysisLeptons[i].flavour) != abs(AnalysisLeptons[j].flavour)) continue;

	    if (AnalysisLeptons[i].flavour * AnalysisLeptons[j].flavour > 0) continue;

	    float inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	    if (fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

	      _m2l = inv_mass;

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

	_pt2l = (ZLepton1.v + ZLepton2.v).Pt();
	_m3l  = (ZLepton1.v + ZLepton2.v + WLepton.v).M();
      }


    // Loop over jets
    //--------------------------------------------------------------------------
    int vector_jet_size = std_vector_jet_pt->size();

    _njet  = 0;
    _nbjet = 0;

    for (int i=0; i<vector_jet_size; i++) {

      float pt  = std_vector_jet_pt ->at(i);
      float eta = std_vector_jet_eta->at(i);
      float phi = std_vector_jet_phi->at(i);

      if (pt < 30. || fabs(eta) < 2.4) continue;

      TLorentzVector jet;

      jet.SetPtEtaPhiM(pt, eta, phi, 0.0);

      bool is_lepton = false;

      for (int j=0; j<_nlepton; j++)
	{
	  if (jet.DeltaR(AnalysisLeptons[j].v) < 0.4) is_lepton = true;
	}

      if (is_lepton) continue;

      _njet++;

      // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating
      // Loose  WP = 0.423
      // Medium WP = 0.814
      // Tight  WP = 0.941

      if (std_vector_jet_csvv2ivf->at(i) > 0.814) _nbjet++;
    }


    // For WZ synchronization
    //--------------------------------------------------------------------------
    if (_nlepton == 3 && _sample.EqualTo("WZTo3LNu") && era.EqualTo("25ns"))
      {
	if (_channel == eee) txt_events_eee << Form("%u:%u:%u\n", run, lumi, event);
	if (_channel == eem) txt_events_eem << Form("%u:%u:%u\n", run, lumi, event);
	if (_channel == emm) txt_events_emm << Form("%u:%u:%u\n", run, lumi, event);
	if (_channel == mmm) txt_events_mmm << Form("%u:%u:%u\n", run, lumi, event);
      }


    // Fill histograms
    //--------------------------------------------------------------------------
    if (_nlepton == 2)
      {
	if (AnalysisLeptons[0].v.Pt() < 20.) continue;
	if (AnalysisLeptons[1].v.Pt() < 10.) continue;
	if (AnalysisLeptons[0].flavour * AnalysisLeptons[1].flavour > 0) continue;

	FillHistograms(_channel, nlep2_cut0_Exactly2Leptons);

	if (pfType1Met < 20.) continue;
	if (_m2l       < 12.) continue;
	if (_nelectron != 1 && fabs(_m2l - Z_MASS) < 15.) continue;
	if (_nelectron != 1 && _pt2l < 45.) continue;
	if (_nelectron == 1 && _pt2l < 30.) continue;

	FillHistograms(_channel, nlep2_cut1_ZVeto);
      }
    else
      {
	FillHistograms(_channel, nlep3_cut0_Exactly3Leptons);
    
	if (_m2l <  60.) continue;
	if (_m2l > 120.) continue;
	if (ZLepton1.v.Pt() < 20.) continue;

	FillHistograms(_channel, nlep3_cut1_HasZ);

	if (WLepton.v.DeltaR(ZLepton1.v) <  0.1) continue;
	if (WLepton.v.DeltaR(ZLepton2.v) <  0.1) continue;
	if (WLepton.v.Pt()               <  20.) continue;
	if (pfType1Met                   <  30.) continue;
	if (_m3l                         < 100.) continue;

	FillHistograms(_channel, nlep3_cut2_HasW);
	
	if (_nbjet > 1) continue;
	
	FillHistograms(_channel, nlep3_cut3_OneBJet);
      }
  }
   
 
  if (verbosity > 0) printf("\n");


  // For synchronization
  //----------------------------------------------------------------------------
  if (_nlepton == 3 && _sample.EqualTo("WZTo3LNu") && era.EqualTo("25ns"))
    {
      txt_events_eee.close();
      txt_events_eem.close();
      txt_events_emm.close();
      txt_events_mmm.close();
    }


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  txt_summary.open("txt/" + era + "/" + _sample + ".txt");

  txt_summary << Form("\n%20s results with %.2f pb-1\n",
		      _sample.Data(),
		      1e3*luminosity);

  Summary("11.0", "raw yields");
  
  if (_ismc) Summary("11.2", "predicted yields");

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
// egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-medium
// egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V2-standalone-medium
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
  h_counter_raw[ichannel][icut]->Fill(1.);
  h_counter_lum[ichannel][icut]->Fill(1., _event_weight);

  h_m2l       [ichannel][icut]->Fill(_m2l,       _event_weight);
  h_m3l       [ichannel][icut]->Fill(_m3l,       _event_weight);
  h_njet      [ichannel][icut]->Fill(_njet,      _event_weight);
  h_nbjet     [ichannel][icut]->Fill(_nbjet,     _event_weight);
  h_nvtx      [ichannel][icut]->Fill(nvtx,       _event_weight);
  h_pfType1Met[ichannel][icut]->Fill(pfType1Met, _event_weight);

  if (_nlepton == 2 && ichannel != ll)  FillHistograms(ll,  icut);
  if (_nlepton == 3 && ichannel != lll) FillHistograms(lll, icut);
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary(TString precision, TString title)
{
  txt_summary << Form("\n%30s", title.Data());

  for (int i=eee; i<nchannel; i++) txt_summary << Form("%11s    %11s", schannel[i].Data(), " ");

  txt_summary << Form("\n-------------------------------\n");

  for (int i=0; i<ncut; i++) {
      
    txt_summary << Form("%30s", scut[i].Data());

    for (int j=eee; j<nchannel; j++) {

      TH1D* h_counter = h_counter_raw[j][i];

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
// GetSampleName
//------------------------------------------------------------------------------
void AnalysisWZ::GetSampleName(TString filename)
{
  TString tok;

  Ssiz_t from = 0;

  while (filename.Tokenize(tok, from, "latino_")) {

    if (tok.Contains(".root")) {

      _sample = tok.ReplaceAll(".root", "");
    }
  }

  _ismc = true;

  if (_sample.EqualTo("DoubleEG"))          _ismc = false;
  if (_sample.EqualTo("DoubleMuon"))        _ismc = false;
  if (_sample.EqualTo("DoubleMuonLowMass")) _ismc = false;
  if (_sample.EqualTo("MuonEG"))            _ismc = false;
  if (_sample.EqualTo("SingleElectron"))    _ismc = false;
  if (_sample.EqualTo("SingleMu"))          _ismc = false;
  if (_sample.EqualTo("SingleMuon"))        _ismc = false;

  return;
}


//------------------------------------------------------------------------------
// ApplyWeights
//------------------------------------------------------------------------------
void AnalysisWZ::ApplyWeights(TString sample,
			      TString era,
			      float   luminosity)
{
  _event_weight = (_ismc) ? puW * baseW * luminosity : 1.;

  if (!_ismc) return;

  float signed_weight = 999.;

  if (era.EqualTo("50ns"))
    {
      if (_sample.EqualTo("WWTo2L2Nu_NLL")) _event_weight *= nllW;

      if (_sample.EqualTo("WJetsToLNu"))      signed_weight /= 0.683927;
      if (_sample.EqualTo("DYJetsToLL_M-50")) signed_weight /= 0.670032;
      if (_sample.EqualTo("ST_t-channel"))    signed_weight /= 0.215131;
      if (_sample.EqualTo("TTJets"))          signed_weight /= 0.331907;
    }
  else if (era.EqualTo("25ns"))
    {
      if (_sample.EqualTo("WJetsToLNu"))          signed_weight /= 0.683938;
      if (_sample.EqualTo("DYJetsToLL_M-10to50")) signed_weight /= 0.727601;
      if (_sample.EqualTo("DYJetsToLL_M-50"))     signed_weight /= 0.66998;
      if (_sample.EqualTo("ZZTo2L2Q"))            signed_weight /= 0.631351;
      if (_sample.EqualTo("ST_t-channel"))        signed_weight /= 0.215648;
      if (_sample.EqualTo("TTJets"))              signed_weight /= 0.331658;
    }

  if (signed_weight > 1.) return;

  if (GEN_weight_SM < 0) signed_weight *= -1.;

  _event_weight *= signed_weight;

  return;
}
