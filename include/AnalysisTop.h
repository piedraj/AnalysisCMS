#ifndef AnalysisTop_h
#define AnalysisTop_h

#include "AnalysisCMS.h"


class AnalysisTop : public AnalysisCMS
{
 public :

  AnalysisTop(TTree* tree, TString systematic);

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
  TH2F*                  h_2ht    [nchannel][ncut][njetbin+1];
  TH2D*                  h_met_m2l[nchannel][ncut][njetbin+1];
};

#endif
