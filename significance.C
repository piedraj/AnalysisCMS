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

          const int nyield = 12;
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
            "12_Zg",
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
        
        TGraph* gr[lastchannel+1];  

        for (Int_t i = firstchannel; i <= lastchannel; i++){

	      inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
     	      inFile<<"\\hline"<< endl;
     	      inFile<< schannel[i];
              inFile<<"\\\\" << endl;
              inFile<<"\\hline"<< endl;

             // inFile << "Varible Cut Level";
             // inFile << " & "<< "Signal" << " & " << "Background" << " & " << "S / sqrt(S + B)";
             // inFile << " \\\\ "<< endl;
             // inFile << "\\hline"<< endl;



              gr[i] = new TGraph();

              bkg = 0.0; 

              TFile *Ttbar =  new TFile(path + Top_signal +".root","read");
              TH1F  *Ttbar_ht = (TH1F*)Ttbar -> Get("Top/00_Met40/2jet/h_ht_" + schannel[i]);
              Int_t nbins = Ttbar_ht -> GetNbinsX();
              std::cout<< "nbins:"<< nbins << std::endl;

              for (Int_t k = 0; k < nyield; ++k){

                  TFile *yield = new TFile(path + "/" + syield[k] + ".root","read");
                  yield_ht[k] = (TH1F*)yield -> Get( "Top/00_Met40/2jet/h_ht_" + schannel[i]);

              }
        	      
 
	      for (Int_t j = 0; j < nbins; ++j){

		  std::cout<< j << std::endl;

                  signal = Ttbar_ht -> Integral (int (Ttbar_ht -> FindBin(j)), int (Ttbar_ht -> FindBin(nbins)));           
             	  inFile << signal;
               
                  for (Int_t k = 0; k < nyield; ++k){                 

                       bkg += (yield_ht[k] -> Integral (int (yield_ht[k] -> FindBin(j)), int (yield_ht[k] -> FindBin(nbins))));
         //         yield -> Close(); 
                  }                  
        
        }






              }
 inFile.close();
 

}
