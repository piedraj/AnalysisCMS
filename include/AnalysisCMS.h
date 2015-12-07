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

  void    EndJob           (TString  analysis);

  void    EventDump        ();

  void    EventSetup       ();

  void    FillHistograms   (int      ichannel,
			    int      icut,
			    int      ijet);

  void    GetDPhiVeto      ();

  void    GetHt            ();

  void    GetJets          ();

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

  bool    IsFiducialLepton (int      k);

  bool    IsIsolatedLepton (int      k);

  bool    IsTightLepton    (int      k);
  
  float   MuonIsolation    (int      k);

  void    PrintProgress    (Long64_t counter,
			    Long64_t total);

  void    Setup            (TString  filename,
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
  bool                   _ismc;
  bool                   _foundsoftmuon;
  bool                   _passdphiveto;

  TString                _filename;
  TString                _sample;
  float                  _luminosity;
  float                  _event_weight;
  float                  _ht;
  float                  _pt2l;
  float                  _ptww;
  float                  _m2l;
  float                  _m3l;
  float                  _mt1;
  float                  _mt2;
  float                  _mtw;
  float                  _mc;
  float                  _mpmet;
  float                  _metvar;

  Long64_t               _nentries;

  int                    _channel;
  unsigned int           _nelectron;
  unsigned int           _nlepton;
  unsigned int           _ntight;
  unsigned int           _njet30;
  unsigned int           _nbjet15;
  unsigned int           _jetbin;
  
  ofstream               txt_summary;
  ofstream               txt_eventdump;
  TFile*                 root_output;


  // Common TH1 histograms
  //----------------------------------------------------------------------------
  TH1D*                  h_counterRaw[nchannel][ncut][njetbin+1];
  TH1D*                  h_counterLum[nchannel][ncut][njetbin+1];
  TH1D*                  h_njet30    [nchannel][ncut][njetbin+1];
  TH1D*                  h_nbjet15   [nchannel][ncut][njetbin+1];
  TH1D*                  h_nvtx      [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltarll  [nchannel][ncut][njetbin+1];
  TH1D*                  h_deltaphill[nchannel][ncut][njetbin+1];
  TH1D*                  h_met       [nchannel][ncut][njetbin+1];
  TH1D*                  h_trkmet    [nchannel][ncut][njetbin+1];
  TH1D*                  h_mpmet     [nchannel][ncut][njetbin+1];
  TH1D*                  h_m2l       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt1       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mt2       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mth       [nchannel][ncut][njetbin+1];
  TH1D*                  h_mc        [nchannel][ncut][njetbin+1];
  TH1D*                  h_ht        [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt1       [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2       [nchannel][ncut][njetbin+1];
  TH1D*                  h_pt2l      [nchannel][ncut][njetbin+1];
  TH1D*                  h_ptww      [nchannel][ncut][njetbin+1];
};

#endif
