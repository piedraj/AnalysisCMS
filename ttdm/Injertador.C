enum { mt2ll, metPfType1, dphillmet, darkpt, lep1pt, lep2pt, nbranch };

TString branchID[nbranch] = { "mt2ll", "metPfType1", "dphillmet", "darkpt", "lep1pt", "lep2pt"};

const TString whichsyst = "METup";

void Injertador2( TString process );

void Injertador(){

	Injertador2("TTTo2L2Nu_ext1__part0");

	//Injertador2("00_Fakes"    );
	//Injertador2("01_Data"     );
	//Injertador2("02_WZTo3LNu" );
	//Injertador2("03_VZ"       );
	//Injertador2("04_TTTo2L2Nu");
	//Injertador2("05_ST"       );
	//Injertador2("06_WW"       );
	//Injertador2("07_ZJets"    );
	//Injertador2("09_TTV"      );
	//Injertador2("10_HWW"      );
	//Injertador2("11_Wg"       );
	//Injertador2("12_Zg"       );
	//Injertador2("13_VVV"      );
	//Injertador2("14_HZ"       );
	//Injertador2("15_WgStar"   );

	//Injertador2("ttDM0001scalar00010");
	//Injertador2("ttDM0001scalar00050");
	//Injertador2("ttDM0001scalar00500");

}

void Injertador2( TString process ){ 

	float      inj_v[nbranch]; 
	float      pie_v[nbranch]; 
	TBranch*   pie_b[nbranch];


	TFile* inj_f = new TFile("../minitrees/" + whichsyst + "/TTDM/" + process + ".root" , "read"  );

	TTree* inj_t = (TTree*) inj_f -> Get( "latino" );

	inj_t -> SetBranchStatus( "*", 0 );


	for( int i = 0; i < nbranch; i++ ){

		inj_t -> SetBranchStatus( branchID[i], 1 ); inj_t -> SetBranchAddress( branchID[i], &inj_v[i] );

	}


	TFile* pie_f = new TFile("../minitrees/week-8/TTDM/" + process + ".root", "update"); 

	TTree* pie_t = (TTree*) pie_f -> Get( "latino" );

	for( int i = 0; i < nbranch; i++ ){

		pie_b[i] = pie_t -> Branch( branchID[i] + "_" + whichsyst, &pie_v[i], branchID[i] + "_" + whichsyst + "/F" ); 

	}

		
	int nentries = inj_t -> GetEntries();

	for( int ievt = 0; ievt < nentries; ievt++ ){

		if( ievt%10000 == 0 ) cout << "  >> " << ievt << " event" << endl;

		inj_t -> GetEntry( ievt ); 

		for( int i = 0; i < nbranch; i++ ){

			pie_v[i] = inj_v[i];

			pie_b[i] -> Fill(); 

		}


	}

	inj_f -> Close();		
	
	pie_t -> Write( "" );
	pie_f -> Close(); 

}
