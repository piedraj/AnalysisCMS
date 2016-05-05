#define AnalysisCMS_cxx
#include "../include/AnalysisCMS.h"


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

  _systematic = systematic;
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

  if      (fabs(flavour) == ELECTRON_FLAVOUR) is_isolated_lepton = true;
  else if (fabs(flavour) == MUON_FLAVOUR)     is_isolated_lepton = (MuonIsolation(k) < 0.15);
  
  return is_isolated_lepton;
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
  h_lep1eta       [ichannel][icut][ijet]->Fill(_lep1eta,        _event_weight);
  h_lep1phi       [ichannel][icut][ijet]->Fill(_lep1phi,        _event_weight);
  h_lep1pt        [ichannel][icut][ijet]->Fill(_lep1pt,         _event_weight);
  h_lep2eta       [ichannel][icut][ijet]->Fill(_lep2eta,        _event_weight);
  h_lep2phi       [ichannel][icut][ijet]->Fill(_lep2phi,        _event_weight);
  h_lep2pt        [ichannel][icut][ijet]->Fill(_lep2pt,         _event_weight);
  h_mc            [ichannel][icut][ijet]->Fill(_mc,             _event_weight);
  h_metPfType1    [ichannel][icut][ijet]->Fill(metPfType1,      _event_weight);
  h_metPfType1Phi [ichannel][icut][ijet]->Fill(metPfType1Phi,   _event_weight);
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


  // TH2 histograms
  //----------------------------------------------------------------------------
  h_metPfType1_m2l[ichannel][icut][ijet]->Fill(metPfType1, _m2l, _event_weight);


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

  if (title.Contains("raw") && (_ismc || _sample.Contains("DD_")))
    Summary(analysis, "11.2", "predicted yields");
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
  
  TString tok;

  Ssiz_t from = 0;

  while (_filename.Tokenize(tok, from, "latino_")) {

    if (tok.Contains(".root")) {

      _sample = tok.ReplaceAll(".root", "");
    }
  }

  printf("\n");
  printf("   analysis: %s\n",        _analysis.Data());
  printf("   filename: %s\n",        _filename.Data());
  printf("     sample: %s\n",        _sample.Data());
  printf(" luminosity: %.3f fb-1\n", _luminosity);
  printf("   nentries: %lld\n",      _nentries);

  if (_sample.Contains("DoubleEG"))       _ismc = false;
  if (_sample.Contains("DoubleMuon"))     _ismc = false;
  if (_sample.Contains("MuonEG"))         _ismc = false;
  if (_sample.Contains("SingleElectron")) _ismc = false;
  if (_sample.Contains("SingleMuon"))     _ismc = false;
  
  gSystem->mkdir("rootfiles/" + _systematic + "/" + _analysis, kTRUE);
  gSystem->mkdir("txt/"       + _systematic + "/" + _analysis, kTRUE);

  root_output = new TFile("rootfiles/" + _systematic + "/" + _analysis + "/" + _sample + ".root", "recreate");

  if (_eventdump) txt_eventdump.open("txt/" + _systematic + "/" + _analysis + "/" + _sample + "_eventdump.txt");

  OpenMinitree();

  // Histograms for QCD, PDF and alpha_s uncertainties
  //----------------------------------------------------------------------------
  root_output->cd();

  list_vectors_weights_0jet = new TH1F("list_vectors_weights_0jet", "", 200, 0, 200);
  list_vectors_weights_1jet = new TH1F("list_vectors_weights_1jet", "", 200, 0, 200);

  return;
}


//------------------------------------------------------------------------------
// ApplyWeights
//------------------------------------------------------------------------------
void AnalysisCMS::ApplyWeights()
{
  _event_weight = trigger * metFilter;

  if (!_ismc && _sample.Contains("DD_")) _event_weight *= _fake_weight;
    
  if (!_ismc) return;

   _event_weight *= _luminosity * baseW * puW;  // Default weights

  // Includes btag, trigger and idiso systematic uncertainties
  //----------------------------------------------------------------------------
  if (std_vector_lepton_idisoW)
    {
      float sf_btag    = bPogSF;
      float sf_trigger = effTrigW; // To be updated for WZ
      float sf_idiso   = std_vector_lepton_idisoW->at(0) * std_vector_lepton_idisoW->at(1);

      if (_systematic_btag_up) sf_btag = bPogSFUp;
      if (_systematic_btag_do) sf_btag = bPogSFDown;

      if (_systematic_idiso_up) sf_idiso = std_vector_lepton_idisoW_Up->at(0)   * std_vector_lepton_idisoW_Up->at(1);
      if (_systematic_idiso_do) sf_idiso = std_vector_lepton_idisoW_Down->at(0) * std_vector_lepton_idisoW_Down->at(1);

      if (_systematic_trigger_up) sf_trigger = effTrigW_Up;
      if (_systematic_trigger_do) sf_trigger = effTrigW_Down;

      if (_analysis.EqualTo("WZ"))
	{
	  sf_idiso = std_vector_lepton_idisoW->at(0) * std_vector_lepton_idisoW->at(1) * std_vector_lepton_idisoW->at(2);

	  if (_systematic_idiso_up) sf_idiso = std_vector_lepton_idisoW_Up->at(0)   * std_vector_lepton_idisoW_Up->at(1)   * std_vector_lepton_idisoW_Up->at(2);
	  if (_systematic_idiso_do) sf_idiso = std_vector_lepton_idisoW_Down->at(0) * std_vector_lepton_idisoW_Down->at(1) * std_vector_lepton_idisoW_Down->at(2);
	}

      _event_weight *= sf_btag * sf_trigger * sf_idiso;

    }
  
  if (_sample.EqualTo("Wg_AMCNLOFXFX")) _event_weight *= 1.23;
  if (_sample.EqualTo("WWTo2L2Nu"))     _event_weight *= nllW;

  _event_weight *= _gen_ptll_weight;

  if (!GEN_weight_SM) return;
  
  _event_weight *= GEN_weight_SM / abs(GEN_weight_SM);

  return;
}


//------------------------------------------------------------------------------
// GetLeptons
//
// Leptons have pt >= 8 GeV after l2Sel
// Leptons are tight after l2Sel
//
//------------------------------------------------------------------------------
void AnalysisCMS::GetLeptons()
{
  bool found_third_tight_lepton = false;

  AnalysisLeptons.clear();
  
  int vector_lepton_size = std_vector_lepton_pt->size();

  for (int i=0; i<vector_lepton_size; i++) {

    float eta     = std_vector_lepton_eta->at(i);
    float flavour = std_vector_lepton_flavour->at(i);
    float phi     = std_vector_lepton_phi->at(i);
    float pt      = std_vector_lepton_pt->at(i);
    float type    = std_vector_lepton_isTightLepton->at(i);

    //    if (std_vector_lepton_isLooseLepton -> at(i) != 1) continue;

    if (pt < 0.) continue;

    bool reject_lepton = false;
    
    if (i > 1 && !_sample.Contains("DD_") && _analysis.EqualTo("WZ"))
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
    goodjet.cmvav2   = std_vector_jet_cmvav2  ->at(i);
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
  if (_njet < 2)
    _sumjpt12 = -999;
  else
    _sumjpt12 = AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt();
}


//------------------------------------------------------------------------------
// EventDump
//------------------------------------------------------------------------------
void AnalysisCMS::EventDump()
{
  for (int i=0; i<_nlepton; i++)
    {
      int index = AnalysisLeptons[i].index;

      txt_eventdump << Form("%d:%d:%f:%f:%f:%.0f",
			    event,
			    AnalysisLeptons[i].flavour,
			    AnalysisLeptons[i].v.Pt(),
			    AnalysisLeptons[i].v.Eta(),
			    AnalysisLeptons[i].iso,
			    std_vector_lepton_isTightLepton->at(index));

      if (fabs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR)
	{
	  txt_eventdump << Form(":%f:%.0f:%f:%f:%.0f",
				std_vector_electron_scEta->at(index),
				std_vector_electron_passConversionVeto->at(index),
				std_vector_lepton_d0->at(index),
				std_vector_lepton_dz->at(index),
				std_vector_electron_expectedMissingInnerHits->at(index));
	}
      
      txt_eventdump << Form("\n");
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

  for (int i=0; i<std_vector_jet_pt->size(); i++)
    {
      if (std_vector_jet_pt->at(i) < 30.) continue;

      _ht     += std_vector_jet_pt->at(i);
      _htjets += std_vector_jet_pt->at(i);
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
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetSoftMuon()
{
  _foundsoftmuon = false;

  // https://twiki.cern.ch/twiki/bin/view/CMS/WW2015Variables#Soft_muons
  for (UInt_t i=0; i<std_vector_softMuPt->size(); i++) {

    if (std_vector_softMuPt->at(i)  < 3.)                       continue;
    if (std_vector_softMuD0->at(i)  < 0.2)                      continue;
    if (std_vector_softMuDz->at(i)  < 0.5)                      continue;
    if (std_vector_softMuIso->at(i) < 0.15)                     continue;
    if (std_vector_softMuIsTrackerMuon->at(i)         == false) continue;
    if (std_vector_softMuTMLastStationAngTight->at(i) == false) continue;
    
    _foundsoftmuon = true;
    
    break;
  }
}


//------------------------------------------------------------------------------
// GetFakeWeights
//------------------------------------------------------------------------------
void AnalysisCMS::GetFakeWeights()
{
  if (!_sample.Contains("DD_"))
    {
      _fake_weight            = 1.;
      _fake_weight_elUp       = 1.;
      _fake_weight_elDown     = 1.;
      _fake_weight_elStatUp   = 1.;
      _fake_weight_elStatDown = 1.;
      _fake_weight_muUp       = 1.;
      _fake_weight_muDown     = 1.;
      _fake_weight_muStatUp   = 1.;
      _fake_weight_muStatDown = 1.;

      return;
    }

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

      return;
    }

  if (_njet == 0)
    {
      _fake_weight            = fakeW2l0j;
      _fake_weight_elUp       = fakeW2l0jElUp;
      _fake_weight_elDown     = fakeW2l0jElDown;
      _fake_weight_elStatUp   = fakeW2l0jstatElUp;
      _fake_weight_elStatDown = fakeW2l0jstatElDown;
      _fake_weight_muUp       = fakeW2l0jMuUp;
      _fake_weight_muDown     = fakeW2l0jMuDown;
      _fake_weight_muStatUp   = fakeW2l0jstatMuUp;
      _fake_weight_muStatDown = fakeW2l0jstatMuDown;
    }
  else if (_njet == 1)
    {
      _fake_weight            = fakeW2l1j;
      _fake_weight_elUp       = fakeW2l1jElUp;
      _fake_weight_elDown     = fakeW2l1jElDown;
      _fake_weight_elStatUp   = fakeW2l1jstatElUp;
      _fake_weight_elStatDown = fakeW2l1jstatElDown;
      _fake_weight_muUp       = fakeW2l1jMuUp;
      _fake_weight_muDown     = fakeW2l1jMuDown;
      _fake_weight_muStatUp   = fakeW2l1jstatMuUp;
      _fake_weight_muStatDown = fakeW2l1jstatMuDown;
    }
  else
    {
      _fake_weight            = fakeW2l2j;
      _fake_weight_elUp       = fakeW2l2jElUp;
      _fake_weight_elDown     = fakeW2l2jElDown;
      _fake_weight_elStatUp   = fakeW2l2jstatElUp;
      _fake_weight_elStatDown = fakeW2l2jstatElDown;
      _fake_weight_muUp       = fakeW2l2jMuUp;
      _fake_weight_muDown     = fakeW2l2jMuDown;
      _fake_weight_muStatUp   = fakeW2l2jstatMuUp;
      _fake_weight_muStatDown = fakeW2l2jstatMuDown;
    }
}


//------------------------------------------------------------------------------
// EventSetup
//------------------------------------------------------------------------------
void AnalysisCMS::EventSetup(float jet_eta_max)
{
 
  GetGenPtllWeight();
 
  GetFakeWeights();
 
  ApplyWeights();
  
  GetMET(metPfType1, metPfType1Phi);
 
  GetTrkMET(metTtrk, metTtrkPhi);
  
  GetLeptons();

  GetJets(jet_eta_max);

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

  txt_summary.open("txt/" + _systematic + "/" + _analysis + "/" + _sample + ".txt");

  txt_summary << "\n";
  txt_summary << Form("   analysis: %s\n",        _analysis.Data());
  txt_summary << Form("   filename: %s\n",        _filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form(" luminosity: %.3f fb-1\n", _luminosity);
  txt_summary << Form("   nentries: %lld\n",      _nentries);
  txt_summary << "\n";
  
   //if (_analysis != "FR") {
  //  Summary(_analysis, "11.0", "raw yields");
  //}

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
  h_counterRaw   [ichannel][icut][ijet] = new TH1D("h_counterRaw"    + suffix, "",    3,    0,    3);
  h_counterLum   [ichannel][icut][ijet] = new TH1D("h_counterLum"    + suffix, "",    3,    0,    3);
  h_fakes        [ichannel][icut][ijet] = new TH1D("h_fakes"         + suffix, "",    9,    0,    9);
  h_nvtx         [ichannel][icut][ijet] = new TH1D("h_nvtx"          + suffix, "",   50,    0,   50);
  h_ptww         [ichannel][icut][ijet] = new TH1D("h_ptww"          + suffix, "", 3000,    0, 3000);
  h_pt2l         [ichannel][icut][ijet] = new TH1D("h_pt2l"          + suffix, "", 3000,    0, 3000);
  h_sumjpt12     [ichannel][icut][ijet] = new TH1D("h_sumjpt12"      + suffix, "", 3000,    0, 3000);
  h_sumpt12      [ichannel][icut][ijet] = new TH1D("h_sumpt12"       + suffix, "", 3000,    0, 3000);


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
  h_lep1eta       [ichannel][icut][ijet] = new TH1D("h_lep1eta"        + suffix, "",   60,   -3,    3);
  h_lep2eta       [ichannel][icut][ijet] = new TH1D("h_lep2eta"        + suffix, "",   60,   -3,    3);
  h_jet1eta       [ichannel][icut][ijet] = new TH1D("h_jet1eta"        + suffix, "",  100,   -5,    5);
  h_jet2eta       [ichannel][icut][ijet] = new TH1D("h_jet2eta"        + suffix, "",  100,   -5,    5);
  h_lep1phi       [ichannel][icut][ijet] = new TH1D("h_lep1phi"        + suffix, "",  200, -3.2,  3.2);
  h_lep2phi       [ichannel][icut][ijet] = new TH1D("h_lep2phi"        + suffix, "",  200, -3.2,  3.2);
  h_jet1phi       [ichannel][icut][ijet] = new TH1D("h_jet1phi"        + suffix, "",  200, -3.2,  3.2);
  h_jet2phi       [ichannel][icut][ijet] = new TH1D("h_jet2phi"        + suffix, "",  200, -3.2,  3.2);
  h_metPfType1Phi [ichannel][icut][ijet] = new TH1D("h_metPfType1Phi"  + suffix, "",  200, -3.2,  3.2);
  h_metTtrkPhi    [ichannel][icut][ijet] = new TH1D("h_metTtrkPhi"     + suffix, "",  200, -3.2,  3.2);
  h_lep1pt        [ichannel][icut][ijet] = new TH1D("h_lep1pt"         + suffix, "", 3000,    0, 3000);
  h_lep2pt        [ichannel][icut][ijet] = new TH1D("h_lep2pt"         + suffix, "", 3000,    0, 3000);
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


  // TH2 histograms
  //----------------------------------------------------------------------------
  h_metPfType1_m2l[ichannel][icut][ijet] = new TH2D("h_metPfType1_m2l" + suffix, "", 100, 0, 100, 100, 40, 140);
}


//------------------------------------------------------------------------------
// OpenMinitree
//------------------------------------------------------------------------------
void AnalysisCMS::OpenMinitree()
{
  if (!_saveminitree) return;

  gSystem->mkdir("minitrees/" + _systematic + "/" + _analysis, kTRUE);

  root_minitree = new TFile("minitrees/" + _systematic + "/" + _analysis + "/" + _sample + ".root", "recreate");


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
}


//------------------------------------------------------------------------------
// GetGenPtllWeight
//------------------------------------------------------------------------------
void AnalysisCMS::GetGenPtllWeight()
{
  _gen_ptll_weight = 1.0;

  if (!_sample.Contains("DYJetsToLL_M")) return;


  // Data-driven
  //----------------------------------------------------------------------------
  _gen_ptll_weight = 0.95 - 0.1*TMath::Erf((gen_ptll-14.)/8.8);


  // From resummed calculations
  //----------------------------------------------------------------------------
  //  float p0 = 1.02852e+00;
  //  float p1 = 9.49640e-02;
  //  float p2 = 1.90422e+01;
  //  float p3 = 1.04487e+01;
  //  float p4 = 7.58834e-02;
  //  float p5 = 5.61146e+01;
  //  float p6 = 4.11653e+01;
  //
  //  _gen_ptll_weight = p0 - p1*TMath::Erf((gen_ptll-p2)/p3) + p4*TMath::Erf((gen_ptll-p5)/p6);
}


//------------------------------------------------------------------------------
// GetSumOfWeightsLHE
// https://github.com/latinos/LatinoTrees/blob/master/AnalysisStep/src/WeightDumper.cc#L157
//------------------------------------------------------------------------------
void AnalysisCMS::GetSumOfWeightsLHE(TH1F* list_vectors_weights)
{
  if (!std_vector_LHE_weight) return;

  for (int iWeight=0; iWeight<list_vectors_weights->GetNbinsX(); iWeight++)
    {
      float ratio = std_vector_LHE_weight->at(iWeight) / std_vector_LHE_weight->at(0);

      list_vectors_weights->Fill(iWeight+0.5, ratio);
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
