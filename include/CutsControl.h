#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_WW0j,
  Control_01_WW1j,
  Control_02_Routin,
  Control_03_Top,
  Control_10_WW0jSS,
  Control_11_WW1jSS,
  Control_12_RoutinSS,
  Control_13_TopSS,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_WW0j",
  "Control/01_WW1j",
  "Control/02_Routin",
  "Control/03_Top",
  "Control/10_WW0jSS",
  "Control/11_WW1jSS",
  "Control/12_RoutinSS",
  "Control/13_TopSS"
};

#endif
