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

 
        // Xmin, Xmax, step
        // -----------------
     
          double Xmin  = 0;  // GeV 
 	  double Xmax  = 3000; // GeV 
	  double Xstep = 100;  // # of cuts you wish

        // General adress
        // --------------

	  TString analysis  = "Top";
          TString root_path = "rootfiles/nominal/"+ analysis + "/" ;
          TString cut_path  = "Top/03_AN15305M/2jet/";
          TString variable  = "mpmet";// ht, htjets, mpmet

        // Top Signal
        // ----------
         
          TString Signal_name = "04_TTTo2L2Nu";

        // Top bakgrounds
        // -------------

          const int nyield = 10;
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
            "14_HZ",
     };


        

          double bkg;
          double signal;  

       //  double significance [lastchannel+1][100];
  
          TFile* yield [nyield];
          TGraph* gr;  
          TLegend* leg;


	// Open .root files
	// ----------------------
	 
          TFile *Signal =  new TFile(root_path + Signal_name +".root","read");

	  for (Int_t k = 0; k < nyield; ++k)
          {
           yield[k] = new TFile(root_path + "/" + syield[k] + ".root","read");
          }
	  

        // Counters
        // ----------------------------

          int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
          int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;
                               
          TH1F  *S_Var = (TH1F*)Signal -> Get( cut_path + "h_" + variable + "_" + "ll");
         
          int    nbins   = S_Var -> GetNbinsX();
          double binMin  = S_Var -> FindBin(Xmin);
          double binMax  = S_Var -> FindBin(Xmax);
          double binStep = (binMax - binMin)/Xstep;

        // Best cut information
        // -------------------------------------------------------------------------------------------------

	  TString tok, icut;
          Ssiz_t from = 0;
          while (cut_path.Tokenize(tok, from, "_")) icut = tok;

          std::ofstream inFile("significance_"+ icut + "_" + variable +".txt",std::ios::out);

          inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
          inFile<<"\\hline"<< endl;
          inFile<< icut;
          inFile<<"\\\\" << endl;
          inFile<<"\\hline"<< endl;
          inFile << "channel" << " & " <<  variable + " Best cut (GeV) " << " & " << "S / sqrt(S + B)";
          inFile<<"\\\\" << endl;
          inFile<<"\\hline"<< endl;


	  // Loop on Channel (ee,mm,em,ll) & Variable cuts
	  // --------------------------------------------------------

          for (Int_t i = firstchannel; i <= lastchannel; i++){

          
              // All cuts information 
              // --------------------


              std::cout << "-----------------------------------------------------------------------" << std::endl;
              std::cout << "channel " << schannel[i] << std::endl;	      
              std::cout << "-----------------------------------------------------------------------" << std::endl;
        


                  //TFile *yield = new TFile(root_path + "/" + syield[k] + ".root","read");
                 //grDot = new TGraph(); 


              int n = 0;
              double xmax = 0; 
	      double ymax = 0;
              gr = new TGraph();

	      TH1F  *Signal_Var = (TH1F*)Signal -> Get( cut_path + "h_" + variable + "_" + schannel[i]);	      

	      for (Int_t j =binMin; j <= binMax; j+= binStep )
              {

                  signal = Signal_Var -> Integral (j, nbins+1);           
               
                  bkg = 0.0; 

                  for (Int_t k = 0; k < nyield; ++k)
                  {                 
		    TH1F*  Yield_Var = (TH1F*)yield[k] -> Get( cut_path + "h_" + variable + "_" + schannel[i]);	
                    bkg +=  Yield_Var -> Integral (j, nbins+1); 
                  }


                  double x = j;
                  double y = (signal / sqrt(signal + bkg));
		  n += 1;  
                  gr->SetPoint(j, x, y);
             
                  if (y > ymax)
                  {
                   xmax = x;
                   ymax = y;
                  }
        
              }
            
	    // Get the maximun value of y (significance)
	    // -------------------------------------------

             std::cout << " MaxElement ymax " << ymax << "\n MaxElement xmax" << xmax << std::endl;
	     double MaxY = TMath::MaxElement(n,gr->GetY());
	     double MaxX = Xmin + Xstep * (TMath::LocMax (n, gr->GetY())); 
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
             mycanvas->SaveAs("significance/" + variable + "_" + schannel[i] +".png");

     }    

  inFile<<"\\end{tabular}"<<endl;
  inFile.close();


        //          gr->SetPoint(k++, x, y);

	//	  if (y > ymax)
	//	    {
	//	      xmax = x;
	//	      ymax = y;
	//	    }
               

	       // all cuts information
	       // ----------------------------

              //    inFile << x << " & "<<  signal << " & " << bkg <<" & "<< y;
              //    inFile<<"\\\\"<<endl;
            
               //   std::cout << variable << " = " << j << " (GeV)" << "\t bkg = " << bkg << "\t signal = " << signal << "\t S/sqrt(S+B):" << y <<std::endl;   
            //grDot -> SetPoint (1, MaxX, MaxY);
            //grDot -> Draw("APE");
            //grDot -> SetMarkerStyle (kFullDotLarge);
            //grDot -> SetMarkerColor(kRed);
  	    //grDot -> Draw("SAME");	    

              //inFile<<"\\begin{tabular}{cccccccccccccccccccccccccc}"<< endl;
     	      //inFile<<"\\hline"<< endl;
     	      //inFile<< schannel[i];
              //inFile<<"\\\\" << endl;
              //inFile<<"\\hline"<< endl;

              //inFile << variable +" Cut Level"<<  " & "<< "Signal" << " & " << "Background" << " & " << "S / sqrt(S + B)";
              //inFile << " \\\\ "<< endl;
              //inFile << "\\hline"<< endl;{c

}
