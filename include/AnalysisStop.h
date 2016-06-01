#ifndef AnalysisStop_h
#define AnalysisStop_h
 
#include "AnalysisCMS.h"


class AnalysisStop : public AnalysisCMS
{
 public :

  AnalysisStop(TTree* tree, TString systematic);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

  float StopCrossSection, StopCrossSectionUncertainty;
  void GetStopCrossSection(float StopMass);


  // Analysis histograms
  //----------------------------------------------------------------------------
};

#endif
