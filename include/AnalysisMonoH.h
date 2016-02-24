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

  // Puesto por Alberto Manjon
  TH1D*                  h_njet           [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet          [nchannel][ncut][njetbin+1];
  TH1D*                  h_pfType1Met     [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarl1met    [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarl2met    [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarllmet    [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltaphill     [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltaphil1met  [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltaphil2met  [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltaphillmet  [nchannel][ncut][njetbin+1];
  //TH1D*                h_deltaR_jet_met [nchannel][ncut][njetbin+1];
  TH2D*                  h_met_deltaphill[nchannel][ncut][njetbin+1];
  TH2D*                  h_met_m2l[nchannel][ncut][njetbin+1];
};

#endif
