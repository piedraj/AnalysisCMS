#include "ttdm.h"

const TString outputdir = "histos/blabla/"; 

void CreateHistograms2( int process );

float cuentita = 0.0; 


void CreateHistograms(){

	Assign();

	gSystem->mkdir( outputdir, kTRUE );

	//-----

	for( int i = 0; i < nprocess; i++ ){

		CreateHistograms2( i ); 

	}


	//CreateHistograms2( ttDM );
	//CreateHistograms2( TT   );

	//-----

	//for( int i = 0; i < npseudo; i++ ){  // -> change 'processID[]' by 'scalarID[]' in 'myfile = new TFile ...'

	//	CreateHistograms2( i ); 

	//}	

	//-----

	cout << "\n\n \t yield total = " << cuentita << endl;
 
	cout << "\n \n The End !!! \n \n" << endl; 

}


void CreateHistograms2( int process ){ 

	//if( process == ttDM ) return; 

	cout << "\n \t process: " << processID[process] << endl; 

	for( int k = 0; k < nsystematic; k++ ){ 

		if( k > nominal          ) continue; 
		//if( k > MuESdo || k%2==0 ) continue; 

		//cout << "\t\t systematic: " << systematicID[k] << endl;


		TCanvas* c1 = new TCanvas("canvas", "the canvas");
	
		//if( process == TT ) continue; //processID[process] = processID[WW];   // to speed-up checks: not including the TT

		TFile* myfile;

		//myfile = new TFile( "/afs/cern.ch/work/j/jgarciaf/public/ttdm-april/" + processID[process] + ".root", "read" );
		//myfile = new TFile( "../minitrees/" + inputdir + "/TTDM/" + processID[process] + ".root", "read" );
		//myfile = new TFile( storageSite + minitreeDir[k] + "/TTDM/" + processID[process] + ".root", "read" );


		if( process == data || process == fakes || process == TTV || process == Wg || process == Zg ){ 

			myfile = new TFile( storageSite + minitreeDir[0] + "/TTDM/" + processID[process] + ".root", "read" ); 

		}

		else{ 

			myfile = new TFile( storageSite + minitreeDir[k] + "/TTDM/" + processID[process] + ".root", "read" ); 

		}


		if(  process == data  &&  k > nominal  ) continue; 

		TFile* storagefile; 

		if( k == nominal ) storagefile = new TFile( outputdir + "/" + processID[process] +                         ".root", "recreate" );
		if( k >  nominal ) storagefile = new TFile( outputdir + "/" + processID[process] + "_" + systematicID[k] + ".root", "recreate" );

		TTree* mytree = (TTree*) myfile -> Get( "latino" );

		TCut thecut = (  process == data  ||  process == fakes )  ?  eventW[0]  :  eventW[k];


		//TCut RemovingFakes = "eventW_truegenmatched&&eventW_genmatched";
		TCut RemovingFakes = "eventW_genMatched && ( abs(lep1mid)==24 || abs(lep1mid)==15 ) && ( abs(lep2mid)==24 || abs(lep2mid)==15 )"; 



		TCut newselection = ( process == TT ) ? selection&&RemovingFakes : selection               ; 

		//TCut newselection = ( process == data || process == fakes  ) ? selection                : selection&&RemovingFakes; 

		//TCut newselection = selection; 


                                                     thecut = newselection                *thecut;

		//if ( process != data               ) thecut = Form("new_puW"             )*thecut; 
                //if ( process != data               ) thecut = Form("         %4.2f", PUrw)*thecut; 
		if ( process == TT                 ) thecut = Form("         %4.2f", ttSF)*thecut; 
		//if ( process == TT && k == toppTrw ) thecut = Form("toppTRwW*%4.2f", ttSF)*thecut; 
		if ( process == DY                 ) thecut = Form("         %4.2f", DYSF)*thecut; 
                if ( process == ttDM               ) thecut = Form("         %4.2f", xs2l)*thecut; 

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

		int HowManyVar = 0; 

		for( int i = 0; i < nhisto; i++ ){

			if(	//i ==  nlepton      ||
				//i == lep1pt        || 
				//i == lep1eta       || 
				//i == lep1phi       || 
				//i == lep1mass      ||
				//i == lep2pt        || 
				//i == lep2eta       || 
				//i == lep2phi       || 
				//i == lep2mass      || 
				//i == jet1pt        || 
				//i == jet1eta       || 
				//i == jet1phi       || 
				//i == jet1mass      ||
				//i == jet2pt        || 
				//i == jet2eta       || 
				//i == jet2phi       || 
				//i == jet2mass      ||
				//i == metPfType1    || 
				//i == metPfType1Phi ||
				//i == m2l           || 
				//i == mt2ll         || 
				//i == mt2lblb       || 
				//i == mtw1          || 
				//i == mtw2          ||
				//i == htjets        || 
				//i == htnojets      ||
				//i == njet          || 
				//i == nbjet30csvv2l || 
				//i == nbjet30csvv2m || 
				//i == nbjet30csvv2t ||  
				//i == dphijet1met   || 
				//i == dphijet2met   || 
				//i == dphijj        || 
				//i == dphijjmet     || 
				//i == dphill        || 
				//i == dphilep1jet1  || 
				//i == dphilep1jet2  || 
				//i == dphilep2jet1  || 
				//i == dphilep2jet2  || 
				//i == dphilmet1     || 
				//i == dphilmet2     ||
				//i == dphillmet     ||	
				//i == nvtx          || 
				//i == darkpt        ||
				i == MVA               )
			{

				HowManyVar += 1; 
		 
				if ( doshape == 0 ) h_name[i] =                                                    b_name[i]; 
				if ( doshape == 1 ) h_name[i] = processID[process] + "_" + systematicID[k] + "_" + b_name[i];



				mytree -> Draw( Form("%s >> %s( %d, %f, %f )", b_name[i].Data(), h_name[i].Data(), nbinraw[i], lowraw[i], upraw[i] ), thecut );

				myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );
	
				myhisto[i] -> Write(); 



				if( HowManyVar == 1 ){ 

					float yield;

					if( process == data                     ) yield =         myhisto[i]-> Integral();
					if( process == fakes                    ) yield =     1.0*myhisto[i]-> Integral();
		                        if( process != data && process != fakes ) yield = thelumi*myhisto[i]-> Integral();

					cout << "\t\t" << yield << endl; 

					if( process != data && process != ttDM ) cuentita += yield;

				}

			}

		}
		
		storagefile -> Close();

		myfile -> Close();

		c1 -> Destructor();

	}  // k 

}
