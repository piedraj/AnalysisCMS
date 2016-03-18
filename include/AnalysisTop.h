#ifndef AnalysisTop_h
#define AnalysisTop_h

#include "AnalysisCMS.h"


class AnalysisTop : public AnalysisCMS
{
 public :

  AnalysisTop(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);


  // Data members
  //----------------------------------------------------------------------------



  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_test[nchannel][ncut][njetbin+1];
  TH1D*                  h_htjets[nchannel][ncut][njetbin+1];
  TH2F*                  h_2ht [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet1 [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet2 [nchannel][ncut][njetbin+1];
  TH1D*                  h_jetpt1    [nchannel][ncut][njetbin+1];
  TH1D*                  h_jetpt2    [nchannel][ncut][njetbin+1];
};

#endif
