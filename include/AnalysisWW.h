#ifndef AnalysisWW_h
#define AnalysisWW_h

#include "AnalysisCMS.h"
#include "CutsWW.h"

class AnalysisWW : public AnalysisCMS
{
 public :

  AnalysisWW(TTree* tree, TString systematic);

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
  TH1D*                  h_fullpmet  [nchannel][ncut][njetbin+1];
  TH1D*                  h_trkpmet   [nchannel][ncut][njetbin+1];
  TH1D*                  h_htjets    [nchannel][ncut][njetbin+1];
  TH1D*                  h_htnojets  [nchannel][ncut][njetbin+1];
  TH1D*                  h_mllstar   [nchannel][ncut][njetbin+1];
};

#endif
