#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1


//#include "CutsMonoH.h"
//#include "CutsTop.h"
//#include "CutsTTDM.h"
//#include "CutsWW.h"
#include "CutsWZ.h"


const float lumi_fb = 1.371;  // Luminosity for any blinded analysis
//const float lumi_fb = 2.318;
const float lumi_error_percent = 2.7;

const int ELECTRON_FLAVOUR = 11;
const int MUON_FLAVOUR     = 13;
const int TAU_FLAVOUR      = 15;
const int Z_FLAVOUR        = 23;

const float ELECTRON_MASS =   0.000511;  // [GeV]
const float MUON_MASS     =   0.106;     // [GeV]
const float TAU_MASS      =   1.777;     // [GeV]
const float Z_MASS        =  91.188;     // [GeV]
const float H_MASS        = 125.0;       // [GeV]


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
const Color_t color_Zg        = kTeal;
const Color_t color_VVV       = kYellow-6;
const Color_t color_HZ        = kOrange+1;


// Branching ratios
//------------------------------------------------------------------------------
const Double_t W2e     = 0.1075;
const Double_t W2m     = 0.1057;
const Double_t W2tau   = 0.1125;
const Double_t Z2ll    = 0.033658;
const Double_t WZ23lnu = 3 * Z2ll * (W2e + W2m + W2tau);


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


// https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation76X
// pfCombinedMVAV2BJetTags
const float cMVAv2L = -0.715;
const float cMVAv2M =  0.185;
const float cMVAv2T =  0.875;


// https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation76X
// pfCombinedInclusiveSecondaryVertexV2BJetTags
const float CSVv2L = 0.460;
const float CSVv2M = 0.800;
const float CSVv2T = 0.935;


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
