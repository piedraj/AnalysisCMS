#if !defined(MYLIB_CUTSFR_H)
#define MYLIB_CUTSFR_H 1

#include "TString.h"

enum {
  SS_01_hww2l2v_13TeV_of0j,
  SS_02_hww2l2v_13TeV_of1j,
  SS_03_hww2l2v_13TeV_em_0j,
  SS_04_hww2l2v_13TeV_em_1j,
  SS_05_hww2l2v_13TeV_me_0j,
  SS_06_hww2l2v_13TeV_me_1j,
  ncut  // This line should be always last                                                                                                                  
};

const TString scut[ncut] = {
  "SS/01_hww2l2v_13TeV_of0j",
  "SS/02_hww2l2v_13TeV_of1j",
  "SS/03_hww2l2v_13TeV_em_0j",
  "SS/04_hww2l2v_13TeV_em_1j",
  "SS/05_hww2l2v_13TeV_me_0j",
  "SS/06_hww2l2v_13TeV_me_1j",
};

#endif 
