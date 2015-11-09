#ifndef HistogramReader_h
#define HistogramReader_h


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


const Float_t _yoffset = 0.050;


class HistogramReader
{
 public :
  
  HistogramReader(TString const &inputdir,
		  TString const &outputdir);

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
			  Float_t        xmin         = -999,
			  Float_t        xmax         = -999,
			  Float_t        ymin         = -999,
			  Float_t        ymax         = -999);

  TLegend* DrawLegend    (Float_t        x1,
			  Float_t        y1,
			  TH1*           hist,
			  TString        label,
			  TString        option  = "p",
			  Float_t        tsize   = 0.030,
			  Float_t        xoffset = 0.200,
			  Float_t        yoffset = _yoffset);

  void     DrawTLatex    (Font_t         tfont,
			  Float_t        x,
			  Float_t        y,
			  Float_t        tsize,
			  Short_t        align,
			  const char*    text,
			  Bool_t         setndc = true);

  Float_t  GetMaximum    (TH1*           hist,
			  Float_t        xmin = -999,
			  Float_t        xmax = -999);

  void     MoveOverflows (TH1*           hist,
			  Float_t        xmin = -999,
			  Float_t        xmax = -999);
 
  void     SetAxis       (TH1*           hist,
			  TString        xtitle,
			  TString        ytitle,
			  Float_t        size,
			  Float_t        xoffset,
			  Float_t        yoffset);

  void     SetLuminosity (Float_t        lumi) {_luminosity_fb = lumi;}

  void     SetStackOption(TString        option) {_stackoption = option;}
  
  Float_t  Yield          (TH1*           hist);
 

 private :

  Bool_t                _drawratio;
  Bool_t                _savepdf;
  Bool_t                _savepng;
  Float_t               _luminosity_fb;
  TString               _inputdir;
  TString               _outputdir;
  TString               _stackoption;

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
