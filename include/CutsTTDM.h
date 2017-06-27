#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1

#include "TString.h"


enum {
  //  TTDM_00_Has2Leptons,
  TTDM_01_NewPresel,
  TTDM_Control_ttV,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  //  "TTDM/00_Has2Leptons",
  "TTDM/01_NewPresel",
  "TTDM/Control_ttV",
};

#endif
