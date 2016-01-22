#ifndef AnalysisMonoH_h
#define AnalysisMonoH_h

#include "AnalysisCMS.h"
#include "CutsMonoH.h"

class AnalysisMonoH : public AnalysisCMS
{
 public :

  AnalysisMonoH(TTree* tree = 0);

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
  TH2D*                  h_metvar_m2l[nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet1[nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet2[nchannel][ncut][njetbin+1];
  TH1D*                  h_fullpmet[nchannel][ncut][njetbin+1];
  TH1D*                  h_trkpmet[nchannel][ncut][njetbin+1];
  TH1D*                  h_jetpt1[nchannel][ncut][njetbin+1];
  TH1D*                  h_metphi[nchannel][ncut][njetbin+1];
  TH1D*                  h_lepphi1[nchannel][ncut][njetbin+1];
  TH1D*                  h_lepphi2[nchannel][ncut][njetbin+1];
  TH1D*                  h_pt1_pdfUp[nchannel][ncut][njetbin+1];
  TH1D*                  h_pt1_pdfDown[nchannel][ncut][njetbin+1];
};

#endif
