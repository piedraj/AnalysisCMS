#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1

#include "TString.h"


enum {
  TTDM_00_Has2Leptons,
  TTDM_01_ZVeto,
  TTDM_02_MET50,
  TTDM_03_Preselection,
  TTDM_10_Routin,
  TTDM_11_AN15305,
  TTDM_20_AN16105,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "TTDM/00_Has2Leptons",
  "TTDM/01_ZVeto",
  "TTDM/02_MET50",
  "TTDM/03_Preselection",
  "TTDM/10_Routin",
  "TTDM/11_AN15305",
  "TTDM/20_AN16105"
};

#endif
