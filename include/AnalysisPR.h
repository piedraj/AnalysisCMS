#ifndef AnalysisPR_h
#define AnalysisPR_h

#include "AnalysisCMS.h"

// Constants
//------------------------------------------------------------------------------
const int nptbin = 8;
const Double_t ptbins[nptbin+1] = {10, 15, 20, 25, 30, 35, 40, 45, 50};

const int netabin = 5;
const Double_t etabins[netabin+1] = {0, 0.5, 1.0, 1.5, 2.0, 2.5};

class AnalysisPR: public AnalysisCMS
{
 public :

  AnalysisPR(TTree* tree, TString systematic);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

  // Data members
  //----------------------------------------------------------------------------
  float _event_weight;
  float _inputJetEt;
  float _leptonPtMin;
  float _leptonEtaMax;

  int   _Zlepton1type;
  int   _Zlepton2type;
  int   _Zdecayflavour;

  // Declare prompt rate histograms
  //----------------------------------------------------------------------------
  TH2D* h_Muon_loose_pt_eta_PR;
  TH2D* h_Muon_tight_pt_eta_PR;
  TH2D* h_Ele_loose_pt_eta_PR;
  TH2D* h_Ele_tight_pt_eta_PR;

  TH1D* h_Muon_loose_pt_PR;
  TH1D* h_Muon_tight_pt_PR;
  TH1D* h_Ele_loose_pt_PR;
  TH1D* h_Ele_tight_pt_PR;

  TH1D* h_Muon_loose_eta_PR;
  TH1D* h_Muon_tight_eta_PR;
  TH1D* h_Ele_loose_eta_PR;
  TH1D* h_Ele_tight_eta_PR;
};

#endif 
