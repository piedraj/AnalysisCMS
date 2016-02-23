#if !defined(MYLIB_CUTSTOP_H)
#define MYLIB_CUTSTOP_H 1

#include "TString.h"


enum {
  Top_00_Has2Leptons,
  //Top_00_mll20Has2Leptons,
  //Top_00_ZVETOHas2Leptons,
  //Top_00_ht250Has2Leptons,
  //Top_00_MET40Has2Leptons,
  //Top_00_MET50Has2Leptons,
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Top/00_Has2Leptons",
  //"Top/00_ZVETOHas2Leptons",
  //"Top/00_mll20Has2Lepton",
  //"Top/00_ht250Has2Leptons",
  //"Top/00_MET40Has2Leptons",
  //"Top/00_MET50Has2Leptons",
  "Top/01_Has2Jets",
  "Top/02_Has1BJet"
};

#endif
