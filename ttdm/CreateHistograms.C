// root -l -b -q CreateHistograms

#include "../include/Constants.h"

enum{ lep1pt, lep1eta, lep1phi, lep1mass,
      lep2pt, lep2eta, lep2phi, lep2mass,
      jet1pt, jet1eta, jet1phi, jet1mass,
      jet2pt, jet2eta, jet2phi, jet2mass, 
      metPfType1, metPfType1Phi,
      m2l, mt2ll, mt2lblb, mtw1, mtw2,
      ht, htjets, htnojets,
      njet, nbjet30csvv2l, nbjet30csvv2m, nbjet30csvv2t,  
      dphijet1met, dphijet2met, dphijj, dphijjmet, dphill, dphilep1jet1, dphilep1jet2, dphilep2jet1, dphilep2jet2, dphilmet1, dphilmet2, dphillmet,	
      top1eta_gen, top1phi_gen, top1pt_gen, top2eta_gen, top2phi_gen, top2pt_gen, detatt_gen,  
      nvtx,
      sphericity, alignment, planarity,
      topRecoW, darkpt,
      mva01, 
      nhisto }; 

TString b_name[nhisto];
TString h_name[nhisto];
TH1F* myhisto [nhisto];

const TString inputdir = "/afs/cern.ch/work/j/jgarciaf/public/minitrees_week-1_with-ttReco/";  // where the minitrees are stored
 //const TString inputdir = "minitrees_week-1";  // where the minitrees are stored
const TString outputdir = "histos"; 

ofstream yields;   

const TCut mycut = "eventW*((channel == 5) && metPfType1 > 40.)"; 
//const TCut mycut = "eventW*((channel==5) && dphillmet > 1.2 && metPfType1 > 50.)";  // the cuts chain 
//channels: ee=3, mm=4, em=5

void CreateHistograms2( TString process );

void CreateHistograms(){

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

	b_name[metPfType1   ] = "metPfType1";
	b_name[metPfType1Phi] = "metPfType1Phi";

	b_name[m2l    ] = "m2l"    ;
	b_name[mt2ll  ] = "mt2ll"  ;
	b_name[mt2lblb] = "mt2lblb";
	b_name[mtw1   ] = "mtw1"   ;
	b_name[mtw2   ] = "mtw2"   ;

	b_name[ht      ] = "ht"      ;
	b_name[htjets  ] = "htjets"  ;
	b_name[htnojets] = "htnojets";

	b_name[njet         ] = "njet"         ;
	b_name[nbjet30csvv2l] = "nbjet30csvv2l";
	b_name[nbjet30csvv2m] = "nbjet30csvv2m";
	b_name[nbjet30csvv2t] = "nbjet30csvv2t";

	b_name[dphijet1met ] = "dphijet1met" ;   
	b_name[dphijet2met ] = "dphijet2met" ;  
	b_name[dphijj      ] = "dphijj"      ;    
	b_name[dphijjmet   ] = "dphijjmet"   ;   
	b_name[dphill      ] = "dphill"      ;   
	b_name[dphilep1jet1] = "dphilep1jet1";
	b_name[dphilep1jet2] = "dphilep1jet2";
	b_name[dphilep2jet1] = "dphilep2jet1";
	b_name[dphilep2jet2] = "dphilep2jet2";
	b_name[dphilmet1   ] = "dphilmet1"   ;
	b_name[dphilmet2   ] = "dphilmet2"   ;
	b_name[dphillmet   ] = "dphillmet"   ;

	b_name[top1eta_gen ] = "top1eta_gen" ;
	b_name[top1phi_gen ] = "top1phi_gen" ;
	b_name[top1pt_gen  ] = "top1pt_gen"  ;
	b_name[top2eta_gen ] = "top2eta_gen" ;
	b_name[top2phi_gen ] = "top2phi_gen" ;
	b_name[top2pt_gen  ] = "top2pt_gen"  ;
	b_name[detatt_gen  ] = "detatt_gen"  ;

	b_name[nvtx        ] = "nvtx"        ;

	b_name[sphericity] = "sphericity";
	b_name[alignment ] = "alignment" ;
	b_name[planarity ] = "planarity" ;

	b_name[topRecoW  ] = "topRecoW"  ;
	b_name[darkpt    ] = "darkpt"    ;

	b_name[mva01     ] = "mva01_ttDM0001scalar00500";

	for( int i = 0; i < nhisto; i++ ){

 		h_name[i] = b_name[i];

	}

	yields.open( outputdir + "/yields.txt" );

	CreateHistograms2("00_Fakes"    );
	CreateHistograms2("01_Data"     );
	CreateHistograms2("02_WZTo3LNu" );
	CreateHistograms2("03_VZ"       );
	CreateHistograms2("04_TTTo2L2Nu");
	//CreateHistograms2("TTTo2L2Nu_alphaS01108");
	CreateHistograms2("05_ST"       );
	CreateHistograms2("06_WW"       );
	CreateHistograms2("07_ZJets"    );
	CreateHistograms2("09_TTV"      );
	CreateHistograms2("10_HWW"      );
	CreateHistograms2("11_Wg"       );
	CreateHistograms2("12_Zg"       );
	CreateHistograms2("13_VVV"      );
	CreateHistograms2("14_HZ"       );
	CreateHistograms2("15_WgStar"   );

	//CreateHistograms2("ttDM0001scalar00010");
	//CreateHistograms2("ttDM0001scalar00300");
	CreateHistograms2("ttDM0001scalar00500");

	cout << "\n \n The End !!! \n \n" << endl; 

	yields.close();

}


void CreateHistograms2( TString process ){ 

        //TFile* myfile = new TFile( "../minitrees/" + inputdir + "/TTDM/" + process + ".root", "read" ); 
        TFile* myfile = new TFile(inputdir + process + ".root", "read"); 

	cout << "\n\n" << process << "\n" << endl; 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	gSystem->mkdir(outputdir, kTRUE);

	TFile* storagefile = new TFile( outputdir + "/" + process + ".root", "update" );

	mytree -> Draw( b_name[lep1pt       ] + " >> " + h_name[lep1pt       ] + "( 3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[lep1eta      ] + " >> " + h_name[lep1eta      ] + "(   60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[lep1phi      ] + " >> " + h_name[lep1phi      ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[lep1mass     ] + " >> " + h_name[lep1mass     ] + "(  100,  0  ,  100   )", mycut );

 	mytree -> Draw( b_name[lep2pt       ] + " >> " + h_name[lep2pt       ] + "( 3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[lep2eta      ] + " >> " + h_name[lep2eta      ] + "(   60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[lep2phi      ] + " >> " + h_name[lep2phi      ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[lep2mass     ] + " >> " + h_name[lep2mass     ] + "(  100,  0  ,  100   )", mycut );

	mytree -> Draw( b_name[jet1pt       ] + " >> " + h_name[jet1pt       ] + "( 3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[jet1eta      ] + " >> " + h_name[jet1eta      ] + "(   60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[jet1phi      ] + " >> " + h_name[jet1phi      ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[jet1mass     ] + " >> " + h_name[jet1mass     ] + "(  100,  0  ,  100   )", mycut );

	mytree -> Draw( b_name[jet2pt       ] + " >> " + h_name[jet2pt       ] + "( 3000,  0  , 3000   )", mycut );
	mytree -> Draw( b_name[jet2eta      ] + " >> " + h_name[jet2eta      ] + "(   60, -3  ,    3   )", mycut );
	mytree -> Draw( b_name[jet2phi      ] + " >> " + h_name[jet2phi      ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[jet2mass     ] + " >> " + h_name[jet2mass     ] + "(  100,  0  ,  100   )", mycut );

	mytree -> Draw( b_name[metPfType1   ] + " >> " + h_name[metPfType1   ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[metPfType1Phi] + " >> " + h_name[metPfType1Phi] + "(  100,  0,     3.2  )", mycut );

	mytree -> Draw( b_name[m2l          ] + " >> " + h_name[m2l          ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[mt2ll        ] + " >> " + h_name[mt2ll        ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[mt2lblb      ] + " >> " + h_name[mt2lblb      ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[mtw1         ] + " >> " + h_name[mtw1         ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[mtw2         ] + " >> " + h_name[mtw2         ] + "( 3000,  0,   3000   )", mycut );

	mytree -> Draw( b_name[ht           ] + " >> " + h_name[ht           ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[htjets       ] + " >> " + h_name[htjets       ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[htnojets     ] + " >> " + h_name[htnojets     ] + "( 3000,  0,   3000   )", mycut );

	mytree -> Draw( b_name[ht           ] + " >> " + h_name[ht           ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[htjets       ] + " >> " + h_name[htjets       ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[htnojets     ] + " >> " + h_name[htnojets     ] + "( 3000,  0,   3000   )", mycut );

	mytree -> Draw( b_name[njet         ] + " >> " + h_name[njet         ] + "(   10,  0,     10   )", mycut );
	mytree -> Draw( b_name[nbjet30csvv2l] + " >> " + h_name[nbjet30csvv2l] + "(   10,  0,     10   )", mycut );
	mytree -> Draw( b_name[nbjet30csvv2m] + " >> " + h_name[nbjet30csvv2m] + "(   10,  0,     10   )", mycut );
	mytree -> Draw( b_name[nbjet30csvv2t] + " >> " + h_name[nbjet30csvv2t] + "(   10,  0,     10   )", mycut );

	mytree -> Draw( b_name[dphijet1met  ] + " >> " + h_name[dphijet1met  ] + "(  100,  0,      3.2 )", mycut );   
	mytree -> Draw( b_name[dphijet2met  ] + " >> " + h_name[dphijet2met  ] + "(  100,  0,      3.2 )", mycut ); 
	mytree -> Draw( b_name[dphijj       ] + " >> " + h_name[dphijj       ] + "(  100,  0,      3.2 )", mycut );    
	mytree -> Draw( b_name[dphijjmet    ] + " >> " + h_name[dphijjmet    ] + "(  100,  0,      3.2 )", mycut );
   	mytree -> Draw( b_name[dphill       ] + " >> " + h_name[dphill       ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilep1jet1 ] + " >> " + h_name[dphilep1jet1 ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilep1jet2 ] + " >> " + h_name[dphilep1jet2 ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilep2jet1 ] + " >> " + h_name[dphilep2jet1 ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilep2jet2 ] + " >> " + h_name[dphilep2jet2 ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilmet1    ] + " >> " + h_name[dphilmet1    ] + "(  100,  0,      3.2 )", mycut );
	mytree -> Draw( b_name[dphilmet2    ] + " >> " + h_name[dphilmet2    ] + "(  100,  0,      3.2 )", mycut ); 
	mytree -> Draw( b_name[dphillmet    ] + " >> " + h_name[dphillmet    ] + "(  100,  0,      3.2 )", mycut ); 

	mytree -> Draw( b_name[top1eta_gen  ] + " >> " + h_name[top1eta_gen  ] + "(   60, -3,      3   )", mycut );
	mytree -> Draw( b_name[top1phi_gen  ] + " >> " + h_name[top1phi_gen  ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[top1pt_gen   ] + " >> " + h_name[top1pt_gen   ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[top2eta_gen  ] + " >> " + h_name[top2eta_gen  ] + "(   60, -3,      3   )", mycut );
	mytree -> Draw( b_name[top2phi_gen  ] + " >> " + h_name[top2phi_gen  ] + "(  200, -3.2,    3.2 )", mycut );
	mytree -> Draw( b_name[top2pt_gen   ] + " >> " + h_name[top2pt_gen   ] + "( 3000,  0,   3000   )", mycut );
	mytree -> Draw( b_name[detatt_gen   ] + " >> " + h_name[detatt_gen   ] + "(   60,  0,      6   )", mycut );

	mytree -> Draw( b_name[nvtx         ] + " >> " + h_name[nvtx         ] + "(   60,  0,     60   )", mycut );

	mytree -> Draw( b_name[sphericity   ] + " >> " + h_name[sphericity   ] + "(  200, -1,      1   )", mycut );
	mytree -> Draw( b_name[alignment    ] + " >> " + h_name[alignment    ] + "(  200, -1,      1   )", mycut );
	mytree -> Draw( b_name[planarity    ] + " >> " + h_name[planarity    ] + "(  200, -1,      1   )", mycut );

	mytree -> Draw( b_name[topRecoW     ] + " >> " + h_name[topRecoW     ] + "(  50,  0.00,   0.01)", mycut );
	mytree -> Draw( b_name[darkpt       ] + " >> " + h_name[darkpt       ] + "( 310,  -100,3000   )", mycut );

	mytree -> Draw( b_name[mva01        ] + " >> " + h_name[mva01        ] + "( 120,  -0.1, 1.1   )", mycut );

	for( int i = 0; i < nhisto; i++ ){	

		myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );

		myhisto[i] -> Write(); 

	}

		if( process == "00_Fakes" || process == "01_Data" ) yields << Form( "%20s \t %10.3f \n", process.Data(),              myhisto[0]->Integral() );
		else                                                yields << Form( "%20s \t %10.3f \n", process.Data(), lumi_fb_2016*myhisto[0]->Integral() );

	storagefile -> Close();

}
