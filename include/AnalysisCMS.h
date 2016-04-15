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


enum {Loose, Tight};

struct Lepton
{
  int            index;
  int            type;
  int            flavour;
  float          iso;
  TLorentzVector v;
};

struct Jet
{
  int            index;
  float          cmvav2;
  float          csvv2ivf;
  TLorentzVector v;
};


class AnalysisCMS : public AnalysisBase
{
 public :

  AnalysisCMS(TTree* tree = 0);

  

  void    ApplyWeights      ();

  void    DefineHistograms  (int      ichannel,
			     int      icut,
			     int      ijet,
			     TString  suffix);

  float   ElectronIsolation (int      k);

  void    EndJob            ();

  void    EventDump         ();

  void    EventSetup        ();

  void    FillHistograms    (int      ichannel,
			     int      icut,
			     int      ijet);

  void    GetFakeWeights    ();

  void    GetGenPtllWeight  ();

  void    GetJets           ();

  void    GetLeptons        ();

  void    GetMET            (float    module,
			     float    phi);

  void    GetTrkMET         (float    module,
			     float    phi);

  bool    IsIsolatedLepton  (int      k);

  float   MuonIsolation     (int      k);

  void    OpenMinitree      ();

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

  void    GetMt             (Lepton   lep,
			     float&   transverse_mass);

  void    GetPtWW           ();

  void    GetSoftMuon       ();

  void    GetStarVar        ();

  void    GetSumOfWeightsLHE();


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Jet>       AnalysisJets;
  std::vector<Lepton>    AnalysisLeptons;
  Lepton                 Lepton1;
  Lepton                 Lepton2;
  TLorentzVector         MET;
  TLorentzVector         trkMET;

  bool                   _saveminitree;
  bool                   _eventdump;
  bool                   _foundsoftmuon;
  bool                   _ismc;
  bool                   _passdphiveto;

  TString                _analysis;
  TString                _filename;
  TString                _sample;

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
  float                  _lep1eta;
  float                  _lep1phi;
  float                  _lep1pt;
  float                  _lep2eta;
  float                  _lep2phi;
  float                  _lep2pt;
  float                  _luminosity;
  float                  _mc;
  float                  _metvar;
  float                  _mllstar;
  float                  _mpmet;
  float                  _mtw;
  float                  _m2l;
  float                  _m3l;
  float                  _nbjet15loose;
  float                  _nbjet15medium;
  float                  _nbjet15tight;
  float                  _nbjet20loose;
  float                  _nbjet20medium;
  float                  _nbjet20tight;
  float                  _nbjet30loose;
  float                  _nbjet30medium;
  float                  _nbjet30tight;
  float                  _ptww;
  float                  _pt2l;
  float                  _sumjpt12;
  float                  _trkpmet;

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
  TH1D*                  h_pdfsum;
  TH1D*                  h_qcdsum;

  TH1D*                  h_counterRaw   [nchannel][ncut][njetbin+1];
  TH1D*                  h_counterLum   [nchannel][ncut][njetbin+1];
  TH1D*                  h_fakes        [nchannel][ncut][njetbin+1];
  TH1D*                  h_nvtx         [nchannel][ncut][njetbin+1];
  TH1D*                  h_ptww         [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2l         [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumjpt12     [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumpt12      [nchannel][ncut][njetbin+1];


  // TH1 histograms with minitree variables
  //----------------------------------------------------------------------------
  TH1D*                  h_channel      [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijet1met  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijet2met  [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijj       [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphijjmet    [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep1jet1 [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep1jet2 [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep2jet1 [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilep2jet2 [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphill       [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphillmet    [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphillstar   [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet1    [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphilmet2    [nchannel][ncut][njetbin+1];
  TH1D*                  h_drll         [nchannel][ncut][njetbin+1];
  TH1D*                  h_ht           [nchannel][ncut][njetbin+1];
  TH1D*                  h_htjets       [nchannel][ncut][njetbin+1];
  TH1D*                  h_htnojets     [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1eta      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1mass     [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1phi      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet1pt       [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2eta      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2mass     [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2phi      [nchannel][ncut][njetbin+1];
  TH1D*                  h_jet2pt       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1eta      [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1phi      [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep1pt       [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2eta      [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2phi      [nchannel][ncut][njetbin+1];
  TH1D*                  h_lep2pt       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mc           [nchannel][ncut][njetbin+1];
  TH1D*                  h_metPfType1   [nchannel][ncut][njetbin+1];
  TH1D*                  h_metPfType1Phi[nchannel][ncut][njetbin+1];
  TH1D*                  h_metTtrk      [nchannel][ncut][njetbin+1];
  TH1D*                  h_metTtrkPhi   [nchannel][ncut][njetbin+1];
  TH1D*                  h_mpmet        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mth          [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw1         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw2         [nchannel][ncut][njetbin+1];
  TH1D*                  h_m2l          [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15loose [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15medium[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15tight [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20loose [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20medium[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet20tight [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30loose [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30medium[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30tight [nchannel][ncut][njetbin+1];
  TH1D*                  h_njet         [nchannel][ncut][njetbin+1];
};

#endif
