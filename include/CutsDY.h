#if !defined(MYLIB_CUTSDY_H)
#define MYLIB_CUTSDY_H 1

#include "TString.h"


enum {
  DY_00_noCuts,
  DY_01_Has2Leptons,  
  DY_02_DYControl,
  DY_03_BVetoLoose,
  DY_04_Ptll,
  DY_05_dphillmet,
  DY_06_PfMet,
  DY_07_mpMet,
  DY_08_Zwindow,
  DY_09_ZVeto,
  DY_10_TopControl,
  
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "DY/00_noCuts",
  "DY/01_Has2Leptons",  
  "DY/02_DYControl",
  "DY/03_BVetoLoose",
  "DY/04_Ptll" ,
  "DY/05_dphillmet" ,
  "DY/06_PfMet",
  "DY/07_mpMet",
  "DY/08_Zwindow",
  "DY/09_ZVeto" ,
  "DY/10_TopControl" ,
  

};

#endif
