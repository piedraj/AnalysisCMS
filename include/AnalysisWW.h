#ifndef AnalysisWW_h
#define AnalysisWW_h

#include "AnalysisCMS.h"


class AnalysisWW : public AnalysisCMS
{
 public :

  AnalysisWW(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString sample,
			      float   luminosity);


  // Analysis histograms
  //----------------------------------------------------------------------------
  TH2D*                  h_metvar_m2l[nchannel][ncut][njetbin+1];
};

#endif
