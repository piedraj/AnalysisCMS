enum{ lep1pt, lep1eta, lep1phi, lep1mass,
      lep2pt, lep2eta, lep2phi, lep2mass,
      jet1pt, jet1eta, jet1phi, jet1mass,
      jet2pt, jet2eta, jet2phi, jet2mass,
      metPfType1,
      dphijet1met, dphijet2met, dphijj, dphijjmet, dphilep1jet1, dphilep1jet2, dphilep2jet1, dphilep2jet2, dphilmet1, dphilmet2,
      nhisto
 }; 

TString b_name[nhisto];
TString h_name[nhisto];
TH1F* myhisto [nhisto];


const TString skimming = "161001/TTDM/";  // where the minitrees are stored

const TCut mycut = "eventW*(metPfType1>60)";  // the cuts chain 


void ConvertToHistograms( TString process );



void PlotFromMinitree(){

 	b_name[lep1pt  ] = "lep1pt"  ;
	b_name[lep1eta ] = "lep1eta" ;
	b_name[lep1phi ] = "lep1phi" ;
	b_name[lep1mass] = "lep1mass";
 	b_name[lep2pt  ] = "lep2pt"  ;
	b_name[lep2eta ] = "lep2eta" ;
	b_name[lep2phi ] = "lep2phi" ;
	b_name[lep2mass] = "lep2mass";
	b_name[jet1pt  ] = "jet1pt"  ;
	b_name[jet1eta ] = "jet1eta" ;
	b_name[jet1phi ] = "jet1phi" ;
	b_name[jet1mass] = "jet1mass";
	b_name[jet2pt  ] = "jet2pt"  ;
	b_name[jet2eta ] = "jet2eta" ;
	b_name[jet2phi ] = "jet2phi" ;
	b_name[jet2mass] = "jet2mass";

	b_name[metPfType1] = "metPfType1";

	b_name[dphijet1met ] = "dphijet1met" ;   
	b_name[dphijet2met ] = "dphijet2met" ;  
	b_name[dphijj      ] = "dphijj"      ;    
	b_name[dphijjmet   ] = "dphijjmet"   ;   
	b_name[dphilep1jet1] = "dphilep1jet1";
	b_name[dphilep1jet2] = "dphilep1jet2";
	b_name[dphilep2jet1] = "dphilep2jet1";
	b_name[dphilep2jet2] = "dphilep2jet2";
	b_name[dphilmet1   ] = "dphilmet1"   ;
	b_name[dphilmet2   ] = "dphilmet2"   ;


	for( int i = 0; i < nhisto; i++ ){

 		h_name[i] = b_name[i];

	}


	ConvertToHistograms("00_Fakes"    );
	ConvertToHistograms("01_Data"     );
	ConvertToHistograms("02_WZTo3LNu" );
	ConvertToHistograms("03_VZ"       );
	ConvertToHistograms("04_TTTo2L2Nu");
	ConvertToHistograms("05_ST"       );
	ConvertToHistograms("06_WW"       );
	ConvertToHistograms("07_ZJets"    );
	//ConvertToHistograms("08_WJets"    );
	ConvertToHistograms("09_TTV"      );
	ConvertToHistograms("10_HWW"      );
	ConvertToHistograms("11_Wg"       );
	ConvertToHistograms("12_Zg"       );
	ConvertToHistograms("13_VVV"      );
	ConvertToHistograms("14_HZ"       );
	ConvertToHistograms("15_WgStar"   );
	
	cout << "\n \n yeah \n \n" << endl; 

}


void ConvertToHistograms( TString process ){ 

	TFile* myfile = new TFile( "minitrees/" + skimming + process + ".root", "read" ); 
	
	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	TFile* storagefile = new TFile("new-histograms/" + process + ".root", "update");

	mytree -> Draw( b_name[lep1pt  ] + " >> " + h_name[lep1pt  ] + "(3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[lep1eta ] + " >> " + h_name[lep1eta ] + "(  60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[lep1phi ] + " >> " + h_name[lep1phi ] + "( 200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[lep1mass] + " >> " + h_name[lep1mass] + "( 100,  0  ,  100   )", mycut );
 	mytree -> Draw( b_name[lep2pt  ] + " >> " + h_name[lep2pt  ] + "(3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[lep2eta ] + " >> " + h_name[lep2eta ] + "(  60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[lep2phi ] + " >> " + h_name[lep2phi ] + "( 200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[lep2mass] + " >> " + h_name[lep2mass] + "( 100,  0  ,  100   )", mycut );
	mytree -> Draw( b_name[jet1pt  ] + " >> " + h_name[jet1pt  ] + "(3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[jet1eta ] + " >> " + h_name[jet1eta ] + "(  60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[jet1phi ] + " >> " + h_name[jet1phi ] + "( 200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[jet1mass] + " >> " + h_name[jet1mass] + "( 100,  0  ,  100   )", mycut );
	mytree -> Draw( b_name[jet2pt  ] + " >> " + h_name[jet2pt  ] + "(3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[jet2eta ] + " >> " + h_name[jet2eta ] + "(  60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[jet2phi ] + " >> " + h_name[jet2phi ] + "( 200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[jet2mass] + " >> " + h_name[jet2mass] + "( 100,  0  ,  100   )", mycut );

	mytree -> Draw( b_name[metPfType1] + " >> " + h_name[metPfType1] + "(3000, 0, 3000)", mycut );

	mytree -> Draw( b_name[dphijet1met ] + " >> " + h_name[dphijet1met ] + "(100, 0, 3.2)", mycut );   
	mytree -> Draw( b_name[dphijet2met ] + " >> " + h_name[dphijet2met ] + "(100, 0, 3.2)", mycut );  
	mytree -> Draw( b_name[dphijj      ] + " >> " + h_name[dphijj      ] + "(100, 0, 3.2)", mycut );    
	mytree -> Draw( b_name[dphijjmet   ] + " >> " + h_name[dphijjmet   ] + "(100, 0, 3.2)", mycut );   
	mytree -> Draw( b_name[dphilep1jet1] + " >> " + h_name[dphilep1jet1] + "(100, 0, 3.2)", mycut );
	mytree -> Draw( b_name[dphilep1jet2] + " >> " + h_name[dphilep1jet2] + "(100, 0, 3.2)", mycut );
	mytree -> Draw( b_name[dphilep2jet1] + " >> " + h_name[dphilep2jet1] + "(100, 0, 3.2)", mycut );
	mytree -> Draw( b_name[dphilep2jet2] + " >> " + h_name[dphilep2jet2] + "(100, 0, 3.2)", mycut );
	mytree -> Draw( b_name[dphilmet1   ] + " >> " + h_name[dphilmet1   ] + "(100, 0, 3.2)", mycut );
	mytree -> Draw( b_name[dphilmet2   ] + " >> " + h_name[dphilmet2   ] + "(100, 0, 3.2)", mycut ); 


	for( int i = 0; i < nhisto; i++ ){	

		myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );

		myhisto[i] -> Write(); 

	}

	storagefile -> Close();

}
