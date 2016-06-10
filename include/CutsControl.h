#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_ZJets,
  Control_01_Routin,
  Control_02_RoutinBtag,
  Control_03_Top,
  Control_04_TopBtag,
  Control_05_WW,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_ZJets",
  "Control/01_Routin",
  "Control/02_RoutinBtag",
  "Control/03_Top",
  "Control/04_TopBtag",
  "Control/05_WW"
};

#endif
