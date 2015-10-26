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
  float          dxy;   // Tighter IP when requiring fabs(dxy) < 0.02
  float          dz;    // Tighter IP when requiring fabs(dz)  < 0.10
  TLorentzVector v;
};

struct Jet
{
  int            index;
  float          csvv2ivf;
  TLorentzVector v;
};


//==============================================================================
//
// Data members
//
//==============================================================================
std::vector<Jet>    AnalysisJets;
std::vector<Lepton> AnalysisLeptons;
std::vector<Lepton> GenLeptons;
Lepton              WLepton;
Lepton              ZLepton1;
Lepton              ZLepton2;

TString             _sample;
bool                _ismc;
float               _event_weight;
float               _ht;
float               _pt2l;
float               _m2l;
float               _m3l;
int                 _channel;
unsigned int        _nelectron;
unsigned int        _nlepton;
unsigned int        _ntight;
unsigned int        _njet;
unsigned int        _nbjet;
unsigned int        _jetbin;

ofstream            txt_summary;
ofstream            txt_events_eee;
ofstream            txt_events_eem;
ofstream            txt_events_emm;
ofstream            txt_events_mmm;
TFile*              root_output;

TH1D*               h_gen_mZ;
TH1D*               h_ntight;

TH1D*               h_counterRaw[nchannel][ncut][njetbin+1];
TH1D*               h_counterLum[nchannel][ncut][njetbin+1];
TH1D*               h_ht        [nchannel][ncut][njetbin+1];
TH1D*               h_m2l       [nchannel][ncut][njetbin+1];
TH1D*               h_m3l       [nchannel][ncut][njetbin+1];
TH1D*               h_njet      [nchannel][ncut][njetbin+1];
TH1D*               h_nbjet     [nchannel][ncut][njetbin+1];
TH1D*               h_nvtx      [nchannel][ncut][njetbin+1];
TH1D*               h_pfType1Met[nchannel][ncut][njetbin+1];


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
  h_ntight = new TH1D("h_ntight", "",  10, 0,  10);

  for (int j=0; j<ncut; j++) {
    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=0; i<nchannel; i++) {

	TString suffix = "_" + schannel[i];

	h_counterRaw[i][j][k] = new TH1D("h_counterRaw" + suffix, "",    3, 0,    3);
	h_counterLum[i][j][k] = new TH1D("h_counterLum" + suffix, "",    3, 0,    3);
	h_ht        [i][j][k] = new TH1D("h_ht"         + suffix, "",  400, 0,  400);
	h_m2l       [i][j][k] = new TH1D("h_m2l"        + suffix, "",  400, 0,  200);
	h_m3l       [i][j][k] = new TH1D("h_m3l"        + suffix, "", 4000, 0, 4000);
	h_njet      [i][j][k] = new TH1D("h_njet"       + suffix, "",    4, 0,    4);
	h_nbjet     [i][j][k] = new TH1D("h_nbjet"      + suffix, "",    4, 0,    4);
	h_nvtx      [i][j][k] = new TH1D("h_nvtx"       + suffix, "",   40, 0,   40);
	h_pfType1Met[i][j][k] = new TH1D("h_pfType1Met" + suffix, "",  200, 0,  200);
      }
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
    if (std_vector_leptonGen_pt != NULL)
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
	  lep.dxy     = 0;
	  lep.dz      = 0;
	  
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
	    
	    if (GenLeptons[i].flavour + GenLeptons[j].flavour != 0) continue;
	    
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
      lep.flavour = std_vector_lepton_flavour     ->at(i);
      lep.dxy     = std_vector_lepton_BestTrackdxy->at(i);
      lep.dz      = std_vector_lepton_BestTrackdz ->at(i);
      
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

    h_ntight->Fill(_ntight);


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
      
	    if (AnalysisLeptons[i].flavour + AnalysisLeptons[j].flavour != 0) continue;

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
    AnalysisJets.clear();

    int vector_jet_size = std_vector_jet_pt->size();

    for (int i=0; i<vector_jet_size; i++) {

      float pt  = std_vector_jet_pt ->at(i);
      float eta = std_vector_jet_eta->at(i);
      float phi = std_vector_jet_phi->at(i);

      if (pt < 30. || fabs(eta) < 2.4) continue;

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(pt, eta, phi, 0.0);

      bool is_lepton = false;

      for (int j=0; j<_nlepton; j++)
	{
	  if (tlv.DeltaR(AnalysisLeptons[j].v) < 0.4) is_lepton = true;
	}

      if (is_lepton) continue;

      Jet goodjet;

      goodjet.index    = i;
      goodjet.csvv2ivf = std_vector_jet_csvv2ivf->at(i);
      goodjet.v        = tlv;

      AnalysisJets.push_back(goodjet);
    }

    _njet = AnalysisJets.size();


    // Count the number of b-jets
    //--------------------------------------------------------------------------
    _nbjet = 0;

    for (int i=0; i<_njet; i++)
      {
	if (AnalysisJets[i].csvv2ivf > csvv2ivf_mediumWP) _nbjet++;
      }

	
    // Compute Ht
    //--------------------------------------------------------------------------
    _ht = pfType1Met;

    for (int i=0; i<_nlepton; i++) _ht += AnalysisLeptons[i].v.Pt();
    for (int i=0; i<_njet;    i++) _ht += AnalysisJets[i].v.Pt();


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
    _jetbin = (_njet < njetbin) ? _njet : njetbin - 1;

    if (_nlepton == 2)
      {
	if (AnalysisLeptons[0].v.Pt() < 20.) continue;
	if (AnalysisLeptons[1].v.Pt() < 20.) continue;
	if (AnalysisLeptons[0].flavour * AnalysisLeptons[1].flavour > 0) continue;

	LevelHistograms(WW00_Exactly2Leptons);

	if (pfType1Met < 20.) continue;
	if (_m2l       < 12.) continue;
	if (_nelectron != 1 && fabs(_m2l - Z_MASS) < 15.) continue;
	if (_nelectron != 1 && _pt2l < 45.) continue;
	if (_nelectron == 1 && _pt2l < 30.) continue;

	LevelHistograms(WW01_ZVeto);

	if (_nbjet > 0) continue;

	LevelHistograms(WW02_BVeto);
      }
    else
      {
	LevelHistograms(WZ00_Exactly3Leptons);
    
	if (_m2l <  60.) continue;
	if (_m2l > 120.) continue;
	if (ZLepton1.v.Pt() < 20.) continue;

	LevelHistograms(WZ01_HasZ);

	if (WLepton.v.DeltaR(ZLepton1.v) <  0.1) continue;
	if (WLepton.v.DeltaR(ZLepton2.v) <  0.1) continue;
	if (WLepton.v.Pt()               <  20.) continue;
	if (pfType1Met                   <  30.) continue;
	if (_m3l                         < 100.) continue;

	LevelHistograms(WZ02_HasW);
	
	if (_nbjet > 0) continue;
	
	LevelHistograms(WZ03_BVeto);
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

  txt_summary << "\n";
  txt_summary << Form("   filename: %s\n",        filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form("        era: %s\n",        era.Data());
  txt_summary << Form(" luminosity: %.2f pb-1\n", 1e3*luminosity);
  txt_summary << Form("   nentries: %lld\n",      nentries);
  txt_summary << "\n";

  Summary("WW", "11.0", "raw yields");
  Summary("WZ", "11.0", "raw yields");
  
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

  float etamax = (fabs(flavour) == MUON_FLAVOUR) ? 2.4 : 2.5;

  bool is_fiducial_lepton = (pt > 10. && fabs(eta) < etamax);

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
// LevelHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::LevelHistograms(int icut)
{
  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(int ichannel, int icut, int ijet)
{
  h_counterRaw[ichannel][icut][ijet]->Fill(1);
  h_counterLum[ichannel][icut][ijet]->Fill(1, _event_weight);

  h_ht        [ichannel][icut][ijet]->Fill(_ht,        _event_weight);
  h_m2l       [ichannel][icut][ijet]->Fill(_m2l,       _event_weight);
  h_m3l       [ichannel][icut][ijet]->Fill(_m3l,       _event_weight);
  h_njet      [ichannel][icut][ijet]->Fill(_njet,      _event_weight);
  h_nbjet     [ichannel][icut][ijet]->Fill(_nbjet,     _event_weight);
  h_nvtx      [ichannel][icut][ijet]->Fill(nvtx,       _event_weight);
  h_pfType1Met[ichannel][icut][ijet]->Fill(pfType1Met, _event_weight);

  if (_nlepton == 2 && ichannel != ll)  FillHistograms(ll,  icut, ijet);
  if (_nlepton == 3 && ichannel != lll) FillHistograms(lll, icut, ijet);
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary(TString analysis,
			 TString precision,
			 TString title)
{
  int firstChannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastChannel  = (analysis.EqualTo("WZ")) ? nchannel : eee;
  int firstCut     = (analysis.EqualTo("WZ")) ? WZ00_Exactly3Leptons : WW00_Exactly2Leptons;
  int lastCut      = (analysis.EqualTo("WZ")) ? ncut : WZ00_Exactly3Leptons;

  txt_summary << Form("\n%30s", title.Data());

  for (int i=firstChannel; i<lastChannel; i++)
    txt_summary << Form("%11s    %11s", schannel[i].Data(), " ");

  txt_summary << Form("\n-------------------------------\n");

  for (int i=firstCut; i<lastCut; i++) {
      
    txt_summary << Form("%30s", scut[i].Data());

    for (int j=firstChannel; j<lastChannel; j++) {

      TH1D* h_counter = h_counterRaw[j][i][njetbin];

      if (title.Contains("predicted")) h_counter = h_counterLum[j][i][njetbin];

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

  if (title.Contains("raw") && _ismc) Summary(analysis, "11.2", "predicted yields");
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

  if (_sample.EqualTo("DoubleEG"))       _ismc = false;
  if (_sample.EqualTo("DoubleMuon"))     _ismc = false;
  if (_sample.EqualTo("MuonEG"))         _ismc = false;
  if (_sample.EqualTo("SingleElectron")) _ismc = false;
  if (_sample.EqualTo("SingleMuon"))     _ismc = false;

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
