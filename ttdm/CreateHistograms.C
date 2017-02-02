#include "ttdm.h"

const TString outputdir = "histos"; 

void CreateHistograms2( TString process );


void CreateHistograms(){

	Assign();

	gSystem->mkdir( outputdir, kTRUE );

	//-----

	for( int i = 0; i < nprocess; i++ ){

		CreateHistograms2( processID[i] ); 

	}

	for( int i = 0; i < nscalar; i++ ){

		CreateHistograms2( scalarID[i] ); 

	}	

	//-----

	cout << "\n \n The End !!! \n \n" << endl; 

}


void CreateHistograms2( TString process ){ 

	cout << "\n \t process: " << process << endl; 

	TCanvas* c1 = new TCanvas("canvas", "the canvas");

	TFile* myfile = new TFile( "../minitrees/" + inputdir + "/" + process + ".root", "read" ); 

	for( int k = 0; k < nsystematic; k++ ){

		cout << "\t\t systematic: " << systematicID[k] << endl;

		TFile* storagefile; 

		if( k == nominal ) storagefile = new TFile( outputdir + "/" + process +                         ".root", "recreate" );
		if( k >  nominal ) storagefile = new TFile( outputdir + "/" + process + "_" + systematicID[k] + ".root", "recreate" );

		TTree* mytree = (TTree*) myfile -> Get( "latino" );

		if ( process == "04_TTTo2L2Nu" ) mycut[k] = Form("%4.2f", ttSF)*mycut[k]; 
		if ( process == "07_ZJets"     ) mycut[k] = Form("%4.2f", DYSF)*mycut[k]; 

		mytree -> Draw( b_name[lep1pt       ] + " >> " + h_name[lep1pt       ] + "( 3000,  0  , 3000   )", mycut[k] );
		mytree -> Draw( b_name[lep1eta      ] + " >> " + h_name[lep1eta      ] + "(   60, -3  ,    3   )", mycut[k] );
		mytree -> Draw( b_name[lep1phi      ] + " >> " + h_name[lep1phi      ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[lep1mass     ] + " >> " + h_name[lep1mass     ] + "(  100,  0  ,  100   )", mycut[k] );

	 	mytree -> Draw( b_name[lep2pt       ] + " >> " + h_name[lep2pt       ] + "( 3000,  0  , 3000   )", mycut[k] );
		mytree -> Draw( b_name[lep2eta      ] + " >> " + h_name[lep2eta      ] + "(   60, -3  ,    3   )", mycut[k] );
		mytree -> Draw( b_name[lep2phi      ] + " >> " + h_name[lep2phi      ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[lep2mass     ] + " >> " + h_name[lep2mass     ] + "(  100,  0  ,  100   )", mycut[k] );

		mytree -> Draw( b_name[jet1pt       ] + " >> " + h_name[jet1pt       ] + "( 3000,  0  , 3000   )", mycut[k] );
		mytree -> Draw( b_name[jet1eta      ] + " >> " + h_name[jet1eta      ] + "(   60, -3  ,    3   )", mycut[k] );
		mytree -> Draw( b_name[jet1phi      ] + " >> " + h_name[jet1phi      ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[jet1mass     ] + " >> " + h_name[jet1mass     ] + "(  100,  0  ,  100   )", mycut[k] );

		mytree -> Draw( b_name[jet2pt       ] + " >> " + h_name[jet2pt       ] + "( 3000,  0  , 3000   )", mycut[k] );
		mytree -> Draw( b_name[jet2eta      ] + " >> " + h_name[jet2eta      ] + "(   60, -3  ,    3   )", mycut[k] );
		mytree -> Draw( b_name[jet2phi      ] + " >> " + h_name[jet2phi      ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[jet2mass     ] + " >> " + h_name[jet2mass     ] + "(  100,  0  ,  100   )", mycut[k] );

		mytree -> Draw( b_name[metPfType1   ] + " >> " + h_name[metPfType1   ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[metPfType1Phi] + " >> " + h_name[metPfType1Phi] + "(  100,  0,     3.2  )", mycut[k] );

		mytree -> Draw( b_name[m2l          ] + " >> " + h_name[m2l          ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[mt2ll        ] + " >> " + h_name[mt2ll        ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[mt2lblb      ] + " >> " + h_name[mt2lblb      ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[mtw1         ] + " >> " + h_name[mtw1         ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[mtw2         ] + " >> " + h_name[mtw2         ] + "( 3000,  0,   3000   )", mycut[k] );

		mytree -> Draw( b_name[ht           ] + " >> " + h_name[ht           ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[htjets       ] + " >> " + h_name[htjets       ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[htnojets     ] + " >> " + h_name[htnojets     ] + "( 3000,  0,   3000   )", mycut[k] );

		mytree -> Draw( b_name[njet         ] + " >> " + h_name[njet         ] + "(   10,  0,     10   )", mycut[k] );
		mytree -> Draw( b_name[nbjet30csvv2l] + " >> " + h_name[nbjet30csvv2l] + "(   10,  0,     10   )", mycut[k] );
		mytree -> Draw( b_name[nbjet30csvv2m] + " >> " + h_name[nbjet30csvv2m] + "(   10,  0,     10   )", mycut[k] );
		mytree -> Draw( b_name[nbjet30csvv2t] + " >> " + h_name[nbjet30csvv2t] + "(   10,  0,     10   )", mycut[k] );

		mytree -> Draw( b_name[dphijet1met  ] + " >> " + h_name[dphijet1met  ] + "(  100,  0,      3.2 )", mycut[k] );   
		mytree -> Draw( b_name[dphijet2met  ] + " >> " + h_name[dphijet2met  ] + "(  100,  0,      3.2 )", mycut[k] ); 
		mytree -> Draw( b_name[dphijj       ] + " >> " + h_name[dphijj       ] + "(  100,  0,      3.2 )", mycut[k] );    
		mytree -> Draw( b_name[dphijjmet    ] + " >> " + h_name[dphijjmet    ] + "(  100,  0,      3.2 )", mycut[k] );
	   	mytree -> Draw( b_name[dphill       ] + " >> " + h_name[dphill       ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilep1jet1 ] + " >> " + h_name[dphilep1jet1 ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilep1jet2 ] + " >> " + h_name[dphilep1jet2 ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilep2jet1 ] + " >> " + h_name[dphilep2jet1 ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilep2jet2 ] + " >> " + h_name[dphilep2jet2 ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilmet1    ] + " >> " + h_name[dphilmet1    ] + "(  100,  0,      3.2 )", mycut[k] );
		mytree -> Draw( b_name[dphilmet2    ] + " >> " + h_name[dphilmet2    ] + "(  100,  0,      3.2 )", mycut[k] ); 
		mytree -> Draw( b_name[dphillmet    ] + " >> " + h_name[dphillmet    ] + "(  100,  0,      3.2 )", mycut[k] ); 

		mytree -> Draw( b_name[top1eta_gen  ] + " >> " + h_name[top1eta_gen  ] + "(   60, -3,      3   )", mycut[k] );
		mytree -> Draw( b_name[top1phi_gen  ] + " >> " + h_name[top1phi_gen  ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[top1pt_gen   ] + " >> " + h_name[top1pt_gen   ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[top2eta_gen  ] + " >> " + h_name[top2eta_gen  ] + "(   60, -3,      3   )", mycut[k] );
		mytree -> Draw( b_name[top2phi_gen  ] + " >> " + h_name[top2phi_gen  ] + "(  200, -3.2,    3.2 )", mycut[k] );
		mytree -> Draw( b_name[top2pt_gen   ] + " >> " + h_name[top2pt_gen   ] + "( 3000,  0,   3000   )", mycut[k] );
		mytree -> Draw( b_name[detatt_gen   ] + " >> " + h_name[detatt_gen   ] + "(   60,  0,      6   )", mycut[k] );

		mytree -> Draw( b_name[nvtx         ] + " >> " + h_name[nvtx         ] + "(   60,  0,     60   )", mycut[k] );

		mytree -> Draw( b_name[sphericity   ] + " >> " + h_name[sphericity   ] + "(  200, -1,      1   )", mycut[k] );
		mytree -> Draw( b_name[alignment    ] + " >> " + h_name[alignment    ] + "(  200, -1,      1   )", mycut[k] );
		mytree -> Draw( b_name[planarity    ] + " >> " + h_name[planarity    ] + "(  200, -1,      1   )", mycut[k] );

		//mytree -> Draw( b_name[darkpt       ] + " >> " + h_name[darkpt       ] + "( 310,  -100,3000   )", thecut );
		//mytree -> Draw( b_name[mva01        ] + " >> " + h_name[mva01        ] + "( 120,  -0.1, 1.1   )", thecut );


		for( int i = 0; i < nhisto; i++ ){	

			myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );

		}


		for( int i = 0; i < nhisto; i++ ){	

			myhisto[i] -> Write(); 

		}

		storagefile -> Close();

		

	}

	c1 -> Destructor();

	myfile -> Close();

}
