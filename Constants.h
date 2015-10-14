#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1


const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =  0.000511;  // [GeV]
const float MUON_MASS     =  0.106;     // [GeV]
const float TAU_MASS      =  1.777;     // [GeV]
const float Z_MASS        = 91.188;     // [GeV]


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
  nlep2_cut0_Exactly2Leptons,
  nlep2_cut1_ZVeto,
  nlep3_cut0_Exactly3Leptons,
  nlep3_cut1_HasZ,
  nlep3_cut2_HasW,
  nlep3_cut3_OneBJet
};

const TString scut[ncut] = {
  "nlep2_cut0_Exactly2Leptons",
  "nlep2_cut1_ZVeto",
  "nlep3_cut0_Exactly3Leptons",
  "nlep3_cut1_HasZ",
  "nlep3_cut2_HasW",
  "nlep3_cut3_OneBJet"
};


#endif
