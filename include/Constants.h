#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

#include "CutsControl.h"
//#include "CutsFR.h"
//#include "CutsMonoH.h"
//#include "CutsTop.h"
//#include "CutsStop.h"
//#include "CutsTTDM.h"
//#include "CutsWW.h"
//#include "CutsWZ.h"

//#include "BTagWorkingPoints74X.h"
#include "BTagWorkingPoints76X.h"


const double lumi_fb_2016B       = 0.218;
const double lumi_fb_blind_2015D = 1.324;
const double lumi_fb_blind       = 1.371;
const double lumi_fb             = 2.318;
const double lumi_error_percent  = 2.7;

const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

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
  Idisodo,
  Idisoup,
  Triggerdo,
  Triggerup,
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
  "Idisodo",
  "Idisoup",
  "Triggerdo",
  "Triggerup"
};


// Process colors
//------------------------------------------------------------------------------
const Color_t color_Signal    = kRed;
const Color_t color_Fakes     = kGray+1;
const Color_t color_Data      = kBlack;
const Color_t color_WZTo3LNu  = kOrange-2;
const Color_t color_ZZ        = kOrange+3;
const Color_t color_TTTo2L2Nu = kYellow;
const Color_t color_ST        = kYellow+3;
const Color_t color_WW        = kAzure-9;
const Color_t color_ZJets     = kGreen+2;
const Color_t color_WJets     = kGray+1;
const Color_t color_TTV       = kGreen-6;
const Color_t color_HWW       = kAzure-7;
const Color_t color_Wg        = kBlue;
const Color_t color_WgStar    = kBlue+2;
const Color_t color_Zg        = kTeal;
const Color_t color_VVV       = kYellow-6;
const Color_t color_HZ        = kOrange+1;


// Branching ratios
//------------------------------------------------------------------------------

const double metvar_bins[] = {20, 25, 30, 45, 1000};  // [GeV]
const double metvar_bins_draw[] = {20, 25, 30, 45, 100};  // [GeV]

const double W2e     = 0.1075;
const double W2m     = 0.1057;
const double W2tau   = 0.1125;
const double Z2ll    = 0.033658;
const double WZ23lnu = 3 * Z2ll * (W2e + W2m + W2tau);


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
