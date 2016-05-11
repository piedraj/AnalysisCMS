#if !defined(MYLIB_CUTSSTOP_H)
#define MYLIB_CUTSSTOP_H 1

#include "TString.h"


enum {
  Stop_00_Has2Leptons,
  Stop_00_mll20,
  Stop_00_Zveto,
  Stop_00_Met40,
  Stop_00_Met50,
//  Stop_00_ht260,
  Stop_00_htjets100,
//  Stop_00_ht300,
  Stop_00_htjets125,
  //Stop_00_ht260Met50,    
  Stop_00_htjets100Met50,    
  //Stop_00_ht300Met50,    
  Stop_00_htjets125Met50,
  Stop_00_htjets150,
  Stop_00_htjets150Met50,    
  Stop_01_Has2Jets,
  Stop_02_Has1BJet,
  Stop_02_Has1BJetHtJets150,
  Stop_02_Has2BJet,
  
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Stop/00_Has2Leptons",
  "Stop/00_mll20",
  "Stop/00_Zveto",
  "Stop/00_Met40",
  "Stop/00_Met50",
 // "Stop/00_ht260",
  "Stop/00_htjets100", 
 // "Stop/00_ht300",
  "Stop/00_htjets125",
 // "Stop/00_ht260Met50",
  "Stop/00_htJet100Met50",
 // "Stop/00_ht300Met50",
  "Stop/00_htJet125Met50",
  "Stop/00_htjets150",
  "Stop/00_htjets150Met50", 
  "Stop/01_Has2Jets",
  "Stop/02_Has1BJet",
  "Stop/02_Has1BJetHtJets150",
  "Stop/02_Has2BJet"
};

#endif
