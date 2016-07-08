#ifndef AnalysisSS_h
#define AnalysisSS_h

#include "AnalysisCMS.h"


class AnalysisSS : public AnalysisCMS
{
 public :

  AnalysisSS(TTree* tree, TString systematic);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
};

#endif
