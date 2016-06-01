
enum{ data, sig, fakes, WZ, ZZ, TT, ST, WW, Zjets, TTV, Wg, HZ, nsample }; // number of processes considered (includes data & signal)
enum{ nominal, LepMupTup, LepMupTdo, LepElepTup, LepElepTdo, METup, METdo, JESMaxup, JESMaxdo, Btagup, Btagdo, Idisoup, Idisodo, Triggerup, Triggerdo, xsQCDup, acQCDup, xsQCDdo, acQCDdo, xsPDF, acPDF, stat, ninput }; // input
enum{ LepMupT, LepElepT, MET, JESMax, Btag, Idiso, Trigger, QCD, PDF, luminosity, stat_ST, stat_WZ, stat_ZZ, stat_TTV, stat_Wg, stat_HZ, xs_ST, xs_WZ, xs_ZZ, xs_TTV, xs_Wg, xs_HZ, DD_TT, DD_WW, DD_DY, DD_fakes, noutput }; // output

TString sampleID[nsample]; 
TString inputID [ninput ]; 
TString outputID[noutput]; 

const float xs_rel_unc = 0.30; // relative unc on xsection from MC


void ProcessSystematics2(TString ttDM);  // first argument neccesary but unimportant


// just the matrix I'll intend to build and manage with this script...  
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//		nom	LepMupTup	LepMupTdo	LepElepTup	LepElepTdo	METup		METdo		JESMaxup	JESMaxdo  	... 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//data     	
//signal
//00_Fakes
//02_WZTo3LNu
//03_ZZ
//04_TTTo2L2Nu
//05_ST
//06_WW
//07_ZJets
//09_TTV
//11_Wg
//14_HZ
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


void ProcessSystematics(){

	// the assignment of TStrings... 

	sampleID[data ] = "data" ; 
	sampleID[sig  ] = "sig"  ; 
	sampleID[fakes] = "fakes"; 
	sampleID[WZ   ] = "WZ"   ; 
	sampleID[ZZ   ] = "ZZ"   ; 
	sampleID[TT   ] = "TT"   ; 
	sampleID[ST   ] = "ST"   ; 
	sampleID[WW   ] = "WW"   ; 
	sampleID[Zjets] = "Zjets"; 
	sampleID[TTV  ] = "TTV"  ; 
	sampleID[Wg   ] = "Wg"   ; 
	sampleID[HZ   ] = "HZ"   ; 

	inputID[nominal   ] = "nominal"   ; 
	inputID[LepMupTup ] = "LepMupTup" ;  
	inputID[LepMupTdo ] = "LepMupTdo" ;  
	inputID[LepElepTup] = "LepElepTup";  
	inputID[LepElepTdo] = "LepElepTdo";  
	inputID[METup     ] = "METup"     ;  
	inputID[METdo     ] = "METdo"     ;  
	inputID[JESMaxup  ] = "JESMaxup"  ;  
	inputID[JESMaxdo  ] = "JESMaxdo"  ;  
	inputID[Btagup    ] = "Btagup"    ;  
	inputID[Btagdo    ] = "Btagdo"    ;  
	inputID[Idisoup   ] = "Idisoup"   ;  
	inputID[Idisodo   ] = "Idisodo"   ;  
	inputID[Triggerup ] = "Triggerup" ;  
	inputID[Triggerdo ] = "Triggerdo" ;  
	inputID[xsQCDup   ] = "xsQCDup"   ;  
	inputID[acQCDup   ] = "acQCDup"   ;  
	inputID[xsQCDdo   ] = "xsQCDdo"   ;  
	inputID[acQCDdo   ] = "acQCDdo"   ;  
	inputID[xsPDF     ] = "xsPDF"     ;  
	inputID[acPDF     ] = "acPDF"     ;  
	inputID[stat      ] = "stat"      ;  

	outputID[LepMupT ] = "LepMupT" ;  
	outputID[LepElepT] = "LepElepT";  
	outputID[MET     ] = "MET"     ;  
	outputID[JESMax  ] = "JESMax"  ;  
	outputID[Btag    ] = "Btag"    ;  
	outputID[Idiso   ] = "Idiso"   ;  
	outputID[Trigger ] = "Trigger" ;  
	outputID[QCD     ] = "QCD"     ;
	outputID[PDF     ] = "PDF"     ;
	outputID[stat_ST ] = "stat_ST" ;
	outputID[stat_WZ ] = "stat_WZ" ;
	outputID[stat_ZZ ] = "stat_ZZ" ;
	outputID[stat_TTV] = "stat_TTV";
	outputID[stat_Wg ] = "stat_Wg" ;
	outputID[stat_HZ ] = "stat_HZ" ;
	outputID[xs_ST   ] = "xs_ST"   ;
	outputID[xs_WZ   ] = "xs_WZ"   ;
	outputID[xs_ZZ   ] = "xs_ZZ"   ;
	outputID[xs_TTV  ] = "xs_TTV"  ;
	outputID[xs_Wg   ] = "xs_Wg"   ;
	outputID[xs_HZ   ] = "xs_HZ"   ;
	outputID[DD_TT   ] = "DD_TT"   ;
	outputID[DD_WW   ] = "DD_WW"   ;
	outputID[DD_DY   ] = "DD_DY"   ;
	outputID[DD_fakes] = "DD_fakes";
	outputID[luminosity] = "lumi"  ;

	// repeating for evry ttDM point...  

	//ProcessSystematics2("ttDM0001scalar0010");
	ProcessSystematics2("ttDM0001scalar0050");

	//ProcessSystematics2("ttDM0001pseudo0010");



}



void ProcessSystematics2(TString ttDM){

	cout << "                       " << endl;
	cout << "                       " << endl;
	cout << "                       " << endl;
	cout << " ===================== " << endl;
	cout << " == " << ttDM            << endl;
	cout << " ===================== " << endl;
	cout << "                       " << endl;

	float yield[ninput][nsample]; 


	//-- loading the yields from *.dat files... 

	for( int i = 0; i < ninput; i++ ){  

		ifstream syst; 

	  	syst.open("/gpfs/csic_projects/cms/jgarciaf/CMSSW_7_6_3/src/AnalysisCMS_old/tmva/yields/" + ttDM + "_" + inputID[i] + ".dat"); 

		int j = 0; 

		while( 1 ){      

			if (!syst.good()) break;

			if (j > nsample) break;
		
			syst >> yield[i][j];   

			j++;

		}

		syst.close();

	}


	if ( yield[nominal][fakes] < 0 )  yield[nominal][fakes] = 0; 				// protection 
	yield[acQCDup][ZZ] = 1.00; yield[acQCDdo][ZZ] = 1.00; yield[acPDF][ZZ] = 1.00;	        // protection 
	yield[acQCDup][ST] = 1.00; yield[acQCDdo][ST] = 1.00; yield[acPDF][ST] = 1.00;	        // protection 



	//-- for the weighting you'll need... 

	//float totalBkg = yield[nominal][fakes] + yield[nominal][WZ]    + yield[nominal][ZZ]  + yield[nominal][TT] + yield[nominal][ST]    
        //               + yield[nominal][WW]    + yield[nominal][Zjets] + yield[nominal][TTV] + yield[nominal][Wg] + yield[nominal][HZ];



	//-- systematics will need a bit of postprocessing before being written in the datacards...

	float rel_unc[noutput][nsample];


	//-- loop over processes

	for(int j = 0; j < nsample; j++){

		//-- just printing out the preavious readout on the screen...

		//cout << yield[nominal][j] << " -- " << yield[LepMupTup][j] << " -- " << yield[LepMupTdo][j] << " -- " << yield[LepElepTup][j] << " -- " << yield[LepElepTdo][j] << " -- " 
                //                                    << yield[METup]    [j] << " -- " << yield[METdo]    [j] << " -- " << yield[JESMaxup]  [j] << " -- " << yield[JESMaxdo]  [j] << " -- " 
                //     << yield[Btagup] [j] << " -- " << yield[Btagdo]   [j] << " -- " << yield[Idisoup]  [j] << " -- " << yield[Idisodo]   [j] << " -- " << yield[Triggerup] [j] << " -- "  << yield[Triggerdo][j] 
		//     << yield[xsQCDup][j] << " -- " << yield[acQCDup]  [j] << " -- " << yield[xsQCDdo]  [j] << " -- " << yield[acQCDdo]   [j] << " -- " << yield[xsPDF    ] [j] << " -- "  << yield[acPDF    ][j] << endl;
 

		//float weight = yield[nominal][j]/totalBkg;   


		//-- some tricky stuff to get reasonable numbers... 

		rel_unc[LepMupT ][j] = (  abs( yield[LepMupTup ][j] - yield[LepMupTdo ][j] )  ) / ( yield[LepMupTup ][j] + yield[LepMupTdo ][j] );//* weight;
		rel_unc[LepElepT][j] = (  abs( yield[LepElepTup][j] - yield[LepElepTdo][j] )  ) / ( yield[LepElepTup][j] + yield[LepElepTdo][j] );//* weight;
		rel_unc[MET     ][j] = (  abs( yield[METup     ][j] - yield[METdo     ][j] )  ) / ( yield[METup     ][j] + yield[METdo     ][j] );//* weight;
		rel_unc[JESMax  ][j] = (  abs( yield[JESMaxup  ][j] - yield[JESMaxdo  ][j] )  ) / ( yield[JESMaxup  ][j] + yield[JESMaxdo  ][j] );//* weight;
		rel_unc[Btag    ][j] = (  abs( yield[Btagup    ][j] - yield[nominal][j] ) + abs( yield[Btagdo    ][j] - yield[nominal][j] )  ) / (  2* yield[nominal][j]  );//* weight;
		rel_unc[Idiso   ][j] = (  abs( yield[Idisoup   ][j] - yield[nominal][j] ) + abs( yield[Idisodo   ][j] - yield[nominal][j] )  ) / (  2* yield[nominal][j]  );//* weight; 
		rel_unc[Trigger ][j] = (  abs( yield[Triggerup ][j] - yield[nominal][j] ) + abs( yield[Triggerdo ][j] - yield[nominal][j] )  ) / (  2* yield[nominal][j]  );//* weight; 
		rel_unc[QCD     ][j] = (  abs( yield[acQCDup   ][j] - 1                 ) + abs( yield[acQCDdo   ][j] - 1                 )  ) / (  2                     );//* weight;   
		rel_unc[PDF     ][j] =    abs( yield[acPDF     ][j] - 1                 )                                                                                  ;//* weight;

   		rel_unc[luminosity][j] = 0.027;

   		rel_unc[stat_ST ][j] = -9999;  if ( j == ST    )  rel_unc[stat_ST    ][j] = yield[stat][ST] /yield[nominal][ST] ; 
   		rel_unc[stat_WZ ][j] = -9999;  if ( j == WZ    )  rel_unc[stat_WZ    ][j] = yield[stat][WZ] /yield[nominal][WZ] ; 
   		rel_unc[stat_ZZ ][j] = -9999;  if ( j == ZZ    )  rel_unc[stat_ZZ    ][j] = yield[stat][ZZ] /yield[nominal][ZZ] ;
   		rel_unc[stat_TTV][j] = -9999;  if ( j == TTV   )  rel_unc[stat_TTV   ][j] = yield[stat][TTV]/yield[nominal][TTV];
   		rel_unc[stat_Wg ][j] = -9999;  if ( j == Wg    )  rel_unc[stat_Wg    ][j] = yield[stat][Wg] /yield[nominal][Wg] ;
   		rel_unc[stat_HZ ][j] = -9999;  if ( j == HZ    )  rel_unc[stat_HZ    ][j] = yield[stat][HZ] /yield[nominal][HZ] ;
   		rel_unc[xs_ST   ][j] = -9999;  if ( j == ST    )  rel_unc[xs_ST      ][j] = xs_rel_unc;
   		rel_unc[xs_WZ   ][j] = -9999;  if ( j == WZ    )  rel_unc[xs_WZ      ][j] = xs_rel_unc;
   		rel_unc[xs_ZZ   ][j] = -9999;  if ( j == ZZ    )  rel_unc[xs_ZZ      ][j] = xs_rel_unc;
   		rel_unc[xs_TTV  ][j] = -9999;  if ( j == TTV   )  rel_unc[xs_TTV     ][j] = xs_rel_unc;
   		rel_unc[xs_Wg   ][j] = -9999;  if ( j == Wg    )  rel_unc[xs_Wg      ][j] = xs_rel_unc;
   		rel_unc[xs_HZ   ][j] = -9999;  if ( j == HZ    )  rel_unc[xs_HZ      ][j] = xs_rel_unc;
   		rel_unc[DD_TT   ][j] = -9999;  if ( j == TT    )  rel_unc[DD_TT      ][j] = 0.020;
   		rel_unc[DD_WW   ][j] = -9999;  if ( j == WW    )  rel_unc[DD_WW      ][j] = 0.020;
   		rel_unc[DD_DY   ][j] = -9999;  if ( j == Zjets )  rel_unc[DD_DY      ][j] = 0.020;
   		rel_unc[DD_fakes][j] = -9999;  if ( j == fakes )  rel_unc[DD_fakes   ][j] = 0.020;


		if( j == TT || j == WW || j == Zjets || j == fakes ){

			for( int i = 0; i < 10; i++){
	
				rel_unc[i][j] = -9999; 

			}

		}
	
	}





	//-- protection against Combine module when the expected signal rate is too small... 
	
	TString mysuffix = "rescale001"; 

		if( yield[nominal][sig] < 0.1 ){

			yield[nominal][sig] = yield[nominal][sig] * 100; 

			mysuffix = "rescale100";

			cout << ttDM << " has been rescaled" << endl;
		
		} 




	//-- datacard

	ofstream datacard;

	datacard.open(Form("/gpfs/csic_projects/cms/jgarciaf/CMSSW_7_6_3/src/AnalysisCMS_old/tmva/datacards/%s_%s.txt", ttDM.Data(), mysuffix.Data() ));

	datacard << "imax 1   number of channels                                                                                                                                                                  \n" ;
	datacard << Form("jmax %d  number of backgrounds                                                                                                                                                 \n", nsample-2);
	datacard << Form("kmax %d  number of nuisance parameters                                                                                                                                         \n", noutput);
	datacard << "------------                                                                                                                                                                                 \n" ;
	datacard << "                                                                                                                                                                                             \n" ;
	datacard << "bin 1                                                                                                                                                                                        \n" ;
	datacard << Form("observation %5.0f                                                                                                                                                 \n", yield[nominal][data]);
	datacard << "------------                                                                                                                                                                                 \n" ;
	datacard << "                                                                                                                                                                                             \n" ;
	datacard << "bin        \t     \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1 \n" ;
	datacard << "process    \t     \t  DM   \t  TT   \t  ST   \t  WW   \t  DY   \t fakes \t  WZ   \t  ZZ   \t  TTV  \t  Wg   \t  HZ \n" ;
	datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \t  10 \n" ;
	datacard << Form("rate       \t     \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \n", 

                                                yield[nominal][sig], yield[nominal][TT], yield[nominal][ST] , yield[nominal][WW], yield[nominal][Zjets], yield[nominal][fakes],

		                                yield[nominal][WZ] , yield[nominal][ZZ], yield[nominal][TTV], yield[nominal][Wg], yield[nominal][HZ]                                                                 );

	datacard << "------------                                                                                                                                                                                 \n" ;

	for(int i = 0; i < noutput; i++){   // systematic

		datacard << Form( "%10s \t lnN \t", outputID[i].Data()); 

		for( int j = 1; j < nsample; j++ ){   // process

			if( rel_unc[i][j] < -9998 ){

				datacard << Form( "%7s \t", "   -   " ); 
			
			}

			else {

				datacard << Form( "%7.3f \t", 1+rel_unc[i][j] );

			}


		}   // process	

		datacard << "\n" ; 

	}   // systematic

	datacard.close();

}


