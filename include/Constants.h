#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

//#include "CutsControl.h"
//#include "CutsDY.h"
//#include "CutsFR.h"
//#include "CutsMonoH.h"
//#include "CutsPR.h"
//#include "CutsShape.h"
#include "CutsStop.h"
//#include "CutsTop.h"
//#include "CutsTTDM.h"
//#include "CutsWW.h"
//#include "CutsWZ.h"

//#include "BTagWorkingPoints74X.h"
//#include "BTagWorkingPoints76X.h"
//#include "BTagWorkingPoints80X.h"
#include "BTagWorkingPoints80XReReco.h"

// https://hypernews.cern.ch/HyperNews/CMS/get/physics-announcements/4495.html
const double lumi_fb_Run2016BCD = 12.610;  // 2016B + 2016C + 2016D
const double lumi_fb_Run2016B   =  5.788;  // From run 272007 to run 275376
const double lumi_fb_2016_susy  =  9.983;  // run<276502
const double lumi_fb_2015       =  2.318;  // 2015C + 2015D35867.060
const double lumi_fb_Full2016   = 35.867;  // 2016B to 2016H
const double lumi_error_percent =  2.6;    // Preliminary

const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const double BOTTOM_MASS   =   4.18;      // [GeV]
const double TOP_MASS      = 173.34;      // [GeV]
const double ELECTRON_MASS =   0.000511;  // [GeV]
const double MUON_MASS     =   0.106;     // [GeV]
const double TAU_MASS      =   1.777;     // [GeV]
const double Z_MASS        =  91.188;     // [GeV]
const double H_MASS        = 125.0;       // [GeV]


// Systematic uncertainties
//------------------------------------------------------------------------------
enum {
  nominal,
  JESMaxdo,
  JESMaxup,
  LepElepTdo,
  LepElepTup,
  LepMupTdo,
  LepMupTup,
  METdo,
  METup,
  Btagdo,
  Btagup,
  BtagFSdo,
  BtagFSup,
  Idisodo,
  Idisoup,
  Triggerdo,
  Triggerup,
  Recodo,
  Recoup,
  Fastsimdo,
  Fastsimup,
  Topptup,
  Topptdo,
  Isrnjetup,
  Isrnjetdo,
  nsystematic  // This line should be always last
};

const TString ssystematic[nsystematic] = {
  "nominal",
  "JESMaxdo",
  "JESMaxup",
  "LepElepTdo",
  "LepElepTup",
  "LepMupTdo",
  "LepMupTup",
  "METdo",
  "METup",
  "Btagdo",
  "Btagup",
  "BtagFSdo",
  "BtagFSup",
  "Idisodo",
  "Idisoup",
  "Triggerdo",
  "Triggerup",
  "Recodo",
  "Recoup",
  "Fastsimdo",
  "Fastsimup",
  "Topptup",
  "Topptdo",
  "Isrnjetup",
  "Isrnjetdo"
};

const bool systematicfromweight[nsystematic] = {
  true,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true
};


// Process colors
//------------------------------------------------------------------------------
const Color_t color_Signal    = kRed;
const Color_t color_Fakes     = kGray+1;
const Color_t color_Data      = kBlack;
const Color_t color_WZTo3LNu  = kOrange-2;
const Color_t color_VZ        = kOrange+3;
const Color_t color_TTTo2L2Nu = kYellow;
const Color_t color_ST        = kYellow+3;
const Color_t color_WW        = kAzure-9;
const Color_t color_ZJets     = kGreen+2;
const Color_t color_WJets     = kGray+1;
const Color_t color_TTV       = kGreen-6;
const Color_t color_TTZ       = kOrange+2;
const Color_t color_HWW       = kAzure-7;
const Color_t color_Wg        = kBlue;
const Color_t color_WgStar    = kBlue+2;
const Color_t color_Zg        = kTeal;
const Color_t color_VVV       = kYellow-6;
const Color_t color_HZ        = kOrange+1;


// Branching ratios
//------------------------------------------------------------------------------
const double W2e     = 0.1075;
const double W2m     = 0.1057;
const double W2tau   = 0.1125;
const double Z2ll    = 0.033658;
const double WZ23lnu = 3 * Z2ll * (W2e + W2m + W2tau);
const double WW2lnu  = (W2e + W2m + W2tau) * (W2e + W2m + W2tau);

const int njetbin = 3;  // 0jet, 1jet, 2+jet, all


enum {
  e,
  m,
  l, 
  ee,
  mm,
  em,
  ll,
  eee,
  eem,
  emm,
  mmm,
  lll,
  nchannel  // This line should be always last
};

const TString schannel[nchannel] = {
  "e",
  "m",
  "l",
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
  "e",
  "#mu",
  "l",
  "ee",
  "#mu#mu",
  "e#mu",
  "ll",
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "lll"
};


enum {roc_background, roc_signal, roc_none};


// MET related
//------------------------------------------------------------------------------
const double metvar_bins     [] = {20, 25, 30, 45, 1000};  // [GeV]
const double metvar_bins_draw[] = {20, 25, 30, 45,  100};  // [GeV]

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
  nfilter  // This line should be always last
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
