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

void  SolveSystem     (float   data1,
		       float   bkg1,
		       float   top1,
		       float   ww1,
		       float   data2,
		       float   bkg2,
		       float   top2,
		       float   ww2);


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
	      float   cut    = 0.44)
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
  printf("\n [GetScaleFactors]\n\n");

  float box1[] = {1, -1, 0, 0};
  float box2[] = {1,  5, 1, 3};

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


  printf("\n MVA region 1 (cut-0.2 < mva < cut)\n\n");
  SolveSystem(r1b1_data, r1b1_bkg, r1b1_top, r1b1_ww, r1b2_data, r1b2_bkg, r1b2_top, r1b2_ww);

  printf("\n MVA region 2 (cut-0.4 < mva < cut-0.2)\n\n");
  SolveSystem(r2b1_data, r2b1_bkg, r2b1_top, r2b1_ww, r2b2_data, r2b2_bkg, r2b2_top, r2b2_ww);
}


//------------------------------------------------------------------------------
// SolveSystem
//
// sf_top*top1 + sf_ww*ww1 + bkg1 = data1
// sf_top*top2 + sf_ww*ww2 + bkg2 = data2
//
//------------------------------------------------------------------------------
void SolveSystem(float data1,
		 float bkg1,
		 float top1,
		 float ww1,
		 float data2,
		 float bkg2,
		 float top2,
		 float ww2)
{
  float yield1 = data1 - bkg1;
  float yield2 = data2 - bkg2;

  printf("            box1 \t box2\n");
  printf(" data-bkg = %.1f \t %.1f\n", yield1, yield2);
  printf(" top      = %.1f \t %.1f\n", top1,   top2);
  printf(" ww       = %.1f \t %.1f\n", ww1,    ww2);
  
  float det = ww1 * top2 - ww2 * top1; 

  float b11 =  top2 / det;
  float b12 = -top1 / det;
  float b21 = -ww2  / det;
  float b22 =  ww1  / det;
  
  float sf_ww  = b11 * yield1 + b12 * yield2;
  float sf_top = b21 * yield1 + b22 * yield2;

  float sf_ww_error   = sqrt(b11*b11*(data1+bkg1) + b12*b12*(data2+bkg2));
  float sf_top_error  = sqrt(b21*b21*(data1+bkg1) + b22*b22*(data2+bkg2));
  float sf_covariance =     (b11*b21*(data1+bkg1) + b12*b22*(data2+bkg2)) / (sf_ww_error * sf_top_error);

  printf("\n [SolveSystem] sf_top = %.2f +- %.2f and sf_ww = %.2f +- %.2f with cov = %.2f\n\n",
	 sf_top,
	 sf_top_error,
	 sf_ww,
	 sf_ww_error,
	 sf_covariance);
}
