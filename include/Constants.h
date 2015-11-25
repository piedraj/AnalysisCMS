#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1


#include "TString.h"


const float lumi50ns_fb = 0.040;
const float lumi25ns_fb = 1.269;


const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =  0.000511;  // [GeV]
const float MUON_MASS     =  0.106;     // [GeV]
const float TAU_MASS      =  1.777;     // [GeV]
const float Z_MASS        = 91.188;     // [GeV]


const int njetbin = 3;  // 0jet, 1jet, 2+jet, all

const UInt_t numberMetCuts = 5;
const Int_t MetCut[numberMetCuts] = {20, 25, 30, 45, 1000}; // [GeV]

const int nchannel = 9;

enum {
  ee,
  em,
  mm,
  ll,
  eee,
  eem,
  emm,
  mmm,
  lll
};

const TString schannel[nchannel] = {
  "ee",
  "em",
  "mm",
  "ll",
  "eee",
  "eem",
  "emm",
  "mmm",
  "lll"
};

const TString lchannel[nchannel] = {
  "ee",
  "e#mu",
  "#mu#mu",
  "ll",
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "lll"
};


const int ncut = 35;

enum {
  Top_00_Has2Leptons,
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  TTDM_00_Exactly2Leptons,
  TTDM_01_ZVeto,
  TTDM_02_Has2Jets,
  TTDM_03_LepPtSum,
  TTDM_04_JetPtSum,
  TTDM_05_LepDeltaPhi,
  TTDM_06_MET,
  WW_00_Has2Leptons,
  WW_01_Exactly2Leptons,
  WW_02_Mll,
  WW_03_PfMet,
  WW_04_ZVeto,
  WW_05_MpMet,
  WW_06_DPhiVeto,
  WW_07_Ptll,
  WW_08_BVeto,
  WW_09_SoftMu,
  WW_10_Ht,
  DY_00_20GeVLoose,
  DY_01_25GeVLoose,
  DY_02_30GeVLoose,
  DY_03_45GeVLoose,
  DY_04_1000GeVLoose,
  DY_05_20GeVTight,
  DY_06_25GeVTight,
  DY_07_30GeVTight,
  DY_08_45GeVTight,
  WZ_00_Exactly3Leptons,
  WZ_01_HasZ,
  WZ_02_HasWdr,
  WZ_02_HasWmll,
  WZ_03_BVeto
};

const TString scut[ncut] = {
  "Top/00_Has2Leptons",
  "Top/01_Has2Jets",
  "Top/02_Has1BJet",
  "TTDM/00_Exactly2Leptons",
  "TTDM/01_ZVeto",
  "TTDM/02_Has2Jets",
  "TTDM/03_LepPtSum",
  "TTDM/04_JetPtSum",
  "TTDM/05_LepDeltaPhi",
  "TTDM/06_MET",
  "WW/00_Has2Leptons",
  "WW/01_Exactly2Leptons",
  "WW/02_Mll",
  "WW/03_PfMet",
  "WW/04_ZVeto",
  "WW/05_MpMet",
  "WW/06_DPhiVeto",
  "WW/07_Ptll",
  "WW/08_BVeto",
  "WW/09_SoftMu",
  "WW/10_Ht",
  "DY/00_20GeVLoose",
  "DY/01_25GeVLoose",
  "DY/02_30GeVLoose",
  "DY/03_45GeVLoose",
  "DY/04_1000GeVLoose",
  "DY/00_20GeVTight",
  "DY/01_25GeVTight",
  "DY/02_30GeVTight",
  "DY/03_45GeVTight",
  "WZ/00_Exactly3Leptons",
  "WZ/01_HasZ",
  "WZ/02_HasWdr",
  "WZ/02_HasWmll",
  "WZ/03_BVeto"
};


// https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
const float csvv2ivf_looseWP  = 0.605;
const float csvv2ivf_mediumWP = 0.89;
const float csvv2ivf_tightWP  = 0.97;


#endif
