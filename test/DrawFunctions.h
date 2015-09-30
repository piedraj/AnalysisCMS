#ifndef _DRAW_FUNCTIONS_H
#define _DRAW_FUNCTIONS_H (1)


#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"


TLegend*  DrawLegend               (Float_t       x1,
				    Float_t       y1,
				    TObject*      hist,
				    TString       label,
				    TString       option  = "p",
				    Float_t       tsize   = 0.030,
				    Float_t       xoffset = 0.200,
				    Float_t       yoffset = 0.048);

void      DrawTLatex               (Font_t        tfont,
				    Double_t      x,
				    Double_t      y,
				    Double_t      tsize,
				    Short_t       align,
				    const char*   text,
				    Bool_t        setndc = true);

Double_t  GetMaximumIncludingErrors(TH1*          h,
				    Double_t      xmin = -999,
				    Double_t      xmax = -999);

void      MoveOverflowBins         (TH1*          h,
				    Double_t      xmin = -999,
				    Double_t      xmax = -999);

void      SetAxis                  (TH1*          hist,
				    TString       xtitle,
				    TString       ytitle,
				    Float_t       size,
				    Float_t       xoffset,
				    Float_t       yoffset);

Double_t  Yield                    (TH1*          h);


#endif
