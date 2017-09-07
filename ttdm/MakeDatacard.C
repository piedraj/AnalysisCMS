#include "ttdm.h"
#include "../systematics/getPdfQcd.C"


float  yield[nprocess][nsystematic][nlevel];   // {soft, hard, MVA}
float relunc[nprocess][nsystematic][2];        // {norm, shape}

ofstream datacard;

void GetRelUnc( int process, float threshold );

void WriteDatacard( float threshold, TString ttdmID ); 


void MakeDatacard(){

	Assign(); 
   
	float cuenta = (1.0-inicio)/paso; 

	int ndatacard = cuenta;

	for( int s = 0; s < ndatacard; s++){   // be careful with the comment/uncomment of the closing bracket

		float threshold = inicio + s*paso; 

		cout << "\n \t\t THE THRESHOLD IS... " << threshold << endl;

		for( int m = 0; m < nscalar; m++ ){

			//if( m != 0 ) continue;

			processID[ttDM] = scalarID[m];	//b_name[ANN] = "ANN_tanh_mt2ll80_camille_" + processID[ttDM];

			for( int i = 0; i < nprocess; i++ ){ 

				//if( i == DY || i == VZ || i == VVV ) continue; 

				if ( i == Wg || i == Zg ) continue; 
		
				GetRelUnc( i, threshold );

			}

			WriteDatacard( threshold, processID[ttDM] ); //scalarMVAcut[m]);

	//continue;

			processID[ttDM] = pseudoID[m];	//b_name[ANN] = "ANN_tanh_mt2ll80_camille_" + processID[ttDM];

			for( int i = 0; i < nprocess; i++ ){ 

				//if( i == DY || i == VZ || i == VVV ) continue; 

				if ( i == Wg || i == Zg ) continue; 
		
				GetRelUnc( i, threshold );

			}

			WriteDatacard( threshold, processID[ttDM] ); //scalarMVAcut[m]);

		} 

	}   // 's'

}

void GetRelUnc( int process, float threshold ){


	cout << "\n\n\n \t" + processID[process] + "\n" << endl;

	TH1F* h_syst[nsystematic];

	TString pathway = histoSite_datacards + processID[ttDM] + "/";



	for( int j = 0; j < nsystematic; j++ ){

		if( process == data && j > nominal ) continue; 

		cout << "\t\t" << systematicID[j] << endl;

		relunc[process][j][lnN] = -9999.; 
		relunc[process][j][shape] = ( process == fakes || (j == toppTrwup && process != TT)  ) ? -9999. : 1.000; 

		if( j == DDtt    && process == TT    ){ relunc[process][j][lnN] = 1 + ettSF/ttSF; } 
		if( j == DDDY    && process == DY    ){ relunc[process][j][lnN] = 1 + 0.30; }// eDYSF/DYSF; } 
		if( j == DDfakes && process == fakes ){ relunc[process][j][lnN] = 1 + efakes    ; }
                if( process == fakes && j != nominal ) continue; 
		if( j == DDttV   && process == TTV   ){ relunc[process][j][lnN] = 1 + ettV      ; }
 		if( j == pileup                      ){ relunc[process][j][lnN] = 1 + epileup   ; }
 		if( j == luminosity && (process != fakes && process != DY )){ relunc[process][j][lnN] = 1 + elumi   ; }

	
		if( j == toppTrwdo ) continue; 

		if( j >= DDtt ) continue;  //goto closure;

		if( j == toppTrwup && process != TT )  continue;  //goto closure;   

		if((j >= QCDup && j <= PDFdo) && (process == data || process == ttDM || process == fakes || process == ST ))  continue;  //goto closure;

		if( j == Triggerup && process != DY  ){ relunc[process][j][lnN] = 1.02; continue; }
		if( j == Idisoup   && process != DY  ){ relunc[process][j][lnN] = 1.04; continue; }

		if( j != nominal ) continue;

		TFile* myfile;
		//TFile* myfileAux;  

		//if( process != TT ){
			///if( j == nominal ) myfile = new TFile( pathway + processID[process] +                         ".root", "read" ); 
			///if( j >  nominal ) myfile = new TFile( pathway + processID[process] + "_" + systematicID[j] + ".root", "read" ); 
			myfile = new TFile( pathway + "simple-shapes-TH1-final.root", "read");

			///h_syst[j] = (TH1F*) myfile -> Get( b_name[ANN] );

			TString hname = ( j == nominal ) ? processID2[process] : processID2[process] + "_" + systematicIDdatacard[j]; 
			h_syst[j] = (TH1F*) myfile -> Get( hname );

			yield[process][j][hard] = h_syst[j] -> Integral(-1, -1);  
			yield[process][j][NN  ] = h_syst[j] -> Integral( nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1, -1); 
			//yield[process][j][NN  ] = h_syst[j] -> Integral( -1, nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1); 

 		//}

		/*if( process == TT ){
			if( j == nominal ){
				 myfile    = new TFile( pathway + "04_TTTo2L2Nu_1.root", "read" );
				 myfileAux = new TFile( pathway + "04_TTTo2L2Nu_2.root", "read" );
			}
			if( j >  nominal ){
				 myfile    = new TFile( pathway + "04_TTTo2L2Nu_1_" + systematicID[j] + ".root", "read" ); 
				 myfileAux = new TFile( pathway + "04_TTTo2L2Nu_2_" + systematicID[j] + ".root", "read" );
			} 

			h_syst[j]   = (TH1F*) myfile    -> Get( b_name[ANN] );
			TH1F* h_aux = (TH1F*) myfileAux -> Get( b_name[ANN] );

			yield[process][j][hard] = h_syst[j] -> Integral(-1, -1) + h_aux -> Integral(-1, -1);  
			yield[process][j][NN  ] = h_syst[j] -> Integral( nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1, -1) + 
						  h_aux     -> Integral( nbinraw[ANN]*(threshold-lowraw[ANN])/(upraw[ANN]-lowraw[ANN])+1, -1);
		}*/


		for( int k = hard; k < nlevel; k++ ){


			if( yield[process][j][k] < 0. ) yield[process][j][k] = 0.;   // CAUTION !!!
		
			///if( process != data && process != fakes ) yield[process][j][k] *= thelumi;

			///if( process == fakes )                    yield[process][j][k] *= thelumi/35.867;

			//if( process == TT ) yield[process][j][k] *= ttSF; 
			//if( process == DY ) yield[process][j][k] *= DYSF; 

		}


		if ( yield[process][nominal][hard] > 0 ) 
		relunc[process][j][lnN] = (yield[process][j][hard]/1.0) / (yield[process][nominal][hard]/1.0); 

		//cout << yield[process][j][hard] << "\t" << yield[process][nominal][hard] << endl;


		if ( yield[process][nominal][NN  ] > 0 )
		relunc[process][j][shape] = (yield[process][j][NN]/yield[process][j][hard]) / (yield[process][nominal][NN]/yield[process][nominal][hard]);
			
		

		// protections

		if( j >= QCDup && j <= PDFdo && process == TT ){ 

			relunc[ttDM][j][lnN  ] = relunc[TT][j][lnN];  
			relunc[ttDM][j][shape] = relunc[TT][j][shape];
			relunc[ST  ][j][lnN  ] = relunc[TT][j][lnN];  
			relunc[ST  ][j][shape] = relunc[TT][j][shape];
			//relunc[TTV ][j][lnN] = relunc[TT][j][lnN];  
			relunc[TTV ][j][shape] = relunc[TT][j][shape];

		}

		if( j >= METup && j <= MuESdo && process == TT ){ 

			//relunc[TTV][j][lnN] = relunc[TT][j][lnN];  
			relunc[TTV][j][shape] = relunc[TT][j][shape];

		}

		if( region == "SR" ){   // remove TT-lnN in SR

			if( process == fakes || process == TT || process == DY || process == TTV ) relunc[process][j][lnN] = -9999.; 

			if( process == fakes                                                     ) relunc[process][j][shape] = -9999.; 

		}

		if( region == "CR" ){   // keep TT-lnN in CR

			if( process == fakes ||                  process == DY ) relunc[process][j][lnN] = -9999.; 

		}

		///closure : 

		myfile -> Close();

		//if( process == TT ) myfileAux -> Close(); 

	}  // j - systematic

	
}


void WriteDatacard( float threshold, TString ttdmID ){

	TString pathway1 = datacard_dir        + datacard_folder + "/" + ttdmID + "/"; 
	TString pathway2 = histoSite_datacards +    ttdmID + "/";
 
	gSystem -> mkdir( pathway1, kTRUE );
	
	TString datacard_suffix = Form( "%s_%s.txt", processID[ttDM].Data(), "definitive" );

	TString datacard_name1 = pathway1 + datacard_suffix;
	TString datacard_name2 = pathway2 + datacard_suffix;

	//cout << datacard_name1 << endl; 

	//datacard.open( pathway + Form( "%s_%s_%4.2f.txt", processID[ttDM].Data(), "camille", threshold ) );
	//datacard.open( pathway + Form( "%s_%s.txt", processID[ttDM].Data(), "camille-side" ) );

	datacard.open( datacard_name1 );
        //datacard.open( datacard_name2 );

	datacard << "imax 1 number of channels \n" ;
	datacard << Form( "jmax %d number of backgrounds \n", 9 );//nprocess );
	datacard << Form( "kmax %d  number of nuisance parameters \n", 112 ); //nsystematic-1 );
	datacard << "------------ \n" ;
	datacard << "shapes * * simple-shapes-TH1-final.root $PROCESS $PROCESS_$SYSTEMATIC \n";
	datacard << "--------------- \n";	
	datacard << "\n" ;
	datacard << Form("bin %s \n", region.Data());
if( region == "SR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][hard]); }
if( region == "CR" ){ datacard << Form("observation %5.0f \n", yield[data][nominal][hard]); }
	datacard << "------------\n" ;
	datacard << "\n" ;




/*datacard << Form("bin        \t     \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t   %s   \n",
	    region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data()  );//\t   1   \t   1   \n";//\t   1   \t   1  \n" ;

datacard << "process    \t     \t signal\t fakes \t  ST   \t  TTV  \t   TT  \t  WW   \t  WZ   \n";//\t  Wg   \t  Zg   \n";//\t  HWW  \t  HZ  \n" ;

datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \n";//\t  10   \t  11   \n";//\t  12   \t  13  \n" ;
if( region == "SR" ){*/

//datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
//                    yield[ttDM][nominal][hard], yield[fakes][nominal][hard], yield[ST][nominal][hard], yield[TTV][nominal][hard], 
//		    yield[TT][nominal][hard], yield[WW][nominal][hard], yield[WZ][nominal][hard] 
//		    /*, yield[Wg][nominal][NN], yield[Zg][nominal][NN], yield[HWW][nominal][NN], yield[HZ][nominal][NN]*/ );
//}


datacard << Form("bin        \t     \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \t  %s   \n",
	    region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data(), region.Data()  );//\t   1   \t   1   \n";//\t   1   \t   1  \n" ;

datacard << "process    \t     \t signal\t fakes \t  ST   \t  TTV  \t  DY   \t  TT   \t  WW   \t  WZ   \t  VZ   \t  VVV  \n";//\t  Wg   \t  Zg   \n";//\t  HWW  \t  HZ  \n" ;

datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \n";//\t  10   \t  11   \n";//\t  12   \t  13  \n" ;
if( region == "SR" ){

datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][hard], yield[fakes][nominal][hard], yield[ST][nominal][hard], yield[TTV][nominal][hard], yield[DY][nominal][hard], 
		    yield[TT][nominal][hard], yield[WW][nominal][hard], yield[WZ][nominal][hard], yield[VZ][nominal][hard], yield[VVV][nominal][hard] 
		    /*, yield[Wg][nominal][NN], yield[Zg][nominal][NN], yield[HWW][nominal][NN], yield[HZ][nominal][NN]*/ );
}




if( region == "CR" ){
datacard << Form("rate  \t\t   \t%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f \n", /*%7.3f %7.3f \n", //%7.3f %7.3f \n",*/ 
                    yield[ttDM][nominal][hard], yield[fakes][nominal][hard], yield[ST][nominal][hard], yield[TTV][nominal][hard], yield[DY][nominal][hard], 
		    yield[TT][nominal][hard], yield[WW][nominal][hard], yield[WZ][nominal][hard], yield[VZ][nominal][hard], yield[VVV][nominal][hard]
		    /*, yield[Wg][nominal][hard], yield[Zg][nominal][hard], yield[HWW][nominal][hard], yield[HZ][nominal][hard]*/ );
}




	datacard << "------------ \n" ;



	for( int j = 1; j < nsystematic; j++ ){   // systematic

		if(  j <= toppTrwdo  &&  j%2 == 0  ) continue; 

		for( int k = 0; k < nsysttype; k++ ){

if( ( j == Btagup || j == METup || j == JESup   || j == EleESup || j == MuESup || j == QCDup || j == PDFup || j == toppTrwup ) && k == lnN ) continue;
if( ( j == DDtt   || j == DDDY  || j == DDfakes || j == DDttV   || j == pileup || j == luminosity ) && k == shape ) continue;
			/*if( j == toppTrwdo && k == lnN ) continue; 
			if( j == DDtt    && k == shape ) continue; 
			if( j == DDDY    && k == shape ) continue; 
			if( j == DDfakes && k == shape ) continue; 
			if( j == DDttV   && k == shape ) continue; 
			if( j == pileup  && k == shape ) continue; 
			if( j == luminosity&&k== shape ) continue;*/

if( j >= METup   && j <= METdo  ) continue;
if( j >= EleESup && j <= MuESdo ) continue; 
if( j == DDtt )                   continue;
// if( j == DDDY )                   continue;
if( j == QCDup)                   continue;

			if( ( j == Triggerup || j == Idisoup ) && k == lnN )

				datacard << Form( "%7s2%5s     \t", systematicIDdatacard0[j].Data(), systtypeID[k].Data() ); 

			else

				datacard << Form( "%7s %5s     \t", systematicIDdatacard0[j].Data(), systtypeID[k].Data() ); 
				
			for( int i = 1; i <= VVV; i++ ){   // process

				//if( i == DY || i == VZ || i == VVV ) continue; 

				if( relunc[i][j][k] < 0 ){

					datacard << Form( "%7s ", "   -   " ); 
		
				}

				else {
					//if( j < toppTrw ){
					
					//	figure = 1+(relunc[i][j][k]-relunc[i][j+1][k])/2; // (up-down)/2

					//} 

					//if( figure < 0.80 )  figure = 0.80;   // cheating...
					//if( figure > 1.20 )  figure = 1.20;   // cheating... 

					//if( j == toppTrw ) datacard << Form( "1./%4.3f", figure );	
					//else               datacard << Form( "%7.3f ",   figure );	

					datacard << Form( "%7.3f ",  relunc[i][j][k]  );
			
				}

				
			}   // process	

			datacard << "\n" ; 


		}   // lnN or shape

	}   // systematic




datacard << "QCD_signal   shape        1.0      -       -       -       -       -       -       -       -       -\n";
datacard << "QCD_TTV      shape         -       -       -      1.0      -       -       -       -       -       -\n";
datacard << "QCD_DY       shape         -       -       -       -      1.0      -       -       -       -       -\n";
datacard << "QCD_TT       shape         -       -       -       -       -      1.0      -       -       -       -\n";
//datacard << "QCD          shape        1.0      -       -      1.0     1.0     1.0      -       -       -       -\n";
datacard << "ibin_1_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n";
datacard << "ibin_2_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_3_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_4_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_5_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_6_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_7_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_8_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_9_stat_ shape        1.0      -       -       -       -       -       -       -       -       -\n"; 
datacard << "ibin_10_stat_ shape       1.0      -       -       -       -       -       -       -       -       -\n";        
datacard << "ibin_1_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";       
datacard << "ibin_2_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n"; 
datacard << "ibin_3_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_4_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_5_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_6_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_7_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_8_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_9_stat_ shape         -       -      1.0      -       -       -       -       -       -       -\n";         
datacard << "ibin_10_stat_ shape        -       -      1.0      -       -       -       -       -       -       -\n";                 
datacard << "ibin_1_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";
datacard << "ibin_2_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n"; 
datacard << "ibin_3_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_4_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_5_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_6_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_7_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_8_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_9_stat_ shape         -       -       -      1.0      -       -       -       -       -       -\n";        
datacard << "ibin_10_stat_ shape        -       -       -      1.0      -       -       -       -       -       -\n";               
datacard << "ibin_1_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";
datacard << "ibin_2_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";  
datacard << "ibin_3_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_4_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_5_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_6_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_7_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_8_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_9_stat_ shape         -       -       -       -      1.0      -       -       -       -       -\n";        
datacard << "ibin_10_stat_ shape        -       -       -       -      1.0      -       -       -       -       -\n";              
datacard << "ibin_1_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";
datacard << "ibin_2_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_3_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_4_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_5_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_6_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_7_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_8_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_9_stat_ shape         -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_10_stat_ shape        -       -       -       -       -      1.0      -       -       -       -\n";        
datacard << "ibin_1_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n";      
datacard << "ibin_2_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_3_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_4_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_5_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_6_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_7_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_8_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_9_stat_ shape         -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_10_stat_ shape        -       -       -       -       -       -      1.0      -       -       -\n"; 
datacard << "ibin_1_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";             
datacard << "ibin_2_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";   
datacard << "ibin_3_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_4_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_5_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_6_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_7_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_8_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_9_stat_ shape         -       -       -       -       -       -       -      1.0      -       -\n";       
datacard << "ibin_10_stat_ shape        -       -       -       -       -       -       -      1.0      -       -\n";           
datacard << "ibin_1_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";             
datacard << "ibin_2_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";    
datacard << "ibin_3_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_4_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_5_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_6_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_7_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_8_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_9_stat_ shape         -       -       -       -       -       -       -       -      1.0      -\n";      
datacard << "ibin_10_stat_ shape        -       -       -       -       -       -       -       -      1.0      -\n";        
datacard << "ibin_1_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";            
datacard << "ibin_2_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";   
datacard << "ibin_3_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_4_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_5_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_6_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_7_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_8_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_9_stat_ shape         -       -       -       -       -       -       -       -       -     1.0\n";         
datacard << "ibin_10_stat_ shape        -       -       -       -       -       -       -       -       -     1.0\n";               
datacard << "ST_xs     lnN              -       -       1.2     -       -       -       -       -       -       -\n";
datacard << "WW_xs     lnN              -       -       -       -       -       -       1.2     -       -       -\n"; 
datacard << "WZ_xs     lnN              -       -       -       -       -       -       -      1.2      -       -\n";
datacard << "VZ_xs     lnN              -       -       -       -       -       -       -       -      1.2      -\n";
datacard << "VVV_xs    lnN              -       -       -       -       -       -       -       -       -      1.2\n";

	datacard.close();

	cout << "\n" << datacard_name1 << " was created"  << endl; 

}
