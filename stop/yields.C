#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
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
#include "TPie.h"
#include "TPieSlice.h"

#include "StopTrees.h"
//#include "../Razor/Razor.C"
//#include "../Razor/SuperRazor.C"


#include <fstream>
#include <iostream>


//----------------------------------------------
// Global Variables
// ---------------------------------------------

// ...............................................................................................................
// A remember 
// ................................................................................................................
// TString FileAddress = "../../minitreesG/nominal/Stop/";
// const int nsample = 7;
// TString SampleName [nsample] = {"16_T2tt_mStop-150to1200.root", "04_TTTo2L2Nu.root", "05_ST.root" "07_ZJets.root", "06_WW.root", "09_TTV.root", "03_VZ.root"};
// TString  _sample   [nsample] = {"T2tt", "TTbar", "ST", "DYJets", "WW", "TTV", "VZ"};
//----------------------------------------------------------------------------------------------------------------
// T2tt  = T2tt_mStop-150to1200.root
// TTbar = TTTo2L2Nu_ext1__part*.root 
// ST    = ST_tW_antitop.root ST_tW_top.root
// ZJets = DYJetsToLL_M-10to50.root DYJetsToLL_M-50_000*.root;
// WW    = WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
// TTV   = TTWJetsToLNu.root TTZjets.root
// VZ    = ZZTo2L2Q*.root WZTo2L2Q__part*.root
// -----------------------------------------------------------------------------------------------------------------
//..................................................................................................................

//const int nbck    = 6; const TString sbck    [nbck]    = { "TTbar", "ST", "DYJets", "WW", "TTV", "VZ"};
//const int nsignal = 3; const TString tssignal [nsignal] = { "T2tt_Sm350_Xm175", "T2tt_Sm350_Xm225", "T2tt_Sm350_Xm263"}; const TString massP [nsignal] = { "(350,175)", "(350,225)", "(350,263)"};
const int nbck    = 7; const TString sbck    [nbck]    = { "TTbar", "ST", "DYJets", "WW", "TTW", "TTZ", "VZ"};
//const int nsignal = 3; const TString tssignal [nsignal] = { "T2tt250to350", "T2tt350to400", "T2tt400to1200"}; const TString massP [nsignal] = { "(250 to 350)", "(350 to 400)", "(400 to 1200)"};
const int nsignal = 3; const TString tssignal [nsignal] = { "T2tt_Sm275_Xm150", "T2tt_Sm350_Xm225", "T2tt_Sm450_Xm325"}; const TString massP [nsignal] = { "(275,150)", "(350,225)", "(450,325)"};
const int nsample = 8; // nbck + 1;
const int nregion = 7; const TString sregion [nregion] = {"SR1", "SR2", "SR3", "CR1", "CR2", "CR3", "VR"};
//const int nregion = 9; const TString sregion [nregion] = {"SR1", "SR2", "SR3", "CR1", "CR2", "CR3", "VR", "CVR", "DY"};
//"SR1", "SR2", "SR3", "CR1", "CR2", "CR3", "VR1", "VR2", "DY"};
// "SR1", "SR2", "SR3", "CR1", "CR2", "CR3", "VR1", "VR2"};
//"SR1", "SR2", "SR3", "CR1", "CR2", "CR3", "VR", "CVR", "DY" 


enum { ee, mm, em, nchannel}; const TString schannel [nchannel] = {"ee", "mm","em"};

//initialize a TH1F, TFile, and some float counters
TH1F  *_histoB [nchannel][nbck];
TH1F  *_histoS [nchannel][nbck];
TFile *_HB [nbck];
TFile *_HS [nsignal];

float ch_B [nregion][nbck][nchannel];
float ch_S [nregion][nbck][nchannel];

float ch_totB [nregion][nchannel] = {0,0,0}; 


// #### TAG SELECTION ####
//

//-----------------------------------------------------------------------------------------------------------
// Table_txt
//----------------------------------------------------------------------------------------------------------- 
// 	
//	This function gets the yields of each sample in all regions
// 	for just one TAG SELECTION and  make a table in a .txt output file
//
//-----------------------------------------------------------------------------------------------------------


void Table_txt () {

  //create the txt file
  std::ofstream inFile("Yields/23March_bVETO_CRcontrol.txt",std::ios::out);  
  //std::ofstream inFile("iplots/Yields/23March_bVETO_Yields.txt",std::ios::out);  
  
  inFile << Form ("\n  \n"); 
  //inFile << Form ("             TAG SELECTION =  BSELECTION                \n");
  inFile << Form ("             TAG VETO SELECTION =  B VETO                \n");
  inFile << Form ("\n  \n"); 


  inFile << Form ("\n\n  *****  BACKGROUND YIELDS  ***** \n\n");  


  for (int r = 0; r < nregion; r++)
  {
   inFile << "\n" ;
   inFile << Form ("%s %11s", sregion [r].Data(), " ");
   for (int b = 0; b < nbck; b++)
   {// bSEL //bVETO   
   inFile << Form ("%7s %13s", sbck[b].Data(), " " );
   _HB [b] = new TFile( "SArootfilebVETO/" + sbck [b] + "/" + "h_MT2ll_" + sregion [r]  +".root","read");
   }  
   inFile << Form ("%s", " Total MC");
   inFile << Form ("\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

   //get the yields of each channel
   for (int ch = 0; ch<nchannel; ch++)
   {
    inFile << Form("%s %11s", schannel [ch].Data(), " ");

    for (int b = 0; b < nbck; b++)
    {
     _histoB[ch][b] = (TH1F*) _HB[b]-> Get ("h_MT2ll_" + sregion [r] + "_" + schannel[ch] );
     float integral = _histoB[ch][b] -> Integral(0, _histoB[ch][b] -> GetNbinsX()+1);
     ch_B [r][b][ch]  = integral;
     ch_totB [r][ch] += integral;
     inFile << Form ("%9.2f %11s", integral, " ");
    }
    inFile << Form ("%9.2f", ch_totB [r][ch]);
    inFile << "\n" << endl; 
   }
  }

   inFile << Form ("\n  \n");
   inFile << Form ("\n\n  *****  SIGNAL YIELDS  ***** \n\n");



   for (int r = 0; r < nregion; r++)
  {
   inFile << "\n" ;
   inFile << Form ("%s %7s", sregion [r].Data(), " ");
   for (int s = 0; s < nsignal; s++)
   {//bSEL//bVETO
    inFile << Form ("%7s %11s", tssignal[s].Data(), " ");
    _HS [s] = new TFile( "SArootfilebVETO/" + tssignal [s] + "/" + "h_MT2ll_" + sregion [r]  +".root","read");
   }
   inFile << Form ("\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"); 
   
   //get the yields of each channel
   for (int ch = 0; ch<nchannel; ch++)
   {
    inFile << Form("%s %11s", schannel [ch].Data(), " ");
    for (int s = 0; s < nsignal; s++)
    {
     _histoS[ch][s] = (TH1F*) _HS[s]-> Get ("h_MT2ll_" + sregion [r] + "_" + schannel[ch] );
     float integral = _histoS[ch][s] -> Integral(0, _histoS[ch][s] -> GetNbinsX()+1);
     ch_S [r][s][ch] = integral;
     inFile << Form ("%9.2f %16s", integral, " ");
    }
    inFile << "\n" << endl;
   } 
  }
 
    
  inFile << Form ("\n\n  *****  SIGNAL / BACKGROUND RATIO  ***** \n\n");

   for (int r = 0; r < nregion; r++)
  {
   for (int s = 0; s < nsignal; s++)
   {
    inFile << "\n" ;
    inFile << Form ("%s %2s %4s", sregion [r].Data(), massP [s].Data(), " ");
    for (int b = 0; b < nbck; b++) { inFile << Form ("%7s %13s", sbck[b].Data(), " " );}
    inFile << Form ("%s", " Total MC");
    inFile << Form ("\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

   for (int ch = 0; ch<nchannel; ch++)
   {
    inFile << Form("%s %11s", schannel [ch].Data(), " ");
    for (int b = 0; b < nbck; b++)
    {
     float ratio = ch_S [r][s][ch] / ch_B [r][b][ch]; 
     inFile << Form ("%9.2f %11s", ratio, " ");
     }
    float ratio_totB = ch_S [r][s][ch] / ch_totB [r][ch];
    inFile << Form ("%9.2f", ch_totB [r][ch]);

    inFile << "\n" << endl;
    }

   }
  inFile << Form ("\n  \n"); 
  inFile << Form ("\n  \n"); 
  inFile << Form ("\n  \n"); 
  }
 inFile.close();
//gSystem->cp("Yields/23March_bVETO_Yields.txt iplots/Yield/.", kTRUE);  
} 





//---------------------------------------------------------------------------------------------------------------------
// PiePlot 
//---------------------------------------------------------------------------------------------------------------------
//
//	This function plots a pie divided into the same number of slices as the number of samples you do have
//	It works for one signal sample at time and USE ONLY the information saved by Table_txt function
//
//---------------------------------------------------------------------------------------------------------------------


void PiePlot ( int SignalSample, int iregion){
  
  Table_txt();  

 std::cout << "type of signal point\n" << std::endl; 
 std::cout << "s = 0 -> T2tt250to350  ; T2tt_Sm350_Xm175\n" << "s = 1 -> T2tt350to400  ; T2tt_Sm350_Xm225\n" << "s = 2 -> T2tt400to1200 ; T2tt_Sm350_Xm263" <<std::endl; 
 std::cout << "type region\n" << std::endl;
 std::cout << "r = 0,1,2 -> SR1,SR2,SR3\n" << "r = 3,4,5 -> CR1,CR2,CR3\n" << "r = 6,7   -> VR,CVR\n" << "r = 8     -> DY" << std::endl; 


 /*
 *
 *          @Inputs:
 *
 *              s = 0 -> T2tt250to350  ; T2tt_Sm350_Xm175
 * 		s = 1 -> T2tt350to400  ; T2tt_Sm350_Xm225
 * 		s = 2 -> T2tt400to1200 ; T2tt_Sm350_Xm263
 *
 * 		r = 0,1,2 -> "SR1,SR2,SR3"
 * 		r = 3,4,5 -> "CR1,CR2,CR3"
 * 		r = 6,7   -> "VR,CVR"
 * 		r = 8     -> "DY" 
 *
 */  
 
 
  int r  = iregion; 
  
  //set the colours
 
  //	2  = "red"           -> Sample
  //	5  = "yellow"        -> ttbar
  //	41 = "browny yellow" -> st
  //	3  = "brigth green"  -> dyjets
  //	7  = "turqoise       -> ww
  //	8  = "dark green"    -> ttw
  //	12 = "dark brown"    -> ttz
  //	28 = "brown"         -> vz
  //
  
  int colors[nbck +1] = {2,5,41,3,7,8,12,28};

  //make the plot for each channel in the selected region for the signal sample 
  for ( int ch = 0; ch < nchannel; ch ++)
  {
   //fill values variable with each yield
   float values [nsample];
   values [0] = {ch_S [r][SignalSample][ch]};
   for (int b = 0; b < nbck; b++){ int c = b +1; values [c] = {ch_B [r][b][ch]};}
   int nvalues = sizeof(values)/sizeof(values[0]);
   
   //set the canvas
   TCanvas *cpie = new TCanvas("cpie_" + sregion[r] + "_" + schannel[ch],"cpie_" + sregion[r] + "_" + schannel[ch], 1200,1200);
   
   //fill the pie
   TPie *pie = new TPie("pie_" + sregion[r] + "_" + schannel[ch] ,"  bVETO  " + sregion[r] + "   " + schannel[ch], nvalues, values ,colors);
   
   //block the labels on the slices
   const char * label[nsample] = { " ", " ", " ", " ", " ", " ", " "};
   pie -> SetLabels(label);
   
   //set the legend
   TLegend *pieleg = new TLegend(.65, .65, .95, .95);
   TString legTitle = Form(" %3s %6s %5s %7s %5s %5s %5s %1s", " ", "colour", " ",  "process", " ", "yield", " ", "% yield/All");
   pieleg ->  SetHeader(legTitle);
   pieleg -> SetX1 (.0);  pieleg -> SetX2 (.3); 

   //fill the legend
   float mcTotal = ch_totB [r][ch] + values [0];
   for (int i = 0; i < nsample; i++)
   {
    float percent = (values [i]/mcTotal)*100;
    //char process = leglabel[i];
    const char* leglabel[nsample]  = { tssignal[SignalSample], "TTbar", "ST", "DYJets", "WW", "TTW", "TTZ", "VZ"};
    TString flabel = Form ("%7s %4s %7.0f %.4s %s %3.1f %s", leglabel[i], " ", values[i], " ", "(", percent, ")" );
    TPieSlice * slice = pie -> GetSlice(i);
    pieleg -> AddEntry( slice ,flabel,"lpf");
   } 
  
   //some cosmetics
   pie->SetEntryRadiusOffset(2,.05);
   pie->SetEntryRadiusOffset(4,.15);
   pie->SetEntryRadiusOffset(5,.10);
   pie->SetEntryRadiusOffset(6,.25);
   //pie4->SetLabelsOffset(.001);
   // pie4 -> SortSlices (kTRUE, .0);
   pie->SetAngularOffset(60.);
   pie -> SetCircle(.5,.5,.30);
   //pie4->SetLabelFormat("#splitline{%val (%perc)}{%txt}");
   //pie4->SetLabelFormat("#splitline{%val}{%txt}");

   //draw
   pie->Draw("3d <"); 
   pieleg -> Draw();
  } 
}

