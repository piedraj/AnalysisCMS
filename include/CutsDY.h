#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  DY_00_noCuts,
  DY_01_Has2Leptons,
  DY_02_Zwindow,
  DY_03_phiMETcorrected,
 
  /* DY_08_BVetoMedium, */
  /* DY_09_BVetoTight, */
  /* DY_10_BVetoLooseOld, */
  /* DY_11_BVetoMediumOld, */
  /* DY_1_BVetoTightOld, */
  /* DY_50_TopCR, */
  /* DY_60_DYttCR, */
  /* DY_70_DYCR, */
  /* DY_71_DYCR_NoMpMet, */
  /* DY_72_DYCR_JustPtll, */
  //  DY_10_DY,
  //  DY_11_ZWindow,
  //  DY_12_ZWindowPtll,
  //  DY_13_ZWindowMpMet,
  //  DY_14_ZWindowPfMet,
  //  DY_15_ZWindowBVeto,
  //  DY_16_ZWindowSoftMu,
  //  DY_17_ZCR,
  //  DY_18_ZWindow25,
  //  DY_19_ZWindow26,
  //  DY_20_ZWindow27,
  //  DY_21_ZWindow28,
  //  DY_22_ZWindow29,
  //  DY_23_ZWindow30,
  //  DY_24_ZWindow31,
  //  DY_25_ZWindow32,
  //  DY_26_ZWindow33,
  //  DY_27_ZWindow34,
  //  DY_28_ZWindow35,
  //  DY_29_ZWindow36,
  //  DY_100_MonoH_Mc,
  //  DY_101_MonoH_DRll,
  //  DY_102_MonoH_MpMet,
  //  DY_103_MonoH_CR,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "DY/00_noCuts",
  "DY/01_Has2Leptons",
  "DY/02_Zwindow",
  "DY/03_METcorrected" ,

    /* "DY/08_BVetoMedium", */
  /* "DY/09_BVetoTight", */
  /* "DY/10_BVetoLooseOld", */
  /* "DY/11_BVetoMediumOld", */
  /* "DY/12_BVetoTightOld", */
  /* "DY/50_TopCR", */
  /* "DY/60_DYttCR", */
  /* "DY/70_DYCR", */
  /* "DY/71_DYCR_NoMpMet", */
  /* "DY/72_DYCR_JustPtll", */
  //  DY/09_Ht",
  //  DY/10_DY",
  //  "DY/11_ZWindow",
  //  "DY/12_ZWindowPtll",
  //  "DY/13_ZWindowMpMet",
  //  "DY/14_ZWindowPfMet",
  //  "DY/15_ZWindowBVeto",
  //  "DY/16_ZWindowSoftMu",
  //  "DY/17_ZCR",
  //  "DY/18_ZWindow25",
  //  "DY/19_ZWindow26",
  //  "DY/20_ZWindow27",
  //  "DY/21_ZWindow28",
  //  "DY/22_ZWindow29",
  //  "DY/23_ZWindow30",
  //  "DY/24_ZWindow31",
  //  "DY/25_ZWindow32",
  //  "DY/26_ZWindow33",
  //  "DY/27_ZWindow34",
  //  "DY/28_ZWindow35",
  //  "DY/29_ZWindow36",
  //  "DY/100_MonoH_Mc",
  //  "DY/101_MonoH_DRll",
  //  "DY/102_MonoH_MpMet",
  //  "DY/103_MonoH_CR"
};

#endif
