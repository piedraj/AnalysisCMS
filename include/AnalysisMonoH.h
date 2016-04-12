#ifndef AnalysisMonoH_h
#define AnalysisMonoH_h

#include "AnalysisCMS.h"
#include "CutsMonoH.h"


class AnalysisMonoH : public AnalysisCMS
{
 public :

  AnalysisMonoH(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

  void GetDeltaR             ();

  // Data members
  //----------------------------------------------------------------------------
  float _deltarjet1met;
  float _deltarjet2met;
  float _deltarjj;
  float _deltarjjmet;
  float _deltarlep1jet1;
  float _deltarlep1jet2;
  float _deltarlep2jet1;
  float _deltarlep2jet2;
  float _deltarllmet;
  float _deltarl1met;
  float _deltarl2met;

  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                h_fullpmet       [nchannel][ncut][njetbin+1];
  TH1D*                h_trkpmet        [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarl1met    [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarl2met    [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarllmet    [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarjet1met  [nchannel][ncut][njetbin+1];  
  TH1D*                h_deltarjet2met  [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarjj       [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarjjmet    [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarlep1jet1 [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarlep1jet2 [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarlep2jet1 [nchannel][ncut][njetbin+1];
  TH1D*                h_deltarlep2jet2 [nchannel][ncut][njetbin+1];
  TH1D*                h_mllstar        [nchannel][ncut][njetbin+1];
  //TH1D*              h_mr             [nchannel][ncut][njetbin+1];
  
  TH2D*                h_metvar_m2l     [nchannel][ncut][njetbin+1];
  //TH2D*              h_met_deltaphill [nchannel][ncut][njetbin+1];
  //TH2D*              h_met_m2l        [nchannel][ncut][njetbin+1];  
};

#endif
