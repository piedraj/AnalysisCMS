#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1

#include "TString.h"


enum {
  TTDM_00_Has2Leptons,
  TTDM_01_ZVeto,
  TTDM_02_Has2Jets,
  TTDM_03_Has1BJet,
  TTDM_04_DeltaPhi,
  TTDM_05_MET,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "TTDM/00_Has2Leptons",
  "TTDM/01_ZVeto",
  "TTDM/02_Has2Jets",
  "TTDM/03_Has1BJet",
  "TTDM/04_DeltaPhi",
  "TTDM/05_MET"
};

#endif
