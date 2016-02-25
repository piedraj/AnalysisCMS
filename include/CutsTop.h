#if !defined(MYLIB_CUTSTOP_H)
#define MYLIB_CUTSTOP_H 1

#include "TString.h"


enum {
  Top_00_Has2Leptons,
  Top_00_ht260Has2Leptons,
  Top_00_ht260Met40Has2Leptons,
  Top_00_ht260Met50Has2Leptons,    
  Top_00_MET40Has2Leptons,
  Top_00_MET50Has2Leptons,
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Top/00_Has2Leptons",
  "Top/00_ht260Has2Leptons",
  "Top/00_ht260Met40Has2Leptons",
  "Top/00_ht260Met50Has2Leptons",
  "Top/00_MET40Has2Leptons",
  "Top/00_MET50Has2Leptons",
  "Top/01_Has2Jets",
  "Top/02_Has1BJet"
};

#endif
