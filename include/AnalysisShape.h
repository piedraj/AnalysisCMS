#ifndef AnalysisShape_h
#define AnalysisShape_h

#include "AnalysisCMS.h"

class AnalysisShape : public AnalysisCMS
{
 public :

  AnalysisShape(TTree* tree, TString systematic);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

};

#endif
