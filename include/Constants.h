#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1


#include "CutsMonoH.h"
//#include "CutsTop.h"
//#include "CutsTTDM.h"
//#include "CutsWW.h"
//#include "CutsWZ.h"


const float lumi50ns_fb = 0.040;
//const float lumi25ns_fb = 1.269;
const float lumi25ns_fb = 2.120;


const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =  0.000511;  // [GeV]
const float MUON_MASS     =  0.106;     // [GeV]
const float TAU_MASS      =  1.777;     // [GeV]
const float Z_MASS        = 91.188;     // [GeV]


const double metvar_bins[] = {20, 25, 30, 45, 1000};  // [GeV]


const int njetbin = 3;  // 0jet, 1jet, 2+jet, all


const int nchannel = 9;

enum {
  ee,
  mm,
  em,
  ll,
  eee,
  eem,
  emm,
  mmm,
  lll
};

const TString schannel[nchannel] = {
  "ee",
  "mm",
  "em",
  "ll",
  "eee",
  "eem",
  "emm",
  "mmm",
  "lll"
};

const TString lchannel[nchannel] = {
  "ee",
  "#mu#mu",
  "e#mu",
  "all",
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "all"
};


// https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
const float csvv2ivf_looseWP  = 0.605;
const float csvv2ivf_mediumWP = 0.89;
const float csvv2ivf_tightWP  = 0.97;


// MET filters
//------------------------------------------------------------------------------
enum {
  HBHENoiseFilter,
  HBHENoiseIsoFilter,
  CSCTightHalo2015Filter,
  EcalDeadCellTriggerPrimitiveFilter,
  goodVertices,
  eeBadScFilter,
  chargedHadronTrackResolutionFilter,
  muonBadTrackFilter,
  noFilter,
  allFilter,
  nfilter
};

const TString sfilter[nfilter] = {
  "HBHENoiseFilter",
  "HBHENoiseIsoFilter",
  "CSCTightHalo2015Filter",
  "EcalDeadCellTriggerPrimitiveFilter",
  "goodVertices",
  "eeBadScFilter",
  "chargedHadronTrackResolutionFilter",
  "muonBadTrackFilter",
  "noFilter",
  "allFilter"
};


#endif
