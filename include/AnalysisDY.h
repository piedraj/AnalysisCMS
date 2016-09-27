#ifndef AnalysisDY_h
#define AnalysisDY_h

#include "AnalysisCMS.h"
#include "CutsDY.h"

class AnalysisDY : public AnalysisCMS
{
 public :

  AnalysisDY(TTree* tree, TString systematic);

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
};

#endif
