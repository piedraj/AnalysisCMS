#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1

#include "TString.h"


enum {
  TTDM_00_Has2Leptons,
  TTDM_01_NewPresel,
  //TTDM_03_AN16105,
  //TTDM_04_tt,
  //TTDM_05_tt,
  //TTDM_06_tt,
  //TTDM_Zjets,
  //TTDM_WW0jet, 
  //TTDM_WW1jet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "TTDM/00_Has2Leptons",
  "TTDM/01_NewPresel",
};

#endif
