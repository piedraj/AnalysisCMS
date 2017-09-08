#include "ttdm.h"

int zoom = 2500; 
TString folder = "Analysis_170803_NN-with-systematics";
float offset = -0.20; 

bool doit[nsystematic]; 

TH1F* histo[nprocess][nsystematic]; 

void SelectSystematics();
void GetHistograms( int process );
void Plot();


void NNwithSystematics(){

	Assign(); 

	SelectSystematics(); 

	for( int j = 0; j < nscalar; j++ ){

		processID[ttDM] = scalarID[j];	b_name[ANN] = "ANN_tanh_mt2ll80_camille_" + processID[ttDM];

		GetHistograms( TT   );

		GetHistograms( ttDM );  

		Plot();

	}

}


void SelectSystematics(){

	for( int k = 0; k < nsystematic; k++ ) doit[k] = false; 

	for( int k = 0; k < nsystematic; k++ ){

		 if(    k == nominal    || 
			k == Btagup     ||  
			k == Btagdo     ||  
			///k == Idisoup    ||  
			///k == Idisodo    ||  
			///k == Triggerup  ||  
			///k == Triggerdo  || 
			///k == METup      || 
			///k == METdo      ||  
			///k == JESup      || 
			///k == JESdo      ||  
			//k == EleESup    || <--
			//k == EleESdo    || 
			///k == MuESup     || 
			///k == MuESdo     || 
			//k == QCDup      || 
			//k == QCDdo      || 
			//k == PDFup      || 
			//k == PDFdo      || 
			1 < 0               )  		doit[k] = true;  

	} 

}


void GetHistograms( int process ){

	cout << "\n \t\t" << processID[process] << endl;

	for( int k = 0; k < nsystematic; k++ ){

		if ( !doit[k] ) continue; 

		cout << "\t\t\t" << systematicID[k] << endl; 

		TCanvas* c1 = new TCanvas("canvas", "the canvas");

		// warning not for all the processes there are MET, JES and LES trees... 
	
		TFile* myfile = new TFile( storageSite + minitreeDir[k] + "/TTDM/" + processID[process] + ".root", "read" ); 

		TTree* mytree = (TTree*) myfile -> Get( "latino" );

		TCut thecut       = ( process == data || process == fakes )  ?  eventW[0] : eventW[k];

		TCut newselection = ( process == data || process == fakes  ) ?  selection : selection && RemovingFakes; 

                                       thecut = newselection                *thecut;
		if ( process == TT   ) thecut = Form("         %4.2f", ttSF)*thecut; 
		if ( process == DY   ) thecut = Form("         %4.2f", DYSF)*thecut; 
                if ( process == ttDM ) thecut = Form("         %4.2f", xs2l)*thecut; 

		mytree -> Draw( Form("1-%s >> myhisto( %d, %f, %f )", b_name[ANN].Data(), nbinraw[ANN], log(lowraw[ANN]), log(upraw[ANN]) ), thecut );
		//mytree -> Draw( Form("%s >> myhisto( %d, %f, %f )", b_name[ANN].Data(), nbinraw[ANN], lowraw[ANN], upraw[ANN] ), thecut );

		histo[process][k] = (TH1F*) gDirectory -> Get( "myhisto" );

		if(  process == data  ||  process == fakes ) histo[process][k] -> Scale( thelumi ); 	

		if(  process == ttDM )                       histo[process][k] -> Scale( zoom ); 
	
		//myfile->Close(); 
		c1->Destructor(); 

	}   // k 

}

void Plot(){

	histo[TT][ 0 ] -> SetMaximum( 1.15* histo[TT][ 0 ]->GetMaximum() );

	for( int k = 0; k < nsystematic; k++ ){

		if( !doit[k] ) continue; 
 
		if( k%2 == 0 ) continue; 

		TCanvas* c1 = new TCanvas("canvas", "the canvas");
		gPad->SetLogx();
		histo[TT][ 0 ] -> SetLineColor( kRed    );  histo[TT][ 0 ] -> SetLineWidth(2.);
		histo[TT][ k ] -> SetLineColor( kBlue+4 );  histo[TT][ k ] -> SetLineWidth(2.);
		histo[TT][k+1] -> SetLineColor( kBlue   );  histo[TT][k+1] -> SetLineWidth(2.);
		
		histo[ttDM][ 0 ] -> SetLineColor( kGreen+1   ); histo[ttDM][ 0 ] -> SetLineWidth(2.);
		histo[ttDM][ k ] -> SetLineColor( kMagenta+4 ); histo[ttDM][ k ] -> SetLineWidth(2.);
		histo[ttDM][k+1] -> SetLineColor( kMagenta+2 ); histo[ttDM][k+1] -> SetLineWidth(2.);

		histo[TT][ 0 ] -> Draw("hist");  
		histo[TT][ 0 ] -> SetStats(false); 
		histo[TT][ 0 ] -> SetTitle(  Form( "%5.2f fb^{-1}", thelumi )  ); 
		histo[TT][ 0 ] -> GetXaxis() -> SetTitle("ANN output");

		histo[TT][ k ] -> Draw("hist,same");
		histo[TT][k+1] -> Draw("hist,same");
		histo[TT][ 0 ] -> Draw("hist,same");

		histo[ttDM][ k ] -> Draw("hist,same");
		histo[ttDM][k+1] -> Draw("hist,same");
		histo[ttDM][ 0 ] -> Draw("hist,same");

		TLegend* theleg = new TLegend( 0.58+offset, 0.60, 0.88+offset, 0.88 );

		theleg->AddEntry( histo[TT][ 0 ],       "t#bar{t} nominal"                                  , "l" );
		theleg->AddEntry( histo[TT][ k ], Form( "t#bar{t} %s up",   systematicIDdatacard[k].Data() ), "l" );
		theleg->AddEntry( histo[TT][k+1], Form( "t#bar{t} %s down", systematicIDdatacard[k].Data() ), "l" );

		theleg->AddEntry( histo[ttDM][ 0 ], Form( "ttDM x%d nominal", zoom                                 ), "l" );
		theleg->AddEntry( histo[ttDM][ k ], Form( "ttDM x%d %s up"  , zoom, systematicIDdatacard[k].Data() ), "l" );
		theleg->AddEntry( histo[ttDM][k+1], Form( "ttDM x%d %s down", zoom, systematicIDdatacard[k].Data() ), "l" );

		theleg->Draw();

		//c1 -> SaveAs("~/www/figures/" + folder + "/" + b_name[ANN] + "_" + systematicIDdatacard[k] +".png");
		c1 -> SaveAs("figures/" + b_name[ANN] + "_" + systematicIDdatacard[k] +".png");
		//c1 -> SaveAs("~/www/figures/" + folder + "/" + b_name[ANN] + "_" + systematicIDdatacard[k] +".pdf");
		c1 -> SaveAs("figures/" + b_name[ANN] + "_" + systematicIDdatacard[k] +".pdf");

		c1 -> Destructor();
	}
}
