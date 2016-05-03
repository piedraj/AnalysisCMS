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

  double ComputeMT2(TLorentzVector VisibleA,  TLorentzVector VisibleB, 
		    TLorentzVector Invisible, int MT2Type = 0, double MT2Precision = 0.);

  // Data members
  //----------------------------------------------------------------------------


  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_htjets[nchannel][ncut][njetbin+1];
  TH2F*                  h_2ht   [nchannel][ncut][njetbin+1];
  //TH1D*                  h_dphill[nchannel][ncut][njetbin+1];
  //TH1D*                  h_mll[nchannel][ncut][njetbin+1];
  TH1D*                  h_dyll[nchannel][ncut][njetbin+1];
  TH1D*                  h_mllbb[nchannel][ncut][njetbin+1];
  TH2F*                  h_dym[nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetjet[nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetlep1[nchannel][ncut][njetbin+1];
  //TH1D*                  h_dphilj[nchannel][ncut][njetbin+1];
  TH1D*                  h_meff[nchannel][ncut][njetbin+1];
  TH1D*                  h_ptbll[nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetptbll[nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2ll[nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2bb[nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2lblb[nchannel][ncut][njetbin+1];

  //double                 _dphill;
  double                 _dyll;        // Rapidity or pseudorapidity?
  double                 _mllbb;
  double                 _dphimetjet;
  double                 _dphimetlep1; // Maybe already defined
  //double                 _dphilj;    // Meaning to be verified
  double                 _meff;
  double                 _ptbll;
  double                 _dphimetptbll;
  double                 _mt2ll;
  double                 _mt2bb;
  double                 _mt2lblb;

};

#endif
