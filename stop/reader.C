#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
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
#include "include/Constants.h"
#include "include/CutsStop.h"
#include <fstream>
#include <iostream>

//----------------------------------------------
// Global Variables
// ---------------------------------------------

TString FileAddress = "Feb2017_rootfiles_Ghent_ReMiniAODSpring16/nominal/Stop/";
const int nsample = 1;
TString SampleName [nsample] = {"DoubleEG_Run2016B-03Feb2017_ver2-v2.root"};
//TString  _sample   [nsample] = {"T2tt", "TTbar", "ST", "DYJets", "WW", "TTV", "VZ"};



/* Functiion to read a simple Histo from AnalysisCMS output
 *  
 * sample    = only the .root output
 * histoname = h_variable 
 * scut = see include/CutsStop.h
 * channel = ee,mm,em
 * jetBin = " ", 0jet, 1jet, 2jet
 * lumi = 5.8 for runB; 35.867 for all 2016
 *
 *
 * */

TH1F *reader0 (TString sample, float lumi, TString histoname, TString scut, TString channel, TString jetBin="")
{

  TFile* file0  = new TFile (sample, "read"); 
  TH1F*  histo0 = (TH1F*) file0->Get(scut + "/" + jetBin + histoname + "_" + channel);
  std::cout << "Histo Integral     " <<  histo0 -> Integral() << std::endl; 
  std::cout << "Scale to lumi = " << lumi << std::endl; 
  histo0 -> Scale(lumi);
  std::cout << "Histo Scaled Integral     " <<  histo0 -> Integral() << std::endl;  
  return histo0;
}

/* This function reads a TTree in a filename and return the number of entries*/

float reader1 (TString filename)
{ 
  
   TFile* file = TFile::Open(filename);

   TTree* latino = (TTree*)file->Get("latino");

   float N = latino -> GetEntries(); 

   return N; 
}
  
/* This function complete for n iteration the file name and uses recursively reader1 giving the sum */ 

void user (TString filename, int maxIter)
{
  float entries = 0; 
  //you have several parts ex: latino_TTTo2L2Nu_ext1_0001__part8.root 9,10, ......34
  for (int i = 0; i <= maxIter; i++)
   {
    TString file = filename;
    file += std::to_string(i);
    file += ".root";
    std::cout<<file<<std::endl;
    float n = reader1(file);
    entries += n;      
   }
  
  std::cout << entries << std::endl; 
}
