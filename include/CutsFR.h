#if !defined(MYLIB_CUTSFR_H)
#define MYLIB_CUTSFR_H 1

#include "TString.h"

enum {
  FR_00_QCD,
  FR_01_WRegion,
  FR_02_ZRegion,
  ncut  // This line should be always last                                                                                                                  
};

const TString scut[ncut] = {
  "FR/00_QCD",
  "FR/01_WRegion",
  "FR/02_ZRegion",
};

#endif 
