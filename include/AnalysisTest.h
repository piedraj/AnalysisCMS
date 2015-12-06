#ifndef AnalysisTest_h
#define AnalysisTest_h

#include "AnalysisCMS.h"


class AnalysisTest : public AnalysisCMS
{
 public :

  AnalysisTest(TTree* tree = 0);

  void    Loop             (TString sample,
			    float   luminosity);


  // Data members
  //----------------------------------------------------------------------------
  //  float                  _m2l;


  // Common TH1 histograms
  //----------------------------------------------------------------------------
  //  TH1D*                  h_counterRaw[nchannel][ncut][njetbin+1];
};

#endif
