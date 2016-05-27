#if !defined(MYLIB_CUTSTTDM_H)
#define MYLIB_CUTSTTDM_H 1

#include "TString.h"


enum {
  TTDM_00_Has2Leptons,
  TTDM_010_Routin,
  TTDM_011_Routin,
  TTDM_02_Preselection,
  TTDM_03_AN16105,
  TTDM_04_ttSideBand,
  Control_00_WW0jet, 
  Control_01_WW1jet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "TTDM/00_Has2Leptons",
  "TTDM/010_Routin",
  "TTDM/011_Routin",
  "TTDM/02_Preselection",
  "TTDM/03_AN16105",
  "TTDM/04_ttSideBand",
  "TTDM/Control_00_WW0jet",
  "TTDM/Control_01_WW1jet"
};

#endif
