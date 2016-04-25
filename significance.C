#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TLegend.h"
#include "include/Constants.h"
#include "include/CutsTop.h"
#include <stdio.h>
#include "TMath.h"

void significance (){

// void significance (analysis, cut_path, variable, Xmin, Xmax, Xstep){

	std::ofstream inFile("significance.txt",std::ios::out);
 
        // Xmin, Xmax, step
     
          Xmin = 150; 
 	  Xmax = 250; 
	  Xstep = 1; 

        // General adress

	  TString analysis = "Top";
          TString root_path = "rootfiles/"+ analysis + "/" ;
          TString cut_path = "Top/00_Met50/2jet/";
          TString variable ="ht";

        // Top Signal
         
          TString Signal_name = "04_TTTo2L2Nu";

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


        // Counters
  
          int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
          int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;
         
        // Xmin, Xmax, step

          double min = Xmin; 
          double max = Xmax; 
          double step = Xstep; 

          double bkg;
          double signal;  

        //  double significance [lastchannel+1][100];
  
          TH1F* yield_ht [nyield];
        
          TGraph* gr;  
          TLegend* leg;

         
           // Best cut information
          // -------------------------------------------------------------------------------------------------

          TString tok, icut;
          Ssiz_t from = 0;
          while (cut_path.Tokenize(tok, from, "_")) icut = tok;


     
          inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
          inFile<<"\\hline"<< endl;
          inFile<< icut;
          inFile<<"\\\\" << endl;
          inFile<<"\\hline"<< endl;
          inFile << "channel" << " & " <<  variable + " Best cut (GeV) " << " & " << "S / sqrt(S + B)";
          inFile<<"\\\\" << endl;
          inFile<<"\\hline"<< endl;


          for (Int_t i = firstchannel; i <= lastchannel; i++){

	    double xmax = -1;
	    double ymax = -1;
	    int n = 0; 

          
              // All cuts information 
              // ----------------------------------------------------------------------------------------------------


              std::cout << "-----------------------------------------------------------------------" << std::endl;
              std::cout << "channel " << schannel[i] << std::endl;	      
              std::cout << "-----------------------------------------------------------------------" << std::endl;
        

              //inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
     	      //inFile<<"\\hline"<< endl;
     	      //inFile<< schannel[i];
              //inFile<<"\\\\" << endl;
              //inFile<<"\\hline"<< endl;

              //inFile << variable +" Cut Level"<<  " & "<< "Signal" << " & " << "Background" << " & " << "S / sqrt(S + B)";
              //inFile << " \\\\ "<< endl;
              //inFile << "\\hline"<< endl;{c



              gr = new TGraph();
	      //grDot = new TGraph(); 

              TFile *Ttbar =  new TFile(root_path + Signal_name +".root","read");
              TH1F  *Ttbar_ht = (TH1F*)Ttbar -> Get( cut_path + "h_" + variable + "_" + schannel[i]);
              Int_t nbins = Ttbar_ht -> GetNbinsX();
              //std::cout<< "nbins:"<< nbins << std::endl;

              for (Int_t k = 0; k < nyield; ++k){

                  TFile *yield = new TFile(root_path + "/" + syield[k] + ".root","read");
                  yield_ht[k] = (TH1F*)yield -> Get( cut_path + "h_" + variable + "_" + schannel[i]);

              }
        	      
              int k=0;
	      for (Int_t j = min; j <= max; j+= step ){

		  //std::cout<< j << std::endl;

                  signal = Ttbar_ht -> Integral (j, nbins+1);           
               
                  bkg = 0.0; 
            
                  for (Int_t k = 0; k < nyield; ++k){                 

                      bkg +=  yield_ht[k] -> Integral (j, nbins+1); // AÃadir FinBin(min/max) 
                  
                  }


                  double x = j;
                  double y = (signal / sqrt(signal + bkg));
		  n += 1;  

                  gr->SetPoint(k++, x, y);

		  if (y > ymax)
		    {
		      xmax = x;
		      ymax = y;
		    }
               
	       // all cuts information
	       // ----------------------------

              //    inFile << x << " & "<<  signal << " & " << bkg <<" & "<< y;
              //    inFile<<"\\\\"<<endl;
            
               //   std::cout << variable << " = " << j << " (GeV)" << "\t bkg = " << bkg << "\t signal = " << signal << "\t S/sqrt(S+B):" << y <<std::endl;   
              }
            
	    // Get the maximun value of y (significance)
	    // -------------------------------------------

             std::cout << " MaxElement ymax " << ymax << "\n MaxElement xmax" << xmax << std::endl;
	     double MaxY = TMath::MaxElement(n,gr->GetY());
	     double MaxX = min + step * (TMath::LocMax (n, gr->GetY())); 
             std::cout << "MaxElement MaxY:  " << MaxY << "\n MaxElement MaxX: " << MaxX << std::endl;                


             // Best cut information
             // -------------------------------

             inFile << schannel[i] << " & " << MaxX << " & "<< MaxY;
             inFile<<"\\\\"<<endl;
             inFile<<"\\ hline"<<endl;
          

            // Draw the graph 
            // --------------------------------
 
             TCanvas *mycanvas = new TCanvas();

             gr -> Draw("APE");
             gr -> SetTitle ( variable + "\t channel \t " + schannel[i]);
             gr -> GetXaxis() -> SetTitle(variable + "(GeV)");
             gr -> GetYaxis() -> SetTitle("S/sqrt(S+B)");
             gr -> SetMarkerStyle (kFullDotSmall);
             //gr -> SetMarkerSize (12);
             gr -> SetMarkerColor(kBlue);
           
             leg = new TLegend(0.7,0.8,0.9,0.9);
             leg->SetHeader(icut); 	     
             leg->Draw();




            //grDot -> SetPoint (1, MaxX, MaxY);
            //grDot -> Draw("APE");
            //grDot -> SetMarkerStyle (kFullDotLarge);
            //grDot -> SetMarkerColor(kRed);
  	    //grDot -> Draw("SAME");	    

            mycanvas->SaveAs("significance/" + variable + "_" + schannel[i] +".png");

     }    

  inFile<<"\\end{tabular}"<<endl;
  inFile.close();

}
