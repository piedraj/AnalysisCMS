#ifndef AnalysisFR_h
#define AnalysisFR_h

#include "AnalysisCMS.h"


// Constants
//------------------------------------------------------------------------------
const int njetet = 7; 

const Double_t muonjetet[njetet] = {10, 15, 20, 25, 30, 35, 45}; 
const Double_t elejetet [njetet] = {10, 15, 20, 25, 30, 35, 45}; 

const int nptbin = 8;

const Double_t ptbins[nptbin+1] = {10, 15, 20, 25, 30, 35, 40, 45, 50};

const int netabin = 5;

const Double_t etabins[netabin+1] = {0, 0.5, 1.0, 1.5, 2.0, 2.5};


class AnalysisFR: public AnalysisCMS
{
 public :

  AnalysisFR(TTree* tree, TString systematic);

  void FillAnalysisHistograms(int     icut,
			      int     i);

  void FillLevelHistograms   (int     icut,
			      int     i,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

  void GetAwayJets           ();


  // Data members
  //----------------------------------------------------------------------------
  float _event_weight;
  float _l2tight_weight;
  float _inputJetEt;
  float _leptonPtMin;
  float _leptonEtaMax;

  int   _Zlepton1type;
  int   _Zlepton2type;
  float _Zlepton1idisoW;
  float _Zlepton2idisoW;
  int   _Zdecayflavour;


  // Declare fake rate histograms
  //----------------------------------------------------------------------------
  TH2D* h_Muon_loose_pt_eta_bin[ncut][njetet];
  TH2D* h_Muon_tight_pt_eta_bin[ncut][njetet];
  TH2D* h_Ele_loose_pt_eta_bin [ncut][njetet];
  TH2D* h_Ele_tight_pt_eta_bin [ncut][njetet];

  TH1D* h_Muon_loose_pt_bin[ncut][njetet];
  TH1D* h_Muon_tight_pt_bin[ncut][njetet];
  TH1D* h_Ele_loose_pt_bin [ncut][njetet];
  TH1D* h_Ele_tight_pt_bin [ncut][njetet];

  TH1D* h_Muon_loose_eta_bin[ncut][njetet];
  TH1D* h_Muon_tight_eta_bin[ncut][njetet];
  TH1D* h_Ele_loose_eta_bin [ncut][njetet];
  TH1D* h_Ele_tight_eta_bin [ncut][njetet];

  TH1D* h_Muon_loose_pt[ncut][njetet];
  TH1D* h_Muon_tight_pt[ncut][njetet];
  TH1D* h_Ele_loose_pt [ncut][njetet];
  TH1D* h_Ele_tight_pt [ncut][njetet];

  TH1D* h_Muon_loose_mtw[ncut][njetet];
  TH1D* h_Muon_tight_mtw[ncut][njetet];
  TH1D* h_Ele_loose_mtw [ncut][njetet];
  TH1D* h_Ele_tight_mtw [ncut][njetet];

  TH1D* h_Muon_loose_m2l[ncut][njetet];
  TH1D* h_Muon_tight_m2l[ncut][njetet];
  TH1D* h_Ele_loose_m2l [ncut][njetet];
  TH1D* h_Ele_tight_m2l [ncut][njetet];

  TH1D* h_Muon_loose_met[ncut][njetet];
  TH1D* h_Muon_tight_met[ncut][njetet];
  TH1D* h_Ele_loose_met [ncut][njetet];
  TH1D* h_Ele_tight_met [ncut][njetet];


  // Declare effective luminosity estimation histograms
  //----------------------------------------------------------------------------
  TH2D* h_Muon_loose_pt_m2l[ncut][njetet];
  TH2D* h_Muon_tight_pt_m2l[ncut][njetet];
  TH2D* h_Ele_loose_pt_m2l [ncut][njetet];
  TH2D* h_Ele_tight_pt_m2l [ncut][njetet];
};

#endif 
