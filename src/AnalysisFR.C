#define AnalysisFR_cxx
#include "../include/AnalysisFR.h"


const Double_t muonjetet[njetet] = {10, 15, 20, 25, 30, 35, 45}; 
const Double_t elejetet [njetet] = {10, 15, 20, 25, 30, 35, 45}; 

const Double_t ptbins[nptbin+1] = {10, 15, 20, 25, 30, 35, 40, 45, 50};

const Double_t etabins[netabin+1] = {0, 0.5, 1.0, 1.5, 2.0, 2.5};


//------------------------------------------------------------------------------
// AnalysisFR
//------------------------------------------------------------------------------
AnalysisFR::AnalysisFR(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisFR::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  h_Ele_loose_pt_eta_PR  = new TH2D("h_Ele_loose_pt_eta_PR",  "h_Ele_loose_PR",  nptbin, ptbins, netabin, etabins);
  h_Ele_tight_pt_eta_PR  = new TH2D("h_Ele_tight_pt_eta_PR",  "h_Ele_tight_PR",  nptbin, ptbins, netabin, etabins);
  h_Muon_loose_pt_eta_PR = new TH2D("h_Muon_loose_pt_eta_PR", "h_Muon_loose_PR", nptbin, ptbins, netabin, etabins);
  h_Muon_tight_pt_eta_PR = new TH2D("h_Muon_tight_pt_eta_PR", "h_Muon_tight_PR", nptbin, ptbins, netabin, etabins);

  h_Ele_loose_pt_PR  = new TH1D("h_Ele_loose_pt_PR",  "h_Ele_loose_PR",  nptbin, ptbins);
  h_Ele_tight_pt_PR  = new TH1D("h_Ele_tight_pt_PR",  "h_Ele_tight_PR",  nptbin, ptbins);
  h_Muon_loose_pt_PR = new TH1D("h_Muon_loose_pt_PR", "h_Muon_loose_PR", nptbin, ptbins);
  h_Muon_tight_pt_PR = new TH1D("h_Muon_tight_pt_PR", "h_Muon_tight_PR", nptbin, ptbins);

  h_Ele_loose_eta_PR  = new TH1D("h_Ele_loose_eta_PR",  "h_Ele_loose_PR",  netabin, etabins);
  h_Ele_tight_eta_PR  = new TH1D("h_Ele_tight_eta_PR",  "h_Ele_tight_PR",  netabin, etabins);
  h_Muon_loose_eta_PR = new TH1D("h_Muon_loose_eta_PR", "h_Muon_loose_PR", netabin, etabins);
  h_Muon_tight_eta_PR = new TH1D("h_Muon_tight_eta_PR", "h_Muon_tight_PR", netabin, etabins);

  for (int i=0; i<ncut; i++) {
    
    TString directory = scut[i];

    root_output->cd();
    gDirectory->mkdir(directory);
    root_output->cd(directory);
      
    for (int j=0; j<njetet; j++) {

      TString muonsuffix = Form("_%.0fGev", muonjetet[j]);
      TString elesuffix  = Form("_%.0fGev", elejetet[j]);
    
      h_Muon_loose_pt_eta_bin[i][j] = new TH2D("h_Muon_loose_pt_eta_bin" + muonsuffix, "h_Muon_loose_pt_eta_bin", nptbin, ptbins, netabin, etabins);
      h_Muon_tight_pt_eta_bin[i][j] = new TH2D("h_Muon_tight_pt_eta_bin" + muonsuffix, "h_Muon_tight_pt_eta_bin", nptbin, ptbins, netabin, etabins);
      
      h_Muon_loose_pt_bin[i][j] = new TH1D("h_Muon_loose_pt_bin" + muonsuffix, "h_Muon_loose_pt_bin", nptbin, ptbins);
      h_Muon_tight_pt_bin[i][j] = new TH1D("h_Muon_tight_pt_bin" + muonsuffix, "h_Muon_tight_pt_bin", nptbin, ptbins);
      
      h_Muon_loose_eta_bin[i][j] = new TH1D("h_Muon_loose_eta_bin" + muonsuffix, "h_Muon_loose_eta_bin", netabin, etabins);
      h_Muon_tight_eta_bin[i][j] = new TH1D("h_Muon_tight_eta_bin" + muonsuffix, "h_Muon_tight_eta_bin", netabin, etabins);

      h_Ele_loose_pt_eta_bin[i][j] = new TH2D("h_Ele_loose_pt_eta_bin" + elesuffix, "h_Ele_loose_pt_eta_bin", nptbin, ptbins, netabin, etabins);
      h_Ele_tight_pt_eta_bin[i][j] = new TH2D("h_Ele_tight_pt_eta_bin" + elesuffix, "h_Ele_tight_pt_eta_bin", nptbin, ptbins, netabin, etabins);

      h_Ele_loose_pt_bin[i][j] = new TH1D("h_Ele_loose_pt_bin" + elesuffix, "h_Ele_loose_pt_bin", nptbin, ptbins);
      h_Ele_tight_pt_bin[i][j] = new TH1D("h_Ele_tight_pt_bin" + elesuffix, "h_Ele_tight_pt_bin", nptbin, ptbins);

      h_Ele_loose_eta_bin[i][j] = new TH1D("h_Ele_loose_eta_bin" + elesuffix, "h_Ele_loose_eta_bin", netabin, etabins);
      h_Ele_tight_eta_bin[i][j] = new TH1D("h_Ele_tight_eta_bin" + elesuffix, "h_Ele_tight_eta_bin", netabin, etabins);

      h_Muon_loose_pt[i][j] = new TH1D("h_Muon_loose_pt" + muonsuffix, "h_Muon_loose_pt", 1000, 0, 200);
      h_Muon_tight_pt[i][j] = new TH1D("h_Muon_tight_pt" + muonsuffix, "h_Muon_tight_pt", 1000, 0, 200);
      h_Ele_loose_pt [i][j] = new TH1D("h_Ele_loose_pt"  + elesuffix,  "h_Ele_loose_pt",  1000, 0, 200);
      h_Ele_tight_pt [i][j] = new TH1D("h_Ele_tight_pt"  + elesuffix,  "h_Ele_tight_pt",  1000, 0, 200);

      h_Muon_loose_mtw[i][j] = new TH1D("h_Muon_loose_mtw" + muonsuffix, "h_Muon_loose_mtw", 1000, 0, 200);
      h_Muon_tight_mtw[i][j] = new TH1D("h_Muon_tight_mtw" + muonsuffix, "h_Muon_tight_mtw", 1000, 0, 200);
      h_Ele_loose_mtw [i][j] = new TH1D("h_Ele_loose_mtw"  + elesuffix,  "h_Ele_loose_mtw",  1000, 0, 200);
      h_Ele_tight_mtw [i][j] = new TH1D("h_Ele_tight_mtw"  + elesuffix,  "h_Ele_tight_mtw",  1000, 0, 200);

      h_Muon_loose_m2l[i][j] = new TH1D("h_Muon_loose_m2l" + muonsuffix, "h_Muon_loose_m2l", 1000, 0, 200);
      h_Muon_tight_m2l[i][j] = new TH1D("h_Muon_tight_m2l" + muonsuffix, "h_Muon_tight_m2l", 1000, 0, 200);
      h_Ele_loose_m2l [i][j] = new TH1D("h_Ele_loose_m2l"  + elesuffix,  "h_Ele_loose_m2l",  1000, 0, 200);
      h_Ele_tight_m2l [i][j] = new TH1D("h_Ele_tight_m2l"  + elesuffix,  "h_Ele_tight_m2l",  1000, 0, 200);
      
      h_Muon_loose_pt_m2l[i][j] = new TH2D("h_Muon_loose_pt_m2l" + muonsuffix, "h_Muon_loose_pt_m2l", 200, 0, 200, nptbin, ptbins);
      h_Muon_tight_pt_m2l[i][j] = new TH2D("h_Muon_tight_pt_m2l" + muonsuffix, "h_Muon_tight_pt_m2l", 200, 0, 200, nptbin, ptbins);
      h_Ele_loose_pt_m2l [i][j] = new TH2D("h_Ele_loose_pt_m2l"  + elesuffix,  "h_Ele_loose_pt_m2l",  200, 0, 200, nptbin, ptbins);
      h_Ele_tight_pt_m2l [i][j] = new TH2D("h_Ele_tight_pt_m2l"  + elesuffix,  "h_Ele_tight_pt_m2l",  200, 0, 200, nptbin, ptbins);
    }
  }

  root_output -> cd();


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    _Zlepton1type  = Loose;
    _Zlepton2type  = Loose;
    _Zdecayflavour = 0;

    _channel = (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) ? e : m;

    if (_channel == e && Lepton1.v.Pt() <= 13) continue;
    if (_channel == m && Lepton1.v.Pt() <= 10) continue;

    if (_channel == e && Lepton1.v.Eta() >= 2.5) continue;
    if (_channel == m && Lepton1.v.Eta() >= 2.4) continue;


    // Make Z candidate
    //--------------------------------------------------------------------------
    _m2l = -999;

    if (AnalysisLeptons.size() >= 2) { 

      for (int iLep1=0; iLep1<AnalysisLeptons.size(); iLep1++) {
	
	if (AnalysisLeptons[iLep1].v.Pt() < 10.) continue;

	for (int iLep2=iLep1+1; iLep2<AnalysisLeptons.size(); iLep2++) {
	  
	  if (AnalysisLeptons[iLep2].v.Pt() < 10.) continue;

	  if ((AnalysisLeptons[iLep1].flavour + AnalysisLeptons[iLep2].flavour) != 0.) continue;
	  
	  float inv_mass = (AnalysisLeptons[iLep1].v + AnalysisLeptons[iLep2].v).M();

	  if (_m2l < 00 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

	    _m2l = inv_mass;

	    _Zlepton1type = AnalysisLeptons[iLep1].type;  
	    _Zlepton2type = AnalysisLeptons[iLep2].type;

	    _Zlepton1index = iLep1;
	    _Zlepton2index = iLep2;

	    _Zdecayflavour = AnalysisLeptons[iLep1].flavour; 
	  }
	}
      }
    }


    // Get the event weight
    //--------------------------------------------------------------------------
    bool passTrigger = true;

    if (_ismc) {

      _event_weight = puW * baseW * GEN_weight_SM / abs(GEN_weight_SM);

    } else {

      _event_weight = 1.0;

      passTrigger = false;


      // Muons
      //------------------------------------------------------------------------
      if (_channel == m) {

	if (Lepton1.v.Pt() <= 20. && std_vector_trigger->at(22)) {  // HLT_Mu8_TrkIsoVVL_v*

	  passTrigger= true;

	  // [2016/07/14]
	  // brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu8_TrkIsoVVL_v*"
	  _event_weight = (1e3 / 4.391);

	} else if (Lepton1.v.Pt() > 20. && std_vector_trigger->at(23)) {  // HLT_Mu17_TrkIsoVVL_v*

	  passTrigger = true;

	  // [2016/07/14]
	  // brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Mu17_TrkIsoVVL_v*"
	  _event_weight = (1e3 / 91.222);
	}
      }


      // Electrons
      //------------------------------------------------------------------------
      if (_channel == e) {
	
	if (Lepton1.v.Pt() <= 25. && std_vector_trigger->at(31)) {  // HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v*

	  passTrigger = true;

	  // [2016/07/14]
	  // brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v*"
	  _event_weight = (1e3 / 3.972);
	  
	} else if (Lepton1.v.Pt() > 25. && std_vector_trigger->at(33)) {  // HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v*

	  passTrigger = true;
	  
	  // [2016/07/14]
	  // brilcalc lumi -b "STABLE BEAMS" --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -u /pb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt --hltpath "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v*"
	  _event_weight = (1e3 / 21.165);
	}
      }
    }


    // Prompt rate from MC
    //--------------------------------------------------------------------------
    if (filename.Contains("DYJetsToLL") && 76. < _m2l && 106. > _m2l && _Zlepton1type == Tight) {
      
      float Zlep2pt  = AnalysisLeptons[_Zlepton2index].v.Pt();
      float Zlep2eta = AnalysisLeptons[_Zlepton2index].v.Eta();
      
      if (fabs(_Zdecayflavour) == ELECTRON_FLAVOUR) {

	h_Ele_loose_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _event_weight);
	h_Ele_loose_pt_PR    ->Fill(Zlep2pt,  _event_weight);
	h_Ele_loose_eta_PR   ->Fill(Zlep2eta, _event_weight);

	if (_Zlepton2type == Tight) {
	  
	  h_Ele_tight_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _event_weight);
	  h_Ele_tight_pt_PR    ->Fill(Zlep2pt,  _event_weight);
	  h_Ele_tight_eta_PR   ->Fill(Zlep2eta, _event_weight);
	}

      }	else if (fabs(_Zdecayflavour) == MUON_FLAVOUR) {
	
	h_Muon_loose_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _event_weight);
	h_Muon_loose_pt_PR    ->Fill(Zlep2pt,  _event_weight);
	h_Muon_loose_eta_PR   ->Fill(Zlep2eta, _event_weight);

	if (_Zlepton2type == Tight) {

	  h_Muon_tight_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _event_weight);
	  h_Muon_tight_pt_PR    ->Fill(Zlep2pt,  _event_weight);
	  h_Muon_tight_eta_PR   ->Fill(Zlep2eta, _event_weight);
	}
      }
    }


    // Preselection
    //--------------------------------------------------------------------------
    if (!passTrigger)             continue;    
    if (!PassJetSelection())      continue;
    if (AnalysisJets.size() < 1 ) continue;

    for (int i=0; i<njetet; i++) {

      if      (_channel == e) _inputJetEt = elejetet [i];
      else if (_channel == m) _inputJetEt = muonjetet[i];

      if (AnalysisJets[0].v.Pt() < _inputJetEt) continue; 


      // Selection
      //------------------------------------------------------------------------
      GetMt(Lepton1, _mtw);

      bool pass;
  
      pass =  (MET.Et() < 20.);
      pass &= (_mtw < 20.);
      pass &= (_nlepton == 1);

      FillLevelHistograms(FR_00_QCD, i, pass);

      pass =  (MET.Et() > 20.);
      pass &= (_mtw > 20.);
      pass &= (_nlepton == 1);
      
      FillLevelHistograms(FR_01_WRegion, i, pass);

      pass =  (MET.Et() < 20.);
      pass &= (_mtw > 20.);
      pass &= (_nlepton == 1);

      FillLevelHistograms(FR_02_WRegionQCD, i, pass);

      pass =  (MET.Et() > 20.);
      pass &= (_mtw > 20.);
      pass &= (_nlepton >= 2);
      pass &= _m2l > 0;

      if (pass && fabs(_Zdecayflavour) == ELECTRON_FLAVOUR) {
 
	h_Ele_loose_m2l   [FR_03_ZRegion][i]->Fill(_m2l, _event_weight);     
	h_Ele_loose_pt_m2l[FR_03_ZRegion][i]->Fill(_m2l, Lepton1.v.Pt(), _event_weight);

	if (_Zlepton1type == Tight && _Zlepton2type == Tight) {

	  h_Ele_tight_m2l   [FR_03_ZRegion][i]->Fill(_m2l, _event_weight);
	  h_Ele_tight_pt_m2l[FR_03_ZRegion][i]->Fill(_m2l, Lepton1.v.Pt(), _event_weight);
	}

      } else if (pass && fabs(_Zdecayflavour) == MUON_FLAVOUR) {

	h_Muon_loose_m2l   [FR_03_ZRegion][i]->Fill(_m2l, _event_weight);
	h_Muon_loose_pt_m2l[FR_03_ZRegion][i]->Fill(_m2l, Lepton1.v.Pt(), _event_weight);

	if (_Zlepton1type == Tight && _Zlepton2type == Tight) {

	  h_Muon_tight_m2l   [FR_03_ZRegion][i]->Fill(_m2l, _event_weight);
	  h_Muon_tight_pt_m2l[FR_03_ZRegion][i]->Fill(_m2l, Lepton1.v.Pt(), _event_weight);
	} 
      }

      pass =  (MET.Et() < 20.);
      pass &= (_mtw < 20.);
      pass &= (_nlepton >= 2);
      pass &= _m2l > 0;
      
      if (pass && fabs(_Zdecayflavour) == ELECTRON_FLAVOUR) {
 
	h_Ele_loose_m2l[FR_04_ZRegionQCD][i]->Fill(_m2l, _event_weight);     
     
	if (_Zlepton1type == Tight && _Zlepton2type == Tight) {

	  h_Ele_tight_m2l[FR_04_ZRegionQCD][i]->Fill(_m2l, _event_weight);
	}

      } else if (pass && fabs(_Zdecayflavour) == MUON_FLAVOUR) {
	
	h_Muon_loose_m2l[FR_04_ZRegionQCD][i] -> Fill(_m2l, _event_weight);
	
	if (_Zlepton1type == Tight && _Zlepton2type == Tight) {

	  h_Muon_tight_m2l[FR_04_ZRegionQCD][i] -> Fill(_m2l, _event_weight);
	} 
      }
    }
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillLevelHistograms(int icut, int i, bool pass)
{
  if (!pass) return;

  FillAnalysisHistograms(icut, i);
}


//------------------------------------------------------------------------------   
// FillanalysisHistograms
//------------------------------------------------------------------------------                                                                        
void AnalysisFR::FillAnalysisHistograms(int icut, int i)
{
  if (Lepton1.type == Loose || Lepton1.type == Tight) {

    if (_channel == m) {

      h_Muon_loose_pt_eta_bin[icut][i]->Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight);
      h_Muon_loose_pt_bin    [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Muon_loose_eta_bin   [icut][i]->Fill(Lepton1.v.Eta(), _event_weight);
     
      h_Muon_loose_pt [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Muon_loose_mtw[icut][i]->Fill(_mtw,           _event_weight);

    } else if (_channel == e) {

      h_Ele_loose_pt_eta_bin[icut][i]->Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight);
      h_Ele_loose_pt_bin    [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Ele_loose_eta_bin   [icut][i]->Fill(Lepton1.v.Eta(), _event_weight);
      
      h_Ele_loose_pt [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Ele_loose_mtw[icut][i]->Fill(_mtw,           _event_weight);
    }
  }
    
  if (Lepton1.type == Tight) {
   
    if (_channel == m) {

      h_Muon_tight_pt_eta_bin[icut][i]->Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight);
      h_Muon_tight_pt_bin    [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Muon_tight_eta_bin   [icut][i]->Fill(Lepton1.v.Eta(), _event_weight);
      
      h_Muon_tight_pt [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Muon_tight_mtw[icut][i]->Fill(_mtw,           _event_weight);	

    } else if (_channel == e) {

      h_Ele_tight_pt_eta_bin[icut][i]->Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight);
      h_Ele_tight_pt_bin    [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Ele_tight_eta_bin   [icut][i]->Fill(Lepton1.v.Eta(), _event_weight);
	
      h_Ele_tight_pt [icut][i]->Fill(Lepton1.v.Pt(), _event_weight);
      h_Ele_tight_mtw[icut][i]->Fill(_mtw,           _event_weight);
    }
  }
}


//------------------------------------------------------------------------------
// PassJetSelection
//------------------------------------------------------------------------------
bool AnalysisFR::PassJetSelection()
{
  bool pass = false;

  AnalysisJets.clear();
   
  int jetsize = std_vector_jet_pt->size();
        
  for (int j=0; j<jetsize; j++) {
      
    Jet jet_;
      
    jet_.index = j; 
      
    float pt  = std_vector_jet_pt ->at(j);
    float eta = std_vector_jet_eta->at(j);
    float phi = std_vector_jet_phi->at(j);

    if (pt <= 10) continue;

    TLorentzVector tlv;

    tlv.SetPtEtaPhiM(pt, eta, phi, 0);
      
    jet_.v = tlv;

    float dR = tlv.DeltaR(AnalysisLeptons[0].v);

    if (dR > 1) {

      pass = true;

      AnalysisJets.push_back(jet_);
    }
  }

  return pass;
}
