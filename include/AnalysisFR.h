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

  TH1D* h_loose [nchannel][ncut][njetbin+1];  
  TH1D* h_tight [nchannel][ncut][njetbin+1];

  TH1D* h_Muon_loose_counter;
  TH1D* h_Muon_tight_counter;

  TH1D* h_Ele_loose_counter;
  TH1D* h_Ele_tight_counter;
 
  TH2D* h_Muon_loose_pt_eta_bin;
  TH2D* h_Muon_tight_pt_eta_bin;

  TH2D* h_Ele_loose_pt_eta_bin;
  TH2D* h_Ele_tight_pt_eta_bin;

};

#endif 
