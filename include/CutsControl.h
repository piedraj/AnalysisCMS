1;95;0c#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_Has2Leptons,
  Control_01_ZJets,
  Control_02_Routin,
  Control_03_RoutinBtag,
  Control_04_Top,
  Control_05_TopBtag,
  Control_06_WW,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_Has2Leptons",
  "Control/01_ZJets",
  "Control/02_Routin",
  "Control/03_RoutinBtag",
  "Control/04_Top",
  "Control/05_TopBtag",
  "Control/06_WW"
};

#endif
