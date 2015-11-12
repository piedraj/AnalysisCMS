#define AnalysisCMS_cxx
#include "../include/AnalysisCMS.h"


//------------------------------------------------------------------------------
// AnalysisCMS
//------------------------------------------------------------------------------
AnalysisCMS::AnalysisCMS(TTree* tree) : AnalysisBase(tree)
{
  _analysis_top  = false;
  _analysis_ttdm = false;
  _analysis_ww   = false;
  _analysis_wz   = false;
  _ismc          = true;
}


//------------------------------------------------------------------------------
// AddAnalysis
//------------------------------------------------------------------------------
void AnalysisCMS::AddAnalysis(TString analysis)
{
  if (analysis.EqualTo("Top"))  _analysis_top  = true;
  if (analysis.EqualTo("TTDM")) _analysis_ttdm = true;
  if (analysis.EqualTo("WW"))   _analysis_ww   = true;
  if (analysis.EqualTo("WZ"))   _analysis_wz   = true;
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisCMS::Loop(TString filename,
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


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {

    if (!_analysis_top  && scut[j].Contains("Top/"))  continue;
    if (!_analysis_ttdm && scut[j].Contains("TTDM/")) continue;
    if (!_analysis_ww   && scut[j].Contains("WW/"))   continue;
    if (!_analysis_wz   && scut[j].Contains("WZ/"))   continue;

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=0; i<nchannel; i++) {

	TString suffix = "_" + schannel[i];


	// Common histograms
	//----------------------------------------------------------------------
	h_counterRaw[i][j][k] = new TH1D("h_counterRaw" + suffix, "",    3, 0,    3);
	h_counterLum[i][j][k] = new TH1D("h_counterLum" + suffix, "",    3, 0,    3);
	h_ht        [i][j][k] = new TH1D("h_ht"         + suffix, "",  400, 0,  400);
	h_m2l       [i][j][k] = new TH1D("h_m2l"        + suffix, "",  400, 0,  200);
	h_njet      [i][j][k] = new TH1D("h_njet"       + suffix, "",    4, 0,    4);
	h_nbjet     [i][j][k] = new TH1D("h_nbjet"      + suffix, "",    4, 0,    4);
	h_nvtx      [i][j][k] = new TH1D("h_nvtx"       + suffix, "",   40, 0,   40);
	h_pfType1Met[i][j][k] = new TH1D("h_pfType1Met" + suffix, "", 2000, 0, 2000);


	// WZ histograms
	//----------------------------------------------------------------------
	h_m3l         [i][j][k] = new TH1D("h_m3l"          + suffix, "", 4000,  0, 4000);
	h_zl1pt       [i][j][k] = new TH1D("h_zl1pt"        + suffix, "",  200,  0,  200);
	h_zl2pt       [i][j][k] = new TH1D("h_zl2pt"        + suffix, "",  200,  0,  200);
	h_wlpt        [i][j][k] = new TH1D("h_wlpt"         + suffix, "",  200,  0,  200);
	h_zl1eta      [i][j][k] = new TH1D("h_zl1eta"       + suffix, "",  120, -3,    3);
	h_zl2eta      [i][j][k] = new TH1D("h_zl2eta"       + suffix, "",  120, -3,    3);
	h_wleta       [i][j][k] = new TH1D("h_wleta"        + suffix, "",  120, -3,    3);
	h_wlzl1_deltar[i][j][k] = new TH1D("h_wlzl1_deltar" + suffix, "",  100,  0,    5);
	h_wlzl2_deltar[i][j][k] = new TH1D("h_wlzl2_deltar" + suffix, "",  100,  0,    5);
	h_wlzl_deltar [i][j][k] = new TH1D("h_wlzl_deltar"  + suffix, "",  100,  0,    5);
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

    GetLeptons();

    GetJets();


    // Compute Ht
    //--------------------------------------------------------------------------
    _ht = pfType1Met;

    for (int i=0; i<_nlepton; i++) _ht += AnalysisLeptons[i].v.Pt();
    for (int i=0; i<_njet;    i++) _ht += AnalysisJets[i].v.Pt();


    // Fill histograms
    //--------------------------------------------------------------------------
    if (_analysis_top)  AnalysisTop();
    if (_analysis_ttdm) AnalysisTTDM();
    if (_analysis_ww)   AnalysisWW();
    if (_analysis_wz)   AnalysisWZ();
  }


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  if (verbosity > 0) printf("\n");

  txt_summary.open("txt/" + era + "/" + _sample + ".txt");

  txt_summary << "\n";
  txt_summary << Form("   filename: %s\n",        filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form("        era: %s\n",        era.Data());
  txt_summary << Form(" luminosity: %.2f pb-1\n", 1e3*luminosity);
  txt_summary << Form("   nentries: %lld\n",      nentries);
  txt_summary << "\n";

  if (_analysis_top)  Summary("Top",  "11.0", "raw yields");
  if (_analysis_ttdm) Summary("TTDM", "11.0", "raw yields");
  if (_analysis_ww)   Summary("WW",   "11.0", "raw yields");
  if (_analysis_wz)   Summary("WZ",   "11.0", "raw yields");
  
  txt_summary.close();

  root_output->cd();

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();
}


//------------------------------------------------------------------------------
// IsFiducialLepton
//------------------------------------------------------------------------------
bool AnalysisCMS::IsFiducialLepton(int k)
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
//------------------------------------------------------------------------------
bool AnalysisCMS::IsTightLepton(int k)
{
  float flavour = std_vector_lepton_flavour->at(k);

  bool is_tight_lepton = false;

  if (fabs(flavour) == MUON_FLAVOUR)
    {
      is_tight_lepton = std_vector_lepton_isTightMuon->at(k);
    }
  else if (fabs(flavour) == ELECTRON_FLAVOUR)
    {
      is_tight_lepton = std_vector_lepton_eleIdMedium->at(k);
    }

  return is_tight_lepton;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
float AnalysisCMS::MuonIsolation(int k)
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
float AnalysisCMS::ElectronIsolation(int k)
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
bool AnalysisCMS::IsIsolatedLepton(int k)
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
void AnalysisCMS::LevelHistograms(int icut, bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisCMS::FillHistograms(int ichannel, int icut, int ijet)
{
  // Common histograms
  //----------------------------------------------------------------------------
  h_counterRaw[ichannel][icut][ijet]->Fill(1);
  h_counterLum[ichannel][icut][ijet]->Fill(1,          _event_weight);
  h_ht        [ichannel][icut][ijet]->Fill(_ht,        _event_weight);
  h_m2l       [ichannel][icut][ijet]->Fill(_m2l,       _event_weight);
  h_njet      [ichannel][icut][ijet]->Fill(_njet,      _event_weight);
  h_nbjet     [ichannel][icut][ijet]->Fill(_nbjet,     _event_weight);
  h_nvtx      [ichannel][icut][ijet]->Fill(nvtx,       _event_weight);
  h_pfType1Met[ichannel][icut][ijet]->Fill(pfType1Met, _event_weight);


  // WZ histograms
  //----------------------------------------------------------------------------
  if (ichannel > ll)
    {
      float wlzl1dr = WLepton.v.DeltaR(ZLepton1.v);
      float wlzl2dr = WLepton.v.DeltaR(ZLepton2.v);
      float wlzldr  = min(wlzl1dr, wlzl2dr);

      h_m3l         [ichannel][icut][ijet]->Fill(_m3l,             _event_weight);
      h_zl1pt       [ichannel][icut][ijet]->Fill(ZLepton1.v.Pt(),  _event_weight);
      h_zl2pt       [ichannel][icut][ijet]->Fill(ZLepton2.v.Pt(),  _event_weight);
      h_wlpt        [ichannel][icut][ijet]->Fill(WLepton.v.Pt(),   _event_weight);
      h_zl1eta      [ichannel][icut][ijet]->Fill(ZLepton1.v.Eta(), _event_weight);
      h_zl2eta      [ichannel][icut][ijet]->Fill(ZLepton2.v.Eta(), _event_weight);
      h_wleta       [ichannel][icut][ijet]->Fill(WLepton.v.Eta(),  _event_weight);
      h_wlzl1_deltar[ichannel][icut][ijet]->Fill(wlzl1dr,          _event_weight);
      h_wlzl2_deltar[ichannel][icut][ijet]->Fill(wlzl2dr,          _event_weight);
      h_wlzl_deltar [ichannel][icut][ijet]->Fill(wlzldr,           _event_weight);
    }


  if (_nlepton == 2 && ichannel != ll)  FillHistograms(ll,  icut, ijet);
  if (_nlepton == 3 && ichannel != lll) FillHistograms(lll, icut, ijet);
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisCMS::Summary(TString analysis,
			  TString precision,
			  TString title)
{
  int firstChannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastChannel  = (analysis.EqualTo("WZ")) ? nchannel : eee;

  txt_summary << Form("\n%30s", title.Data());

  for (int i=firstChannel; i<lastChannel; i++)
    txt_summary << Form("%11s    %11s", schannel[i].Data(), " ");

  txt_summary << Form("\n-------------------------------\n");

  for (int i=0; i<ncut; i++) {

    if (!scut[i].Contains(analysis)) continue;
      
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
void AnalysisCMS::GetSampleName(TString filename)
{
  TString tok;

  Ssiz_t from = 0;

  while (filename.Tokenize(tok, from, "latino_")) {

    if (tok.Contains(".root")) {

      _sample = tok.ReplaceAll(".root", "");
    }
  }

  if (_sample.Contains("DoubleEG"))       _ismc = false;
  if (_sample.Contains("DoubleMuon"))     _ismc = false;
  if (_sample.Contains("MuonEG"))         _ismc = false;
  if (_sample.Contains("SingleElectron")) _ismc = false;
  if (_sample.Contains("SingleMuon"))     _ismc = false;

  return;
}


//------------------------------------------------------------------------------
// ApplyWeights
//------------------------------------------------------------------------------
void AnalysisCMS::ApplyWeights(TString sample,
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


//------------------------------------------------------------------------------
// GetLeptons
//------------------------------------------------------------------------------
void AnalysisCMS::GetLeptons()
{
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

    if (abs(lep.flavour) == ELECTRON_FLAVOUR)
      {
	mass    = ELECTRON_MASS;
	lep.iso = ElectronIsolation(i);
      }
    else if (abs(lep.flavour) == MUON_FLAVOUR)
      {
	mass    = MUON_MASS;
	lep.iso = MuonIsolation(i);
      }

    if (IsTightLepton(i) && IsIsolatedLepton(i)) lep.type = Tight;

    TLorentzVector tlv;
    
    tlv.SetPtEtaPhiM(pt, eta, phi, mass);
    
    lep.v = tlv;

    AnalysisLeptons.push_back(lep);
  }

  _nlepton = AnalysisLeptons.size();


  // Count the number of tight leptons
  //----------------------------------------------------------------------------
  _ntight = 0;

  for (int i=0; i<_nlepton; i++)
    {
      if (AnalysisLeptons[i].type == Tight) _ntight++;
    }


  // Count the number of electrons
  //----------------------------------------------------------------------------
  _nelectron = 0;

  for (int i=0; i<_nlepton; i++)
    {
      if (abs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR) _nelectron++;
    }
}


//------------------------------------------------------------------------------
// GetJets
//------------------------------------------------------------------------------
void AnalysisCMS::GetJets()
{
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
	if (AnalysisLeptons[j].type == Loose) continue;
	
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
  //----------------------------------------------------------------------------
  _nbjet = 0;

  for (int i=0; i<_njet; i++)
    {
      if (AnalysisJets[i].csvv2ivf > csvv2ivf_looseWP) _nbjet++;
    }


  // Define the jet bin
  //----------------------------------------------------------------------------
  _jetbin = (_njet < njetbin) ? _njet : njetbin - 1;
}


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisTop()
{
  if (_ntight < 2) return;


  // Pick the opposite-sign lepton-pair with highest _pt2l
  //----------------------------------------------------------------------------
  _pt2l = -999;

  for (UInt_t i=0; i<_nlepton; i++) {
    
    if (AnalysisLeptons[i].type == Loose) continue;

    for (UInt_t j=i+1; j<_nlepton; j++) {
      
      if (AnalysisLeptons[j].type == Loose) continue;

      if (AnalysisLeptons[i].flavour * AnalysisLeptons[j].flavour > 0) continue;

      float dilepton_pt = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).Pt();

      if (dilepton_pt > _pt2l)
	{
	  _pt2l = dilepton_pt;
	  
	  Lepton1 = AnalysisLeptons[i];
	  Lepton2 = AnalysisLeptons[j];
	}
    }
  }

  if (_pt2l < 0) return;

  _m2l = (Lepton1.v + Lepton2.v).M();

  int nelec =0;

  if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) nelec++;
  if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) nelec++;

  if      (nelec == 2) _channel = ee;
  else if (nelec == 1) _channel = em;
  else if (nelec == 0) _channel = mm;


  // Top selection
  //----------------------------------------------------------------------------
  bool pass = true;

  LevelHistograms(Top_00_Has2Leptons, pass);

  pass &= (_njet > 1);

  LevelHistograms(Top_01_Has2Jets, pass);

  pass &= (_nbjet > 0);

  LevelHistograms(Top_02_Has1BJet, pass);
}


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisTTDM()
{
  if (_nlepton != 2) return;
  if (_ntight  != 2) return;

  if      (_nelectron == 2) _channel = ee;
  else if (_nelectron == 1) _channel = em;
  else if (_nelectron == 0) _channel = mm;

  Lepton1 = AnalysisLeptons[0];
  Lepton2 = AnalysisLeptons[1];

  _m2l  = (Lepton1.v + Lepton2.v).M();
  _pt2l = (Lepton1.v + Lepton2.v).Pt();


  // TTDM selection
  //----------------------------------------------------------------------------
  bool pass = true;

  LevelHistograms(TTDM_00_Exactly2Leptons, pass);

  bool pass_sf = (_nelectron != 1 && fabs(_m2l - Z_MASS) > 15.);
  bool pass_df = (_nelectron == 1);

  pass &= (_m2l > 20.);
  pass &= (pass_sf || pass_df);

  LevelHistograms(TTDM_01_ZVeto, pass);

  pass &= (_njet > 1);

  LevelHistograms(TTDM_02_Has2Jets, pass);

  pass &= (Lepton1.v.Pt() + Lepton2.v.Pt() > 120.);

  LevelHistograms(TTDM_03_LepPtSum, pass);
  
  pass &= (AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt() < 400.);

  LevelHistograms(TTDM_04_JetPtSum, pass);

  pass &= (Lepton1.v.DeltaPhi(Lepton2.v) < 2.);

  LevelHistograms(TTDM_05_LepDeltaPhi, pass);

  pass &= (pfType1Met > 320.);

  LevelHistograms(TTDM_06_MET, pass);
}


//------------------------------------------------------------------------------
// AnalysisWW
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisWW()
{
  if (_nlepton != 2) return;
  if (_ntight  != 2) return;

  if      (_nelectron == 2) _channel = ee;
  else if (_nelectron == 1) _channel = em;
  else if (_nelectron == 0) _channel = mm;

  Lepton1 = AnalysisLeptons[0];
  Lepton2 = AnalysisLeptons[1];

  _m2l  = (Lepton1.v + Lepton2.v).M();
  _pt2l = (Lepton1.v + Lepton2.v).Pt();


  // WW selection
  //----------------------------------------------------------------------------
  bool pass = true;

  pass &= (Lepton1.v.Pt() > 20.);
  pass &= (Lepton2.v.Pt() > 20.);
  pass &= (Lepton1.flavour * Lepton2.flavour < 0);

  LevelHistograms(WW_00_Exactly2Leptons, pass);

  bool pass_sf = (_nelectron != 1 && _pt2l > 45. && fabs(_m2l - Z_MASS) > 15.);
  bool pass_df = (_nelectron == 1 && _pt2l > 30.);

  pass &= (pfType1Met > 20.);
  pass &= (_m2l > 12.);
  pass &= (pass_sf || pass_df);

  LevelHistograms(WW_01_ZVeto, pass);
  
  pass &= (_nbjet == 0);

  LevelHistograms(WW_02_BVeto, pass);
}


//------------------------------------------------------------------------------
// AnalysisWZ
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisWZ()
{
  if (_nlepton != 3) return;
  if (_ntight  != 3) return;

  if      (_nelectron == 3) _channel = eee;
  else if (_nelectron == 2) _channel = eem;
  else if (_nelectron == 1) _channel = emm;
  else if (_nelectron == 0) _channel = mmm;


  // Make Z and W candidates
  //----------------------------------------------------------------------------
  _m2l = -999;

  for (UInt_t i=0; i<_nlepton; i++) {
    
    for (UInt_t j=i+1; j<_nlepton; j++) {

      if (AnalysisLeptons[i].flavour + AnalysisLeptons[j].flavour != 0) continue;

      float inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

      if (_m2l < 0 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

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


  // WZ selection
  //----------------------------------------------------------------------------
  if (_m2l < 0) return;

  _m3l  = (ZLepton1.v + ZLepton2.v + WLepton.v).M();
  _pt2l = (ZLepton1.v + ZLepton2.v).Pt();

  bool pass = true;

  LevelHistograms(WZ_00_Exactly3Leptons, pass);
    
  pass &= (_m2l > 60. && _m2l < 120.);
  pass &= (ZLepton1.v.Pt() > 20.);

  LevelHistograms(WZ_01_HasZ, pass);

  pass &= (WLepton.v.DeltaR(ZLepton1.v) >  0.1);
  pass &= (WLepton.v.DeltaR(ZLepton2.v) >  0.1);
  pass &= (WLepton.v.Pt()               >  20.);
  pass &= (pfType1Met                   >  30.);
  pass &= (_m3l                         > 100.);

  LevelHistograms(WZ_02_HasW, pass);
	
  pass &= (_nbjet == 0);
	
  LevelHistograms(WZ_03_BVeto, pass);
}
