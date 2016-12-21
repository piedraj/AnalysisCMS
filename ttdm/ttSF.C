// root -l -b -q CreateHistograms

#include "../include/Constants.h"

enum{ fakes, data, WZ, VZ, TT, ST, WW, DY, TTV, HWW, Wg, Zg, VVV, HZ, WgStar, nprocess }; 

const float theLumi = 2.15; 

const int nband = 4; 

TString processID[nprocess]       ; 
TH1F*     myhisto[nprocess]       ;
float       yield[nprocess][nband];  
float          SF          [nband];
float         eSF          [nband];

float threshold = 100.; 
float      width = 5.;

const TString  inputdir = "diciembre";  // where the minitrees are stored

const TCut mycut = "eventW*(mt2ll<100.&&darkpt>0.)";                 

void GetHistogram( int process );


void ttSF(){

	processID[fakes ] = "00_Fakes_reduced_1outof6"; 
	processID[data  ] = "01_Data_reduced_1outof6" ; 
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

		float myyield = ( i > data ) ?  theLumi*myhisto[i]->Integral() : myhisto[i]->Integral(); 

		//cout << myyield << endl;

		if( i > data ) myhisto[i]->Scale(theLumi); 

		for( int j = 0; j < nband; j++ ){

			yield[i][j] = myhisto[i]->Integral( myhisto[i]->FindBin(threshold - (nband-j)*width), myhisto[i]->FindBin(threshold - (nband-j-1)*width));

		
		}

	}


   	TH1F* ttSF = new TH1F( "ttSF","tt SF", nband, threshold-nband*width, threshold );
 
	for( int j = 0; j < nband; j++ ){

		float bkg = yield[fakes][j] + yield[WZ][j] + yield[VZ][j] + yield[ST][j] + 
                            yield[WW][j] + yield[DY][j] + yield[TTV][j] + yield[HWW][j] +
                            yield[Wg][j] + yield[Zg][j] + yield[VVV][j] + yield[HZ][j] + 
                            yield[WgStar][j];
		
		 SF[j] = ( yield[data][j] - bkg )/yield[TT][j];
		eSF[j] = sqrt( yield[data][j] )  /yield[TT][j]; 

		//cout << j << " -  " << SF[j] << " +/- " << eSF[j] << endl;

		ttSF -> SetBinContent( ttSF->FindBin(threshold - (nband-j)*width+width/2), SF[j]  ); 
		ttSF -> SetBinError  ( ttSF->FindBin(threshold - (nband-j)*width+width/2), eSF[j] );  

	}

	ttSF->Fit("pol0");
	

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas"); 

	ttSF -> SetStats(false); 

	ttSF -> Draw(); 

	mycanvas -> SaveAs("figures/ttSF.pdf");
	mycanvas -> SaveAs("figures/ttSF.png");
	


}


void GetHistogram( int process ){

	TFile* myfile = new TFile( "../minitrees/" + inputdir + "/" + processID[process] + ".root", "read" ); 
	
	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	mytree -> Draw( "mt2ll >> mt2ll             ( 3000,   0  ,   3000   )", mycut );

	myhisto[process] = (TH1F*) gDirectory -> Get( "mt2ll"            );


}
