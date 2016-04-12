#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TLegend.h"
#include "include/Constants.h"
#include "include/CutsTop.h"
#include <stdio.h>

void significance (){

	std::ofstream inFile("significance.txt",std::ios::out);
 
        // General adress

	  TString analysis = "Top";
          TString path = "rootfiles/"+ analysis + "/" ;

        // Top bakgrounds

          const int nyield = 11;
	  const TString syield [nyield] = {

            "00_Fakes",
            "02_WZTo3LNu",
            "03_ZZ",
            "05_ST",
            "06_WW",
            "07_ZJets",
            "09_TTV",
            "10_HWW",
            "11_Wg",
            "13_VVV",
            "14_HZ",
     };


        // Top Signal

          TString Top_signal = "04_TTTo2L2Nu";

        // Counters
  
          int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
          int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;


          double bkg;
          double signal;   
          double significance [lastchannel+1][100];
  
          TH1F* yield_ht [nyield];
        
          TGraph* gr;  

          for (Int_t i = firstchannel; i <= lastchannel; i++){


              std::cout << "-----------------------------------------------------------------------" << std::endl;
              std::cout << "channel " << schannel[i] << std::endl;	      
              std::cout << "-----------------------------------------------------------------------" << std::endl;
        

              inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
     	      inFile<<"\\hline"<< endl;
     	      inFile<< schannel[i];
              inFile<<"\\\\" << endl;
              inFile<<"\\hline"<< endl;

              inFile << "Ht Cut Level"<<  " & "<< "Signal" << " & " << "Background" << " & " << "S / sqrt(S + B)";
              inFile << " \\\\ "<< endl;
              inFile << "\\hline"<< endl;



              gr = new TGraph();


              TFile *Ttbar =  new TFile(path + Top_signal +".root","read");
              TH1F  *Ttbar_ht = (TH1F*)Ttbar -> Get("Top/00_Met40/2jet/h_ht_" + schannel[i]);
              Int_t nbins = Ttbar_ht -> GetNbinsX();
              //std::cout<< "nbins:"<< nbins << std::endl;

              for (Int_t k = 0; k < nyield; ++k){

                  TFile *yield = new TFile(path + "/" + syield[k] + ".root","read");
                  yield_ht[k] = (TH1F*)yield -> Get( "Top/00_Met40/2jet/h_ht_" + schannel[i]);

              }
        	      
              int k=0;
              int limit=250;
	      for (Int_t j = 150; j <= limit; j++){

		  //std::cout<< j << std::endl;

                  signal = Ttbar_ht -> Integral (j, nbins+1);           
               
                  bkg = 0.0; 
            
                  for (Int_t k = 0; k < nyield; ++k){                 

                      bkg +=  yield_ht[k] -> Integral (j, nbins+1);
                  
                  }

                  double x = j;
                  double y = (signal / sqrt(signal + bkg));
                  gr->SetPoint(k++, x, y);

                  inFile << x << " & "<<  signal << " & " << bkg <<" & "<< y;
                  inFile<<"\\\\"<<endl;
            
                  std::cout <<"Ht = " << j << " (GeV)" << "\t bkg = " << bkg << "\t signal = " << signal << "\t S/sqrt(S+B):" << y <<std::endl;   
              }
            
              
             TCanvas *mycanvas = new TCanvas();

             gr -> Draw("APE");
             gr -> SetTitle ("channel \t " + schannel[i]);
             gr -> GetXaxis() -> SetTitle("Ht (GeV)");
             gr -> GetYaxis() -> SetTitle("S/sqrt(S+B)");
             gr -> SetMarkerStyle (kFullDotMedium);
             gr -> SetMarkerSize (3);
             gr -> SetMarkerColor(kBlue);

             mycanvas->SaveAs("significance/"+ schannel[i] +".png");

             inFile<<"\\end{tabular}"<<endl;
             inFile<<"\\ hline"<<endl;

 
             }

              for (Int_t l= 0 ; l <= 100 ; l += 10 ){
     
                  std::cout << l << "\t sqrt():  "<< sqrt(l) << std::endl;
                  

  	      }    
         

 inFile.close();

}
