void GetRecoEff( TString process, float& a, float& b, float& c );

const TString params = "1-1-10"; 

const TString cut = "01_NewPresel";

TString process[3]; 

void TTbarRecoEff() {

	process[0] = "TTTo2L2Nu_ext1__part0"; 
	process[1] = "ttDM0001scalar00010"  ; 
	process[2] = "ttDM0001scalar00500"  ; 

	for( int i = 0; i < 3; i++ ){

		float a = 0;
		float b = 0;
		float c = 0;

		GetRecoEff( process[i], a, b, c );

		cout << process[i] << "\t\t" << a << "\t" << b << "\t" << c << "\t -> " << (b+c)/(a+b+c) << endl; 
		
	}

}

void GetRecoEff( TString process, float& a, float& b, float& c ){

	TFile* myfile = new TFile("rootfiles/" + params + "/TTDM/" + process + ".root", "READ");

	TH1F* myhisto  = (TH1F*) myfile -> Get( "TTDM/" + cut + "/h_topReco_ll" ); 	

	//cout << "wtf" << endl; 

	a = myhisto -> GetBinContent(1);

	b = myhisto -> GetBinContent(3);

	c = myhisto -> GetBinContent(5);

	myfile -> Close();

}

