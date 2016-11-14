#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_NoCuts,
  Control_01_TwoMediumLeptons,
  Control_02_TwoTightLeptons,
  Control_03_Routin,
  Control_04_WW,
  Control_05_Top,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_NoCuts",
  "Control/01_TwoMediumLeptons",
  "Control/02_TwoTightLeptons",
  "Control/03_Routin",
  "Control/04_WW",
  "Control/05_Top"
};

#endif
