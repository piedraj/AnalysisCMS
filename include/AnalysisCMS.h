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
  float          csvv2ivf;
  TLorentzVector v;
};


class AnalysisCMS : public AnalysisBase
{
 public :

  AnalysisCMS(TTree* tree = 0);

  void    ApplyWeights     ();

  void    DefineHistograms (int      ichannel,
			    int      icut,
			    int      ijet,
			    TString  suffix);

  float   ElectronIsolation(int      k);

  void    EndJob           ();

  void    EventDump        ();

  void    EventSetup       ();

  void    FillHistograms   (int      ichannel,
			    int      icut,
			    int      ijet);

  void    GetDeltaPhiVeto  ();

  void    GetHt            ();

  void    GetJets          ();

  void    GetJetPtSum      ();

  void    GetLeptons       ();

  void    GetMc            ();

  void    GetMET           (float    module,
			    float    phi);

  void    GetMetVar        ();

  void    GetMpMet         ();

  void    GetMt            (Lepton   lep,
			    float&   transverse_mass);

  void    GetPtWW          ();

  void    GetSoftMuon      ();

  void    GetFakeWeights   ();

  bool    IsIsolatedLepton (int      k);

  float   MuonIsolation    (int      k);

  void    PrintProgress    (Long64_t counter,
			    Long64_t total);

  void    Setup            (TString  analysis,
			    TString  filename,
			    float    luminosity);

  void    Summary          (TString  analysis,
			    TString  precision,
			    TString  title);


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Jet>       AnalysisJets;
  std::vector<Lepton>    AnalysisLeptons;
  Lepton                 Lepton1;
  Lepton                 Lepton2;
  TLorentzVector         MET;

  bool                   _eventdump;
  bool                   _foundsoftmuon;
  bool                   _ismc;
  bool                   _passdphiveto;

  TString                _analysis;
  TString                _filename;
  TString                _sample;

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
  float                  _ht;
  float                  _htjets;
  float                  _luminosity;
  float                  _mc;
  float                  _metvar;
  float                  _mpmet;
  float                  _fullpmet;
  float                  _trkpmet;
  float                  _mtw;
  float                  _m2l;
  float                  _m3l;
  float                  _ptww;
  float                  _pt2l;
  float                  _sumjpt12;

  Long64_t               _nentries;

  int                    _channel;
  unsigned int           _jetbin;
  unsigned int           _nbjet15loose;
  unsigned int           _nbjet15tight;
  unsigned int           _nbjet30tight;
  unsigned int           _nelectron;
  unsigned int           _nlepton;
  
  ofstream               txt_eventdump;
  ofstream               txt_summary;
  TFile*                 root_output;


  // Common TH1 histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_counterRaw  [nchannel][ncut][njetbin+1];
  TH1D*                  h_counterLum  [nchannel][ncut][njetbin+1];
  TH1D*                  h_njet        [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15loose[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15tight[nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet30tight[nchannel][ncut][njetbin+1];
  TH1D*                  h_nvtx        [nchannel][ncut][njetbin+1];
  TH1D*                  h_drll        [nchannel][ncut][njetbin+1];
  TH1D*                  h_dphill      [nchannel][ncut][njetbin+1];
  TH1D*                  h_met         [nchannel][ncut][njetbin+1];
  TH1D*                  h_trkmet      [nchannel][ncut][njetbin+1];
  TH1D*                  h_mpmet       [nchannel][ncut][njetbin+1];
  TH1D*                  h_m2l         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw1        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mtw2        [nchannel][ncut][njetbin+1];
  TH1D*                  h_mth         [nchannel][ncut][njetbin+1];
  TH1D*                  h_mc          [nchannel][ncut][njetbin+1];
  TH1D*                  h_ht          [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt1         [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2         [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumpt12     [nchannel][ncut][njetbin+1];
  TH1D*                  h_sumjpt12    [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2l        [nchannel][ncut][njetbin+1];
  TH1D*                  h_ptww        [nchannel][ncut][njetbin+1];
  TH1D*                  h_fakes       [nchannel][ncut][njetbin+1];
};

#endif
