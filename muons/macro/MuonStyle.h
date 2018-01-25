/**
 * A collection of simple ROOT macros.
 *
 * 
 */

#ifndef MuonStyle_h
#define MuonStyle_h 1

#include <sstream>
#include <iomanip>

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TProfile.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TVirtualPad.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TPostScript.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TAxis.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include <iostream>
#endif

using namespace std;

class MuonStyle {

 public:
  //  MuonStyle();

  TStyle *getStyle(TString name);

// Print all open canvases in separate files
// Type = "png", "eps", etc.

  TCanvas *newCanvas(TString title="", Int_t xdiv=1, Int_t ydiv =1, Int_t w=600, Int_t h=600);

  void printCanvases(TString type);

  //  ~MuonStyle();
};

#endif	/*  */

