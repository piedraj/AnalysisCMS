#if !defined(MYLIB_CUTSTOP_H)
#define MYLIB_CUTSTOP_H 1

#include "TString.h"


enum {
  Top_00_Has2Leptons,
  Top_00_Has2Leptons_MET,
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Top/00_Has2Leptons",
  "Top/00_Has2Leptons_MET",
  "Top/01_Has2Jets",
  "Top/02_Has1BJet"
};

#endif
