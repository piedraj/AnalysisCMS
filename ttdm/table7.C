enum{ data,
      fakes,
      TT,
      ST,
      DY,
      TTV,
      WW,
      WZ, 
      VZ,
      VVV,
      Wg,
      Zg,
      HWW,
      HZ,
      ttDMlight,
      ttDMheavy,
      nprocess
}; 

enum { ee, mm, em, ll, nchannel }; 

const float thelumi = 2.15; 

TCut mycut = ""; 

TString processID[nprocess]   ;
TH1F*     myhisto[nprocess][4];
float       yield[nprocess][4];
float      eyield[nprocess][4]; 
float      allbkg          [4];
float     eallbkg          [4];

void GetHistogram( int process );

void table7(){

	processID[ttDMlight] = "ttDM0001scalar00010" ;  
	processID[ttDMheavy] = "ttDM0001scalar00010" ;  
	processID[data ] = "01_Data_reduced_1outof6" ;
	processID[fakes] = "00_Fakes_reduced_1outof6"; 
	processID[TT   ] = "04_TTTo2L2Nu"            ; 
	processID[ST   ] = "05_ST"                   ; 
	processID[DY   ] = "07_ZJets"                ; 
	//processID[TTV  ] = "09_TTV"                  ; 
	processID[TTV  ] = "09_TTV_updated"          ; 
	processID[WW   ] = "06_WW"                   ; 
	processID[WZ   ] = "02_WZTo3LNu"             ; 
	processID[VZ   ] = "03_VZ"                   ; 
	processID[VVV  ] = "13_VVV"                  ; 
	processID[Wg   ] = "11_Wg"                   ; 
	processID[Zg   ] = "12_Zg"                   ; 
	processID[HWW  ] = "10_HWW"                  ; 
	processID[HZ   ] = "14_HZ"                   ;  

	for( int i = 0; i < nprocess; i++ ){

		GetHistogram(i); 

	}

	for( int ch = 0; ch < nchannel; ch++ ){

		for( int i = 0; i < nprocess; i++ ){

			 yield[i][ch] = myhisto[i][ch] -> Integral();				if(  yield[i][ch]<0 )  yield[i][ch] = 0; 

			eyield[i][ch] = sqrt( myhisto[i][ch] -> GetSumw2() -> GetSum() );	if( eyield[i][ch]<0 ) eyield[i][ch] = 0;

			if( i != data && i != fakes ){

				 yield[i][ch] *= thelumi; 
				eyield[i][ch] *= thelumi; 

			}

		}

		 yield[TT][ch] *= 0.97; 
		eyield[TT][ch] *= 0.97;
 		 yield[DY][ch] *= 1.07; 
		eyield[DY][ch] *= 1.07;

		 yield[ttDMlight][ch] *= 10; 
		eyield[ttDMlight][ch] *= 10;
	 	 yield[ttDMheavy][ch] *= 1e3; 
		eyield[ttDMheavy][ch] *= 1e3;

		 allbkg[ch] = yield[HZ][ch] + yield[HWW][ch] + yield[WW][ch] + yield[WZ][ch] + yield[VZ][ch]
			   + yield[DY][ch] + yield[TTV][ch] + yield[TT][ch] + yield[ST][ch] + yield[fakes][ch];

		eallbkg[ch] = sqrt( pow( eyield[HZ][ch], 2) + pow( eyield[HWW][ch], 2) + pow( eyield[WW][ch], 2) + pow( eyield[WZ][ch], 2)
			    + pow( eyield[VZ][ch], 2) + pow( eyield[VVV][ch], 2) + pow( eyield[DY][ch], 2) + pow( eyield[TTV][ch], 2) 
			    + pow( eyield[TT][ch], 2) + pow( eyield[ST][ch], 2) + pow( eyield[fakes][ch], 2) );    

	}

	gSystem -> mkdir( "tables/", kTRUE );

	ofstream mytable; 

	mytable.open( "tables/table.txt" );

	mytable << "\\hline \n"; 

	mytable << "process & ee & $\\mu\\mu$ & e$\\mu$ & $\\ell\\ell$ \\\\ \\hline \n"; 

        mytable << Form("HZ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[HZ][ee], eyield[HZ][ee], yield[HZ][mm], eyield[HZ][mm], yield[HZ][em], eyield[HZ][em], yield[HZ][ll], eyield[HZ][ll]);

        mytable << Form("HWW & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                      yield[HWW][ee], eyield[HWW][ee], yield[HWW][mm], eyield[HWW][mm], yield[HWW][em], eyield[HWW][em], yield[HWW][ll], eyield[HWW][ll]);

        mytable << Form("WW & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[WW][ee], eyield[WW][ee], yield[WW][mm], eyield[WW][mm], yield[WW][em], eyield[WW][em], yield[WW][ll], eyield[WW][ll]);

        mytable << Form("WZ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[WZ][ee], eyield[WZ][ee], yield[WZ][mm], eyield[WZ][mm], yield[WZ][em], eyield[WZ][em], yield[WZ][ll], eyield[WZ][ll]);

        mytable << Form("VZ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[VZ][ee], eyield[VZ][ee], yield[VZ][mm], eyield[VZ][mm], yield[VZ][em], eyield[VZ][em], yield[VZ][ll], eyield[VZ][ll]);

        mytable << Form("VVV & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[VVV][ee], eyield[VVV][ee], yield[VVV][mm], eyield[VVV][mm], yield[VVV][em], eyield[VVV][em], yield[VVV][ll], eyield[VVV][ll]);

        mytable << Form("Z+jets & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[DY][ee], eyield[DY][ee], yield[DY][mm], eyield[DY][mm], yield[DY][em], eyield[DY][em], yield[DY][ll], eyield[DY][ll]);

        mytable << Form("$\\ttbar$V & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                      yield[TTV][ee], eyield[TTV][ee], yield[TTV][mm], eyield[TTV][mm], yield[TTV][em], eyield[TTV][em], yield[TTV][ll], eyield[TTV][ll]);

        mytable << Form("$\\ttbar$ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[TT][ee], eyield[TT][ee], yield[TT][mm], eyield[TT][mm], yield[TT][em], eyield[TT][em], yield[TT][ll], eyield[TT][ll]);

        mytable << Form("tW & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[ST][ee], eyield[ST][ee], yield[ST][mm], eyield[ST][mm], yield[ST][em], eyield[ST][em], yield[ST][ll], eyield[ST][ll]);

        mytable << Form("non-prompt & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \n", 
      yield[fakes][ee], eyield[fakes][ee], yield[fakes][mm], eyield[fakes][mm], yield[fakes][em], eyield[fakes][em], yield[fakes][ll], eyield[fakes][ll]);

	mytable << Form("total bkg & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \\hline \n", 
                                              allbkg[ee], eallbkg[ee], allbkg[mm], eallbkg[mm], allbkg[em], eallbkg[em], allbkg[ll], eallbkg[ll]);

        mytable << Form("m$_{\\chi}$ 1 m$_{\\phi}$ 10 \\,\\, x10 & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[ttDMlight][ee], eyield[ttDMlight][ee], yield[ttDMlight][mm], eyield[ttDMlight][mm], yield[ttDMlight][em], eyield[ttDMlight][em], yield[ttDMlight][ll], eyield[ttDMlight][ll]);

        mytable << Form("m$_{\\chi}$ 1 m$_{\\phi}$ 500 \\,\\, x10$^{3}$& %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \\hline \n", 
                              yield[ttDMheavy][ee], eyield[ttDMheavy][ee], yield[ttDMheavy][mm], eyield[ttDMheavy][mm], yield[ttDMheavy][em], eyield[ttDMheavy][em], yield[ttDMheavy][ll], eyield[ttDMheavy][ll]);

	mytable << Form("data & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \n", 
              yield[data][ee], eyield[data][ee], yield[data][mm], eyield[data][mm], yield[data][em], eyield[data][em], yield[data][ll], eyield[data][ll]);

 	mytable.close();

}


void GetHistogram( int process ){

	TFile* myfile = new TFile( "../minitrees/diciembre/" + processID[process] + ".root", "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	/*mytree -> Draw( "metPfType1 >> htemp_ee", "eventW*(metPfType1>80&&mt2ll>100&&darkpt>0.&&ANN_met80_mt2ll100_ttDM0001scalar00100>0.45&&channel==3)" );
	mytree -> Draw( "metPfType1 >> htemp_mm", "eventW*(metPfType1>80&&mt2ll>100&&darkpt>0.&&ANN_met80_mt2ll100_ttDM0001scalar00100>0.45&&channel==4)" );
	mytree -> Draw( "metPfType1 >> htemp_em", "eventW*(metPfType1>80&&mt2ll>100&&darkpt>0.&&ANN_met80_mt2ll100_ttDM0001scalar00100>0.45&&channel==5)" );
	mytree -> Draw( "metPfType1 >> htemp_ll", "eventW*(metPfType1>80&&mt2ll>100&&darkpt>0.&&ANN_met80_mt2ll100_ttDM0001scalar00100>0.45&&1==1)"       );*/

	mytree -> Draw( "metPfType1 >> htemp_ee", "eventW*(metPfType1>80&&channel==3)" );
	mytree -> Draw( "metPfType1 >> htemp_mm", "eventW*(metPfType1>80&&channel==4)" );
	mytree -> Draw( "metPfType1 >> htemp_em", "eventW*(metPfType1>80&&channel==5)" );
	mytree -> Draw( "metPfType1 >> htemp_ll", "eventW*(metPfType1>80&&1==1)"       );

	myhisto[process][ee] = (TH1F*) gDirectory -> Get( "htemp_ee" );
	myhisto[process][mm] = (TH1F*) gDirectory -> Get( "htemp_mm" );
	myhisto[process][em] = (TH1F*) gDirectory -> Get( "htemp_em" );
	myhisto[process][ll] = (TH1F*) gDirectory -> Get( "htemp_ll" );
	
};


	

