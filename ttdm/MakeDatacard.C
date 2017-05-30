#include "ttdm.h"
#include "../systematics/getPdfQcd.C"


float  yield[nprocess][nsystematic][nlevel];   // {soft, hard, MVA}
float relunc[nprocess][nsystematic][2];        // {norm, shape}

ofstream datacard;

void GetRelUnc( int process );

void WriteDatacard( float threshold ); 


void MakeDatacard(){

	Assign();
   
	///for( int s = 0; s < (1.-inicio)/paso; s++){

	///float threshold = inicio + s*paso; //cout << "the threshold is... " << threshold << endl; 

		for( int m = 0; m < nscalar; m++ ){

			if ( m != ttDM0001scalar00010 && m != ttDM0001scalar00010 ) continue;

			//MVA_cut = hard_cut&&Form("ANN_tanh_mt2ll80_regina_%s>%4.2f", scalarID[m].Data(), scalarMVAcut[m] ); //threshold);

			processID[ttDM] = scalarID[m]; 
			//processID[ttDM] = pseudoID[m]; 


			for( int i = 0; i < nprocess; i++ ){

				if ( i == Wg || i == Zg ) continue; 
		
				GetRelUnc( i );

			}

			//GetRelUnc( TT );

			WriteDatacard( scalarMVAcut[m] ); //threshold);


		} 

	///}   // 's'

}

void GetRelUnc( int process ){

	//if ( process == TT ) processID[process] = processID[WW];  // to avoid to go through TT sample -> for checks 

	cout << "\n\n\n" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic][nlevel];	


	for( int j = 0; j < nsystematic; j++ ){ 

		//if ( j !=toppTrw ) continue;

		TCanvas* c1 = new TCanvas("canvas", "the canvas");  

		TString filename;

		if( process == data || process == fakes || process == TTV || process == Wg || process == Zg ){ filename =  storageSite + minitreeDir[0] + "/TTDM/" + processID[process] + ".root"; }

		else{ filename =  storageSite + minitreeDir[j] + "/TTDM/" + processID[process] + ".root"; }

		TFile* myfile = new TFile( filename, "read" ); 

		TTree* mytree = (TTree*) myfile -> Get( "latino" );


		if ( j <= MuESdo ){

			TCut myeventW = eventW[j];
	
			mytree -> Draw( "metPfType1 >> htemp_soft", soft_cut*myeventW ); 
			mytree -> Draw( "metPfType1 >> htemp_hard", hard_cut*myeventW );
			mytree -> Draw( "metPfType1 >> htemp_MVA" , MVA_cut *myeventW );

			h_syst[j][soft] = (TH1F*) gDirectory -> Get( "htemp_soft" );
			h_syst[j][hard] = (TH1F*) gDirectory -> Get( "htemp_hard" );
			h_syst[j][NN  ] = (TH1F*) gDirectory -> Get( "htemp_MVA"  );

		}
	

		// QCD 

		if( process != data && process != ttDM && process != fakes && process != ST && j >= QCDup && j <= PDFdo ){ //&& process != HZ

			int bin; 

			if( j == QCDup ) bin = 8; 
			if( j == QCDdo ) bin = 4; 
			if( j == PDFup ) bin = 9; 
			if( j == PDFdo ) bin = 10; 
	  
			TH1F* weights = (TH1F*) myfile -> Get( "list_vectors_weights" );

			float norm = weights->GetBinContent(bin+1)/weights->GetBinContent(1);

			mytree -> Draw( "metPfType1 >> htemp_theory_soft", soft_cut*Form("eventW*(LHEweight[%d]/LHEweight[0])*%7.4f", bin, norm));
			mytree -> Draw( "metPfType1 >> htemp_theory_hard", hard_cut*Form("eventW*(LHEweight[%d]/LHEweight[0])*%7.4f", bin, norm));
			mytree -> Draw( "metPfType1 >> htemp_theory_MVA" , MVA_cut *Form("eventW*(LHEweight[%d]/LHEweight[0])*%7.4f", bin, norm));


			h_syst[j][soft] = (TH1F*) gDirectory -> Get( "htemp_theory_soft" );
			h_syst[j][hard] = (TH1F*) gDirectory -> Get( "htemp_theory_hard" );
			h_syst[j][NN  ] = (TH1F*) gDirectory -> Get( "htemp_theory_MVA"  );

		}



		// top pT reweighing

		if( j == toppTrw && process == TT ){

			mytree -> Draw( "metPfType1 >> htemp_toppTrw_soft", soft_cut*"eventW_Toppt" );
			mytree -> Draw( "metPfType1 >> htemp_toppTrw_hard", hard_cut*"eventW_Toppt" );
			mytree -> Draw( "metPfType1 >> htemp_toppTrw_MVA" , MVA_cut *"eventW_Toppt" );

			h_syst[toppTrw][soft] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_soft" );
			h_syst[toppTrw][hard] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_hard" );
			h_syst[toppTrw][NN  ] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_MVA"  );

		}



		relunc[process][j][nrmlz] = -9999.; 
		relunc[process][j][shape] = -9999.; 

		if( j == DDtt    && process == TT    ){ relunc[process][j][nrmlz] = 100.00; }//1 + ettSF/ttSF; } 
		if( j == DDDY    && process == DY    ){ relunc[process][j][nrmlz] = 1 + eDYSF/DYSF; } 
		if( j == DDfakes && process == fakes ){ relunc[process][j][nrmlz] = 1 + efakes    ; } 

		if( j >= DDtt ) goto closure;

		if( j == toppTrw && process != TT ) goto closure;   

		if((j >= QCDup && j <= PDFdo) && (process == data || process == ttDM || process == fakes || process == ST /*|| process == HZ*/)) goto closure;


/*   SPEED UP   */	//if( j >= QCDup && j <= PDFdo ) goto closure;

		for( int k = 0; k < nlevel; k++ ){

			yield[process][j][k] = h_syst[j][k] -> Integral(); 


			if( process == TT ){
						
				if( j == METup   ) yield[process][j][k] *= 1.00/0.98; 
				if( j == METdo   ) yield[process][j][k] *= 1.00/0.98; 
				if( j == JESup   ) yield[process][j][k] *= 0.93/0.98; 
				if( j == JESdo   ) yield[process][j][k] *= 0.99/0.98; 
				if( j == EleESup ) yield[process][j][k] *= 0.93/0.98; 
				if( j == EleESdo ) yield[process][j][k] *= 0.94/0.98; 
				if( j == MuESup  ) yield[process][j][k] *= 0.98/0.98; 
				if( j == MuESdo  ) yield[process][j][k] *= 0.92/0.98; 
 						
			}


			if( yield[process][j][k] < 0. ) yield[process][j][k] = 0.;   // CAUTION !!!
		
			if( process != data && process != fakes ) yield[process][j][k] *= thelumi;

			//yield[TT][j][k] *= ttSF; 
			yield[DY][j][k] *= DYSF; 

		}


		if( j >= METup  &&  j <= MuESdo ){

			if ( yield[process][nominal][hard] > 0 ) 
			relunc[process][j][nrmlz] = (yield[process][j][hard]/1.0) / (yield[process][nominal][hard]/1.0); 


			if ( yield[process][nominal][NN  ] > 0 )
			relunc[process][j][shape] = (yield[process][j][NN  ]/yield[process][j][hard]) / (yield[process][nominal][NN  ]/yield[process][nominal][hard]);

		}

		if( j < METup  ||  j > MuESdo ){

			if ( yield[process][nominal][hard] > 0 ) 
			relunc[process][j][nrmlz] = (yield[process][j][hard]/yield[process][j][soft]) / (yield[process][nominal][hard]/yield[process][nominal][soft]); 


			if ( yield[process][nominal][NN  ] > 0 )
			relunc[process][j][shape] = (yield[process][j][NN  ]/yield[process][j][hard]) / (yield[process][nominal][NN  ]/yield[process][nominal][hard]);

		}


		// protections

		if( j >= QCDup && j <= PDFdo && process == TT ){ 

			relunc[ttDM][j][nrmlz] = relunc[TT][j][nrmlz];  
			relunc[ttDM][j][shape] = relunc[TT][j][shape];

		}

		if( region == "SR" ){   // remove TT-nrmlz in SR

			if( process == fakes || process == TT || process == DY ) relunc[process][j][nrmlz] = -9999.; 

		}

		if( region == "CR" ){   // keep TT-nrmlz in CR

			if( process == fakes ||                  process == DY ) relunc[process][j][nrmlz] = -9999.; 

		}

		closure : 

		c1 -> Destructor(); 

		myfile->Close();

	}  // j 
	
}


void WriteDatacard( float threshold ){

	gSystem -> mkdir( "datacards/", kTRUE );

	datacard.open( Form("/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170525/%s_%s_%4.2f_%s_fixed.txt", processID[ttDM].Data(), "mt2ll80", threshold, region.Data() ) );

	datacard << "imax 1 number of channels \n" ;
	datacard << Form( "jmax %d number of backgrounds \n", 9 ); //nprocess );
	datacard << Form( "kmax %d number of nuisance parameters \n", nsystematic-1 );
	datacard << "------------ \n" ;
	datacard << "\n" ;
	datacard << Form("bin %s \n", region.Data());
if( region == "SR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][NN]);   }
if( region == "CR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][hard]); }
	datacard << "------------\n" ;
	datacard << "\n" ;
datacard << Form("bin        \t     \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \n",
	    region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data()  );//\t   1   \t   1   \n";//\t   1   \t   1  \n" ;
datacard << "process    \t     \t  DM   \t fakes \t  TT   \t  ST   \t  DY   \t  TTV  \t  WW   \t  WZ   \t  VZ   \t  VVV  \n";//\t  Wg   \t  Zg   \n";//\t  HWW  \t  HZ  \n" ;
datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \n";//\t  10   \t  11   \n";//\t  12   \t  13  \n" ;
if( region == "SR" ){
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][NN]/9, yield[fakes][nominal][NN], yield[TT][nominal][NN], yield[ST][nominal][NN], yield[DY][nominal][NN], 
		    yield[TTV][nominal][NN], yield[WW][nominal][NN], yield[WZ][nominal][NN], yield[VZ][nominal][NN], yield[VVV][nominal][NN] 
		    /*, yield[Wg][nominal][NN], yield[Zg][nominal][NN], yield[HWW][nominal][NN], yield[HZ][nominal][NN]*/ );
}
if( region == "CR" ){
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][hard]/9, yield[fakes][nominal][hard], yield[TT][nominal][hard], yield[ST][nominal][hard], yield[DY][nominal][hard], 
		    yield[TTV][nominal][hard], yield[WW][nominal][hard], yield[WZ][nominal][hard], yield[VZ][nominal][hard], yield[VVV][nominal][hard]
		    /*, yield[Wg][nominal][hard], yield[Zg][nominal][hard], yield[HWW][nominal][hard], yield[HZ][nominal][hard]*/ );
}
	datacard << "------------ \n" ;

	for( int j = 1; j < nsystematic; j++ ){   // systematic

		if(  j < toppTrw  &&  j%2 == 0  ) continue; 

		for( int k = 0; k < nsysttype; k++ ){

			if( j == toppTrw && k == nrmlz ) continue; 
			if( j == DDtt    && k == shape ) continue; 
			if( j == DDDY    && k == shape ) continue; 
			if( j == DDfakes && k == shape ) continue; 

			if ( j == DDtt && k == nrmlz ) datacard << Form( "%s_%s \t lnU \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 
			else                           datacard << Form( "%s_%s \t lnN \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 

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

					if( figure < 1.0 ) figure = 2 - figure; 


					datacard << Form( "%7.3f ", figure );					

				}


			}   // process	

			datacard << "\n" ; 

		}   // nrmlz or shape

	}   // systematic


	datacard.close();

}
