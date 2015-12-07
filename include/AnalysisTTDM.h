#ifndef AnalysisTTDM_h
#define AnalysisTTDM_h

#include "AnalysisCMS.h"


class AnalysisTTDM : public AnalysisCMS
{
 public :

  AnalysisTTDM(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);


  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_test[nchannel][ncut][njetbin+1];
};

#endif
