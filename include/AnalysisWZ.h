#ifndef AnalysisWZ_h
#define AnalysisWZ_h

#include "AnalysisCMS.h"


class AnalysisWZ : public AnalysisCMS
{
 public :

  AnalysisWZ(TTree* tree, TString systematic);

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
  Lepton                 WLepton;
  Lepton                 ZLepton1;
  Lepton                 ZLepton2;


  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_m3l       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw       [nchannel][ncut][njetbin+1];
  TH1D*                  h_zl1pt     [nchannel][ncut][njetbin+1];
  TH1D*                  h_zl2pt     [nchannel][ncut][njetbin+1];
  TH1D*                  h_wlpt      [nchannel][ncut][njetbin+1];
  TH1D*                  h_wlzldeltar[nchannel][ncut][njetbin+1];
};

#endif
