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
  _eventdump     = false;
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
    }


  // Additional settings
  //----------------------------------------------------------------------------
  gSystem->mkdir(Form("rootfiles/%s", era.Data()), kTRUE);
  gSystem->mkdir(Form("txt/%s",       era.Data()), kTRUE);

  root_output = new TFile("rootfiles/" + era + "/" + _sample + ".root", "recreate");

  if (_eventdump) txt_eventdump.open("txt/" + era + "/" + _sample + "_eventdump.txt");


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {

    if (!_analysis_top  && scut[j].Contains("Top/"))   continue;
    if (!_analysis_ttdm && scut[j].Contains("TTDM/"))  continue;
    if (!_analysis_ww   && scut[j].Contains("WW/"))    continue;
    if (!_analysis_ww   && scut[j].Contains("monoH/")) continue;
    if (!_analysis_wz   && scut[j].Contains("WZ/"))    continue;

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=0; i<nchannel; i++) {

	TString suffix = "_" + schannel[i];


	// Common TH1 histograms
	//----------------------------------------------------------------------
	h_counterRaw[i][j][k] = new TH1D("h_counterRaw" + suffix, "",    3,    0,    3);
	h_counterLum[i][j][k] = new TH1D("h_counterLum" + suffix, "",    3,    0,    3);
	h_njet30    [i][j][k] = new TH1D("h_njet30"     + suffix, "",    7, -0.5,  6.5);
	h_nbjet15   [i][j][k] = new TH1D("h_nbjet15"    + suffix, "",    7, -0.5,  6.5);
	h_nvtx      [i][j][k] = new TH1D("h_nvtx"       + suffix, "",   50,    0,   50);
	h_deltarll  [i][j][k] = new TH1D("h_deltarll"   + suffix, "",  100,    0,    5);
        h_deltaphill[i][j][k] = new TH1D("h_deltaphill" + suffix, "",  100,    0,    5);
	h_met       [i][j][k] = new TH1D("h_met"        + suffix, "", 3000,    0, 3000);
        h_trkmet    [i][j][k] = new TH1D("h_trkmet"     + suffix, "", 3000,    0, 3000);
        h_mpmet     [i][j][k] = new TH1D("h_mpmet"      + suffix, "", 3000,    0, 3000);
	h_m2l       [i][j][k] = new TH1D("h_m2l"        + suffix, "", 3000,    0, 3000);
        h_mt1       [i][j][k] = new TH1D("h_mt1"        + suffix, "", 3000,    0, 3000);
        h_mt2       [i][j][k] = new TH1D("h_mt2"        + suffix, "", 3000,    0, 3000);
        h_mth       [i][j][k] = new TH1D("h_mth"        + suffix, "", 3000,    0, 3000);
        h_mc        [i][j][k] = new TH1D("h_mc"         + suffix, "", 3000,    0, 3000);
	h_ht        [i][j][k] = new TH1D("h_ht"         + suffix, "", 3000,    0, 3000);
        h_pt1       [i][j][k] = new TH1D("h_pt1"        + suffix, "", 3000,    0, 3000);
        h_pt2       [i][j][k] = new TH1D("h_pt2"        + suffix, "", 3000,    0, 3000);
        h_pt2l      [i][j][k] = new TH1D("h_pt2l"       + suffix, "", 3000,    0, 3000);
        h_ptww      [i][j][k] = new TH1D("h_ptww"       + suffix, "", 3000,    0, 3000);

	
	// Common TH2 histograms
	//----------------------------------------------------------------------
	h_metvar_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 2000, 0, 200);


	// WZ histograms
	//----------------------------------------------------------------------
	h_m3l       [i][j][k] = new TH1D("h_m3l"        + suffix, "", 2000, 0, 2000);
	h_mtw       [i][j][k] = new TH1D("h_mtw"        + suffix, "", 1000, 0, 1000);
	h_zl1pt     [i][j][k] = new TH1D("h_zl1pt"      + suffix, "", 1000, 0, 1000);
	h_zl2pt     [i][j][k] = new TH1D("h_zl2pt"      + suffix, "", 1000, 0, 1000);
	h_wlpt      [i][j][k] = new TH1D("h_wlpt"       + suffix, "", 1000, 0, 1000);
	h_wlzldeltar[i][j][k] = new TH1D("h_wlzldeltar" + suffix, "",  100, 0,    5);
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


    // Print progress
    //--------------------------------------------------------------------------
    if (verbosity > 0)
      {
        double progress = 1e2 * (jentry+1) / nentries;

	double fractpart, intpart;

	fractpart = modf(progress, &intpart);

	if (fractpart < 1e-2)
	  {
	    std::cout << "   progress: " << int(ceil(progress)) << "%\r";
	    std::cout.flush();
	  }
      }


    ApplyWeights(_sample, luminosity);

    GetMET(pfType1Met, pfType1Metphi);

    GetLeptons();

    GetJets();

    GetHt();

    GetSoftMuon();

    GetEventVariables();


    // Fill 3 lepton histograms
    //--------------------------------------------------------------------------
    if (_analysis_wz) AnalysisWZ();


    // Fill 2 lepton histograms
    //--------------------------------------------------------------------------
    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    _nelectron = 0;  // Redefine _nelectron

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;
    _pt2l = ptll;
    
    if (_analysis_top)  AnalysisTop();
    if (_analysis_ttdm) AnalysisTTDM();
    if (_analysis_ww)   AnalysisWW();
  }


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  if (_eventdump) txt_eventdump.close();

  txt_summary.open("txt/" + era + "/" + _sample + ".txt");

  txt_summary << "\n";
  txt_summary << Form("   filename: %s\n",        filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form("        era: %s\n",        era.Data());
  txt_summary << Form(" luminosity: %.2f pb-1\n", 1e3*luminosity);
  txt_summary << Form("   nentries: %lld\n",      nentries);
  txt_summary << "\n";

  if (_analysis_top)  Summary("Top",   "11.0", "raw yields");
  if (_analysis_ttdm) Summary("TTDM",  "11.0", "raw yields");
  if (_analysis_ww)   Summary("WW",    "11.0", "raw yields");
  if (_analysis_ww)   Summary("monoH", "11.0", "raw yields");
  if (_analysis_wz)   Summary("WZ",    "11.0", "raw yields");

  txt_summary.close();

  root_output->cd();
 
  if (verbosity > 0)
    {
      printf("\n\n Writing histograms. This can take a while...\n");
    }

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();

  if (verbosity > 0) printf("\n Done!\n\n");
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

  if      (fabs(flavour) == ELECTRON_FLAVOUR) is_isolated_lepton = true;
  else if (fabs(flavour) == MUON_FLAVOUR)     is_isolated_lepton = (MuonIsolation(k) < 0.12);
  
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
  // Common TH1 histograms
  //----------------------------------------------------------------------------
  float deltarll   = Lepton1.v.DeltaR(Lepton2.v);
  float deltaphill = fabs(Lepton1.v.DeltaPhi(Lepton2.v));

  h_counterRaw[ichannel][icut][ijet]->Fill(1);
  h_counterLum[ichannel][icut][ijet]->Fill(1,              _event_weight);
  h_ht        [ichannel][icut][ijet]->Fill(_ht,            _event_weight);
  h_m2l       [ichannel][icut][ijet]->Fill(_m2l,           _event_weight);
  h_njet30    [ichannel][icut][ijet]->Fill(njet,           _event_weight);
  h_nbjet15   [ichannel][icut][ijet]->Fill(_nbjet15,       _event_weight);
  h_nvtx      [ichannel][icut][ijet]->Fill(nvtx,           _event_weight);
  h_met       [ichannel][icut][ijet]->Fill(MET.Et(),       _event_weight);
  h_deltarll  [ichannel][icut][ijet]->Fill(deltarll,       _event_weight);
  h_mpmet     [ichannel][icut][ijet]->Fill(_mpmet,         _event_weight);
  h_pt1       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(), _event_weight);
  h_pt2       [ichannel][icut][ijet]->Fill(Lepton2.v.Pt(), _event_weight);
  h_pt2l      [ichannel][icut][ijet]->Fill(_pt2l,          _event_weight);
  h_mth       [ichannel][icut][ijet]->Fill(mth,            _event_weight);
  h_mt1       [ichannel][icut][ijet]->Fill(_mt1,           _event_weight);
  h_mt2       [ichannel][icut][ijet]->Fill(_mt2,           _event_weight);
  h_trkmet    [ichannel][icut][ijet]->Fill(trkMet,         _event_weight);
  h_deltaphill[ichannel][icut][ijet]->Fill(deltaphill,     _event_weight);
  h_mc        [ichannel][icut][ijet]->Fill(_mc,            _event_weight);
  h_ptww      [ichannel][icut][ijet]->Fill(_ptww,          _event_weight);


  // Common TH2 histograms
  //----------------------------------------------------------------------------
  h_metvar_m2l[ichannel][icut][ijet]->Fill(_metvar, _m2l, _event_weight);


  // WZ histograms
  //----------------------------------------------------------------------------
  if (ichannel > ll)
    {
      float wlzl1deltar = WLepton.v.DeltaR(ZLepton1.v);
      float wlzl2deltar = WLepton.v.DeltaR(ZLepton2.v);
      float wlzldeltar  = min(wlzl1deltar, wlzl2deltar);

      h_m3l       [ichannel][icut][ijet]->Fill(_m3l,             _event_weight);
      h_mtw       [ichannel][icut][ijet]->Fill(_mtw,             _event_weight);
      h_zl1pt     [ichannel][icut][ijet]->Fill(ZLepton1.v.Pt(),  _event_weight);
      h_zl2pt     [ichannel][icut][ijet]->Fill(ZLepton2.v.Pt(),  _event_weight);
      h_wlpt      [ichannel][icut][ijet]->Fill(WLepton.v.Pt(),   _event_weight);
      h_wlzldeltar[ichannel][icut][ijet]->Fill(wlzldeltar,       _event_weight);
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
void AnalysisCMS::ApplyWeights(TString sample, float luminosity)
{
  _event_weight = 1.;
  
  if (!_ismc) return;

  _event_weight *= puW * baseW * luminosity;

  if (sample.EqualTo("WWTo2L2Nu")) _event_weight *= 12.178 / 10.481;

  if (!GEN_weight_SM) return;

  _event_weight *= GEN_weight_SM / abs(GEN_weight_SM);

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

    if (i > 1 && !IsFiducialLepton(i)) continue;

    float pt  = std_vector_lepton_pt ->at(i);
    float eta = std_vector_lepton_eta->at(i);
    float phi = std_vector_lepton_phi->at(i);

    Lepton lep;
      
    lep.index   = i;
    lep.type    = Loose;
    lep.flavour = std_vector_lepton_flavour->at(i);
      
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

    if (i == 0) Lepton1 = lep;
    if (i == 1) Lepton2 = lep;
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

  _nbjet15 = 0;

  int vector_jet_size = std_vector_jet_pt->size();

  for (int i=0; i<vector_jet_size; i++) {

    float pt  = std_vector_jet_pt ->at(i);
    float eta = std_vector_jet_eta->at(i);
    float phi = std_vector_jet_phi->at(i);

    if (pt < 15. || fabs(eta) > 2.4) continue;

    TLorentzVector tlv;

    tlv.SetPtEtaPhiM(pt, eta, phi, 0.0);

    //    bool is_lepton = false;
    //
    //    for (int j=0; j<_nlepton; j++)
    //      {
    //	if (AnalysisLeptons[j].type == Loose) continue;
    //	
    //	if (tlv.DeltaR(AnalysisLeptons[j].v) < 0.4) is_lepton = true;
    //      }
    //
    //    if (is_lepton) continue;

    Jet goodjet;

    goodjet.index    = i;
    goodjet.csvv2ivf = std_vector_jet_csvv2ivf->at(i);
    goodjet.v        = tlv;

    if (goodjet.csvv2ivf > csvv2ivf_looseWP) _nbjet15++;

    if (pt < 30.) continue;

    AnalysisJets.push_back(goodjet);
  }

  _njet30 = AnalysisJets.size();


  // Define the jet bin
  //----------------------------------------------------------------------------
  _jetbin = (njet < njetbin) ? njet : njetbin - 1;
}


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisTop()
{
  bool pass = true;

  LevelHistograms(Top_00_Has2Leptons, pass);

  pass &= (njet > 1);

  LevelHistograms(Top_01_Has2Jets, pass);

  pass &= (_nbjet15 > 0);

  LevelHistograms(Top_02_Has1BJet, pass);
}


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
void AnalysisCMS::AnalysisTTDM()
{
  bool pass = true;

  LevelHistograms(TTDM_00_Has2Leptons, pass);

  bool pass_sf = (_nelectron != 1 && fabs(mll - Z_MASS) > 15.);
  bool pass_df = (_nelectron == 1);

  pass &= (mll > 20.);
  pass &= (pass_sf || pass_df);

  LevelHistograms(TTDM_01_ZVeto, pass);

  pass &= (njet > 1);

  if (njet < 2) return;

  LevelHistograms(TTDM_02_Has2Jets, pass);

  pass &= (Lepton1.v.Pt() + Lepton2.v.Pt() > 120.);

  LevelHistograms(TTDM_03_LepPtSum, pass);

  pass &= (AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt() < 400.);

  LevelHistograms(TTDM_04_JetPtSum, pass);

  pass &= (Lepton1.v.DeltaPhi(Lepton2.v) < 2.);

  LevelHistograms(TTDM_05_LepDeltaPhi, pass);

  pass &= (MET.Et() > 320.);

  LevelHistograms(TTDM_06_MET, pass);
}


//------------------------------------------------------------------------------                                                               
// AnalysisWW                                                                                                                                  
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::AnalysisWW()
{
  bool pass = true;

  LevelHistograms(WW_00_Has2Leptons, pass);

  pass &= (mll > 12.);
  LevelHistograms(WW_01_Mll, pass);

  pass &= (MET.Et() > 20.);
  LevelHistograms(WW_02_PfMet, pass);

  bool pass_zveto = (_nelectron == 1 || _nelectron != 1 && _metvar > 45. && fabs(mll - Z_MASS) > 15.);

  LevelHistograms(WW_03_ZVeto, pass && pass_zveto);

  pass &= (_mpmet > 20.);
  LevelHistograms(WW_04_MpMet, pass && pass_zveto);

  //pass &= (_passdphiveto);
  LevelHistograms(WW_05_DPhiVeto, pass && pass_zveto);

  bool pass_ptll = (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);

  pass &= pass_ptll;
  LevelHistograms(WW_06_Ptll, pass && pass_zveto);

  pass &= (_nbjet15 == 0);
  LevelHistograms(WW_07_BVeto, pass && pass_zveto);

  pass &= (!_foundsoftmuon);
  LevelHistograms(WW_08_SoftMu, pass && pass_zveto);

  bool pass_ht = (_ht < 250.);
  LevelHistograms(WW_09_Ht, pass && pass_zveto && pass_ht);

  LevelHistograms(WW_10_DY, pass);  // Data-driven DY

  bool passZwindow = (fabs(mll - Z_MASS) < 15.);  // Z window at 2 leptons level
  LevelHistograms(WW_11_ZWindow, passZwindow);

  bool Jet[10];

  for (UInt_t j=0; j<10; ++j)
    {
      Jet[j] = (std_vector_jet_pt->at(0) < 25 + j);
      LevelHistograms(WW_18_ZWindow25 + j, passZwindow && pass_ptll && Jet[j]);
    }

  passZwindow &= (MET.Et() > 20.);
  LevelHistograms(WW_12_ZWindowPfMet, passZwindow);

  passZwindow &= (_mpmet > 20.);
  LevelHistograms(WW_13_ZWindowMpMet, passZwindow);

  passZwindow &= (pass_ptll);
  LevelHistograms(WW_14_ZWindowPtll, passZwindow);

  passZwindow &= (_nbjet15 == 0);
  LevelHistograms(WW_15_ZWindowBVeto, passZwindow);

  passZwindow &= (!_foundsoftmuon);
  LevelHistograms(WW_16_ZWindowSoftMu, passZwindow);

  LevelHistograms(WW_17_ZCR, pass && pass_ht && passZwindow);  // Z control region - orthogonal to WW one


  // monoH selection - on top of WW excluding Ht selection
  //----------------------------------------------------------------------------
  bool pass_monoh = (pass && pass_zveto);
  bool pass_drll  = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);

  LevelHistograms(monoH_80_CR, pass_monoh && !pass_drll);

  pass_monoh &= (_mc < 100);
  LevelHistograms(monoH_00_Mc, pass_monoh);

  pass_monoh &= pass_drll;
  LevelHistograms(monoH_01_DRll, pass_monoh);

  pass_monoh &= (_mpmet > 60);
  LevelHistograms(monoH_02_MpMet, pass_monoh);
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


  if (_eventdump) EventDump();


  // WZ selection
  //----------------------------------------------------------------------------
  if (_m2l < 0) return;

  _m3l  = (ZLepton1.v + ZLepton2.v + WLepton.v).M();
  _pt2l = (ZLepton1.v + ZLepton2.v).Pt();
  _mtw  = GetMt(WLepton);

  bool pass = true;

  LevelHistograms(WZ_00_Exactly3Leptons, pass);
    
  pass &= (_m2l > 60. && _m2l < 120.);
  pass &= (ZLepton1.v.Pt() > 20.);

  LevelHistograms(WZ_01_HasZ, pass);

  pass &= (WLepton.v.Pt() >  20.);
  pass &= (MET.Et()       >  30.);
  pass &= (_m3l           > 100.);

  pass &= ((WLepton.v + ZLepton1.v).M() > 4.);
  pass &= ((WLepton.v + ZLepton2.v).M() > 4.);

  LevelHistograms(WZ_02_HasW, pass);

  pass &= (_nbjet15 == 0);
	
  LevelHistograms(WZ_03_BVeto, pass);
}


//------------------------------------------------------------------------------
// EventDump
//------------------------------------------------------------------------------
void AnalysisCMS::EventDump()
{
  for (int i=0; i<_nlepton; i++)
    {
      int index = AnalysisLeptons[i].index;

      txt_eventdump << Form("%.0f:%d:%f:%f:%f:%d",
			    evt,
			    AnalysisLeptons[i].flavour,
			    AnalysisLeptons[i].v.Pt(),
			    AnalysisLeptons[i].v.Eta(),
			    AnalysisLeptons[i].iso,
			    IsTightLepton(index));

      if (fabs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR)
	{
	  txt_eventdump << Form(":%f:%.0f:%f:%f:%.0f",
				std_vector_electron_scEta->at(index),
				std_vector_electron_passConversionVeto->at(index),
				std_vector_electron_d0->at(index),
				std_vector_electron_dz->at(index),
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
// GetHt
//------------------------------------------------------------------------------
void AnalysisCMS::GetHt()
{
  _ht = MET.Et();

  _ht += Lepton1.v.Pt();
  _ht += Lepton2.v.Pt();

  for (int i=0; i<std_vector_jet_pt->size(); i++)
    {
      if (std_vector_jet_pt->at(i) < 30.) continue;

      _ht += std_vector_jet_pt->at(i);
    }
}


//------------------------------------------------------------------------------                                                               
// GetMpMet
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMpMet()
{
  Float_t dphi1   = fabs(Lepton1.v.DeltaPhi(MET));
  Float_t dphi2   = fabs(Lepton2.v.DeltaPhi(MET));
  Float_t dphimin = min(dphi1, dphi2);

  Float_t fullpmet = MET.Et();
  Float_t trkpmet  = trkMet;

  if (dphimin < TMath::Pi() / 2.)
    {
      fullpmet *= sin(dphimin);
      trkpmet  *= sin(dphimin);
    }

  _mpmet = min(trkpmet, fullpmet);
}


//------------------------------------------------------------------------------                                                               
// GetMetVar                                                                                                                                   
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMetVar()
{
  _metvar = (njet <= 1) ? _mpmet : MET.Et();
}


//------------------------------------------------------------------------------                                                               
// GetDPhiVeto                                                                                                                                 
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetDPhiVeto()
{
  _passdphiveto = (njet < 2 || dphilljetjet < 165.*TMath::DegToRad());
}


//------------------------------------------------------------------------------                                                               
// GetMt
//------------------------------------------------------------------------------                                                               
float AnalysisCMS::GetMt(Lepton lep)
{
  float transverse_mass = 0;

  float met  = MET.Et();
  float pt   = lep.v.Pt();
  float dphi = lep.v.DeltaPhi(MET);

  if (met < 0) return transverse_mass;
  if (pt  < 0) return transverse_mass;

  transverse_mass = sqrt(2*pt*met*(1. - cos(dphi)));

  return transverse_mass;
}


//------------------------------------------------------------------------------                                                               
// GetMc                                                                                                                                       
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMc()
{
  _mc = 0;

  float met = MET.Et();

  if (ptll > 0 && mll > 0 && met > 0)
    _mc = sqrt(pow(sqrt(ptll*ptll + mll*mll) + met, 2) - pow(ptll + met, 2));
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
  
  for (UInt_t i=0; i<std_vector_jet_softMuPt->size(); ++i)
    {
      if (std_vector_jet_pt->at(i)       < 10.) continue;
      if (std_vector_jet_pt->at(i)       > 30.) continue;
      if (std_vector_jet_softMuPt->at(i) <  3.) continue;

      _foundsoftmuon = true;

      break;
    }
}


//------------------------------------------------------------------------------
// GetEventVariables
//------------------------------------------------------------------------------
void AnalysisCMS::GetEventVariables()
{
  _mt1 = GetMt(Lepton1);
  _mt2 = GetMt(Lepton2);

  GetMc();
  GetPtWW();
  GetMpMet();
  GetMetVar();
  GetDPhiVeto();
}
