void SplitMiniTree() {

	TFile* origin = new TFile("../minitrees/nominal/TTDM/04_TTTo2L2Nu.root", "READ");
	TTree* tree = (TTree*) origin -> Get( "latino" );


	TFile* f_training = new TFile("../minitrees/nominal/TTDM/training.root", "RECREATE");

	TTree* t_training = tree -> CloneTree(0);


	TFile* f_application = new TFile("../minitrees/nominal/TTDM/application.root", "RECREATE");

	TTree* t_application = tree -> CloneTree(0);


	for ( int ievt = 0; ievt < tree->GetEntries(); ievt++ ) {

		tree -> GetEntry(ievt);

		if ( ievt < 460 ) {

			t_training    -> Fill();

		} 

		else {

			t_application -> Fill();

		}

	}


	f_training    -> Write();

	f_application -> Write();

}
