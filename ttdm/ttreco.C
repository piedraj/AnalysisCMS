#include "../top-reco/src/MassVariations.cc"
#include "TLorentzVector.h"
#include "TVector2.h"
#include <vector>

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"

const TString  inputdir = "week-1";  // where the minitrees are stored

void ttreco2(TString process); 

void ttreco(){

	//ttreco2("00_Fakes"    );
	//ttreco2("01_Data"     );
	//ttreco2("02_WZTo3LNu" );
	//ttreco2("03_VZ"       );
	//ttreco2("04_TTTo2L2Nu");
	//ttreco2("05_ST"       );
	//ttreco2("06_WW"       );
	//ttreco2("07_ZJets"    );
	//ttreco2("08_WJets"    );
	//ttreco2("09_TTV"      );
	//ttreco2("10_HWW"      );
	//ttreco2("11_Wg"       );
	//ttreco2("12_Zg"       );
	//ttreco2("13_VVV"      );
	//ttreco2("14_HZ"       );
	//ttreco2("15_WgStar"   );*/

	ttreco2("ttDM0001scalar00010");
	//ttreco2("ttDM0001scalar00500");

	cout << "\n \n done !!! \n \n" << endl; 

}

void ttreco2( TString process ) {

	//TFile myfile( "../minitrees/" + inputdir + "/TTDM/" + process + ".root", "read" ); 
	TFile myfile( process + ".root", "update" ); 

   	TTreeReader myreader( "latino", &myfile );

	TTree*      mytree = (TTree*) myfile.Get( "latino" );


	//----- read -------------------------------------------------------

   	TTreeReaderValue<float> metPfType1   ( myreader, "metPfType1"    );
   	TTreeReaderValue<float> metPfType1Phi( myreader, "metPfType1Phi" );

   	TTreeReaderValue<float> lep1pt  ( myreader, "lep1pt"   );
   	TTreeReaderValue<float> lep1eta ( myreader, "lep1eta"  );
   	TTreeReaderValue<float> lep1phi ( myreader, "lep1phi"  );
   	TTreeReaderValue<float> lep1mass( myreader, "lep1mass" );

	TTreeReaderValue<float> lep2pt  ( myreader, "lep2pt"   );
   	TTreeReaderValue<float> lep2eta ( myreader, "lep2eta"  );
   	TTreeReaderValue<float> lep2phi ( myreader, "lep2phi"  );
   	TTreeReaderValue<float> lep2mass( myreader, "lep2mass" );

   	TTreeReaderValue<std::vector<float>> jet_pt ( myreader, "jet_pt"  );
   	TTreeReaderValue<std::vector<float>> jet_eta( myreader, "jet_eta" );
   	TTreeReaderValue<std::vector<float>> jet_phi( myreader, "jet_phi" );


	//----- write ------------------------------------------------------

	int nsol; 

	TBranch* b_nsol = mytree -> Branch( "nsol", &nsol, "nsol/I" );



	//----- loop -------------------------------------------------------

	int nentries = myreader.GetEntries(1); 

	for ( Long64_t ievt = 0; ievt < nentries; ievt++ ) {

		myreader.SetEntry(ievt);


		TVector2 MET; 

		MET.SetMagPhi( *metPfType1, *metPfType1Phi ); 


		TLorentzVector l1, l2; 

		l1.SetPtEtaPhiM( *lep1pt, *lep1eta, *lep1phi, *lep1mass ); 
  		l2.SetPtEtaPhiM( *lep2pt, *lep2eta, *lep2phi, *lep2mass );


		std::vector<TLorentzVector> jets; 
		std::vector<Float_t>        unc;
 
		for( int i = 0; i < jet_pt->size(); i++ ){

			TLorentzVector jet_tlv;

			jet_tlv.SetPtEtaPhiM( jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), 0. ); 

			jets.push_back(jet_tlv);

			unc.push_back(5.);   // GeV 

		}


		std::vector<TLorentzVector> nu1, nu2;

		
		MassVariations theMass;

		theMass.performAllVariations( /*1,*/ 10, 1, 1, l1, l2, jets, unc, MET, nu1, nu2 );

		nsol = nu1.size(); 

		b_nsol -> Fill();
		
	}

	mytree -> Write();

  	myfile.Close();

}


