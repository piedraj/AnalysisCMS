#if !defined(MYLIB_CUTSMONOH_H)
#define MYLIB_CUTSMONOH_H 1

#include "TString.h"


enum {
  MonoH_00_Mc,
  MonoH_01_DRll,
  MonoH_02_MpMet,
  MonoH_03_CR,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "MonoH/00_Mc",
  "MonoH/01_DRll",
  "MonoH/02_MpMet",
  "MonoH/03_CR"
};

#endif
