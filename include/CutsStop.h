#if !defined(MYLIB_CUTSSTOP_H)
#define MYLIB_CUTSSTOP_H 1

#include "TString.h"


enum {
  Stop_00_Has2Leptons,
  Stop_00_2LMt2upper100, 
  Stop_00_mll20,
  Stop_00_Zveto,
  // We fill our testing minitre
  Stop_00_Tag,
  Stop_00_NoTag,
  Stop_01_MET,
  Stop_02_Has2Jets,
  Stop_02_METTight,
  Stop_03_Has1BJet,
  //Stop_03_PassPt30,
//  Stop_02_Has2BJet,
  
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Stop/00_Has2Leptons",
  "Stop/00_2LMt2upper100",
  "Stop/00_mll20",
  "Stop/00_Zveto",
  "Stop/00_Tag",
  "Stop/00_NoTag",
  "Stop/01_MET",
  "Stop/02_Has2Jets",
  "Stop/02_METTight",
  "Stop/03_Has1BJet"
  //"Stop/03_PassPt30",
 // "Stop/02_Has2BJet"
};

#endif
