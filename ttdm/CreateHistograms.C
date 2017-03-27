#include "ttdm.h"

const TString outputdir = "histos"; 

void CreateHistograms2( int process );


void CreateHistograms(){

	Assign();

	gSystem->mkdir( outputdir, kTRUE );

	//-----

	for( int i = 0; i < nprocess; i++ ){

		CreateHistograms2( i ); 

	}

//CreateHistograms2( data );
//CreateHistograms2( ttDM );
//CreateHistograms2( TT   );

	//for( int i = 0; i < nscalar; i++ ){

	//	CreateHistograms2( i ); 

	//}	

	//-----

	cout << "\n \n The End !!! \n \n" << endl; 

}


void CreateHistograms2( int process ){ 

	cout << "\n \t process: " << processID[process] << endl; 

	TCanvas* c1 = new TCanvas("canvas", "the canvas");

	//TFile* myfile = new TFile( "../minitrees/" + inputdir + "/" + processID[process] + ".root", "read" ); 
	TFile* myfile = new TFile( "/afs/cern.ch/user/c/cprieels/work/public/TTDM36fbMinitrees/" + processID[process] + ".root", "read" );
 
	for( int k = 0; k < nsystematic; k++ ){

		if( k > nominal ) continue; //toppTrw ) continue;

		cout << "\t\t systematic: " << systematicID[k] << endl;

		TFile* storagefile; 

		if( k == nominal ) storagefile = new TFile( outputdir + "/" + processID[process] +                         ".root", "recreate" );
		if( k >  nominal ) storagefile = new TFile( outputdir + "/" + processID[process] + "_" + systematicID[k] + ".root", "recreate" );

		TTree* mytree = (TTree*) myfile -> Get( "latino" );

		TCut thecut = mycut[k]; 

		if ( process == TT && k != toppTrw ) thecut = Form("         %4.2f", ttSF)*thecut; 
		if ( process == TT && k == toppTrw ) thecut = Form("toppTRwW*%4.2f", ttSF)*thecut; 
		if ( process == DY                 ) thecut = Form("         %4.2f", DYSF)*thecut; 

		/*if( (k >= QCDup && k <= PDFdo) && (process != data && process != ttDM && process != fakes && process != ST && process != HZ) ){

			TH1F* weights = (TH1F*) myfile -> Get( "list_vectors_weights" );

			if( k == QCDup ){

				float qcd_norm_up = weights->GetBinContent(9)/weights->GetBinContent(1);

				thecut = Form("(LHEweight[8]/LHEweight[0])*%7.4f", qcd_norm_up)*thecut;
			
			}	

			if( k == QCDdo ){

				float qcd_norm_do = weights->GetBinContent(5)/weights->GetBinContent(1);

				thecut = Form("(LHEweight[4]/LHEweight[0])*%7.4f", qcd_norm_do)*thecut;
			
			}

			if( k == PDFup ){

				float PDF_norm_up = weights->GetBinContent(10)/weights->GetBinContent(1);

				thecut = Form("(LHEweight[9]/LHEweight[0])*%7.4f", PDF_norm_up)*thecut;
			
			}	

			if( k == PDFdo ){

				float PDF_norm_do = weights->GetBinContent(11)/weights->GetBinContent(1);

				thecut = Form("(LHEweight[10]/LHEweight[0])*%7.4f", PDF_norm_do)*thecut;
			
			}

		}*/


		TString h_name[nhisto];

		for( int i = 0; i < nhisto; i++ ){ 

			if ( doshape == 0 ) h_name[i] =                                                    g_name[i]; 
			if ( doshape == 1 ) h_name[i] = processID[process] + "_" + systematicID[k] + "_" + g_name[i]; 

		}

		mytree -> Draw( b_name[lep1pt       ] + " >> " + h_name[lep1pt       ] + "( 3000,  0  , 3000   )", thecut );
		mytree -> Draw( b_name[lep1eta      ] + " >> " + h_name[lep1eta      ] + "(   60, -3  ,    3   )", thecut );
		mytree -> Draw( b_name[lep1phi      ] + " >> " + h_name[lep1phi      ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[lep1mass     ] + " >> " + h_name[lep1mass     ] + "(  100,  0  ,  100   )", thecut );

	 	mytree -> Draw( b_name[lep2pt       ] + " >> " + h_name[lep2pt       ] + "( 3000,  0  , 3000   )", thecut );
		mytree -> Draw( b_name[lep2eta      ] + " >> " + h_name[lep2eta      ] + "(   60, -3  ,    3   )", thecut );
		mytree -> Draw( b_name[lep2phi      ] + " >> " + h_name[lep2phi      ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[lep2mass     ] + " >> " + h_name[lep2mass     ] + "(  100,  0  ,  100   )", thecut );

		mytree -> Draw( b_name[jet1pt       ] + " >> " + h_name[jet1pt       ] + "( 3000,  0  , 3000   )", thecut );
		mytree -> Draw( b_name[jet1eta      ] + " >> " + h_name[jet1eta      ] + "(   60, -3  ,    3   )", thecut );
		mytree -> Draw( b_name[jet1phi      ] + " >> " + h_name[jet1phi      ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[jet1mass     ] + " >> " + h_name[jet1mass     ] + "(  100,  0  ,  100   )", thecut );

		mytree -> Draw( b_name[jet2pt       ] + " >> " + h_name[jet2pt       ] + "( 3000,  0  , 3000   )", thecut );
		mytree -> Draw( b_name[jet2eta      ] + " >> " + h_name[jet2eta      ] + "(   60, -3  ,    3   )", thecut );
		mytree -> Draw( b_name[jet2phi      ] + " >> " + h_name[jet2phi      ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[jet2mass     ] + " >> " + h_name[jet2mass     ] + "(  100,  0  ,  100   )", thecut );

		mytree -> Draw( b_name[metPfType1   ] + " >> " + h_name[metPfType1   ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[metPfType1Phi] + " >> " + h_name[metPfType1Phi] + "(  100,  0,     3.2  )", thecut );

		mytree -> Draw( b_name[m2l          ] + " >> " + h_name[m2l          ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[mt2ll        ] + " >> " + h_name[mt2ll        ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[mt2lblb      ] + " >> " + h_name[mt2lblb      ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[mtw1         ] + " >> " + h_name[mtw1         ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[mtw2         ] + " >> " + h_name[mtw2         ] + "( 3000,  0,   3000   )", thecut );

		mytree -> Draw( b_name[ht           ] + " >> " + h_name[ht           ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[htjets       ] + " >> " + h_name[htjets       ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[htnojets     ] + " >> " + h_name[htnojets     ] + "( 3000,  0,   3000   )", thecut );

		mytree -> Draw( b_name[njet         ] + " >> " + h_name[njet         ] + "(   10,  0,     10   )", thecut );
		mytree -> Draw( b_name[nbjet30csvv2l] + " >> " + h_name[nbjet30csvv2l] + "(   10,  0,     10   )", thecut );
		mytree -> Draw( b_name[nbjet30csvv2m] + " >> " + h_name[nbjet30csvv2m] + "(   10,  0,     10   )", thecut );
		mytree -> Draw( b_name[nbjet30csvv2t] + " >> " + h_name[nbjet30csvv2t] + "(   10,  0,     10   )", thecut );

		mytree -> Draw( b_name[dphijet1met  ] + " >> " + h_name[dphijet1met  ] + "(  100,  0,      3.2 )", thecut );   
		mytree -> Draw( b_name[dphijet2met  ] + " >> " + h_name[dphijet2met  ] + "(  100,  0,      3.2 )", thecut ); 
		mytree -> Draw( b_name[dphijj       ] + " >> " + h_name[dphijj       ] + "(  100,  0,      3.2 )", thecut );    
		mytree -> Draw( b_name[dphijjmet    ] + " >> " + h_name[dphijjmet    ] + "(  100,  0,      3.2 )", thecut );
	   	mytree -> Draw( b_name[dphill       ] + " >> " + h_name[dphill       ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilep1jet1 ] + " >> " + h_name[dphilep1jet1 ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilep1jet2 ] + " >> " + h_name[dphilep1jet2 ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilep2jet1 ] + " >> " + h_name[dphilep2jet1 ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilep2jet2 ] + " >> " + h_name[dphilep2jet2 ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilmet1    ] + " >> " + h_name[dphilmet1    ] + "(  100,  0,      3.2 )", thecut );
		mytree -> Draw( b_name[dphilmet2    ] + " >> " + h_name[dphilmet2    ] + "(  100,  0,      3.2 )", thecut ); 
		mytree -> Draw( b_name[dphillmet    ] + " >> " + h_name[dphillmet    ] + "(  100,  0,      3.2 )", thecut ); 

		mytree -> Draw( b_name[top1eta_gen  ] + " >> " + h_name[top1eta_gen  ] + "(   60, -3,      3   )", thecut );
		mytree -> Draw( b_name[top1phi_gen  ] + " >> " + h_name[top1phi_gen  ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[top1pt_gen   ] + " >> " + h_name[top1pt_gen   ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[top2eta_gen  ] + " >> " + h_name[top2eta_gen  ] + "(   60, -3,      3   )", thecut );
		mytree -> Draw( b_name[top2phi_gen  ] + " >> " + h_name[top2phi_gen  ] + "(  200, -3.2,    3.2 )", thecut );
		mytree -> Draw( b_name[top2pt_gen   ] + " >> " + h_name[top2pt_gen   ] + "( 3000,  0,   3000   )", thecut );
		mytree -> Draw( b_name[detatt_gen   ] + " >> " + h_name[detatt_gen   ] + "(   60,  0,      6   )", thecut );

		mytree -> Draw( b_name[nvtx         ] + " >> " + h_name[nvtx         ] + "(   60,  0,     60   )", thecut );

		//mytree -> Draw( b_name[sphericity   ] + " >> " + h_name[sphericity   ] + "(  200, -1,      1   )", thecut );
		//mytree -> Draw( b_name[alignment    ] + " >> " + h_name[alignment    ] + "(  200, -1,      1   )", thecut );
		//mytree -> Draw( b_name[planarity    ] + " >> " + h_name[planarity    ] + "(  200, -1,      1   )", thecut );

		//mytree -> Draw( b_name[darkpt       ] + " >> " + h_name[darkpt       ] + "( 310,  -100,3000   )", thecut );
		//mytree -> Draw( b_name[MVA80        ] + " >> " + h_name[MVA80        ] + "( 120,  -0.1, 1.1   )", thecut );
		//mytree -> Draw( b_name[MVA90        ] + " >> " + h_name[MVA90        ] + "( 120,  -0.1, 1.1   )", thecut );
		//mytree -> Draw( b_name[MVA100       ] + " >> " + h_name[MVA100       ] + "( 120,  -0.1, 1.1   )", thecut );

		//mytree -> Draw( b_name[alpha005    ] + " >> " + h_name[alpha005    ] + "( 120,  -0.1, 1.1   )", thecut );
		//mytree -> Draw( b_name[alpha01     ] + " >> " + h_name[alpha01     ] + "( 120,  -0.1, 1.1   )", thecut );
		//mytree -> Draw( b_name[alpha05     ] + " >> " + h_name[alpha05     ] + "( 120,  -0.1, 1.1   )", thecut );

		for( int i = 0; i < nhisto; i++ ){	

			myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );

		}


		for( int i = 0; i < nhisto; i++ ){	

			/*if( i == mt2ll )*/ myhisto[i] -> Write(); 

		}

		storagefile -> Close();

		

	}

	c1 -> Destructor();

	myfile -> Close();

}
