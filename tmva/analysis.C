#include <iostream>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"


// Constants
//------------------------------------------------------------------------------
const TString inputdir = "../minitrees/TTDM/";

enum {njetmin, njetmax, nbjetmin, nbjetmax};


// Functions
//------------------------------------------------------------------------------
float GetYield        (TString sample);

void  PrintYield      (TString sample,
		       float   yield);

void  PrintYields     ();

void  GetBoxPopulation(TString sample,
		       float*  box1,
		       float*  box2,
		       float&  mvaregion1_box1_yield,
		       float&  mvaregion1_box2_yield,
		       float&  mvaregion2_box1_yield,
		       float&  mvaregion2_box2_yield);

void  GetScaleFactors ();


// Data members
//------------------------------------------------------------------------------
TString _signal;
float   _cut;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// analysis
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void analysis(TString signal = "ttDM0001pseudo0010",
	      float   cut    = 0.56)
{
  _signal = signal;
  _cut    = cut;

  PrintYields();

  GetScaleFactors();
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

  float yield = 0; 

  Long64_t nentries = tree->GetEntries();

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    if (mva > _cut) yield += eventW;
  }

  PrintYield(sample, yield);

  return yield;
}


//------------------------------------------------------------------------------
// PrintYield
//------------------------------------------------------------------------------
void PrintYield(TString sample, float yield)
{
  printf(" %-20s   %.2f\n", sample.Data(), yield);
}


//------------------------------------------------------------------------------
// PrintYields
//------------------------------------------------------------------------------
void PrintYields()
{
  printf("\n [PrintYields]\n\n");

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
// GetBoxPopulation
//------------------------------------------------------------------------------
void GetBoxPopulation(TString sample,
		      float*  box1,
		      float*  box2,
		      float&  mvaregion1_box1_yield,
		      float&  mvaregion1_box2_yield,
		      float&  mvaregion2_box1_yield,
		      float&  mvaregion2_box2_yield)
{
  TFile* file = new TFile(inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float eventW;
  float nbjet;
  float njet;
  float mva;

  tree->SetBranchAddress("eventW",         &eventW);
  tree->SetBranchAddress("nbjet20loose",   &nbjet);
  tree->SetBranchAddress("njet",           &njet);
  tree->SetBranchAddress("mva_" + _signal, &mva);

  mvaregion1_box1_yield = 0;
  mvaregion1_box2_yield = 0;
  mvaregion2_box1_yield = 0;
  mvaregion2_box2_yield = 0;

  Long64_t nentries = tree->GetEntries();

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    bool reject_mvaregion1 = (mva < _cut-0.2 || mva > _cut);
    bool reject_mvaregion2 = (mva < _cut-0.4 || mva > _cut-0.2);

    bool reject_box1 = ((box1[njetmin]  > 0 && njet  < box1[njetmin])  ||
			(box1[njetmax]  > 0 && njet  > box1[njetmax])  ||
			(box1[nbjetmin] > 0 && nbjet < box1[nbjetmin]) ||
			(box1[nbjetmax] > 0 && nbjet > box1[nbjetmax]));

    bool reject_box2 = ((box2[njetmin]  > 0 && njet  < box2[njetmin])  ||
			(box2[njetmax]  > 0 && njet  > box2[njetmax])  ||
			(box2[nbjetmin] > 0 && nbjet < box2[nbjetmin]) ||
			(box2[nbjetmax] > 0 && nbjet > box2[nbjetmax]));

    if (!reject_mvaregion1 && !reject_box1) mvaregion1_box1_yield += eventW;
    if (!reject_mvaregion1 && !reject_box2) mvaregion1_box2_yield += eventW;
    if (!reject_mvaregion2 && !reject_box1) mvaregion2_box1_yield += eventW;
    if (!reject_mvaregion2 && !reject_box2) mvaregion2_box2_yield += eventW;
  }
}


//------------------------------------------------------------------------------
// GetScaleFactors
//------------------------------------------------------------------------------
void GetScaleFactors()
{
  float box1[] = {1, -1, 0,  1};
  float box2[] = {1, -1, 2, -1};

  float r1b1_data,  r1b2_data,  r2b1_data,  r2b2_data;
  float r1b1_top,   r1b2_top,   r2b1_top,   r2b2_top;
  float r1b1_ww,    r1b2_ww,    r2b1_ww,    r2b2_ww;
  float r1b1_hz,    r1b2_hz,    r2b1_hz,    r2b2_hz;
  float r1b1_wz,    r1b2_wz,    r2b1_wz,    r2b2_wz;
  float r1b1_zz,    r1b2_zz,    r2b1_zz,    r2b2_zz;
  float r1b1_wg,    r1b2_wg,    r2b1_wg,    r2b2_wg;
  float r1b1_zj,    r1b2_zj,    r2b1_zj,    r2b2_zj;
  float r1b1_ttv,   r1b2_ttv,   r2b1_ttv,   r2b2_ttv;
  float r1b1_st,    r1b2_st,    r2b1_st,    r2b2_st;
  float r1b1_fakes, r1b2_fakes, r2b1_fakes, r2b2_fakes;

  GetBoxPopulation("01_Data",      box1, box2, r1b1_data,  r1b2_data,  r2b1_data,  r2b2_data);
  GetBoxPopulation("04_TTTo2L2Nu", box1, box2, r1b1_top,   r1b2_top,   r2b1_top,   r2b2_top);
  GetBoxPopulation("06_WW",        box1, box2, r1b1_ww,    r1b2_ww,    r2b1_ww,    r2b2_ww);
  GetBoxPopulation("14_HZ",        box1, box2, r1b1_hz,    r1b2_hz,    r2b1_hz,    r2b2_hz);
  GetBoxPopulation("02_WZTo3LNu",  box1, box2, r1b1_wz,    r1b2_wz,    r2b1_wz,    r2b2_wz);
  GetBoxPopulation("03_ZZ",        box1, box2, r1b1_zz,    r1b2_zz,    r2b1_zz,    r2b2_zz);
  GetBoxPopulation("11_Wg",        box1, box2, r1b1_wg,    r1b2_wg,    r2b1_wg,    r2b2_wg);
  GetBoxPopulation("07_ZJets",     box1, box2, r1b1_zj,    r1b2_zj,    r2b1_zj,    r2b2_zj);
  GetBoxPopulation("09_TTV",       box1, box2, r1b1_ttv,   r1b2_ttv,   r2b1_ttv,   r2b2_ttv);
  GetBoxPopulation("05_ST",        box1, box2, r1b1_st,    r1b2_st,    r2b1_st,    r2b2_st);
  GetBoxPopulation("00_Fakes",     box1, box2, r1b1_fakes, r1b2_fakes, r2b1_fakes, r2b2_fakes);

  float r1b1_bkg = r1b1_hz + r1b1_wz + r1b1_zz + r1b1_wg + r1b1_zj + r1b1_ttv + r1b1_st + r1b1_fakes;
  float r1b2_bkg = r1b2_hz + r1b2_wz + r1b2_zz + r1b2_wg + r1b2_zj + r1b2_ttv + r1b2_st + r1b2_fakes;
  float r2b1_bkg = r2b1_hz + r2b1_wz + r2b1_zz + r2b1_wg + r2b1_zj + r2b1_ttv + r2b1_st + r2b1_fakes;
  float r2b2_bkg = r2b2_hz + r2b2_wz + r2b2_zz + r2b2_wg + r2b2_zj + r2b2_ttv + r2b2_st + r2b2_fakes;


  // Tested     with MVA region 1
  // To be done with MVA region 2
  //----------------------------------------------------------------------------
  float r1b1_yield = r1b1_data - r1b1_bkg;
  float r1b2_yield = r1b2_data - r1b2_bkg;

  printf("\n MVA region 1 (cut-0.2 < mva < cut)\n");
  printf("------------------------------------\n");
  printf("            box1    box2\n");
  printf(" data-bkg = %.1f    %.1f\n", r1b1_yield, r1b2_yield);
  printf(" top      = %.1f    %.1f\n", r1b1_top,  r1b2_top);
  printf(" ww       = %.1f    %.1f\n", r1b1_ww,   r1b2_ww);
  
  float det = r1b1_ww * r1b2_top - r1b2_ww * r1b1_top; 

  float b11 =  r1b2_top / det;
  float b12 = -r1b1_top / det;
  float b21 = -r1b2_ww  / det;
  float b22 =  r1b1_ww  / det;
  
  float sf_ww  = b11 * r1b1_yield + b12 * r1b2_yield;
  float sf_top = b21 * r1b1_yield + b22 * r1b2_yield;

  float sf_ww_error   = sqrt(b11*b11*(r1b1_data+r1b1_bkg) + b12*b12*(r1b2_data+r1b2_bkg));
  float sf_top_error  = sqrt(b21*b21*(r1b1_data+r1b1_bkg) + b22*b22*(r1b2_data+r1b2_bkg));
  float sf_covariance =     (b11*b21*(r1b1_data+r1b1_bkg) + b12*b22*(r1b2_data+r1b2_bkg)) / (sf_ww_error * sf_top_error);

  printf("\n [GetScaleFactors] sf_top = %.2f +- %.2f and sf_ww = %.2f +- %.2f with cov = %.2f\n\n",
	 sf_top,
	 sf_top_error,
	 sf_ww,
	 sf_ww_error,
	 sf_covariance);
}
