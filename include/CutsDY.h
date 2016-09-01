#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  DY_01_DYee,
  DY_02_DYmm,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "DY/01_DYee",
  "DY/02_DYmm",
};

#endif 
