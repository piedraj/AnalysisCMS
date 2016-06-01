#ifndef AnalysisFR_h
#define AnalysisFR_h

#include "AnalysisCMS.h"

class AnalysisFR: public AnalysisCMS
{
 public :

  AnalysisFR(TTree* tree, TString systematic);

  void FillAnalysisHistograms(int icut);

  void FillLevelHistograms(int icut, 
			   bool pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
  bool PassJetSelection      ();

  // Data members                                                                                                                                          
  //----------------------------------------------------------------------------                                                                           

  // Analysis histograms                                                                                                                                   
  //----------------------------------------------------------------------------                                                                           

  TH2D* h_Muon_loose_pt_eta_bin[ncut];
  TH2D* h_Muon_tight_pt_eta_bin[ncut];

  TH1D* h_Muon_loose_pt_bin[ncut];
  TH1D* h_Muon_tight_pt_bin[ncut];

  TH1D* h_Muon_loose_eta_bin[ncut];
  TH1D* h_Muon_tight_eta_bin[ncut];

  TH2D* h_Ele_loose_pt_eta_bin[ncut];
  TH2D* h_Ele_tight_pt_eta_bin[ncut];

  TH1D* h_Ele_loose_pt_bin[ncut];
  TH1D* h_Ele_tight_pt_bin[ncut];

  TH1D* h_Ele_loose_eta_bin[ncut];
  TH1D* h_Ele_tight_eta_bin[ncut];

  TH1D* h_Muon_loose_pt[ncut];
  TH1D* h_Muon_tight_pt[ncut];
  TH1D* h_Ele_loose_pt[ncut];
  TH1D* h_Ele_tight_pt[ncut];

  TH1D* h_Muon_loose_mtw[ncut];
  TH1D* h_Muon_tight_mtw[ncut];
  TH1D* h_Ele_loose_mtw[ncut];
  TH1D* h_Ele_tight_mtw[ncut];

  TH1D* h_Muon_loose_m2l[ncut];
  TH1D* h_Muon_tight_m2l[ncut];
  TH1D* h_Ele_loose_m2l[ncut];
  TH1D* h_Ele_tight_m2l[ncut];

};

#endif 
