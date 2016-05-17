#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_WW,
  Control_10_Routin,
  Control_11_Top,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_WW",
  "Control/10_Routin",
  "Control/11_Top"
};

#endif
