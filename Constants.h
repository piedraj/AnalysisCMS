#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1


const float lumi50ns_fb = 0.04003;
const float lumi25ns_fb = 0.01547;


const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =  0.000511;  // [GeV]
const float MUON_MASS     =  0.106;     // [GeV]
const float TAU_MASS      =  1.777;     // [GeV]
const float Z_MASS        = 91.188;     // [GeV]


const int njetbin = 3;  // 0jet, 1jet, 2+jet, all


const int nchannel = 9;

enum {
  ee,
  em,
  mm,
  ll,
  eee,
  eem,
  emm,
  mmm,
  lll
};

const TString schannel[nchannel] = {
  "ee",
  "em",
  "mm",
  "ll",
  "eee",
  "eem",
  "emm",
  "mmm",
  "lll"
};

TString lchannel[nchannel] = {
  "ee",
  "e#mu",
  "#mu#mu",
  "ll",
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "lll"
};


const int ncut = 6;

enum {
  WW00_Exactly2Leptons,
  WW01_ZVeto,
  WZ00_Exactly3Leptons,
  WZ01_HasZ,
  WZ02_HasW,
  WZ03_OneBJet
};

const TString scut[ncut] = {
  "WW00_Exactly2Leptons",
  "WW01_ZVeto",
  "WZ00_Exactly3Leptons",
  "WZ01_HasZ",
  "WZ02_HasW",
  "WZ03_OneBJet"
};


#endif
