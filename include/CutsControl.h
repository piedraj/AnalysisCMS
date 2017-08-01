#if !defined(MYLIB_CUTSCONTROL_H)
#define MYLIB_CUTSCONTROL_H 1

#include "TString.h"


enum {
  Control_00_NoCuts,
  Control_01_DY,
  Control_02_WW,
  Control_03_Top,
  Control_04_SS,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Control/00_NoCuts",
  "Control/01_DY",
  "Control/02_WW",
  "Control/03_Top",
  "Control/04_SS"
};

#endif
