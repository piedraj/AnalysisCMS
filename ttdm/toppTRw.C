const TString  inputdir = "diciembre";

const bool WriteBranch = 1; 

void toppTRw(){

	TFile myfile( "../minitrees/" + inputdir + "/04_TTTo2L2Nu.root", "update" ); 

	TTree* mytree = (TTree*) myfile.Get( "latino" );

	//----- read -------------------------------------------------------

	float top1pt_gen; mytree -> SetBranchAddress("top1pt_gen", &top1pt_gen); 
	float top2pt_gen; mytree -> SetBranchAddress("top2pt_gen", &top2pt_gen); 

	//----- write ------------------------------------------------------

	float toppTRwW;

	TBranch* b_toppTRwW = mytree -> Branch( "toppTRwW", &toppTRwW, "toppTRwW/F" );


	//----- loop -------------------------------------------------------

	int nentries = mytree->GetEntries(); 

	for ( Long64_t ievt = 0; ievt < nentries; ievt++ ) {

		if( ievt%10000 == 0 ) cout << "\n\n ievt: " << ievt << endl;

		mytree->GetEntry(ievt);

toppTRwW = ( top1pt_gen > 0. && top2pt_gen > 0. ) ? TMath::Sqrt(TMath::Exp(0.0615-0.0005*top1pt_gen)*TMath::Exp(0.0615-0.0005*top2pt_gen)) : 1.0;

		//cout << toppTRwW << endl;

		//--- writing... 

		if (WriteBranch){

			b_toppTRwW -> Fill(); 
 
		}

	}   // ievt

	if (WriteBranch) mytree -> Write( "", TObject::kOverwrite );

  	myfile.Close();

}


