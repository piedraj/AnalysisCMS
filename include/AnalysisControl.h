#ifndef AnalysisControl_h
#define AnalysisControl_h

#include "AnalysisCMS.h"


class AnalysisControl : public AnalysisCMS
{
 public :

  AnalysisControl(TTree* tree, TString systematic);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
};

#endif
