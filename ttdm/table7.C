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
      ttDM,
      nprocess
}; 

enum { ee, mm, em, ll, nchannel }; 

const float thelumi = 2.15; 

TCut mycut = "metPfType1>80."; 

TString processID[nprocess]   ;
TH1F*     myhisto[nprocess][4];
float       yield[nprocess][4];
float      eyield[nprocess][4]; 

void GetHistogram( int process );

void table7(){

	processID[ttDM ] = "ttDM0001scalar00500"     ;  
	processID[data ] = "01_Data_reduced_1outof6" ;
	processID[fakes] = "00_Fakes_reduced_1outof6"; 
	processID[TT   ] = "04_TTTo2L2Nu"            ; 
	processID[ST   ] = "05_ST"                   ; 
	processID[DY   ] = "07_ZJets"                ; 
	processID[TTV  ] = "09_TTV"                  ; 
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

			 yield[i][ch] = myhisto[i][ch] -> Integral();

			eyield[i][ch] = sqrt( myhisto[i][ch] -> GetSumw2() -> GetSum() );

			if( i != data && i != fakes ){

				 yield[i][ch] *= thelumi; 
				eyield[i][ch] *= thelumi; 

			}

		}

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

	//mytable << Form("total bkg & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \n", 
        //                                      yield[][ee], eyield[][ee], yield[][mm], eyield[][mm], yield[][em], eyield[][em], yield[][ll], eyield[][ll]);

	mytable << Form("data & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \n", 
              yield[data][ee], eyield[data][ee], yield[data][mm], eyield[data][mm], yield[data][em], eyield[data][em], yield[data][ll], eyield[data][ll]);

 	mytable.close();

}


void GetHistogram( int process ){

	TFile* myfile = new TFile( "../minitrees/diciembre/" + processID[process] + ".root", "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	mytree -> Draw( "metPfType1 >> htemp_ee", mycut*"channel==3" );
	mytree -> Draw( "metPfType1 >> htemp_mm", mycut*"channel==4" );
	mytree -> Draw( "metPfType1 >> htemp_em", mycut*"channel==5" );
	mytree -> Draw( "metPfType1 >> htemp_ll", mycut              );

	myhisto[process][ee] = (TH1F*) gDirectory -> Get( "htemp_ee" );
	myhisto[process][mm] = (TH1F*) gDirectory -> Get( "htemp_mm" );
	myhisto[process][em] = (TH1F*) gDirectory -> Get( "htemp_em" );
	myhisto[process][ll] = (TH1F*) gDirectory -> Get( "htemp_ll" );
	
};


	

