#ifndef HistogramReader_h
#define HistogramReader_h


#pragma once


#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"

#include <fstream>
#include <iostream>
#include <vector>


class HistogramReader
{
 public :
  
  HistogramReader(TString const &rootfiles);

  ~HistogramReader() {}
 
  void     AddProcess    (TString const &filename,
			  TString const &label,
			  Color_t        color);
  
  void     Draw          (TString        hname,
			  TString        xtitle,
			  Int_t          ngroup       = -1,
			  Int_t          precision    =  0,
			  TString        units        = "NULL",
			  Bool_t         setlogy      = false,
			  Bool_t         moveoverflow = true,
			  Double_t       xmin         = -999,
			  Double_t       xmax         = -999,
			  Double_t       ymin         = -999,
			  Double_t       ymax         = -999);

  TLegend* DrawLegend    (Float_t        x1,
			  Float_t        y1,
			  TObject*       hist,
			  TString        label,
			  TString        option  = "p",
			  Float_t        tsize   = 0.030,
			  Float_t        xoffset = 0.200,
			  Float_t        yoffset = 0.048);

  void     DrawTLatex    (Font_t         tfont,
			  Double_t       x,
			  Double_t       y,
			  Double_t       tsize,
			  Short_t        align,
			  const char*    text,
			  Bool_t         setndc = true);

  Double_t GetMaximum    (TH1*           hist,
			  Double_t       xmin = -999,
			  Double_t       xmax = -999);

  void     MoveOverflows (TH1*           hist,
			  double         xmin = -999,
			  double         xmax = -999);
 
  void     SetAxis       (TH1*           hist,
			  TString        xtitle,
			  TString        ytitle,
			  Float_t        size,
			  Float_t        xoffset,
			  Float_t        yoffset);
  
  Double_t Yield         (TH1*           hist);
 

 private :

  Bool_t                _drawratio;
  Bool_t                _savepdf;
  Bool_t                _savepng;
  TString               _rootfiles;

  TFile*                _datafile;
  TH1*                  _datahist;
  Color_t               _datacolor;
  TString               _datalabel;

  std::vector<TFile*>   _mcfile;
  std::vector<TH1*>     _mchist;
  std::vector<Color_t>  _mccolor;
  std::vector<TString>  _mclabel;
};

#endif
