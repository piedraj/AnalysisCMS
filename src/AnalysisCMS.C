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
  _analysis_dy   = false;
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
  if (analysis.EqualTo("DY"))   _analysis_dy   = true;
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

    if (!_analysis_top  && scut[j].Contains("Top/"))  continue;
    if (!_analysis_ttdm && scut[j].Contains("TTDM/")) continue;
    if (!_analysis_ww   && scut[j].Contains("WW/"))   continue;
    if (!_analysis_wz   && scut[j].Contains("WZ/"))   continue;
    if (!_analysis_dy   && scut[j].Contains("DY/"))   continue;

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
	h_counterRaw[i][j][k] = new TH1D("h_counterRaw" + suffix, "",    3,    0,    3);
	h_counterLum[i][j][k] = new TH1D("h_counterLum" + suffix, "",    3,    0,    3);
	h_njet30    [i][j][k] = new TH1D("h_njet30"     + suffix, "",    7, -0.5,  6.5);
	h_nbjet20   [i][j][k] = new TH1D("h_nbjet20"    + suffix, "",    7, -0.5,  6.5);
	h_nvtx      [i][j][k] = new TH1D("h_nvtx"       + suffix, "",   50,    0,   50);
	h_deltarll  [i][j][k] = new TH1D("h_deltarll"   + suffix, "",  100,    0,    5);
        h_deltaphill[i][j][k] = new TH1D("h_deltaphill" + suffix, "",  100,    0,    5);
        h_trkmet    [i][j][k] = new TH1D("h_trkmet"     + suffix, "", 2000,    0, 2000);
	h_met       [i][j][k] = new TH1D("h_met"        + suffix, "", 2000,    0, 2000);
        h_mpmet     [i][j][k] = new TH1D("h_mpmet"      + suffix, "", 2000,    0, 2000);
	h_m2l       [i][j][k] = new TH1D("h_m2l"        + suffix, "", 2000,    0, 2000);
        h_mt1       [i][j][k] = new TH1D("h_mt1"        + suffix, "", 2000,    0, 2000);
        h_mt2       [i][j][k] = new TH1D("h_mt2"        + suffix, "", 2000,    0, 2000);
        h_mth       [i][j][k] = new TH1D("h_mth"        + suffix, "", 2000,    0, 2000);
        h_mc        [i][j][k] = new TH1D("h_mc"         + suffix, "", 2000,    0, 2000);
	h_ht        [i][j][k] = new TH1D("h_ht"         + suffix, "", 2000,    0, 2000);
        h_pt1       [i][j][k] = new TH1D("h_pt1"        + suffix, "", 2000,    0, 2000);
        h_pt2       [i][j][k] = new TH1D("h_pt2"        + suffix, "", 2000,    0, 2000);
        h_pt2l      [i][j][k] = new TH1D("h_pt2l"       + suffix, "", 2000,    0, 2000);
        h_ptww      [i][j][k] = new TH1D("h_ptww"       + suffix, "", 2000,    0, 2000);

	// DY Data Driven histograms
	//----------------------------------------------------------------------
	h_NinZevents     [i][j][k] = new TH1D("h_NinZevents"      + suffix, "", 3,       0,    3);
	h_NoutZevents    [i][j][k] = new TH1D("h_NoutZevents"     + suffix, "", 3,       0,    3);
	h_NinLooseZevents[i][j][k] = new TH1D("h_NinLooseZevents" + suffix, "", 3,       0,    3);
	h_MassInZevents  [i][j][k] = new TH1D("h_MassInZevents"   + suffix, "", 2000,    0, 2000);
	h_MassOutZevents [i][j][k] = new TH1D("h_MassOutZevents"  + suffix, "", 2000,    0, 2000);

	// WZ histograms
	//----------------------------------------------------------------------
	h_m3l        [i][j][k] = new TH1D("h_m3l"         + suffix, "", 4000,     0,  4000);
	h_mtw        [i][j][k] = new TH1D("h_mtw"         + suffix, "", 2000,     0,  2000);
	h_zl1pt      [i][j][k] = new TH1D("h_zl1pt"       + suffix, "", 2000,     0,  2000);
	h_zl2pt      [i][j][k] = new TH1D("h_zl2pt"       + suffix, "", 2000,     0,  2000);
	h_wlpt       [i][j][k] = new TH1D("h_wlpt"        + suffix, "", 2000,     0,  2000);
	h_zl1eta     [i][j][k] = new TH1D("h_zl1eta"      + suffix, "",  120,    -3,     3);
	h_zl2eta     [i][j][k] = new TH1D("h_zl2eta"      + suffix, "",  120,    -3,     3);
	h_wleta      [i][j][k] = new TH1D("h_wleta"       + suffix, "",  120,    -3,     3);
	h_wlzl1deltar[i][j][k] = new TH1D("h_wlzl1deltar" + suffix, "",  100,     0,     5);
	h_wlzl2deltar[i][j][k] = new TH1D("h_wlzl2deltar" + suffix, "",  100,     0,     5);
	h_wldxy      [i][j][k] = new TH1D("h_wldxy"       + suffix, "",  500, -0.05,  0.05);
	h_wldz       [i][j][k] = new TH1D("h_wldz"        + suffix, "",  500, -0.05,  0.05);
	h_zl1dxy     [i][j][k] = new TH1D("h_zl1dxy"      + suffix, "",  500, -0.05,  0.05);
	h_zl1dz      [i][j][k] = new TH1D("h_zl1dz"       + suffix, "",  500, -0.05,  0.05);
	h_zl2dxy     [i][j][k] = new TH1D("h_zl2dxy"      + suffix, "",  500, -0.05,  0.05);
	h_zl2dz      [i][j][k] = new TH1D("h_zl2dz"       + suffix, "",  500, -0.05,  0.05);
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


    ApplyWeights(_sample, era, luminosity);

    GetMET(pfType1Met, pfType1Metphi);

    GetLeptons();

    GetJets();

    GetHt();

    GetSoftMuon();


    // Fill histograms
    //--------------------------------------------------------------------------
    if (_analysis_top)  AnalysisTop();
    if (_analysis_ttdm) AnalysisTTDM();
    if (_analysis_ww)   AnalysisWW();
    if (_analysis_wz)   AnalysisWZ();
    if (_analysis_dy)   AnalysisDY();
  }


  //----------------------------------------------------------------------------
  // Summary
  //----------------------------------------------------------------------------
  if (_eventdump) txt_eventdump.close();

  if (verbosity > 0) printf("\n\n");

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
  if (_analysis_dy)   Summary("DY",   "11.0", "raw yields");
  
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
  // Common histograms
  //----------------------------------------------------------------------------
  float deltarll   = Lepton1.v.DeltaR  (Lepton2.v);
  float deltaphill = fabs(Lepton1.v.DeltaPhi(Lepton2.v));

  h_counterRaw[ichannel][icut][ijet]->Fill(1);
  h_counterLum[ichannel][icut][ijet]->Fill(1,              _event_weight);
  h_ht        [ichannel][icut][ijet]->Fill(_ht,            _event_weight);
  h_m2l       [ichannel][icut][ijet]->Fill(_m2l,           _event_weight);
  h_njet30    [ichannel][icut][ijet]->Fill(_njet30,        _event_weight);
  h_nbjet20   [ichannel][icut][ijet]->Fill(_nbjet20,       _event_weight);
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

  // DY histograms (maybe we have to put them in a dedicated filling class?)
  //----------------------------------------------------------------------
  _m2l = (Lepton1.v + Lepton2.v).M();

  if (fabs(_m2l - Z_MASS) < 7.5) {
    h_NinZevents   [_channel][icut][_jetbin]->Fill(1,    _event_weight);
    h_NinZevents   [_channel][icut][njetbin]->Fill(1,    _event_weight);
    h_MassInZevents[_channel][icut][_jetbin]->Fill(_m2l, _event_weight);
    h_MassInZevents[_channel][icut][njetbin]->Fill(_m2l, _event_weight);
  }    

  else if (fabs(_m2l - Z_MASS) > 15) {  
    h_NoutZevents   [_channel][icut][_jetbin]->Fill(1,    _event_weight);
    h_NoutZevents   [_channel][icut][njetbin]->Fill(1,    _event_weight);
    h_MassOutZevents[_channel][icut][_jetbin]->Fill(_m2l, _event_weight);
    h_MassOutZevents[_channel][icut][njetbin]->Fill(_m2l, _event_weight);
  }
  
  // WZ histograms
  //----------------------------------------------------------------------------
  if (ichannel > ll)
    {
      float wlzl1deltar = WLepton.v.DeltaR(ZLepton1.v);
      float wlzl2deltar = WLepton.v.DeltaR(ZLepton2.v);

      h_m3l        [ichannel][icut][ijet]->Fill(_m3l,             _event_weight);
      h_mtw        [ichannel][icut][ijet]->Fill(_mtw,             _event_weight);
      h_zl1pt      [ichannel][icut][ijet]->Fill(ZLepton1.v.Pt(),  _event_weight);
      h_zl2pt      [ichannel][icut][ijet]->Fill(ZLepton2.v.Pt(),  _event_weight);
      h_wlpt       [ichannel][icut][ijet]->Fill(WLepton.v.Pt(),   _event_weight);
      h_zl1eta     [ichannel][icut][ijet]->Fill(ZLepton1.v.Eta(), _event_weight);
      h_zl2eta     [ichannel][icut][ijet]->Fill(ZLepton2.v.Eta(), _event_weight);
      h_wleta      [ichannel][icut][ijet]->Fill(WLepton.v.Eta(),  _event_weight);
      h_wlzl1deltar[ichannel][icut][ijet]->Fill(wlzl1deltar,      _event_weight);
      h_wlzl2deltar[ichannel][icut][ijet]->Fill(wlzl2deltar,      _event_weight);
      h_wldxy      [ichannel][icut][ijet]->Fill(WLepton.dxy,      _event_weight);
      h_wldz       [ichannel][icut][ijet]->Fill(WLepton.dz,       _event_weight);
      h_zl1dxy     [ichannel][icut][ijet]->Fill(ZLepton1.dxy,     _event_weight);
      h_zl1dz      [ichannel][icut][ijet]->Fill(ZLepton1.dz,      _event_weight);
      h_zl2dxy     [ichannel][icut][ijet]->Fill(ZLepton2.dxy,     _event_weight);
      h_zl2dz      [ichannel][icut][ijet]->Fill(ZLepton2.dz,      _event_weight);
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

  float signed_weight = -999;

  if (era.EqualTo("50ns"))
    {
      //      if (_sample.EqualTo("WWTo2L2Nu_NLL")) _event_weight *= nllW;

      if (_sample.EqualTo("WJetsToLNu"))      signed_weight = 1. / 0.683927;
      if (_sample.EqualTo("DYJetsToLL_M-50")) signed_weight = 1. / 0.670032;
      if (_sample.EqualTo("ST_t-channel"))    signed_weight = 1. / 0.215131;
      if (_sample.EqualTo("TTJets"))          signed_weight = 1. / 0.331907;
    }
  else if (era.EqualTo("25ns"))
    {
      if (_sample.EqualTo("WWTo2L2Nu"))           _event_weight *= 12.178 / 10.481;
      if (_sample.EqualTo("WJetsToLNu"))          signed_weight = 1. / 0.683938;
      if (_sample.EqualTo("DYJetsToLL_M-10to50")) signed_weight = 1. / 0.727601;
      if (_sample.EqualTo("DYJetsToLL_M-50"))     signed_weight = 1. / 0.66998;
      if (_sample.EqualTo("ZZTo2L2Q"))            signed_weight = 1. / 0.631351;
      if (_sample.EqualTo("ST_t-channel"))        signed_weight = 1. / 0.215648;
      if (_sample.EqualTo("TTJets"))              signed_weight = 1. / 0.331658;
    }

  if (signed_weight < 0) return;

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

  _nbjet20 = 0;

  int vector_jet_size = std_vector_jet_pt->size();

  for (int i=0; i<vector_jet_size; i++) {

    float pt  = std_vector_jet_pt ->at(i);
    float eta = std_vector_jet_eta->at(i);
    float phi = std_vector_jet_phi->at(i);

    if (pt < 20. || fabs(eta) > 2.4) continue;

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

    if (goodjet.csvv2ivf > csvv2ivf_looseWP) _nbjet20++;

    if (pt < 30.) continue;

    AnalysisJets.push_back(goodjet);
  }

  _njet30 = AnalysisJets.size();


  // Define the jet bin
  //----------------------------------------------------------------------------
  _jetbin = (_njet30 < njetbin) ? _njet30 : njetbin - 1;
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

  GetEventVariables();

  _m2l = (Lepton1.v + Lepton2.v).M();

  int nelec = 0;

  if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) nelec++;
  if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) nelec++;

  if      (nelec == 2) _channel = ee;
  else if (nelec == 1) _channel = em;
  else if (nelec == 0) _channel = mm;


  // Top selection
  //----------------------------------------------------------------------------
  bool pass = true;

  LevelHistograms(Top_00_Has2Leptons, pass);

  pass &= (_njet30 > 1);

  LevelHistograms(Top_01_Has2Jets, pass);

  pass &= (_nbjet20 > 0);

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

  GetEventVariables();

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

  pass &= (_njet30 > 1);

  if (_njet30 < 2) return;

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
  if (_nlepton < 2) return;
  if (_ntight  < 2) return;

  bool found_1st_lepton = false;
  bool found_2nd_lepton = false;

  for (UInt_t i=0; i<_nlepton; i++)
    {
      if (AnalysisLeptons[i].type == Loose) continue;

      if (!found_1st_lepton)
	{
	  found_1st_lepton = true;
	  Lepton1 = AnalysisLeptons[i];
	}
      else if (!found_2nd_lepton && i != Lepton1.index)
	{
	  found_2nd_lepton = true;
	  Lepton2 = AnalysisLeptons[i];
	}
    }

  GetEventVariables();

  int nelec = 0;

  if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) nelec++;
  if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) nelec++;

  if      (nelec == 2) _channel = ee;
  else if (nelec == 1) _channel = em;
  else if (nelec == 0) _channel = mm;

  _m2l  = (Lepton1.v + Lepton2.v).M();
  _pt2l = (Lepton1.v + Lepton2.v).Pt();

  // WW selection
  //----------------------------------------------------------------------------                                                               
  bool pass = true;

  // Two leptons Level
  pass &= (Lepton1.flavour * Lepton2.flavour < 0);
  pass &= (Lepton1.v.Pt() > 20.);
  pass &= (Lepton2.v.Pt() > 20.);
  LevelHistograms(WW_00_Has2Leptons, pass);

  // No additional tight leptons
  pass &= (_ntight == 2);
  LevelHistograms(WW_01_Exactly2Leptons, pass);

  // m2l > 12 GeV
  pass &= (_m2l > 12.);
  LevelHistograms(WW_02_Mll, pass);

  // met > 20 GeV
  pass &= (MET.Et() > 20.);
  LevelHistograms(WW_03_PfMet, pass);

  // Z-Veto (mll + metvar)                                                                                                                       
  bool pass_sf_Z = (nelec != 1 && _metvar > 45. && fabs(_m2l - Z_MASS) > 15.);
  bool pass_df_Z = (nelec == 1);

  pass &= (pass_sf_Z || pass_df_Z);
  LevelHistograms(WW_04_ZVeto, pass);

  // mpmet > 20 GeV                                                                                                                            
  pass &= (_mpmet > 20.);
  LevelHistograms(WW_05_MpMet, pass);

  // DeltaPhi veto (only SF)                                                                                                                    
  pass &= (_dphiv);
  LevelHistograms(WW_06_DPhiVeto, pass);

  // ptll > 30 GeV (45 GeV for SF)                                                                                                              
  bool pass_sf_pt2l = (nelec != 1 && _pt2l > 45.);
  bool pass_df_pt2l = (nelec == 1 && _pt2l > 30.);

  pass &= (pass_sf_pt2l || pass_df_pt2l);
  LevelHistograms(WW_07_Ptll, pass);

  // B veto
  pass &= (_nbjet20 == 0);
  LevelHistograms(WW_08_BVeto, pass);

  // Soft muon veto
  pass &= (!_foundsoftmuon);
  LevelHistograms(WW_09_SoftMu, pass);

  // Ht < 250 GeV
  pass &= (_ht < 250.);
  LevelHistograms(WW_10_Ht, pass);
}

//------------------------------------------------------------------------------                                                               
// AnalysisDY - Data Driven                                                                                     
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::AnalysisDY()
{
  if (_nlepton < 2) return;
  if (_ntight  < 2) return;

  bool found_1st_lepton = false;
  bool found_2nd_lepton = false;

  for (UInt_t i=0; i<_nlepton; i++)
    {
      if (AnalysisLeptons[i].type == Loose) continue;

      if (!found_1st_lepton)
	{
	  found_1st_lepton = true;
	  Lepton1 = AnalysisLeptons[i];
	}
      else if (!found_2nd_lepton && i != Lepton1.index)
	{
	  found_2nd_lepton = true;
	  Lepton2 = AnalysisLeptons[i];
	}
    }

  GetEventVariables();

  int nelec = 0;

  if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) nelec++;
  if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) nelec++;

  if      (nelec == 2) _channel = ee;
  else if (nelec == 1) _channel = em;
  else if (nelec == 0) _channel = mm;

  _m2l  = (Lepton1.v + Lepton2.v).M();
  _pt2l = (Lepton1.v + Lepton2.v).Pt();

  // Data Driven methods - DY
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool pass = true;

  // Two leptons Level
  pass &= (Lepton1.flavour * Lepton2.flavour < 0);
  pass &= (Lepton1.v.Pt() > 20.);
  pass &= (Lepton2.v.Pt() > 20.);

  // No additional tight leptons
  pass &= (_ntight == 2);

  // m2l > 12 GeV
  pass &= (_m2l > 12.);

  // met > 20 GeV
  pass &= (MET.Et() > 20.);

  // met > 20 GeV
  pass &= (_mpmet > 20.);

  // dphiveto
  pass &= (_dphiv);

  // ptll > 30 GeV
  pass &= (_pt2l > 30.);

  // b-veto csvv2ivf loose (0.605)
  pass &= (_nbjet20 == 0);

  // no soft muons
  pass &= (!_foundsoftmuon);
  
  // metvar > 20 GeV
  LevelHistograms(DY_00_20GeVLoose, pass && _metvar > 20.);

  // metvar > 25 GeV
  LevelHistograms(DY_01_25GeVLoose, pass && _metvar > 25.);

  // metvar > 30 GeV
  LevelHistograms(DY_02_30GeVLoose, pass && _metvar > 30.);

  // metvar > 45 GeV
  LevelHistograms(DY_03_45GeVLoose, pass && _metvar > 45.);

  // metvar > 1000 GeV
  LevelHistograms(DY_04_1000GeVLoose, pass && _metvar > 1000.);

  // 20 GeV < metvar < 25 GeV
  LevelHistograms(DY_05_20GeVTight, pass && _metvar >= 20. && _metvar < 25.);

  // 25 GeV < metvar < 30 GeV
  LevelHistograms(DY_06_25GeVTight, pass && _metvar >= 25. && _metvar < 30.);

  // 30 GeV < metvar < 45 GeV
  LevelHistograms(DY_07_30GeVTight, pass && _metvar >= 30. && _metvar < 45.);

  // 45 GeV < metvar < 1000 GeV
  LevelHistograms(DY_08_45GeVTight, pass && _metvar >= 40. && _metvar < 1000.);
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

  Lepton1 = AnalysisLeptons[0];
  Lepton2 = AnalysisLeptons[1];

  GetEventVariables();


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

  bool pass_dr  = (WLepton.v.DeltaR(ZLepton1.v) > 0.1 && WLepton.v.DeltaR(ZLepton2.v) > 0.1);
  bool pass_mll = ((WLepton.v + ZLepton1.v).M() > 4. && (WLepton.v + ZLepton2.v).M() > 4.);

  LevelHistograms(WZ_02_HasWdr,  pass && pass_dr);
  LevelHistograms(WZ_02_HasWmll, pass && pass_mll);

  pass &= (_nbjet20 == 0);
	
  LevelHistograms(WZ_03_BVeto, pass && pass_mll);
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

  for (int i=0; i<_nlepton; i++)
    {
      if (AnalysisLeptons[i].type == Loose) continue;

      _ht += AnalysisLeptons[i].v.Pt();
    }

  for (int i=0; i<_njet30; i++)
    {
      _ht += AnalysisJets[i].v.Pt();
    }
}


//------------------------------------------------------------------------------                                                               
// GetMpMet
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetMpMet()
{
  Float_t dphi1   = Lepton1.v.DeltaPhi(MET);
  Float_t dphi2   = Lepton2.v.DeltaPhi(MET);
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
  _metvar = (_njet30 <= 1) ? _mpmet : MET.Et();
}


//------------------------------------------------------------------------------                                                               
// GetDPhiVeto                                                                                                                                 
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetDPhiVeto()
{
  _dphiv = (_njet30 <= 1 || (_njet30 > 1 && dphilljetjet < 165.*TMath::DegToRad()));
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

  if (_pt2l > 0 && _m2l > 0 && met > 0)
    _mc = sqrt(pow(sqrt(_pt2l*_pt2l + _m2l*_m2l) + met, 2) - pow(_pt2l + met, 2));
}


//------------------------------------------------------------------------------                                                               
// GetPtWW                                                                                                                                     
//------------------------------------------------------------------------------                                                                
void AnalysisCMS::GetPtWW()
{
  _ptww = 0;

  if (Lepton1.v.Pt() > 0 && Lepton2.v.Pt() > 0)
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
