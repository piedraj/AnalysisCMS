#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_WW0jet,
  Control_01_WW1jet,
  Control_10_Routin,
  Control_11_Top,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_WW0jet",
  "Control/01_WW1jet",
  "Control/10_Routin",
  "Control/11_Top"
};

#endif
