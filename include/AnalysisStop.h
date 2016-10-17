#ifndef AnalysisStop_h
#define AnalysisStop_h
 
#include "AnalysisCMS.h"
#include <map>


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

  typedef pair<int, int> MassPoint;
  typedef pair<float, float> StopCrossSection;
  typedef pair<StopCrossSection, int> MassPointParameters;

  typedef map<MassPoint, MassPointParameters> MassPointMap;
  MassPointMap StopNeutralinoMap;

  void SetStopNeutralinoMap();

  // Analysis histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_mt2lblbcomb      [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2bbtrue        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2lblbtrue      [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2lblbmatch     [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb1comb         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb2comb         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb1true         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb2true         [nchannel][ncut][njetbin+1];
  TH2D*                  h_mt2lblbvsmlbtrue [nchannel][ncut][njetbin+1];
  
};

#endif
