ofstream DatFile;

void ReadProcess( TString whichProcess ); 


void CreateDat() {

	DatFile.open("ANN.dat");

	ReadProcess("ttDM0001scalar00010");

	ReadProcess("04_TTTo2L2Nu"       ); 

	DatFile.close();

}

void ReadProcess( TString whichProcess ) {

	DatFile << Form("\n\n\n -----\n %10s\n -----\n\n", whichProcess.Data()) ;

	TFile* TheFile = new TFile( "../minitrees/nominal/TTDM/" + whichProcess + ".root", "read"); 

	TTree* tree = (TTree*) TheFile -> Get( "latino" );

	float channel;
	float metPfType1;
	float m2l;
	float njet;
	float nbjet20cmvav2l;
	float lep1pt;
	float lep2pt;
	float jet1pt;
	float jet2pt;
	float ht;
	float mtw1;
	float mtw2;
	float dphill;
	float dphilep1jet1;
	float dphilep1jet2;
	float dphilmet1;
	float dphilep2jet1;
	float dphilep2jet2;
	float dphilmet2;
	float dphijj;
	float dphijet1met;
	float dphijet2met;
	float dphillmet;

	tree->SetBranchAddress("channel",        &channel);
	tree->SetBranchAddress("metPfType1",     &metPfType1);
	tree->SetBranchAddress("m2l",            &m2l);
	tree->SetBranchAddress("njet",           &njet);
	tree->SetBranchAddress("nbjet20cmvav2l", &nbjet20cmvav2l);
	tree->SetBranchAddress("lep1pt",         &lep1pt);
	tree->SetBranchAddress("lep2pt",         &lep2pt);
	tree->SetBranchAddress("jet1pt",         &jet1pt);
	tree->SetBranchAddress("jet2pt",         &jet2pt);
	tree->SetBranchAddress("ht",             &ht);
	tree->SetBranchAddress("mtw1",           &mtw1);
	tree->SetBranchAddress("mtw2",           &mtw2);
	tree->SetBranchAddress("dphill",         &dphill);
	tree->SetBranchAddress("dphilep1jet1",   &dphilep1jet1);
	tree->SetBranchAddress("dphilep1jet2",   &dphilep1jet2);
	tree->SetBranchAddress("dphilmet1",      &dphilmet1);
	tree->SetBranchAddress("dphilep2jet1",   &dphilep2jet1);
	tree->SetBranchAddress("dphilep2jet2",   &dphilep2jet2);
	tree->SetBranchAddress("dphilmet2",      &dphilmet2);
	tree->SetBranchAddress("dphijj",         &dphijj);
	tree->SetBranchAddress("dphijet1met",    &dphijet1met);
	tree->SetBranchAddress("dphijet2met",    &dphijet2met);
	tree->SetBranchAddress("dphillmet",      &dphillmet);


DatFile << "ievt \t channel \t metPfType1 \t m2l \t njet \t nbjet20cmvav2l \t lep1pt \t lep2pt \t jet1pt \t jet2pt \t ht \t mtw1 \t mtw2 \t dphill \t dphilep1jet1 \t dphilep1jet2 \t dphilmet1 \t dphilep2jet1 \t dphilep2jet2 \t dphilmet2 \t dphijj \t dphijet1met \t dphijet2met \t dphillmet \n" ;


	int nentries; 

	( tree->GetEntries() > 2000 )  ?  nentries = 2000  :  nentries = tree->GetEntries(); 
 
	for ( int ievt = 0; ievt < nentries; ievt++ ) {


		if( ievt%100 == 0 )  cout << " >>  evt " << ievt << endl;

		tree -> GetEntry(ievt);


DatFile << Form( "%4d \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \t %7.3f \n", 
ievt+1, channel, metPfType1, m2l, njet, nbjet20cmvav2l, lep1pt, lep2pt, jet1pt, jet2pt, ht, mtw1, mtw2, dphill, dphilep1jet1, dphilep1jet2, dphilmet1, dphilep2jet1, dphilep2jet2, dphilmet2, dphijj, dphijet1met, dphijet2met, dphillmet );

	}

}
