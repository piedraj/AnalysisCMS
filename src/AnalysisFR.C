#define AnalysisFR_cxx
#include "../include/AnalysisFR.h"

float _event_weight_fr;
float inputJetEt;

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;

float _luminosity = 2.263;

int pTbin = 8;
Double_t pTbins[9] = { 10., 15., 20., 25., 30., 35.,40., 45., 50.};

int etabin = 5;
Double_t etabins[6] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5};

//------------------------------------------------------------------------------
// AnalysisFR
//------------------------------------------------------------------------------
AnalysisFR::AnalysisFR(TTree* tree) : AnalysisCMS(tree)
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

  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=e; i<=m; i++) { 

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);

	h_loose  [i][j][k] = new TH1D("h_loose" + suffix,  "", 100, 0, 100); 
	h_tight  [i][j][k] = new TH1D("h_tight" + suffix,  "", 100, 0, 100); 
        
	h_Muon_loose_pt_eta_bin   = new TH2F("h_Muon_loose_pt_eta_bin", "h_Muon_loose_pt_eta_bin", pTbin, pTbins, etabin, etabins);
	h_Muon_tight_pt_eta_bin = new TH2F("h_Muon_tight_pt_eta_bin", "h_Muon_tight_pt_eta_bin", pTbin, pTbins, etabin, etabins);

	h_Ele_loose_pt_eta_bin    = new TH2F("h_Ele_loose_pt_eta_bin", "h_Ele_loose_pt_eta_bin", pTbin, pTbins, etabin, etabins);
	h_Ele_tight_pt_eta_bin  = new TH2F("h_Ele_tight_pt_eta_bin", "h_Ele_tight_pt_eta_bin", pTbin, pTbins, etabin, etabins);

      }
    }
  }

  root_output->cd();

  // Loop over events
  //--------------------------------------------------------------------------

    for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    // Analysis
    //--------------------------------------------------------------------------

    // Z+jets

    if (_ismc && filename.Contains("DYJetsToLL")) {

      Double_t genWeight = 1.0;
      genWeight = GEN_weight_SM/abs(GEN_weight_SM);
      _event_weight_fr = puW * baseW * _luminosity * genWeight;

      for (int iMu1 = 0; iMu1 < AnalysisLeptons.size(); iMu1++) {

	if (AnalysisLeptons[iMu1].v.Pt() < 10.) continue;
	if (AnalysisLeptons[iMu1].type != Tight) continue; 
	if (AnalysisLeptons[iMu1].flavour != ELECTRON_FLAVOUR && AnalysisLeptons[iMu1].flavour != MUON_FLAVOUR) continue;

	for (int iMu2 = iMu1+1; iMu2 < AnalysisLeptons.size(); iMu2++) {
	  
	  if (AnalysisLeptons[iMu2].v.Pt() < 10.) continue;
	  if (fabs(AnalysisLeptons[iMu1].flavour) !=fabs( AnalysisLeptons[iMu2].flavour)) continue;
	  if ((AnalysisLeptons[iMu1].flavour + AnalysisLeptons[iMu2].flavour) != 0.) continue;

	  float inv_mass = (AnalysisLeptons[iMu1].v+AnalysisLeptons[iMu2].v).M();

	  if (inv_mass < 70. || inv_mass > 110.) continue;

	  float pt = std_vector_lepton_pt -> at(iMu2);
	  float eta = fabs(std_vector_lepton_eta -> at(iMu2));

	  if (AnalysisLeptons[iMu1].flavour == MUON_FLAVOUR) {

	    h_Muon_loose_pt_eta_bin -> Fill(pt, eta, _event_weight_fr);

	    if (AnalysisLeptons[iMu2].type == Tight) {

	      h_Muon_tight_pt_eta_bin -> Fill(pt, eta, _event_weight_fr);

	    }
	  }

	  else if (AnalysisLeptons[iMu1].flavour == ELECTRON_FLAVOUR) {

	    h_Ele_loose_pt_eta_bin -> Fill(pt, eta, _event_weight_fr);

	    if (AnalysisLeptons[iMu2].type == Tight) {

	      h_Ele_tight_pt_eta_bin -> Fill(pt, eta, _event_weight_fr);

	    }
	  }
	}
      }
    }

    _channel = (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) ? e : m;

    // Data and W+jets analysis

    bool passTrigger = true;
    _event_weight_fr = 1.0;

    if (_channel == e && Lepton1.v.Pt() <= 13) continue;
    if (_channel == m && Lepton1.v.Pt() <= 10) continue;
    if (_channel == e && Lepton1.v.Eta() >= 2.5 ) continue;
    if (_channel == m && Lepton1.v.Eta() >= 2.4 ) continue;

    if (_nlepton != 1) continue;

    if (!_ismc) {

      passTrigger = false;

      if (_channel == m) {

	if (Lepton1.v.Pt() > 10. && Lepton1.v.Pt() <= 20. && (std_vector_trigger -> at(22) == 1) ) { //Lumi HLT_Mu8_TrkIsoVVL: 1.386 pb

	  if (Lepton1.type == Tight && Lepton1.v.Pt() > 15.) printf(" We have tight muons with pt = %f\n", Lepton1.v.Pt());

	  passTrigger= true;
	  _event_weight_fr *= (_luminosity*1000 / 1.386);

	}
	else if (Lepton1.v.Pt() > 20. && (std_vector_trigger->at(23) == 1) ) { //Lumi HLT_Mu17_TrkIsoVVL: 201.951 pb

	  passTrigger = true;
	  _event_weight_fr *= (_luminosity*1000 / 201.951);

	}
    
      }

      if (_channel == e) {
	counter2 ++;
	if (Lepton1.v.Pt() > 13. && Lepton1.v.Pt() <= 25. && (std_vector_trigger->at(31) == 1) ) { //Lumi HLT_Ele8_Iso: 11.204 pb;

	  passTrigger = true;
	  _event_weight_fr *= (_luminosity*1000 / 11.204);
	 
	} else if (Lepton1.v.Pt() > 25. && (std_vector_trigger->at(33) == 1) ) { //Lumi HLT_Ele23_Iso: 3.201

	  passTrigger = true;
	  _event_weight_fr *= (_luminosity*1000 / 3.201);
	
       	}
  
      }
    
    }

    if (!passTrigger) continue;
    
    if (!PassJetSelection()) continue;
    if (AnalysisJets.size() < 1 ) continue;

    // Temporary solution
    if (_channel == e) {
      inputJetEt = 20.;
    } else if (_channel == m) {
      inputJetEt = 20.;
    }

    if (AnalysisJets[0].v.Pt() <  inputJetEt) continue; 

    // Selection
    //--------------------------------------------------------------------------
    
    GetMt(Lepton1, _mtw);
     
    bool pass = true;

    pass &= (MET.Et() < 20.);
    pass &= (_mtw < 20.);

    if (pass & Lepton1.type == Loose || Lepton1.type == Tight) {
      if (_channel == m) {
	h_Muon_loose_pt_eta_bin -> Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight_fr);
      } else if (_channel == e) {
	h_Ele_loose_pt_eta_bin -> Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight_fr);
      }
    }
    
    if (pass & Lepton1.type == Tight) {
      if (_channel == m) {
	h_Muon_tight_pt_eta_bin -> Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight_fr);
      } else if (_channel == e) {
	h_Ele_tight_pt_eta_bin -> Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight_fr);
      }
    }

    //FillLevelHistograms(FR_00_QCD, pass);
     
    if (pass && _saveminitree) minitree->Fill();
 
    }

  EndJob();
 
}

//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{

  /*
  if (Lepton1.type == Loose || Lepton1.type == Tight) {
    
    h_loose       [ichannel][icut][ijet] -> Fill(Lepton1.v.Pt(), _event_weight_fr);

  } 

  if (Lepton1.type == Tight) {

    h_tight       [ichannel][icut][ijet] -> Fill(Lepton1.v.Pt(), _event_weight_fr);

  }
  */
  //  h_Muon_fake_pt_eta_bin [ichannel][icut][ijet] -> Fill(Lepton1.v.Pt(), Lepton1.v.Eta(), _event_weight_fr); 

  //  if (ichannel != l) FillAnalysisHistograms(l, icut, ijet);

}

//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;
  
  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);

}


//------------------------------------------------------------------------------                                                                                               
// PassJetSelection
//------------------------------------------------------------------------------  

bool AnalysisFR::PassJetSelection()
{
  bool pass = false;

  AnalysisJets.clear();
   
  int jetsize = std_vector_jet_pt->size();
        
  for ( int j=0; j<jetsize; j++) {
      
    Jet jet_; 
      
    jet_.index = j; 
      
    float pt = std_vector_jet_pt->at(j);
    float eta = std_vector_jet_eta->at(j);
    float phi = std_vector_jet_phi->at(j);
    float btag = std_vector_jet_csvv2ivf->at(j);     

    if ( pt <= 10.0) continue;

    TLorentzVector jetvt;
    jetvt.SetPtEtaPhiM(pt, eta, phi, 0);
      
    jet_.v = jetvt;

    float dR =  jetvt.DeltaR(AnalysisLeptons[0].v);

    if ( dR > 1 )   {       
      AnalysisJets.push_back(jet_);
    }

  }

  pass = true;

  return pass;
}
