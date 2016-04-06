#if !defined(MYLIB_CUTSFR_H)
#define MYLIB_CUTSFR_H 1

#include "TString.h"

enum {
  FR_00_QCD,
  ncut  // This line should be always last                                                                                                                  
};

const TString scut[ncut] = {
  "FR/00_QCD"
};

#endif 
