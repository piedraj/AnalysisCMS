// root -l -b -q CreateHistograms

#include "../include/Constants.h"

enum{ fakes, data, WZ, VZ, TT, ST, WW, DY, TTV, Wg, Zg, VVV, nprocess }; 

const float theLumi = 35.87; 

const int nband = 8; 

TString processID[nprocess]       ; 
TH1F*     myhisto[nprocess]       ;
float       yield[nprocess][nband];  
float          SF          [nband];
float         eSF          [nband];

float threshold = 80.; 
float      width = 5.;

const TString  inputdir = "/eos/user/j/jgarciaf/minitrees/fucking-mom/";  // where the minitrees are stored

const TCut mycut = "mt2ll<80.&&darkpt>=0.";                 

void GetHistogram( int process );


void ttSF(){

	processID[fakes ] = "00_Fakes_1outof15"; 
	processID[data  ] = "01_Data_Full2016" ; 
	processID[WZ    ] = "02_WZTo3LNu"  ; 
	processID[VZ    ] = "03_VZ"        ; 
	processID[TT    ] = "04_TTTo2L2Nu" ; 
	processID[ST    ] = "05_ST"        ; 
	processID[WW    ] = "06_WW"        ; 
	processID[DY    ] = "07_ZJets"     ; 
	processID[TTV   ] = "09_TTV"       ; 
	processID[Wg    ] = "11_Wg"        ; 
	processID[Zg    ] = "12_Zg"        ; 
	processID[VVV   ] = "13_VVV"       ; 




	for( int i = 0; i < nprocess; i++ ){

		cout << "\n\n" << processID[i] << "\n" << endl; 

		GetHistogram( i ); 

		float myyield;

		if ( i == fakes              ) myyield =    15.0*myhisto[i]->Integral(); 
		if ( i == data               ) myyield =         myhisto[i]->Integral(); 
		if ( i != fakes && i != data ) myyield = theLumi*myhisto[i]->Integral(); 

		cout << myyield << endl;

		if ( i == fakes              ) myhisto[i]->Scale(15.    ); 
		if ( i != fakes && i != data ) myhisto[i]->Scale(theLumi); 
		if ( i == DY )                 myhisto[i]->Scale(0.93); 

		for( int j = 0; j < nband; j++ ){

			yield[i][j] = myhisto[i]->Integral( myhisto[i]->FindBin(threshold - (nband-j)*width), myhisto[i]->FindBin(threshold - (nband-j-1)*width));

		
		}

	}


   	TH1F* ttSF = new TH1F( "ttSF","tt SF", nband, threshold-nband*width, threshold );
    	TH1F* ttSF2 = new TH1F( "ttSF-2","tt SF 2", 16, 0, 80 );
    	TH1F* ttSF3 = new TH1F( "ttSF-3","tt SF 3",  4,80,100 );

	for( int j = 0; j < nband; j++ ){

		float bkg = yield[fakes][j] + yield[WZ][j] + yield[VZ][j] + yield[ST][j] + 
                            yield[WW][j] + yield[DY][j] + yield[TTV][j] +
                            yield[Wg][j] + yield[Zg][j] + yield[VVV][j];
		
		 SF[j] = ( yield[data][j] - bkg )/yield[TT][j];
		eSF[j] = sqrt( yield[data][j] )  /yield[TT][j]; 

		cout << j << "\t" << yield[data][j] << "\t" << bkg << "\t" << yield[TT][j] << "\t" << SF[j] << "\t" << eSF[j] << endl;

		ttSF -> SetBinContent( ttSF->FindBin(threshold - (nband-j)*width+width/2), SF[j]  ); 
		ttSF -> SetBinError  ( ttSF->FindBin(threshold - (nband-j)*width+width/2), eSF[j] ); 
 
			//cout << ttSF->FindBin(threshold - (nband-j)*width+width/2) << " -- " << SF[j] << endl; 

		if( j < 16 ){

			ttSF2 -> SetBinContent( ttSF2->FindBin(threshold - (nband-j)*width+width/2), SF[j]  ); 
			ttSF2 -> SetBinError  ( ttSF2->FindBin(threshold - (nband-j)*width+width/2), eSF[j] );

			//cout << ttSF2->FindBin(threshold - (nband-j)*width+width/2) << " -- " << SF[j] << endl; 
		}
  
		else{

			ttSF3 -> SetBinContent( ttSF3->FindBin(threshold - (nband-j)*width+width/2), SF[j]  ); 
			ttSF3 -> SetBinError  ( ttSF3->FindBin(threshold - (nband-j)*width+width/2), eSF[j] );

			//cout << ttSF3->FindBin(threshold - (nband-j)*width+width/2) << " -- " << SF[j] << endl; 

		}

	}


	ttSF2->Fit("pol0");
	//ttSF3->Fit("pol0");
	
	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas"); 

	ttSF -> SetStats(false); 
	ttSF -> SetTitle("");
	ttSF -> GetXaxis()->SetTitle("m_{T2}^{ll}");
	ttSF -> GetYaxis()->SetTitle("tt SF");
	ttSF -> Draw();
	ttSF2-> Draw("same");
	//ttSF3 ->SetLineColor(kBlack);
 	//ttSF3-> Draw("same");

	mycanvas -> SaveAs("~/www/figures/Analysis_170601_ttCR-extended/ttSF-running_short.pdf");
	mycanvas -> SaveAs("~/www/figures/Analysis_170601_ttCR-extended/ttSF-running_short.png");
	


}


/*void GetHistogram( int process ){

	TFile* myfile = new TFile( "../minitrees/" + inputdir + "/" + processID[process] + ".root", "read" ); 
	
	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	mytree -> Draw( "mt2ll >> mt2ll             ( 3000,   0  ,   3000   )", mycut );

	myhisto[process] = (TH1F*) gDirectory -> Get( "mt2ll"            );


}*/

void GetHistogram( int process ){

	TFile* myfile = new TFile( "histos/ttCR-extended/" + processID[process] + ".root", "read" ); 

	myhisto[process] = (TH1F*) myfile -> Get( "mt2ll" );

}



