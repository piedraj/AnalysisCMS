enum{ 	ttDM0001scalar00010, 
      	ttDM0001scalar00020, 
      	ttDM0001scalar00050, 
      	ttDM0001scalar00100, 
      	ttDM0001scalar00200, 
      	ttDM0001scalar00300, 
      	ttDM0001scalar00500,	
	nscalar	};

enum{ 	ttDM0001pseudo00010, 
      	ttDM0001pseudo00020, 
      	ttDM0001pseudo00050, 
      	ttDM0001pseudo00100, 
      	ttDM0001pseudo00200, 
      	ttDM0001pseudo00300, 
      	ttDM0001pseudo00500,	
	npseudo	};

TString scalarID[nscalar]; 
TString pseudoID[npseudo];
 
TCut selection;

TH2F* myhisto; 

void DarkGenVsRecoPlot2(TString process, TCut selection);


void DarkGenVsRecoPlot(){

	selection = "darkpt==00&&1>0"; 

	scalarID[ttDM0001scalar00010] = "ttDM0001scalar00010"; 
	scalarID[ttDM0001scalar00020] = "ttDM0001scalar00020"; 
	scalarID[ttDM0001scalar00050] = "ttDM0001scalar00050"; 
	scalarID[ttDM0001scalar00100] = "ttDM0001scalar00100"; 
	scalarID[ttDM0001scalar00200] = "ttDM0001scalar00200"; 
	scalarID[ttDM0001scalar00300] = "ttDM0001scalar00300"; 
	scalarID[ttDM0001scalar00500] = "ttDM0001scalar00500";

	pseudoID[ttDM0001pseudo00010] = "ttDM0001pseudo00010"; 
	pseudoID[ttDM0001pseudo00020] = "ttDM0001pseudo00020"; 
	pseudoID[ttDM0001pseudo00050] = "ttDM0001pseudo00050"; 
	pseudoID[ttDM0001pseudo00100] = "ttDM0001pseudo00100"; 
	pseudoID[ttDM0001pseudo00200] = "ttDM0001pseudo00200"; 
	pseudoID[ttDM0001pseudo00300] = "ttDM0001pseudo00300"; 
	pseudoID[ttDM0001pseudo00500] = "ttDM0001pseudo00500";


	for( int i = 0; i < nscalar; i++ ){

		DarkGenVsRecoPlot2(scalarID[i], selection); 

	}

	for( int i = 0; i < npseudo; i++ ){

		DarkGenVsRecoPlot2(pseudoID[i], selection); 

	}

	DarkGenVsRecoPlot2( "04_TTTo2L2Nu", selection); 

}

void DarkGenVsRecoPlot2(TString process, TCut selection){

	TCanvas* mycanvas = new TCanvas("canvas", "the canvas");

	TFile* myfile = new TFile( "/eos/user/j/jgarciaf/minitrees/fucking-mom/TTDM/" + process + ".root", "read" ); 

	TTree* mytree = (TTree*) myfile -> Get( "latino" );

	mytree -> Draw( "mt2ll:darkpt>>thatone(32,-10,310,32,-10,310)", selection );

	myhisto = (TH2F*) gDirectory -> Get( "thatone" );


	myhisto -> SetStats(false);

 	myhisto -> Draw("colz");

	TString spin0 = ( process.Contains("scalar") ) ? "S" : "P";

 	//myhisto -> SetTitle("GEN vs RECO p_{T}^{" + spin0 + "} for the mass point: " + process);
 	myhisto -> SetTitle("m_{T2}^{ll} vs #it{dark p_{T}} for the mass point: " + process);

	//myhisto -> GetXaxis() -> SetTitle("GEN  p_{T}^{" + spin0 + "} [GeV]");
	myhisto -> GetXaxis() -> SetTitle("m_{T2}^{ll} [GeV]");
	myhisto -> GetYaxis() -> SetTitle("RECO  p_{T}^{" + spin0 + "} [GeV]");

	mycanvas -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/Analysis_170613_Pupi/mt2ll-vs-darkpt_" + process + ".png" ); 
	mycanvas -> SaveAs( "/afs/cern.ch/user/j/jgarciaf/www/figures/Analysis_170613_Pupi/mt2ll-vs-darkpt_" + process + ".pdf" ); 

	myfile -> Close(); 

	mycanvas -> Destructor(); 

}
