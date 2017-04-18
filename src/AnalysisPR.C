#define AnalysisPR_cxx
#include "../include/AnalysisPR.h"

//------------------------------------------------------------------------------
// AnalysisPR
//------------------------------------------------------------------------------
AnalysisPR::AnalysisPR(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisPR::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);

  root_output->cd();
  
  // Define prompt rate histograms
  //----------------------------------------------------------------------------
  h_Muon_loose_pt_eta_PR = new TH2D("h_Muon_loose_pt_eta_PR", "", nptbin, ptbins, netabin, etabins);
  h_Muon_tight_pt_eta_PR = new TH2D("h_Muon_tight_pt_eta_PR", "", nptbin, ptbins, netabin, etabins);
  h_Ele_loose_pt_eta_PR  = new TH2D("h_Ele_loose_pt_eta_PR",  "", nptbin, ptbins, netabin, etabins);
  h_Ele_tight_pt_eta_PR  = new TH2D("h_Ele_tight_pt_eta_PR",  "", nptbin, ptbins, netabin, etabins);
    
  h_Muon_loose_pt_PR = new TH1D("h_Muon_loose_pt_PR", "", nptbin, ptbins);
  h_Muon_tight_pt_PR = new TH1D("h_Muon_tight_pt_PR", "", nptbin, ptbins);
  h_Ele_loose_pt_PR  = new TH1D("h_Ele_loose_pt_PR",  "", nptbin, ptbins);
  h_Ele_tight_pt_PR  = new TH1D("h_Ele_tight_pt_PR",  "", nptbin, ptbins);
    
  h_Muon_loose_eta_PR = new TH1D("h_Muon_loose_eta_PR", "", netabin, etabins);
  h_Muon_tight_eta_PR = new TH1D("h_Muon_tight_eta_PR", "", netabin, etabins);
  h_Ele_loose_eta_PR  = new TH1D("h_Ele_loose_eta_PR",  "", netabin, etabins);
  h_Ele_tight_eta_PR  = new TH1D("h_Ele_tight_eta_PR",  "", netabin, etabins);


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;
    //    if (jentry > 1000) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    _Zlepton1type  = Loose;
    _Zlepton2type  = Loose;
    _Zdecayflavour = 0;

    _channel = (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) ? e : m;

    _leptonPtMin  = (_channel == e) ?  13 :  10;
    _leptonEtaMax = (_channel == e) ? 2.5 : 2.4;

    if (Lepton1.v.Pt()        < _leptonPtMin)  continue;
    if (fabs(Lepton1.v.Eta()) > _leptonEtaMax) continue;


    // Make Z candidate
    //--------------------------------------------------------------------------

    _m2l = -999;
    
    _l2tight_weight = 1.;
    
    if (AnalysisLeptons.size() >= 2) { 
      
      for (int iLep1=0; iLep1<AnalysisLeptons.size(); iLep1++) {
	
	if (AnalysisLeptons[iLep1].v.Pt() < 10.) continue;
	
	for (int iLep2=iLep1+1; iLep2<AnalysisLeptons.size(); iLep2++) {
	  
	  if (AnalysisLeptons[iLep2].v.Pt() < 10.) continue;
	  
	  if ((AnalysisLeptons[iLep1].flavour + AnalysisLeptons[iLep2].flavour) != 0.) continue;
	  
	  float inv_mass = (AnalysisLeptons[iLep1].v + AnalysisLeptons[iLep2].v).M();
	  
	  if (_m2l < 0 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {
	    
	    _m2l = inv_mass;
	    
	    _Zlepton1type = AnalysisLeptons[iLep1].type;  
	    _Zlepton2type = AnalysisLeptons[iLep2].type;
	    
	    if (_Zlepton1type == Tight && _Zlepton2type == Tight)
	      {
		_l2tight_weight = (AnalysisLeptons[iLep1].idisoW * AnalysisLeptons[iLep2].idisoW);
	      }
	    
	    _Zlepton1index = iLep1;
	    _Zlepton2index = iLep2;
	    
	    _Zdecayflavour = AnalysisLeptons[iLep1].flavour; 
	  }
	}
      }
    }
    

    // Get the event weight
    //--------------------------------------------------------------------------
    bool passTrigger;

    if (_ismc) {

      passTrigger = true;

      Float_t corrected_baseW = baseW; 

      //if (_sample.Contains("DYJetsToLL_M-10to50")) corrected_baseW = 0.829752445221; 
      //if (_sample.Contains("DYJetsToLL_M-50"))     corrected_baseW = 0.318902641535;

      _base_weight = (corrected_baseW / 1e3) * puW * GEN_weight_SM / abs(GEN_weight_SM);

      _event_weight = _base_weight;


      // Muons
      //------------------------------------------------------------------------
      if (_channel == m)
	{
	  (Lepton1.v.Pt() <= 20.) ? _event_weight *= 7.283 : _event_weight *= 217.234;
	}

      
      // Electrons
      //------------------------------------------------------------------------
      if (_channel == e)
	{
	  (Lepton1.v.Pt() <= 25.) ? _event_weight *= 13.866 : _event_weight *= 62.94;
	}

    } else {

      _event_weight = 1.0;

      passTrigger = false;


      // Muons
      //------------------------------------------------------------------------
      if (_sample.Contains("DoubleMuon") && _channel == m) {

	if (Lepton1.v.Pt() <= 20. && std_vector_trigger->at(22)) {  // HLT_Mu8_TrkIsoVVL_v*

	  passTrigger = true;

	} else if (Lepton1.v.Pt() > 20. && std_vector_trigger->at(23)) {  // HLT_Mu17_TrkIsoVVL_v*

	  passTrigger = true;
	}
      }


      // Electrons
      //------------------------------------------------------------------------
      if (_sample.Contains("DoubleEG") && _channel == e) {
	
	if (Lepton1.v.Pt() <= 25. && std_vector_trigger->at(31)) {  // HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v*

	  passTrigger = true;
	  
	} else if (Lepton1.v.Pt() > 25. && std_vector_trigger->at(33)) {  // HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v*

	  passTrigger = true;
	}
      }
    }


    // Prompt rate from MC
    //--------------------------------------------------------------------------
    if (_sample.Contains("DYJetsToLL") && 76. < _m2l && 106. > _m2l && _Zlepton1type == Tight && _mtw < 20.) {

      float Zlep2pt  = AnalysisLeptons[_Zlepton2index].v.Pt();
      float Zlep2eta = fabs(AnalysisLeptons[_Zlepton2index].v.Eta());
      
      if (fabs(_Zdecayflavour) == ELECTRON_FLAVOUR) {
      
	h_Ele_loose_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _base_weight);
	h_Ele_loose_pt_PR    ->Fill(Zlep2pt,  _base_weight);
	h_Ele_loose_eta_PR   ->Fill(Zlep2eta, _base_weight);

	if (_Zlepton2type == Tight) {
      
	  h_Ele_tight_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _base_weight);
	  h_Ele_tight_pt_PR    ->Fill(Zlep2pt,  _base_weight);
	  h_Ele_tight_eta_PR   ->Fill(Zlep2eta, _base_weight);
	}

      }	else if (fabs(_Zdecayflavour) == MUON_FLAVOUR) {

	h_Muon_loose_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _base_weight);
	h_Muon_loose_pt_PR    ->Fill(Zlep2pt,  _base_weight);
	h_Muon_loose_eta_PR   ->Fill(Zlep2eta, _base_weight);

	if (_Zlepton2type == Tight) {

	  h_Muon_tight_pt_eta_PR->Fill(Zlep2pt, Zlep2eta, _base_weight);
	  h_Muon_tight_pt_PR    ->Fill(Zlep2pt,  _base_weight);
	  h_Muon_tight_eta_PR   ->Fill(Zlep2eta, _base_weight);
	}
      }
    }

  }


  EndJob();
}

