#include "ttdm.h"
#include "../systematics/getPdfQcd.C"


float  yield[nprocess][nsystematic][nlevel];   // {soft, hard, MVA}
float relunc[nprocess][nsystematic][2];        // {norm, shape}

ofstream datacard;

void GetYield( int process );

void WriteDatacard(); 

void MakeDatacard(){

	Assign();

	for( int i = 0; i < nprocess; i++ ){

		GetYield( i ); 

	}

	//WriteDatacard(); 

}


void GetYield( int process ){

	cout << "\n\n\n" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic][nlevel];

	TString filename = "../minitrees/week-13/" + processID[process] + ".root"; 

	TFile* myfile = new TFile( filename, "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );
	

	mytree -> Draw( "metPfType1 >> htemp_soft"          , soft_cut*"eventW"          );
	mytree -> Draw( "metPfType1 >> htemp_Btagup_soft"   , soft_cut*"eventW_Btagup"   );
	mytree -> Draw( "metPfType1 >> htemp_Btagdo_soft"   , soft_cut*"eventW_Btagdo"   );
	mytree -> Draw( "metPfType1 >> htemp_Idisoup_soft"  , soft_cut*"eventW_Idisoup"  );
	mytree -> Draw( "metPfType1 >> htemp_Idisodo_soft"  , soft_cut*"eventW_Idisodo"  );
	mytree -> Draw( "metPfType1 >> htemp_Triggerup_soft", soft_cut*"eventW_Triggerup");
	mytree -> Draw( "metPfType1 >> htemp_Triggerdo_soft", soft_cut*"eventW_Triggerdo");

	h_syst[nominal  ][soft] = (TH1F*) gDirectory -> Get( "htemp_soft"           );
	h_syst[Btagup   ][soft] = (TH1F*) gDirectory -> Get( "htemp_Btagup_soft"    );
	h_syst[Btagdo   ][soft] = (TH1F*) gDirectory -> Get( "htemp_Btagdo_soft"    );
	h_syst[Idisoup  ][soft] = (TH1F*) gDirectory -> Get( "htemp_Idisoup_soft"   );
	h_syst[Idisodo  ][soft] = (TH1F*) gDirectory -> Get( "htemp_Idisodo_soft"   );
	h_syst[Triggerup][soft] = (TH1F*) gDirectory -> Get( "htemp_Triggerup_soft" );
	h_syst[Triggerdo][soft] = (TH1F*) gDirectory -> Get( "htemp_Triggerdo_soft" );

	mytree -> Draw( "metPfType1 >> htemp_hard"          , hard_cut*"eventW"          );
	mytree -> Draw( "metPfType1 >> htemp_Btagup_hard"   , hard_cut*"eventW_Btagup"   );
	mytree -> Draw( "metPfType1 >> htemp_Btagdo_hard"   , hard_cut*"eventW_Btagdo"   );
	mytree -> Draw( "metPfType1 >> htemp_Idisoup_hard"  , hard_cut*"eventW_Idisoup"  );
	mytree -> Draw( "metPfType1 >> htemp_Idisodo_hard"  , hard_cut*"eventW_Idisodo"  );
	mytree -> Draw( "metPfType1 >> htemp_Triggerup_hard", hard_cut*"eventW_Triggerup");
	mytree -> Draw( "metPfType1 >> htemp_Triggerdo_hard", hard_cut*"eventW_Triggerdo");

	h_syst[nominal  ][hard] = (TH1F*) gDirectory -> Get( "htemp_hard"           );
	h_syst[Btagup   ][hard] = (TH1F*) gDirectory -> Get( "htemp_Btagup_hard"    );
	h_syst[Btagdo   ][hard] = (TH1F*) gDirectory -> Get( "htemp_Btagdo_hard"    );
	h_syst[Idisoup  ][hard] = (TH1F*) gDirectory -> Get( "htemp_Idisoup_hard"   );
	h_syst[Idisodo  ][hard] = (TH1F*) gDirectory -> Get( "htemp_Idisodo_hard"   );
	h_syst[Triggerup][hard] = (TH1F*) gDirectory -> Get( "htemp_Triggerup_hard" );
	h_syst[Triggerdo][hard] = (TH1F*) gDirectory -> Get( "htemp_Triggerdo_hard" );

	mytree -> Draw( "metPfType1 >> htemp_hard"          , hard_cut*"eventW"          );
	mytree -> Draw( "metPfType1 >> htemp_Btagup_hard"   , hard_cut*"eventW_Btagup"   );
	mytree -> Draw( "metPfType1 >> htemp_Btagdo_hard"   , hard_cut*"eventW_Btagdo"   );
	mytree -> Draw( "metPfType1 >> htemp_Idisoup_hard"  , hard_cut*"eventW_Idisoup"  );
	mytree -> Draw( "metPfType1 >> htemp_Idisodo_hard"  , hard_cut*"eventW_Idisodo"  );
	mytree -> Draw( "metPfType1 >> htemp_Triggerup_hard", hard_cut*"eventW_Triggerup");
	mytree -> Draw( "metPfType1 >> htemp_Triggerdo_hard", hard_cut*"eventW_Triggerdo");

	h_syst[nominal  ][MVA] = (TH1F*) gDirectory -> Get( "htemp_hard"           );
	h_syst[Btagup   ][MVA] = (TH1F*) gDirectory -> Get( "htemp_Btagup_hard"    );
	h_syst[Btagdo   ][MVA] = (TH1F*) gDirectory -> Get( "htemp_Btagdo_hard"    );
	h_syst[Idisoup  ][MVA] = (TH1F*) gDirectory -> Get( "htemp_Idisoup_hard"   );
	h_syst[Idisodo  ][MVA] = (TH1F*) gDirectory -> Get( "htemp_Idisodo_hard"   );
	h_syst[Triggerup][MVA] = (TH1F*) gDirectory -> Get( "htemp_Triggerup_hard" );
	h_syst[Triggerdo][MVA] = (TH1F*) gDirectory -> Get( "htemp_Triggerdo_hard" );


	// MET, JES, LES, minitrees  ... to be done


return;
	
	//if( process != data && process != fakes && process != ttDM && process != HZ ) getPdfQcd( filename, soft*"eventW" );//, relunc[process][QCDup], relunc[process][QCDdo], relunc[process][PDF] );

	for( int j = 0; j < nsystematic; j++ ){

		//if( j < QCDup){

			for( int k = 0; k < nlevel; k++ ){

				yield[process][j][k] = h_syst[j][k] -> Integral(); 

				if( process != data && process != fakes ) yield[process][j][k] *= thelumi;

				yield[TT][j][k] *= ttSF; 
				yield[DY][j][k] *= DYSF; 

			}

relunc[process][j][nrmlz] = (yield[process][j][hard]/yield[process][j][soft]) / (yield[process][nominal][hard]/yield[process][nominal][soft]);
relunc[process][j][shape] = (yield[process][j][MVA ]/yield[process][j][hard]) / (yield[process][nominal][MVA ]/yield[process][nominal][hard]);

		//}

		//else{

		//}

		// protections

		if( process == fakes ) relunc[process][j][nrmlz] = -9999.;  

		//cout << systematicID[j] << "\t" << yield[process][j] << "\t" << yield[process][nominal] << "\t" << relunc[process][j] << endl; 

	}
	
}


void WriteDatacard(){

	gSystem -> mkdir( "datacards/", kTRUE );

	datacard.open( Form("datacards/%s_%s.txt", "ttDM0001scalar0500", "check" ) );

	datacard << "imax 1   number of channels \n" ;
	datacard << Form( "jmax %d  number of backgrounds \n", nprocess-2 );
	datacard << Form( "kmax %d  number of nuisance parameters \n", nsystematic );
	datacard << "------------ \n" ;
	datacard << "\n" ;
	datacard << "bin 1 \n" ;
	datacard << Form("observation %5.0f \n", yield[data][nominal][MVA]);
	datacard << "------------\n" ;
	datacard << "\n" ;
datacard << "bin        \t     \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1    \t 1   \n" ;
datacard << "process    \t     \t  DM   \t fakes \t  TT   \t  ST   \t  DY   \t  TTV  \t  WW   \t  WZ   \t  VZ   \t  VVV  \t  Wg   \t  Zg   \t  HWW   \t  HZ  \n" ;
datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \t  10   \t  11   \t  12    \t    13  \n" ;
datacard << Form("rate  \t     \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \n", 
                    yield[ttDM][nominal][MVA], yield[fakes][nominal][MVA], yield[TT][nominal][MVA], yield[ST][nominal][MVA], yield[DY][nominal][MVA], 
		    yield[TTV][nominal][MVA], yield[WW][nominal][MVA], yield[WZ][nominal][MVA], yield[VZ][nominal][MVA], yield[VVV][nominal][MVA], 
		    yield[Wg][nominal][MVA], yield[Zg][nominal][MVA], yield[HWW][nominal][MVA], yield[HZ][nominal][MVA] );

	datacard << "------------ \n" ;

	for(int j = 1; j < nsystematic; j++){   // systematic

		if( j%2 == 0 ) continue; 

		datacard << Form( "%10s \t lnN \t", systematicID[j].Data()); 

		for( int i = 1; i < nprocess; i++ ){   // process

			if( relunc[i][j][nrmlz] < 0 ){

				datacard << Form( "%7s \t", "   -   " ); 
			
			}

			else {

				datacard << Form( "%7.3f \t", relunc[i][j][nrmlz] );

			}


		}   // process	

		datacard << "\n" ; 

	}   // systematic

	datacard.close();

}
