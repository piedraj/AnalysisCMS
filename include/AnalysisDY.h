#ifndef AnalysisDY_h
#define AnalysisDY_h

#include "AnalysisCMS.h"


class AnalysisDY : public AnalysisCMS
{
 public :

  AnalysisDY(TTree* tree, TString systematic);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
};

#endif
