#if !defined(MYLIB_CUTSWW_H)
#define MYLIB_CUTSWW_H 1

#include "TString.h"


enum {
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
  WW_29_ZWindow36,
  WW_30_ZWindow37,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
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
  "WW/29_ZWindow36",  
  "WW/30_ZWindow37"
};

#endif
