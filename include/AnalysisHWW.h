#ifndef AnalysisHWW_h
#define AnalysisHWW_h

#include "AnalysisCMS.h"


class AnalysisHWW : public AnalysisCMS
{
 public :

  AnalysisHWW(TTree* tree, TString systematic);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
};

#endif
