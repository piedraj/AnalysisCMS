#ifndef AnalysisStop_h
#define AnalysisStop_h
 
#include "AnalysisCMS.h"
#include "../../BTagSFUtil/BTagSFUtil.C"
#include <map>


class AnalysisStop : public AnalysisCMS
{
 public :

  AnalysisStop(TTree* tree, TString systematic);
  AnalysisStop(TFile* file, TString systematic,	int FillAllHistograms = 1);

  void BookAnalysisHistograms(); 

  void BookSystematicHistograms(); 

  void GetAnalysisVariables(); 

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet); 

  void FillSystematicHistograms(int     ichannel,
				int     icut);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void SaveSystematicHistograms();

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity,
			      float   StopRefMass = -1.,
			      float   NeutralinoRefMass = -1.);

  void CorrectEventWeight    ();

  void SetStopNeutralinoMap  ();

  void GetMiniTree           (TFile *MiniTreeFile, TString systematic);

  TString FastSimDataset;
  BTagSFUtil *BTagSF, *BTagSF_Upb, *BTagSF_Dob, *BTagSF_UpFSb, *BTagSF_DoFSb;

  typedef pair<int, int> MassPoint;
  typedef pair<float, float> StopCrossSection;
  typedef pair<StopCrossSection, int> MassPointParameters;

  typedef map<MassPoint, MassPointParameters> MassPointMap;
  MassPointMap StopNeutralinoMap;

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

  int _SaveHistograms;

  float _metmeff, _MT2ll, _MT2llgen;
  TH1D*                  h_metmeff          [nchannel][ncut][njetbin+1];
  TH1F*                  h_MT2ll            [nchannel][ncut][njetbin+1];
  TH1F*                  h_MT2llgen         [nchannel][ncut][njetbin+1];
  TH1F*                  h_MT2ll_fake       [nchannel][ncut][njetbin+1];
  TH1F*                  h_MT2ll_truth      [nchannel][ncut][njetbin+1];
  TH1F*                  h_MET_fake         [nchannel][ncut][njetbin+1];
  TH1F*                  h_MET_truth        [nchannel][ncut][njetbin+1];

  int _nLeptonsMatched;

  float _MT2_Met; int NbinsMT2 = 7; int NbinsMet = 5;
  float vMinMT2 = 0., vMinMet = 0., vMaxMT2 = 140., vMaxMet = 500.;
  TH1D*                  h_MT2_Met          [nchannel][ncut][njetbin+1];

  float _HTvisible_Met; int NbinsHTvisible = 5;
  float vMinHTvisible = 0., vMaxHTvisible = 400.;
  TH1D*                  h_HTvisible_Met    [nchannel][ncut][njetbin+1];

  float _MetMeff_Met; int NbinsMetMeff = 8;
  float vMinMetMeff = 0., vMaxMetMeff = 0.8;
  TH1D*                  h_MetMeff_Met      [nchannel][ncut][njetbin+1];

  float _R2_Met; int NbinsR2 = 7;
  float vMinR2 = 0., vMaxR2 = 1.4;
  TH1D*                  h_R2_Met           [nchannel][ncut][njetbin+1];

  // Systematic output
  TFile*                 root_output_systematic[nsystematic];
  TH1F*                  h_MT2ll_systematic [nchannel][ncut][nsystematic];
  TH1F*                  h_MT2llgen_systematic [nchannel][ncut][nsystematic];
  
};

#endif
