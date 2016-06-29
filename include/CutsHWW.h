#if !defined(MYLIB_CUTSFR_H)
#define MYLIB_CUTSFR_H 1

#include "TString.h"

enum {
  HWW_01_hww2l2v_13TeV_of0j,
  HWW_02_hww2l2v_13TeV_of1j,
  HWW_03_hww2l2v_13TeV_em_0j,
  HWW_04_hww2l2v_13TeV_em_1j,
  HWW_05_hww2l2v_13TeV_me_0j,
  HWW_06_hww2l2v_13TeV_me_1j,
  ncut  // This line should be always last                                                                                                                  
};

const TString scut[ncut] = {
  "HWW/01_hww2l2v_13TeV_of0j",
  "HWW/02_hww2l2v_13TeV_of1j",
  "HWW/03_hww2l2v_13TeV_em_0j",
  "HWW/04_hww2l2v_13TeV_em_1j",
  "HWW/05_hww2l2v_13TeV_me_0j",
  "HWW/06_hww2l2v_13TeV_me_1j",
};

#endif 
