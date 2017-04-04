#include "MET.h"

void MET3(){

	if( DoFit == false ) { cout << "   >>   OrderFits deactivated !!!   " << endl;  return; };

	Assign(); 


	// load histograms

	TFile* allhistos = new TFile( "histograms/" + allhistosReadFrom + ".root", "read" );


	for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){


		for( int i = 0; i < nvariable_R; i++ ){

			//for( int j = 0; j < nprocess; j++ ){
			for( int j = alpha; j < omega+1; j++ ){ 

				//if (  j == DoubleEG2016E     || j == DoubleEG2016F     ) continue; 
				//if (  j == DoubleMuon2016E   || j == DoubleMuon2016F   ) continue; 
				//if (  j == SinglePhoton2016E || j == SinglePhoton2016F ) continue;

				/*if ( j == DoubleEG2016B     || j == DoubleEG2016C     || j == DoubleEG2016D     ) continue; 
				if ( j == DoubleMuon2016B   || j == DoubleMuon2016C   || j == DoubleMuon2016D   ) continue; 
				if ( j == SinglePhoton2016B || j == SinglePhoton2016C || j == SinglePhoton2016D ) continue;*/
 
				int TheKanal = kanal[j];

				for( int k = 0; k < nbinpT   ; k++){

					h_resol_pT   [i][j][k][s] = (TH1F*) allhistos -> Get( Form("h_resol_pT_"    + variableID_R[i] + "_" + kanalID[TheKanal] + "_" + processID[j] + "_%d_" + systematicID[s], k) );
				}

				///for( int k = 0; k < nbinsumET; k++){	

				///	h_resol_sumET[i][j][k][s] = (TH1F*) allhistos -> Get( Form("h_resol_sumET_" + variableID_R[i] + "_" + kanalID[TheKanal] + "_"+ processID[j] + "_%d_" + systematicID[s], k) );

				///}

				for( int k = 0; k < nbinNVtx ; k++){	

					h_resol_NVtx [i][j][k][s] = (TH1F*) allhistos -> Get( Form("h_resol_NVtx_"  + variableID_R[i] + "_" + kanalID[TheKanal] + "_"+ processID[j] + "_%d_" + systematicID[s], k) );

				}

			}

		}


		// create all-background template for the fit procedure

		for( int k = 0; k < nbinpT; k++ ){

			for( int m = 0; m < nkanal; m++ ){
			//for( int m = 0; m < 1; m++ ){

				h_resol_pT_fit   [parall_R][0][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPara, minuPara, maxuPara );
	 			h_resol_pT_fit   [parall_R][1][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPara, minuPara, maxuPara );
	 			h_resol_pT_fit   [parall_R][2][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPara, minuPara, maxuPara );
				h_resol_pT_fit   [transv_R][0][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPerp, minuPerp, maxuPerp ); 
				h_resol_pT_fit   [transv_R][1][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPerp, minuPerp, maxuPerp ); 
				h_resol_pT_fit   [transv_R][2][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPerp, minuPerp, maxuPerp ); 
				h_resol_pT_fit   [scale   ][0][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinscale, minscale, maxscale ); 
				h_resol_pT_fit   [scale   ][1][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinscale, minscale, maxscale ); 
				h_resol_pT_fit   [scale   ][2][k][m][s] = new TH1F( Form("h_resol_pT_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinscale, minscale, maxscale );

			}

		}

		/*for( int k = 0; k < nbinsumET; k++ ){

			for( int m = 0; m < nkanal; m++ ){
			//for( int m = 0; m < 1; m++ ){

				h_resol_sumET_fit[parall_R][0][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[parall_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPara, minuPara, maxuPara );
		 		h_resol_sumET_fit[parall_R][1][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[parall_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPara, minuPara, maxuPara );
	 			h_resol_sumET_fit[parall_R][2][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[parall_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPara, minuPara, maxuPara );
				h_resol_sumET_fit[transv_R][0][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[transv_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPerp, minuPerp, maxuPerp );
				h_resol_sumET_fit[transv_R][1][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[transv_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPerp, minuPerp, maxuPerp );
				h_resol_sumET_fit[transv_R][2][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[transv_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPerp, minuPerp, maxuPerp );
				h_resol_sumET_fit[scale   ][0][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[scale   ] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinscale, minscale, maxscale ); 
				h_resol_sumET_fit[scale   ][1][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[scale   ] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinscale, minscale, maxscale ); 
				h_resol_sumET_fit[scale   ][2][k][m][s] = new TH1F( Form("h_resol_sumET_" + variableID_R[scale   ] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinscale, minscale, maxscale );

			}

		}*/

		for( int k = 0; k < nbinNVtx; k++ ){

			for( int m = 0; m < nkanal; m++ ){
			//for( int m = 0; m < 1; m++ ){

				h_resol_NVtx_fit [parall_R][0][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPara, minuPara, maxuPara );
		 		h_resol_NVtx_fit [parall_R][1][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPara, minuPara, maxuPara );
	 			h_resol_NVtx_fit [parall_R][2][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[parall_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPara, minuPara, maxuPara );
				h_resol_NVtx_fit [transv_R][0][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinuPerp, minuPerp, maxuPerp );
				h_resol_NVtx_fit [transv_R][1][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinuPerp, minuPerp, maxuPerp );
				h_resol_NVtx_fit [transv_R][2][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[transv_R] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinuPerp, minuPerp, maxuPerp );
				h_resol_NVtx_fit [scale   ][0][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_data_%d_" + systematicID[s],   k), "data   ", nbinscale, minscale, maxscale ); 
				h_resol_NVtx_fit [scale   ][1][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_allbkg_%d_" + systematicID[s], k), "all bkg", nbinscale, minscale, maxscale ); 
				h_resol_NVtx_fit [scale   ][2][k][m][s] = new TH1F( Form("h_resol_NVtx_"  + variableID_R[scale   ] + "_" + kanalID[m] + "_signal_%d_" + systematicID[s], k), "signal ", nbinscale, minscale, maxscale );

			}

		}



		for( int i = 0; i < nvariable_R; i++ ){

			for( int k = 0; k < nbinpT; k++ ){

				h_resol_pT_fit   [i][0][k][Zee][s] -> Add( h_resol_pT   [i][data_ee][k][s] );
				h_resol_pT_fit   [i][2][k][Zee][s] -> Add( h_resol_pT   [i][DY_ee  ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Zee][s] -> Add( h_resol_pT   [i][TT_ee  ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Zee][s] -> Add( h_resol_pT   [i][EW_ee  ][k][s] );

				h_resol_pT_fit   [i][0][k][Zmumu][s] -> Add( h_resol_pT   [i][data_mm][k][s] ); 
				h_resol_pT_fit   [i][2][k][Zmumu][s] -> Add( h_resol_pT   [i][DY_mm  ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Zmumu][s] -> Add( h_resol_pT   [i][TT_mm  ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Zmumu][s] -> Add( h_resol_pT   [i][EW_mm  ][k][s] ); 

				/*h_resol_pT_fit   [i][0][k][Gamma][s] -> Add( h_resol_pT   [i][SinglePhoton2016B][k][s] ); 
				h_resol_pT_fit   [i][0][k][Gamma][s] -> Add( h_resol_pT   [i][SinglePhoton2016C][k][s] ); 
				h_resol_pT_fit   [i][0][k][Gamma][s] -> Add( h_resol_pT   [i][SinglePhoton2016D][k][s] ); 
				//h_resol_pT_fit   [i][0][k][Gamma][s] -> Add( h_resol_pT   [i][SinglePhoton2016E][k][s] ); 
				//h_resol_pT_fit   [i][0][k][Gamma][s] -> Add( h_resol_pT   [i][SinglePhoton2016F][k][s] ); 
				h_resol_pT_fit   [i][2][k][Gamma][s] -> Add( h_resol_pT   [i][GJets40100 ][k][s] ); 
				h_resol_pT_fit   [i][2][k][Gamma][s] -> Add( h_resol_pT   [i][GJets100200][k][s] ); 
				h_resol_pT_fit   [i][2][k][Gamma][s] -> Add( h_resol_pT   [i][GJets200400][k][s] ); 
				h_resol_pT_fit   [i][2][k][Gamma][s] -> Add( h_resol_pT   [i][GJets400600][k][s] ); 
				h_resol_pT_fit   [i][2][k][Gamma][s] -> Add( h_resol_pT   [i][GJets600Inf][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD200300      ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD300500      ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD500700      ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD7001000     ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD10001500    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD15002000    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][QCD2000Inf     ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets100200    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets200400    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets400600    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets600800    ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets8001200   ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets12002500  ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WJets2500Inf   ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][ZGJets         ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][ZNuNuGJets40130][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][ZGTo2LG        ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WGJets         ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][WGToLNuG       ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][TTGJets        ][k][s] ); 
				h_resol_pT_fit   [i][1][k][Gamma][s] -> Add( h_resol_pT   [i][TGJets         ][k][s] );*/ 

			}

			/*for( int k = 0; k < nbinsumET; k++ ){

				h_resol_sumET_fit   [i][0][k][Zee][s] -> Add( h_resol_sumET   [i][DoubleEG2016B][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zee][s] -> Add( h_resol_sumET   [i][DoubleEG2016C][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zee][s] -> Add( h_resol_sumET   [i][DoubleEG2016D][k][s] ); 
				//h_resol_sumET_fit   [i][0][k][Zee][s] -> Add( h_resol_sumET   [i][DoubleEG2016E][k][s] ); 
				//h_resol_sumET_fit   [i][0][k][Zee][s] -> Add( h_resol_sumET   [i][DoubleEG2016F][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Zee][s] -> Add( h_resol_sumET   [i][DY_ee        ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][TT_ee        ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][TTsemiT_ee   ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][TTsemiTbar_ee][k][s] );
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WW_ee        ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WZTo2L2Q_ee  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WZTo3LNu_ee  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][ZZTo4L_ee    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][ZZTo2L2Q_ee  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][ZZTo2L2Nu_ee ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WWW_ee       ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WWZ_ee       ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][WZZ_ee       ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][ZZZ_ee       ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][SingleTop1_ee][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][SingleTop2_ee][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][SingleTop3_ee][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][SingleTop4_ee][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zee][s] -> Add( h_resol_sumET   [i][SingleTop5_ee][k][s] );


				h_resol_sumET_fit   [i][0][k][Zmumu][s] -> Add( h_resol_sumET   [i][DoubleMuon2016B][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zmumu][s] -> Add( h_resol_sumET   [i][DoubleMuon2016C][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zmumu][s] -> Add( h_resol_sumET   [i][DoubleMuon2016D][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zmumu][s] -> Add( h_resol_sumET   [i][DoubleMuon2016E][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Zmumu][s] -> Add( h_resol_sumET   [i][DoubleMuon2016F][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Zmumu][s] -> Add( h_resol_sumET   [i][DY_mm          ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][TT_mm          ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][TTsemiT_mm     ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][TTsemiTbar_mm  ][k][s] );
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WW_mm          ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WZTo2L2Q_mm    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WZTo3LNu_mm    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][ZZTo4L_mm      ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][ZZTo2L2Q_mm    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][ZZTo2L2Nu_mm   ][k][s] );
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WWW_mm         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WWZ_mm         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][WZZ_mm         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][ZZZ_mm         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][SingleTop1_mm  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][SingleTop2_mm  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][SingleTop3_mm  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][SingleTop4_mm  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Zmumu][s] -> Add( h_resol_sumET   [i][SingleTop5_mm  ][k][s] ); 


				h_resol_sumET_fit   [i][0][k][Gamma][s] -> Add( h_resol_sumET   [i][SinglePhoton2016B][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Gamma][s] -> Add( h_resol_sumET   [i][SinglePhoton2016C][k][s] ); 
				h_resol_sumET_fit   [i][0][k][Gamma][s] -> Add( h_resol_sumET   [i][SinglePhoton2016D][k][s] ); 
				//h_resol_sumET_fit   [i][0][k][Gamma][s] -> Add( h_resol_sumET   [i][SinglePhoton2016E][k][s] ); 
				//h_resol_sumET_fit   [i][0][k][Gamma][s] -> Add( h_resol_sumET   [i][SinglePhoton2016F][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Gamma][s] -> Add( h_resol_sumET   [i][GJets40100 ][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Gamma][s] -> Add( h_resol_sumET   [i][GJets100200][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Gamma][s] -> Add( h_resol_sumET   [i][GJets200400][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Gamma][s] -> Add( h_resol_sumET   [i][GJets400600][k][s] ); 
				h_resol_sumET_fit   [i][2][k][Gamma][s] -> Add( h_resol_sumET   [i][GJets600Inf][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD200300      ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD300500      ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD500700      ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD7001000     ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD10001500    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD15002000    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][QCD2000Inf     ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets100200    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets200400    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets400600    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets600800    ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets8001200   ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets12002500  ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WJets2500Inf   ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][ZGJets         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][ZNuNuGJets40130][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][ZGTo2LG        ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WGJets         ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][WGToLNuG       ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][TTGJets        ][k][s] ); 
				h_resol_sumET_fit   [i][1][k][Gamma][s] -> Add( h_resol_sumET   [i][TGJets         ][k][s] );

			}*/

			for( int k = 0; k < nbinNVtx; k++ ){

				h_resol_NVtx_fit   [i][0][k][Zee][s] -> Add( h_resol_NVtx   [i][data_ee][k][s] );
				h_resol_NVtx_fit   [i][2][k][Zee][s] -> Add( h_resol_NVtx   [i][DY_ee  ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Zee][s] -> Add( h_resol_NVtx   [i][TT_ee  ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Zee][s] -> Add( h_resol_NVtx   [i][EW_ee  ][k][s] );

				h_resol_NVtx_fit   [i][0][k][Zmumu][s] -> Add( h_resol_NVtx   [i][data_mm][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Zmumu][s] -> Add( h_resol_NVtx   [i][DY_mm  ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Zmumu][s] -> Add( h_resol_NVtx   [i][TT_mm  ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Zmumu][s] -> Add( h_resol_NVtx   [i][EW_mm  ][k][s] );


				/*h_resol_NVtx_fit   [i][0][k][Gamma][s] -> Add( h_resol_NVtx   [i][SinglePhoton2016B][k][s] ); 
				h_resol_NVtx_fit   [i][0][k][Gamma][s] -> Add( h_resol_NVtx   [i][SinglePhoton2016C][k][s] ); 
				h_resol_NVtx_fit   [i][0][k][Gamma][s] -> Add( h_resol_NVtx   [i][SinglePhoton2016D][k][s] ); 
				//h_resol_NVtx_fit   [i][0][k][Gamma][s] -> Add( h_resol_NVtx   [i][SinglePhoton2016E][k][s] ); 
				//h_resol_NVtx_fit   [i][0][k][Gamma][s] -> Add( h_resol_NVtx   [i][SinglePhoton2016F][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Gamma][s] -> Add( h_resol_NVtx   [i][GJets40100 ][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Gamma][s] -> Add( h_resol_NVtx   [i][GJets100200][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Gamma][s] -> Add( h_resol_NVtx   [i][GJets200400][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Gamma][s] -> Add( h_resol_NVtx   [i][GJets400600][k][s] ); 
				h_resol_NVtx_fit   [i][2][k][Gamma][s] -> Add( h_resol_NVtx   [i][GJets600Inf][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD200300      ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD300500      ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD500700      ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD7001000     ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD10001500    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD15002000    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][QCD2000Inf     ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets100200    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets200400    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets400600    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets600800    ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets8001200   ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets12002500  ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WJets2500Inf   ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][ZGJets         ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][ZNuNuGJets40130][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][ZGTo2LG        ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WGJets         ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][WGToLNuG       ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][TTGJets        ][k][s] ); 
				h_resol_NVtx_fit   [i][1][k][Gamma][s] -> Add( h_resol_NVtx   [i][TGJets         ][k][s] );*/ 

			}

		}


	}   // systematic

	 
	// fits & final plots 

	float ySR[3][7][nkanal];   // { zentral, error, chi2 (or error-mc in data/mc) } x { bkg-substraction(=data), pure-mc(=mc), data/mc, JEC-up, JEC-do, UnEu, UnEd } x { Zee, Zmumu, Gamma }

	for( int i = 0; i < nvariable_R; i++){
	//	for( int i = 0; i < 1; i++){

		for( int a = 0; a < 3; a++      ){   // histos
		for( int k = 0; k < 2; k++ ){   // for( int k = 0; k < nkanal; k++ ){

			h_SR[a][pT   ][k] = new TH1F( "h_SR_" + histoID[a] + "_" + parameterID[pT   ] + "_" + kanalID[k], "h_SR pT"   ,  nbinpT   , minpT   , maxpT    );
			//h_SR[a][sumET][k] = new TH1F( "h_SR_" + histoID[a] + "_" + parameterID[sumET] + "_" + kanalID[k], "h_SR sumET",  nbinsumET, minsumET, maxsumET );
			h_SR[a][NVtx ][k] = new TH1F( "h_SR_" + histoID[a] + "_" + parameterID[NVtx ] + "_" + kanalID[k], "h_SR NVtx ",  nbinNVtx , minNVtx , maxNVtx  );

		}}

		// ----- resolution: photon pT

		float current_pT;

		for( int k = 0; k < nbinpT; k++ ){

			current_pT = minpT + (1.0*k+0.5)*(maxpT-minpT)/nbinpT; 

			for( int m = 0; m < 2; m++ ){   // for( int m = 0; m < nkanal; m++ ){

				if(  m == Gamma  &&  current_pT < 50.  ) { 

					ySR[0][0][m] = 9999; 
					ySR[1][0][m] = 9999; 
					ySR[0][2][m] = 9999;
					ySR[1][2][m] = 9999;
					ySR[2][2][m] = 9999;

				}

				else {

					GetResolution( m  , i, pT, k, ySR[0][0][m],    ySR[1][0][m],    ySR[2][0][m], "GJetsVoigtian", nominal );
					GetResolution( m  , i, pT, k, ySR[0][1][m],    ySR[1][1][m],    ySR[2][1][m], "GJetsFromMC"  , nominal );
					GetResolution( m  , i, pT, k, ySR[0][3][m],    ySR[1][3][m],    ySR[2][3][m], "GJetsVoigtian", nominal/*JECu*/    );   // JEC-up
					GetResolution( m  , i, pT, k, ySR[0][4][m],    ySR[1][4][m],    ySR[2][4][m], "GJetsVoigtian", nominal/*JECd*/    );   // JEC-do
					GetResolution( m  , i, pT, k, ySR[0][5][m],    ySR[1][5][m],    ySR[2][5][m], "GJetsVoigtian", nominal/*UnEu*/    );   // UnE-up 
					GetResolution( m  , i, pT, k, ySR[0][6][m],    ySR[1][6][m],    ySR[2][6][m], "GJetsVoigtian", nominal/*UnEd*/    );   // UnE-do


					GetRatioValues( ySR[0][0][m], ySR[1][0][m], ySR[0][1][m], ySR[1][1][m], ySR[0][3][m], ySR[0][4][m], ySR[0][5][m], ySR[0][6][m], ySR[0][2][m], ySR[1][2][m], ySR[2][2][m] );

					h_SR[0][pT][m] -> SetBinContent( k+1, ySR[0][0][m] );
					h_SR[0][pT][m] -> SetBinError  ( k+1, ySR[1][0][m] );
					h_SR[1][pT][m] -> SetBinContent( k+1, ySR[0][2][m] );
					h_SR[1][pT][m] -> SetBinError  ( k+1, ySR[1][2][m] );
					h_SR[2][pT][m] -> SetBinContent( k+1, 1.0          );
					h_SR[2][pT][m] -> SetBinError  ( k+1, ySR[2][2][m] );     


				}

			}

		}
		



		// ----- resolution: sumEt

		// ... 



		// ----- resolution: number of vertices

		for( int k = 0; k < nbinNVtx; k++ ){

			for( int m = 0; m < 2; m++ ){//for( int m = 0; m < nkanal; m++ ){

				GetResolution( m  , i, NVtx, k, ySR[0][0][m],    ySR[1][0][m],    ySR[2][0][m], "GJetsVoigtian", nominal );
				GetResolution( m  , i, NVtx, k, ySR[0][1][m],    ySR[1][1][m],    ySR[2][1][m], "GJetsFromMC"  , nominal );
				GetResolution( m  , i, NVtx, k, ySR[0][3][m],    ySR[1][3][m],    ySR[2][3][m], "GJetsVoigtian", nominal/*JECu*/    );   // JEC-up
				GetResolution( m  , i, NVtx, k, ySR[0][4][m],    ySR[1][4][m],    ySR[2][4][m], "GJetsVoigtian", nominal/*JECd*/    );   // JEC-do
				GetResolution( m  , i, NVtx, k, ySR[0][5][m],    ySR[1][5][m],    ySR[2][5][m], "GJetsVoigtian", nominal/*UnEu*/    );   // UnE-up 
				GetResolution( m  , i, NVtx, k, ySR[0][6][m],    ySR[1][6][m],    ySR[2][6][m], "GJetsVoigtian", nominal/*UnEd*/    );   // UnE-do

				float e_mc; 

		GetRatioValues( ySR[0][0][m], ySR[1][0][m], ySR[0][1][m], ySR[1][1][m], ySR[0][3][m], ySR[0][4][m], ySR[0][5][m], ySR[0][6][m], ySR[0][2][m], ySR[1][2][m], e_mc );


				h_SR[0][NVtx][m] -> SetBinContent( k+1, ySR[0][0][m] );
				h_SR[0][NVtx][m] -> SetBinError  ( k+1, ySR[1][0][m] );
				h_SR[1][NVtx][m] -> SetBinContent( k+1, ySR[0][2][m] );
				h_SR[1][NVtx][m] -> SetBinError  ( k+1, ySR[1][2][m] );
				h_SR[2][NVtx][m] -> SetBinContent( k+1, 1.0          );
				h_SR[2][NVtx][m] -> SetBinError  ( k+1, ySR[1][2][m] );  
			}

		}


		// ---------------------------------------------

		PlotResolution( i, pT    );  cout << "final plot for " << parameterID[pT   ]  << " done !! " << endl;
		//PlotResolution( i, sumET );  cout << "final plot for " << parameterID[sumET]  << " done !! " << endl;
		PlotResolution( i, NVtx  );  cout << "final plot for " << parameterID[NVtx ]  << " done !! " << endl;

		// ---------------------------------------------

	}

	cout << "                 " << endl; 
	cout << "exiting OrderFits" << endl; 
	cout << "                 " << endl; 
}




void PlotResolution( int ivar, int param ){

	h_SR[0][param][0] -> SetMarkerStyle( 22 );   //h_SR[0][param][0] -> SetMarkerSize( 1.2*h_SR[0][param][0]->GetMarkerSize() );
	h_SR[0][param][1] -> SetMarkerStyle( 23 );   //h_SR[0][param][1] -> SetMarkerSize( 1.2*h_SR[0][param][0]->GetMarkerSize() );
	//h_SR[0][param][2] -> SetMarkerStyle( 21 );

	h_SR[1][param][0] -> SetMarkerStyle( 22 );   //h_SR[1][param][0] -> SetMarkerSize( 1.2*h_SR[1][param][0]->GetMarkerSize() );
	h_SR[1][param][1] -> SetMarkerStyle( 23 );   //h_SR[1][param][1] -> SetMarkerSize( 1.2*h_SR[1][param][0]->GetMarkerSize() );
	//h_SR[1][param][2] -> SetMarkerStyle( 21 );


	h_SR[0][param][0] -> SetMarkerColor( kBlue );
	h_SR[0][param][1] -> SetMarkerColor( kRed  );
	//h_SR[0][param][2] -> SetMarkerColor( kGreen);

	h_SR[1][param][0] -> SetMarkerColor( kBlue );
	h_SR[1][param][1] -> SetMarkerColor( kRed  );
	//h_SR[1][param][2] -> SetMarkerColor( kGreen);

	
	h_SR[0][param][0] -> SetLineColor( kBlue  );
	h_SR[0][param][1] -> SetLineColor( kRed   );
	//h_SR[0][param][2] -> SetLineColor( kGreen );

	h_SR[1][param][0] -> SetLineColor( kBlue  );
	h_SR[1][param][1] -> SetLineColor( kRed   );
	//h_SR[1][param][2] -> SetLineColor( kGreen );


	h_SR[0][param][0] -> SetFillStyle(0);
	h_SR[0][param][1] -> SetFillStyle(0);
	//h_SR[0][param][2] -> SetFillStyle(0);

	h_SR[1][param][0] -> SetFillStyle(0);
	h_SR[1][param][1] -> SetFillStyle(0);
	//h_SR[1][param][2] -> SetFillStyle(0);


	h_SR[2][param][0] -> SetLineColor(11);
	h_SR[2][param][1] -> SetLineColor(11);
	//h_SR[2][param][2] -> SetLineColor(11);

	h_SR[2][param][0] -> SetFillColor(11);
	h_SR[2][param][1] -> SetFillColor(11);
	//h_SR[2][param][2] -> SetFillColor(11);

	//----------------------------------------------------------

	TCanvas* c = new TCanvas("resol_" + variableID_R[ivar] + "_" + parameterID[param], variableID_R[ivar] + "_" + parameterID[param], 600, 600);		

	TPad* pad1 = new TPad("pad1", "pad1", 0.05, 0.0, 1.00, 0.3);
	TPad* pad2 = new TPad("pad2", "pad2", 0.05, 0.3, 1.00, 1.0);

	pad1 -> SetTopMargin   (0.08);
	pad1 -> SetBottomMargin(0.35);
	pad1 -> Draw();

	pad2 -> SetTopMargin   (0.08);
	pad2 -> SetBottomMargin(0.02);
	pad2 -> Draw();

	//----------------------------------------------------------

	pad2 -> cd();

	h_SR[0][param][Zee] -> SetTitle("");

	h_SR[0][param][Zee] -> SetStats(false);

 	SetAxis( h_SR[0][param][Zee], "", sigma_variable[ivar], 1.5, 1.0 );

	( ivar < 2 )  ?  h_SR[0][param][Zee] -> GetYaxis() -> SetRangeUser(0., 40.)  :  h_SR[0][param][Zee] -> GetYaxis() -> SetRangeUser(0., 2.5);

	h_SR[0][param][Zee  ] -> Draw("e"      );	
	h_SR[0][param][Zmumu] -> Draw("e, same");
	//h_SR[0][param][2] -> Draw("e, same");

	DrawLatex( 61, 0.100, 0.945, 0.050, 11, "CMS"                                             );

     	DrawLatex( 52, 0.205, 0.945, 0.030, 11, "Preliminary"                                     );

	DrawLatex( 42, 0.900, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV, 2016)", TheUnifiedLuminosity) );

  	pad2 -> RedrawAxis();

	//----------------------------------------------------------

	TLegend* TheLegend = new TLegend( 0.18, 0.65, 0.38, 0.88 );

	TheLegend -> SetBorderSize(0);

	TheLegend -> SetTextSize(0.050);

	TheLegend -> AddEntry( h_SR[0][param][Zee  ], kanalIDfancy[Zee  ], "lp" ); 
	TheLegend -> AddEntry( h_SR[0][param][Zmumu], kanalIDfancy[Zmumu], "lp" ); 
	//TheLegend -> AddEntry( h_SR[0][param][2], kanalIDfancy[2], "lp" ); 

	TheLegend -> Draw();

	//----------------------------------------------------------

	pad1 -> cd();

	h_SR[1][param][Zee] -> SetTitle("");
	h_SR[1][param][Zee] -> SetStats(false);   // it has priority over the gStyle->SetOptStats option

 	SetAxis( h_SR[1][param][Zee], parameterIDfancy[param], "data / MC ", 1.4, 0.5 );

	h_SR[1][param][Zee] -> GetYaxis() -> SetRangeUser(0.85, 1.15);
      	//( ivar < 2 )  ?  h_SR[1][param][0] -> GetYaxis() -> SetRangeUser(0.85, 1.15)  :  h_SR[1][param][0] -> GetYaxis() -> SetRangeUser(0.5, 2.0);

	//h_SR[1][param][Zmumu] -> Draw("ep"      );
	//h_SR[2][param][Zmumu] -> Draw("e2, same");
	//h_SR[1][param][Zmumu] -> Draw("ep, same");

	h_SR[1][param][0] -> Draw("ep      ");
	h_SR[2][param][0] -> Draw("e2, same");
	h_SR[2][param][1] -> Draw("e2, same");
	//h_SR[2][param][2] -> Draw("e2, same");
	h_SR[1][param][1] -> Draw("ep, same");
	//h_SR[1][param][2] -> Draw("ep, same");
	h_SR[1][param][0] -> Draw("ep, same");

	pad1 -> RedrawAxis();

	//----------------------------------------------------------

	c -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/resol/" + variableID_R[ivar] + "_" + parameterID[param] + ".pdf" ); 
	c -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/resol/" + variableID_R[ivar] + "_" + parameterID[param] + ".png" ); 

}


void GetResolution( int ch, int whichvar, int parameter, int ibin, float& resol, float& eresol, float& chichi, TString GJetsOrigin, int syst ){

	int ivar = whichvar; 

	float tacon, liga, mediasnegras; 

	if( ivar < 2 ) { tacon = -10.; liga = 10.; mediasnegras = 0.; }
	if( ivar ==2 ) { tacon =   0.; liga =  2.; mediasnegras = 1.; }

	
	RooRealVar x           ( "variable"   , "variable"          ,  0  , -500, 500 );   // random variable; redefine its range
	RooRealVar Gauss_mean  ( "Gauss_mean" , "Gauss mean"        ,  mediasnegras, tacon, liga );  
	RooRealVar BW_gamma    ( "BW_gamma"   , "Breit-Wigner gamma",  2.3,    0, 100 );  
	RooRealVar Gauss_sigma ( "Gauss_sigma", "Gauss sigma"       , 10  ,    0, 100 );  


	TH1F* h1;
	TH1F* h2; 
	TH1F* h3;

	int min; int width; 

	if( parameter == pT    ){

		h1 = (TH1F*)  h_resol_pT_fit[ivar][0][ibin][ch][syst] -> Clone();
		h2 = (TH1F*)  h_resol_pT_fit[ivar][1][ibin][ch][syst] -> Clone();
		h3 = (TH1F*)  h_resol_pT_fit[ivar][2][ibin][ch][syst] -> Clone();

		min = minpT; 
		width = (maxpT - minpT)/nbinpT; 

	}

	if( parameter == sumET ){

		h1 = (TH1F*)  h_resol_sumET_fit[ivar][0][ibin][ch][syst] -> Clone();
		h2 = (TH1F*)  h_resol_sumET_fit[ivar][1][ibin][ch][syst] -> Clone();
		h3 = (TH1F*)  h_resol_sumET_fit[ivar][2][ibin][ch][syst] -> Clone();

		min = minsumET; 
		width = (maxsumET - minsumET)/nbinsumET; 

	}

	if( parameter == NVtx  ){

		h1 = (TH1F*)  h_resol_NVtx_fit[ivar][0][ibin][ch][syst] -> Clone();
		h2 = (TH1F*)  h_resol_NVtx_fit[ivar][1][ibin][ch][syst] -> Clone();
		h3 = (TH1F*)  h_resol_NVtx_fit[ivar][2][ibin][ch][syst] -> Clone();

		min = minNVtx; 
		width = (maxNVtx - minNVtx)/nbinNVtx; 

	}

	int low = min + ibin*width; //cout << low << endl;
	int up  = low + width; 

	// convert TH1F to RooDataHist
	RooDataHist  data( "data" , "data"      , x,  h1 ); 
	RooDataHist  bckg( "bkg"  , "background", x,  h2 );
	RooDataHist gjets( "gjets", "gammajets" , x,  h3 );


	// convert  RooDataHist into template (PDF)
	RooHistPdf bkgpdf  ( "bkgpdf"  , "bkgpdf"  , x, bckg  );
	RooHistPdf gjetspdf( "gjetspdf", "gjetspdf", x, gjets );

	// pure PDF (do not import signal, but recreate it)
	RooVoigtian voigt ("", "", x, Gauss_mean, BW_gamma, Gauss_sigma ); 

	// fraction 
	RooRealVar fbkg( "fbkg", "bkg fraction", 0.5, 0., 1. );

	// model = (1-f)*voigt + f*bkg 
	RooAddPdf modelA( "modelA", "modelA", RooArgList( voigt   , bkgpdf ), fbkg); 
	RooAddPdf modelB( "modelB", "modelB", RooArgList( gjetspdf, bkgpdf ), fbkg); 


	RooFitResult* result; 

	int DoVerbose = -1;	if(  syst == nominal  &&  ch == Zee  ) DoVerbose = 1; 

	if( ch <  Gamma  &&  ivar <  scale )   x.setRange( "ref", -80., 80. );
	if( ch <  Gamma  &&  ivar == scale )   x.setRange( "ref", -0.5, 2.5 );
	if( ch == Gamma  &&  ivar <  scale )   x.setRange( "ref", -80., 80. );
	if( ch == Gamma  &&  ivar == scale )   x.setRange( "ref",  0.0, 2.0 );

	if( GJetsOrigin == "GJetsVoigtian" ) result = modelA.fitTo( data , Extended(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(DoVerbose) );   
	if( GJetsOrigin == "GJetsFromMC"   ) result =  voigt.fitTo( gjets, Extended(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(DoVerbose) );   

	// --------  -------  -------  -------  -------  -------  -------  -------  ------- 

	double sigma  = Gauss_sigma.getVal()  ;   double gamma  = BW_gamma.getVal()   ;
  	double esigma = Gauss_sigma.getError();   double egamma = BW_gamma.getError ();

	double  scala  = Gauss_mean.getVal();
	double escala = Gauss_mean.getError();

  	double Vss = result -> correlation( Gauss_sigma, Gauss_sigma );  double Vsg = result -> correlation( Gauss_sigma, BW_gamma );
  	double Vgs = result -> correlation( BW_gamma   , Gauss_sigma );  double Vgg = result -> correlation( BW_gamma   , BW_gamma );
  	
 	double FWHM  = GetFWHM     (sigma, gamma                                    );
  	double eFWHM = GetFWHMerror(sigma, gamma, esigma, egamma, Vss, Vsg, Vgs, Vgg);


	if(  ivar == parall_R  ||  ivar == transv_R  ) {

		resol  =  FWHM/2.3546;  
		eresol = eFWHM/2.3546;

	}

	if(  ivar == scale  ) {

		resol  =  scala;  
		eresol = escala;

	}
 
	// --------  -------  -------  -------  -------  -------  -------  -------  ------- 

	if( GJetsOrigin == "GJetsFromMC" ) return; 

	TCanvas* mycanvas = new TCanvas("c", "c", 600, 600);

	RooPlot* frame = x.frame( Title(" ") );

	frame -> GetXaxis() -> SetRangeUser( -200, 200 );

	frame -> GetXaxis() -> SetTitle(variableIDfancy_R[ivar]);

	data.plotOn( frame );

	modelA.plotOn( frame, Components(bkgpdf), LineColor(kRed)  , LineStyle(kDashed), FillColor(kRed)  , DrawOption("F") );

	modelA.plotOn( frame, Components(voigt) , LineColor(kGreen), LineStyle(kDashed), FillColor(kGreen), DrawOption("L") );

	modelA.plotOn( frame,                     LineColor(kBlue) , LineStyle(kDashed), FillColor(kBlue) , DrawOption("L") );

	frame -> Draw();

	float chi2 = frame -> chiSquare();

	chichi = chi2; 

	int ndof = data.numEntries() - modelA.getParameters(data)->selectByAttrib( "Constant", kFALSE )->getSize(); 

	float pvalue = TMath::Prob( chi2*((double) ndof), ndof);

	cout << "\n\n\n chi2 = " << chi2 << "\n ndof = " << ndof << "\n p-value = " << pvalue << "\n\n\n" << endl;

	DrawLatex( 61, 0.100, 0.945, 0.050, 11, "CMS"                                             );
 	DrawLatex( 52, 0.205, 0.945, 0.030, 11, "Preliminary"                                     );
	DrawLatex( 42, 0.900, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV, 2016)", TheLuminosity[ch]) );
	DrawLatex( 42, 0.150, 0.800, 0.040, 11, Form("#chi^{2}/ndof = %5.2f ", chi2  )            );
	DrawLatex( 42, 0.150, 0.725, 0.040, 11, Form("ndof = %3d "      , ndof  )                 );
	DrawLatex( 42, 0.150, 0.650, 0.040, 11, Form("p-value = %6.4f " , pvalue)                 );
	//DrawLatex( 42, 0.150, 0.700, 0.040, 11, Form( "%5.2f", frame -> chiSquare("modelA", "h_data", 0) ));
	//DrawLatex( 42, 0.150, 0.600, 0.040, 11, Form( "%5.2f", frame -> chiSquare("modelA", "h_data", 1) ));
	//DrawLatex( 42, 0.150, 0.500, 0.040, 11, Form( "%5.2f", frame -> chiSquare("modelA", "h_data", 2) ));



 	//cout << "\n\n\n\n" << endl; 

  	//for ( int i = 0; i < frame -> numItems(); i++ ) {

    	//	TString obj_name = frame -> nameOf(i); 

	//	if ( obj_name == "" ) continue;

    	//	cout << Form("%d. '%s'\n", i, obj_name.Data() );

  	//}

 	//cout << "\n\n\n\n" << endl; 

	TLegend* mylegend = new TLegend( 0.60, 0.55, 0.85, 0.85 );

	mylegend -> AddEntry( frame -> findObject("h_data"                            ), "data"         , "p" );

	mylegend -> AddEntry( frame -> findObject("modelA_Norm[variable]"             ), "fit"          , "l" );

	mylegend -> AddEntry( frame -> findObject("modelA_Norm[variable]_Comp[]"      ), "signal-fitted", "l" );

	mylegend -> AddEntry( frame -> findObject("modelA_Norm[variable]_Comp[bkgpdf]"), "bkg-fitted"   , "f" );

	mylegend->Draw();

	if( syst == nominal ){

		mycanvas -> SaveAs( Form( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/fit/" + kanalID[ch] + "_" + variableID_R[ivar] + "_" + parameterID[parameter] +  "_%dto%d.pdf", low, up ) );
		mycanvas -> SaveAs( Form( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/fit/" + kanalID[ch] + "_" + variableID_R[ivar] + "_" + parameterID[parameter] +  "_%dto%d.png", low, up ) );
	
	}

}

 
double GetFWHM( double sigma, double gamma ){

	double Gauss_FWHM = c * sigma;
  	double BW_FWHM    = 2 * gamma;

  	return a * BW_FWHM + sqrt ( b * pow( BW_FWHM, 2 ) + pow( Gauss_FWHM, 2 ) );

}


double GetFWHMerror( double sigma, double gamma, double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg ){

	double Gauss_FWHM = c * sigma;
  	double BW_FWHM    = 2 * gamma;
	double sqroot     = sqrt ( b * pow( BW_FWHM, 2 ) + pow( Gauss_FWHM, 2 ) );

	// partial derivatives FWHM(voigtian) w.r.t sigma and gamma
	double dFWHMdsigma = c * (      Gauss_FWHM / sqroot );
	double dFWHMdgamma = 2 * ( a + b * BW_FWHM / sqroot );

	// esigma * esigma * pow( Vss, 2 ) gives covariance(sigma, sigma) etc
	double p1 = dFWHMdsigma * dFWHMdsigma * esigma * esigma * pow( Vss, 2 );
	double p2 = dFWHMdsigma * dFWHMdgamma * esigma * egamma * pow( Vsg, 2 );
	double p3 = dFWHMdgamma * dFWHMdsigma * egamma * esigma * pow( Vgs, 2 );
	double p4 = dFWHMdgamma * dFWHMdgamma * egamma * egamma * pow( Vgg, 2 );

	return sqrt ( p1 + p2 + p3 + p4 );

}

void GetRatioValues( float x, float ex, float y, float ey, float y_JECu, float y_JECd, float y_UnEu, float y_UnEd, float& z, float& ez_data, float& ez_mc ){

	z       = x/y       ; 
	ez_data = z * ex/x;
	ez_mc   = z * sqrt(  pow( ey/y, 2 ) + pow( (y_JECu-y_JECd)/(2*y), 2 ) +  pow( (y_UnEu-y_UnEd)/(2*y), 2 )  );  

}





