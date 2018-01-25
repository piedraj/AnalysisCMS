////////////////////////////////////////////////////////////
//
// A group of helper functions used to compare histograms
// and profiles between two different releases
//
////////////////////////////////////////////////////////////

/////
// Some includes

#ifndef PlotHelpers_h
#define PlotsHelpers_h 1

#include "TH1F.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include <vector>
//#include "MuonStyle.h"
//#include <iostream.h>




/////
// Uncomment the following line to get more debuggin output
//#define DEBUG

////////////////////////////////////////////////////////////
//
// Sets the global style for the whole system.
//
// Note: There might be redundancies in other places so
//       things can be further simplified
//


class PlotHelpers {


 public:

  //  PlotHelpers();
  //~PlotHelpers();



//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function sets the style for a given histogram
//
// Note: There might be redundancies in other places so
//       things can be further simplified
//
  void SetHistogramStyle(TH1* h, Style_t mstyle, Color_t color, Size_t msize = 0.7, Width_t lwidth = 2,Float_t tsize = 0.05, Float_t toffset = 1.2) ;
//
////////////////////////////////////////////////////////////

  void SetGlobalStyle();
////////////////////////////////////////////////////////////
//
// This function finds the list of TDirectories in a branch
// that match a given string

  TList* GetListOfDirectories(TDirectory* dir, const char* match = 0);

////////////////////////////////////////////////////////////
//
// This function goes to the right branch inside the file
// looking for branches having branchContent.
// It returns a list with all those branches,
//
  TList* getListOfBranches(const char* dataType, TFile* file, const char* branchContent);
  TList* GetListOfBranches(const char* dataType, TFile* file);
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function performs a compatibility test between two
// histogram based on the Kolmogorov-Smirnof algorithm. It
// also prints the value in a TPaveLabel at the upper-right
// corner.
// The return value contains the result of the test
//
double KolmogorovTest(TH1 *h1, TH1 *h2);
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function draws the stat box for the two plots
//
 void setStats(TH1* s, TH1* r, double startingY, double startingX = .1, bool fit = false);
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// Plot a page with several histograms
//
 void PlotNHistograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     unsigned int nhistos, const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0, bool resol = false,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0                     
		     ) ;
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Plot a page with 4 histograms
//
 void Plot4Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol = 0,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0);
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Plot a page with 6 histograms
//
 void Plot6Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol=false,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0);
//
////////////////////////////////////////////////////////////

 void Plot5Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol=false,	     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0);
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Normalize the two histograms to the entries on the first histogram
//
 void NormalizeHistogramsToFirst(TH1* h1, TH1* h2);
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Normalize the two histograms to unity
//
void NormalizeHistogramsTo1(TH1* h1, TH1* h2);
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// do the ratio of the two histograms
// 
TH1F* PlotRatiosHistograms(TH1* h1, TH1* h2, const char *collname);
//
////////////////////////////////////////////////////////////

};

#endif
