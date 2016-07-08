#if !defined(MYLIB_CUTSFR_H)
#define MYLIB_CUTSFR_H 1

#include "TString.h"

enum {
  FR_00_QCD,
  FR_01_WRegion,
  FR_02_WRegionQCD,
  FR_03_ZRegion,
  FR_04_ZRegionQCD,
  ncut  // This line should be always last                                                                                                                  
};

const TString scut[ncut] = {
  "FR/00_QCD",
  "FR/01_WRegion",
  "FR/02_WRegionQCD",
  "FR/03_ZRegion",
  "FR/04_ZRegionQCD",
};

#endif 
