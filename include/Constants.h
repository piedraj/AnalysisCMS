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


const double metvar_bins[] = {20, 25, 30, 45, 1000};  // [GeV]


const int njetbin = 3;  // 0jet, 1jet, 2+jet, all


const int nchannel = 9;

enum {
  ee,
  mm,
  em,
  ll,
  eee,
  eem,
  emm,
  mmm,
  lll
};

const TString schannel[nchannel] = {
  "ee",
  "mm",
  "em",
  "ll",
  "eee",
  "eem",
  "emm",
  "mmm",
  "lll"
};

const TString lchannel[nchannel] = {
  "ee",
  "#mu#mu",
  "e#mu",
  "all",
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "all"
};


const int ncut = 30;

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
  WW_11_DY,
  monoH_00_mc,
  monoH_01_drll,
  monoH_02_mpmet,
  monoH_80_CR,
  WZ_00_Exactly3Leptons,
  WZ_01_HasZ,
  WZ_02_HasW,
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
  "WW/11_DY",
  "monoH/00_mc",
  "monoH/01_drll",
  "monoH/02_mpmet",
  "monoH/80_CR",
  "WZ/00_Exactly3Leptons",
  "WZ/01_HasZ",
  "WZ/02_HasW",
  "WZ/03_BVeto"
};


// https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
const float csvv2ivf_looseWP  = 0.605;
const float csvv2ivf_mediumWP = 0.89;
const float csvv2ivf_tightWP  = 0.97;


#endif
