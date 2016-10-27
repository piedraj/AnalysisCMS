#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_NoCuts,
  Control_01_Has2Leptons,
  Control_02_Top,
  Control_03_Routin,
  Control_04_WW,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_NoCuts",
  "Control/01_Has2Leptons",
  "Control/02_Top",
  "Control/03_Routin",
  "Control/04_WW"
};

#endif
