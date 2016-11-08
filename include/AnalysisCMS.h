#ifndef AnalysisCMS_h
#define AnalysisCMS_h

#include "AnalysisBase.h"
#include "Constants.h"

#include <fstream>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>


enum {Loose, Tight};

struct Lepton
{
  int            index;
  int            type;
  int            flavour;
  float          idisoW;
  float          iso;
  TLorentzVector v;
};

struct Jet
{
  int            index;
  float          cmvav2;
  float          csvv2ivf;
  float          mass;
  TLorentzVector v;
};


class AnalysisCMS : public AnalysisBase
{
 public :

  AnalysisCMS(TTree* tree = 0, TString systematic = "nominal");

  void    ApplyWeights      ();

  void    DefineHistograms  (int      ichannel,
			     int      icut,
			     int      ijet,
			     TString  suffix);

  float   ElectronIsolation (int      k);

  void    EndJob            ();

  void    EventDump         ();

  void    EventSetup        (float    jet_eta_max = -1,
			     float    jet_pt_min  = 30);

  void    FillHistograms    (int      ichannel,
			     int      icut,
			     int      ijet);

  void    GetFakeWeights    ();

  void    GetGenPtllWeight  ();

  void    GetJets           (float    jet_eta_max,
			     float    jet_pt_min);

  void    GetLeptons        ();

  void    GetTops           (); 

  void    GetTopReco        ();

  TMatrixDSym GetMomentumTensor();

  TVectorD GetEigenvalues   (TMatrixDSym _smatrix);

  float GetSphericity       (TMatrixDSym _smatrix);

  float GetAlignment        (TMatrixDSym _smatrix);

  float GetPlanarity        (TMatrixDSym _smatrix);

  float GetCentrality       ();

  float GetST               ();

  void    GetMET            (float    module,
			     float    phi);

  void    GetTrkMET         (float    module,
			     float    phi);

  float   MuonIsolation     (int      k);

  void    OpenMinitree      ();

  bool    PassTrigger       ();

  void    PrintProgress     (Long64_t counter,
			     Long64_t total);

  void    SetSaveMinitree   (Bool_t   saveminitree) {_saveminitree = saveminitree;}

  void    Setup             (TString  analysis,
			     TString  filename,
			     float    luminosity);

  void    Summary           (TString  analysis,
			     TString  precision,
			     TString  title);


  // New analysis variables
  //----------------------------------------------------------------------------
  void    GetDeltaPhi       ();

  void    GetDeltaPhiVeto   ();

  void    GetDeltaR         ();

  void    GetHt             ();

  void    GetJetPtSum       ();

  void    GetMc             ();

  void    GetMetVar         ();

  void    GetMpMet          ();

  void    GetPtWW           ();

  void    GetSoftMuon       ();

  void    GetStarVar        ();

  void    GetZHCRVar        ();

  void    GetStopVar        ();

  void    GetMt             (Lepton         lep,
			     float&         transverse_mass);

  double  ComputeMT2        (TLorentzVector VisibleA,
			     TLorentzVector VisibleB, 
			     TLorentzVector Invisible,
			     int            MT2Type      = 0,
			     double         MT2Precision = 0.);


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Jet>       AnalysisJets;
  std::vector<Lepton>    AnalysisLeptons;
  Lepton                 Lepton1;
  Lepton                 Lepton2;
  TLorentzVector         MET;
  TLorentzVector         trkMET;

  std::vector<float>     _jet_pt; 
  std::vector<float>     _jet_eta;
  std::vector<float>     _jet_phi;

  std::vector<float>     _bjet30csvv2m_pt; 
  std::vector<float>     _bjet30csvv2m_eta;
  std::vector<float>     _bjet30csvv2m_phi;

  bool                   _saveminitree;
  bool                   _eventdump;
  bool                   _foundsoftmuon;
  bool                   _ismc;
  bool                   _passdphiveto;
  bool                   _systematic_btag_do;
  bool                   _systematic_btag_up;
  bool                   _systematic_idiso_do;
  bool                   _systematic_idiso_up;
  bool                   _systematic_trigger_do;
  bool                   _systematic_trigger_up;
  bool                   _systematic_reco_do;
  bool                   _systematic_reco_up;

  TString                _analysis;
  TString                _dataperiod;
  TString                _isdatadriven;
  TString                _filename;
  TString                _sample;
  TString                _systematic;

  float                  _channel;
  float                  _deltarjet1met;
  float                  _deltarjet2met;
  float                  _deltarjj;
  float                  _deltarjjmet;
  float                  _deltarlep1jet1;
  float                  _deltarlep1jet2;
  float                  _deltarlep2jet1;
  float                  _deltarlep2jet2;
  float                  _deltarllmet;
  float                  _deltarl1met;
  float                  _deltarl2met;
  float                  _dphijet1met;
  float                  _dphijet2met;
  float                  _dphijj;
  float                  _dphijjmet;
  float                  _dphilep1jet1;
  float                  _dphilep1jet2;
  float                  _dphilep2jet1;
  float                  _dphilep2jet2;
  float                  _dphillmet;
  float                  _dphillstar;
  float			 _dphitt_gen;
  float                  _detatt_gen; 
  float                  _event_weight;
  float                  _fake_weight;
  float                  _fake_weight_elUp;
  float                  _fake_weight_elDown;
  float                  _fake_weight_elStatUp;
  float                  _fake_weight_elStatDown;
  float                  _fake_weight_muUp;
  float                  _fake_weight_muDown;
  float                  _fake_weight_muStatUp;
  float                  _fake_weight_muStatDown;
  float                  _gen_ptll_weight;
  float                  _fullpmet;
  float                  _ht;
  float                  _htjets;
  float                  _htnojets;
  float 		 _detall; 	
  float                  _lep1eta;
  float                  _lep1phi;
  float                  _lep1pt;
  float                  _lep1mass;
  float                  _lep1eta_gen;
  float                  _lep1phi_gen;
  float                  _lep1pt_gen;
  float                  _lep2eta;
  float                  _lep2phi;
  float                  _lep2pt;
  float			 _lep2mass;
  float                  _lep2eta_gen;
  float                  _lep2phi_gen;
  float                  _lep2pt_gen;
  float                  _luminosity;
  float                  _mc;
  float                  _metvar;
  float                  _mllstar;
  float                  _mpmet;
  float                  _mtw;
  float                  _m2l;
  float                  _m3l;
  float			 _m2t_gen;
  float                  _leadingPtCSVv2L;
  float                  _leadingPtCSVv2M;
  float                  _leadingPtCSVv2T;
  float                  _trailingPtCSVv2L;
  float                  _trailingPtCSVv2M;
  float                  _trailingPtCSVv2T;
  float                  _nbjet30csvv2l;
  float                  _nbjet30csvv2m;
  float                  _nbjet30csvv2t;
  float                  _nbjet20cmvav2l;
  float                  _nbjet20cmvav2m;
  float                  _nbjet20cmvav2t;
  float                  _nbjet30cmvav2l;
  float                  _nbjet30cmvav2m;
  float                  _nbjet30cmvav2t;
  float                  _njet;
  float                  _nsol_1_1_10;
  float                  _nsol_10_10_10;
  float                  _ptww;
  float                  _pt2l;
  float                  _sumjpt12;
  float                  _trkpmet;
  float                  _top1eta_gen;
  float                  _top1phi_gen;
  float                  _top1pt_gen;
  float                  _top2eta_gen;
  float                  _top2phi_gen;
  float                  _top2pt_gen;
  float			 _topReco;
  float                  _sphericity;
  float                  _alignment;
  float                  _planarity;
  float                  _centrality;
  float                  _ST;

  float                  _mll13;
  float                  _mll23;
  float                  _mll14;
  float                  _mll24;
  float                  _mll34;
  float                  _dyll;
  float                  _ptbll;
  float                  _mt2ll;
  float                  _dphimetptbll;
  float                  _dphimetjet;
  float                  _dphimetbbll;
  float                  _mllbb;
  float                  _meff;
  float                  _mt2bb;
  float                  _mt2lblb;
  float                  _mlb1;
  float                  _mlb2;
  float                  _mt2lblbcomb;
  float                  _mt2bbtrue;
  float                  _mt2lblbtrue;
  float                  _mt2lblbmatch;
  float                  _mlb1comb;
  float                  _mlb2comb;
  float                  _mlb1true;
  float                  _mlb2true;
  float                  _bjet1pt;
  float                  _bjet1eta;
  float                  _bjet1phi;
  float                  _bjet1mass;
  float                  _bjet1csvv2ivf;
  float                  _bjet2pt;
  float                  _bjet2eta;
  float                  _bjet2phi;
  float                  _bjet2mass;
  float                  _bjet2csvv2ivf;
  float                  _tjet1pt;
  float                  _tjet1eta;
  float                  _tjet1phi;
  float                  _tjet1mass;
  float                  _tjet1csvv2ivf;
  float                  _tjet1assignment;
  float                  _tjet2pt;
  float                  _tjet2eta;
  float                  _tjet2phi;
  float                  _tjet2mass;
  float                  _tjet2csvv2ivf;
  float                  _tjet2assignment;


  Long64_t               _nentries;

  unsigned int           _jetbin;
  unsigned int           _nelectron;
  unsigned int           _nlepton;
  
  ofstream               txt_eventdump;
  ofstream               txt_summary;
  TFile*                 root_output;
  TFile*                 root_minitree;
  TTree*                 minitree;


  // TH1 histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_counterRaw   [nchannel][ncut][njetbin+1];
  TH1D*                  h_counterLum   [nchannel][ncut][njetbin+1];
  TH1D*                  h_fakes        [nchannel][ncut][njetbin+1];
  TH1D*                  h_nvtx         [nchannel][ncut][njetbin+1];
  TH1D*                  h_ptww         [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2l         [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumjpt12     [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumpt12      [nchannel][ncut][njetbin+1];
  TH1D*                  h_dyll         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mllbb        [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetjet   [nchannel][ncut][njetbin+1];
  TH1D*                  h_meff         [nchannel][ncut][njetbin+1];
  TH1D*                  h_ptbll        [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetptbll [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphimetbbll  [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2ll        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2bb        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2lblb      [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb1         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mlb2         [nchannel][ncut][njetbin+1];

  // TH1 histograms with minitree variables
  //----------------------------------------------------------------------------
  TH1D*                  h_channel       [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijet1met   [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijet2met   [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijj        [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijjmet     [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep1jet1  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep1jet2  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep2jet1  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep2jet2  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphill        [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphillmet     [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphillstar    [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet1     [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet2     [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphitt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_detatt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_drll          [nchannel][ncut][njetbin+1];
  TH1D*                  h_ht            [nchannel][ncut][njetbin+1];
  TH1D*                  h_htjets        [nchannel][ncut][njetbin+1];
  TH1D*                  h_htnojets      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1eta       [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1mass      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1phi       [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1pt        [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2eta       [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2mass      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2phi       [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2pt        [nchannel][ncut][njetbin+1];
  TH1D*                  h_detall        [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1eta       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1phi       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1pt        [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1eta_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1phi_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1pt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2eta       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2phi       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2pt        [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2eta_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2phi_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2pt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_mc            [nchannel][ncut][njetbin+1];
  TH1D*                  h_metPfType1    [nchannel][ncut][njetbin+1];
  TH1D*                  h_metPfType1Phi [nchannel][ncut][njetbin+1];
  TH1D*                  h_metTtrk       [nchannel][ncut][njetbin+1];
  TH1D*                  h_metTtrkPhi    [nchannel][ncut][njetbin+1];
  TH1D*                  h_mpmet         [nchannel][ncut][njetbin+1];
  TH1D*                  h_metPuppi      [nchannel][ncut][njetbin+1];
  TH1D*                  h_mth           [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw1          [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw2          [nchannel][ncut][njetbin+1];
  TH1D*                  h_m2l           [nchannel][ncut][njetbin+1];
  TH1D*                  h_m2t_gen       [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15csvv2l [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15csvv2m [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15csvv2t [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30csvv2l [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30csvv2m [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30csvv2t [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20cmvav2l[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20cmvav2m[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20cmvav2t[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30cmvav2l[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30cmvav2m[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30cmvav2t[nchannel][ncut][njetbin+1];
  TH1D*                  h_njet          [nchannel][ncut][njetbin+1];
  TH1D*                  h_top1eta_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_top1phi_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_top1pt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_top2eta_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_top2phi_gen   [nchannel][ncut][njetbin+1];
  TH1D*                  h_top2pt_gen    [nchannel][ncut][njetbin+1];
  TH1D*                  h_topReco       [nchannel][ncut][njetbin+1];
  TH1D*                  h_sphericity    [nchannel][ncut][njetbin+1];
  TH1D*                  h_alignment     [nchannel][ncut][njetbin+1];
  TH1D*                  h_planarity     [nchannel][ncut][njetbin+1];
  TH1D*                  h_centrality    [nchannel][ncut][njetbin+1];
  TH1D*                  h_ST            [nchannel][ncut][njetbin+1];

  // TH2 histograms
  //----------------------------------------------------------------------------
  TH2D*                  h_metPfType1_m2l[nchannel][ncut][njetbin+1];
  TH2D*                  h_mpmet_m2l     [nchannel][ncut][njetbin+1];
  TH2D*                  h_2ht           [nchannel][ncut][njetbin+1];
  TH2D*                  h_dym           [nchannel][ncut][njetbin+1];

};

#endif
