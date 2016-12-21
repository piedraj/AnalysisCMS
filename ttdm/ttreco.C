#include "/afs/cern.ch/user/j/jgarciaf/mimick/src/MassReconstructor.cc"
#include "TLorentzVector.h"
#include "TVector2.h"
#include <vector>


const TString  inputdir = "minitrees_week-1"; 

const bool WriteBranch = 1; 

const bool WriteBranch = 1; 

void ttreco2(TString process); 

void ttreco(){

	//ttreco2("00_Fakes_reduced"    );
	//ttreco2("01_Data_reduced"     );
	//ttreco2("02_WZTo3LNu" );
	//ttreco2("03_VZ"       );
	ttreco2("04_TTTo2L2Nu");
	//ttreco2("05_ST"       );
	//ttreco2("06_WW"       );
	//ttreco2("07_ZJets"    );
	//ttreco2("08_WJets"    );
	ttreco2("09_TTV"      );
	ttreco2("10_HWW"      );
	ttreco2("11_Wg"       );
	ttreco2("12_Zg"       );
	ttreco2("13_VVV"      );
	ttreco2("14_HZ"       );
	ttreco2("15_WgStar"   );

	ttreco2("ttDM0001scalar00010");
	ttreco2("ttDM0001scalar00050");
	ttreco2("ttDM0001scalar00500");
	ttreco2("ttDM0001scalar00020");
	ttreco2("ttDM0001scalar00100");
	ttreco2("ttDM0001scalar00200");
	ttreco2("ttDM0001scalar00300");
	ttreco2("00_Fakes_reduced_1outof6");
	ttreco2("01_Data_reduced_1outof6");
	cout << "\n \n done !!! \n \n" << endl; 

}

void ttreco2( TString process ) {

	cout << "\n\n" << process << "\n" <<  endl; 

	TFile* fshape  = new TFile( "/afs/cern.ch/user/j/jgarciaf/mimick/mlb.root" ); 

	TH1F* shapemlb = (TH1F*) fshape->Get( "mlb" );

	MassReconstructor theMass( 100, shapemlb );  


	//TFile myfile( "../minitrees/" + inputdir + "/TTDM/" + process + ".root", "update" ); 
	TFile myfile( "../../../../public/minitrees_week-1_with-ttReco/" + process + ".root", "update" ); 
	//TFile myfile( "../minitrees/week-1-checks/" + process + ".root", "update" ); 

   	TTreeReader myreader( "latino", &myfile );

	TTree* mytree = (TTree*) myfile.Get( "latino" );


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

   	TTreeReaderValue<std::vector<float>> bjet30csvv2m_pt ( myreader, "bjet30csvv2m_pt"  );
   	TTreeReaderValue<std::vector<float>> bjet30csvv2m_eta( myreader, "bjet30csvv2m_eta" );
   	TTreeReaderValue<std::vector<float>> bjet30csvv2m_phi( myreader, "bjet30csvv2m_phi" );

	//TTreeReaderValue<float> top1pt  ( myreader, "top1pt_gen"   );
   	//TTreeReaderValue<float> top1eta ( myreader, "top1eta_gen"  );
   	//TTreeReaderValue<float> top1phi ( myreader, "top1phi_gen"  );

	//TTreeReaderValue<float> top2pt  ( myreader, "top2pt_gen"   );
   	//TTreeReaderValue<float> top2eta ( myreader, "top2eta_gen"  );
   	//TTreeReaderValue<float> top2phi ( myreader, "top2phi_gen"  );


	//----- write ------------------------------------------------------

	float topRecoW;
	float darkpt  ; 

	TBranch* b_topRecoW = mytree -> Branch( "topRecoW", &topRecoW, "topRecoW/F" );
	TBranch* b_darkpt   = mytree -> Branch( "newdarkpt"  , &darkpt  , "darkpt/F"   );

	TBranch* b_mimick = mytree -> Branch( "mimick"  , &mimick  , "mimick/F"   );

	

	//----- loop -------------------------------------------------------


	int nentries = myreader.GetEntries(1); 

	//if ( nentries > 10 ) nentries = 10; 

	for ( Long64_t ievt = 0; ievt < nentries; ievt++ ) {

		if( ievt%10000 == 0 ) cout << "\n\n ievt: " << ievt << endl;

		myreader.SetEntry(ievt);

		//--- MET

		TVector2 MET;

		MET.SetMagPhi( *metPfType1, *metPfType1Phi );

		myreader.SetEntry(ievt);

		//--- leptons

		TLorentzVector l1, l2;   // top1gen, top2gen; 

		l1.SetPtEtaPhiM( *lep1pt, *lep1eta, *lep1phi, *lep1mass ); 
  		l2.SetPtEtaPhiM( *lep2pt, *lep2eta, *lep2phi, *lep2mass );

		//top1gen.SetPtEtaPhiM( *top1pt, *top1eta, *top1phi, 173.34 );
		//top2gen.SetPtEtaPhiM( *top2pt, *top2eta, *top2phi, 173.34 );


		//--- jets

		std::vector<TLorentzVector> jets;
 		std::vector<TLorentzVector> bjets;
		std::vector<Float_t>        unc;   // unimportant, but keep it, please

		for( int i = 0; i < jet_pt->size(); i++ ){

			TLorentzVector jet_tlv;

			jet_tlv.SetPtEtaPhiM( jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), 0. ); 

			jets.push_back(jet_tlv);

			unc.push_back(5.);   // GeV 

		}


		for( int i = 0; i < bjet30csvv2m_pt->size(); i++ ){

			TLorentzVector bjet30csvv2m_tlv;

			bjet30csvv2m_tlv.SetPtEtaPhiM( bjet30csvv2m_pt->at(i), bjet30csvv2m_eta->at(i), bjet30csvv2m_phi->at(i), 0. ); 

			bjets.push_back(bjet30csvv2m_tlv);

		}

		//--- arguments by reference: neutrinos & tops

		std::vector<TLorentzVector> nu1, nu2;

	        TVector2 top1, top2;


		theMass.startVariations( l1, l2, bjets, jets, MET, top1, top2, topRecoW );
		

		//--- 'rosquillas' reco

		if(  top1.X() == 0  &&  top1.Y() == 0  &&  top2.X() == 0  &&  top2.Y() == 0 ){

			int theJet1 = -1; 
			int theJet2 = -1; 

			darkpt = theMass.performAllVariations( 1, 1, 1, l1, l2, jets, unc, MET, nu1, nu2, theJet1, theJet2 ); 

		}
	
		else{

			darkpt = 0.; 
			//darkpt = (top1+top2).Mod(); 

		}


		//--- writing... 

		if (WriteBranch){

			b_topRecoW -> Fill(); 
			b_darkpt   -> Fill(); 
 
		}

	}   // ievt

	fshape->Close();

	if (WriteBranch) mytree -> Write( "", TObject::kOverwrite );

  	myfile.Close();


}


