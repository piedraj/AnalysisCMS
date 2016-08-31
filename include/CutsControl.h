#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_NoCuts,
  Control_01_Has2Leptons,
  Control_02_ZJets,
  Control_03_Routin,
  Control_04_RoutinBtag,
  Control_05_Top,
  Control_06_TopBtag,
  Control_07_WW,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_NoCuts",
  "Control/01_Has2Leptons",
  "Control/02_ZJets",
  "Control/03_Routin",
  "Control/04_RoutinBtag",
  "Control/05_Top",
  "Control/06_TopBtag",
  "Control/07_WW"
};

#endif
