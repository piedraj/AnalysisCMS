#if !defined(MYLIB_CUTSMONOH_H)
#define MYLIB_CUTSMONOH_H 1

#include "TString.h"


enum {
  MonoH_00_Has2Leptons,
  MonoH_01_Mll,
  MonoH_02_PfMet,
  MonoH_03_ZVeto,
  MonoH_04_MpMet,
  MonoH_05_DPhiVeto,
  MonoH_06_Ptll,
  MonoH_07_BVeto,
  MonoH_08_ZHCR,
  // MonoH_08_SoftMu,
  // MonoH_100_Mc,
  // MonoH_101_DRll,
  // MonoH_102_MpMet,
  // MonoH_103_CR,
  /* MonoH_09_mpmet100, */
  /* MonoH_10_mth200, */
  /* MonoH_11_dphil1met, */
  /* MonoH_12_dphil2met, */
  /* MonoH_13_deltarll, */
  /* MonoH_14_mtw1, */
  /* MonoH_15_mtw2, */
  /* MonoH_16_trkmet, */
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "MonoH/00_Has2Leptons",
  "MonoH/01_Mll",
  "MonoH/02_PfMet",
  "MonoH/03_ZVeto",
  "MonoH/04_MpMet",
  "MonoH/05_DPhiVeto",
  "MonoH/06_Ptll",
  "MonoH/07_BVeto",
  "MonoH/08_ZHCR",
  // "MonoH/08_SoftMu",
  // "MonoH/100_Mc",
  // "MonoH/101_DRll",
  // "MonoH/102_MpMet",
  // "MonoH/103_CR",
  /* "MonoH/09_mpmet100", */
  /* "MonoH/10_mth200", */
  /* "MonoH/11_dphil1met", */
  /* "MonoH/12_dphil2met",  */
  /* "MonoH/13_deltarll",  */
  /* "MonoH/14_mtw1",  */
  /* "MonoH/15_mtw2",  */
  /* "MonoH/16_trkmet"  */
};

#endif
