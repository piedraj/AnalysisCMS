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


const int ncut = 47;

enum {
  Top_00_Has2Leptons,
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  TTDM_00_Has2Leptons,
  TTDM_01_ZVeto,
  TTDM_02_Has2Jets,
  TTDM_03_LepPtSum,
  TTDM_04_JetPtSum,
  TTDM_05_LepDeltaPhi,
  TTDM_06_MET,
  WW_00_Has2Leptons,
  WW_01_Mll,
  WW_02_PfMet,
  WW_03_ZVeto,
  WW_04_MpMet,
  WW_05_DPhiVeto,
  WW_06_Ptll,
  WW_07_BVeto,
  WW_08_SoftMu,
  WW_09_Ht,
  WW_10_DY,
  WW_11_ZWindow,
  WW_12_ZWindowPfMet,
  WW_13_ZWindowMpMet,
  WW_14_ZWindowPtll,
  WW_15_ZWindowBVeto,
  WW_16_ZWindowSoftMu,
  WW_17_ZCR,
  WW_18_ZWindow25,
  WW_19_ZWindow26,
  WW_20_ZWindow27,
  WW_21_ZWindow28,
  WW_22_ZWindow29,
  WW_23_ZWindow30,
  WW_24_ZWindow31,
  WW_25_ZWindow32,
  WW_26_ZWindow33,
  WW_27_ZWindow34,
  WW_28_ZWindow35,
  monoH_00_Mc,
  monoH_01_DRll,
  monoH_02_MpMet,
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
  "TTDM/00_Has2Leptons",
  "TTDM/01_ZVeto",
  "TTDM/02_Has2Jets",
  "TTDM/03_LepPtSum",
  "TTDM/04_JetPtSum",
  "TTDM/05_LepDeltaPhi",
  "TTDM/06_MET",
  "WW/00_Has2Leptons",
  "WW/01_Mll",
  "WW/02_PfMet",
  "WW/03_ZVeto",
  "WW/04_MpMet",
  "WW/05_DPhiVeto",
  "WW/06_Ptll",
  "WW/07_BVeto",
  "WW/08_SoftMu",
  "WW/09_Ht",
  "WW/10_DY",
  "WW/11_ZWindow",
  "WW/12_ZWindowPfMet",
  "WW/13_ZWindowMpMet",
  "WW/14_ZWindowPtll",
  "WW/15_ZWindowBVeto",
  "WW/16_ZWindowSoftMu",
  "WW/17_ZCR",
  "WW/18_ZWindow25",
  "WW/19_ZWindow26",
  "WW/20_ZWindow27",
  "WW/21_ZWindow28",
  "WW/22_ZWindow29",
  "WW/23_ZWindow30",
  "WW/24_ZWindow31",
  "WW/25_ZWindow32",
  "WW/26_ZWindow33",
  "WW/27_ZWindow34",
  "WW/28_ZWindow35",  
  "monoH/00_Mc",
  "monoH/01_DRll",
  "monoH/02_MpMet",
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
