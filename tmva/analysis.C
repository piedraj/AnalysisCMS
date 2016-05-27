#include <iostream>

#include <fstream>
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"


// Constants
//------------------------------------------------------------------------------
const int     _verbosity = 0;
//const TString _inputdir  = "../minitrees/nominal/TTDM/";

enum {njmin, njmax, nbmin, nbmax};


// Functions
//------------------------------------------------------------------------------
void LHEfunction();
void LHEfunction2(TString sample);

void  GetPdfQcdSyst   (TString sample);

void  GetYield        (TString sample,
		       float&  yield,
		       float&  erryield,
		       float   cut = -999);

void  PrintYield      (TString sample,
		       float   yield);

void  PrintYields     (float   cut = -999);

void  GetBoxPopulation(TString sample,
		       float*  box1,
		       float*  box2,
		       float&  region1_box1_yield,
		       float&  region1_box2_yield,
		       float&  region2_box1_yield,
		       float&  region2_box2_yield);

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
TString			 _systematic;
ofstream		 _yields;
TString                  _inputdir;
float			 _thunc[6];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// analysis
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void analysis(TString signal            = "ttDM0001scalar0500",
	      float   cut               = 0.8,
	      TString systematic        = "nominal",
	      bool    doPrintYields     = true,
	      bool    doGetScaleFactors = false,
	      bool    doGetPdfQcdSyst   = false)
{
  if (!doPrintYields && !doGetScaleFactors && !doGetPdfQcdSyst) return;

  printf("\n [analysis] signal = %s and MVA cut = %.2f\n\n", signal.Data(), cut);

  _signal = signal;
  _cut    = cut;
  _systematic = systematic;  if(_systematic == "stat") _systematic = "nominal";

  _inputdir  = "/gpfs/csic_projects/cms/jgarciaf/CMSSW_7_6_3/src/AnalysisCMS_old/minitrees/" + _systematic + "/TTDM/";

  // Print yields
  //----------------------------------------------------------------------------
  if (doPrintYields) {

    //gSystem->mkdir("datacards", kTRUE);
    gSystem->mkdir("yields"   , kTRUE);

    //    PrintYields(_cut);
    
    int   nstep = 50;
    float step  = 1. / nstep;

    //for (int i=0; i<nstep; i++) PrintYields(step * i);

    PrintYields(_cut);

    LHEfunction();

  }

  // Get scale factors
  //----------------------------------------------------------------------------
  if (doGetScaleFactors) {

    if (_verbosity > 0) printf("\n [GetScaleFactors]\n\n");

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


  // Get PDF and QCD systematic uncertainties
  //----------------------------------------------------------------------------
  if (doGetPdfQcdSyst)
    {
      if (_verbosity > 0) printf("\n [GetPdfQcdSyst]\n\n");

      float thunc[10][6]; 

      //GetPdfQcdSyst(_signal);       thunc[9][0] = _thunc[0];  thunc[9][1] = _thunc[1];  thunc[9][2] = _thunc[2];  thunc[9][3] = _thunc[3];  thunc[9][4] = _thunc[4];  thunc[9][5] = _thunc[5];
      GetPdfQcdSyst("02_WZTo3LNu"); thunc[0][0] = _thunc[0];  thunc[0][1] = _thunc[1];  thunc[0][2] = _thunc[2];  thunc[0][3] = _thunc[3];  thunc[0][4] = _thunc[4];  thunc[0][5] = _thunc[5];
      GetPdfQcdSyst("03_ZZ");	    thunc[1][0] = _thunc[0];  thunc[1][1] = _thunc[1];  thunc[1][2] = _thunc[2];  thunc[1][3] = _thunc[3];  thunc[1][4] = _thunc[4];  thunc[1][5] = _thunc[5];
      GetPdfQcdSyst("04_TTTo2L2Nu");thunc[7][0] = _thunc[0];  thunc[7][1] = _thunc[1];  thunc[7][2] = _thunc[2];  thunc[7][3] = _thunc[3];  thunc[7][4] = _thunc[4];  thunc[7][5] = _thunc[5];
      GetPdfQcdSyst("05_ST");       thunc[2][0] = _thunc[0];  thunc[2][1] = _thunc[1];  thunc[2][2] = _thunc[2];  thunc[2][3] = _thunc[3];  thunc[2][4] = _thunc[4];  thunc[2][5] = _thunc[5];
      GetPdfQcdSyst("06_WW");       thunc[8][0] = _thunc[0];  thunc[8][1] = _thunc[1];  thunc[8][2] = _thunc[2];  thunc[8][3] = _thunc[3];  thunc[8][4] = _thunc[4];  thunc[8][5] = _thunc[5];   
      GetPdfQcdSyst("07_ZJets");    thunc[3][0] = _thunc[0];  thunc[3][1] = _thunc[1];  thunc[3][2] = _thunc[2];  thunc[3][3] = _thunc[3];  thunc[3][4] = _thunc[4];  thunc[3][5] = _thunc[5];
      GetPdfQcdSyst("09_TTV");      thunc[4][0] = _thunc[0];  thunc[4][1] = _thunc[1];  thunc[4][2] = _thunc[2];  thunc[4][3] = _thunc[3];  thunc[4][4] = _thunc[4];  thunc[4][5] = _thunc[5];
    //GetPdfQcdSyst("10_HWW");
      GetPdfQcdSyst("11_Wg");       thunc[5][0] = _thunc[0];  thunc[5][1] = _thunc[1];  thunc[5][2] = _thunc[2];  thunc[5][3] = _thunc[3];  thunc[5][4] = _thunc[4];  thunc[5][5] = _thunc[5];
    //GetPdfQcdSyst("12_Zg");
    //GetPdfQcdSyst("13_VVV");
      GetPdfQcdSyst("14_HZ");       thunc[6][0] = _thunc[0];  thunc[6][1] = _thunc[1];  thunc[6][2] = _thunc[2];  thunc[6][3] = _thunc[3];  thunc[6][4] = _thunc[4];  thunc[6][5] = _thunc[5];

      if (_verbosity > 0) printf("\n");
    

  //printf("\n [analysis] I hope you are happy with the results!\n\n");

  ofstream xsQCDup; 
  ofstream acQCDup;  
  ofstream xsQCDdo; 
  ofstream acQCDdo; 
  ofstream xsPDF  ; 
  ofstream acPDF  ; 

  xsQCDup.open(Form("yields/%s_%s.dat", _signal.Data(), "xsQCDup"));
  acQCDup.open(Form("yields/%s_%s.dat", _signal.Data(), "acQCDup"));
  xsQCDdo.open(Form("yields/%s_%s.dat", _signal.Data(), "xsQCDdo"));
  acQCDdo.open(Form("yields/%s_%s.dat", _signal.Data(), "acQCDdo"));
  xsPDF  .open(Form("yields/%s_%s.dat", _signal.Data(), "xsPDF"  ));
  acPDF  .open(Form("yields/%s_%s.dat", _signal.Data(), "acPDF"  ));

  xsQCDup << Form( "%f \n", 0.0        );    acQCDup << Form( "%f \n", 0.0        );    xsQCDdo << Form( "%f \n", 0.0        );    acQCDdo << Form( "%f \n", 0.0        );
  xsQCDup << Form( "%f \n", 0.0        );    acQCDup << Form( "%f \n", 0.0        );    xsQCDdo << Form( "%f \n", 0.0        );    acQCDdo << Form( "%f \n", 0.0        );
  xsQCDup << Form( "%f \n", 0.0        );    acQCDup << Form( "%f \n", 0.0        );    xsQCDdo << Form( "%f \n", 0.0        );    acQCDdo << Form( "%f \n", 0.0        );
  xsQCDup << Form( "%f \n", thunc[0][0]);    acQCDup << Form( "%f \n", thunc[0][1]);    xsQCDdo << Form( "%f \n", thunc[0][2]);    acQCDdo << Form( "%f \n", thunc[0][3]);
  xsQCDup << Form( "%f \n", thunc[1][0]);    acQCDup << Form( "%f \n", thunc[1][1]);    xsQCDdo << Form( "%f \n", thunc[1][2]);    acQCDdo << Form( "%f \n", thunc[1][3]);
  xsQCDup << Form( "%f \n", thunc[7][0]);    acQCDup << Form( "%f \n", thunc[7][1]);    xsQCDdo << Form( "%f \n", thunc[7][2]);    acQCDdo << Form( "%f \n", thunc[7][3]);
  xsQCDup << Form( "%f \n", thunc[2][0]);    acQCDup << Form( "%f \n", thunc[2][1]);    xsQCDdo << Form( "%f \n", thunc[2][2]);    acQCDdo << Form( "%f \n", thunc[2][3]);
  xsQCDup << Form( "%f \n", thunc[8][0]);    acQCDup << Form( "%f \n", thunc[8][1]);    xsQCDdo << Form( "%f \n", thunc[8][2]);    acQCDdo << Form( "%f \n", thunc[8][3]);
  xsQCDup << Form( "%f \n", thunc[3][0]);    acQCDup << Form( "%f \n", thunc[3][1]);    xsQCDdo << Form( "%f \n", thunc[3][2]);    acQCDdo << Form( "%f \n", thunc[3][3]);
  xsQCDup << Form( "%f \n", thunc[4][0]);    acQCDup << Form( "%f \n", thunc[4][1]);    xsQCDdo << Form( "%f \n", thunc[4][2]);    acQCDdo << Form( "%f \n", thunc[4][3]);
  xsQCDup << Form( "%f \n", thunc[5][0]);    acQCDup << Form( "%f \n", thunc[5][1]);    xsQCDdo << Form( "%f \n", thunc[5][2]);    acQCDdo << Form( "%f \n", thunc[5][3]);
  xsQCDup << Form( "%f \n", thunc[6][0]);    acQCDup << Form( "%f \n", thunc[6][1]);    xsQCDdo << Form( "%f \n", thunc[6][2]);    acQCDdo << Form( "%f \n", thunc[6][3]);

  xsPDF   << Form( "%f \n", 0.0        );        acPDF   << Form( "%f \n", 0.0        );
  xsPDF   << Form( "%f \n", 0.0        );        acPDF   << Form( "%f \n", 0.0        );
  xsPDF   << Form( "%f \n", 0.0        );        acPDF   << Form( "%f \n", 0.0        );
  xsPDF   << Form( "%f \n", thunc[0][4]);        acPDF   << Form( "%f \n", thunc[0][5]);
  xsPDF   << Form( "%f \n", thunc[1][4]);        acPDF   << Form( "%f \n", thunc[1][5]);
  xsPDF   << Form( "%f \n", thunc[7][4]);        acPDF   << Form( "%f \n", thunc[7][5]);
  xsPDF   << Form( "%f \n", 0.0        );        acPDF   << Form( "%f \n", 0.0        );
  xsPDF   << Form( "%f \n", thunc[8][4]);        acPDF   << Form( "%f \n", thunc[8][5]);
  xsPDF   << Form( "%f \n", thunc[3][4]);        acPDF   << Form( "%f \n", thunc[3][5]);
  xsPDF   << Form( "%f \n", thunc[4][4]);        acPDF   << Form( "%f \n", thunc[4][5]);
  xsPDF   << Form( "%f \n", thunc[5][4]);        acPDF   << Form( "%f \n", thunc[5][5]);
  xsPDF   << Form( "%f \n", thunc[6][4]);        acPDF   << Form( "%f \n", thunc[6][5]);

  xsQCDup.close();
  acQCDup.close();
  xsQCDdo.close();
  acQCDdo.close();
  xsPDF  .close();
  acPDF  .close();

   }

}


//------------------------------------------------------------------------------
// GetPdfQcdSyst
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample)
{

  _thunc[0] = 0; 
  _thunc[1] = 0;
  _thunc[2] = 0;
  _thunc[3] = 0;
  _thunc[4] = 0;  
  _thunc[5] = 0;


  TFile* file = new TFile(_inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float          mva;
  float          njet;
  vector<float> *LHEweight = 0;

  tree->SetBranchAddress("mva_" + _signal, &mva);
  tree->SetBranchAddress("njet",           &njet);
  tree->SetBranchAddress("LHEweight",      &LHEweight );

  TH1D* h_pdfsum_gen = (TH1D*)file->Get("h_pdfsum");
  TH1D* h_qcdsum_gen = (TH1D*)file->Get("h_qcdsum");

  TH1D* h_pdfsum_rec = (TH1D*)h_pdfsum_gen->Clone("h_pdfsum_rec");
  TH1D* h_qcdsum_rec = (TH1D*)h_pdfsum_gen->Clone("h_qcdsum_rec");

  h_pdfsum_rec->Reset();
  h_qcdsum_rec->Reset();

  int nbinpdf = h_pdfsum_gen->GetNbinsX();
  int nbinqcd = h_qcdsum_gen->GetNbinsX();


  // Loop over the tree
  //----------------------------------------------------------------------------
  Long64_t nentries = tree->GetEntries();

  for (Long64_t i=0; i<nentries; i++) {

    tree->GetEntry(i);

    if (mva < _cut) continue;

    if (njet < 1) continue;

    for (int a=0; a<nbinpdf; a++) h_pdfsum_rec->Fill(a, LHEweight->at(a+9));
    for (int a=0; a<nbinqcd; a++) h_qcdsum_rec->Fill(a, LHEweight->at(a));
  }


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  float qcdratio_gen_up   = h_qcdsum_gen->GetBinContent(9) / h_qcdsum_gen->GetBinContent(1);
  float qcdratio_gen_down = h_qcdsum_gen->GetBinContent(5) / h_qcdsum_gen->GetBinContent(1);

  float qcdratio_rec_up   = h_qcdsum_rec->GetBinContent(9) / h_qcdsum_rec->GetBinContent(1);
  float qcdratio_rec_down = h_qcdsum_rec->GetBinContent(5) / h_qcdsum_rec->GetBinContent(1);


  // Produce the PDF uncertainties
  //----------------------------------------------------------------------------
  float pdf_gen_mean = h_pdfsum_gen->Integral() / nbinpdf;
  float pdf_rec_mean = h_pdfsum_rec->Integral() / nbinpdf;

  float pdf_gen_mean_sq = 0;
  float pdf_rec_mean_sq = 0;

  for (int a=1; a<=nbinpdf; a++)
    {
      pdf_gen_mean_sq += (pow(h_pdfsum_gen->GetBinContent(a), 2) / nbinpdf);
      pdf_rec_mean_sq += (pow(h_pdfsum_rec->GetBinContent(a), 2) / nbinpdf);
    }

  float pdf_gen_sd = sqrt(pdf_gen_mean_sq - pow(pdf_gen_mean, 2)); 
  float pdf_rec_sd = sqrt(pdf_rec_mean_sq - pow(pdf_rec_mean, 2)); 

  float pdf_gen_ratio = 1 + (pdf_gen_sd / pdf_gen_mean);
  float pdf_rec_ratio = 1 + (pdf_rec_sd / pdf_rec_mean);


  // Print the uncertainties
  //----------------------------------------------------------------------------
  //printf("\n %s\n", sample.Data());
  //printf("---------------------------------------\n");
  //printf(" QCD up   -- xs = %5.3f -- acc = %5.3f\n", qcdratio_gen_up,   qcdratio_rec_up   / qcdratio_gen_up);
  //printf(" QCD down -- xs = %5.3f -- acc = %5.3f\n", qcdratio_gen_down, qcdratio_rec_down / qcdratio_gen_down);
  //printf(" PDF      -- xs = %5.3f -- acc = %5.3f\n", pdf_gen_ratio,     pdf_rec_ratio     / pdf_gen_ratio);

  _thunc[0] = qcdratio_gen_up                    ; 
  _thunc[1] = qcdratio_rec_up/qcdratio_gen_up    ;
  _thunc[2] = qcdratio_gen_down                  ;
  _thunc[3] = qcdratio_rec_down/qcdratio_gen_down;
  _thunc[4] = pdf_gen_ratio                      ;  
  _thunc[5] = pdf_rec_ratio/pdf_gen_ratio        ; 

}  



//---------------
// LHEfunction
//---------------
void LHEfunction(){

	LHEfunction2(_signal);
	LHEfunction2("00_Fakes");
	LHEfunction2("02_WZTo3LNu");
	LHEfunction2("03_ZZ");
	LHEfunction2("04_TTTo2L2Nu");
	LHEfunction2("05_ST");
	LHEfunction2("06_WW");
	LHEfunction2("07_ZJets");
	LHEfunction2("09_TTV");
	LHEfunction2("11_Wg");
	LHEfunction2("14_HZ");
}

void LHEfunction2(TString sample){

	cout << " --- vine a campala... " << sample << endl; 

	TH1F* list_vectors_weights = new TH1F("list_vectors_weights", "", 200, 0, 200);

  	TFile* file = new TFile(_inputdir + sample + ".root", "read");

  	TTree* tree = (TTree*)file->Get("latino");


	float          mva;
  	float          njet;
  	vector<float> *LHEweight = 0;

  	tree->SetBranchAddress("mva_" + _signal, &mva);
  	tree->SetBranchAddress("njet",           &njet);
  	tree->SetBranchAddress("LHEweight", &LHEweight );

	if (!LHEweight) return;

  	Long64_t nentries = tree->GetEntries();

  	for (Long64_t i=0; i<nentries; i++) {

    		tree->GetEntry(i);

    		if (mva < _cut) continue;

	    	if (njet < 1) continue;

	  	for (int iWeight=0; iWeight<list_vectors_weights->GetNbinsX(); iWeight++){

	  	      float ratio = LHEweight->at(iWeight) / LHEweight->at(0);

	      	      list_vectors_weights->Fill(iWeight+0.5, ratio);
	    	}

	}

}

//------------------------------------------------------------------------------
// GetYield
//------------------------------------------------------------------------------
void GetYield(TString sample, float& yield, float& erryield, float cut)
{
  if (cut < 0) cut = _cut;

  TFile* file = new TFile(_inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float eventW;
  float njet;
  float nbjet30loose;
  float mva;

  tree->SetBranchAddress("eventW", &eventW);
  tree->SetBranchAddress("njet",   &njet);
  tree->SetBranchAddress("nbjet30loose",   &nbjet30loose);
  tree->SetBranchAddress("mva_" + _signal, &mva);

  Long64_t nentries = tree->GetEntries();

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    if (njet > 1  && mva > cut) { yield += eventW;  erryield += (eventW * eventW); }

  }

  erryield = sqrt(erryield); 	

  if (_verbosity > 0) PrintYield(sample, yield);

  delete tree;

  file->Close();

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
void PrintYields(float cut)
{
  if (cut < 0) cut = _cut;

  if (_verbosity > 0) printf("\n");

  printf(" [PrintYields] MVA cut = %.2f\n", cut);

  if (_verbosity > 0) printf("\n");

  float nsignal;      float errsignal;                                   GetYield(_signal, nsignal, errsignal, cut);
   
  float ndata = 0.0;  float errdata = 0.0;  if(_systematic == "nominal") GetYield("01_Data",  ndata, errdata, cut);
  
  //float nexpected = 0;
  
  //nexpected += GetYield("14_HZ",        cut);  
  //nexpected += GetYield("06_WW",        cut);
  //nexpected += GetYield("02_WZTo3LNu",  cut);
  //nexpected += GetYield("03_ZZ",        cut);
  //nexpected += GetYield("11_Wg",        cut);
  //nexpected += GetYield("07_ZJets",     cut);
  //nexpected += GetYield("09_TTV",       cut);
  //nexpected += GetYield("04_TTTo2L2Nu", cut);
  //nexpected += GetYield("05_ST",        cut);
  //nexpected += GetYield("00_Fakes",     cut);

  float yield00 = 0.0; float err00 = 0.0;  if(_systematic == "nominal") GetYield("00_Fakes"    , yield00, err00, cut );
  float yield02;       float err02;                                     GetYield("02_WZTo3LNu" , yield02, err02, cut );
  float yield03;       float err03;                                     GetYield("03_ZZ"       , yield03, err03, cut );
  float yield04;       float err04;                                     GetYield("04_TTTo2L2Nu", yield04, err04, cut );
  float yield05;       float err05;                                     GetYield("05_ST"       , yield05, err05, cut );
  float yield06;       float err06;                                     GetYield("06_WW"       , yield06, err06, cut );
  float yield07;       float err07;                                     GetYield("07_ZJets"    , yield07, err07, cut );
  float yield09;       float err09;                                     GetYield("09_TTV"      , yield09, err09, cut );
  float yield11;       float err11;                                     GetYield("11_Wg"       , yield11, err11, cut );
  float yield14;       float err14;                                     GetYield("14_HZ"       , yield14, err14, cut );

  if (_verbosity > 0)
    {
      printf("--------------------------------\n");

      PrintYield("signal",   nsignal);
      PrintYield("data",     ndata);
      //PrintYield("expected", nexpected);

      printf("\n");
    }



  _yields.open(Form("yields/%s_%s.dat", _signal.Data(), _systematic.Data() ));

  if( _systematic != "stat" ){
  
	  _yields << Form( "%f \n", ndata  );
	  _yields << Form( "%f \n", nsignal);
	  _yields << Form( "%f \n", yield00);
	  _yields << Form( "%f \n", yield02);
	  _yields << Form( "%f \n", yield03);
	  _yields << Form( "%f \n", yield04);
	  _yields << Form( "%f \n", yield05);
	  _yields << Form( "%f \n", yield06);
	  _yields << Form( "%f \n", yield07);
	  _yields << Form( "%f \n", yield09);
	  _yields << Form( "%f \n", yield11);
	  _yields << Form( "%f \n", yield14);

  }

  else{

	  _yields << Form( "%f \n", errdata  );
	  _yields << Form( "%f \n", errsignal);
	  _yields << Form( "%f \n", err00);
	  _yields << Form( "%f \n", err02);
	  _yields << Form( "%f \n", err03);
	  _yields << Form( "%f \n", err04);
	  _yields << Form( "%f \n", err05);
	  _yields << Form( "%f \n", err06);
	  _yields << Form( "%f \n", err07);
	  _yields << Form( "%f \n", err09);
	  _yields << Form( "%f \n", err11);
	  _yields << Form( "%f \n", err14);
  }

  _yields.close();

}


//------------------------------------------------------------------------------
// GetBoxPopulation
//------------------------------------------------------------------------------
void GetBoxPopulation(TString sample,
		      float*  box1,
		      float*  box2,
		      float&  region1_box1_yield,
		      float&  region1_box2_yield,
		      float&  region2_box1_yield,
		      float&  region2_box2_yield)
{
  TFile* file = new TFile(_inputdir + sample + ".root", "read");

  TTree* tree = (TTree*)file->Get("latino");

  float eventW;
  float mva;
  float nbjet;
  float njet;

  tree->SetBranchAddress("eventW",         &eventW);
  tree->SetBranchAddress("mva_" + _signal, &mva);
  tree->SetBranchAddress("nbjet20loose",   &nbjet);
  tree->SetBranchAddress("njet",           &njet);

  region1_box1_yield = 0;
  region1_box2_yield = 0;
  region2_box1_yield = 0;
  region2_box2_yield = 0;

  Long64_t nentries = tree->GetEntries();

  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    tree->GetEntry(ievt);

    bool reject_region1 = (mva < _cut-0.2 || mva > _cut);
    bool reject_region2 = (mva < _cut-0.4 || mva > _cut-0.2);

    bool reject_box1 = ((box1[njmin] > -1 && njet  < box1[njmin]) ||
			(box1[njmax] > -1 && njet  > box1[njmax]) ||
			(box1[nbmin] > -1 && nbjet < box1[nbmin]) ||
			(box1[nbmax] > -1 && nbjet > box1[nbmax]));

    bool reject_box2 = ((box2[njmin] > -1 && njet  < box2[njmin]) ||
			(box2[njmax] > -1 && njet  > box2[njmax]) ||
			(box2[nbmin] > -1 && nbjet < box2[nbmin]) ||
			(box2[nbmax] > -1 && nbjet > box2[nbmax]));

    if (sample != "01_Data"){

	if (!reject_region1) region1_box1_yield += eventW;
        if (!reject_region2) region2_box1_yield += eventW;

    }

    else{

    if (!reject_region1 && !reject_box1) region1_box1_yield += eventW;
    if (!reject_region1 && !reject_box2) region1_box2_yield += eventW;
    if (!reject_region2 && !reject_box1) region2_box1_yield += eventW;
    if (!reject_region2 && !reject_box2) region2_box2_yield += eventW;

    }

  }

  if(sample != "01_Data") cout << sample << " -- " << region1_box1_yield << " -- " << region2_box1_yield << endl;
}


//------------------------------------------------------------------------------
// GetScaleFactors
//------------------------------------------------------------------------------
void GetScaleFactors(float* box_ww,
		     float* box_top)
{

  if (_verbosity > 0)
    {
      printf("\n-------------------------------------------------------------\n\n");
      printf("     box = { njmin, njmax, nbmin, nbmax, }\n");
      printf(" ww  box = {"); for (UInt_t i=0; i<4; i++) printf("%6.0f,", box_ww [i]); printf(" }\n");
      printf(" top box = {"); for (UInt_t i=0; i<4; i++) printf("%6.0f,", box_top[i]); printf(" }");
    }

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
  GetBoxPopulation(_signal,      box_ww, box_top, r1b1_fakes, r1b2_fakes, r2b1_fakes, r2b2_fakes);


  float r1b1_bkg = r1b1_hz + r1b1_wz + r1b1_zz + r1b1_wg + r1b1_zj + r1b1_ttv + r1b1_st + r1b1_fakes;
  float r1b2_bkg = r1b2_hz + r1b2_wz + r1b2_zz + r1b2_wg + r1b2_zj + r1b2_ttv + r1b2_st + r1b2_fakes;
  float r2b1_bkg = r2b1_hz + r2b1_wz + r2b1_zz + r2b1_wg + r2b1_zj + r2b1_ttv + r2b1_st + r2b1_fakes;
  float r2b2_bkg = r2b2_hz + r2b2_wz + r2b2_zz + r2b2_wg + r2b2_zj + r2b2_ttv + r2b2_st + r2b2_fakes;

  printf("\n");

  SolveSystem("[ cut-0.2 < MVA < cut     ]", r1b1_data, r1b1_bkg, r1b1_top, r1b1_ww, r1b2_data, r1b2_bkg, r1b2_top, r1b2_ww);
  SolveSystem("[ cut-0.4 < MVA < cut-0.2 ]", r2b1_data, r2b1_bkg, r2b1_top, r2b1_ww, r2b2_data, r2b2_bkg, r2b2_top, r2b2_ww);

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
  if (_verbosity > 0)
    {
      printf("\n");
      printf("        ww box \t top box\n");
      printf(" data = %6.1f \t %7.1f\n", data1, data2);
      printf(" bkg  = %6.1f \t %7.1f\n", bkg1,  bkg2);
      printf(" top  = %6.1f \t %7.1f\n", top1,  top2);
      printf(" ww   = %6.1f \t %7.1f\n", ww1,   ww2);
      printf("\n");
    }

  printf(" [SolveSystem] %s sf_top = %5.2f +- %5.2f and sf_ww = %5.2f +- %5.2f with cov = %5.2f\n",
	 region.Data(),
	 sf_top,
	 sf_top_error,
	 sf_ww,
	 sf_ww_error,
	 sf_covariance);
}
