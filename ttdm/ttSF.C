// root -l -b -q CreateHistograms

#include "../include/Constants.h"

enum{ fakes, data, WZ, VZ, TT, ST, WW, DY, TTV, HWW, Wg, Zg, VVV, HZ, WgStar, nprocess }; 

const int nband = 5; 

TString processID[nprocess]       ; 
TH1F*     myhisto[nprocess]       ;
float       yield[nprocess][nband];   // 5-bands  
float          SF          [nband];
float         eSF          [nband];

float threshold = 0.60; 
float      step = 0.05;

const TString  inputdir = "week-1";  // where the minitrees are stored

const TCut mycut = "eventW*(channel==5)";                 

void GetHistogram( int process );


void ttSF(){

	processID[fakes ] = "00_Fakes"     ; 
	processID[data  ] = "01_Data"      ; 
	processID[WZ    ] = "02_WZTo3LNu"  ; 
	processID[VZ    ] = "03_VZ"        ; 
	processID[TT    ] = "04_TTTo2L2Nu" ; 
	processID[ST    ] = "05_ST"        ; 
	processID[WW    ] = "06_WW"        ; 
	processID[DY    ] = "07_ZJets"     ; 
	processID[TTV   ] = "09_TTV"       ; 
	processID[HWW   ] = "10_HWW"       ; 
	processID[Wg    ] = "11_Wg"        ; 
	processID[Zg    ] = "12_Zg"        ; 
	processID[VVV   ] = "13_VVV"       ; 
	processID[HZ    ] = "14_HZ"        ; 
	processID[WgStar] = "15_WgStar"    ; 


	for( int i = 0; i < nprocess; i++ ){

		cout << "\n\n" << processID[i] << "\n" << endl; 

		GetHistogram( i ); 

		float myyield = ( i > data ) ?  lumi_fb_2016*myhisto[i]->Integral() : myhisto[i]->Integral(); 

		if( i > data ) myhisto[i]->Scale(lumi_fb_2016); 

		for( int j = 0; j < nband; j++ ){

			yield[i][j] = myhisto[i]->Integral( myhisto[i]->FindBin(threshold - (nband-j)*step), myhisto[i]->FindBin(threshold - (nband-j-1)*step));

		
		}

	}


   	TH1F* ttSF = new TH1F( "ttSF","tt SF", 20, 0., 1.0 );
 
	for( int j = 0; j < nband; j++ ){

		float bkg = yield[fakes][j] + yield[WZ][j] + yield[VZ][j] + yield[ST][j] + 
                            yield[WW][j] + yield[DY][j] + yield[TTV][j] + yield[HWW][j] +
                            yield[Wg][j] + yield[Zg][j] + yield[VVV][j] + yield[HZ][j] + 
                            yield[WgStar][j];
		
		 SF[j] = ( yield[data][j] - bkg )/yield[TT][j];
		eSF[j] = sqrt( yield[data][j] )  /yield[TT][j]; 

		//cout << SF[j] << " +/- " << eSF[j] << endl;

		ttSF -> SetBinContent( ttSF->FindBin(threshold - (nband-j)*step+step/2), SF[j]  ); 
		ttSF -> SetBinError  ( ttSF->FindBin(threshold - (nband-j)*step+step/2), eSF[j] );  

	}

	cout << ttSF->Integral() << endl;
	
	ttSF->Fit("pol1");

}


void GetHistogram( int process ){

	TFile* myfile = new TFile( "../minitrees/" + inputdir + "/TTDM/" + processID[process] + ".root", "read" ); 
	
	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	//mytree -> Draw( "metPfType1                >> metPfType1               ( 3000,   0  ,   3000   )", mycut );
	mytree -> Draw( "mva01_ttDM0001scalar00500 >> mva01_ttDM0001scalar00500 ( 120,  -0.1,      1.1 )", mycut );

	//myhisto[process] = (TH1F*) gDirectory -> Get( "metPfType1"                );
	myhisto[process] = (TH1F*) gDirectory -> Get( "mva01_ttDM0001scalar00500" );

}
