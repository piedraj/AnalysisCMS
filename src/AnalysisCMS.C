#define AnalysisCMS_cxx
#include "../include/AnalysisCMS.h"


//------------------------------------------------------------------------------
// AnalysisCMS
//------------------------------------------------------------------------------
AnalysisCMS::AnalysisCMS(TTree* tree) : AnalysisBase(tree)
{
  _eventdump = false;
  _ismc      = true;
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
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisCMS::FillHistograms(int ichannel, int icut, int ijet)
{
  h_counterRaw[ichannel][icut][ijet]->Fill(1);
  h_counterLum[ichannel][icut][ijet]->Fill(1,              _event_weight);
  h_ht        [ichannel][icut][ijet]->Fill(_ht,            _event_weight);
  h_m2l       [ichannel][icut][ijet]->Fill(_m2l,           _event_weight);
  h_njet30    [ichannel][icut][ijet]->Fill(njet,           _event_weight);
  h_nbjet15   [ichannel][icut][ijet]->Fill(_nbjet15,       _event_weight);
  h_nvtx      [ichannel][icut][ijet]->Fill(nvtx,           _event_weight);
  h_met       [ichannel][icut][ijet]->Fill(MET.Et(),       _event_weight);
  h_drll      [ichannel][icut][ijet]->Fill(drll,           _event_weight);
  h_mpmet     [ichannel][icut][ijet]->Fill(_mpmet,         _event_weight);
  h_pt1       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(), _event_weight);
  h_pt2       [ichannel][icut][ijet]->Fill(Lepton2.v.Pt(), _event_weight);
  h_pt2l      [ichannel][icut][ijet]->Fill(_pt2l,          _event_weight);
  h_mth       [ichannel][icut][ijet]->Fill(mth,            _event_weight);
  h_mtw1      [ichannel][icut][ijet]->Fill(mtw1,           _event_weight);
  h_mtw2      [ichannel][icut][ijet]->Fill(mtw2,           _event_weight);
  h_trkmet    [ichannel][icut][ijet]->Fill(trkMet,         _event_weight);
  h_deltaphill[ichannel][icut][ijet]->Fill(_deltaphill,    _event_weight);
  h_mc        [ichannel][icut][ijet]->Fill(_mc,            _event_weight);
  h_ptww      [ichannel][icut][ijet]->Fill(_ptww,          _event_weight);

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
  
  gSystem->mkdir("rootfiles/" + _analysis, kTRUE);
  gSystem->mkdir("txt/"       + _analysis, kTRUE);

  root_output = new TFile("rootfiles/" + _analysis + "/" + _sample + ".root", "recreate");

  if (_eventdump) txt_eventdump.open("txt/" + _analysis + "/" + _sample + "_eventdump.txt");

  return;
}


//------------------------------------------------------------------------------
// ApplyWeights
//------------------------------------------------------------------------------
void AnalysisCMS::ApplyWeights()
{
  _event_weight = 1.;

  if (!_ismc) return;

  _event_weight = puW * baseW * _luminosity;

  if (_sample.EqualTo("WWTo2L2Nu")) _event_weight *= 12.178 / 10.481;

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

    bool pass = (pt > 15. && fabs(eta) < 4.7);

    if (!pass) continue;

    TLorentzVector tlv;

    tlv.SetPtEtaPhiM(pt, eta, phi, 0.0);

    bool is_lepton = false;
    
    for (int j=0; j<_nlepton; j++)
      {
    	if (AnalysisLeptons[j].type == Loose) continue;
    	
    	if (tlv.DeltaR(AnalysisLeptons[j].v) < 0.3) is_lepton = true;
      }
    
    //    if (is_lepton) continue;

    Jet goodjet;

    goodjet.index    = i;
    goodjet.csvv2ivf = std_vector_jet_csvv2ivf->at(i);
    goodjet.v        = tlv;

    if (goodjet.csvv2ivf > csvv2ivf_looseWP) _nbjet15++;

    if (pt > 30.) AnalysisJets.push_back(goodjet);
  }

  _njet30 = AnalysisJets.size();


  // Define the jet bin
  //----------------------------------------------------------------------------
  _jetbin = (njet < njetbin) ? njet : njetbin - 1;
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
  Float_t dphimin = min(dphilmet1, dphilmet2);

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
// GetDeltaPhiVeto                                                                                                                                 
//------------------------------------------------------------------------------                                                               
void AnalysisCMS::GetDeltaPhiVeto()
{
  _passdphiveto = (njet < 2 || dphilljetjet < 165.*TMath::DegToRad());
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
// EventSetup
//------------------------------------------------------------------------------
void AnalysisCMS::EventSetup()
{
  ApplyWeights();
  
  GetMET(pfType1Met, pfType1Metphi);
  
  GetLeptons();

  GetJets();

  GetHt();

  GetSoftMuon();

  GetMc();
  
  GetPtWW();

  GetMpMet();

  GetMetVar();

  GetDeltaPhill();

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
      std::cout << "   progress: " << int(ceil(progress)) << "%\r";
      std::cout.flush();
    }
}


//------------------------------------------------------------------------------
// EndJob
//------------------------------------------------------------------------------
void AnalysisCMS::EndJob()
{
  if (_eventdump) txt_eventdump.close();

  txt_summary.open("txt/" + _analysis + "/" + _sample + ".txt");

  txt_summary << "\n";
  txt_summary << Form("   analysis: %s\n",        _analysis.Data());
  txt_summary << Form("   filename: %s\n",        _filename.Data());
  txt_summary << Form("     sample: %s\n",        _sample.Data());
  txt_summary << Form(" luminosity: %.3f fb-1\n", _luminosity);
  txt_summary << Form("   nentries: %lld\n",      _nentries);
  txt_summary << "\n";

  Summary(_analysis, "11.0", "raw yields");

  if (_analysis.EqualTo("WW")) Summary("monoH", "11.0", "raw yields");

  txt_summary.close();

  root_output->cd();
 
  printf("\n\n Writing histograms. This can take a while...\n");

  root_output->Write("", TObject::kOverwrite);
  
  root_output->Close();
  
  printf("\n Done!\n\n");
}


//------------------------------------------------------------------------------
// DefineHistograms
//------------------------------------------------------------------------------
void AnalysisCMS::DefineHistograms(int     ichannel,
				   int     icut,
				   int     ijet,
				   TString suffix)
{
  h_counterRaw[ichannel][icut][ijet] = new TH1D("h_counterRaw" + suffix, "",    3,    0,    3);
  h_counterLum[ichannel][icut][ijet] = new TH1D("h_counterLum" + suffix, "",    3,    0,    3);
  h_njet30    [ichannel][icut][ijet] = new TH1D("h_njet30"     + suffix, "",    7, -0.5,  6.5);
  h_nbjet15   [ichannel][icut][ijet] = new TH1D("h_nbjet15"    + suffix, "",    7, -0.5,  6.5);
  h_nvtx      [ichannel][icut][ijet] = new TH1D("h_nvtx"       + suffix, "",   50,    0,   50);
  h_drll      [ichannel][icut][ijet] = new TH1D("h_drll"       + suffix, "",  100,    0,    5);
  h_deltaphill[ichannel][icut][ijet] = new TH1D("h_deltaphill" + suffix, "",  100,    0,    5);
  h_met       [ichannel][icut][ijet] = new TH1D("h_met"        + suffix, "", 3000,    0, 3000);
  h_trkmet    [ichannel][icut][ijet] = new TH1D("h_trkmet"     + suffix, "", 3000,    0, 3000);
  h_mpmet     [ichannel][icut][ijet] = new TH1D("h_mpmet"      + suffix, "", 3000,    0, 3000);
  h_m2l       [ichannel][icut][ijet] = new TH1D("h_m2l"        + suffix, "", 3000,    0, 3000);
  h_mtw1      [ichannel][icut][ijet] = new TH1D("h_mtw1"       + suffix, "", 3000,    0, 3000);
  h_mtw2      [ichannel][icut][ijet] = new TH1D("h_mtw2"       + suffix, "", 3000,    0, 3000);
  h_mth       [ichannel][icut][ijet] = new TH1D("h_mth"        + suffix, "", 3000,    0, 3000);
  h_mc        [ichannel][icut][ijet] = new TH1D("h_mc"         + suffix, "", 3000,    0, 3000);
  h_ht        [ichannel][icut][ijet] = new TH1D("h_ht"         + suffix, "", 3000,    0, 3000);
  h_pt1       [ichannel][icut][ijet] = new TH1D("h_pt1"        + suffix, "", 3000,    0, 3000);
  h_pt2       [ichannel][icut][ijet] = new TH1D("h_pt2"        + suffix, "", 3000,    0, 3000);
  h_pt2l      [ichannel][icut][ijet] = new TH1D("h_pt2l"       + suffix, "", 3000,    0, 3000);
  h_ptww      [ichannel][icut][ijet] = new TH1D("h_ptww"       + suffix, "", 3000,    0, 3000);
}


//------------------------------------------------------------------------------
// GetDeltaPhill
//------------------------------------------------------------------------------
void AnalysisCMS::GetDeltaPhill()
{
  _deltaphill = fabs(Lepton1.v.DeltaPhi(Lepton2.v));
}
