#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1


#include "TString.h"


enum {
  TTDM_00_Has2Leptons,
  TTDM_01_ZVeto,
  TTDM_02_Has2Jets,
  TTDM_03_LepPtSum,
  TTDM_04_JetPtSum,
  TTDM_05_LepDeltaPhi,
  TTDM_06_MET,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "TTDM/00_Has2Leptons",
  "TTDM/01_ZVeto",
  "TTDM/02_Has2Jets",
  "TTDM/03_LepPtSum",
  "TTDM/04_JetPtSum",
  "TTDM/05_LepDeltaPhi",
  "TTDM/06_MET"
};


#endif
