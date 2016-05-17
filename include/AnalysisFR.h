#ifndef AnalysisFR_h
#define AnalysisFR_h

#include "AnalysisCMS.h"

class AnalysisFR: public AnalysisCMS
{
 public :

  AnalysisFR(TTree* tree, TString systematic);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
  bool PassJetSelection      ();

  // Data members                                                                                                                                          
  //----------------------------------------------------------------------------                                                                           

  // Analysis histograms                                                                                                                                   
  //----------------------------------------------------------------------------                                                                           

  TH1D* h_zjets_loose;
  TH1D* h_zjets_tight;

  TH1D* h_wjets_loose;
  TH1D* h_wjets_tight;

  TH1D* h_data_loose;
  TH1D* h_data_tight;

  TH2D* h_Muon_loose_pt_eta_bin;
  TH2D* h_Muon_tight_pt_eta_bin;

  TH1D* h_Muon_loose_pt_bin;
  TH1D* h_Muon_tight_pt_bin;

  TH1D* h_Muon_loose_eta_bin;
  TH1D* h_Muon_tight_eta_bin;

  TH2D* h_Ele_loose_pt_eta_bin;
  TH2D* h_Ele_tight_pt_eta_bin;

  TH1D* h_Ele_loose_pt_bin;
  TH1D* h_Ele_tight_pt_bin;

  TH1D* h_Ele_loose_eta_bin;
  TH1D* h_Ele_tight_eta_bin;
  

};

#endif 
