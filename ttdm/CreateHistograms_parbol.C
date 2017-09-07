#include "ttdm.h"
#include <string>
#include "TCollection.h"
#include "TKey.h"

void CreateHistograms2( int process );
void AddStatHistograms();
void SplitQCDHistograms();

float cuentita; 
TString pathway; 


void CreateHistograms_parbol(){

	Assign();

	gSystem->mkdir( histoSite, kTRUE );

	//-----

	for(int j = 0; j < nscalar; j++ ){

		if( j != 4 ) continue;

		cuentita = 0.0; 

		processID[ttDM] = scalarID[j];	b_name[ANN] = "ANN_170904_justMET50SF_" + processID[ttDM];

		//pathway = histoSite + processID[ttDM] + "/";	
		pathway = datacard_dir + datacard_folder + "/" + processID[ttDM] + "/";

		gSystem -> mkdir( pathway, kTRUE );
		

		/*for( int i = 0; i < nprocess; i++ ){

			if ( i == Wg || i == Zg ) continue; 

			CreateHistograms2( i ); 

		}*/

		cout << "\n\n \t yield total = " << cuentita << endl;

		//if( doshape == 1 ){ AddStatHistograms(); SplitQCDHistograms(); }

		//continue;
	
		cuentita = 0.0; 

		processID[ttDM] = pseudoID[j];	b_name[ANN] = "ANN_170904_justMET50SF_" + processID[ttDM]; 
		//pathway = histoSite + processID[ttDM] + "/";	
		pathway = datacard_dir + datacard_folder + "/" + processID[ttDM] + "/";

		gSystem -> mkdir( pathway, kTRUE );
		

		for( int i = 0; i < nprocess; i++ ){

			if ( i == Wg || i == Zg ) continue; 

			CreateHistograms2( i ); 

		}

		cout << "\n\n \t yield total = " << cuentita << endl;



	}   // j 
 
	cout << "\n \n The End !!! \n \n" << endl; 

}


void CreateHistograms2( int process ){ 

	//if(  process == data || process == fakes || process == TT || process == TTV ) return; 

	//if ( process != WZ ) return;

	//if ( process == VZ ) return;

	//if ( process != ST ) return;

	//if ( process != WW ) return;

	//if ( process != DY ) return;

	//if ( process != Wg ) return;

	//if ( process != Zg ) return;

	//if ( process == VVV) return;

	//if ( process != TT ) return;

	//if ( process !=ttDM) return;



	cout << "\n \t process: " << processID[process] << endl; 

	for( int k = 0; k <= toppTrwdo; k++ ){ 

		if(  k > nominal ) continue; 

		//if(  k != METup  && k != METdo ) continue; 

		//if ( processID[process] == "ttDM0001pseudo00010" && k == EleESdo ) continue;  // 1-pseudo-010 (jefferson)     VALUABLE INFORMATION, DO NOT ERASE !!! 

		//if( k >= EleESup && k <= MuESdo ) continue;
		//if( k >= METup   && k <= METdo  ) continue;
		//if( k >= JESup   && k <= JESdo  ) continue;

		if(  process == data  &&  k > nominal  ) continue; 


		cout << "\t \t" << systematicID[k] << endl; 

		TCanvas* c1 = new TCanvas("canvas", "the canvas");
	
		TFile* myfile;

		if( process == data || process == fakes || process == TTV /*|| process == Wg || process == Zg*/ ){ 

			myfile = new TFile( storageSite + minitreeDir[0] + "/TTDM/" + processID[process]  + ".root", "read" ); 

		}

		else{ 

			myfile = new TFile( storageSite + minitreeDir[k] + "/TTDM/" + processID[process] + ".root", "read" ); 

		}



		TFile* storagefile; 

		if ( doshape == 1 ){ storagefile = new TFile( pathway + "simple-shapes-TH1-semi.root", "update"); }
		
		if ( doshape == 0 ){ 
			if( k == nominal ) storagefile = new TFile( pathway + processID[process] +                         ".root", "recreate" );
			if( k >  nominal ) storagefile = new TFile( pathway + processID[process] + "_" + systematicID[k] + ".root", "recreate" );
		}

		TTree* mytree = (TTree*) myfile -> Get( "latino" );

		TCut thecut       = ( process == data || process == fakes ) ? eventW[0] : eventW[k];

		// jefferson and after jefferson
		TCut newselection = ( process == data || process == fakes ) ? selection : selection&&RemovingFakes; 

		// fucking-mum
		//TCut newselection = ( process == TT && ( k == nominal || k == toppTrw )  ) ? selection&&RemovingFakes : selection               ; 

		// nothing
		//TCut newselection = selection; 


                                                           thecut = newselection          *thecut; 
		if ( process == TT    )                    thecut = Form("%4.2f", ttSF   )*thecut; 
		if ( process == TT && k != toppTrwdo )     thecut ="eventW_Toppt"         *thecut; 
		if ( process == DY    )                    thecut = Form("%4.2f", DYSF   )*thecut; 
                if ( process == ttDM  )                    thecut = Form("%4.2f", xs2l   )*thecut;
                if ( process != data && process != fakes ) thecut = Form("%6.3f", thelumi)*thecut;
                if ( process == data || process == fakes ) thecut = Form("%6.3f", 1./15. )*thecut;

		if( (k >= QCDup && k <= PDFdo) && (process != data && process != ttDM && process != fakes && process != ST ) ){

			//TH1F* weights = (TH1F*) myfile -> Get( "list_vectors_weights" );

			if( k == QCDup ){

				//float qcd_norm_up = weights->GetBinContent(9)/weights->GetBinContent(1);

				//thecut = Form("(LHEweight[8]/LHEweight[0])/%7.4f", qcd_norm_up)*thecut;
				thecut = "(LHEweight[8]/LHEweight[0])"*thecut;
			}	

			if( k == QCDdo ){

				//float qcd_norm_do = weights->GetBinContent(5)/weights->GetBinContent(1);

				//thecut = Form("(LHEweight[4]/LHEweight[0])/%7.4f", qcd_norm_do)*thecut;
				thecut = "(LHEweight[4]/LHEweight[0])"*thecut;
			}

			if( k == PDFup ){

				//float PDF_norm_up = weights->GetBinContent(10)/weights->GetBinContent(1);

				//thecut = Form("(LHEweight[9]/LHEweight[0])/%7.4f", PDF_norm_up)*thecut;
				thecut = "(LHEweight[9]/LHEweight[0])"*thecut;
			}	

			if( k == PDFdo ){

				//float PDF_norm_do = weights->GetBinContent(11)/weights->GetBinContent(1);

				//thecut = Form("(LHEweight[10]/LHEweight[0])/%7.4f", PDF_norm_do)*thecut;
				thecut = "(LHEweight[10]/LHEweight[0])"*thecut;
			}

		}


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
			i == metPfType1    || 
				//i == metPfType1Phi ||
				//i == m2l           || 
			i == mt2ll         || 
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
			i == dphillmet     ||	
				//i == nvtx          || 
			i == darkpt        ||
			i == ANN           ||
                                1 < 0               )
			{

				HowManyVar += 1; 
		 
				if ( doshape == 0 ) h_name[i] = b_name[i]; 
				if ( doshape == 1 ){ 

					if( k == nominal ) h_name[i] = processID2[process]                                ;
					if( k != nominal ) h_name[i] = processID2[process] + "_" + systematicIDdatacard[k];

				}

				//--- 1st approach: TTree::Project()
				if( i == ANN && doshape == 1 ){

					double cloison[] = {  0.00, 0.30, 0.50, 0.70, 0.80, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00 };

					myhisto[i] = new TH1F(h_name[i], h_name[i], 10, cloison);

					//double cloison[] = {  0.00, 0.30, 0.50, 0.70, 0.80, 0.95, 1.00 };

					//myhisto[i] = new TH1F(h_name[i], h_name[i], 6, cloison);



					//myhisto[i] = new TH1F(h_name[i], h_name[i], nbinraw[i], lowraw[i], upraw[i]);


				} 

				else if( i == mt2ll && doshape == 1 ){

					double cloison[] = { 100.00, 140.00, 3000.00 };

					myhisto[i] = new TH1F(h_name[i], h_name[i], 2, cloison);

				}

				else{

					myhisto[i] = new TH1F(h_name[i], h_name[i], nbinraw[i], lowraw[i], upraw[i]); 

				}

				//TH1::Sumw2();

				mytree -> Project( h_name[i], b_name[i], thecut ); 


				//--- 2nd approach: TTree::Draw()
				//mytree -> Draw( Form("%s >> %s( %d, %f, %f )", b_name[i].Data(), h_name[i].Data(), nbinraw[i], lowraw[i], upraw[i] ), thecut );

				//myhisto[i] = (TH1F*) gDirectory -> Get( h_name[i] );
	


				myhisto[i] -> Write(); 



				if( HowManyVar == 1 ){ 

					float yield;

					if( process == data                     ) yield = myhisto[i]-> Integral(-1, -1);
					if( process == fakes                    ) yield = myhisto[i]-> Integral(-1, -1);
		                        if( process != data && process != fakes ) yield = myhisto[i]-> Integral(-1, -1);

					//cout << "\t\t\t" << myhisto[i] -> GetEntries() << "\t\t" << yield << endl; 

					cout << Form("\t\t\t %8.0f \t\t %8.2f", myhisto[i]->GetEntries(), yield) << endl; 
 
					if( k == nominal && process != data && process != ttDM ) cuentita += yield;

				}

			}

		}

                storagefile->Close();

		myfile -> Close();

		c1 -> Destructor();

	}  // k 

}


void AddStatHistograms(){

        TFile* storagefile = new TFile( pathway + "simple-shapes-TH1-semi.root", "read");
        TFile* storagefile2 = new TFile( pathway + "simple-shapes-TH1.root", "update");
        storagefile2->cd();
       
        TIter nextkey(storagefile->GetListOfKeys());
        TKey *key;
        while (key = (TKey*)nextkey()) {
            TH1F *current = (TH1F*)key->ReadObj();
            for(unsigned int i = 0; i < current->GetNbinsX(); i++) {
                if(current->GetBinContent(i) < 0) {
                    std::cout << "Found negative bin with number " << i << " in histogram " << current->GetName() << std::endl;
                    current->SetBinContent(i, 0);
                    current->SetBinError(i, 0);
                }
            }
            current->Write();
            std::string histo_name(current->GetName()); 
            if(histo_name.find("Up") == std::string::npos && histo_name.find("Down") == std::string::npos) {
                if(histo_name.find("data") != std::string::npos) continue; 
                for(unsigned int i = 1; i < current->GetNbinsX()+1; i++) {
                    char theNameUp[50];
                    sprintf(theNameUp, "%s_ibin_%d_stat_Up", histo_name.c_str(), i);
                    char theNameDown[50];
                    sprintf(theNameDown, "%s_ibin_%d_stat_Down", histo_name.c_str(), i);
                    TH1F *hup = (TH1F *) current->Clone(theNameUp);
                    TH1F *hdown = (TH1F *) current->Clone(theNameDown);
                    hup->SetBinContent(i, current->GetBinContent(i) + current->GetBinError(i));
                    if(current->GetBinContent(i) - current->GetBinError(i) < 0) {
                        hdown->SetBinContent(i, 0);
                    } else { 
                        hdown->SetBinContent(i, current->GetBinContent(i) - current->GetBinError(i));
                    } 
                    hup->Write();
                    hdown->Write();
                }
            }  
        }
	                 
        storagefile2->Close();
        storagefile->Close();

}


void SplitQCDHistograms(){

	//TString pathway = histoSite + processID[ttDM] + "/";
	//TString pathway = datacard_dir + datacard_folder + "/" + processID[ttDM] + "/";

        TFile* storagefile = new TFile( pathway + "simple-shapes-TH1.root", "read");
        TFile* storagefile2 = new TFile( pathway + "simple-shapes-TH1-final.root", "update");
        storagefile2->cd();
       
        TIter nextkey(storagefile->GetListOfKeys());
        TKey *key;
        while (key = (TKey*)nextkey()) {
            TH1F *current = (TH1F*)key->ReadObj();
            std::string histo_name(current->GetName());
            if(histo_name.find("QCD") != std::string::npos) {
               if(histo_name.find("TTV") != std::string::npos) {
                    if(histo_name.find("Up") != std::string::npos) {
                        TH1F *clone = (TH1F *)current->Clone("TTV_QCD_TTVUp");
                        clone->Write();
                    } else {
                        TH1F *clone = (TH1F *)current->Clone("TTV_QCD_TTVDown");
                        clone->Write();
                    }
               } else if(histo_name.find("TT") != std::string::npos) {
                    if(histo_name.find("Up") != std::string::npos) {
                        TH1F *clone = (TH1F *)current->Clone("TT_QCD_TTUp");
                        clone->Write();
                    } else {
                        TH1F *clone = (TH1F *)current->Clone("TT_QCD_TTDown");
                        clone->Write();
                    } 
               } else if(histo_name.find("signal") != std::string::npos) {
                    if(histo_name.find("Up") != std::string::npos) {
                        TH1F *clone = (TH1F *)current->Clone("signal_QCD_signalUp");
                        clone->Write();
                    } else {
                        TH1F *clone = (TH1F *)current->Clone("signal_QCD_signalDown");
                        clone->Write();
                    } 
               } else if(histo_name.find("DY") != std::string::npos) {
                    if(histo_name.find("Up") != std::string::npos) {
                        TH1F *clone = (TH1F *)current->Clone("DY_QCD_DYUp");
                        clone->Write();
                    } else {
                        TH1F *clone = (TH1F *)current->Clone("DY_QCD_DYDown");
                        clone->Write();
                    }
               }
            } else {
                current->Write();
            }
       }
           storagefile2->Close();
           storagefile->Close();

}
