#include "ttdm.h"


float  yield[nprocess][nsystematic];
float relunc[nprocess][nsystematic];

ofstream datacard;

void GetYield( int process );

void WriteDatacard(); 

void MakeDatacard(){

	Assign();

	for( int i = 0; i < nprocess; i++ ){

		GetYield( i ); 

		GetRelativeUncertainties( i ); 

	}

	//GetYield( TT ); 

	WriteDatacard(); 

}


void GetYield( int process ){

	cout << "\n\n\n" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic];

	//for( int j = 0; j < nsystematic; j++ ){

	//	h_syst[j] = new TH1F( "htemp_" + systematicID[j], "histogram for " + systematicID[j], 30, 0, 3000 ); 
	
	//}


	// 'nominal' minitree related

	TFile* myfile = new TFile( "../minitrees/week-13/" + processID[process] + ".root", "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );
	
	//for( int j = 0; j < nsystematic; j++ ){

	//	mytree -> Draw( "metPfType1 >> htemp_" + systematicID[j], mycut*"eventW_" + systematicID[j] );

	//}

	mytree -> Draw( "metPfType1 >> htemp"          , mycut*"eventW"          );
	mytree -> Draw( "metPfType1 >> htemp_Btagup"   , mycut*"eventW_Btagup"   );
	mytree -> Draw( "metPfType1 >> htemp_Btagdo"   , mycut*"eventW_Btagdo"   );
	mytree -> Draw( "metPfType1 >> htemp_Idisoup"  , mycut*"eventW_Idisoup"  );
	mytree -> Draw( "metPfType1 >> htemp_Idisodo"  , mycut*"eventW_Idisodo"  );
	mytree -> Draw( "metPfType1 >> htemp_Triggerup", mycut*"eventW_Triggerup");
	mytree -> Draw( "metPfType1 >> htemp_Triggerdo", mycut*"eventW_Triggerdo");

	h_syst[nominal  ] = (TH1F*) gDirectory -> Get( "htemp"           );
	h_syst[Btagup   ] = (TH1F*) gDirectory -> Get( "htemp_Btagup"    );
	h_syst[Btagdo   ] = (TH1F*) gDirectory -> Get( "htemp_Btagdo"    );
	h_syst[Idisoup  ] = (TH1F*) gDirectory -> Get( "htemp_Idisoup"   );
	h_syst[Idisodo  ] = (TH1F*) gDirectory -> Get( "htemp_Idisodo"   );
	h_syst[Triggerup] = (TH1F*) gDirectory -> Get( "htemp_Triggerup" );
	h_syst[Triggerdo] = (TH1F*) gDirectory -> Get( "htemp_Triggerdo" );


	// MET, JES, LES, minitrees  ... to be done


	for( int j = 0; j < nsystematic; j++ ){

		yield[process][j] = h_syst[j] -> Integral(); 

		if( process != data && process != fakes ) yield[process][j] *= thelumi;

		relunc[process][j] = yield[process][j]/yield[process][nominal];  

		//cout << systematicID[j] << "\t" << yield[process][j] << "\t" << yield[process][nominal] << "\t" << relunc[process][j] << endl; 

	}
	
}


void WriteDatacard(){

	gSystem -> mkdir( "datacards/", kTRUE );

	datacard.open( Form("datacards/%s_%s.txt", "ttDM0001scalar0500", "100" ) );

	datacard << "imax 1   number of channels \n" ;
	datacard << Form( "jmax %d  number of backgrounds \n", nprocess-2 );
	datacard << Form( "kmax %d  number of nuisance parameters \n", nsystematic );
	datacard << "------------ \n" ;
	datacard << "\n" ;
	datacard << "bin 1 \n" ;
	datacard << Form("observation %5.0f \n", yield[data][nominal]);
	datacard << "------------\n" ;
	datacard << "\n" ;
	datacard << "bin        \t     \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1 \n" ;
	datacard << "process    \t     \t  DM   \t  TT   \t  ST   \t  WW   \t  DY   \t fakes \t  WZ   \t  ZZ   \t  TTV  \t  Wg   \t  HZ \n" ;
	datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \t  10 \n" ;
	datacard << Form("rate       \t     \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \n", 
                    yield[ttDM][nominal], yield[TT][nominal], yield[ST][nominal] , yield[WW][nominal], yield[DY][nominal], yield[fakes][nominal], 
                    yield[WZ][nominal] , yield[VZ][nominal], yield[TTV][nominal], yield[Wg][nominal], yield[HZ][nominal] );
	datacard << "------------ \n" ;

	for(int j = 1; j < nsystematic; j++){   // systematic

		datacard << Form( "%10s \t lnN \t", systematicID[j].Data()); 

		for( int i = 1; i < nprocess; i++ ){   // process

			//if( rel_unc[i][j] < -9998 ){

			//	datacard << Form( "%7s \t", "   -   " ); 
			
			//}

			//else {

				datacard << Form( "%7.3f \t", relunc[i][j] );

			//}


		}   // process	

		datacard << "\n" ; 

	}   // systematic

	datacard.close();

}
