#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_000_WW0j,
  Control_001_WW1j,
  Control_010_Routin,
  Control_011_Top,
  Control_100_WW0jSS,
  Control_101_WW1jSS,
  Control_110_RoutinSS,
  Control_111_TopSS,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/000_WW0j",
  "Control/001_WW1j",
  "Control/010_Routin",
  "Control/011_Top",
  "Control/100_WW0jSS",
  "Control/101_WW1jSS",
  "Control/110_RoutinSS",
  "Control/111_TopSS"
};

#endif
