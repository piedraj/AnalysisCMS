#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_ZJets,
  Control_01_Top,
  Control_02_WW0j,
  Control_03_WW1j,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_ZJets",
  "Control/01_Top",
  "Control/02_WW0j",
  "Control/03_WW1j"
};

#endif
