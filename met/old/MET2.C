#include "MET.h"

void MET2(){

	if( DoGlobalPlots == false ) { cout << "   >>   GlobalPlots deactivated !!!   " << endl;  return; }


	Assign();



	TFile* allhistos = new TFile( "histograms/" + allhistosReadFrom + ".root", "read" );

	//TFile* auxiliar  = new TFile( "histograms/161021-wtf.root", "read" );


	for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){


		for( int i = 0; i < nvariable_G; i++ ){

			//for( int j = 0; j < nprocess; j++ ){
			for( int j = alpha; j < omega+1; j++ ){

				//if (  j == DoubleEG2016E     || j == DoubleEG2016F     || j == DoubleEG2016G     ) continue; 
				//if (  j == DoubleMuon2016E   || j == DoubleMuon2016F   || j == DoubleMuon2016G   ) continue; 
				//if (  j == SinglePhoton2016E || j == SinglePhoton2016F || j == SinglePhoton2016G ) continue;

				//if ( j == DoubleEG2016B     || j == DoubleEG2016C     || j == DoubleEG2016D     ) continue; 
				//if ( j == DoubleMuon2016B   || j == DoubleMuon2016C   || j == DoubleMuon2016D   ) continue; 
				//if ( j == SinglePhoton2016B || j == SinglePhoton2016C || j == SinglePhoton2016D ) continue; 

				int TheKanal = kanal[j];
 

				//if ( i == VpT && j == DY_mm ){

				//h_global[i][j][s] = (TH1F*) auxiliar -> Get("h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[j]+ "_" + systematicID[s] );

				//}

				//else{

				h_global[i][j][s] = (TH1F*) allhistos -> Get("h_global_" + variableID_G[i] + "_" + kanalID[TheKanal] + "_" + processID[j]+ "_" + systematicID[s] );

				//}


				MoveOverflows( h_global[i][j][s] );

				//if ( i == MET ) h_global[i][j][s] -> Rebin();

			}

		}


		//for( int j = 0; j < nprocess; j++ ){
		for( int j = alpha; j < omega+1; j++ ){

			//if (  j == DoubleEG2016E     || j == DoubleEG2016F     || j == DoubleEG2016G     ) continue; 
			//if (  j == DoubleMuon2016E   || j == DoubleMuon2016F   || j == DoubleMuon2016G   ) continue; 
			//if (  j == SinglePhoton2016E || j == SinglePhoton2016F || j == SinglePhoton2016G ) continue;

			//if ( j == DoubleEG2016B     || j == DoubleEG2016C     || j == DoubleEG2016D     ) continue; 
			//if ( j == DoubleMuon2016B   || j == DoubleMuon2016C   || j == DoubleMuon2016D   ) continue; 
			//if ( j == SinglePhoton2016B || j == SinglePhoton2016C || j == SinglePhoton2016D ) continue;

			if( isData[j] ) continue; 

			for( int i = 0; i < nvariable_G; i++ ){ 	

				int TheKanal = kanal[j]; 	 

	 			h_global[i][j][s] -> Scale( TheLuminosity[TheKanal] );  //if(  s == 0  &&  i == MET  ) cout << h_global[i][j][s] -> GetName() << endl;

				h_global[i][j][s] -> SetFillColorAlpha( ProcessColor[j], 0.7 );
		
			}		

		}

		for( int i = 0; i < nvariable_G; i++ ){

				/*h_global[i][TT_ee][s]->Add(h_global[i][TTsemiT_ee   ][s]);
				h_global[i][TT_ee][s]->Add(h_global[i][TTsemiTbar_ee][s]);

				h_global[i][WW_ee][s]->Add(h_global[i][WZTo2L2Q_ee  ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][WZTo3LNu_ee  ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][ZZTo4L_ee    ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][ZZTo2L2Q_ee  ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][ZZTo2L2Nu_ee ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][WWW_ee       ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][WWZ_ee       ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][WZZ_ee       ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][ZZZ_ee       ][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][SingleTop1_ee][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][SingleTop2_ee][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][SingleTop3_ee][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][SingleTop4_ee][s]);
				h_global[i][WW_ee][s]->Add(h_global[i][SingleTop5_ee][s]);


				h_global[i][TT_mm][s]->Add(h_global[i][TTsemiT_mm   ][s]);
				h_global[i][TT_mm][s]->Add(h_global[i][TTsemiTbar_mm][s]);

				h_global[i][WW_mm][s]->Add(h_global[i][WZTo2L2Q_mm  ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][WZTo3LNu_mm  ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][ZZTo4L_mm    ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][ZZTo2L2Q_mm  ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][ZZTo2L2Nu_mm ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][WWW_mm       ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][WWZ_mm       ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][WZZ_mm       ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][ZZZ_mm       ][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][SingleTop1_mm][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][SingleTop2_mm][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][SingleTop3_mm][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][SingleTop4_mm][s]);
				h_global[i][WW_mm][s]->Add(h_global[i][SingleTop5_mm][s]);



				h_global[i][GJets40100][s]->Add(h_global[i][GJets100200][s]);
				h_global[i][GJets40100][s]->Add(h_global[i][GJets200400][s]);
				h_global[i][GJets40100][s]->Add(h_global[i][GJets400600][s]);
				h_global[i][GJets40100][s]->Add(h_global[i][GJets600Inf][s]);  

				h_global[i][QCD200300][s]->Add(h_global[i][QCD300500  ][s]);
				h_global[i][QCD200300][s]->Add(h_global[i][QCD500700  ][s]);
				h_global[i][QCD200300][s]->Add(h_global[i][QCD7001000 ][s]);
				h_global[i][QCD200300][s]->Add(h_global[i][QCD10001500][s]);
				h_global[i][QCD200300][s]->Add(h_global[i][QCD15002000][s]);
				h_global[i][QCD200300][s]->Add(h_global[i][QCD2000Inf ][s]);

				h_global[i][WJets100200][s]->Add(h_global[i][WJets200400  ][s]);
				h_global[i][WJets100200][s]->Add(h_global[i][WJets400600  ][s]);
				h_global[i][WJets100200][s]->Add(h_global[i][WJets600800  ][s]);
				h_global[i][WJets100200][s]->Add(h_global[i][WJets8001200 ][s]);
				h_global[i][WJets100200][s]->Add(h_global[i][WJets12002500][s]);
				h_global[i][WJets100200][s]->Add(h_global[i][WJets2500Inf ][s]);

				h_global[i][ZGJets ][s]->Add( h_global[i][ZNuNuGJets40130][s] );
				h_global[i][ZGJets ][s]->Add( h_global[i][ZGTo2LG        ][s] );

				h_global[i][WGJets ][s]->Add( h_global[i][WGToLNuG       ][s] );

				h_global[i][TTGJets][s]->Add( h_global[i][TGJets         ][s] );*/

		}

	}   // systematic

	GlobalPlots( Zee   ); 
	GlobalPlots( Zmumu ); 
	//GlobalPlots( Gamma ); 

}


void GlobalPlots( int ch ){  

	int data; //runB, runC, runD, runE, runF, runG; 

	if ( ch == Zee   ) data = data_ee; //{ runB =DoubleEG2016B    ; runC =DoubleEG2016C    ; runD =DoubleEG2016D    ; runE =DoubleEG2016E    ; runF =DoubleEG2016F    ; runG =DoubleEG2016G    ;}
	if ( ch == Zmumu ) data = data_mm; //{ runB =DoubleMuon2016B  ; runC =DoubleMuon2016C  ; runD =DoubleMuon2016D  ; runE =DoubleMuon2016E  ; runF =DoubleMuon2016F  ; runG =DoubleMuon2016G  ;}
	///if ( ch == Gamma ) { runB =SinglePhoton2016B; runC =SinglePhoton2016C; runD =SinglePhoton2016D; runE =SinglePhoton2016E; runF =SinglePhoton2016F; runG =SinglePhoton2016G;}

	
	for( int s = 0; s < 1; s++ ){   // for( int s = 0; s < nsystematic; s++ ){


		for( int i = 0; i < nvariable_G; i++ ){

			h_data[i][s] = (TH1F*) h_global[i][data][s] -> Clone( "h_data_" + variableID_G[i] + "_" + systematicID[s] );

			///h_data[i][s] -> Add( h_global[i][runC][s] );
			///h_data[i][s] -> Add( h_global[i][runD][s] );

			//h_data[i][s] = (TH1F*) h_global[i][runE][s] -> Clone( "h_data_" + variableID_G[i] + "_" + systematicID[s] );
			///h_data[i][s] -> Add( h_global[i][runE][s] );
			///h_data[i][s] -> Add( h_global[i][runF][s] );
			///h_data[i][s] -> Add( h_global[i][runG][s] );

			h_data[i][s] -> SetMarkerStyle(20);

			h_data[i][s] -> SetMarkerColor(kBlack);

			h_data[i][s] -> SetLineColor(kBlack);

		}



		//-- h_mc 
	 
		for( int i = 0; i < nvariable_G; i++ ){

			if( ch == Zee ){

				h_mc[i][s] = (TH1F*) h_global[i][DY_ee][s] -> Clone( "h_mc_" + variableID_G[i] + "_" + systematicID[s] );

				h_mc[i][s] -> Add( h_global[i][TT_ee][s] );  
				h_mc[i][s] -> Add( h_global[i][EW_ee][s] ); 

				///h_mc[i][s] = (TH1F*) h_global[i][DY_ee][s] -> Clone( "h_mc_" + variableID_G[i] + "_" + systematicID[s] );

				///h_mc[i][s] -> Add( h_global[i][TT_ee][s] );  
				///h_mc[i][s] -> Add( h_global[i][WW_ee][s] );  
		
			}

			if( ch == Zmumu ){

				h_mc[i][s] = (TH1F*) h_global[i][DY_mm][s] -> Clone( "h_mc_" + variableID_G[i] + "_" + systematicID[s] );

				h_mc[i][s] -> Add( h_global[i][TT_mm][s] );
				h_mc[i][s] -> Add( h_global[i][EW_mm][s] );
	
			}


			if( ch == Gamma ){

				h_mc[i][s] = (TH1F*) h_global[i][GJets40100][s] -> Clone( "h_mc_" + variableID_G[i] + "_" + systematicID[s] );

				h_mc[i][s] -> Add( h_global[i][QCD200300  ][s] );
				h_mc[i][s] -> Add( h_global[i][WJets100200][s] );
				h_mc[i][s] -> Add( h_global[i][WGJets     ][s] );
				h_mc[i][s] -> Add( h_global[i][ZGJets     ][s] );
				h_mc[i][s] -> Add( h_global[i][TTGJets    ][s] );			

			}

		}	

	}   // systematic 


	//-- PU-reweighting

	//GetPUreweighting( ch );



	//-- stack
 
	for( int i = 0; i < nvariable_G; i++ ){

		s_global[i]  = new THStack( variableID_G[i], variableID_G[i] );

		if(  ch == Zee ){

			s_global[i] -> Add( h_global[i][TT_ee][nominal] );
			s_global[i] -> Add( h_global[i][EW_ee][nominal] );
			s_global[i] -> Add( h_global[i][DY_ee][nominal] );

		}

		if(  ch == Zmumu ){

			s_global[i] -> Add( h_global[i][TT_mm][nominal] );
			s_global[i] -> Add( h_global[i][EW_mm][nominal] );
			s_global[i] -> Add( h_global[i][DY_mm][nominal] );

		}

		if( ch == Gamma ){	

			s_global[i] -> Add( h_global[i][TTGJets        ][nominal] );
			s_global[i] -> Add( h_global[i][ZGJets         ][nominal] );
			s_global[i] -> Add( h_global[i][WJets100200    ][nominal] );
			s_global[i] -> Add( h_global[i][WGJets         ][nominal] );
			s_global[i] -> Add( h_global[i][QCD200300      ][nominal] );
			s_global[i] -> Add( h_global[i][GJets40100     ][nominal] );

		}

	}


	//-- ratio

	for( int i = 0; i < nvariable_G; i++ ){

		Ratio[i] = (TH1F*) h_data[i][nominal] -> Clone( "ratio_" + variableID_G[i] );
		Unc  [i] = (TH1F*) h_mc  [i][nominal] -> Clone( "ratio_" + variableID_G[i] ); 

		Unc[i]->SetLineColor(kRed);
		Unc[i]->SetFillColor(11);


	}

	BuildRatio( parall_G, nbinuPara ); 
	BuildRatio( transv_G, nbinuPerp ); 
	BuildRatio( MET     , nbinMET   ); 
	BuildRatio( VpT     , nbinpT    ); 
	BuildRatio( nvert   , nbinnvert );
	

	for( int i = 0; i < nvariable_G; i++ ){

		TCanvas* c = new TCanvas( "canvas_" + kanalID[ch] + "_" + variableID_G[i], variableID_G[i], 550, 720 );  

		TPad* pad1 = new TPad("pad1", "pad1", 0.05, 0.0, 1.00, 0.3);
		TPad* pad2 = new TPad("pad2", "pad2", 0.05, 0.3, 1.00, 1.0);

		pad1 -> SetTopMargin   (0.08);
		pad1 -> SetBottomMargin(0.35);
		pad1 -> Draw();

		pad2 -> SetTopMargin   (0.08);
		pad2 -> SetBottomMargin(0.02);
		pad2 -> Draw();

		// ---------------------------------------------------------------

		pad2 -> cd();

		pad2 -> SetLogy(); //h_data[i][nominal] -> SetMaximum( 1.8* h_data[i][nominal]->GetMaximum() );  

		h_data[i][nominal] -> SetTitle("");

		h_data[i][nominal] -> SetStats(false);   // it has priority over the gStyle->SetOptStats option

 		SetAxis( h_data[i][nominal], "", Form("Events / %1.0f GeV", (maxMET-minMET)/nbinMET), 1.5, 1.8 );

		h_data[i][nominal] -> SetMinimum(0.1);


		h_data[i][nominal] -> Draw("e"); 

		s_global[i] -> Draw("hist same");

		h_data[i][nominal] -> Draw("e same");

		// ---------------------------------------------------------------
		
		TLegend* TheLegend = new TLegend( 0.68, 0.65, 0.88, 0.88 );

		TheLegend -> SetBorderSize(0);

		TheLegend -> SetTextSize(0.030);

		for( int j = alpha; j < omega+1; j++ ){   // for( int j = 0; j < nprocess; j++ ){ 


			if(  kanal[j] == ch ) {

			( isData[j] )   ?   TheLegend -> AddEntry( h_data[i][nominal], processIDfancy[j], "p" )   :   TheLegend -> AddEntry( h_global[i][j][nominal], processIDfancy[j], "f" );

			}

			//if(  ch == Zmumu && (1>0) ) {

			//( isData[j] )   ?   TheLegend -> AddEntry( h_data[i][nominal], processIDfancy[j], "p" )   :   TheLegend -> AddEntry( h_global[i][j][nominal], processIDfancy[j], "f" );

			//}

			//if(  ch == Gamma && ( j == SinglePhoton2016B || j ==  GJets40100 || j == QCD200300 || j == WJets100200 || j == WJets100200 || j == ZGJets || j == WGJets || j == TTGJets ) ) {

			//( isData[j] )   ?   TheLegend -> AddEntry( h_data[i][nominal], processIDfancy[j], "p" )   :   TheLegend -> AddEntry( h_global[i][j][nominal], processIDfancy[j], "f" );

			//}

			else continue; 

		}


		TheLegend -> Draw();

		// ---------------------------------------------------------------

		DrawLatex( 61, 0.100, 0.945, 0.050, 11, "CMS"                                                 );

     	 	DrawLatex( 52, 0.205, 0.945, 0.030, 11, "Preliminary"                                         );

		DrawLatex( 42, 0.900, 0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV, 2016)", TheLuminosity[ch]) );

		// ---------------------------------------------------------------
		
  		pad2 -> RedrawAxis();

		// ---------------------------------------------------------------

		pad1 -> cd();

		//pad1 -> SetLogy();		

		Ratio[i] -> SetTitle("");

		Ratio[i] -> SetStats(false);   // it has priority over the gStyle->SetOptStats option

 		SetAxis( Ratio[i], variableIDfancy_G[i], "data / MC ", 1.4, 0.75 );

      		//Ratio[i] -> GetYaxis() -> SetRangeUser(1e-1, 1e1);
      		Ratio[i] -> GetYaxis() -> SetRangeUser(0.5, 1.5);

		Ratio[i] -> Draw("ep");
		//Unc[i]   -> Draw("e2, same");
		Unc[i] -> SetFillColor(kWhite); Unc[i] -> Draw("hist, c, same");
		Ratio[i] -> Draw("ep, same");

		pad1 -> RedrawAxis();

      		// ---------------------------------------------------------------

		c -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/global/" + kanalID[ch] + "_" + variableID_G[i] + ".pdf" );
		c -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/MET_" + www + "/global/" + kanalID[ch] + "_" + variableID_G[i] + ".png" );

	}
 
}



