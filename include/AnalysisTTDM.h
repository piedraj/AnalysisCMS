#ifndef AnalysisTTDM_h
#define AnalysisTTDM_h

#include "AnalysisCMS.h"


class AnalysisTTDM : public AnalysisCMS
{
 public :

  AnalysisTTDM(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);


  // MET filters histograms
  //----------------------------------------------------------------------------
  TH1D*                  met_Flag_none;
  TH1D*                  met_Flag_all;
  TH1D*                  met_Flag_trackingFailureFilter;
  TH1D*                  met_Flag_goodVertices;
  TH1D*                  met_Flag_CSCTightHaloFilter;
  TH1D*                  met_Flag_trkPOGFilters;
  TH1D*                  met_Flag_trkPOG_logErrorTooManyClusters;
  TH1D*                  met_Flag_EcalDeadCellTriggerPrimitiveFilter;
  TH1D*                  met_Flag_ecalLaserCorrFilter;
  TH1D*                  met_Flag_trkPOG_manystripclus53X;
  TH1D*                  met_Flag_eeBadScFilter;
  TH1D*                  met_Flag_METFilters;
  TH1D*                  met_Flag_HBHENoiseFilter;
  TH1D*                  met_Flag_trkPOG_toomanystripclus53X;
  TH1D*                  met_Flag_hcalLaserEventFilter;
};

#endif
