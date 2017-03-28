#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TAxis.h"
#include "StopTrees.h"



#include <fstream>
#include <iostream>


//----------------------------------------------
// Global Variables
// ---------------------------------------------


// Falta comentar el output de las funciones y/o del codigo. Es una buena idea separarlo en signal, background y data y falta poner mejor el rango o punto de masa/s ademas de hacerlo para muchos puntos de masa, mandando un job quiza?

TString FileAddress = "minitrees/nominal/Stop/";
//TString FileAddress = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/2016/Stop/minitrees/nominal/Stop/";
const int nsample = 8;
TString SampleName [nsample] = {"T2tt_mStop-250to350_Sm275_Xm150.root", "04_TTTo2L2Nu.root", "05_ST.root",  "07_ZJets.root", "06_WW.root", "09_TTW.root", "03_VZ.root", "10_TTZ.root"};
//TString SampleName [nsample] = {"T2tt_mStop-150to250.root", "04_TTTo2L2Nu.root", "05_ST.root",  "07_ZJets.root", "06_WW.root", "09_TTW.root", "03_VZ.root", "10_TTZ.root"};
TString  _sample   [nsample] = {"T2tt", "TTbar", "ST", "DYJets", "WW", "TTW", "VZ", "TTZ"};

/* Signal sample: 
 *
 * T2tt_mStop-150to250.root
 * T2tt_mStop-250to350.root
 * T2tt_mStop-350to400.root
 * T2tt_mStop-400to1200.root
 *
 */



//TString FileAddress = "../../minitreesG/nominal/Stop/";

//const int nsample = 7;
//TString SampleName [nsample] = {"16_T2tt_mStop-150to1200.root", "04_TTTo2L2Nu.root", "05_ST.root",  "07_ZJets.root", "06_WW.root", "09_TTV.root", "03_VZ.root"};
//TString  _sample   [nsample] = {"T2tt", "TTbar", "ST", "DYJets", "WW", "TTV", "VZ"};

//const int nsample = 7;
//TString SampleName [nsample] = {"16_T2tt_mStop-150to1200.root", "04_TTTo2L2Nu.root", "05_ST.root",  "07_ZJets.root", "06_WW.root", "09_TTV.root", "03_VZ.root"};
//TString  _sample   [nsample] = {"T2tt", "TTbar", "ST", "DYJets", "WW", "TTV", "VZ"};

//----------------------------------------------------------------------------------------------------------------
// T2tt  = T2tt_mStop-150to1200.root
// TTbar = TTTo2L2Nu_ext1__part*.root 
// ST    = ST_tW_antitop.root ST_tW_top.root
// ZJets = DYJetsToLL_M-10to50.root DYJetsToLL_M-50_000*.root;
// WW    = WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
// TTV   = TTWJetsToLNu.root TTZjets.root
// VZ    = ZZTo2L2Q*.root WZTo2L2Q__part*.root
// -----------------------------------------------------------------------------------------------------------------
                                    
enum { ee, mm, em, nchannel}; const TString schannel[nchannel] = {"ee", "mm","em"};

// SR = Signal Region Histograms

TH1F* h_MT2ll_SR1    [nchannel]; // MT2ll > 40 && 140 <= Met < 200; 
TH1F* h_MT2ll_SR2    [nchannel]; // MT2ll > 40 && 200 <= Met < 300; 
TH1F* h_MT2ll_SR3    [nchannel]; // MT2ll > 40 && 300 <= Met;

// CR = Control Region Histograms

TH1F* h_MT2ll_CR1    [nchannel]; // MT2ll < 40 && 140 <= Met < 200; 
TH1F* h_MT2ll_CR2    [nchannel]; // MT2ll < 40 && 200 <= Met < 300; 
TH1F* h_MT2ll_CR3    [nchannel]; // MT2ll < 40 && 300 <= Met;

// VR = Validation Region Histograms

TH1F* h_VR          [nchannel]; // MT2ll > 40 && 100 <= Met < 140;
TH1F* h_CVR         [nchannel]; // MT2ll < 40 && 100 <= Met < 140;

// DY = Drell Yan Estimation Region

TH1F* h_DY          [nchannel]; // Met < 100;
 

// Get the Maximun of each Histogram
    
     float max_MT2ll_SR1; 
     float max_MT2ll_SR2;
     float max_MT2ll_SR3;

     float max_MT2ll_CR1;
     float max_MT2ll_CR2;
     float max_MT2ll_CR3;

     float max_VR;
     float max_CVR;
    
     float max_DY;

//---------------------------------------------------------------------
// Set Mass Point Function
//--------------------------------------------------------------------- 

TString massPoint(float Smass, float Xmass){

   TString sSmass = ""; sSmass += int(Smass); TString sXmass = ""; sXmass += int(Xmass);
   TString masspoint = "_Sm" + sSmass + "_" + "Xm" + sXmass;
   return masspoint;

}


//-------------------------------------------------------
// Save Histograms as .root File
// ------------------------------------------------------

void saveHistoRoot (TString irootfileName, TH1F* histoName){
     TFile* OutFileName = new TFile( irootfileName , "update");
     histoName   -> Write();
     OutFileName -> Close();
}


//--------------------------------------------------------
// Define Histograms
//--------------------------------------------------------

void defineHistograms(int ichannel, TString suffix){

     h_MT2ll_SR1   [ichannel] = new TH1F ("h_MT2ll_SR1"  +  suffix, "", 5, 40.,140.); // MT2ll > 40 && 140 <= Met < 200; 
     h_MT2ll_SR2   [ichannel] = new TH1F ("h_MT2ll_SR2"  +  suffix, "", 5, 40.,140.); // MT2ll > 40 && 200 <= Met < 300; 
     h_MT2ll_SR3   [ichannel] = new TH1F ("h_MT2ll_SR3"  +  suffix, "", 5, 40.,140.); // MT2ll > 40 && 300 <= Met;	
  
     h_MT2ll_CR1   [ichannel] = new TH1F ("h_MT2ll_CR1"  +  suffix, "", 2, 0.,40.); // MT2ll < 40 && 140 <= Met < 200; 
     h_MT2ll_CR2   [ichannel] = new TH1F ("h_MT2ll_CR2"  +  suffix, "", 2, 0.,40.); // MT2ll < 40 && 200 <= Met < 300; 
     h_MT2ll_CR3   [ichannel] = new TH1F ("h_MT2ll_CR3"  +  suffix, "", 2, 0.,40.); // MT2ll < 40 && 300 <= Met;
     
     h_VR          [ichannel] = new TH1F ("h_MT2ll_VR"   +  suffix, "", 5, 40.,140.); // 100 <= Met < 140;
//     h_CVR         [ichannel] = new TH1F ("h_MT2ll_CVR"  +  suffix, "", 2, 0., 40.);  // MT2ll < 40 && 100 <= Met < 140; 

//     h_DY          [ichannel] = new TH1F ("h_MT2ll_DY"   +  suffix, "", 5, 0.,140.);  // Met > 100;
 
     max_MT2ll_SR1 = h_MT2ll_SR1   [ichannel] -> GetBinLowEdge(h_MT2ll_SR1   [ichannel] -> GetNbinsX()+1);  
     max_MT2ll_SR2 = h_MT2ll_SR2   [ichannel] -> GetBinLowEdge(h_MT2ll_SR2   [ichannel] -> GetNbinsX()+1);
     max_MT2ll_SR3 = h_MT2ll_SR3   [ichannel] -> GetBinLowEdge(h_MT2ll_SR3   [ichannel] -> GetNbinsX()+1);

     max_MT2ll_CR1 = h_MT2ll_CR1   [ichannel] -> GetBinLowEdge(h_MT2ll_CR1   [ichannel] -> GetNbinsX()+1);
     max_MT2ll_CR2 = h_MT2ll_CR2   [ichannel] -> GetBinLowEdge(h_MT2ll_CR2   [ichannel] -> GetNbinsX()+1);
     max_MT2ll_CR3 = h_MT2ll_CR3   [ichannel] -> GetBinLowEdge(h_MT2ll_CR3   [ichannel] -> GetNbinsX()+1);

     max_VR  = h_VR  [ichannel] -> GetBinLowEdge(h_VR  [ichannel] -> GetNbinsX()+1);  
    // max_CVR = h_CVR [ichannel] -> GetBinLowEdge(h_CVR [ichannel] -> GetNbinsX()+1);
     
    // max_DY  = h_DY  [ichannel] -> GetBinLowEdge(h_DY [ichannel] -> GetNbinsX()+1);
}     
     
//----------------------------------------------------------
// Reset Histograms
//----------------------------------------------------------

void resetHistograms(int ichannel, TString suffix){

     h_MT2ll_SR1   [ichannel] -> Reset();
     h_MT2ll_SR2   [ichannel] -> Reset();
     h_MT2ll_SR3   [ichannel] -> Reset();

     h_MT2ll_CR1   [ichannel] -> Reset();
     h_MT2ll_CR2   [ichannel] -> Reset();
     h_MT2ll_CR3   [ichannel] -> Reset();

     h_VR          [ichannel] -> Reset();
     //h_CVR         [ichannel] -> Reset();

     //h_DY          [ichannel] -> Reset();

}

//-------------------------------------------------------
// Shape Analysis
//-------------------------------------------------------

void shape_Analysis( TString MassRange = "", float Smass = -1, float Xmass = -1){

     // set mass point
     TString massP; 
     if ( Smass == -1 || Xmass == -1) 
     {
      // for a mass range
      massP = MassRange;
      cout << "Stop mass = " << Smass << "   LSP mass =  " << Xmass << endl;     
     } 
     else 
     { 
      // for a mass point
      massP = massPoint(Smass, Xmass);
      cout << "Stop mass = " << Smass << "   LSP mass =  " << Xmass << endl;     
     }
 
     // define the histograms
     for (int ichannel = 0; ichannel < nchannel; ichannel++) 
     { 
       TString suffix = "_" + schannel[ichannel];
       defineHistograms(ichannel, suffix);
     }

    // for (int s = 1; s < nsample ; s++)
    // for (int s = 0; s < 1 ; s++)
    for (int s = 1; s < nsample ; s++)
     {
       // reset the histograms
       for (int ichannel = 0; ichannel < nchannel; ichannel++) 
       {
         TString suffix = "_" + schannel[ichannel];
         resetHistograms(ichannel, suffix);
       }
       cout << "sample = " << _sample[s] << endl;    
 
       // open the minitrees and make the loop
       cout << "open the minitrees and make the loop" << endl;
       TFile *MiniTreeFile  = TFile::Open(FileAddress + SampleName [s]);
       TTree *MiniTree      = GetMiniTree(MiniTreeFile);
       Int_t  nentries      = (Int_t) MiniTree -> GetEntries();

       for (Int_t en = 0; en<nentries; en++)
       {
         MiniTree -> GetEntry(en);
         eventW *= 36.;
        
         // get the mass point events
         
         if (  Smass != -1 ||  Xmass != -1)
         { //std::cout << Smass << "    " << Xmass << "    " <<std::endl; 
          bool stopsample = true;
          if (_sample[s].Contains("T2tt")) 
          { 
           if ( Mstop != Smass || Mlsp != Xmass) stopsample = false;
          }
          if (stopsample == false) continue;
         }

         // set the channel
         int i=-1;
         if (channel == 3) { i = ee; }
         else if (channel == 4) { i = mm; }
         else if (channel == 5) { i = em; }                  
         if (i==-1) { cout << "Warning! # l < 2 " <<  channel <<  endl; continue;}
         
         // cut and Fill
         // ------------
         // and block the overflow

         //####   tagMVETO   #################
//         if (en == 1) cout << " tagMVETO " << endl;
//         if ( leadingPtCSVv2M > 20) continue;
	 //###################################

         //####   tagMSELECTION  #############
         if ( leadingPtCSVv2M <= 20) continue;
         // ################################# 

         // SR
         if ( mt2ll > 40. && 140 <= metPfType1 && metPfType1 < 200) { if (mt2ll >= max_MT2ll_SR1) mt2ll = max_MT2ll_SR1  -0.01;  h_MT2ll_SR1 [i] -> Fill( mt2ll, eventW );} 
         if ( mt2ll > 40. && 200 <= metPfType1 && metPfType1 < 300) { if (mt2ll >= max_MT2ll_SR2) mt2ll = max_MT2ll_SR2  -0.01;  h_MT2ll_SR2 [i] -> Fill( mt2ll, eventW );}
         if ( mt2ll > 40. && 300 <= metPfType1 )                    { if (mt2ll >= max_MT2ll_SR3) mt2ll = max_MT2ll_SR3  -0.01;  h_MT2ll_SR3 [i] -> Fill( mt2ll, eventW );}
         
         // CR
         if ( mt2ll < 40. && 140 <= metPfType1 && metPfType1 < 200 ){ if (mt2ll >= max_MT2ll_CR1) mt2ll = max_MT2ll_CR1  -0.01;  h_MT2ll_CR1 [i] -> Fill( mt2ll, eventW );}
         if ( mt2ll < 40. && 200 <= metPfType1 && metPfType1 < 300 ){ if (mt2ll >= max_MT2ll_CR2) mt2ll = max_MT2ll_CR2  -0.01;  h_MT2ll_CR2 [i] -> Fill( mt2ll, eventW );}
         if ( mt2ll < 40. && 300 <= metPfType1 )                    { if (mt2ll >= max_MT2ll_CR3) mt2ll = max_MT2ll_CR3  -0.01;  h_MT2ll_CR3 [i] -> Fill( mt2ll, eventW );}

         // VR
         if ( 100 <= metPfType1 && metPfType1 < 140 ) { if (mt2ll >= max_VR) mt2ll = max_VR  -0.01;  h_VR [i] -> Fill( mt2ll, eventW );}
        // if ( mt2ll < 40. && 100 <= metPfType1 && metPfType1 < 140 ) { if (mt2ll >= max_CVR) mt2ll = max_CVR  -0.01;  h_CVR [i] -> Fill( mt2ll, eventW );}   

         // DY
        // if ( metPfType1 < 100) { if (mt2ll >= max_DY) mt2ll = max_DY  -0.01;  h_DY [i] -> Fill( mt2ll, eventW );}     
       }
       // create the folder to save the histograms
       if (_sample[s].Contains("T2tt")) _sample[s] = "T2tt" + massP;
//       gSystem->mkdir("SArootfilebVETO/" + _sample[s] + "/", kTRUE);
       gSystem->mkdir("SArootfilebSEL/" + _sample[s] + "/", kTRUE);
    
//       std::cout << " tagMVETO " << std::endl;
       std::cout << " tagMSELECTION " << std::endl;

       // save the histograms
       cout << " save the histograms " << endl;
       for (int ch = ee; ch < nchannel; ch ++)
       { 


         // SR
//          saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_SR1.root", h_MT2ll_SR1   [ch]);
//          saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_SR2.root", h_MT2ll_SR2   [ch]);
//          saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_SR3.root", h_MT2ll_SR3   [ch]);

         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_SR1.root", h_MT2ll_SR1   [ch]);
         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_SR2.root", h_MT2ll_SR2   [ch]);
         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_SR3.root", h_MT2ll_SR3   [ch]);
        
         // CR
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_CR1.root", h_MT2ll_CR1   [ch]);
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_CR2.root", h_MT2ll_CR2   [ch]);
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_CR3.root", h_MT2ll_CR3   [ch]);

         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_CR1.root", h_MT2ll_CR1   [ch]);
         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_CR2.root", h_MT2ll_CR2   [ch]);
         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_CR3.root", h_MT2ll_CR3   [ch]);
        
         // VR
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_VR.root",  h_VR   [ch]);
         saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_VR.root", h_VR   [ch]);
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_CVR.root", h_CVR   [ch]);
        // saveHistoRoot ("SArootfilebSEL/" + _sample[s] + "/h_MT2ll_CVR.root", h_CVR   [ch]);

        // DY
//         saveHistoRoot ("SArootfilebVETO/" + _sample[s] + "/h_MT2ll_DY.root", h_DY   [ch]);
        // saveHistoRoot ("SArootfilebSEL/"  + _sample[s] + "/h_MT2ll_DY.root", h_DY   [ch]);
   
       } 
     
      }

}







//---------------------------------------------------------------------------------------
// Yields Table
//---------------------------------------------------------------------------------------
//
//	This function gets the yields of each sample in the given region
//
//	Smass, Xmass = sTop and LSP masses
//	region       = MT2ll_SR*, MT2ll_CR*, VR, CVR, DY_R
//	ntag         = bVETO, bSEL
//

void Yield( float Smass, float Xmass, TString region, TString ntag){

  //set the mass point
  TString massP = massPoint(Smass, Xmass);
  
  //initialize a TH1F, TFile, and some float counters
  TH1F  *_histo[nchannel][nsample];
  TFile *_H;  
  float total_B [3] = {0,0,0}; 
  float signal  [3] = {0,0,0};
  //make the loop over all nsample sample  
  cout << " ---------------------------------------------------------" << endl;
  cout << "        get the yields of each channel                    " << endl;
  cout << " ---------------------------------------------------------" << endl;
  for (int s = 0; s < nsample; s++)
  {
   if (_sample[s].Contains("T2tt")) { _sample[s] = "T2tt" + massP;}
   cout << " ---------------------------------------------------------" << endl;
   cout << "SAMPLE     " << _sample[s] << endl; 
 
   //select the SArootfile : SArootfilebVETO or SArootfilebSEL
   if (ntag == "bVETO") 
     { 
      cout << " reading ... " << "SArootfilebVETO/" + _sample[s] + "/" + "h_" + region  +".root" << endl;
   cout << " ---------------------------------------------------------" << endl;
      _H = new TFile( "SArootfilebVETO/" + _sample[s] + "/" + "h_" + region  +".root","read");         
     
     } else if  (ntag == "bSEL") 
     { 
      cout << " reading ... " << "SArootfilebSEL/" + _sample[s] + "/" + "h_" + region  +".root" << endl;
      _H = new TFile( "SArootfilebSEL/" + _sample[s] + "/" + "h_" + region  +".root","read"); 
     } else 
     {
      cout << "your root file does not exit, baby ! " << endl; 
     }

   cout << "                                  " << endl;

   //get the yields of each channel
   for (int ch = 0; ch<nchannel; ch++)
   { 
     cout << "CHANNEL   "  << schannel [ch] << "       "; 
     _histo[ch][s] = (TH1F*) _H-> Get ("h_" + region + "_" + schannel[ch] );
     float integral = _histo[ch][s] -> Integral(0, _histo[ch][s] -> GetNbinsX()+1);
     cout << integral; 
  //   cout << "                                  " << endl;
     if ( _sample[s].Contains("T2tt")) {  signal [ch]    = integral;}
     else if (! _sample[s].Contains("T2tt"))
     { total_B [ch] += integral;
       if ( integral != 0) cout << "             "  << "S/B = " << signal [ch] / integral << endl;
       else cout << "                    "  <<  _sample[s] << "  yield = 0  " << endl;    
     }

   cout << "                                  " << endl;
   cout << "                                  " << endl;
   }
  }
  //get the total yield for the Background of each channel 
  cout << " ---------------------------------------------------------" << endl;
  cout << "        get the total yield of bck of each channel        " << endl;
  cout << " ---------------------------------------------------------" << endl;  
  for (int ch = 0; ch<nchannel; ch++)
  {
    cout << "CHANNEL   "  << schannel [ch] << "       ";
    cout << " TOTAL BCK     " << total_B [ch];
    cout << "                                  " << endl;
    //float signal_integral = _histo[ch][1] -> Integral(0, _histo[ch][1] -> GetNbinsX()+1); 
    cout << " S/B =  " <<  signal [ch]/total_B[ch] << endl;
    cout << "                                  " << endl;
  }
}






//---------------------------------------------------------------------------------------
// Ingredients for Combined Higgs Tool -> Made for 1 variable, for 1 final cut selection
//---------------------------------------------------------------------------------------

void Ingredients(float Smass, float Xmass, TString varName, TString SAdatacardName){

  TString massP = massPoint(Smass, Xmass);
  gSystem->mkdir("SAdatacard/T2tt" + massP + "/", kTRUE);
//  gSystem->mkdir("SAdatacardbVETO/T2tt" + massP + "/", kTRUE);
//  gSystem->mkdir("SAdatacardbSEL/T2tt" + massP + "/", kTRUE);

  TH1F *_histo[nchannel][nsample];

  for (int s = 0; s < nsample; s++)
  {
    if (_sample[s].Contains("T2tt")) { _sample[s] = "T2tt" + massP;}    
//    TFile* _H = new TFile( "SArootfilebVETO/" + _sample[s] + "/" + "h_" + varName  +".root","read");
   TFile* _H = new TFile( "SArootfilebSEL/" + _sample[s] + "/" + "h_" + varName  +".root","read");

    for (int ch = 0; ch<nchannel; ch++)
    {
      _histo[ch][s] = (TH1F*)_H-> Get ("h_" + varName + "_" + schannel[ch] );
      _histo[ch][s] -> SetName(_sample[s] + "_" + schannel[ch]);
      //_histo[ch][s] -> Rebin(4);
    }  
  }

  TFile* OutFileName = new TFile( "SAdatacard/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_G_" + SAdatacardName + "_" + varName + "_TH1F.root" , "recreate");
  cout << " creating ... " << "SAdatacard/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_G_" + SAdatacardName + "_" + varName + "_TH1F.root" << endl;

//  TFile* OutFileName = new TFile( "SAdatacardbVETO/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + "_TH1F.root" , "recreate");
//  cout << " creating ... " << "SAdatacardbVETO/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + "_TH1F.root" << endl;
//  TFile* OutFileName = new TFile( "SAdatacardbSEL/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + "_TH1F.root" , "recreate");
//  cout << " creating ... " << "SAdatacardbSEL/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + "_TH1F.root"  << endl;

  for (int ch = 0; ch < nchannel; ch ++)
  {
    for (int s = 0; s < nsample; s++)
    {
       _histo[ch][s] -> Write();
    }
    _histo[ch][1]->SetName("data_obs_" + schannel[ch]);
    _histo[ch][1]-> Write();
  }
  
  OutFileName-> Close();

  // Datacard .txt writting

  std::ofstream inFile("SAdatacard/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_G_" + SAdatacardName + "_" + varName + ".txt",std::ios::out);
  cout << " creating ... " << "SAdatacard/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_G_" + SAdatacardName + "_" + varName + ".txt" << endl;
//  std::ofstream inFile("SAdatacardbVETO/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + ".txt",std::ios::out);
//  cout << " creating ... " << "SAdatacardbVETO/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + ".txt" << endl;
//  std::ofstream inFile("SAdatacardbSEL/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + ".txt",std::ios::out);
//  cout << " creating ... " << "SAdatacardbSEL/T2tt" + massP + "/" + "T2tt_" + massP + "_shapes_" + SAdatacardName + "_" + varName + ".txt" << endl; 

  inFile << "\n";
  inFile << "   imax  "<<"     " << nchannel   << endl;
  inFile << "   jmax "<<"     " << nsample -1 << endl;
  inFile << "   kmax "<<"     " <<  "*"       << endl;
  inFile << "-----------------------------------------------------------------------------------" << endl;
  inFile << "  shapes * * T2tt_" +  massP + "_shapes_G_" + SAdatacardName + "_" + varName + "_TH1F.root  $PROCESS_$CHANNEL" << endl;
  inFile << "-----------------------------------------------------------------------------------" << endl;
  inFile << "   bin  "<<"      "<< "     ee          "<<"     mm             "<<"      em             "<<endl;
  inFile << "observation           ";
  for (int ch = 0; ch < nchannel; ch++){ float NBck = _histo[ch][1] -> Integral(0, _histo[ch][1] -> GetNbinsX()+1);
  inFile <<"" <<  NBck << "         ";
  }
  inFile << ""<< endl;
  inFile << "-----------------------------------------------------------------------------------" << endl;
  inFile << "   bin  "<<"        ";
  for (int p = 0; p < nsample; p++){
                       inFile << "   ee        "<<"     mm         "<<"      em      ";
  }
  inFile << " " <<endl;
  inFile <<" process "<<"           ";
  for (int s = 0; s < nsample; s++){ for (int ch = 0; ch < nchannel; ch++){
                       inFile << " " << s  << "             ";
   }
  }
  inFile << " " <<endl;
  inFile <<" process              ";
  for (int s = 0; s < nsample; s++){
  for(int ch = 0; ch < nchannel; ch++){
                 inFile << "   " << _sample[s] << "   ";
    }
  }
 inFile << "" <<endl;
 inFile << " rate        ";
 for (int s = 0; s < nsample; s++){ for (int ch = 0; ch < nchannel; ch++){float integral = _histo[ch][s] -> Integral(0, _histo[ch][s] -> GetNbinsX()+1);
   inFile << integral<<"      ";}}
   inFile.close();
}


void All_datacard (float Smass, float Xmass, TString SAdatacardName){
  
       //Ingredients( Smass,  Xmass, "MT2ll_SR1", SAdatacardName);
       //Ingredients( Smass,  Xmass, "MT2ll_SR2", SAdatacardName);
       //Ingredients( Smass,  Xmass, "MT2ll_SR3", SAdatacardName);
       Ingredients( Smass,  Xmass, "MT2ll_CR1", SAdatacardName);
       Ingredients( Smass,  Xmass, "MT2ll_CR2", SAdatacardName);
       Ingredients( Smass,  Xmass, "MT2ll_CR3", SAdatacardName);
}






