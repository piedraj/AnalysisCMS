#if !defined(MYLIB_CUTSTOP_H)
#define MYLIB_CUTSTOP_H 1

#include "TString.h"


enum {
  Top_00_Has2Leptons,
  Top_00_mll20,
  Top_00_Zveto,
  Top_00_Met40,
  Top_00_Met50,
  Top_00_ht260,
  //Top_00_htJets100,
  Top_00_ht300,
  //Top_00_htJets125,
  Top_00_ht260Met50,    
  //Top_00_htJets100Met50,    
  Top_00_ht300Met50,    
  //Top_00_htJets125Met50,    
  Top_01_Has2Jets,
  Top_02_Has1BJet,
  ncut  // This line should be always last
};

const TString scut[ncut] = {
  "Top/00_Has2Leptons",
  "Top/00_mll20",
  "Top/00_Zveto",
  "Top/00_Met40",
  "Top/00_Met50",
  "Top/00_ht260",
 // "Top/00_htJets100", 
  "Top/00_ht300",
 // "Top/00_htJets125",
  "Top/00_ht260Met50",
 // "Top/00_htJet100Met50",
  "Top/00_ht300Met50",
 // "Top/00_htJet125Met50",

  "Top/01_Has2Jets",
  "Top/02_Has1BJet"
};

#endif
