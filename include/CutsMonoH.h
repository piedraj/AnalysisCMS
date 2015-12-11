#if !defined(MYLIB_CUTSMONOH_H)
#define MYLIB_CUTSMONOH_H 1

#include "TString.h"


enum {
  MonoH_100_Mc,
  MonoH_101_DRll,
  MonoH_102_MpMet,
  MonoH_103_CR,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "MonoH/100_Mc",
  "MonoH/101_DRll",
  "MonoH/102_MpMet",
  "MonoH/103_CR"
};

#endif
