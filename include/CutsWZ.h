#if !defined(MYLIB_CUTSWZ_H)
#define MYLIB_CUTSWZ_H 1

#include "TString.h"


enum {
  WZ_00_Exactly3Leptons,
  WZ_01_HasZ,
  WZ_02_HasW,
  WZ_03_BVeto,
  WZ_04_ZRegion,
  WZ_05_TopRegion,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "WZ/00_Exactly3Leptons",
  "WZ/01_HasZ",
  "WZ/02_HasW",
  "WZ/03_BVeto",
  "WZ/04_ZRegion",
  "WZ/05_TopRegion",
};

#endif
