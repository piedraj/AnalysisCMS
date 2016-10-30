#if !defined(MYLIB_CUTSSHAPE_H)
#define MYLIB_CUTSSHAPE_H 1

#include "TString.h"


enum {
  Shape_01_Has2Leptons,
  Shape_02_HasOnly2Leptons,
  Shape_03_Has2Jets,
  Shape_04_MET,
  Shape_05_DeltaPhi,
  Shape_06_mll,
  Shape_07_Zveto,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Shape/01_Has2Leptons",
  "Shape/02_HasOnly2Leptons",
  "Shape/03_Has2Jets",
  "Shape/04_MET",
  "Shape/05_DeltaPhi",
  "Shape/06_mll",
  "Shape/07_Zveto"
};

#endif
