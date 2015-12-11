#ifndef AnalysisMonoH_h
#define AnalysisMonoH_h

#include "AnalysisCMS.h"


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


  // Analysis histograms
  //----------------------------------------------------------------------------
  TH2D*                  h_metvar_m2l[nchannel][ncut][njetbin+1];
};

#endif
