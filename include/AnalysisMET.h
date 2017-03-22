#ifndef AnalysisMET_h
#define AnalysisMET_h

#include "AnalysisCMS.h"


class AnalysisMET : public AnalysisCMS
{
 public :

  AnalysisMET(TTree* tree, TString systematic);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);
};

#endif
