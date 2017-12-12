#ifndef HistogramReader_h
#define HistogramReader_h

#include "../include/Constants.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
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
  
  HistogramReader(const TString& inputdir,
		  const TString& outputdir);

  ~HistogramReader() {}
 
  void     AddData            (const TString& filename,
			       const TString& label,
			       Color_t        color);

  void     AddPrefit          (const TString& filename,
			       const TString& label,
			       Color_t        color);

  void     AddProcess         (const TString& filename,
			       const TString& label,
			       Color_t        color,
			       Int_t          kind  = roc_background,
			       Float_t        scale = -1);

  void     AddSignal          (const TString& filename,
			       const TString& label,
			       Color_t        color,
			       Int_t          kind  = roc_none,
			       Float_t        scale = -1);

  void     AddSystematic      (TString        systematic); 
  
  void     Draw               (TString        hname,
			       TString        xtitle       = "",
			       Int_t          ngroup       = -1,
			       Int_t          precision    = -1,
			       TString        units        = "NULL",
			       Bool_t         setlogy      = false,
			       Bool_t         moveoverflow = true,
			       Float_t        xmin         = -999,
			       Float_t        xmax         = -999,
			       Float_t        ymin         = -999,
			       Float_t        ymax         = -999);

  Float_t  GetBestSignalScoreX(TString        hname,
			       TString        fom,
			       Int_t          ngroup = -1);

  void     DrawLatex          (Font_t         tfont,
			       Float_t        x,
			       Float_t        y,
			       Float_t        tsize,
			       Short_t        align,
			       const char*    text,
			       Bool_t         setndc = true);

  TLegend* DrawLegend         (Float_t        x1,
			       Float_t        y1,
			       TH1*           hist,
			       TString        label,
			       TString        option    = "p",
			       Bool_t         drawyield = true,
			       Float_t        tsize     = 0.030,
			       Float_t        xoffset   = 0.200,
			       Float_t        yoffset   = 0.050);

  Float_t  GetMaximum         (TH1*           hist,
			       Float_t        xmin     = -999,
			       Float_t        xmax     = -999,
			       Bool_t         binError = true);
  
  void     MoveOverflows      (TH1*           hist,
			       Float_t        xmin = -999,
			       Float_t        xmax = -999);
 
  void     SetAxis            (TH1*           hist,
			       TString        xtitle,
			       TString        ytitle,
			       Float_t        xoffset = 5.1,
			       Float_t        yoffset = 2.6);

  void     SetHistogram       (TH1*           hist,
			       Color_t        color,
			       Style_t        fstyle,
			       Style_t        mstyle,
			       Style_t        lstyle,
			       Width_t        lwidth,
			       Int_t          ngroup,
			       Bool_t         moveoverflow,
			       Float_t&       xmin,
			       Float_t&       xmax);

  void     SetChangeLabel     (Bool_t         changelabel) {_changelabel = changelabel;}

  void     SetDataNorm        (Bool_t         datanorm) {_datanorm = datanorm;}

  void     SetDrawRatio       (Bool_t         drawratio) {_drawratio = drawratio;}

  void     SetDrawSignificance(Bool_t         drawsignificance) {_drawsignificance = drawsignificance;}

  void     SetDrawYield       (Bool_t         drawyield) {_drawyield = drawyield;}

  void     SetPublicStyle     (Bool_t         publicstyle) {_publicstyle = publicstyle;}

  void     SetLuminosity      (Float_t        lumi) {_luminosity_fb = lumi;}

  void     SetStackOption     (TString        option) {_stackoption = option;}

  void     SetTitle           (TString        title) {_title = title;}

  void     SetSavePdf         (Bool_t         savepdf) {_savepdf = savepdf;}

  void     SetSavePng         (Bool_t         savepng) {_savepng = savepng;}

  void     SetMinitreeBased   (Bool_t         minitreebased) {_minitreebased = minitreebased;}
  
  Float_t  Yield              (TH1*           hist);

  Float_t  Error              (TH1*           hist);

  void     EventsByCut        (TFile*         file,
			       TString        analysis,
			       TString        hname);

  void     LoopEventsByCut    (TString        analysis,
			       TString        hname);

  void     EventsByChannel    (TFile*         file,
			       TString        level);

  void     LoopEventsByChannel(TString        level);

  Float_t  GetBestScoreX      (TH1*           sig_hist,
			       TH1*           bkg_hist,
			       TString        fom = "S/sqrt(S+B)");

  void     WriteYields        (TH1*           hist,
			       TString        label);

  void     Roc                (TString        hname,
			       TString        xtitle  = "",
			       Int_t          npoints = 100,
			       TString        units   = "NULL",
			       Float_t        xmin    = -999,
			       Float_t        xmax    = -999,
			       TString        fom     = "S / #sqrt{B}");

  void 	   IncludeSystematics (TString        hname);


 private :

  Bool_t                _changelabel;
  Bool_t                _datanorm;
  Bool_t                _drawratio;
  Bool_t                _drawsignificance;
  Bool_t                _drawyield;
  Bool_t                _publicstyle;
  Bool_t                _savepdf;
  Bool_t                _savepng;
  Bool_t                _saveratio;
  Bool_t                _writelabels;
  Bool_t                _writeyields;
  Bool_t                _minitreebased;

  Float_t               _luminosity_fb;
  Float_t               _xmin;
  Float_t               _xmax;
  TString               _inputdir;
  TString               _outputdir;
  TString               _stackoption;
  TString               _title;

  TString 		_datafilename;
  TFile*                _datafile;
  TH1*                  _datahist;
  Color_t               _datacolor;
  TString               _datalabel;

  TString 		_prefitfilename;
  TFile*                _prefitfile;
  TH1*                  _prefithist;
  Color_t               _prefitcolor;
  TString               _prefitlabel;

  TH1*                  _allmchist;
  TString               _allmclabel;

  std::vector<TString>  _mcfilename;
  std::vector<TFile*>   _mcfile;
  std::vector<TH1*>     _mchist;
  std::vector<TH1*>     _mchist_syst;
  std::vector<Color_t>  _mccolor;
  std::vector<TString>  _mclabel;
  std::vector<Float_t>  _mcscale;

  std::vector<TString>  _signalfilename;
  std::vector<TFile*>   _signalfile;
  std::vector<TH1*>     _signalhist;
  std::vector<Color_t>  _signalcolor;
  std::vector<TString>  _signallabel;
  std::vector<Float_t>  _signalscale;

  std::vector<TString>  _systematics;

  std::ofstream         _yields_table;

  std::vector<TFile*>   _roc_signalfile;
  std::vector<TFile*>   _roc_backgroundfile;
  std::vector<Float_t>  _roc_signalscale;
  std::vector<Float_t>  _roc_backgroundscale;
};

#endif
