#include "ttdm.h"

const bool WriteBranch = 1; 

TH1F* h_pu;

void insertPUrw2( int process ); 

void LoadHistogram(); 


void insertPUrw(){

	Assign();

	LoadHistogram(); 

	//insertPUrw2( WZ ); 
	//insertPUrw2( VZ ); 
	//insertPUrw2( TT ); 
	//insertPUrw2( ST ); 
	//insertPUrw2( WW ); 
	//insertPUrw2( DY ); 
	//insertPUrw2( VVV );
	insertPUrw2( ttDM ); 

	//for( int i = 0; i < nprocess; i++ ){

	//	insertPUrw2( i ); 

	//}

}

void insertPUrw2( int process ){

	cout << "\n\n" << processID[process] << "\n" << endl; 	

	WriteBranch == 1    ?    cout << "\t\t ***** writing... ***** \n" << endl    :    cout << "\t\t ***** not writing... ***** \n" << endl; 
	

	for( int k = nominal; k <= nominal; k++ ){ 

		cout << "\t systematic: " << systematicID[k] << endl;

		TFile myfile( storageSite + minitreeDir[k] + "/TTDM/" + processID[process] + ".root", "update" );

		TTree* mytree = (TTree*) myfile.Get( "latino" );


		//----- read -------------------------------------------------------

		float nvtx; mytree -> SetBranchAddress("nvtx", &nvtx); 


		float new_puW;

		TBranch* b_new_puW = mytree -> Branch( "new_puW", &new_puW, "new_puW/F" );


		//----- loop -------------------------------------------------------

		int nentries = mytree->GetEntries(); 

		for ( Long64_t ievt = 0; ievt < nentries; ievt++ ) {
		//for ( Long64_t ievt = 0; ievt < 1; ievt++ ) {

			if( ievt%10000 == 0 ) cout << "\t\t ievt: " << ievt << endl;

			mytree->GetEntry(ievt);

			if( nvtx > 29 ) nvtx = 29; 


			if( process == data ){ 

				new_puW = 1.0;

			}

			else{

				new_puW = h_pu -> GetBinContent( h_pu->FindBin(nvtx) );

			}

			//cout << nvtx << " --- " << new_puW << endl;

			//--- writing... 

			if (WriteBranch){

				b_new_puW -> Fill(); 
	 
			}

		} cout << "" << endl;   // ievt

		if (WriteBranch) mytree -> Write( "", TObject::kOverwrite );

	  	myfile.Close();
	
	}   // k

}


void LoadHistogram(){

	TFile* f_pu = new TFile( "newPU-rw.root", "read");

	h_pu = (TH1F*) f_pu -> Get( "ratio" );

}

