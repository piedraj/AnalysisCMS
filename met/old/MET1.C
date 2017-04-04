#include "MET.h"

void MET1(){

	if( DoFillHistograms == false ) { cout << "   >>   FillHistograms deactivated !!!   " << endl;  return; }

	Assign(); 

	TH1::SetDefaultSumw2(); 

	FillHistograms(); 

}


void FillHistograms(){

	//for( int j = 0; j < nprocess; j++ ){
	for( int j = alpha; j < omega+1; j++ ){

		//if ( j == DoubleEG2016B     || j == DoubleEG2016C     || j == DoubleEG2016D     ) continue; 
		//if ( j == DoubleMuon2016B   || j == DoubleMuon2016C   || j == DoubleMuon2016D   ) continue; 
		//if ( j == SinglePhoton2016B || j == SinglePhoton2016C || j == SinglePhoton2016D ) continue; 
		//if ( j != DoubleEG2016E && j != DoubleEG2016F && j != DoubleMuon2016E && j != DoubleMuon2016F && j != SinglePhoton2016E && j != SinglePhoton2016F ) continue; 

		//if ( j == DoubleEG2016E      || j == DoubleEG2016F     || j == DoubleEG2016G     ) continue;     
		//if ( j == DoubleMuon2016E    || j == DoubleMuon2016F   || j == DoubleMuon2016G   ) continue;      
		//if ( j == SinglePhoton2016E  || j == SinglePhoton2016F || j == SinglePhoton2016G ) continue;       

		cout << "next n-tupla: " << processID[j] << endl; 

		FillHistogram( j );

		if( SkipEventLoop == false ){

			TFile* allhistos = new TFile("histograms/" + allhistosWriteTo +".root", "update");

			for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){ 

				for( int i = 0; i < nvariable_G; i++ ){

					h_global_W[i][j][s] -> Write();  

				}


				for( int i = 0; i < nvariable_R; i++ ){		
						
					for( int k = 0; k < nbinpT   ; k++) 	h_resol_pT_W   [i][j][k][s] -> Write();
					for( int k = 0; k < nbinsumET; k++)	h_resol_sumET_W[i][j][k][s] -> Write();
					for( int k = 0; k < nbinNVtx ; k++)	h_resol_NVtx_W [i][j][k][s] -> Write();

				}

			}

			allhistos -> Close();

		}

	}   // nprocess

	cout << "                          " << endl; 
	cout << "                          " << endl;
	cout << "    histograms filled !!! " << endl;
	cout << "                          " << endl;
	cout << "                          " << endl;

}



void FillHistogram( int process ){


	//ofstream sync;   // for the sync with Leonora

	//sync.open( "sync/160926_" + processID[process] + "_weights.txt" );

	//sync << "RUN       \t LUMI     \t EVT   \t Zll pT  \t lep-1 pT \t lep-2 pT \t MET       \n \n \n"; 

	//sync << "baseW \t puW \t genW \t eventW (total)       \n \n \n"; 


	int TheKanal = kanal[process]; 


	//----- k-factor weights

	///TFile* kfactorFile = new TFile( "kfactor.root", "read");

	///TF1*  kfactor_Gjets = (TF1*)  kfactorFile -> Get( "gj-40"    );
	///TH1F* kfactor_ZnnG  = (TH1F*) kfactorFile -> Get( "znng-130" );  
	///TH1F* kfactor_ZllG  = (TH1F*) kfactorFile -> Get( "zllg-130" );  


	//----- pu reweighing

	///TFile* f_pu = new TFile( "pileup/170131_Zmm_rereco-data_Spring16-MC_nTrueInt.root", "read");

	///TH1F* h_pu[nkanal];

	///h_pu[Zee  ] = (TH1F*) f_pu -> Get( "h_pu-reweighting_" + kanalID[Zee  ] );
	///h_pu[Zmumu] = (TH1F*) f_pu -> Get( "h_pu-reweighting_" + kanalID[Zmumu] );
	///h_pu[Gamma] = (TH1F*) f_pu -> Get( "h_pu-reweighting_" + kanalID[Gamma] );
 

	//----- id/iso efficiencies

	/*TFile* f_ele_idiso = new TFile("efficiencies/egammaEffi.txt_SF2D.root"             , "read"); 
	TFile* f_ele_trk   = new TFile("efficiencies/egammaEffi.txt_SF2D_Gsf_tracking.root", "read"); 

	TFile* f_mu_id  = new TFile("efficiencies/MuonID_Z_RunBCD_prompt80X_7p65.root"     , "read"); 
	TFile* f_mu_iso = new TFile("efficiencies/EfficienciesAndSF_ISO.root"              , "read");
 	TFile* f_mu_trk = new TFile("efficiencies/tracking_muons.root"                     , "read");

	TFile* f_gamma_idiso = new TFile("efficiencies/egammaEffi.txt_SF2DTight.root"      , "read"); 



	TH2F* h_eff[nkanal][2];


	h_eff[Zee][0] = (TH2F*) f_ele_idiso -> Get( "EGamma_SF2D" ); 
	h_eff[Zee][1] = (TH2F*) f_ele_trk   -> Get( "EGamma_SF2D" ); 


	h_eff[Zmumu][0] = (TH2F*) f_mu_id  -> Get( "MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/abseta_pt_ratio"   ); 
	h_eff[Zmumu][1] = (TH2F*) f_mu_iso -> Get( "MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/abseta_pt_ratio" ); 

	TGraphAsymmErrors* mu_trk = (TGraphAsymmErrors*) f_mu_trk -> Get("ratio_eta");


	h_eff[Gamma][0] = (TH2F*) f_gamma_idiso -> Get( "EGamma_SF2D" );*/ 


	//----- trigger efficiencies

	/*ifstream leg1_el_txt, leg2_el_txt, single_el_txt;
	ifstream leg1_mu_txt, leg2_mu_txt, single_mu_txt;  

	  leg1_el_txt.open("efficiencies/leg1_el.txt"  ); 
	  leg2_el_txt.open("efficiencies/leg2_el.txt"  ); 
	single_el_txt.open("efficiencies/single_el.txt"); 

	  leg1_mu_txt.open("efficiencies/leg1_mu.txt"  ); 
	  leg2_mu_txt.open("efficiencies/leg2_mu.txt"  ); 
	single_mu_txt.open("efficiencies/single_mu.txt"); 

	float leg1_el  [100][6];
	float leg2_el  [100][6];
	float single_el[100][6];

	float leg1_mu  [156][7];
	float leg2_mu  [117][7];
	float single_mu[156][7];

	int j = 0; 

	while( 1 ){      

		if (!leg1_el_txt.good()) break;

		if (j > 99) break;

		leg1_el_txt   >> leg1_el  [j][0] >> leg1_el  [j][1] >> leg1_el  [j][2] >> leg1_el  [j][3] >> leg1_el  [j][4] >> leg1_el  [j][5] ;  
		leg2_el_txt   >> leg2_el  [j][0] >> leg2_el  [j][1] >> leg2_el  [j][2] >> leg2_el  [j][3] >> leg2_el  [j][4] >> leg2_el  [j][5] ;   
		single_el_txt >> single_el[j][0] >> single_el[j][1] >> single_el[j][2] >> single_el[j][3] >> single_el[j][4] >> single_el[j][5] ;    

		j++;

	}

	int k  = 0;
	int k2 = 0; 

	while( 1 ){      

		if (!leg1_mu_txt.good()) break;

		if (k > 156) break;

		leg1_mu_txt   >> leg1_mu  [k][0] >> leg1_mu  [k][1] >> leg1_mu  [k][2] >> leg1_mu  [k][3] >> leg1_mu  [k][4] >> leg1_mu  [k][5] >> leg1_mu  [k][6];  
		single_mu_txt >> single_mu[k][0] >> single_mu[k][1] >> single_mu[k][2] >> single_mu[k][3] >> single_mu[k][4] >> single_mu[k][5] >> single_mu[k][6];   

		//cout << leg1_mu[k][0] << "\t" << leg1_mu[k][1] << "\t" << leg1_mu[k][2] << "\t"<< leg1_mu[k][3] << "\t" << leg1_mu[k][4] << "\t" <<  leg1_mu[k][5] << endl;  

		k++;

	}

	while( 1 ){      

		if (!leg2_mu_txt.good()) break;

		if (k2 > 117) break;

		leg2_mu_txt   >> leg2_mu  [k2][0] >> leg2_mu  [k2][1] >> leg2_mu  [k2][2] >> leg2_mu  [k2][3] >> leg2_mu  [k2][4] >> leg2_mu  [k2][5] >> leg2_mu  [k2][6];  

		k2++;

	}		 

	leg1_el_txt  .close();
	leg2_el_txt  .close();
	single_el_txt.close();

	leg1_mu_txt  .close();
	leg2_mu_txt  .close();
	single_mu_txt.close();*/




	//----- opening the tree...


 	TFile* TheFile = new TFile( NTuplaDir[TheKanal] + sampleID[process] + ".root", "read"); 


	///TH1F* TheSumGenWeight; float TheDenominator; 

	///if( !isData[process] ) { TH1F* TheSumGenWeight = (TH1F*) TheFile -> Get( "SumGenWeights" ); TheDenominator = TheSumGenWeight -> Integral(); }  // cout << "total nentries from the histo = " << TheDenominator << endl;

	TTree* tree = (TTree*) TheFile -> Get( "latino" );

	tree -> SetBranchStatus("*", 0);  // disable all branches


	int    run ; 
	int    lumi;
	int    evt ; 

	float eventW; 
	float Scale ;
	float uPara ;
	float uPerp ; 

	///int nLepGood20                          ;
	///float zll_pt                            ;
	///float zll_phi                           ; 
	///float zll_mass                          ; 

	///int   ngamma                            ;
	///int   gamma_idCutBased[3]               ;
	///float gamma_hOverE[3]			;
	///float gamma_sigmaIetaIeta[3]		;
	///float gamma_r9[3]                       ;	
	///float gamma_pt[3]                       ;
	///float gamma_eta[3]                      ;
	///float gamma_phi[3]                      ;

	///int   nLepGood10                        ;  
	///float lep_pt[4]                         ;  
	///float lep_eta[4]                        ;  
	///int   lep_pdgId[4]                      ;

        float lep1pt, lep1eta, lep1phi, lep1id; 
	float lep2pt, lep2eta, lep2phi, lep2id;

	///int   HBHENoiseFilter                   ;
	///int   HBHENoiseIsoFilter                ;
	///int   CSCTightHalo2015Filter            ;
	///int   EcalDeadCellTriggerPrimitiveFilter;
	///int   goodVertices                      ;
	///int   eeBadScFilter                     ;
	///int   globalTightHalo2016Filter         ;
	///float badMuonFilter                     ;
	///float badChargedHadronFilter            ;


	///float met_sumEt                         ;
	float nVert                             ;
	float nTrueInt                          ;
	float met_pt                            ;
	float met_phi                           ;
	///float met_jecUp_pt                      ;
	///float met_jecUp_phi                     ; 
	///float met_jecDown_pt                    ;
	///float met_jecDown_phi                   ; 
	///float met_UnEup_pt                      ;
	///float met_UnEup_phi                     ;
	///float met_UnEdown_pt                    ;
	///float met_UnEdown_phi                   ;

	///float metPuppi_sumEt                    ;
	///float metPuppi_pt                       ; 
	///float metPuppi_phi                      ; 
	///float metPuppi_jecUp_pt                 ; 
	///float metPuppi_jecUp_phi                ; 
	///float metPuppi_jecDown_pt               ;
	///float metPuppi_jecDown_phi              ; 
	///float metPuppi_UnEup_pt                 ;
	///float metPuppi_UnEup_phi                ;
	///float metPuppi_UnEdown_pt               ;
	///float metPuppi_UnEdown_phi              ;

	///float puWeight                          ;
	///float genWeight	                        ;

	///int   nGenPart                          ; 
	///int   GenPart_pdgId[40]                 ;
	///float GenPart_pt[40]                    ;


	tree -> SetBranchStatus( "eventW" , 1 ); tree -> SetBranchAddress( "eventW" , &eventW  );

	tree -> SetBranchStatus( "scale" , 1 ); tree -> SetBranchAddress( "scale" , &Scale  );
	tree -> SetBranchStatus( "uPara" , 1 ); tree -> SetBranchAddress( "uPara" , &uPara  );
	tree -> SetBranchStatus( "uPerp" , 1 ); tree -> SetBranchAddress( "uPerp" , &uPerp  );

	tree -> SetBranchStatus( "run" , 1 );	tree -> SetBranchAddress( "run" , &run  );
	tree -> SetBranchStatus( "lumi", 1 );	tree -> SetBranchAddress( "lumi", &lumi );
	tree -> SetBranchStatus( "event",1 );	tree -> SetBranchAddress( "event",&evt  );


	if( TheKanal == Zee || TheKanal == Zmumu ){

		///tree -> SetBranchStatus("nLepGood20", 1);	tree -> SetBranchAddress( "nLepGood20"                             , &nLepGood20                         );
		///tree -> SetBranchStatus("zll_pt"    , 1);	tree -> SetBranchAddress( "zll_pt"                                 , &zll_pt                             );
		///tree -> SetBranchStatus("zll_phi"   , 1);	tree -> SetBranchAddress( "zll_phi"                                , &zll_phi                            );
		///tree -> SetBranchStatus("zll_mass"  , 1);	tree -> SetBranchAddress( "zll_mass"                               , &zll_mass                           );

	}

	if( TheKanal == Gamma ){

		///tree -> SetBranchStatus("ngamma"             , 1);	tree -> SetBranchAddress( "ngamma"                                 , &ngamma                             );
		///tree -> SetBranchStatus("gamma_idCutBased"   , 1);	tree -> SetBranchAddress( "gamma_idCutBased"                       , &gamma_idCutBased                   );
		///tree -> SetBranchStatus("gamma_hOverE"       , 1);	tree -> SetBranchAddress( "gamma_hOverE"                           , &gamma_hOverE                       );
		///tree -> SetBranchStatus("gamma_sigmaIetaIeta", 1);	tree -> SetBranchAddress( "gamma_sigmaIetaIeta"                    , &gamma_sigmaIetaIeta                );
		///tree -> SetBranchStatus("gamma_r9"           , 1);	tree -> SetBranchAddress( "gamma_r9"                               , &gamma_r9                           );
		///tree -> SetBranchStatus("gamma_pt"           , 1);	tree -> SetBranchAddress( "gamma_pt"                               , &gamma_pt                           );
		///tree -> SetBranchStatus("gamma_eta"          , 1);	tree -> SetBranchAddress( "gamma_eta"                              , &gamma_eta                          );
		///tree -> SetBranchStatus("gamma_phi"          , 1);	tree -> SetBranchAddress( "gamma_phi"                              , &gamma_phi                          );

		///tree -> SetBranchStatus("nLepGood10"         , 1);	tree -> SetBranchAddress( "nLepGood10"                             , &nLepGood10                         );

	}

	///tree -> SetBranchStatus("lep_pt"                                 , 1);		tree -> SetBranchAddress( "lep_pt"                                 , &lep_pt                             );
	///tree -> SetBranchStatus("lep_eta"                                , 1);		tree -> SetBranchAddress( "lep_eta"                                , &lep_eta                            );
	///tree -> SetBranchStatus("lep_pdgId"                              , 1);		tree -> SetBranchAddress( "lep_pdgId"                              , &lep_pdgId                          );


tree -> SetBranchStatus("lep1pt" , 1); tree -> SetBranchAddress( "lep1pt" , &lep1pt  );
tree -> SetBranchStatus("lep1eta", 1); tree -> SetBranchAddress( "lep1eta", &lep1eta );
tree -> SetBranchStatus("lep1phi", 1); tree -> SetBranchAddress( "lep1phi", &lep1phi );
tree -> SetBranchStatus("lep1id" , 1); tree -> SetBranchAddress( "lep1id" , &lep1id  );

tree -> SetBranchStatus("lep2pt" , 1); tree -> SetBranchAddress( "lep2pt" , &lep2pt  );
tree -> SetBranchStatus("lep2eta", 1); tree -> SetBranchAddress( "lep2eta", &lep2eta );
tree -> SetBranchStatus("lep2phi", 1); tree -> SetBranchAddress( "lep2phi", &lep2phi );
tree -> SetBranchStatus("lep2id" , 1); tree -> SetBranchAddress( "lep2id" , &lep2id  );

	///tree -> SetBranchStatus("Flag_HBHENoiseFilter"                   , 1);		tree -> SetBranchAddress( "Flag_HBHENoiseFilter"                   , &HBHENoiseFilter                    );
	///tree -> SetBranchStatus("Flag_HBHENoiseIsoFilter"                , 1);		tree -> SetBranchAddress( "Flag_HBHENoiseIsoFilter"                , &HBHENoiseIsoFilter                 );
	///tree -> SetBranchStatus("Flag_CSCTightHalo2015Filter"            , 1);		tree -> SetBranchAddress( "Flag_CSCTightHalo2015Filter"            , &CSCTightHalo2015Filter             );
	///tree -> SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter", 1);		tree -> SetBranchAddress( "Flag_EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter );
	///tree -> SetBranchStatus("Flag_goodVertices"                      , 1);		tree -> SetBranchAddress( "Flag_goodVertices"                      , &goodVertices                       );
	///tree -> SetBranchStatus("Flag_eeBadScFilter"                     , 1);		tree -> SetBranchAddress( "Flag_eeBadScFilter"                     , &eeBadScFilter                      );
	///tree -> SetBranchStatus("Flag_globalTightHalo2016Filter"         , 1);		tree -> SetBranchAddress( "Flag_globalTightHalo2016Filter"         , &globalTightHalo2016Filter          );
	///tree -> SetBranchStatus("Flag_badMuonFilter"                     , 1);		tree -> SetBranchAddress( "Flag_badMuonFilter"                     , &badMuonFilter                      );
	///tree -> SetBranchStatus("Flag_badChargedHadronFilter"            , 1);		tree -> SetBranchAddress( "Flag_badChargedHadronFilter"            , &badChargedHadronFilter             );




	///tree -> SetBranchStatus("met_sumEt"      , 1);		tree -> SetBranchAddress( "met_sumEt"                              , &met_sumEt                          );
	tree -> SetBranchStatus("nvtx"          , 1);		tree -> SetBranchAddress( "nvtx"                                  , &nVert                              );
	tree -> SetBranchStatus("metPfType1"         , 1);		tree -> SetBranchAddress( "metPfType1"                                 , &met_pt                             );
	tree -> SetBranchStatus("metPfType1Phi"      , 1);		tree -> SetBranchAddress( "metPfType1Phi"                              , &met_phi                            );
	///tree -> SetBranchStatus("met_jecUp_pt"   , 1);		tree -> SetBranchAddress( "met_jecUp_pt"                           , &met_jecUp_pt                       );
	///tree -> SetBranchStatus("met_jecUp_phi"  , 1);		tree -> SetBranchAddress( "met_jecUp_phi"                          , &met_jecUp_phi                      );
	///tree -> SetBranchStatus("met_jecDown_pt" , 1);		tree -> SetBranchAddress( "met_jecDown_pt"                         , &met_jecDown_pt                     );
	///tree -> SetBranchStatus("met_jecDown_phi", 1);		tree -> SetBranchAddress( "met_jecDown_phi"                        , &met_jecDown_phi                    );

	///tree -> SetBranchStatus("met_shifted_UnclusteredEnUp_pt"   , 1); tree -> SetBranchAddress( "met_shifted_UnclusteredEnUp_pt"   , &met_UnEup_pt   );
	///tree -> SetBranchStatus("met_shifted_UnclusteredEnUp_phi"  , 1); tree -> SetBranchAddress( "met_shifted_UnclusteredEnUp_phi"  , &met_UnEup_phi  );
	///tree -> SetBranchStatus("met_shifted_UnclusteredEnDown_pt" , 1); tree -> SetBranchAddress( "met_shifted_UnclusteredEnDown_pt" , &met_UnEdown_pt );
	///tree -> SetBranchStatus("met_shifted_UnclusteredEnDown_phi", 1); tree -> SetBranchAddress( "met_shifted_UnclusteredEnDown_phi", &met_UnEdown_phi);


	/*tree -> SetBranchStatus("metPuppi_sumEt"      , 1); tree -> SetBranchAddress( "metPuppi_sumEt"      , &metPuppi_sumEt        );
	tree -> SetBranchStatus("metPuppi_pt"         , 1); tree -> SetBranchAddress( "metPuppi_pt"         , &metPuppi_pt           );
	tree -> SetBranchStatus("metPuppi_phi"        , 1); tree -> SetBranchAddress( "metPuppi_phi"        , &metPuppi_phi          );
	tree -> SetBranchStatus("metPuppi_jecUp_pt"   , 1); tree -> SetBranchAddress( "metPuppi_jecUp_pt"   , &metPuppi_jecUp_pt     );
	tree -> SetBranchStatus("metPuppi_jecUp_phi"  , 1); tree -> SetBranchAddress( "metPuppi_jecUp_phi"  , &metPuppi_jecUp_phi    );
	tree -> SetBranchStatus("metPuppi_jecDown_pt" , 1); tree -> SetBranchAddress( "metPuppi_jecDown_pt" , &metPuppi_jecDown_pt   );
	tree -> SetBranchStatus("metPuppi_jecDown_phi", 1); tree -> SetBranchAddress( "metPuppi_jecDown_phi", &metPuppi_jecDown_phi  );

	tree -> SetBranchStatus("metPuppi_shifted_UnclusteredEnUp_pt"   , 1); tree -> SetBranchAddress( "metPuppi_shifted_UnclusteredEnUp_pt"   , &metPuppi_UnEup_pt   );
	tree -> SetBranchStatus("metPuppi_shifted_UnclusteredEnUp_phi"  , 1); tree -> SetBranchAddress( "metPuppi_shifted_UnclusteredEnUp_phi"  , &metPuppi_UnEup_phi  );
	tree -> SetBranchStatus("metPuppi_shifted_UnclusteredEnDown_pt" , 1); tree -> SetBranchAddress( "metPuppi_shifted_UnclusteredEnDown_pt" , &metPuppi_UnEdown_pt );
	tree -> SetBranchStatus("metPuppi_shifted_UnclusteredEnDown_phi", 1); tree -> SetBranchAddress( "metPuppi_shifted_UnclusteredEnDown_phi", &metPuppi_UnEdown_phi);*/

	
	if(  !isData[process] ){

		///tree -> SetBranchStatus("puWeight" , 1);	tree -> SetBranchAddress( "puWeight"                               , &puWeight                           );
		///tree -> SetBranchStatus("genWeight", 1);	tree -> SetBranchAddress( "genWeight"                              , &genWeight                          );
		tree -> SetBranchStatus("ntrueint" , 1); 	tree -> SetBranchAddress( "ntrueint"                               , &nTrueInt                           ); 

	}

	if( process == GJets40100 || process == GJets100200 || process == GJets200400|| process == GJets400600 || process == GJets600Inf || process == ZGJets || process == ZGTo2LG ){

			///tree -> SetBranchStatus("nGenPart"     , 1);	tree -> SetBranchAddress( "nGenPart"     , &nGenPart      );
			///tree -> SetBranchStatus("GenPart_pdgId", 1);	tree -> SetBranchAddress( "GenPart_pdgId", &GenPart_pdgId );
			///tree -> SetBranchStatus("GenPart_pt"   , 1);	tree -> SetBranchAddress( "GenPart_pt"   , &GenPart_pt    );

	}
	



	for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){
	for( int i = 0; i < nvariable_G; i++ ){

		if( i == parall_R ) h_global_W[i][process][s] = new TH1F( "h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[process] + "_" + systematicID[s], variableID_G[i], nbinuPara, minuPara, maxuPara );
		if( i == transv_R ) h_global_W[i][process][s] = new TH1F( "h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[process] + "_" + systematicID[s], variableID_G[i], nbinuPerp, minuPerp, maxuPerp );
		if( i == MET      ) h_global_W[i][process][s] = new TH1F( "h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[process] + "_" + systematicID[s], variableID_G[i], nbinMET  , minMET  , maxMET   );
		if( i == VpT      ) h_global_W[i][process][s] = new TH1F( "h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[process] + "_" + systematicID[s], variableID_G[i], nbinpT   , minpT   , maxpT    );
		if( i == nvert    ) h_global_W[i][process][s] = new TH1F( "h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[process] + "_" + systematicID[s], variableID_G[i], nbinnvert, minnvert, maxnvert );
		
	}  // variable_G	



		for( int k = 0; k < nbinpT; k++ ){

			h_resol_pT_W   [parall_R][process][k][s] = new TH1F( Form("h_resol_pT_" + variableID_R[parall_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution pT"        , nbinuPara, minuPara, maxuPara );

			h_resol_pT_W   [transv_R][process][k][s] = new TH1F( Form("h_resol_pT_" + variableID_R[transv_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution pT"        , nbinuPerp, minuPerp, maxuPerp );

			h_resol_pT_W   [scale   ][process][k][s] = new TH1F( Form("h_resol_pT_" + variableID_R[scale   ] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution pT"        , nbinscale, minscale, maxscale );

		}

		for( int k = 0; k < nbinsumET; k++ ){

			h_resol_sumET_W[parall_R][process][k][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[parall_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution sum ET"    , nbinuPara, minuPara, maxuPara );

			h_resol_sumET_W[transv_R][process][k][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[transv_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution sum ET"    , nbinuPerp, minuPerp, maxuPerp );

			h_resol_sumET_W[scale   ][process][k][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[scale   ] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution sum ET"    , nbinscale, minscale, maxscale );

		}

		for( int k = 0; k < nbinNVtx; k++ ){

			h_resol_NVtx_W [parall_R][process][k][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[parall_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution num of vtx", nbinuPara, minuPara, maxuPara );

			h_resol_NVtx_W [transv_R][process][k][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[transv_R] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution num of vtx", nbinuPerp, minuPerp, maxuPerp );

			h_resol_NVtx_W [scale   ][process][k][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[scale   ] + "_" + kanalID[TheKanal] + "_"  + processID[process] + "_%d_" + systematicID[s], k), "resolution num of vtx", nbinscale, minscale, maxscale );

		}

	}  // systematic 


	int nentries = tree -> GetEntries(); //cout << " nentries = " << nentries << endl;

	if (  isData[process]  &&  RunOverAllData == false  &&  MaxEntriesData < nentries )  nentries = MaxEntriesData; 
		
	if ( !isData[process]  &&  RunOverAllMC   == false  &&  MaxEntriesMC   < nentries )  nentries = MaxEntriesMC  ;

	float baseW = 1.0;   ///if ( !isData[process] ) baseW = xs[process]/TheDenominator;  //cout << xs[process] << " -- " << nentries << " -- " << TheDenominator << " -- " << baseW << endl; 

	if ( SkipEventLoop == true ) return; 
 
	for ( Long64_t ievt = 0; ievt < nentries; ievt++ ) {



		if( ievt%10000 == 0 ) cout << processID[process] << "   >>  evt " << ievt << endl;

		tree -> GetEntry(ievt);	

		//float eventW = 1.0 ;



		if ( TheKanal == Zee || TheKanal == Zmumu ){

			///if ( nLepGood20 != 2                     ) continue;

			///if (  abs(lep_eta[0]) > 2.4             ||  abs(lep_eta[1]) > 2.4             ) continue; 	 

			///if (  abs( abs(lep_eta[0])-1.5 ) < 0.1  ||  abs( abs(lep_eta[1])-1.5 ) < 0.1  ) continue;


			///if ( zll_mass < 81. || zll_mass > 101.   ) continue; 

			///if ( lep_pt[0] < 20. || lep_pt[1] < 20.  ) continue; 	//cout << lep_pt[0] << " -- " << lep_pt[1] << endl; 

			if ( lep1pt < 25. ) continue; 

			if ( TheKanal == Zee && lep1id*lep2id != -11*11 ) continue;
				
			if ( TheKanal == Zmumu && lep1id*lep2id != -13*13 ) continue;



	
		}



		/*if ( TheKanal == Gamma ){ 

			if ( ngamma != 1                            ) continue;

			//if ( gamma_idCutBased[0] != 3                                      ) continue; 
			if ( gamma_hOverE[0] > 0.05                                          ) continue; 
			if ( gamma_sigmaIetaIeta[0] < 0.005 || gamma_sigmaIetaIeta[0] > 0.01 ) continue; 

			if ( gamma_r9[0] < 0.9 || gamma_r9[0] > 1.0 ) continue;
			if ( gamma_pt[0] < 50                       ) continue; 
			if ( abs(gamma_eta[0]) > 1.4442             ) continue; 

			if ( nLepGood10 != 0                        ) continue; 

			if( isData[process] ){   // triggers


				if( HLT_Photon30 == 0  &&  HLT_Photon50 == 0  &&  HLT_Photon75 == 0  &&  HLT_Photon90 == 0  &&  HLT_Photon120 == 0 ) continue;


				if( HLT_Photon120 == 1 ) eventW *= HLT_Photon120_Prescale;

				else{

					if( HLT_Photon90 == 1 )	eventW *= HLT_Photon90_Prescale;

					else{

						if( HLT_Photon75 == 1 )	eventW *= HLT_Photon75_Prescale;

						else{

							if( HLT_Photon50 == 1 )	eventW *= HLT_Photon50_Prescale;

							else{

								if( HLT_Photon30 == 1 )	eventW *= HLT_Photon30_Prescale;


							}

						}

					}
	
				}

			}

		}*/



		// MET filters
		// -----------------------------------------------------
		///if ( HBHENoiseFilter                    != 1 ) continue; 
		///if ( HBHENoiseIsoFilter                 != 1 ) continue; 
		//if ( CSCTightHalo2015Filter             != 1 ) continue; 
		///if ( EcalDeadCellTriggerPrimitiveFilter != 1 ) continue; 
		///if ( goodVertices                       != 1 ) continue; 
		///if ( eeBadScFilter                      != 1 ) continue; 	
		///if ( globalTightHalo2016Filter          != 1 ) continue;
		///if ( badMuonFilter                      != 1 ) continue;
		///if ( badChargedHadronFilter             != 1 ) continue;
		// -----------------------------------------------------


		// ----- sync -------------------------			



		//if(  ( process == DoubleEG2016B  &&  run == 273158 )  ||  ( process == DoubleMuon2016B  &&  run == 273158 )  ){



		//if(  process == DY_mm  &&  ( lumi > 153860 && lumi <= 155000 ) ){

		//	sync << Form( "%12u * %12u * %12llu * %12f * %12f * %12f * %12f *\n", run, lumi, evt, zll_pt, lep_pt[0], lep_pt[1], met_pt );

		//}




		//} 

		//if(  process == DoubleEG2016C    &&  run == 275658  )
		//if(  process == DoubleEG2016D    &&  run == 276315  ) 
		

		// ------------------------------------

 
		if( !isData[process] ){

			///eventW *= baseW    ; 	

			//cout << nVert << " -- " << h_pu[TheKanal] -> GetBinContent( h_pu[TheKanal]->FindBin(nVert) ) << endl;	

			//int nVert_rw;  ( nVert >= 38 )  ?  nVert_rw = 38  :  nVert_rw = nVert; 

			//float puW = h_pu[TheKanal] -> GetBinContent( h_pu[TheKanal]->FindBin(nVert) );
			///float puW = h_pu[TheKanal] -> GetBinContent( h_pu[TheKanal]->FindBin(nTrueInt) );

			///eventW *= puW;   		

			///eventW *= genWeight;// /abs(genWeight);

			///if( TheKanal == Zee || TheKanal == Zmumu ){

				///int lep1flavour = ( abs(lep_pdgId[0]) == 11 )  ?  0  :  1  ; //if ( lep1flavour == 0 ) continue;
				///int lep2flavour = ( abs(lep_pdgId[1]) == 11 )  ?  0  :  1  ; //if ( lep2flavour == 0 ) continue;  

				//cout << "\n (pT, eta) = (" << lep_pt[0] << ", " << lep_eta[0] << ")\n\n" << endl; 
				//cout << "\n (pT, eta) = (" << lep_pt[1] << ", " << lep_eta[1] << ")\n\n" << endl;  

				//lep1flavour = 1; 
				//lep2flavour = 1; 

/*float idW1  = ( lep_pt[0] < 200. )                                                                          ?   h_eff[lep1flavour][0]->GetBinContent( h_eff[lep1flavour][0]->FindBin(fabs(lep_eta[0]), lep_pt[0]))   :   1.0 ;  

float isoW1 = (  ( lep1flavour == 0 && lep_pt[0] < 200 )  ||  ( lep1flavour == 1 && lep_pt[0] < 120. )  )   ?   h_eff[lep1flavour][1]->GetBinContent( h_eff[lep1flavour][1]->FindBin(fabs(lep_eta[0]), lep_pt[0]))   :   1.0 ;
				
 
float idW2  = ( lep_pt[1] < 200. )                                                                          ?   h_eff[lep2flavour][0]->GetBinContent( h_eff[lep2flavour][0]->FindBin(fabs(lep_eta[1]), lep_pt[1]))   :   1.0 ;

float isoW2 = (  ( lep1flavour == 0 && lep_pt[1] < 200 )  ||  ( lep1flavour == 1 && lep_pt[1] < 120. )  )   ?   h_eff[lep2flavour][1]->GetBinContent( h_eff[lep2flavour][1]->FindBin(fabs(lep_eta[1]), lep_pt[1]))   :   1.0 ; */ 


				//if( zll_pt > 150 ){

				//cout << "\n ievt: " << ievt << endl; 
				//cout << "leading  pt = " << lep_pt[0] << "   --- eta = " << lep_eta[0] << "   --- ID-W = " << idW1 << "   --- Iso-W = " << isoW1 << endl;  
				//cout << "trailing pt = " << lep_pt[1] << "   --- eta = " << lep_eta[1] << "   --- ID-W = " << idW2 << "   --- Iso-W = " << isoW2 << endl;  
				//cout << "\n" << idW1*isoW1*idW2*isoW2 << "\n\n" << endl; 

				//}

				///eventW *= idW1*isoW1*idW2*isoW2; 

				///float mu_trk_W1 = ( lep1flavour == 1 )  ?  mu_trk -> Eval( lep_eta[0] )  :  1.0; 
				///float mu_trk_W2 = ( lep2flavour == 1 )  ?  mu_trk -> Eval( lep_eta[1] )  :  1.0; 

				///eventW *= mu_trk_W1 * mu_trk_W1; 

//cout << "id1 = " << idW1 << "\t iso1 = " << isoW1 << "\t id2 = " << idW2 << "\t iso2 = " << isoW2 << "\t trk1 = " << mu_trk_W1 << "\t trk2 = " <<  mu_trk_W2 << endl; 

			///}

			/*if( TheKanal == Gamma ){

				//cout << "\n (eta, pT) = (" << gamma_eta[0] << ", " << gamma_pt[0] << ")\n\n" << endl; 

				float W = ( gamma_pt[0] < 200. )   ?   h_eff[Gamma][0]->GetBinContent( h_eff[Gamma][0]->FindBin(fabs(gamma_eta[0]), gamma_pt[0]))   :   1 ;  
								  
				//cout << "\n" << W << "\n\n" << endl; 

				eventW *= W; 

			}*/





/*			if( TheKanal == Zee ){

				// --------------------
				// trigger efficiencies
				// --------------------

				// P = pass-single-trigger  +  not-pass-single-trigger * pass-double-trigger 

				// 	 pass-single-trigger = 1 - not-pass-single-trigger = 1 - not-pass-leading-single * not-pass-trailing-single

				// 	 pass-double-trigger = pass-direct-way  +  not-pass-direct-way * pass-inverse-way

				//  		 pass-direct-way  = pass-leading-leg1  * pass-trailing-leg2
				// 		 pass-inverse-way = pass-trailing-leg1 * pass-leading-leg2


				float PS1, PS2, P11, P22, P12, P21;  
	
				for( int whichlep = 0; whichlep < 2; whichlep++ ){

					float last_eta = -999; 

					int line    = 0; 
					int lineNLO = 0; 

					for( int i = 0; i < 100; i++ ){

						if( last_eta - leg1_el[i][0] == 0 ) continue; 

						last_eta = leg1_el[i][0]; 

						if (     ( lep_eta[whichlep] - leg1_el[i][0] ) * ( lep_eta[whichlep] - leg1_el[i][1] )    < 0.     ){ 

							//cout << i << " -- " << leg1[i][0] << " -- " << lep_eta[whichlep] << endl; 

							line = i; 

							break; 

						}  

					}

					for( int i = line; i < line+10; i++ ){

						if (     ( lep_pt[whichlep] - leg1_el[i][2] ) * ( lep_pt[whichlep] - leg1_el[i][3] )    < 0.     ){ 

							//cout << "\t\t" << i << " -- " << leg1[i][2] << " -- " << lep_pt[whichlep] << endl; 

							lineNLO = i; 

							break; 

						}  

					}	

					//cout << "SF: \t" << leg1[lineNLO][4] << "\t" << leg2[lineNLO][4]<< "\t"  << single[lineNLO][4] << "\n" << endl; 

					if ( whichlep == 0 ){ 

						PS1 = single_el[lineNLO][4]; 
						P11 = leg1_el  [lineNLO][4];
						P21 = leg2_el  [lineNLO][4]; 

					} 

					if ( whichlep == 1 ){ 

						PS2 = single_el[lineNLO][4]; 
						P12 = leg1_el  [lineNLO][4];
						P22 = leg2_el  [lineNLO][4]; 

					}

				}   // whichlep

				float PS  =  1  -  ( 1-PS1 ) * ( 1-PS2 ); 

				float Pd  =  P11 * P22;  

				float Pi  =  P12 * P21; 

				float PD  =  Pd  +  ( 1-Pd ) * Pi ;

				float effTriggerW = PS + ( 1-PS )*PD;    

				//if( lep_pt[0] < 100 ) cout << "\n eff trigger weight = " << effTriggerW << "\n\n" << endl; 

				eventW *= effTriggerW; 

			}*/

		
/*			if( TheKanal == Zmumu ){

				// --------------------
				// trigger efficiencies
				// --------------------

				// P = pass-single-trigger  +  not-pass-single-trigger * pass-double-trigger 

				// 	 pass-single-trigger = 1 - not-pass-single-trigger = 1 - not-pass-leading-single * not-pass-trailing-single

				// 	 pass-double-trigger = pass-direct-way  +  not-pass-direct-way * pass-inverse-way

				//  		 pass-direct-way  = pass-leading-leg1  * pass-trailing-leg2
				// 		 pass-inverse-way = pass-trailing-leg1 * pass-leading-leg2


				float PS1, PS2, P11, P22, P12, P21;  
	
				for( int whichlep = 0; whichlep < 2; whichlep++ ){

					// --------------------------------

					float last_pt_1 = -999; 

					int line_1    = 0; 
					int lineNLO_1 = 0; 

					for( int i = 0; i < 156; i++ ){

						if( last_pt_1 - leg1_mu[i][2] == 0 ) continue; 

						last_pt_1 = leg1_mu[i][2]; 

						if (     ( lep_pt[whichlep] - leg1_mu[i][2] ) * ( lep_pt[whichlep] - leg1_mu[i][3] )    < 0.     ){ 

							//cout << i << " -- " << leg1_mu[i][2] << " -- " << lep_pt[whichlep] <<  " -- " << leg1_mu[i][3] << endl; 

							line_1 = i; 

							break; 

						}  

					}

					for( int i = line_1; i < line_1+13; i++ ){

						if (     ( lep_eta[whichlep] - leg1_mu[i][0] ) * ( lep_eta[whichlep] - leg1_mu[i][1] )    < 0.     ){ 

							//cout << i << " -- " << leg1_mu[i][0] << " -- " << lep_eta[whichlep] << " -- " << leg1_mu[i][1] << "\n" <<  endl; 

							lineNLO_1 = i; 

							break; 

						}  

					}

					// --------------------------------

					float last_pt_2 = -999; 

					int line_2    = 0; 
					int lineNLO_2 = 0; 

					for( int i = 0; i < 117; i++ ){

						if( last_pt_2 - leg2_mu[i][2] == 0 ) continue; 

						last_pt_2 = leg2_mu[i][2]; 

						if (     ( lep_pt[whichlep] - leg2_mu[i][2] ) * ( lep_pt[whichlep] - leg2_mu[i][3] )    < 0.     ){ 

							//cout << i << " -- " << leg2_mu[i][2] << " -- " << lep_pt[whichlep] <<  " -- " << leg2_mu[i][3] << endl; 

							line_2 = i; 

							break; 

						}  

					}

					for( int i = line_2; i < line_2+13; i++ ){

						if (     ( lep_eta[whichlep] - leg2_mu[i][0] ) * ( lep_eta[whichlep] - leg2_mu[i][1] )    < 0.     ){ 

							//cout << i << " -- " << leg2_mu[i][0] << " -- " << lep_eta[whichlep] << " -- " << leg2_mu[i][1] << "\n" << endl; 

							lineNLO_2 = i; 

							break; 

						}  

					}

					// --------------------------------

					float last_pt_3 = -999; 

					int line_3    = 0; 
					int lineNLO_3 = 0; 

					for( int i = 0; i < 156; i++ ){

						if( last_pt_3 - single_mu[i][2] == 0 ) continue; 

						last_pt_3 = single_mu[i][2]; 

						if (     ( lep_pt[whichlep] - single_mu[i][2] ) * ( lep_pt[whichlep] - single_mu[i][3] )    < 0.     ){ 

							//cout << i << " -- " << single_mu[i][2] << " -- " << lep_pt[whichlep] <<  " -- " << single_mu[i][3] << endl; 

							line_3 = i; 

							break; 

						}  

					}

					for( int i = line_3; i < line_3+13; i++ ){

						if (     ( lep_eta[whichlep] - single_mu[i][0] ) * ( lep_eta[whichlep] - single_mu[i][1] )    < 0.     ){ 

							//cout << i << " -- " << single_mu[i][0] << " -- " << lep_eta[whichlep] << " -- " << single_mu[i][1] << "\n" <<  endl; 

							lineNLO_3 = i; 

							break; 

						}  

					}

					// --------------------------------

					//cout << " trigger: \t double = " << leg1_mu[lineNLO_1][4]*leg2_mu[lineNLO_2][4]<< "\t single = "  << single_mu[lineNLO_3][4]  << endl; 	

					if ( whichlep == 0 ){ 

						PS1 = single_mu[lineNLO_3][4]; 
						P11 = leg1_mu  [lineNLO_1][4];
						P21 = leg2_mu  [lineNLO_2][4]; 

					} 

					if ( whichlep == 1 ){ 

						PS2 = single_mu[lineNLO_3][4]; 
						P12 = leg1_mu  [lineNLO_1][4];
						P22 = leg2_mu  [lineNLO_2][4]; 

					}

				}   // whichlep

				float PS  =  1  -  ( 1-PS1 ) * ( 1-PS2 ); 

				float Pd  =  P11 * P22;  

				float Pi  =  P12 * P21; 

				float PD  =  Pd  +  ( 1-Pd ) * Pi ;

				float effTriggerW = PS + ( 1-PS )*PD;    

				//cout << "\ntrigger SF = " << effTriggerW << "\n\n" << endl; 

				eventW *= effTriggerW; 

			}*/

		}   // !isData[process]



		// ----- k-factor ----------------------------------------
 
		/*if( process == GJets40100 || process == GJets100200 || process == GJets200400|| process == GJets400600 || process == GJets600Inf || process == ZGJets || process == ZGTo2LG ){

			int genindex = -1; 

      			for( int j = 0; j < nGenPart; j++){

	  			if( abs( GenPart_pdgId[j] ) == 22 ){

	      				genindex = j;

	      				break;
	    
				}

			}

			float gamma_pt_GEN = GenPart_pt[genindex];

			if( process == GJets40100 || process == GJets100200 || process == GJets200400|| process == GJets400600 || process == GJets600Inf ){

				eventW *= kfactor_Gjets -> Eval( gamma_pt_GEN );

			}

			else if( process == ZGJets ){

				float TheFactor = kfactor_ZnnG -> GetBinContent( kfactor_ZnnG->FindBin(gamma_pt_GEN) ); 

				if( TheFactor > 1. )  eventW *= TheFactor; 

			}

			else if( process == ZGTo2LG ){

				float TheFactor = kfactor_ZllG -> GetBinContent( kfactor_ZllG->FindBin(gamma_pt_GEN) ); 

				if( TheFactor > 1. )  eventW *= TheFactor; 

			}

			
		}*/

		// -------------------------------------------------------

		float boson_pt  = -999.;
		float boson_phi = -999.;	
	
		if ( TheKanal == Zee || TheKanal == Zmumu ) { 

			TLorentzVector lep1, lep2; 

			lep1.SetPtEtaPhiM( lep1pt, lep1eta, lep1phi, 0 ); 
			lep2.SetPtEtaPhiM( lep2pt, lep2eta, lep2phi, 0 ); 
	
			boson_pt  = (lep1+lep2).Pt() ;  
			boson_phi = (lep1+lep2).Phi();

		 }   

		///if ( TheKanal == Gamma )                    { boson_pt = gamma_pt[0];  boson_phi = gamma_phi[0]; }


		TVector2 qT, ET, uT; 

		float _met_pt, _met_phi; 

		for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){

			if( s == nominal ) { _met_pt = met_pt        ; _met_phi = met_phi;         }
			///if( s == JECu    ) { _met_pt = met_jecUp_pt  ; _met_phi = met_jecUp_phi;   }
			///if( s == JECd    ) { _met_pt = met_jecDown_pt; _met_phi = met_jecDown_phi; }
			///if( s == UnEu    ) { _met_pt = met_UnEup_pt  ; _met_phi = met_UnEup_phi;   }
			///if( s == UnEd    ) { _met_pt = met_UnEdown_pt; _met_phi = met_UnEdown_phi; }

			/////if( s == nominal ) { _met_pt = metPuppi_pt        ; _met_phi = metPuppi_phi;         }
			/////if( s == JECu    ) { _met_pt = metPuppi_jecUp_pt  ; _met_phi = metPuppi_jecUp_phi;   }
			/////if( s == JECd    ) { _met_pt = metPuppi_jecDown_pt; _met_phi = metPuppi_jecDown_phi; }
			/////if( s == UnEu    ) { _met_pt = metPuppi_UnEup_pt  ; _met_phi = metPuppi_UnEup_phi;   }
			/////if( s == UnEd    ) { _met_pt = metPuppi_UnEdown_pt; _met_phi = metPuppi_UnEdown_phi; }

			ET.SetMagPhi( _met_pt  , _met_phi   );

			qT.SetMagPhi( boson_pt, boson_phi );

			uT = -1* ( ET + qT ); 


			//float uPara = (  uT.Px() * qT.Px() + uT.Py() * qT.Py()  ) / qT.Mod();

			//float Scale = -1. * uPara/qT.Mod();   // cout << abs(uPara) << " -- " << qT.Mod() <<  " -- " << Scale << endl; 
	 
			//uPara += qT.Mod();   // cout <<  uPara << endl;

			//float uPerp = (  uT.Px() * qT.Py() - uT.Py() * qT.Px()  ) / qT.Mod();


			//if(run==273158 && s==nominal) sync << Form( "*%12llu*%12f*%12f*\n", evt, uPara, uPerp );


			h_global_W     [parall_G][process][s]    -> Fill( uPara   , eventW );  
			h_global_W     [transv_G][process][s]    -> Fill( uPerp   , eventW );
			h_global_W     [MET     ][process][s]    -> Fill( _met_pt , eventW );
			h_global_W     [VpT     ][process][s]    -> Fill( boson_pt, eventW );
			if(  isData[process] ) h_global_W[nvert   ][process][s] -> Fill( nVert   , eventW );
			if( !isData[process] ) h_global_W[nvert   ][process][s] -> Fill( nTrueInt, eventW );

			if(  isData[process] ){ if( boson_pt < minpT || boson_pt >= maxpT || /*met_sumEt < minsumET || met_sumEt >= maxsumET ||*/ nVert < minNVtx || nVert >= maxNVtx) continue;}
			if( !isData[process] ){ if( boson_pt < minpT || boson_pt >= maxpT || /*met_sumEt < minsumET || met_sumEt >= maxsumET ||*/ nTrueInt < minNVtx || nTrueInt >= maxNVtx) continue;}
			int l = floor(    nbinpT    * (boson_pt    - minpT   ) / (maxpT    - minpT   )    ); //cout << " l = " << l << endl;
			int m = 0; ///floor(    nbinsumET * (met_sumEt   - minsumET) / (maxsumET - minsumET)    ); //cout << " m = " << m << endl;
			int n;
			if(  isData[process] ) n = floor(    nbinNVtx  * (nVert       - minNVtx ) / (maxNVtx  - minNVtx )    );
			if( !isData[process] ) n = floor(    nbinNVtx  * (nTrueInt    - minNVtx ) / (maxNVtx  - minNVtx )    );
			//cout << "\t" << n << endl; 

			h_resol_pT_W   [parall_R][process][l][s] -> Fill( uPara , eventW ); 
			h_resol_pT_W   [transv_R][process][l][s] -> Fill( uPerp , eventW );	
			h_resol_pT_W   [scale   ][process][l][s] -> Fill( Scale , eventW );	

			h_resol_sumET_W[parall_R][process][m][s] -> Fill( uPara , eventW ); 
			h_resol_sumET_W[transv_R][process][m][s] -> Fill( uPerp , eventW );
			h_resol_sumET_W[scale   ][process][m][s] -> Fill( Scale , eventW );

			h_resol_NVtx_W [parall_R][process][n][s] -> Fill( uPara , eventW );
			h_resol_NVtx_W [transv_R][process][n][s] -> Fill( uPerp , eventW );
			h_resol_NVtx_W [scale   ][process][n][s] -> Fill( Scale , eventW );

		}   // systematic 

	}   // ievt

	//sync.close();

}
