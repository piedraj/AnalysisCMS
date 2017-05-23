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

			if ( m != ttDM0001scalar00500 && m != ttDM0001scalar00500 ) continue;

			MVA_cut = hard_cut&&Form("ANN_tanh_mt2ll80_regina_%s>%4.2f", scalarID[m].Data(), scalarMVAcut[m] ); //threshold);

			processID[ttDM] = scalarID[m]; 
			//processID[ttDM] = pseudoID[m]; 


			for( int i = 0; i < nprocess; i++ ){

		
				GetRelUnc( i );

			}

			//GetRelUnc( DY );

			cout << " ...writing starts " << endl; 

			WriteDatacard( scalarMVAcut[m] ); //threshold);

			cout << " ...writing ended" << endl;

		} 

	///}   // 's'

}


void GetRelUnc( int process ){

	if ( process == TT ) processID[process] = processID[WW];  // to avoid to go through TT sample -> for checks 

	cout << "\n\n\n" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic][nlevel];	


	for( int j = 0; j < nsystematic; j++ ){ 

		TCanvas* c1 = new TCanvas("canvas", "the canvas");  

		TString filename;

		if( process == ttDM ){ filename =  storageSite + minitreeDir[j] + "/TTDM/" + processID[process] + ".root"; }
		else                 { filename =  storageSite + minitreeDir[0] + "/TTDM/" + processID[process] + ".root"; }

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
	
		if( process != data && process != ttDM && process != fakes && process != ST && j >= QCDup && j <= QCDdo ){ //&& process != HZ
	  
			TH1F* weights = (TH1F*) myfile -> Get( "list_vectors_weights" );

			float qcd_norm_up = weights->GetBinContent(9)/weights->GetBinContent(1);
			float qcd_norm_do = weights->GetBinContent(5)/weights->GetBinContent(1);

			float pdf_norm_up = weights->GetBinContent(10)/weights->GetBinContent(1);
			float pdf_norm_do = weights->GetBinContent(11)/weights->GetBinContent(1);


			mytree -> Draw( "metPfType1 >> htemp_QCDup_soft", soft_cut*"eventW*(LHEweight[8]/LHEweight[0])"*Form("%7.4f", qcd_norm_up));
			mytree -> Draw( "metPfType1 >> htemp_QCDup_hard", hard_cut*"eventW*(LHEweight[8]/LHEweight[0])"*Form("%7.4f", qcd_norm_up));
			mytree -> Draw( "metPfType1 >> htemp_QCDup_MVA" , MVA_cut *"eventW*(LHEweight[8]/LHEweight[0])"*Form("%7.4f", qcd_norm_up));

			mytree -> Draw( "metPfType1 >> htemp_QCDdo_soft", soft_cut*"eventW*(LHEweight[4]/LHEweight[0])"*Form("%7.4f", qcd_norm_do));
			mytree -> Draw( "metPfType1 >> htemp_QCDdo_hard", hard_cut*"eventW*(LHEweight[4]/LHEweight[0])"*Form("%7.4f", qcd_norm_do));
			mytree -> Draw( "metPfType1 >> htemp_QCDdo_MVA" , MVA_cut *"eventW*(LHEweight[4]/LHEweight[0])"*Form("%7.4f", qcd_norm_do));

			mytree -> Draw( "metPfType1 >> htemp_PDFup_soft", soft_cut*"eventW*(LHEweight[9]/LHEweight[0])"*Form("%7.4f", pdf_norm_up));
			mytree -> Draw( "metPfType1 >> htemp_PDFup_hard", hard_cut*"eventW*(LHEweight[9]/LHEweight[0])"*Form("%7.4f", pdf_norm_up));
			mytree -> Draw( "metPfType1 >> htemp_PDFup_MVA" , MVA_cut *"eventW*(LHEweight[9]/LHEweight[0])"*Form("%7.4f", pdf_norm_up));

			mytree -> Draw( "metPfType1 >> htemp_PDFdo_soft", soft_cut*"eventW*(LHEweight[10]/LHEweight[0])"*Form("%7.4f", pdf_norm_do));
			mytree -> Draw( "metPfType1 >> htemp_PDFdo_hard", hard_cut*"eventW*(LHEweight[10]/LHEweight[0])"*Form("%7.4f", pdf_norm_do));
			mytree -> Draw( "metPfType1 >> htemp_PDFdo_MVA" , MVA_cut *"eventW*(LHEweight[10]/LHEweight[0])"*Form("%7.4f", pdf_norm_do));


			h_syst[QCDup][soft] = (TH1F*) gDirectory -> Get( "htemp_QCDup_soft" );
			h_syst[QCDup][hard] = (TH1F*) gDirectory -> Get( "htemp_QCDup_hard" );
			h_syst[QCDup][NN  ] = (TH1F*) gDirectory -> Get( "htemp_QCDup_MVA"  );

			h_syst[QCDdo][soft] = (TH1F*) gDirectory -> Get( "htemp_QCDdo_soft" );
			h_syst[QCDdo][hard] = (TH1F*) gDirectory -> Get( "htemp_QCDdo_hard" );
			h_syst[QCDdo][NN  ] = (TH1F*) gDirectory -> Get( "htemp_QCDdo_MVA"  );

			h_syst[PDFup][soft] = (TH1F*) gDirectory -> Get( "htemp_PDFup_soft" );
			h_syst[PDFup][hard] = (TH1F*) gDirectory -> Get( "htemp_PDFup_hard" );
			h_syst[PDFup][NN  ] = (TH1F*) gDirectory -> Get( "htemp_PDFup_MVA"  );

			h_syst[PDFdo][soft] = (TH1F*) gDirectory -> Get( "htemp_PDFdo_soft" );
			h_syst[PDFdo][hard] = (TH1F*) gDirectory -> Get( "htemp_PDFdo_hard" );
			h_syst[PDFdo][NN  ] = (TH1F*) gDirectory -> Get( "htemp_PDFdo_MVA"  );

		}


//if( process != data && process != fakes && process != ttDM && process != HZ ) getPdfQcd( filename, soft*"eventW" );//, relunc[process][QCDup], relunc[process][QCDdo], relunc[process][PDF] );   // USABLE? 



		// top pT reweighing

		if( j == toppTrw && process == TT ){

			/*mytree -> Draw( "metPfType1 >> htemp_toppTrw_soft", soft_cut*"eventW*toppTRwW" );
			mytree -> Draw( "metPfType1 >> htemp_toppTrw_hard", hard_cut*"eventW*toppTRwW" );
			mytree -> Draw( "metPfType1 >> htemp_toppTrw_MVA" , MVA_cut *"eventW*toppTRwW" );

			h_syst[toppTrw][soft] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_soft" );
			h_syst[toppTrw][hard] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_hard" );
			h_syst[toppTrw][NN  ] = (TH1F*) gDirectory -> Get( "htemp_toppTrw_MVA"  );*/

		}



		relunc[process][j][nrmlz] = -9999.; 
		relunc[process][j][shape] = -9999.; 

		if( j == DDtt    && process == TT    ){ relunc[process][j][nrmlz] = 1 + ettSF/ttSF; } 
		if( j == DDDY    && process == DY    ){ relunc[process][j][nrmlz] = 1 + eDYSF/DYSF; } 
		if( j == DDfakes && process == fakes ){ relunc[process][j][nrmlz] = 1 + efakes    ; } 

		if( j > toppTrw ) goto closure;  

		if((j >= QCDup && j <= PDFdo) && (process == data || process == ttDM || process == fakes || process == ST /*|| process == HZ*/)) goto closure;

		if( j == toppTrw /*&& process != TT*/ ) goto closure; 

/*   SPEED UP   */	if( j >= QCDup && j <= PDFdo ) goto closure;

		for( int k = 0; k < nlevel; k++ ){

			yield[process][j][k] = h_syst[j][k] -> Integral(); 

			if( yield[process][j][k] < 0. ) yield[process][j][k] = 0.;   // CAUTION !!!
		
			if( process != data && process != fakes ) yield[process][j][k] *= thelumi;

			yield[TT][j][k] *= ttSF; 
			yield[DY][j][k] *= DYSF; 

		}


		if ( yield[process][nominal][hard] > 0 ) 
relunc[process][j][nrmlz] = (yield[process][j][hard]/yield[process][j][soft]) / (yield[process][nominal][hard]/yield[process][nominal][soft]); 

		if ( yield[process][nominal][NN  ] > 0 )
relunc[process][j][shape] = (yield[process][j][NN  ]/yield[process][j][hard]) / (yield[process][nominal][NN  ]/yield[process][nominal][hard]);

	//if( process == Wg || process == Zg ) cout << yield[process][j][NN  ] << " -- " << yield[process][j][hard] << " -- " << yield[process][nominal][NN  ] << " -- " << yield[process][nominal][hard] << endl;

		// ? }

		// ? else{s

		// ? }

		// protections

		if( j == QCDup && process == TT ){ 

			relunc[ttDM][QCDup][nrmlz] = relunc[TT][QCDup][nrmlz];  
			relunc[ttDM][QCDup][shape] = relunc[TT][QCDup][shape];

		}

		if( j == QCDdo && process == TT ){ 

			relunc[ttDM][QCDdo][nrmlz] = relunc[TT][QCDdo][nrmlz];  
			relunc[ttDM][QCDdo][shape] = relunc[TT][QCDdo][shape];

		}

		if( j == PDFup && process == TT ){ 

			relunc[ttDM][PDFup][nrmlz] = relunc[TT][PDFup][nrmlz];  
			relunc[ttDM][PDFup][shape] = relunc[TT][PDFup][shape];

		}

		if( j == PDFdo && process == TT ){ 

			relunc[ttDM][PDFdo][nrmlz] = relunc[TT][PDFdo][nrmlz];  
			relunc[ttDM][PDFdo][shape] = relunc[TT][PDFdo][shape];

		}

		// DataDriven backgrounds: 

		if( process == fakes || process == TT || process == DY ) relunc[process][j][nrmlz] = -9999.; 

		closure : 

		c1 -> Destructor(); 

		myfile->Close();

	}  // j 
	
}


void WriteDatacard( float threshold ){

	gSystem -> mkdir( "datacards/", kTRUE );

	datacard.open( Form("/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170516/%s_%s_%4.2f.txt", processID[ttDM].Data(), "mt2ll100", threshold ) );

	datacard << "imax 1 number of channels \n" ;
	datacard << Form( "jmax %d number of backgrounds \n", nprocess-2 );
	datacard << Form( "kmax %d number of nuisance parameters \n", nsystematic-1 );
	datacard << "------------ \n" ;
	datacard << "\n" ;
	datacard << "bin 1 \n" ;
	datacard << Form("observation %5.0f \n", yield[data][nominal][NN]);
	datacard << "------------\n" ;
	datacard << "\n" ;
datacard << "bin        \t     \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \n";//\t   1   \t   1  \n" ;
datacard << "process    \t     \t  DM   \t fakes \t  TT   \t  ST   \t  DY   \t  TTV  \t  WW   \t  WZ   \t  VZ   \t  VVV  \t  Wg   \t  Zg   \n";//\t  HWW  \t  HZ  \n" ;
datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \t  10   \t  11   \n";//\t  12   \t  13  \n" ;
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", //%7.3f %7.3f \n", 
                    yield[ttDM][nominal][NN]/9, yield[fakes][nominal][NN], yield[TT][nominal][NN], yield[ST][nominal][NN], yield[DY][nominal][NN], 
		    yield[TTV][nominal][NN], yield[WW][nominal][NN], yield[WZ][nominal][NN], yield[VZ][nominal][NN], yield[VVV][nominal][NN], 
		    yield[Wg][nominal][NN], yield[Zg][nominal][NN] /*yield[HWW][nominal][NN], yield[HZ][nominal][NN]*/ );

	datacard << "------------ \n" ;

	for( int j = 1; j < nsystematic; j++ ){   // systematic

		if(  j < toppTrw  &&  j%2 == 0  ) continue; 

		for( int k = 0; k < nsysttype; k++ ){

			if( j == toppTrw && k == nrmlz ) continue; 
			if( j == DDtt    && k == shape ) continue; 
			if( j == DDDY    && k == shape ) continue; 
			if( j == DDfakes && k == shape ) continue; 

			datacard << Form( "%s_%s \t lnN \t", systematicIDdatacard[j].Data(), systtypeID[k].Data() ); 

			for( int i = 1; i < nprocess; i++ ){   // process

				if( relunc[i][j][k] < 0 ){

					datacard << Form( "%7s ", "   -   " ); 
			
				}

				else {
 
					datacard << Form( "%7.3f ", relunc[i][j][k] );

				}


			}   // process	

			datacard << "\n" ; 

		}   // nrmlz or shape

	}   // systematic


	datacard.close();

}
