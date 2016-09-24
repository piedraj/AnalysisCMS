#define AnalysisCMS_cxx
#include "../include/AnalysisCMS.h"
#include "../include/lester_mt2_bisect.h"
#include "../top-reco/src/MassVariations.cc"


//------------------------------------------------------------------------------
// AnalysisCMS
//------------------------------------------------------------------------------
AnalysisCMS::AnalysisCMS(TTree* tree, TString systematic) : AnalysisBase(tree)
{
  _ismc         = true;
  _saveminitree = false;
  _eventdump    = false;

  _systematic_btag_do    = (systematic.Contains("Btagdo"))    ? true : false;
  _systematic_btag_up    = (systematic.Contains("Btagup"))    ? true : false;
  _systematic_idiso_do   = (systematic.Contains("Idisodo"))   ? true : false;
  _systematic_idiso_up   = (systematic.Contains("Idisoup"))   ? true : false;
  _systematic_trigger_do = (systematic.Contains("Triggerdo")) ? true : false;
  _systematic_trigger_up = (systematic.Contains("Triggerup")) ? true : false;
  _systematic_reco_do    = (systematic.Contains("Recodo"))    ? true : false;
  _systematic_reco_up    = (systematic.Contains("Recoup"))    ? true : false;

  _systematic = systematic;
}


//------------------------------------------------------------------------------
// PassTrigger
//------------------------------------------------------------------------------
bool AnalysisCMS::PassTrigger()
{
  if (!std_vector_trigger) return true;

  // HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*        #  6
  // HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*       #  8
  // HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*                      # 11
  // HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v*                    # 13
  // HLT_IsoTkMu22_v*                                         # 42
  // HLT_IsoMu22_v*                                           # 43
  // HLT_Ele27_eta2p1_WPLoose_Gsf_v*                          #  0
  // HLT_Ele45_WPLoose_Gsf_v*                                 # 56
  // HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*             # 46

  bool pass_MuonEG         = (std_vector_trigger->at(6)  || std_vector_trigger->at(8));
  bool pass_DoubleMuon     = (std_vector_trigger->at(11) || std_vector_trigger->at(13));
  bool pass_SingleMuon     = (std_vector_trigger->at(42) || std_vector_trigger->at(43));
  bool pass_SingleElectron = (std_vector_trigger->at(0)  || std_vector_trigger->at(56));
  bool pass_DoubleEG       = (std_vector_trigger->at(46));

  if      (_sample.Contains("MuonEG"))         return ( pass_MuonEG);
  else if (_sample.Contains("DoubleMuon"))     return (!pass_MuonEG &&  pass_DoubleMuon);
  else if (_sample.Contains("SingleMuon"))     return (!pass_MuonEG && !pass_DoubleMuon &&  pass_SingleMuon);
  else if (_sample.Contains("DoubleEG"))       return (!pass_MuonEG && !pass_DoubleMuon && !pass_SingleMuon &&  pass_DoubleEG);
  else if (_sample.Contains("SingleElectron")) return (!pass_MuonEG && !pass_DoubleMuon && !pass_SingleMuon && !pass_DoubleEG && pass_SingleElectron);
  else                                         return true;
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
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisCMS::FillHistograms(int ichannel, int icut, int ijet)
{
  // TH1 histograms
  //----------------------------------------------------------------------------
  h_counterRaw    [ichannel][icut][ijet]->Fill(1);
  h_counterLum    [ichannel][icut][ijet]->Fill(1,               _event_weight);
  h_nvtx          [ichannel][icut][ijet]->Fill(nvtx,            _event_weight);
  h_ptww          [ichannel][icut][ijet]->Fill(_ptww,           _event_weight);
  h_pt2l          [ichannel][icut][ijet]->Fill(_pt2l,           _event_weight);
  h_sumjpt12      [ichannel][icut][ijet]->Fill(_sumjpt12,       _event_weight);
  h_sumpt12       [ichannel][icut][ijet]->Fill(pt1+pt2,         _event_weight);  // Needs l2Sel
  h_dyll          [ichannel][icut][ijet]->Fill(_dyll,           _event_weight);
  h_mllbb         [ichannel][icut][ijet]->Fill(_mllbb,          _event_weight);
  h_dphimetjet    [ichannel][icut][ijet]->Fill(_dphimetjet,     _event_weight);
  h_meff          [ichannel][icut][ijet]->Fill(_meff,           _event_weight);
  h_ptbll         [ichannel][icut][ijet]->Fill(_ptbll,          _event_weight);
  h_dphimetptbll  [ichannel][icut][ijet]->Fill(_dphimetptbll,   _event_weight);
  h_mt2ll         [ichannel][icut][ijet]->Fill(_mt2ll,          _event_weight);
  h_mt2bb         [ichannel][icut][ijet]->Fill(_mt2bb,          _event_weight);
  h_mt2lblb       [ichannel][icut][ijet]->Fill(_mt2lblb,        _event_weight);
  h_mlb1          [ichannel][icut][ijet]->Fill(_mlb1,           _event_weight);
  h_mlb2          [ichannel][icut][ijet]->Fill(_mlb2,           _event_weight);


  // TH1 histograms with minitree variables
  //----------------------------------------------------------------------------
  h_channel       [ichannel][icut][ijet]->Fill(_channel,        _event_weight);
  h_dphijet1met   [ichannel][icut][ijet]->Fill(_dphijet1met,    _event_weight);
  h_dphijet2met   [ichannel][icut][ijet]->Fill(_dphijet2met,    _event_weight);
  h_dphijj        [ichannel][icut][ijet]->Fill(_dphijj,         _event_weight);
  h_dphijjmet     [ichannel][icut][ijet]->Fill(_dphijjmet,      _event_weight);
  h_dphill        [ichannel][icut][ijet]->Fill(dphill,          _event_weight);  // Needs l2Sel
  h_dphillmet     [ichannel][icut][ijet]->Fill(_dphillmet,      _event_weight);
  h_dphillstar    [ichannel][icut][ijet]->Fill(_dphillstar,     _event_weight);
  h_dphilmet1     [ichannel][icut][ijet]->Fill(dphilmet1,       _event_weight);  // Needs l2Sel
  h_dphilmet2     [ichannel][icut][ijet]->Fill(dphilmet2,       _event_weight);  // Needs l2Sel
  h_dphilep1jet1  [ichannel][icut][ijet]->Fill(_dphilep1jet1,   _event_weight);
  h_dphilep1jet2  [ichannel][icut][ijet]->Fill(_dphilep1jet2,   _event_weight);
  h_dphilep2jet1  [ichannel][icut][ijet]->Fill(_dphilep2jet1,   _event_weight);
  h_dphilep2jet2  [ichannel][icut][ijet]->Fill(_dphilep2jet2,   _event_weight);
  h_drll          [ichannel][icut][ijet]->Fill(drll,            _event_weight);  // Needs l2Sel
  h_ht            [ichannel][icut][ijet]->Fill(_ht,             _event_weight);
  h_htjets        [ichannel][icut][ijet]->Fill(_htjets,         _event_weight);
  h_htnojets      [ichannel][icut][ijet]->Fill(_htnojets,       _event_weight);
  h_jet1eta       [ichannel][icut][ijet]->Fill(jeteta1,         _event_weight);
  h_jet1mass      [ichannel][icut][ijet]->Fill(jetmass1,        _event_weight);
  h_jet1phi       [ichannel][icut][ijet]->Fill(jetphi1,         _event_weight);
  h_jet1pt        [ichannel][icut][ijet]->Fill(jetpt1,          _event_weight);
  h_jet2eta       [ichannel][icut][ijet]->Fill(jeteta2,         _event_weight);
  h_jet2mass      [ichannel][icut][ijet]->Fill(jetmass2,        _event_weight);
  h_jet2phi       [ichannel][icut][ijet]->Fill(jetphi2,         _event_weight);
  h_jet2pt        [ichannel][icut][ijet]->Fill(jetpt2,          _event_weight);
  h_detall        [ichannel][icut][ijet]->Fill(_detall,         _event_weight);
  h_lep1eta       [ichannel][icut][ijet]->Fill(_lep1eta,        _event_weight);
  h_lep1phi       [ichannel][icut][ijet]->Fill(_lep1phi,        _event_weight);
  h_lep1pt        [ichannel][icut][ijet]->Fill(_lep1pt,         _event_weight);
  h_lep2eta       [ichannel][icut][ijet]->Fill(_lep2eta,        _event_weight);
  h_lep2phi       [ichannel][icut][ijet]->Fill(_lep2phi,        _event_weight);
  h_lep2pt        [ichannel][icut][ijet]->Fill(_lep2pt,         _event_weight);
  h_lep1eta_gen   [ichannel][icut][ijet]->Fill(_lep1eta_gen,    _event_weight);
  h_lep1phi_gen   [ichannel][icut][ijet]->Fill(_lep1phi_gen,    _event_weight);
  h_lep1pt_gen    [ichannel][icut][ijet]->Fill(_lep1pt_gen,     _event_weight);
  h_lep2eta_gen   [ichannel][icut][ijet]->Fill(_lep2eta_gen,    _event_weight);
  h_lep2phi_gen   [ichannel][icut][ijet]->Fill(_lep2phi_gen,    _event_weight);
  h_lep2pt_gen    [ichannel][icut][ijet]->Fill(_lep2pt_gen,     _event_weight);
  h_mc            [ichannel][icut][ijet]->Fill(_mc,             _event_weight);
  h_metPfType1    [ichannel][icut][ijet]->Fill(MET.Et(),        _event_weight);
  h_metPfType1Phi [ichannel][icut][ijet]->Fill(MET.Phi(),       _event_weight);
  h_metTtrk       [ichannel][icut][ijet]->Fill(metTtrk,         _event_weight);
  h_metTtrkPhi    [ichannel][icut][ijet]->Fill(metTtrkPhi,      _event_weight);
  h_mpmet         [ichannel][icut][ijet]->Fill(_mpmet,          _event_weight);
  h_mth           [ichannel][icut][ijet]->Fill(mth,             _event_weight);  // Needs l2Sel
  h_mtw1          [ichannel][icut][ijet]->Fill(mtw1,            _event_weight);  // Needs l2Sel
  h_mtw2          [ichannel][icut][ijet]->Fill(mtw2,            _event_weight);  // Needs l2Sel
  h_m2l           [ichannel][icut][ijet]->Fill(_m2l,            _event_weight);
  h_nbjet15csvv2l [ichannel][icut][ijet]->Fill(_nbjet15csvv2l,  _event_weight);
  h_nbjet15csvv2m [ichannel][icut][ijet]->Fill(_nbjet15csvv2m,  _event_weight);
  h_nbjet15csvv2t [ichannel][icut][ijet]->Fill(_nbjet15csvv2t,  _event_weight);
  h_nbjet30csvv2l [ichannel][icut][ijet]->Fill(_nbjet30csvv2l,  _event_weight);
  h_nbjet30csvv2m [ichannel][icut][ijet]->Fill(_nbjet30csvv2m,  _event_weight);
  h_nbjet30csvv2t [ichannel][icut][ijet]->Fill(_nbjet30csvv2t,  _event_weight);
  h_nbjet20cmvav2l[ichannel][icut][ijet]->Fill(_nbjet20cmvav2l, _event_weight);
  h_nbjet20cmvav2m[ichannel][icut][ijet]->Fill(_nbjet20cmvav2m, _event_weight);
  h_nbjet20cmvav2t[ichannel][icut][ijet]->Fill(_nbjet20cmvav2t, _event_weight);
  h_nbjet30cmvav2l[ichannel][icut][ijet]->Fill(_nbjet30cmvav2l, _event_weight);
  h_nbjet30cmvav2m[ichannel][icut][ijet]->Fill(_nbjet30cmvav2m, _event_weight);
  h_nbjet30cmvav2t[ichannel][icut][ijet]->Fill(_nbjet30cmvav2t, _event_weight);
  h_njet          [ichannel][icut][ijet]->Fill(_njet,           _event_weight);
  h_top1eta_gen   [ichannel][icut][ijet]->Fill(_top1eta_gen,    _event_weight);
  h_top1phi_gen   [ichannel][icut][ijet]->Fill(_top1phi_gen,    _event_weight);
  h_top1pt_gen    [ichannel][icut][ijet]->Fill(_top1pt_gen,     _event_weight);
  h_top2eta_gen   [ichannel][icut][ijet]->Fill(_top2eta_gen,    _event_weight);
  h_top2phi_gen   [ichannel][icut][ijet]->Fill(_top2phi_gen,    _event_weight);
  h_top2pt_gen    [ichannel][icut][ijet]->Fill(_top2pt_gen,     _event_weight);
  h_m2t_gen       [ichannel][icut][ijet]->Fill(_m2t_gen,        _event_weight);
  h_dphitt_gen    [ichannel][icut][ijet]->Fill(_dphitt_gen,     _event_weight);
  h_detatt_gen    [ichannel][icut][ijet]->Fill(_detatt_gen,     _event_weight);
  h_topReco	  [ichannel][icut][ijet]->Fill(_topReco,        _event_weight);


  // TH2 histograms
  //----------------------------------------------------------------------------
  h_metPfType1_m2l[ichannel][icut][ijet]->Fill(MET.Et(), _m2l,    _event_weight);
  h_2ht           [ichannel][icut][ijet]->Fill(_ht,      _htjets, _event_weight);
  h_dym           [ichannel][icut][ijet]->Fill(_mllbb,   _dyll,   _event_weight);


  // Non-prompt systematic uncertainties
  //----------------------------------------------------------------------------
  h_fakes[ichannel][icut][ijet]->Fill(0., _fake_weight);
  h_fakes[ichannel][icut][ijet]->Fill(1., _fake_weight_elUp);
  h_fakes[ichannel][icut][ijet]->Fill(2., _fake_weight_elDown);
  h_fakes[ichannel][icut][ijet]->Fill(3., _fake_weight_elStatUp);
  h_fakes[ichannel][icut][ijet]->Fill(4., _fake_weight_elStatDown);
  h_fakes[ichannel][icut][ijet]->Fill(5., _fake_weight_muUp);
  h_fakes[ichannel][icut][ijet]->Fill(6., _fake_weight_muDown);
  h_fakes[ichannel][icut][ijet]->Fill(7., _fake_weight_muStatUp);
  h_fakes[ichannel][icut][ijet]->Fill(8., _fake_weight_muStatDown);
  
  if (_analysis.EqualTo("WZ"))
    {
      if (ichannel != lll) FillHistograms(lll, icut, ijet);
    }
  else
    {
      if (ichannel != ll)  FillHistograms(ll, icut, ijet);
    }
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisCMS::Summary(TString analysis,
			  TString precision,
			  TString title)
{
  int firstchannel = ee;
  int lastchannel  = ll;

  if (analysis.EqualTo("FR")) {firstchannel = e;   lastchannel = l;}
  if (analysis.EqualTo("WZ")) {firstchannel = eee; lastchannel = lll;}

  txt_summary << Form("\n%30s", title.Data());

  for (int i=firstchannel; i<=lastchannel; i++)
    txt_summary << Form("%11s    %11s", schannel[i].Data(), " ");

  txt_summary << Form("\n-------------------------------\n");

  for (int i=0; i<ncut; i++) {

    if (!scut[i].Contains(analysis)) continue;
      
    txt_summary << Form("%30s", scut[i].Data());

    for (int j=firstchannel; j<=lastchannel; j++) {

      TH1D* h_counter = h_counterRaw[j][i][njetbin];

      if (title.Contains("weighed")) h_counter = h_counterLum[j][i][njetbin];

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

  if (title.Contains("raw"))
    Summary(analysis, "11.2", "weighed yields");
}


//------------------------------------------------------------------------------
// Setup
//------------------------------------------------------------------------------
void AnalysisCMS::Setup(TString analysis,
			TString filename,
			float   luminosity)
{
  _analysis   = analysis;
  _filename   = filename;
  _luminosity = luminosity;
  _nentries   = fChain->GetEntries();

  TH1::SetDefaultSumw2();

  asymm_mt2_lester_bisect::disableCopyrightMessage();
  
  TString tok;

  Ssiz_t from = 0;

  while (_filename.Tokenize(tok, from, "latino_")) {

    if (tok.Contains(".root")) {

      _sample = tok.ReplaceAll(".root", "");
    }
  }

  if (_sample.Contains("DoubleEG"))       _ismc = false;
  if (_sample.Contains("DoubleMuon"))     _ismc = false;
  if (_sample.Contains("MuonEG"))         _ismc = false;
  if (_sample.Contains("SingleElectron")) _ismc = false;
  if (_sample.Contains("SingleMuon"))     _ismc = false;

  printf("\n");
  printf("   analysis: %s\n",        _analysis.Data());
  printf("   filename: %s\n",        _filename.Data());
  printf("     sample: %s\n",        _sample.Data());
  printf(" luminosity: %.3f fb-1\n", _luminosity);
  printf("   nentries: %lld\n",      _nentries);
  printf("       ismc: %d\n",        _ismc);
  
  gSystem->mkdir("rootfiles/" + _systematic + "/" + _analysis, kTRUE);
  gSystem->mkdir("txt/"       + _systematic + "/" + _analysis, kTRUE);

  _dataperiod = "";

  if (_filename.Contains("21Jun2016_v2_Run2016B")) _dataperiod = "_21Jun2016";
  if (_filename.Contains("05Jul2016_Run2016B"))    _dataperiod = "_05Jul2016";
  if (_filename.Contains("08Jul2016_Run2016B"))    _dataperiod = "_08Jul2016";
  if (_filename.Contains("08Jul2016_Run2016C"))    _dataperiod = "_08Jul2016";
  if (_filename.Contains("11Jul2016_Run2016C"))    _dataperiod = "_11Jul2016";
  if (_filename.Contains("15Jul2016_Run2016C"))    _dataperiod = "_15Jul2016";
  if (_filename.Contains("15Jul2016_Run2016D"))    _dataperiod = "_15Jul2016";
  if (_filename.Contains("26Jul2016_Run2016D"))    _dataperiod = "_26Jul2016";

  _isdatadriven = "";

  if (_filename.Contains("fakeW")) _isdatadriven = "fakeW_";

  root_output = new TFile("rootfiles/" + _systematic + "/" + _analysis + "/" + _isdatadriven + _sample + _dataperiod + ".root", "recreate");

  if (_eventdump) txt_eventdump.open("txt/" + _systematic + "/" + _analysis + "/" + _isdatadriven + _sample + _dataperiod + "_eventdump.txt");


  OpenMinitree();


  // Histograms for QCD, PDF and alpha_s uncertainties
  //----------------------------------------------------------------------------
  root_output->cd();

  list_vectors_weights_gen = GetGenWeightsLHE();

  list_vectors_weights_0jet = new TH1F("list_vectors_weights_0jet", "", 200, 0, 200);
  list_vectors_weights_1jet = new TH1F("list_vectors_weights_1jet", "", 200, 0, 200);
  list_vectors_weights_2jet = new TH1F("list_vectors_weights_2jet", "", 200, 0, 200);

  list_vectors_weights_wh3l      = new TH1F("list_vectors_weights_wh3l",      "", 200, 0, 200);
  list_vectors_weights_wh3l_ossf = new TH1F("list_vectors_weights_wh3l_ossf", "", 200, 0, 200);
  list_vectors_weights_wh3l_sssf = new TH1F("list_vectors_weights_wh3l_sssf", "", 200, 0, 200);


  return;
}


//------------------------------------------------------------------------------
// ApplyWeights
//------------------------------------------------------------------------------
void AnalysisCMS::ApplyWeights()
{
  _event_weight = 1.0;

  if (_analysis.EqualTo("FR")) return;

  _event_weight = PassTrigger();  // _event_weight = PassTrigger() * metFilter;
  
  if (!_ismc && _filename.Contains("fakeW")) _event_weight *= _fake_weight;

  if (!_ismc) return;

  _event_weight *= _luminosity * baseW * puW;


  // Include btag, trigger and idiso systematic uncertainties
  //----------------------------------------------------------------------------
  if (std_vector_lepton_idisoW)
    {
      float sf_btag = 1.0;

      if (_analysis.EqualTo("Top")  ||
	  _analysis.EqualTo("TTDM") ||
	  _analysis.EqualTo("Stop") ||
	  _analysis.EqualTo("Control"))
	{
	  sf_btag = bPogSF_CSVM;
	  if (_systematic_btag_up) sf_btag = bPogSF_CSVM_Up;
	  if (_systematic_btag_do) sf_btag = bPogSF_CSVM_Down;
	}
      else
	{
	  sf_btag = bPogSF_CMVAL;
	  if (_systematic_btag_up) sf_btag = bPogSF_CMVAL_Up;
	  if (_systematic_btag_do) sf_btag = bPogSF_CMVAL_Down;
	}

      float sf_trigger = effTrigW;  // To be updated for WZ
      float sf_idiso   = std_vector_lepton_idisoW->at(0) * std_vector_lepton_idisoW->at(1);
      float sf_reco    = std_vector_lepton_recoW->at(0) * std_vector_lepton_recoW->at(1);

      if (_systematic_trigger_up) sf_trigger = effTrigW_Up;
      if (_systematic_trigger_do) sf_trigger = effTrigW_Down;

      if (_systematic_idiso_up) sf_idiso = std_vector_lepton_idisoW_Up->at(0)   * std_vector_lepton_idisoW_Up->at(1);
      if (_systematic_idiso_do) sf_idiso = std_vector_lepton_idisoW_Down->at(0) * std_vector_lepton_idisoW_Down->at(1);

      if (_systematic_reco_up) sf_reco = std_vector_lepton_recoW_Up->at(0)   * std_vector_lepton_recoW_Up->at(1);
      if (_systematic_reco_do) sf_reco = std_vector_lepton_recoW_Down->at(0) * std_vector_lepton_recoW_Down->at(1);

      if (_analysis.EqualTo("WZ"))
	{
	  sf_idiso = std_vector_lepton_idisoW->at(0) * std_vector_lepton_idisoW->at(1) * std_vector_lepton_idisoW->at(2);

	  if (_systematic_idiso_up) sf_idiso = std_vector_lepton_idisoW_Up->at(0)   * std_vector_lepton_idisoW_Up->at(1)   * std_vector_lepton_idisoW_Up->at(2);
	  if (_systematic_idiso_do) sf_idiso = std_vector_lepton_idisoW_Down->at(0) * std_vector_lepton_idisoW_Down->at(1) * std_vector_lepton_idisoW_Down->at(2);
	  
	  sf_reco = std_vector_lepton_recoW->at(0) * std_vector_lepton_recoW->at(1) * std_vector_lepton_recoW->at(2);

	  if (_systematic_reco_up) sf_reco = std_vector_lepton_recoW_Up->at(0)   * std_vector_lepton_recoW_Up->at(1)   * std_vector_lepton_recoW_Up->at(2);
	  if (_systematic_reco_do) sf_reco = std_vector_lepton_recoW_Down->at(0) * std_vector_lepton_recoW_Down->at(1) * std_vector_lepton_recoW_Down->at(2);
	}

      _event_weight *= sf_btag * sf_trigger * sf_idiso * sf_reco;
    }

  if (_sample.EqualTo("WWTo2L2Nu"))        _event_weight *= nllW;
  if (_sample.EqualTo("WgStarLNuEE"))      _event_weight *= 1.4;  // k_factor = 1.4 +- 0.5
  if (_sample.EqualTo("WgStarLNuMuMu"))    _event_weight *= 1.4;  // k_factor = 1.4 +- 0.5
  if (_sample.EqualTo("DYJetsToTT_MuEle")) _event_weight *= 1.26645;
  if (_sample.EqualTo("Wg_MADGRAPHMLM"))   _event_weight *= !(Gen_ZGstar_mass > 0. && Gen_ZGstar_MomId == 22);

  _event_weight *= (std_vector_lepton_genmatched->at(0)*std_vector_lepton_genmatched->at(1));

  if (_analysis.EqualTo("WZ")) _event_weight *= std_vector_lepton_genmatched->at(2);

  _event_weight *= _gen_ptll_weight;

  if (!GEN_weight_SM) return;
  
  _event_weight *= GEN_weight_SM / abs(GEN_weight_SM);

  return;
}


//------------------------------------------------------------------------------
// GetLeptons
//------------------------------------------------------------------------------
void AnalysisCMS::GetLeptons()
{
  // GEN
  //----------------------------------------------------------------------------
  _lep1eta_gen = (_ismc) ? std_vector_leptonGen_eta->at(0) : -999;
  _lep1phi_gen = (_ismc) ? std_vector_leptonGen_phi->at(0) : -999;
  _lep1pt_gen  = (_ismc) ? std_vector_leptonGen_pt->at(0)  : -999;

  _lep2eta_gen = (_ismc) ? std_vector_leptonGen_eta->at(1) : -999;
  _lep2phi_gen = (_ismc) ? std_vector_leptonGen_phi->at(1) : -999;
  _lep2pt_gen  = (_ismc) ? std_vector_leptonGen_pt->at(1)  : -999;


  // RECO
  //----------------------------------------------------------------------------
  bool found_third_tight_lepton = false;

  AnalysisLeptons.clear();

  int vector_lepton_size = std_vector_lepton_pt->size();

  for (int i=0; i<vector_lepton_size; i++) {

    float eta     = std_vector_lepton_eta->at(i);
    float flavour = std_vector_lepton_flavour->at(i);
    float phi     = std_vector_lepton_phi->at(i);
    float pt      = std_vector_lepton_pt->at(i);
    float type    = std_vector_lepton_isTightLepton->at(i);
    float idisoW  = (std_vector_lepton_idisoW) ? std_vector_lepton_idisoW->at(i) : 1.;

    if (!std_vector_lepton_isLooseLepton->at(i)) continue;

    if (pt < 0.) continue;

    bool reject_lepton = false;
    
    if (i > 1 && !_filename.Contains("fakeW") && _analysis.EqualTo("WZ"))
      {
	if (!found_third_tight_lepton)
	  {
	    if (type != Tight)
	      reject_lepton = true;
	    else
	      found_third_tight_lepton = true;
	  }
      }

    if (reject_lepton) continue;

    Lepton lep;
      
    lep.index   = i;
    lep.type    = type;
    lep.flavour = flavour;
    lep.idisoW  = idisoW;
      
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

    TLorentzVector tlv;
    
    tlv.SetPtEtaPhiM(pt, eta, phi, mass);

    lep.v = tlv;

    AnalysisLeptons.push_back(lep);

    if (i == 0) Lepton1 = lep;
    if (i == 1) Lepton2 = lep;
  }

  _nlepton = AnalysisLeptons.size();

  _lep1eta = Lepton1.v.Eta();
  _lep1phi = Lepton1.v.Phi();
  _lep1pt  = Lepton1.v.Pt();

  _lep2eta = Lepton2.v.Eta();
  _lep2phi = Lepton2.v.Phi();
  _lep2pt  = Lepton2.v.Pt();

  _detall = fabs(_lep1eta - _lep2eta);
}


//------------------------------------------------------------------------------
// GetJets
//------------------------------------------------------------------------------
void AnalysisCMS::GetJets(float jet_eta_max)
{
  AnalysisJets.clear();

  _nbjet15csvv2l  = 0;
  _nbjet15csvv2m  = 0;
  _nbjet15csvv2t  = 0;
  _nbjet30csvv2l  = 0;
  _nbjet30csvv2m  = 0;
  _nbjet30csvv2t  = 0;
  _nbjet20cmvav2l = 0;
  _nbjet20cmvav2m = 0;
  _nbjet20cmvav2t = 0;
  _nbjet30cmvav2l = 0;
  _nbjet30cmvav2m = 0;
  _nbjet30cmvav2t = 0;

  int vector_jet_size = std_vector_jet_pt->size();

  for (int i=0; i<vector_jet_size; i++) {

    float pt  = std_vector_jet_pt ->at(i);
    float eta = std_vector_jet_eta->at(i);
    float phi = std_vector_jet_phi->at(i);

    if (jet_eta_max > 0 && fabs(eta) > jet_eta_max) continue;

    TLorentzVector tlv;

    tlv.SetPtEtaPhiM(pt, eta, phi, 0.0);

    Jet goodjet;

    goodjet.index    = i;
    goodjet.cmvav2   = std_vector_jet_cmvav2->at(i);
    goodjet.csvv2ivf = std_vector_jet_csvv2ivf->at(i);
    goodjet.v        = tlv;

    if (pt > 15. && goodjet.csvv2ivf > CSVv2L) _nbjet15csvv2l++;
    if (pt > 15. && goodjet.csvv2ivf > CSVv2M) _nbjet15csvv2m++;
    if (pt > 15. && goodjet.csvv2ivf > CSVv2T) _nbjet15csvv2t++;

    if (pt > 20. && goodjet.cmvav2 > cMVAv2L) _nbjet20cmvav2l++;
    if (pt > 20. && goodjet.cmvav2 > cMVAv2M) _nbjet20cmvav2m++;
    if (pt > 20. && goodjet.cmvav2 > cMVAv2T) _nbjet20cmvav2t++;

    if (pt < 30.) continue;

    if (goodjet.csvv2ivf > CSVv2L) _nbjet30csvv2l++;
    if (goodjet.csvv2ivf > CSVv2M) _nbjet30csvv2m++;
    if (goodjet.csvv2ivf > CSVv2T) _nbjet30csvv2t++;

    if (goodjet.cmvav2 > cMVAv2L) _nbjet30cmvav2l++;
    if (goodjet.cmvav2 > cMVAv2M) _nbjet30cmvav2m++;
    if (goodjet.cmvav2 > cMVAv2T) _nbjet30cmvav2t++;

    AnalysisJets.push_back(goodjet);
  }


  // Define the jet bin
  //----------------------------------------------------------------------------
  _njet = AnalysisJets.size();

  _jetbin = (_njet < njetbin) ? _njet : njetbin - 1;
}


//------------------------------------------------------------------------------
// GetJetPtSum
//------------------------------------------------------------------------------
void AnalysisCMS::GetJetPtSum()
{
  _sumjpt12 = (_njet < 2) ? -999 : (AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt());
}


//------------------------------------------------------------------------------
// EventDump
//------------------------------------------------------------------------------
void AnalysisCMS::EventDump()
{
  for (int i=0; i<_nlepton; i++)
    {
      int index = AnalysisLeptons[i].index;

      txt_eventdump << Form("%d:%d:%f:%f:%f:%.0f\n",
			    event,
			    AnalysisLeptons[i].flavour,
			    AnalysisLeptons[i].v.Pt(),
			    AnalysisLeptons[i].v.Eta(),
			    AnalysisLeptons[i].iso,
			    std_vector_lepton_isTightLepton->at(index));
    }
}


//------------------------------------------------------------------------------
// GetMET
//------------------------------------------------------------------------------
void AnalysisCMS::GetMET(float module, float phi)
{
  MET.SetPtEtaPhiM(module, 0.0, phi, 0.0);
}


//------------------------------------------------------------------------------
// GetTrkMET
//------------------------------------------------------------------------------
void AnalysisCMS::GetTrkMET(float module, float phi)
{
  trkMET.SetPtEtaPhiM(module, 0.0, phi, 0.0);
}



//------------------------------------------------------------------------------
// GetStarVar
//------------------------------------------------------------------------------
void AnalysisCMS::GetStarVar()
{
  float met    = MET.Et();
  float metphi = MET.Phi();
  
  float beta = sqrt(met*met / (met*met + H_MASS*H_MASS));

  TVector3 BL(beta * cos(metphi), beta * sin(metphi), 0);

  TLorentzVector L1Star = Lepton1.v;
  TLorentzVector L2Star = Lepton2.v;
  
  L1Star.Boost(BL);
  L2Star.Boost(BL);
  
  _dphillstar = L1Star.DeltaPhi(L2Star);
  _mllstar    = (L1Star + L2Star).M();
}


//------------------------------------------------------------------------------
// GetZHCRVar
//------------------------------------------------------------------------------
void AnalysisCMS::GetZHCRVar()
{
  _mll13 = -1.;
  _mll23 = -1.;
  _mll14 = -1.;
  _mll24 = -1.;
  _mll34 = -1.;

  if (_nlepton > 3) {
    _mll13 = (AnalysisLeptons[0].v + AnalysisLeptons[2].v).M();
    _mll23 = (AnalysisLeptons[1].v + AnalysisLeptons[2].v).M();
    _mll14 = (AnalysisLeptons[0].v + AnalysisLeptons[3].v).M();
    _mll24 = (AnalysisLeptons[1].v + AnalysisLeptons[3].v).M();
    _mll34 = (AnalysisLeptons[2].v + AnalysisLeptons[3].v).M();
  }
}


//------------------------------------------------------------------------------
// GetDeltaPhi
//------------------------------------------------------------------------------
void AnalysisCMS::GetDeltaPhi()
{
  // Reset variables
  //----------------------------------------------------------------------------
  _dphijet1met  = -0.1;
  _dphijet2met  = -0.1;
  _dphijj       = -0.1;
  _dphijjmet    = -0.1;
  _dphilep1jet1 = -0.1;
  _dphilep1jet2 = -0.1;
  _dphilep2jet1 = -0.1;
  _dphilep2jet2 = -0.1;
  _dphillmet    = -0.1;


  // Fill variables
  //----------------------------------------------------------------------------
  _dphillmet = fabs((Lepton1.v + Lepton2.v).DeltaPhi(MET));

  if (_njet > 0)
    {
      _dphijet1met  = fabs(AnalysisJets[0].v.DeltaPhi(MET));
      _dphilep1jet1 = fabs(Lepton1.v.DeltaPhi(AnalysisJets[0].v));
      _dphilep2jet1 = fabs(Lepton2.v.DeltaPhi(AnalysisJets[0].v));
    }

  if (_njet > 1)
    {
      _dphijet2met  = fabs(AnalysisJets[1].v.DeltaPhi(MET));
      _dphilep1jet2 = fabs(Lepton1.v.DeltaPhi(AnalysisJets[1].v));
      _dphilep2jet2 = fabs(Lepton2.v.DeltaPhi(AnalysisJets[1].v));
      _dphijj       = fabs(AnalysisJets[0].v.DeltaPhi(AnalysisJets[1].v));
      _dphijjmet    = fabs((AnalysisJets[0].v + AnalysisJets[1].v).DeltaPhi(MET));
    }
}


//------------------------------------------------------------------------------
// GetHt
//------------------------------------------------------------------------------
void AnalysisCMS::GetHt()
{
  _htjets = 0;

  _htnojets = MET.Et() + Lepton1.v.Pt() + Lepton2.v.Pt();

  _ht = _htnojets;

  for (int ijet=0; ijet<_njet; ijet++) {
    
    _ht     += AnalysisJets[ijet].v.Pt();
    _htjets += AnalysisJets[ijet].v.Pt();

  }
}


//------------------------------------------------------------------------------  
// GetMpMet
//------------------------------------------------------------------------------     
void AnalysisCMS::GetMpMet()
{
  _fullpmet = MET.Et();
  _trkpmet  = trkMET.Et();

  float dphil1trkmet = fabs(Lepton1.v.DeltaPhi(trkMET));
  float dphil2trkmet = fabs(Lepton2.v.DeltaPhi(trkMET));
  float dphiltrkmet  = min(dphil1trkmet, dphil2trkmet);

  if (dphilmet    < TMath::Pi() / 2.) _fullpmet *= sin(dphilmet);  // Needs l2Sel
  if (dphiltrkmet < TMath::Pi() / 2.) _trkpmet  *= sin(dphiltrkmet);

  _mpmet = min(_trkpmet, _fullpmet);
}


//------------------------------------------------------------------------------                                                               
// GetMetVar                                                                                                                                   
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMetVar()
{
  _metvar = (_njet <= 1) ? _mpmet : MET.Et();
}


//------------------------------------------------------------------------------                                                               
// GetDeltaPhiVeto                                                                                                                                 
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetDeltaPhiVeto()
{
  _passdphiveto = (_njet < 2 || dphilljetjet < 165.*TMath::DegToRad());  // Needs l2Sel
}


//------------------------------------------------------------------------------                                                               
// GetMt
//------------------------------------------------------------------------------
void AnalysisCMS::GetMt(Lepton lep, float& transverse_mass)
{
  transverse_mass = 0;

  float met  = MET.Et();
  float pt   = lep.v.Pt();
  float dphi = lep.v.DeltaPhi(MET);

  if (met < 0) return;
  if (pt  < 0) return;

  transverse_mass = sqrt(2*pt*met*(1. - cos(dphi)));

  return;
}


//------------------------------------------------------------------------------                                                               
// GetMc                                                                                                                                       
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMc()
{
  _mc = 0;

  float met = MET.Et();

  if (ptll > 0 && mll > 0 && met > 0)                                          // Needs l2Sel
    _mc = sqrt(pow(sqrt(ptll*ptll + mll*mll) + met, 2) - pow(ptll + met, 2));  // Needs l2Sel
}


//------------------------------------------------------------------------------                                                               
// GetPtWW                                                                                                                                     
//------------------------------------------------------------------------------                                                                
void AnalysisCMS::GetPtWW()
{
  _ptww = (Lepton1.v + Lepton2.v + MET).Pt();
}


//------------------------------------------------------------------------------                                                               
// GetSoftMuon
// https://twiki.cern.ch/twiki/bin/view/CMS/WW2015Variables#Soft_muons
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetSoftMuon()
{
  _foundsoftmuon = false;

  for (UInt_t i=0; i<std_vector_softMuPt->size(); i++) {
      
    if (std_vector_softMuPt->at(i)  < 3.)               continue;
    if (std_vector_softMuD0->at(i)  < 0.2)              continue;
    if (std_vector_softMuDz->at(i)  < 0.5)              continue;
    if (std_vector_softMuIso->at(i) < 0.15)             continue;
    if (!std_vector_softMuIsTrackerMuon->at(i))         continue;
    if (!std_vector_softMuTMLastStationAngTight->at(i)) continue;
      
    _foundsoftmuon = true;
      
    break;
  }
}


//------------------------------------------------------------------------------
// GetFakeWeights
//------------------------------------------------------------------------------
void AnalysisCMS::GetFakeWeights()
{
  if (_analysis.EqualTo("WZ"))
    {
      _fake_weight            = fakeW3l;
      _fake_weight_elUp       = fakeW3lElUp;
      _fake_weight_elDown     = fakeW3lElDown;
      _fake_weight_elStatUp   = fakeW3lstatElUp;
      _fake_weight_elStatDown = fakeW3lstatElDown;
      _fake_weight_muUp       = fakeW3lMuUp;
      _fake_weight_muDown     = fakeW3lMuDown;
      _fake_weight_muStatUp   = fakeW3lstatMuUp;
      _fake_weight_muStatDown = fakeW3lstatMuDown;
    }
  else
    {
      _fake_weight            = (fakeW2l0j          *(_njet == 0) + fakeW2l1j          *(_njet == 1) + fakeW2l2j          *(_njet >= 2));
      _fake_weight_elUp       = (fakeW2l0jElUp      *(_njet == 0) + fakeW2l1jElUp      *(_njet == 1) + fakeW2l2jElUp      *(_njet >= 2));
      _fake_weight_elDown     = (fakeW2l0jElDown    *(_njet == 0) + fakeW2l1jElDown    *(_njet == 1) + fakeW2l2jElDown    *(_njet >= 2));
      _fake_weight_elStatUp   = (fakeW2l0jstatElUp  *(_njet == 0) + fakeW2l1jstatElUp  *(_njet == 1) + fakeW2l2jstatElUp  *(_njet >= 2));
      _fake_weight_elStatDown = (fakeW2l0jstatElDown*(_njet == 0) + fakeW2l1jstatElDown*(_njet == 1) + fakeW2l2jstatElDown*(_njet >= 2));
      _fake_weight_muUp       = (fakeW2l0jMuUp      *(_njet == 0) + fakeW2l1jMuUp      *(_njet == 1) + fakeW2l2jMuUp      *(_njet >= 2));
      _fake_weight_muDown     = (fakeW2l0jMuDown    *(_njet == 0) + fakeW2l1jMuDown    *(_njet == 1) + fakeW2l2jMuDown    *(_njet >= 2));
      _fake_weight_muStatUp   = (fakeW2l0jstatMuUp  *(_njet == 0) + fakeW2l1jstatMuUp  *(_njet == 1) + fakeW2l2jstatMuUp  *(_njet >= 2));
      _fake_weight_muStatDown = (fakeW2l0jstatMuDown*(_njet == 0) + fakeW2l1jstatMuDown*(_njet == 1) + fakeW2l2jstatMuDown*(_njet >= 2));
    }
}


//------------------------------------------------------------------------------
// EventSetup
//------------------------------------------------------------------------------
void AnalysisCMS::EventSetup(float jet_eta_max)
{
  GetMET(metPfType1, metPfType1Phi);

  GetTrkMET(metTtrk, metTtrkPhi);

  GetLeptons();

  GetJets(jet_eta_max);

  GetTops();

  GetTopReco();

  GetGenPtllWeight();

  GetFakeWeights();

  ApplyWeights();


  // Additional analysis variables
  //----------------------------------------------------------------------------
  GetDeltaPhi();

  GetDeltaR(); 

  GetJetPtSum();

  GetHt();

  GetStarVar();

  GetMpMet();

  GetSoftMuon();

  GetMc();

  GetPtWW();

  GetMetVar();

  GetDeltaPhiVeto();

  GetStopVar();
}


//------------------------------------------------------------------------------
// PrintProgress
//------------------------------------------------------------------------------
void AnalysisCMS::PrintProgress(Long64_t counter, Long64_t total)
{
  double progress = 1e2 * (counter+1) / total;

  double fractpart, intpart;

  fractpart = modf(progress, &intpart);

  if (fractpart < 1e-2)
    {
      std::cout << "   " << _sample.Data() << " progress: " << int(ceil(progress)) << "%\r";
      std::cout.flush();
    }
}


//------------------------------------------------------------------------------
// EndJob
//------------------------------------------------------------------------------
void AnalysisCMS::EndJob()
{
  if (_eventdump) txt_eventdump.close();

  if (_saveminitree)
    {
      root_minitree->cd();

      printf("\n\n Writing minitree. This can take a while...");

      root_minitree->Write("", TObject::kOverwrite);

      root_minitree->Close();
    }

  txt_summary.open("txt/" + _systematic + "/" + _analysis + "/" + _isdatadriven + _sample + _dataperiod + ".txt");

  txt_summary << "\n";
  txt_summary << Form("   analysis: %s\n",        _analysis.Data());
  txt_summary << Form("   filename: %s\n",        _filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form(" luminosity: %.3f fb-1\n", _luminosity);
  txt_summary << Form("   nentries: %lld\n",      _nentries);
  txt_summary << "\n";
  
  if (!_analysis.EqualTo("FR")) Summary(_analysis, "11.0", "raw yields");

  txt_summary.close();

  root_output->cd();

  printf("\n\n Writing histograms. This can take a while...\n");

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();

  printf("\n Done with %s\n\n", _filename.Data());
}


//------------------------------------------------------------------------------
// DefineHistograms
//------------------------------------------------------------------------------
void AnalysisCMS::DefineHistograms(int     ichannel,
				   int     icut,
				   int     ijet,
				   TString suffix)
{
  // TH1 histograms
  //----------------------------------------------------------------------------
  h_counterRaw   [ichannel][icut][ijet] = new TH1D("h_counterRaw"    + suffix, "",    3, 0,    3);
  h_counterLum   [ichannel][icut][ijet] = new TH1D("h_counterLum"    + suffix, "",    3, 0,    3);
  h_fakes        [ichannel][icut][ijet] = new TH1D("h_fakes"         + suffix, "",    9, 0,    9);
  h_nvtx         [ichannel][icut][ijet] = new TH1D("h_nvtx"          + suffix, "",   50, 0,   50);
  h_ptww         [ichannel][icut][ijet] = new TH1D("h_ptww"          + suffix, "", 3000, 0, 3000);
  h_pt2l         [ichannel][icut][ijet] = new TH1D("h_pt2l"          + suffix, "", 3000, 0, 3000);
  h_sumjpt12     [ichannel][icut][ijet] = new TH1D("h_sumjpt12"      + suffix, "", 3000, 0, 3000);
  h_sumpt12      [ichannel][icut][ijet] = new TH1D("h_sumpt12"       + suffix, "", 3000, 0, 3000);
  h_dyll         [ichannel][icut][ijet] = new TH1D("h_dyll"          + suffix, "",  100, 0,    5);
  h_dphimetjet   [ichannel][icut][ijet] = new TH1D("h_dphimetjet"    + suffix, "",  100, 0,  3.2);
  h_dphimetptbll [ichannel][icut][ijet] = new TH1D("h_dphimetptbll"  + suffix, "",  100, 0,  3.2);
  h_mllbb        [ichannel][icut][ijet] = new TH1D("h_mllbb"         + suffix, "", 3000, 0, 3000);
  h_meff         [ichannel][icut][ijet] = new TH1D("h_meff"          + suffix, "", 3000, 0, 3000);
  h_ptbll        [ichannel][icut][ijet] = new TH1D("h_ptbll"         + suffix, "", 3000, 0, 3000);
  h_mt2ll        [ichannel][icut][ijet] = new TH1D("h_mt2ll"         + suffix, "", 3000, 0, 3000);
  h_mt2bb        [ichannel][icut][ijet] = new TH1D("h_mt2bb"         + suffix, "", 3000, 0, 3000);
  h_mt2lblb      [ichannel][icut][ijet] = new TH1D("h_mt2lblb"       + suffix, "", 3000, 0, 3000);
  h_mlb1         [ichannel][icut][ijet] = new TH1D("h_mlb1"          + suffix, "", 3000, 0, 3000);
  h_mlb2         [ichannel][icut][ijet] = new TH1D("h_mlb2"          + suffix, "", 3000, 0, 3000);


  // TH1 histograms with minitree variables
  //----------------------------------------------------------------------------
  h_channel       [ichannel][icut][ijet] = new TH1D("h_channel"        + suffix, "",   10,    0,   10);
  h_dphijet1met   [ichannel][icut][ijet] = new TH1D("h_dphijet1met"    + suffix, "",  100,    0,  3.2);
  h_dphijet2met   [ichannel][icut][ijet] = new TH1D("h_dphijet2met"    + suffix, "",  100,    0,  3.2);
  h_dphijj        [ichannel][icut][ijet] = new TH1D("h_dphijj"         + suffix, "",  100,    0,  3.2);
  h_dphijjmet     [ichannel][icut][ijet] = new TH1D("h_dphijjmet"      + suffix, "",  100,    0,  3.2);
  h_dphilep1jet1  [ichannel][icut][ijet] = new TH1D("h_dphilep1jet1"   + suffix, "",  100,    0,  3.2);
  h_dphilep1jet2  [ichannel][icut][ijet] = new TH1D("h_dphilep1jet2"   + suffix, "",  100,    0,  3.2);
  h_dphilep2jet1  [ichannel][icut][ijet] = new TH1D("h_dphilep2jet1"   + suffix, "",  100,    0,  3.2);
  h_dphilep2jet2  [ichannel][icut][ijet] = new TH1D("h_dphilep2jet2"   + suffix, "",  100,    0,  3.2);
  h_dphill        [ichannel][icut][ijet] = new TH1D("h_dphill"         + suffix, "",  100,    0,  3.2);
  h_dphillmet     [ichannel][icut][ijet] = new TH1D("h_dphillmet"      + suffix, "",  100,    0,  3.2);
  h_dphillstar    [ichannel][icut][ijet] = new TH1D("h_dphillstar"     + suffix, "",  100,    0,  3.2);
  h_dphilmet1     [ichannel][icut][ijet] = new TH1D("h_dphilmet1"      + suffix, "",  100,    0,  3.2);
  h_dphilmet2     [ichannel][icut][ijet] = new TH1D("h_dphilmet2"      + suffix, "",  100,    0,  3.2);
  h_drll          [ichannel][icut][ijet] = new TH1D("h_drll"           + suffix, "",  100,    0,    5);
  h_detall        [ichannel][icut][ijet] = new TH1D("h_detall"         + suffix, "",  100,    0,   10);
  h_lep1eta       [ichannel][icut][ijet] = new TH1D("h_lep1eta"        + suffix, "",   60,   -3,    3);
  h_lep2eta       [ichannel][icut][ijet] = new TH1D("h_lep2eta"        + suffix, "",   60,   -3,    3);
  h_lep1eta_gen   [ichannel][icut][ijet] = new TH1D("h_lep1eta_gen"    + suffix, "",   60,   -3,    3);
  h_lep2eta_gen   [ichannel][icut][ijet] = new TH1D("h_lep2eta_gen"    + suffix, "",   60,   -3,    3);
  h_jet1eta       [ichannel][icut][ijet] = new TH1D("h_jet1eta"        + suffix, "",  100,   -5,    5);
  h_jet2eta       [ichannel][icut][ijet] = new TH1D("h_jet2eta"        + suffix, "",  100,   -5,    5);
  h_lep1phi       [ichannel][icut][ijet] = new TH1D("h_lep1phi"        + suffix, "",  200, -3.2,  3.2);
  h_lep2phi       [ichannel][icut][ijet] = new TH1D("h_lep2phi"        + suffix, "",  200, -3.2,  3.2);
  h_lep1phi_gen   [ichannel][icut][ijet] = new TH1D("h_lep1phi_gen"    + suffix, "",  200, -3.2,  3.2);
  h_lep2phi_gen   [ichannel][icut][ijet] = new TH1D("h_lep2phi_gen"    + suffix, "",  200, -3.2,  3.2);
  h_jet1phi       [ichannel][icut][ijet] = new TH1D("h_jet1phi"        + suffix, "",  200, -3.2,  3.2);
  h_jet2phi       [ichannel][icut][ijet] = new TH1D("h_jet2phi"        + suffix, "",  200, -3.2,  3.2);
  h_metPfType1Phi [ichannel][icut][ijet] = new TH1D("h_metPfType1Phi"  + suffix, "",  200, -3.2,  3.2);
  h_metTtrkPhi    [ichannel][icut][ijet] = new TH1D("h_metTtrkPhi"     + suffix, "",  200, -3.2,  3.2);
  h_lep1pt        [ichannel][icut][ijet] = new TH1D("h_lep1pt"         + suffix, "", 3000,    0, 3000);
  h_lep2pt        [ichannel][icut][ijet] = new TH1D("h_lep2pt"         + suffix, "", 3000,    0, 3000);
  h_lep1pt_gen    [ichannel][icut][ijet] = new TH1D("h_lep1pt_gen"     + suffix, "", 3000,    0, 3000);
  h_lep2pt_gen    [ichannel][icut][ijet] = new TH1D("h_lep2pt_gen"     + suffix, "", 3000,    0, 3000);
  h_jet1pt        [ichannel][icut][ijet] = new TH1D("h_jet1pt"         + suffix, "", 3000,    0, 3000);
  h_jet2pt        [ichannel][icut][ijet] = new TH1D("h_jet2pt"         + suffix, "", 3000,    0, 3000);
  h_jet1mass      [ichannel][icut][ijet] = new TH1D("h_jet1mass"       + suffix, "",  100,    0,  100);
  h_jet2mass      [ichannel][icut][ijet] = new TH1D("h_jet2mass"       + suffix, "",  100,    0,  100);
  h_ht            [ichannel][icut][ijet] = new TH1D("h_ht"             + suffix, "", 3000,    0, 3000);
  h_htjets        [ichannel][icut][ijet] = new TH1D("h_htjets"         + suffix, "", 3000,    0, 3000);
  h_htnojets      [ichannel][icut][ijet] = new TH1D("h_htnojets"       + suffix, "", 3000,    0, 3000);
  h_mc            [ichannel][icut][ijet] = new TH1D("h_mc"             + suffix, "", 3000,    0, 3000);
  h_metPfType1    [ichannel][icut][ijet] = new TH1D("h_metPfType1"     + suffix, "", 3000,    0, 3000);
  h_metTtrk       [ichannel][icut][ijet] = new TH1D("h_metTtrk"        + suffix, "", 3000,    0, 3000);
  h_mpmet         [ichannel][icut][ijet] = new TH1D("h_mpmet"          + suffix, "", 3000,    0, 3000);
  h_mth           [ichannel][icut][ijet] = new TH1D("h_mth"            + suffix, "", 3000,    0, 3000);
  h_mtw1          [ichannel][icut][ijet] = new TH1D("h_mtw1"           + suffix, "", 3000,    0, 3000);
  h_mtw2          [ichannel][icut][ijet] = new TH1D("h_mtw2"           + suffix, "", 3000,    0, 3000);
  h_m2l           [ichannel][icut][ijet] = new TH1D("h_m2l"            + suffix, "", 3000,    0, 3000);
  h_nbjet15csvv2l [ichannel][icut][ijet] = new TH1D("h_nbjet15csvv2l"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet15csvv2m [ichannel][icut][ijet] = new TH1D("h_nbjet15csvv2m"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet15csvv2t [ichannel][icut][ijet] = new TH1D("h_nbjet15csvv2t"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet30csvv2l [ichannel][icut][ijet] = new TH1D("h_nbjet30csvv2l"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet30csvv2m [ichannel][icut][ijet] = new TH1D("h_nbjet30csvv2m"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet30csvv2t [ichannel][icut][ijet] = new TH1D("h_nbjet30csvv2t"  + suffix, "",    7, -0.5,  6.5);
  h_nbjet20cmvav2l[ichannel][icut][ijet] = new TH1D("h_nbjet20cmvav2l" + suffix, "",    7, -0.5,  6.5);
  h_nbjet20cmvav2m[ichannel][icut][ijet] = new TH1D("h_nbjet20cmvav2m" + suffix, "",    7, -0.5,  6.5);
  h_nbjet20cmvav2t[ichannel][icut][ijet] = new TH1D("h_nbjet20cmvav2t" + suffix, "",    7, -0.5,  6.5);
  h_nbjet30cmvav2l[ichannel][icut][ijet] = new TH1D("h_nbjet30cmvav2l" + suffix, "",    7, -0.5,  6.5);
  h_nbjet30cmvav2m[ichannel][icut][ijet] = new TH1D("h_nbjet30cmvav2m" + suffix, "",    7, -0.5,  6.5);
  h_nbjet30cmvav2t[ichannel][icut][ijet] = new TH1D("h_nbjet30cmvav2t" + suffix, "",    7, -0.5,  6.5);
  h_njet          [ichannel][icut][ijet] = new TH1D("h_njet"           + suffix, "",    7, -0.5,  6.5);
  h_top1pt_gen    [ichannel][icut][ijet] = new TH1D("h_top1pt_gen"     + suffix, "", 3000,    0, 3000);
  h_top1eta_gen   [ichannel][icut][ijet] = new TH1D("h_top1eta_gen"    + suffix, "",   60,   -3,    3);  
  h_top1phi_gen   [ichannel][icut][ijet] = new TH1D("h_top1phi_gen"    + suffix, "",  200, -3.2,  3.2);
  h_top2pt_gen    [ichannel][icut][ijet] = new TH1D("h_top2pt_gen"     + suffix, "", 3000,    0, 3000);
  h_top2eta_gen   [ichannel][icut][ijet] = new TH1D("h_top2eta_gen"    + suffix, "",   60,   -3,    3);  
  h_top2phi_gen   [ichannel][icut][ijet] = new TH1D("h_top2phi_gen"    + suffix, "",  200, -3.2,  3.2);
  h_m2t_gen       [ichannel][icut][ijet] = new TH1D("h_m2t_gen"        + suffix, "", 3000,    0, 3000);
  h_dphitt_gen    [ichannel][icut][ijet] = new TH1D("h_dphitt_gen"     + suffix, "",  100,    0,  3.2);
  h_detatt_gen    [ichannel][icut][ijet] = new TH1D("h_detatt_gen"     + suffix, "",  100,    0,   10);
  h_topReco       [ichannel][icut][ijet] = new TH1D("h_topReco"        + suffix, "",   10,    0,   10);


  // TH2 histograms
  //----------------------------------------------------------------------------
  h_metPfType1_m2l[ichannel][icut][ijet] = new TH2D("h_metPfType1_m2l" + suffix, "", 100, 0,  100, 100, 40, 140);
  h_2ht           [ichannel][icut][ijet] = new TH2D("h_2ht"            + suffix, "", 300, 0,  800, 300,  0, 800);
  h_dym           [ichannel][icut][ijet] = new TH2D("h_dym"            + suffix, "", 200, 0, 1000, 100,  0,   5);
}


//------------------------------------------------------------------------------
// OpenMinitree
//------------------------------------------------------------------------------
void AnalysisCMS::OpenMinitree()
{
  if (!_saveminitree) return;

  gSystem->mkdir("minitrees/" + _systematic + "/" + _analysis, kTRUE);

  root_minitree = new TFile("minitrees/" + _systematic + "/" + _analysis + "/" + _isdatadriven + _sample + _dataperiod + ".root", "recreate");


  // Minitree branches
  //----------------------------------------------------------------------------
  minitree = new TTree("latino", "minitree");

  minitree->Branch("channel",        &_channel,        "channel/F");
  minitree->Branch("dphijet1met",    &_dphijet1met,    "dphijet1met/F");
  minitree->Branch("dphijet2met",    &_dphijet2met,    "dphijet2met/F");
  minitree->Branch("dphijj",         &_dphijj,         "dphijj/F");
  minitree->Branch("dphijjmet",      &_dphijjmet,      "dphijjmet/F");
  minitree->Branch("dphilep1jet1",   &_dphilep1jet1,   "dphilep1jet1/F");
  minitree->Branch("dphilep1jet2",   &_dphilep1jet2,   "dphilep1jet2/F");
  minitree->Branch("dphilep2jet1",   &_dphilep2jet1,   "dphilep2jet1/F");
  minitree->Branch("dphilep2jet2",   &_dphilep2jet2,   "dphilep2jet2/F");
  minitree->Branch("dphill",         &dphill,          "dphill/F" );
  minitree->Branch("dphillmet",      &_dphillmet,      "dphillmet/F");
  minitree->Branch("dphillstar",     &_dphillstar,     "dphillstar/F");
  minitree->Branch("dphilmet1",      &dphilmet1,       "dphilmet1/F");
  minitree->Branch("dphilmet2",      &dphilmet2,       "dphilmet2/F");
  minitree->Branch("drll",           &drll,            "drll/F");
  minitree->Branch("eventW",         &_event_weight,   "eventW/F");
  minitree->Branch("ht",             &_ht,             "ht/F");
  minitree->Branch("htjets",         &_htjets,         "htjets/F");
  minitree->Branch("htnojets",       &_htnojets,       "htnojets/F");
  minitree->Branch("jet1eta",        &jeteta1,         "jet1eta/F");
  minitree->Branch("jet1mass",       &jetmass1,        "jet1mass/F");
  minitree->Branch("jet1phi",        &jetphi1,         "jet1phi/F");
  minitree->Branch("jet1pt",         &jetpt1,          "jet1pt/F");
  minitree->Branch("jet2eta",        &jeteta2,         "jet2eta/F");
  minitree->Branch("jet2mass",       &jetmass2,        "jet2mass/F");
  minitree->Branch("jet2phi",        &jetphi2,         "jet2phi/F");
  minitree->Branch("jet2pt",         &jetpt2,          "jet2pt/F");
  minitree->Branch("lep1eta",        &_lep1eta,        "lep1eta/F");
  minitree->Branch("lep1phi",        &_lep1phi,        "lep1phi/F");
  minitree->Branch("lep1pt",         &_lep1pt,         "lep1pt/F");
  minitree->Branch("lep2eta",        &_lep2eta,        "lep2eta/F");
  minitree->Branch("lep2phi",        &_lep2phi,        "lep2phi/F");
  minitree->Branch("lep2pt",         &_lep2pt,         "lep2pt/F");
  minitree->Branch("mc",             &_mc,             "mc/F");
  minitree->Branch("m2l",            &_m2l,            "m2l/F");
  minitree->Branch("mpmet",          &_mpmet,          "mpmet/F");
  minitree->Branch("metPfType1",     &metPfType1,      "metPfType1/F");
  minitree->Branch("metPfType1Phi",  &metPfType1Phi,   "metPfType1Phi/F");
  minitree->Branch("metTtrk",        &metTtrk,         "metTtrk/F");
  minitree->Branch("metTtrkPhi",     &metTtrkPhi,      "metTtrkPhi/F");
  minitree->Branch("mth",            &mth,             "mth/F");
  minitree->Branch("mtw1",           &mtw1,            "mtw1/F");
  minitree->Branch("mtw2",           &mtw2,            "mtw2/F");
  minitree->Branch("nbjet15csvv2l",  &_nbjet15csvv2l,  "nbjet15csvv2l/F");
  minitree->Branch("nbjet15csvv2m",  &_nbjet15csvv2m,  "nbjet15csvv2m/F");
  minitree->Branch("nbjet15csvv2t",  &_nbjet15csvv2t,  "nbjet15csvv2t/F");
  minitree->Branch("nbjet30csvv2l",  &_nbjet30csvv2l,  "nbjet30csvv2l/F");
  minitree->Branch("nbjet30csvv2m",  &_nbjet30csvv2m,  "nbjet30csvv2m/F");
  minitree->Branch("nbjet30csvv2t",  &_nbjet30csvv2t,  "nbjet30csvv2t/F");
  minitree->Branch("nbjet20cmvav2l", &_nbjet20cmvav2l, "nbjet20cmvav2l/F");
  minitree->Branch("nbjet20cmvav2m", &_nbjet20cmvav2m, "nbjet20cmvav2m/F");
  minitree->Branch("nbjet20cmvav2t", &_nbjet20cmvav2t, "nbjet20cmvav2t/F");
  minitree->Branch("nbjet30cmvav2l", &_nbjet30cmvav2l, "nbjet30cmvav2l/F");
  minitree->Branch("nbjet30cmvav2m", &_nbjet30cmvav2m, "nbjet30cmvav2m/F");
  minitree->Branch("nbjet30cmvav2t", &_nbjet30cmvav2t, "nbjet30cmvav2t/F");
  minitree->Branch("njet",           &_njet,           "njet/F");

  if (std_vector_LHE_weight)
    minitree->Branch("LHEweight", &std_vector_LHE_weight);

  // Stop variables
  minitree->Branch("dyll",            &_dyll,            "dyll/F");
  minitree->Branch("ptbll",           &_ptbll,           "ptbll/F");
  minitree->Branch("dphimetptbll",    &_dphimetptbll,    "dphimetptbll/F");
  minitree->Branch("mt2ll",           &_mt2ll,           "mt2ll/F");
  minitree->Branch("dphimetjet",      &_dphimetjet,      "dphimetjet/F");
  minitree->Branch("mllbb",           &_mllbb,           "mllbb/F");
  minitree->Branch("meff",            &_meff,            "meff/F");
  minitree->Branch("mt2bb",           &_mt2bb,           "mt2bb/F");
  minitree->Branch("mt2lblb",         &_mt2lblb,         "mt2lblb/F");
  minitree->Branch("mlb1",            &_mlb1,            "mlb1/F");
  minitree->Branch("mlb2",            &_mlb2,            "mlb2/F");
  minitree->Branch("mt2lblbcomb",     &_mt2lblbcomb,     "mt2lblbcomb/F");
  minitree->Branch("mt2bbtrue",       &_mt2bbtrue,       "mt2bbtrue/F");
  minitree->Branch("mt2lblbtrue",     &_mt2lblbtrue,     "mt2lblbtrue/F");
  minitree->Branch("mt2lblbmatch",    &_mt2lblbmatch,    "mt2lblbmatch/F");
  minitree->Branch("mlb1comb",        &_mlb1comb,        "mlb1comb/F");
  minitree->Branch("mlb2comb",        &_mlb2comb,        "mlb2comb/F");
  minitree->Branch("mlb1true",        &_mlb1true,        "mlb1true/F");
  minitree->Branch("mlb2true",        &_mlb2true,        "mlb2true/F");
  minitree->Branch("bjet1pt",         &_bjet1pt,         "bjet1pt/F");
  minitree->Branch("bjet1eta",        &_bjet1eta,        "bjet1eta/F");
  minitree->Branch("bjet1phi",        &_bjet1phi,        "bjet1phi/F");
  minitree->Branch("bjet1mass",       &_bjet1mass,       "bjet1mass/F");
  minitree->Branch("bjet1csvv2ivf",   &_bjet1csvv2ivf,   "bjet1csvv2ivf/F");
  minitree->Branch("bjet2pt",         &_bjet2pt,         "bjet2pt/F");
  minitree->Branch("bjet2eta",        &_bjet2eta,        "bjet2eta/F");
  minitree->Branch("bjet2phi",        &_bjet2phi,        "bjet2phi/F");
  minitree->Branch("bjet2mass",       &_bjet2mass,       "bjet2mass/F");
  minitree->Branch("bjet2csvv2ivf",   &_bjet2csvv2ivf,   "bjet2csvv2ivf/F");
  minitree->Branch("tjet1pt",         &_tjet1pt,         "tjet1pt/F");
  minitree->Branch("tjet1eta",        &_tjet1eta,        "tjet1eta/F");
  minitree->Branch("tjet1phi",        &_tjet1phi,        "tjet1phi/F");
  minitree->Branch("tjet1mass",       &_tjet1mass,       "tjet1mass/F");
  minitree->Branch("tjet1csvv2ivf",   &_tjet1csvv2ivf,   "tjet1csvv2ivf/F");
  minitree->Branch("tjet1assignment", &_tjet1assignment, "tjet1assignment/F");
  minitree->Branch("tjet2pt",         &_tjet2pt,         "tjet2pt/F");
  minitree->Branch("tjet2eta",        &_tjet2eta,        "tjet2eta/F");
  minitree->Branch("tjet2phi",        &_tjet2phi,        "tjet2phi/F");
  minitree->Branch("tjet2mass",       &_tjet2mass,       "tjet2mass/F");
  minitree->Branch("tjet2csvv2ivf",   &_tjet2csvv2ivf,   "tjet2csvv2ivf/F");
  minitree->Branch("tjet2assignment", &_tjet2assignment, "tjet2assignment/F");
}


//------------------------------------------------------------------------------
// GetGenPtllWeight
//------------------------------------------------------------------------------
void AnalysisCMS::GetGenPtllWeight()
{
  _gen_ptll_weight = 1.0;

  if (!_sample.Contains("DYJetsToLL_M")) return;

  _gen_ptll_weight = 1.08683 * (0.95 - 0.0657370*TMath::Erf((gen_ptll-12.5151)/5.51582));
}


//------------------------------------------------------------------------------
// GetGenWeightsLHE
// https://github.com/latinos/LatinoTrees/blob/master/AnalysisStep/src/WeightDumper.cc#L157
//------------------------------------------------------------------------------
TH1F* AnalysisCMS::GetGenWeightsLHE()
{
  TFile* file = TFile::Open(_filename);

  TH1F* dummy = (TH1F*)file->Get("list_vectors_weights");

  root_output->cd();

  if (!dummy) return NULL;

  TH1F* hist = (TH1F*)dummy->Clone("list_vectors_weights_gen");

  return hist;
}


//------------------------------------------------------------------------------
// GetRecoWeightsLHE
// https://github.com/latinos/LatinoTrees/blob/master/AnalysisStep/src/WeightDumper.cc#L157
//------------------------------------------------------------------------------
void AnalysisCMS::GetRecoWeightsLHE(TH1F* hist)
{
  if (!std_vector_LHE_weight) return;

  for (int iWeight=0; iWeight<hist->GetNbinsX(); iWeight++)
    {
      float ratio = std_vector_LHE_weight->at(iWeight) / std_vector_LHE_weight->at(0);

      hist->Fill(iWeight+0.5, ratio);
    }
}


//------------------------------------------------------------------------------                                                            
// GetDeltaR
//------------------------------------------------------------------------------                                                
void AnalysisCMS::GetDeltaR()
{
  // Reset variables
  //----------------------------------------------------------------------------                                 
  _deltarjet1met  = -0.1;
  _deltarjet2met  = -0.1;
  _deltarjj       = -0.1;
  _deltarjjmet    = -0.1;
  _deltarlep1jet1 = -0.1;
  _deltarlep1jet2 = -0.1;
  _deltarlep2jet1 = -0.1;
  _deltarlep2jet2 = -0.1;
  _deltarllmet    = -0.1;
  _deltarl1met    = -0.1;
  _deltarl2met    = -0.1;


  // Fill variables
  //----------------------------------------------------------------------------                                  
  _deltarllmet = fabs((Lepton1.v + Lepton2.v).DeltaR(MET));
  _deltarl1met = fabs(Lepton1.v.DeltaR(MET));
  _deltarl2met = fabs(Lepton2.v.DeltaR(MET));

  if (_njet > 0)
    {
      _deltarjet1met  = fabs(AnalysisJets[0].v.DeltaR(MET));
      _deltarlep1jet1 = fabs(Lepton1.v.DeltaR(AnalysisJets[0].v));
      _deltarlep2jet1 = fabs(Lepton2.v.DeltaR(AnalysisJets[0].v));
    }

  if (_njet > 1)
    {
      _deltarjet2met  = fabs(AnalysisJets[1].v.DeltaR(MET));
      _deltarjj       = fabs(AnalysisJets[0].v.DeltaR(AnalysisJets[1].v));
      _deltarjjmet    = fabs((AnalysisJets[0].v + AnalysisJets[1].v).DeltaR(MET));
      _deltarlep1jet2 = fabs(Lepton1.v.DeltaR(AnalysisJets[1].v));
      _deltarlep2jet2 = fabs(Lepton2.v.DeltaR(AnalysisJets[1].v));
    }
}


//------------------------------------------------------------------------------
// ComputeMT2
//------------------------------------------------------------------------------
double AnalysisCMS::ComputeMT2(TLorentzVector VisibleA,
			       TLorentzVector VisibleB, 
			       TLorentzVector Invisible,
			       int            MT2Type,
			       double         MT2Precision) 
{
  double mVisA = fabs(VisibleA.M());  // Mass of visible object on side A. Must be >= 0
  double mVisB = fabs(VisibleB.M());  // Mass of visible object on side B. Must be >= 0

  double chiA = 0.;  // Hypothesised mass of invisible on side A. Must be >= 0
  double chiB = 0.;  // Hypothesised mass of invisible on side B. Must be >= 0
  
  if (MT2Type == 1)
    {
      mVisA =  5.;
      mVisB =  5.;
      chiA  = 80.;
      chiB  = 80.;
    }

  double pxA = VisibleA.Px();  // x momentum of visible object on side A
  double pyA = VisibleA.Py();  // y momentum of visible object on side A
  
  double pxB = VisibleB.Px();  // x momentum of visible object on side B
  double pyB = VisibleB.Py();  // y momentum of visible object on side B
  
  double pxMiss = Invisible.Px();  // x component of missing transverse momentum
  double pyMiss = Invisible.Py();  // y component of missing transverse momentum
  
  // Must be >= 0
  // If = 0 algorithm aims for machine precision
  // If > 0 MT2 computed to supplied absolute precision
  double desiredPrecisionOnMt2 = MT2Precision;
  
  //  asymm_mt2_lester_bisect::disableCopyrightMessage();
  
  double MT2 = asymm_mt2_lester_bisect::get_mT2(mVisA, pxA, pyA,
						mVisB, pxB, pyB,
						pxMiss, pyMiss,
						chiA, chiB,
						desiredPrecisionOnMt2);

  return MT2;
}


//------------------------------------------------------------------------------
// GetStopVar
//------------------------------------------------------------------------------
void AnalysisCMS::GetStopVar()
{
  _dyll         = fabs(Lepton1.v.Eta() - Lepton2.v.Eta());  // Should be rapidity?
  _ptbll        = (Lepton1.v + Lepton2.v + MET).Pt();
  _dphimetptbll = fabs((Lepton1.v + Lepton2.v + MET).DeltaPhi(MET));
  _mt2ll        = ComputeMT2(Lepton1.v, Lepton2.v, MET);

  _dphimetjet  = -0.1;
  _mllbb       = -0.1;
  _meff        = -0.1;
  _mt2bb       = -0.1;
  _mt2lblb     = -0.1;
  _mlb1        = -0.1;
  _mlb2        = -0.1;

  _mt2lblbcomb  = -0.1;
  _mt2bbtrue    = -0.1;
  _mt2lblbtrue  = -0.1;
  _mt2lblbmatch = -0.1;

  _mlb1comb     = -0.1;
  _mlb2comb     = -0.1;
  _mlb1true     = -0.1;
  _mlb2true     = -0.1;
  
  _bjet1pt         = _bjet2pt         = _tjet1pt       = _tjet2pt       =  -10.;
  _bjet1eta        = _bjet2eta        = _tjet1eta      = _tjet2eta      = -999.;
  _bjet1phi        = _bjet2phi        = _tjet1phi      = _tjet2phi      = -999.;
  _bjet1mass       = _bjet2mass       = _tjet1mass     = _tjet2mass     = -999.;
  _bjet1csvv2ivf   = _bjet2csvv2ivf   = _tjet1csvv2ivf = _tjet2csvv2ivf = -999.;
  _tjet1assignment = _tjet2assignment = 0.;

  double minDeltaPhiMetJet = 999.;
  
  for (int ijet=0; ijet<_njet; ijet++) {
      
    double thisDeltaPhiMetJet = fabs(AnalysisJets[ijet].v.DeltaPhi(MET));

    if (thisDeltaPhiMetJet < minDeltaPhiMetJet) {
	
      minDeltaPhiMetJet = thisDeltaPhiMetJet;

      _dphimetjet = thisDeltaPhiMetJet;
    }
  }
    
  if (_njet >= 2) {
      
    _meff = MET.Pt() + Lepton1.v.Pt() + Lepton2.v.Pt() + AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt();

    int bjetindex[2] = {-1, -1};

    if (_nbjet30csvv2m >= 1) {
	
      int nbjetfound       = 0;
      int nbjetfromleading = 0;

      for (int ijet=0; ijet<_njet; ijet++) {
	if (nbjetfound < 2) {
	  if (AnalysisJets[ijet].csvv2ivf > CSVv2M) {
	    bjetindex[1] = bjetindex[0];
	    bjetindex[nbjetfound] = ijet;
	    nbjetfound++;
	  } else if (nbjetfromleading < 1) {
	    bjetindex[nbjetfound] = ijet;
	    nbjetfromleading++;
	  }  
	}
      }

      if (bjetindex[0] >= 0 && bjetindex[1] >= 0) {
	
	_mllbb = (Lepton1.v + Lepton2.v + AnalysisJets[bjetindex[0]].v + AnalysisJets[bjetindex[1]].v).M();

	_mt2bb = ComputeMT2(AnalysisJets[bjetindex[0]].v, AnalysisJets[bjetindex[1]].v, Lepton1.v + Lepton2.v + MET, 1);
	    
	_mt2lblb = ComputeMT2(AnalysisJets[bjetindex[0]].v + Lepton1.v, AnalysisJets[bjetindex[1]].v + Lepton2.v, MET);

	double combinatorialMT2lblb = ComputeMT2(AnalysisJets[bjetindex[0]].v + Lepton2.v, AnalysisJets[bjetindex[1]].v + Lepton1.v, MET, 2);
	
	if (combinatorialMT2lblb < _mt2lblb) { 
	 
	  _mt2lblbcomb = _mt2lblb;
	  _mt2lblb     = combinatorialMT2lblb;

	  _tjet1pt       = AnalysisJets[bjetindex[1]].v.Pt();
	  _tjet1eta      = AnalysisJets[bjetindex[1]].v.Eta();
	  _tjet1phi      = AnalysisJets[bjetindex[1]].v.Phi();
	  _tjet1mass     = std_vector_jet_mass->at(AnalysisJets[bjetindex[1]].index);
	  _tjet1csvv2ivf = AnalysisJets[bjetindex[1]].csvv2ivf;

	  _tjet2pt       = AnalysisJets[bjetindex[0]].v.Pt();
	  _tjet2eta      = AnalysisJets[bjetindex[0]].v.Eta();
	  _tjet2phi      = AnalysisJets[bjetindex[0]].v.Phi();
	  _tjet2mass     = std_vector_jet_mass->at(AnalysisJets[bjetindex[0]].index);
	  _tjet2csvv2ivf = AnalysisJets[bjetindex[0]].csvv2ivf;

	  _mlb1 = (AnalysisJets[bjetindex[1]].v + Lepton1.v).M();
	  _mlb2 = (AnalysisJets[bjetindex[0]].v + Lepton2.v).M();

	  _mlb1comb = (AnalysisJets[bjetindex[0]].v + Lepton1.v).M();
	  _mlb2comb = (AnalysisJets[bjetindex[1]].v + Lepton2.v).M();

	} else {

	  _mt2lblbcomb = combinatorialMT2lblb;

	  _tjet1pt = AnalysisJets[bjetindex[0]].v.Pt();
	  _tjet1eta = AnalysisJets[bjetindex[0]].v.Eta();
	  _tjet1phi = AnalysisJets[bjetindex[0]].v.Phi();
	  _tjet1mass = std_vector_jet_mass->at(AnalysisJets[bjetindex[0]].index);
	  _tjet1csvv2ivf = AnalysisJets[bjetindex[0]].csvv2ivf;

	  _tjet2pt = AnalysisJets[bjetindex[1]].v.Pt();
	  _tjet2eta = AnalysisJets[bjetindex[1]].v.Eta();
	  _tjet2phi = AnalysisJets[bjetindex[1]].v.Phi();
	  _tjet2mass = std_vector_jet_mass->at(AnalysisJets[bjetindex[1]].index);
	  _tjet2csvv2ivf = AnalysisJets[bjetindex[1]].csvv2ivf;

	  _mlb1 = (AnalysisJets[bjetindex[0]].v + Lepton1.v).M();
	  _mlb2 = (AnalysisJets[bjetindex[1]].v + Lepton2.v).M();

	  _mlb1comb = (AnalysisJets[bjetindex[1]].v + Lepton1.v).M();
	  _mlb2comb = (AnalysisJets[bjetindex[0]].v + Lepton2.v).M();
	}
      }
    }
  }

  if (!_analysis.EqualTo("Stop")) return;

  if (!_ismc) return;

  // Top quark reco
  int lepIndex[2] = {-999, -999}, bIndex[2] = {-999, -999};
  int nCandidateBJets = 0;
  int CandidateBJetIndex[50];
  float CandidateBDeltaTopMass[50][2];
  for (int cb = 0; cb<50; cb++) {
    for (int iw = 0; iw<2; iw++) {
      CandidateBJetIndex[cb] = -1;
      CandidateBDeltaTopMass[cb][iw] = 999.;
    }
  }

  for (int wb = std_vector_VBoson_pt->size()-1; wb>=0; wb--) {
    if (std_vector_VBoson_pt->at(wb)>-999.) {

      int Wid = std_vector_VBoson_pid->at(wb);
      int IdxW = (Wid+24)/48;

      if (lepIndex[IdxW]==-999) {
	
	TLorentzVector WBoson; 
	WBoson.SetPtEtaPhiM(std_vector_VBoson_pt->at(wb), std_vector_VBoson_eta->at(wb), std_vector_VBoson_phi->at(wb), std_vector_VBoson_mass->at(wb));

	for (int lp = 0; lp<std_vector_leptonGen_pt->size(); lp++) {
	  if (std_vector_leptonGen_pt->at(lp)>-999. && Wid*std_vector_leptonGen_pid->at(lp)<0 && lepIndex[IdxW]<0) {

	    float LeptonMass = 0.000511;
	    if (fabs(std_vector_leptonGen_pid->at(lp))==13) LeptonMass = 0.1056583715;

	    TLorentzVector ChargedLepton;
	    ChargedLepton.SetPtEtaPhiM(std_vector_leptonGen_pt->at(lp), std_vector_leptonGen_eta->at(lp), std_vector_leptonGen_phi->at(lp), LeptonMass);
	    
	    for (int nt = 0; nt<std_vector_neutrinoGen_pt->size(); nt++) {
	      if (std_vector_neutrinoGen_pt->at(nt)>-999. && lepIndex[IdxW]<0) {
		
		TLorentzVector CandidateNeutrino;
		CandidateNeutrino.SetPtEtaPhiM(std_vector_neutrinoGen_pt->at(nt), std_vector_neutrinoGen_eta->at(nt), std_vector_neutrinoGen_phi->at(nt), 0.);

		float ThisDeltaR = WBoson.DeltaR(ChargedLepton+CandidateNeutrino);
		if (ThisDeltaR<0.00001) {

		  lepIndex[IdxW] = lp;
		  
		  //cout << " Now look for the b quark (no partons in 74X)" << endl;
		  for (int rj = 0; rj<_njet; rj++) {

		    if (fabs(std_vector_jet_HadronFlavour->at(AnalysisJets[rj].index))==5 && 
			(fabs(std_vector_jet_PartonFlavour->at(AnalysisJets[rj].index))!=5 || 
			 std_vector_jet_PartonFlavour->at(AnalysisJets[rj].index)*Wid>0) ) {
		      for (int gj = 0; gj<std_vector_jetGen_pt->size(); gj++){
			
			if (std_vector_jetGen_pt->at(gj)>8.) {
		      
			  TLorentzVector BottomQuark;
			  BottomQuark.SetPtEtaPhiM(std_vector_jetGen_pt->at(gj), std_vector_jetGen_eta->at(gj), std_vector_jetGen_phi->at(gj), 4.18);
			  float TopMass = (WBoson+BottomQuark).M();
			  float DeltaTopMass = fabs(TopMass-173.34);
			  
			  if (DeltaTopMass<250. && (AnalysisJets[rj].v).DeltaR(BottomQuark)<0.3) {
			    
			    bool NewCandidateJet = true;
			    for (int cb = 0; cb<nCandidateBJets; cb++) {
			      if (CandidateBJetIndex[cb]==rj) {

				CandidateBDeltaTopMass[cb][IdxW] = DeltaTopMass;
				NewCandidateJet = false;

			      }
			    }
			    
			    if (NewCandidateJet) {

			      CandidateBJetIndex[nCandidateBJets] = rj;
			      CandidateBDeltaTopMass[nCandidateBJets][IdxW] = DeltaTopMass;
			      nCandidateBJets++;
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }


  float MinMassDistance = 999999.;
  for (int b0 = 0; b0<nCandidateBJets; b0++) {
    for (int b1 = 0; b1<nCandidateBJets; b1++) {
      if (b0!=b1) {

	float MassDistance = sqrt( pow(CandidateBDeltaTopMass[b0][0], 2) +
				   pow(CandidateBDeltaTopMass[b1][1], 2));

	if (MassDistance<MinMassDistance) {
	  
	  bIndex[0] = CandidateBJetIndex[b0];
	  bIndex[1] = CandidateBJetIndex[b1];
	  MinMassDistance = MassDistance;

	}
	
      } else if (nCandidateBJets==1) {
	
	int giw = -1;
	for (int iw = 0; iw<2; iw++) {
	  if (CandidateBDeltaTopMass[b0][iw]<MinMassDistance) {
	    
	    giw = iw;
	    MinMassDistance = CandidateBDeltaTopMass[b0][iw];

	  }
	}

	if (giw>=0) bIndex[giw] = CandidateBJetIndex[b0];
	
      }
    }
  }

  int IdxB1 = -999, IdxB2 = -999;

  if (lepIndex[0]>=0) {

    TLorentzVector LepGen1;
    LepGen1.SetPtEtaPhiM(std_vector_leptonGen_pt->at(lepIndex[0]), std_vector_leptonGen_eta->at(lepIndex[0]), std_vector_leptonGen_phi->at(lepIndex[0]), 0.1); // Mass does not matter here
    
    float DeltaRLep1LepGen1 = (Lepton1.v).DeltaR(LepGen1);
    float DeltaRLep2LepGen1 = (Lepton2.v).DeltaR(LepGen1);

    if (std_vector_lepton_ch->at(Lepton1.index)<0 && DeltaRLep1LepGen1<0.1) {
      if (bIndex[0]>=0) {

	_bjet1pt = AnalysisJets[bIndex[0]].v.Pt();
	_bjet1eta = AnalysisJets[bIndex[0]].v.Eta();
	_bjet1phi = AnalysisJets[bIndex[0]].v.Phi();
	_bjet1mass = std_vector_jet_mass->at(AnalysisJets[bIndex[0]].index);
	_bjet1csvv2ivf = AnalysisJets[bIndex[0]].csvv2ivf;
	IdxB1 = bIndex[0];

	if (_tjet1pt==_bjet1pt) _tjet1assignment = 2;
	if (_tjet2pt==_bjet1pt) _tjet2assignment = 1;

	_mlb1true = (AnalysisJets[IdxB1].v + Lepton1.v).M();

      }
    }

    if (std_vector_lepton_ch->at(Lepton2.index)<0 && DeltaRLep2LepGen1<0.1) {
      if (bIndex[0]>=0) {

	_bjet2pt = AnalysisJets[bIndex[0]].v.Pt();
	_bjet2eta = AnalysisJets[bIndex[0]].v.Eta();
	_bjet2phi = AnalysisJets[bIndex[0]].v.Phi();
	_bjet2mass = std_vector_jet_mass->at(AnalysisJets[bIndex[0]].index);
	_bjet2csvv2ivf = AnalysisJets[bIndex[0]].csvv2ivf;	
	IdxB2 = bIndex[0];

	if (_tjet1pt==_bjet2pt) _tjet1assignment = 1;
	if (_tjet2pt==_bjet2pt) _tjet2assignment = 2;

	_mlb2true = (AnalysisJets[IdxB2].v + Lepton2.v).M();
	
      }
    }

  }

  if (lepIndex[1]>=0) {
    
    TLorentzVector LepGen2;
    LepGen2.SetPtEtaPhiM(std_vector_leptonGen_pt->at(lepIndex[1]), std_vector_leptonGen_eta->at(lepIndex[1]), std_vector_leptonGen_phi->at(lepIndex[1]), 0.1); // Mass does not matter here
    
    float DeltaRLep1LepGen2 = (Lepton1.v).DeltaR(LepGen2);
    float DeltaRLep2LepGen2 = (Lepton2.v).DeltaR(LepGen2);
    
    if (std_vector_lepton_ch->at(Lepton1.index)>0 && DeltaRLep1LepGen2<0.1) {
      if (bIndex[1]>=0) {

	_bjet1pt = AnalysisJets[bIndex[1]].v.Pt();
	_bjet1eta = AnalysisJets[bIndex[1]].v.Eta();
	_bjet1phi = AnalysisJets[bIndex[1]].v.Phi();
	_bjet1mass = std_vector_jet_mass->at(AnalysisJets[bIndex[1]].index);
	_bjet1csvv2ivf = AnalysisJets[bIndex[1]].csvv2ivf;
	IdxB1 = bIndex[1];

	if (_tjet1pt==_bjet1pt) _tjet1assignment = 2;
	if (_tjet2pt==_bjet1pt) _tjet2assignment = 1;

	_mlb1true = (AnalysisJets[IdxB1].v + Lepton1.v).M();
	
      }
    }
    
    if (std_vector_lepton_ch->at(Lepton2.index)>0 && DeltaRLep2LepGen2<0.1) {
      if (bIndex[1]>=0) {

	_bjet2pt = AnalysisJets[bIndex[1]].v.Pt();
	_bjet2eta = AnalysisJets[bIndex[1]].v.Eta();
	_bjet2phi = AnalysisJets[bIndex[1]].v.Phi();
	_bjet2mass = std_vector_jet_mass->at(AnalysisJets[bIndex[1]].index);
	_bjet2csvv2ivf = AnalysisJets[bIndex[1]].csvv2ivf;	
	IdxB2 = bIndex[1];

	if (_tjet1pt==_bjet2pt) _tjet1assignment = 1;
	if (_tjet2pt==_bjet2pt) _tjet2assignment = 2;

	_mlb2true = (AnalysisJets[IdxB2].v + Lepton2.v).M();
	
      }
    }
    
  }

  if (IdxB1>=0 && IdxB2>=0) {

    _mt2bbtrue = ComputeMT2(AnalysisJets[IdxB1].v, AnalysisJets[IdxB2].v, Lepton1.v + Lepton2.v + MET, 1);
    _mt2lblbtrue = ComputeMT2(AnalysisJets[IdxB1].v + Lepton1.v, AnalysisJets[IdxB2].v + Lepton2.v, MET, 2);

    _mt2lblbmatch = ComputeMT2(AnalysisJets[IdxB2].v + Lepton1.v, AnalysisJets[IdxB1].v + Lepton2.v, MET, 2);
  }
}


//------------------------------------------------------------------------------
// GetTops
//------------------------------------------------------------------------------
void AnalysisCMS::GetTops()
{
  _top1eta_gen = -999;
  _top1phi_gen = -999;
  _top1pt_gen  = -999;
  _top2eta_gen = -999;
  _top2phi_gen = -999;
  _top2pt_gen  = -999;
  _m2t_gen     = -999; 
  _dphitt_gen  = -999;
  _detatt_gen  = -999;

  if (!_ismc) return;

  int ntop_pairs = 0;

  for (int i=0; i<std_vector_partonGen_pt->size(); i++) {

    if (std_vector_partonGen_pid->at(i)           != 6) continue;
    if (std_vector_partonGen_isHardProcess->at(i) != 1) continue;
    
    _top1eta_gen = std_vector_partonGen_eta->at(i);
    _top1phi_gen = std_vector_partonGen_phi->at(i);
    _top1pt_gen  = std_vector_partonGen_pt ->at(i);

    for (int j=i+1; j<std_vector_partonGen_pt->size(); j++) {

      if (std_vector_partonGen_pid->at(j)           != 6) continue;
      if (std_vector_partonGen_isHardProcess->at(j) != 1) continue;

      _top2eta_gen = std_vector_partonGen_eta->at(j);
      _top2phi_gen = std_vector_partonGen_phi->at(j);
      _top2pt_gen  = std_vector_partonGen_pt ->at(j);

      if (fabs(_top1eta_gen - _top2eta_gen) < 0.5) continue;
      if (fabs(_top1phi_gen - _top2phi_gen) < 0.2) continue;

      ntop_pairs++;

      TLorentzVector top1, top2; 

      top1.SetPtEtaPhiM(_top1pt_gen, _top1eta_gen, _top1phi_gen, 173.);
      top2.SetPtEtaPhiM(_top2pt_gen, _top2eta_gen, _top2phi_gen, 173.);

      _m2t_gen    = (top1 + top2).M();  
      _dphitt_gen = fabs(top1.DeltaPhi(top2));
      _detatt_gen = fabs(top1.Eta() - top2.Eta());
    }
  }

  if (ntop_pairs > 1)
    printf("\n [AnalysisCMS::GetTops] Warning, ntop_pairs = %d\n\n", ntop_pairs);
}


//------------------------------------------------------------------------------
// GetTopReco
//------------------------------------------------------------------------------
void AnalysisCMS::GetTopReco()
{
  MassVariations theMass;

  std::vector<TLorentzVector> myjets, nu1, nu2;
  std::vector<Float_t> unc;

  TVector2 myMET(metPfType1, metPfType1Phi);

  for (int i=0; i<AnalysisJets.size(); i++) {

    myjets.push_back(AnalysisJets.at(i).v);

    unc.push_back(0.05);
  }

  theMass.performAllVariations(1, 1, 1, Lepton1.v, Lepton2.v, myjets, unc, myMET, nu1, nu2);

  _topReco = nu1.size();
}
