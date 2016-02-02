#include <vector>
#include <fstream>
#include <iostream>
#include "include/Constants.h"


//=================================================
//Definition of the variables
//=================================================

TString 	 _datapath = "rootfiles/";
TString		 _era = "50ns";
TString 	 _directory;
TString		 _sample;
TString	       	_analysis;
float	 	 _luminosity;
float		eficiencia;
float		N;
float		nsenal;
float		error_stat;
double		 crossSection_b = 0;
double		crossSection_pb=0;

Int_t		 _cut;

const UInt_t nprocess=10;

enum {
Data,
WZ,
ZZ,
Top,
SingleTop,
WW,
ZJets,
WJets,
TTW,
TTZ
};

TFile* input[nprocess];
TString sprocess[nprocess];

vector<UInt_t> vprocess; 

//================================================
//Definition of the functions
//================================================

Int_t	SetParameters		(Int_t		cut,
				 Int_t 	 	jetbin);

Int_t	ReadInputFiles		();

TString	GetName			(TString	prefix,
		UInt_t		channel);


//===================================================================================
// Determination of ndata and nbackground
//===================================================================================
void CrossSection(int cut = WZ00_Exactly3Leptons)
{
	int kk = SetParameters(cut,3);

	if (kk < 0) return;

	Double_t ndata;
	Double_t nbackground = 0;

	TH1D* h[nprocess];

	for(UInt_t i=0;i<vprocess.size();i++){

		UInt_t j = vprocess.at(i);

		h[j] = (TH1D*)input[j]->Get(Form("%s/h_counterLum_lll", scut[cut].Data()));

		if(j==Data){
			ndata = h[j]->Integral();
			error_stat=1/sqrt(ndata);
		}

		else if(j==WZ){
			nsenal=h[j]->Integral();
		}

		else {
			nbackground += h[j]->Integral();
		}
	}

	_luminosity = (_era.EqualTo("50ns")) ? lumi50ns_fb : lumi25ns_fb;

	//===================================================================================
	// Determination of the efficiency
	// ===================================================================================

	N = (_era.EqualTo("50ns")) ? 996920 : 1; 	// Hay que cambiar el valor 1! 
	
			
	eficiencia=nsenal/N;


	//====================================================================================
	//Determination of the cross section
	//====================================================================================


	crossSection_pb = (ndata-nbackground)/(_luminosity*eficiencia);
	crossSection_b = crossSection_pb * pow(10,-12);

	error_stat=error_stat*crossSection_pb;
float		N;

	printf(" The value of the cross section is %f pb (%f b) \n ",crossSection_pb,crossSection_b);
	printf("The statistical error is %f pb\n",error_stat);
}

//================================================
//Function SetParameters
//================================================

Int_t SetParameters(Int_t cut, Int_t jetbin)
{

	_cut = cut;
	_luminosity = (_era.EqualTo("50ns")) ? lumi50ns_fb : lumi25ns_fb;

	if(cut<0){
		printf("There is an error with the SetParameters function");
		return -1;
	}

	sprocess[Data]  	= "01_Data";
	sprocess[WZ]		= "02_WZ";
	sprocess[ZZ]		= "03_ZZ";
	sprocess[Top]		= "04_Top";




	sprocess[SingleTop]	= "05_SingleTop";
	sprocess[WW]		= "06_WW";
	sprocess[ZJets]		= "07_ZJets";
	sprocess[WJets]		= "08_WJets";
	sprocess[TTW]		= "09_TTW";
	sprocess[TTZ]		= "10_TTZ";

	vprocess.clear();
	vprocess.push_back(Data);
	vprocess.push_back(WZ);
	vprocess.push_back(ZZ);
	vprocess.push_back(Top);
	vprocess.push_back(SingleTop);
	vprocess.push_back(WW);
	vprocess.push_back(ZJets);
	vprocess.push_back(WJets);


	if(ReadInputFiles()<0){
		printf("Issue when reading the input file");
		return -1;
	}

	return 1;
}


//================================================
//Function ReadInputFiles
//================================================


Int_t ReadInputFiles()
{
	for(UInt_t i=0; i<vprocess.size(); i++){

		UInt_t j = vprocess.at(i);

		TString fname = _datapath +"/" + _era + "/" + sprocess[j] + ".root";

		input[j] = new TFile(fname);		
	}
	return 0;
}




//================================================
//Function GetName
//================================================

TString GetName(TString prefix, UInt_t channel)
{
	TString hname = _directory + prefix + "_" + schannel[channel];
	return hname;
}
