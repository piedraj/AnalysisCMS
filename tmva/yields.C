#include <iostream>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"


// Constants
//------------------------------------------------------------------------------
const TString inputdir = "../minitrees/TTDM/";


// Functions
//------------------------------------------------------------------------------
float GetYield  (TString sample);
void  PrintYield(TString sample,
		 float   yield);


// Data members
//------------------------------------------------------------------------------
TString _signal;
float   _cut;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// yields
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void yields(TString signal = "ttDM0001pseudo0010",
	    float   cut    = 0.56)
{
  _signal = signal;
  _cut    = cut;

  printf("\n");

  float nsignal = GetYield(_signal);
  float ndata   = GetYield("01_Data");

  float nexpected = 0;
  
  nexpected += GetYield("14_HZ");
  nexpected += GetYield("06_WW");
  nexpected += GetYield("02_WZTo3LNu");
  nexpected += GetYield("03_ZZ");
  nexpected += GetYield("11_Wg");
  nexpected += GetYield("07_ZJets");
  nexpected += GetYield("09_TTV");
  nexpected += GetYield("04_TTTo2L2Nu");
  nexpected += GetYield("05_ST");
  nexpected += GetYield("00_Fakes");

  printf("--------------------------------\n");

  PrintYield("signal",   nsignal);
  PrintYield("data",     ndata);
  PrintYield("expected", nexpected);

  printf("\n");
}


//------------------------------------------------------------------------------
// GetYield
//------------------------------------------------------------------------------
float GetYield(TString sample)
{
  TFile* file = new TFile(inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float eventW;
  float mva;

  tree->SetBranchAddress("eventW", &eventW);
  tree->SetBranchAddress("mva_" + _signal, &mva);

  Long64_t nentries = tree->GetEntries();

  float counter = 0; 

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    if (mva > _cut) counter += eventW;
  }

  PrintYield(sample, counter);

  return counter;
}


//------------------------------------------------------------------------------
// PrintYield
//------------------------------------------------------------------------------
void PrintYield(TString sample, float yield)
{
  printf(" %-20s   %.2f\n", sample.Data(), yield);
}
