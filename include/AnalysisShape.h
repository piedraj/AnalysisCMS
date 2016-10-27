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
  
  TMatrixDSym GetMomentumTensor();

  TVectorD GetEigenvalues(TMatrixDSym _smatrix);

  float GetSphericity(TMatrixDSym _smatrix);

};

#endif
