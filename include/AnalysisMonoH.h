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

  // Data members
  //----------------------------------------------------------------------------

  
  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_fullpmet   [nchannel][ncut][njetbin+1];
  TH1D*                  h_trkpmet    [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarl1met[nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarl2met[nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarllmet[nchannel][ncut][njetbin+1];
  TH1D*                  h_mllstar    [nchannel][ncut][njetbin+1];
  TH1D*                  h_htjets     [nchannel][ncut][njetbin+1];
  TH1D*                  h_htnojets   [nchannel][ncut][njetbin+1];
  TH1D*                  h_mr         [nchannel][ncut][njetbin+1];
  TH2D*                  h_metvar_m2l [nchannel][ncut][njetbin+1];
};

#endif
