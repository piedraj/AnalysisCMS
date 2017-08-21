#include "ttdm.h" 
 
TH1F*       histo[nprocess][4];
float       yield[nprocess][4];
float      eyield[nprocess][4]; 
float      allbkg          [4];
float     eallbkg          [4];

void GetHistogram( int process );

void table7(){

	Assign(); 
	
	int zoom = 1;   // zoomx for the signal

	for( int i = 0; i < nprocess; i++ ){

		//if ( i == Wg || i == Zg ) continue;

		GetHistogram(i); 

	}


	for( int ch = 0; ch < nchannel; ch++ ){

		for( int i = 0; i < nprocess; i++ ){

			yield[i][ch] = histo[i][ch] -> Integral(-1 ,-1);				

			//if(  yield[i][ch]<0 )  yield[i][ch] = 0; 

			eyield[i][ch] = sqrt( histo[i][ch] -> GetSumw2() -> GetSum() );	

			//if( eyield[i][ch]<0 ) eyield[i][ch] = 0;

			if( i != data && i != fakes ){

				 yield[i][ch] *= thelumi; 
				eyield[i][ch] *= thelumi; 

			}

			if( i == fakes ){

				 yield[i][ch] *= 1./15.; 
				eyield[i][ch] *= 1./15.; 

			}

		}

		 yield[TT][ch] *= ttSF; 
		eyield[TT][ch] *= ttSF;

 		 yield[DY][ch] *= DYSF; 
		eyield[DY][ch] *= DYSF;

	         yield[ttDM][ch] *= xs2l; 
		eyield[ttDM][ch] *= xs2l; 

	         yield[ttDM][ch] *= zoom; 
		eyield[ttDM][ch] *= zoom; 

		 allbkg[ch] = yield[WW][ch] + yield[WZ][ch] + yield[VZ][ch] + yield[VVV][ch]+ 
                              yield[DY][ch] + yield[TTV][ch]+ yield[TT][ch] + yield[ST][ch] + 
			      yield[fakes][ch] + yield[Zg][ch] + yield[Wg][ch] ;

		eallbkg[ch] = sqrt( pow( eyield[WW][ch], 2) + pow( eyield[WZ][ch], 2) + pow( eyield[VZ][ch], 2) + pow( eyield[VVV][ch], 2)+ 
				    pow( eyield[DY][ch], 2) + pow( eyield[TTV][ch], 2)+ pow( eyield[TT][ch], 2) + pow( eyield[ST][ch], 2) +
 				    pow( eyield[fakes][ch], 2) + pow( eyield[Zg][ch], 2) + pow( eyield[Wg][ch], 2) );    

	}

	gSystem -> mkdir( "yields/", kTRUE );

	ofstream mytable; 

	mytable.open( "yields/table.txt" );

	mytable << "\\hline \n"; 

	mytable << "process & ee & $\\mu\\mu$ & e$\\mu$ & $\\ell\\ell$ \\\\ \\hline \n"; 

        mytable << Form("WW & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[WW][ee], eyield[WW][ee], yield[WW][mm], eyield[WW][mm], yield[WW][em], eyield[WW][em], yield[WW][ll], eyield[WW][ll]);

        mytable << Form("WZ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[WZ][ee], eyield[WZ][ee], yield[WZ][mm], eyield[WZ][mm], yield[WZ][em], eyield[WZ][em], yield[WZ][ll], eyield[WZ][ll]);

        mytable << Form("VZ & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[VZ][ee], eyield[VZ][ee], yield[VZ][mm], eyield[VZ][mm], yield[VZ][em], eyield[VZ][em], yield[VZ][ll], eyield[VZ][ll]);

        mytable << Form("VVV & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[VVV][ee], eyield[VVV][ee], yield[VVV][mm], eyield[VVV][mm], yield[VVV][em], eyield[VVV][em], yield[VVV][ll], eyield[VVV][ll]);

        mytable << Form("Zg  & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[Zg][ee], eyield[Zg][ee], yield[Zg][mm], eyield[Zg][mm], yield[Zg][em], eyield[Zg][em], yield[Zg][ll], eyield[Zg][ll]);

        mytable << Form("Wg  & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \n", 
                              yield[Wg][ee], eyield[Wg][ee], yield[Wg][mm], eyield[Wg][mm], yield[Wg][em], eyield[Wg][em], yield[Wg][ll], eyield[Wg][ll]);

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

	mytable << Form("m$_{\\chi}$ 1 m$_{\\phi}$ 10 \\,\\, x%d & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f & %7.2f $\\pm$ %7.2f \\\\ \\hline \\hline \n", 
      zoom, yield[ttDM][ee], eyield[ttDM][ee], yield[ttDM][mm], eyield[ttDM][mm], yield[ttDM][em], eyield[ttDM][em], yield[ttDM][ll], eyield[ttDM][ll]);

	mytable << Form("data & %7.0f & %7.0f & %7.0f & %7.0f \\\\ \\hline \n", 
              yield[data][ee], yield[data][mm], yield[data][em], yield[data][ll] );



	mytable << "\n\n\n\n \t yields without statistical uncertainties \n\n"; 


	mytable << "\\begin{table}[htbp] \n"; 
	mytable << "\\centering \n"; 
	mytable << "\\caption{ } \n"; 
	mytable << "\\begin{tabular}{l|r|r|r|r} \n"; 

	mytable << "\\hline \n"; 

	mytable << "process & ee & $\\mu\\mu$ & e$\\mu$ & $\\ell\\ell$ \\\\ \\hline \n"; 

        mytable << Form("WW & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[WW][ee], yield[WW][mm], yield[WW][em], yield[WW][ll]);

        mytable << Form("WZ & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[WZ][ee], yield[WZ][mm], yield[WZ][em], yield[WZ][ll]);

        mytable << Form("VZ & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[VZ][ee], yield[VZ][mm], yield[VZ][em], yield[VZ][ll]);

        mytable << Form("VVV & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[VVV][ee], yield[VVV][mm], yield[VVV][em], yield[VVV][ll]);

        mytable << Form("Zg  & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[Zg][ee], yield[Zg][mm], yield[Zg][em], yield[Zg][ll]);

        mytable << Form("Wg  & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[Wg][ee], yield[Wg][mm], yield[Wg][em], yield[Wg][ll]);

        mytable << Form("Z+jets & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[DY][ee], yield[DY][mm], yield[DY][em], yield[DY][ll]);

        mytable << Form("$\\ttbar$V & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                      yield[TTV][ee], yield[TTV][mm], yield[TTV][em], yield[TTV][ll] );

        mytable << Form("$\\ttbar$ & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[TT][ee], yield[TT][mm], yield[TT][em], yield[TT][ll]);

        mytable << Form("tW & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \n", 
                              yield[ST][ee], yield[ST][mm], yield[ST][em], yield[ST][ll]);

        mytable << Form("non-prompt & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \\hline \n", 
      yield[fakes][ee], yield[fakes][mm], yield[fakes][em], yield[fakes][ll]);

	mytable << Form("total bkg & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \\hline \\hline \n", 
                                              allbkg[ee], allbkg[mm], allbkg[em], allbkg[ll]);

	mytable << Form("m$_{\\chi}$ 1 m$_{\\phi}$ 10 \\,\\, x%d & %7.2f   & %7.2f   & %7.2f   & %7.2f   \\\\ \\hline \\hline \n", 
      zoom, yield[ttDM][ee], yield[ttDM][mm], yield[ttDM][em], yield[ttDM][ll]);

	mytable << Form("data & %7.0f & %7.0f & %7.0f & %7.0f \\\\ \\hline \n", 
              yield[data][ee], yield[data][mm], yield[data][em], yield[data][ll] );

	mytable << "\\end{tabular} \n";
	mytable << "\\end{table} \n";

 	mytable.close();

}


void GetHistogram( int process ){

	cout << "\t\t" << processID[process] << "\n" << endl;

	TCanvas* c1 = new TCanvas("canvas", "the canvas");

	TFile* myfile = new TFile( storageSite + minitreeDir[0] + "/TTDM/" + processID[process] + ".root", "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );
		
	TCut ee_ch = "channel==3"; 
	TCut mm_ch = "channel==4"; 
	TCut em_ch = "channel==5"; 
	

	//---jefferson
	TCut RemovingFakes = "eventW_genmatched";
	TCut newselection = ( process == data || process == fakes  ) ? selection : selection&&RemovingFakes; 



	TCut ee_cut = ee_ch && newselection; 
	TCut mm_cut = mm_ch && newselection; 
	TCut em_cut = em_ch && newselection; 
        TCut ll_cut =          newselection;


	if ( process == TT )  eventW[0] ="eventW_Toppt" * eventW[0]; 

	ee_cut = eventW[0] * ee_cut; 
	mm_cut = eventW[0] * mm_cut; 
	em_cut = eventW[0] * em_cut; 
	ll_cut = eventW[0] * ll_cut; 


	mytree -> Draw( "metPfType1 >> h_ee", ee_cut );
	mytree -> Draw( "metPfType1 >> h_mm", mm_cut );
	mytree -> Draw( "metPfType1 >> h_em", em_cut );
	mytree -> Draw( "metPfType1 >> h_ll", ll_cut );

	histo[process][ee] = (TH1F*) gDirectory -> Get( "h_ee" );
	histo[process][mm] = (TH1F*) gDirectory -> Get( "h_mm" );
	histo[process][em] = (TH1F*) gDirectory -> Get( "h_em" );
	histo[process][ll] = (TH1F*) gDirectory -> Get( "h_ll" );
	
	//myfile->Close(); 

	c1->Destructor(); 

};


	

