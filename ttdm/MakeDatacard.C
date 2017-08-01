#include "ttdm.h"
#include "../systematics/getPdfQcd.C"


float  yield[nprocess][nsystematic][nlevel];   // {soft, hard, MVA}
float relunc[nprocess][nsystematic][2];        // {norm, shape}

ofstream datacard;

void GetRelUnc( int process, float threshold );

void WriteDatacard( float threshold ); 


void MakeDatacard(){

	Assign(); 
   
	float cuenta = (1.0-inicio)/paso; 

	int ndatacard = cuenta;

	for( int s = 0; s < ndatacard; s++){   // be careful with the comment/uncomment of the closing bracket

		float threshold = inicio + s*paso; 

		cout << "\n \t\t THE THRESHOLD IS... " << threshold << endl;

		for( int m = 0; m < nscalar; m++ ){

			processID[ttDM] = pseudoID[m];	b_name[ANN] = "ANN_tanh_mt2ll80_camille_" + processID[ttDM];

			for( int i = 0; i < nprocess; i++ ){ 

				if ( i == Wg || i == Zg ) continue; 
		
				GetRelUnc( i, threshold );

			}

			WriteDatacard( threshold ); //scalarMVAcut[m]);

		} 

	}   // 's'

}

void GetRelUnc( int process, float threshold ){


	cout << "\n\n\n \t" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic];

	TString pathway = histoSite + processID[ttDM] + "/";



	for( int j = 0; j < nsystematic; j++ ){

		if( process == data && j > nominal ) continue; 

		cout << "\t\t" << systematicID[j] << endl;



		relunc[process][j][nrmlz] = -9999.; 
		relunc[process][j][shape] = -9999.; 

		if( j == DDtt    && process == TT    ){ relunc[process][j][nrmlz] = 1 + ettSF/ttSF; } 
		if( j == DDDY    && process == DY    ){ relunc[process][j][nrmlz] = 1 + eDYSF/DYSF; } 
		if( j == DDfakes && process == fakes ){ relunc[process][j][nrmlz] = 1 + efakes    ; } 

		if( j >= DDtt ) continue;  //goto closure;

		if( j == toppTrw && process != TT )  continue;  //goto closure;   

		if((j >= QCDup && j <= PDFdo) && (process == data || process == ttDM || process == fakes || process == ST ))  continue;  //goto closure;



		TFile* myfile; 

		if( j == nominal ) myfile = new TFile( pathway + processID[process] +                         ".root", "read" ); 
		if( j >  nominal ) myfile = new TFile( pathway + processID[process] + "_" + systematicID[j] + ".root", "read" ); 

		h_syst[j] = (TH1F*) myfile -> Get( b_name[ANN] );

		yield[process][j][hard] = h_syst[j] -> Integral(-1, -1);  
		yield[process][j][NN  ] = h_syst[j] -> Integral( nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1, -1); 
		//yield[process][j][NN  ] = h_syst[j] -> Integral( -1, nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1); 
 

		for( int k = hard; k < nlevel; k++ ){


			if( yield[process][j][k] < 0. ) yield[process][j][k] = 0.;   // CAUTION !!!
		
			if( process != data && process != fakes ) yield[process][j][k] *= thelumi;

			if( process == fakes )                    yield[process][j][k] *= thelumi/35.867;

			if( process == TT ) yield[process][j][k] *= ttSF; 
			if( process == DY ) yield[process][j][k] *= DYSF; 

		}


		if ( yield[process][nominal][hard] > 0 ) 
		relunc[process][j][nrmlz] = (yield[process][j][hard]/1.0) / (yield[process][nominal][hard]/1.0); 


		if ( yield[process][nominal][NN  ] > 0 ){

			//if ( /*process == ttDM ||*/ process == TT || process == fakes || process == DY ){
			//relunc[process][j][shape] = (yield[process][j][NN  ]/1.0) / (yield[process][nominal][NN  ]/1.0); 
			//}

			//else{
			relunc[process][j][shape] = (yield[process][j][NN  ]/yield[process][j][hard]) / (yield[process][nominal][NN  ]/yield[process][nominal][hard]);
			//}

		}


		// protections

		if( j >= QCDup && j <= PDFdo && process == TT ){ 

			relunc[ttDM][j][nrmlz] = relunc[TT][j][nrmlz];  
			relunc[ttDM][j][shape] = relunc[TT][j][shape];
			relunc[ST  ][j][nrmlz] = relunc[TT][j][nrmlz];  
			relunc[ST  ][j][shape] = relunc[TT][j][shape];
			relunc[TTV ][j][nrmlz] = relunc[TT][j][nrmlz];  
			relunc[TTV ][j][shape] = relunc[TT][j][shape];

		}

		if( j >= METup && j <= MuESdo && process == TT ){ 

			relunc[TTV][j][nrmlz] = relunc[TT][j][nrmlz];  
			relunc[TTV][j][shape] = relunc[TT][j][shape];

		}

		if( region == "SR" ){   // remove TT-nrmlz in SR

			if( process == fakes || process == TT || process == DY ) relunc[process][j][nrmlz] = -9999.; 

			if( process == fakes                                   ) relunc[process][j][shape] = -9999.; 

		}

		if( region == "CR" ){   // keep TT-nrmlz in CR

			if( process == fakes ||                  process == DY ) relunc[process][j][nrmlz] = -9999.; 

		}

		///closure : 

		myfile->Close();

	}  // j - systematic

	
}


void WriteDatacard( float threshold ){

	TString pathway = datacard_dir + datacard_folder; 

	gSystem -> mkdir( pathway, kTRUE );

	datacard.open( pathway + Form( "%s_%s_%4.2f.txt", processID[ttDM].Data(), "camille", threshold ) );
	//datacard.open( pathway + Form( "%s_%s.txt", processID[ttDM].Data(), "camille-side" ) );

	datacard << "imax 1 number of channels \n" ;
	datacard << Form( "jmax %d number of backgrounds \n", 9 );//nprocess );
	datacard << Form( "kmax %d  number of nuisance parameters \n", nsystematic-1 );
	datacard << "------------ \n" ;
	datacard << "\n" ;
	datacard << Form("bin %s \n", region.Data());
if( region == "SR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][NN]);   }
if( region == "CR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][hard]); }
	datacard << "------------\n" ;
	datacard << "\n" ;
datacard << Form("bin        \t     \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \n",
	    region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data()  );//\t   1   \t   1   \n";//\t   1   \t   1  \n" ;
datacard << "process    \t     \t  DM   \t fakes \t  ST   \t  TTV  \t  DY   \t  TT   \t  WW   \t  WZ   \t  VZ   \t  VVV  \n";//\t  Wg   \t  Zg   \n";//\t  HWW  \t  HZ  \n" ;
datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \n";//\t  10   \t  11   \n";//\t  12   \t  13  \n" ;
if( region == "SR" ){
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][NN], yield[fakes][nominal][NN], yield[ST][nominal][NN], yield[TTV][nominal][NN], yield[DY][nominal][NN], 
		    yield[TT][nominal][NN], yield[WW][nominal][NN], yield[WZ][nominal][NN], yield[VZ][nominal][NN], yield[VVV][nominal][NN] 
		    /*, yield[Wg][nominal][NN], yield[Zg][nominal][NN], yield[HWW][nominal][NN], yield[HZ][nominal][NN]*/ );
}
if( region == "CR" ){
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][hard], yield[fakes][nominal][hard], yield[ST][nominal][hard], yield[TTV][nominal][hard], yield[DY][nominal][hard], 
		    yield[TT][nominal][hard], yield[WW][nominal][hard], yield[WZ][nominal][hard], yield[VZ][nominal][hard], yield[VVV][nominal][hard]
		    /*, yield[Wg][nominal][hard], yield[Zg][nominal][hard], yield[HWW][nominal][hard], yield[HZ][nominal][hard]*/ );
}
	datacard << "------------ \n" ;

	//datacard << "DDtt_nrmlz 	 lnN 	   -       -      1.073    -       -       -       -       -       -       -    \n" ;


	for( int j = 1; j < nsystematic; j++ ){   // systematic

		if(  j < toppTrw  &&  j%2 == 0  ) continue; 

		for( int k = 0; k < nsysttype; k++ ){

			if( j == toppTrw && k == nrmlz ) continue; 
			if( j == DDtt    && k == shape ) continue; 
			if( j == DDDY    && k == shape ) continue; 
			if( j == DDfakes && k == shape ) continue; 

			//if ( j == DDtt && k == nrmlz ) datacard << Form( "%s_%s \t lnU \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 
			//else                           datacard << Form( "%s_%s \t lnN \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 
			datacard << Form( "%s_%s \t lnN \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 

			for( int i = 1; i <= VVV; i++ ){   // process

				if( relunc[i][j][k] < 0 ){

					datacard << Form( "%7s ", "   -   " ); 
			
				}

				else {
 					float figure = relunc[i][j][k]; 

					if( j < toppTrw ){

						figure = 1+(relunc[i][j][k]-relunc[i][j+1][k])/2; // (up-down)/2

						if( figure < 0.80 )  figure = 0.80;   // cheating...
						if( figure > 1.20 )  figure = 1.20;   // cheating... 

					} 

					//if( figure < 1.0 ) figure = 2 - figure; 


					datacard << Form( "%7.3f ", figure );					

				}


			}   // process	

			datacard << "\n" ; 

		}   // nrmlz or shape

	}   // systematic


	datacard.close();

	cout << "\n" << pathway + Form( "%s_%s_%4.2f.txt", processID[ttDM].Data(), "camille", threshold ) << " was created"  << endl; 

}
