#include <iostream>

#include <fstream>
#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"


// Constants
//------------------------------------------------------------------------------
const TString inputdir = "../minitrees/TTDM/";

enum {njmin, njmax, nbmin, nbmax};


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

void  GetScaleFactors (float*  box_ww,
		       float*  box_top);

void  SolveSystem     (TString region,
		       float   data1,
		       float   bkg1,
		       float   top1,
		       float   ww1,
		       float   data2,
		       float   bkg2,
		       float   top2,
		       float   ww2);


// Data members
//------------------------------------------------------------------------------
TString                  _signal;
float                    _cut;
ofstream                 _datacard;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// analysis
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void analysis(TString signal            = "ttDM0001scalar0500",
	      float   cut               = 0.80,
	      bool    doPrintYields     = true,
	      bool    doGetScaleFactors = false)
{
  if (!doPrintYields && !doGetScaleFactors) return;

  printf("\n [analysis] signal = %s and MVA cut = %.2f\n\n", signal.Data(), cut);

  _signal = signal;
  _cut    = cut;


  // Print yields
  //----------------------------------------------------------------------------
  if (doPrintYields) {

    gSystem->mkdir("datacards", kTRUE);
  
    PrintYields();
  }


  // Get scale factors
  //----------------------------------------------------------------------------
  if (doGetScaleFactors) {

    printf("\n [GetScaleFactors]\n\n");

    float ww_box1[] = {0,  0, 0, 0};
    float ww_box2[] = {1, -1, 0, 0};
    float ww_box3[] = {0, -1, 0, 0};
    
    float top_box1[] = {2, 3, 1, 2};
    float top_box2[] = {1, 4, 1, 2};
    
    GetScaleFactors(ww_box1, top_box1);
    GetScaleFactors(ww_box1, top_box2);
    GetScaleFactors(ww_box2, top_box1);
    GetScaleFactors(ww_box2, top_box2);
    GetScaleFactors(ww_box3, top_box1);
    GetScaleFactors(ww_box3, top_box2);
  }
}


//------------------------------------------------------------------------------
// GetYield
//------------------------------------------------------------------------------
float GetYield(TString sample)
{
  TFile* file = new TFile(inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float eventW;
  float njet;
  float mva;

  tree->SetBranchAddress("eventW", &eventW);
  tree->SetBranchAddress("njet",   &njet);
  tree->SetBranchAddress("mva_" + _signal, &mva);

  float yield = 0; 

  Long64_t nentries = tree->GetEntries();

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    if (njet > 1 && mva > _cut) yield += eventW;
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


  //  Create the datacard
  //----------------------------------------------------------------------------
  _datacard.open("datacards/" + _signal + ".txt");

  _datacard << "imax 1   number of channels\n";
  _datacard << "jmax 1   number of backgrounds\n";
  _datacard << "kmax 0   number of nuisance parameters\n";
  _datacard << "------------\n";
  _datacard << "\n";
  _datacard << "bin 1\n";
  _datacard << Form("observation %f\n", ndata);
  _datacard << "------------\n";
  _datacard << "\n";
  _datacard << "bin             1           1\n";
  _datacard << "process         DM          bkg\n";
  _datacard << "process         0           1\n";
  _datacard << Form("rate            %f    %f\n", nsignal, nexpected);
  _datacard << "------------\n";

  _datacard.close();
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
  float temporary_sf = (sample.Contains("TTTo2L2Nu")) ? (1./0.93) : 1.;

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

    //    bool reject_mvaregion1 = (mva < _cut-0.2 || mva > _cut);
    //    bool reject_mvaregion2 = (mva < _cut-0.4 || mva > _cut-0.2);

    bool reject_mvaregion1 = (mva < _cut/2. || mva > _cut);
    bool reject_mvaregion2 = (mva > _cut/2.);

    bool reject_box1 = ((box1[njmin] > -1 && njet  < box1[njmin]) ||
			(box1[njmax] > -1 && njet  > box1[njmax]) ||
			(box1[nbmin] > -1 && nbjet < box1[nbmin]) ||
			(box1[nbmax] > -1 && nbjet > box1[nbmax]));

    bool reject_box2 = ((box2[njmin] > -1 && njet  < box2[njmin]) ||
			(box2[njmax] > -1 && njet  > box2[njmax]) ||
			(box2[nbmin] > -1 && nbjet < box2[nbmin]) ||
			(box2[nbmax] > -1 && nbjet > box2[nbmax]));

    if (!reject_mvaregion1 && !reject_box1) mvaregion1_box1_yield += (eventW * temporary_sf);
    if (!reject_mvaregion1 && !reject_box2) mvaregion1_box2_yield += (eventW * temporary_sf);
    if (!reject_mvaregion2 && !reject_box1) mvaregion2_box1_yield += (eventW * temporary_sf);
    if (!reject_mvaregion2 && !reject_box2) mvaregion2_box2_yield += (eventW * temporary_sf);
  }
}


//------------------------------------------------------------------------------
// GetScaleFactors
//------------------------------------------------------------------------------
void GetScaleFactors(float* box_ww,
		     float* box_top)
{
  printf("\n-------------------------------------------------------------\n\n");
  printf("     box = { njmin, njmax, nbmin, nbmax, }\n");
  printf(" ww  box = {"); for (UInt_t i=0; i<4; i++) printf("%6.0f,", box_ww [i]); printf(" }\n");
  printf(" top box = {"); for (UInt_t i=0; i<4; i++) printf("%6.0f,", box_top[i]); printf(" }");

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

  GetBoxPopulation("01_Data",      box_ww, box_top, r1b1_data,  r1b2_data,  r2b1_data,  r2b2_data);
  GetBoxPopulation("04_TTTo2L2Nu", box_ww, box_top, r1b1_top,   r1b2_top,   r2b1_top,   r2b2_top);
  GetBoxPopulation("06_WW",        box_ww, box_top, r1b1_ww,    r1b2_ww,    r2b1_ww,    r2b2_ww);
  GetBoxPopulation("14_HZ",        box_ww, box_top, r1b1_hz,    r1b2_hz,    r2b1_hz,    r2b2_hz);
  GetBoxPopulation("02_WZTo3LNu",  box_ww, box_top, r1b1_wz,    r1b2_wz,    r2b1_wz,    r2b2_wz);
  GetBoxPopulation("03_ZZ",        box_ww, box_top, r1b1_zz,    r1b2_zz,    r2b1_zz,    r2b2_zz);
  GetBoxPopulation("11_Wg",        box_ww, box_top, r1b1_wg,    r1b2_wg,    r2b1_wg,    r2b2_wg);
  GetBoxPopulation("07_ZJets",     box_ww, box_top, r1b1_zj,    r1b2_zj,    r2b1_zj,    r2b2_zj);
  GetBoxPopulation("09_TTV",       box_ww, box_top, r1b1_ttv,   r1b2_ttv,   r2b1_ttv,   r2b2_ttv);
  GetBoxPopulation("05_ST",        box_ww, box_top, r1b1_st,    r1b2_st,    r2b1_st,    r2b2_st);
  GetBoxPopulation("00_Fakes",     box_ww, box_top, r1b1_fakes, r1b2_fakes, r2b1_fakes, r2b2_fakes);

  float r1b1_bkg = r1b1_hz + r1b1_wz + r1b1_zz + r1b1_wg + r1b1_zj + r1b1_ttv + r1b1_st + r1b1_fakes;
  float r1b2_bkg = r1b2_hz + r1b2_wz + r1b2_zz + r1b2_wg + r1b2_zj + r1b2_ttv + r1b2_st + r1b2_fakes;
  float r2b1_bkg = r2b1_hz + r2b1_wz + r2b1_zz + r2b1_wg + r2b1_zj + r2b1_ttv + r2b1_st + r2b1_fakes;
  float r2b2_bkg = r2b2_hz + r2b2_wz + r2b2_zz + r2b2_wg + r2b2_zj + r2b2_ttv + r2b2_st + r2b2_fakes;

  printf("\n");

  SolveSystem("[ cut/2 < MVA < cut   ]", r1b1_data, r1b1_bkg, r1b1_top, r1b1_ww, r1b2_data, r1b2_bkg, r1b2_top, r1b2_ww);
  SolveSystem("[         MVA < cut/2 ]", r2b1_data, r2b1_bkg, r2b1_top, r2b1_ww, r2b2_data, r2b2_bkg, r2b2_top, r2b2_ww);

  printf("\n");
}


//------------------------------------------------------------------------------
//
// SolveSystem
//
// sf_top*top1 + sf_ww*ww1 + bkg1 = data1
// sf_top*top2 + sf_ww*ww2 + bkg2 = data2
//
//------------------------------------------------------------------------------
void SolveSystem(TString region,
		 float   data1,
		 float   bkg1,
		 float   top1,
		 float   ww1,
		 float   data2,
		 float   bkg2,
		 float   top2,
		 float   ww2)
{
  float yield1 = data1 - bkg1;
  float yield2 = data2 - bkg2;

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


  // Print
  //----------------------------------------------------------------------------
  printf("\n");
  printf("        ww box \t top box\n");
  printf(" data = %6.1f \t %7.1f\n", data1, data2);
  printf(" bkg  = %6.1f \t %7.1f\n", bkg1,  bkg2);
  printf(" top  = %6.1f \t %7.1f\n", top1,  top2);
  printf(" ww   = %6.1f \t %7.1f\n", ww1,   ww2);
  printf("\n");
  printf(" [SolveSystem] %s sf_top = %5.2f +- %5.2f and sf_ww = %5.2f +- %5.2f with cov = %5.2f\n",
	 region.Data(),
	 sf_top,
	 sf_top_error,
	 sf_ww,
	 sf_ww_error,
	 sf_covariance);
}
