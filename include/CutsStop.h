#if !defined(MYLIB_CUTSSTOP_H)
#define MYLIB_CUTSSTOP_H 1

#include "TString.h"


enum {
  Stop_00_Has2Leptons,
  Stop_00_2LMt2upper100, 
  Stop_00_mll20,
  Stop_00_Zveto,
  // We fill our testing minitre// We fill our testing minitreee 
  Stop_01_Has2Jets,
  Stop_01_PreSelection,
  Stop_02_Has1BJet,
  Stop_02_PassPt30,
//  Stop_02_Has2BJet,
  
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Stop/00_Has2Leptons",
  "Stop/00_2LMt2upper100",
  "Stop/00_mll20",
  "Stop/00_Zveto",
  "Stop/01_Has2Jets",
  "Stop/01_PreSelection",
  "Stop/02_Has1BJet",
  "Stop/02_PassPt30",
 // "Stop/02_Has2BJet"
};

#endif
