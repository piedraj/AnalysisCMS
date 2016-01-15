#ifndef HistogramReader_h
#define HistogramReader_h

//#include "../include/Constants.h"

#include "TCanvas.h"
//#include "TFile.h"
//#include "TFrame.h"
//#include "TH1.h"
//#include "TROOT.h"
//#include "TTree.h"
#include "TString.h"

class HistogramReader
{
 public : 

  HistogramReader() {}
  ~HistogramReader() {}

  void     Draw               (TString        hname);
			       
};




#endif
