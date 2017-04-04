/*
* ===============================================================================================================================
*                     FitAnalyzer.C
*
*
*      This script uses the mlfit.root from combine MasLikeliHoodFit performing as input. It gets the shape uncertainty 
*      between the data and mc background shapes in each of the Validation Regions (VR), namely, Validation Region after 
*      asking a b jet (Tag) and Validation Region after vetoing a b jet (NoTag).                 
*      
*      Name of regions to perform the fit: VR1Tag, VR1NoTag, VR1. Out files produced by the fit:  mlfitVR1Tag.root, mlfitVR1NoTag.root, mlfitVR1.root 
*     
*      The general adress and suffix used: 
*
*      Input fit file:   the corresponding mlfit.root ( "*./mlfitVR1Tag.root" || "*./mlfitVR1NoTag.root" || "*./mlfitVR1.root")
*      Input shape file: "./Shapes/ShapesVR1.root"
*      Output dir:       "./Datacards"
*      Output suffix:    "Tag", "NoTag"
*
*      Output file:      OutputDirectory ** + "/VR1" + FitRegion + "Fit.root";
*
*      **  OutputDirectory = Datacards/
*      * == "./Datacards/ValidationRegions/" -> Directory where are keeped the fit file
*
* ================================================================================================================================
*/

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include "../../../../AnalysisCMS/include/CutsStop.h"


//----------------------
// Global Variables
//----------------------

// set the channels 
const int nChannels = 3;
TString Channel[nChannels] = {"em", "ee", "mm"};

// set the samples 
TString DataSampleName = "01_PseudoDataSmeared"; // Data sample
const int nBackgroundSamples = 7;
TString BackgroundSampleName[nBackgroundSamples] = {"04_TTTo2L2Nu", "05_ST", "06_WW", "03_VZ", "07_ZJets", "09_TTW", "10_TTZ"}; // Background samples

// init some stuff
bool VRForChannel = false;
TString FitDirectory = "shapes_fit_b";
bool VR1FitSplitChannels = false;
TString shapeRoot = "Shapes/Shapes_mStop-250to350_Sm300_Xm200.root"; // You can use any root file for whatever mass point. Never mind the mass point in study (mc background does not change)   

/*-------------------------
 *        VR1Fit
 *-------------------------
 *   This function uses the data and background shapes to create the shape uncertainty histograms for each channel (ee,mm,em) and each cut.
 *   The ratio between data/mc is used as SF to correct the mc shape. Because of not simetrical shape uncertanty is considered, here it is just calculated the systematic shape uncertainty up histogram. 
 *   For the systematic shape uncertainty up histogram we are using the nominal one. 
 *   
 *   This shapes uncertainties are obtained only for TTbar + TW process in Tag region and only for WW process in NoTag region.
 * 
 *   @param TString FitFileName     => Input fit file  
 *   @param TString ShapeFileName   => Input shape file
 *   @param TString OutputDirectory => Output dir
 *   @param TString FitRegion       => Output suffix 
 */
void VR1Fit(TString FitFileName, TString ShapeFileName, TString OutputDirectory, TString FitRegion) 
{
  // open the input files
  TFile *FitFile    = new TFile(FitFileName, "read");
  TFile *ShapeVRFile  = new TFile(ShapeFileName, "read");
  TFile *ShapeFile  = new TFile(shapeRoot, "read"); 

  // Set number of bins and edges of the variable histogram. For the moment only one variable/histogram
  int nBins = 7; float LowEdgeHisto = 0., HighEdgeHisto = 140.;

  // init the output histograms for data and backgrounds 
  TH1F *DataHisto[nChannels+1], *BkgHisto[nChannels+1];
  
  for (int ch = 0; ch<nChannels; ch++) 
  {
    // exclusive channels: ee, mm, em 
    DataHisto[ch]    = new TH1F("Data_"  + Channel[ch], "", nBins, LowEdgeHisto, HighEdgeHisto);
    BkgHisto[ch]     = new TH1F("Bkg_"   + Channel[ch], "", nBins, LowEdgeHisto, HighEdgeHisto);

    if (ch==nChannels-1) 
    {
      // inclusive channel: ll 
      DataHisto[nChannels]    = new TH1F("Data_all",  "", nBins, LowEdgeHisto, HighEdgeHisto);
      BkgHisto[nChannels]     = new TH1F("Bkg_all",   "", nBins, LowEdgeHisto, HighEdgeHisto);
    }

  }

 /* Loop 1
  * ----------
  * Looping over the three exclusive channels (ee,mm,em) to fill the DataHisto and BckHisto output histogram per channel by adding in function of Tag, NoTag FitRegions: 
  *    - In DataHisto the data histogram shape stored in the input shape file  
  *    - In BackHisto the post fit histogram shape of each background proccess stored in the input fit file
  */

  // Special reference for data input shape file, namely, VR1_ + Tag, VR1_ + NoTag regions
  TString VRName  = "VR1_" + FitRegion;

  std:: cout << "1" << std::endl;
  for (int ch = 0; ch<nChannels; ch++) 
  { 
    // Add data from the input shape file
    TString DataHistoName = VRName + "_" + Channel[ch] + "/MT2ll/histo_" + DataSampleName;
    TH1F *ThisDataHisto = (TH1F*) ShapeVRFile->Get(DataHistoName);
    DataHisto[ch]->Add(ThisDataHisto);
   
  std:: cout << "1.1" << std::endl;
    // Add background from the input fit file 
    TString Bin = "";
    if (!FitFileName.Contains("Tag")) Bin = FitRegion + "_"; // Special suffix for mlfitVR1.root
    
    for (int bkg = 0; bkg<nBackgroundSamples; bkg++) 
    {
  std:: cout << "1.2" << std::endl;
      // The combine mlfit.root post fit histograms do not keep the values of bin edges in x-axis, so, you need to rebuild them if you want to compare the shapes with data
      TString BackHistoName = FitDirectory + "/" + Bin + Channel[ch] + "/" + BackgroundSampleName[bkg];
      TH1F *FitBackHisto = (TH1F*) FitFile->Get(BackHistoName);
  std::cout << BackHistoName << std::endl;
      TH1F *ThisBackHisto = new TH1F("Fit_" + BackgroundSampleName[bkg] + "_" + Channel[ch], "", nBins, LowEdgeHisto, HighEdgeHisto);
  std:: cout << "1.3" << std::endl;
      for (int ib = 1; ib<=nBins; ib++) 
      {
	ThisBackHisto->SetBinContent(ib, FitBackHisto->GetBinContent(ib));
	ThisBackHisto->SetBinError(ib, FitBackHisto->GetBinError(ib));

      }
      // Add the backgrounds histograms selecting them in function of the FitRegion  
  std:: cout << "1.4" << std::endl;
      if (FitRegion=="Tag" && bkg<2) {BkgHisto[ch]->Add(ThisBackHisto);// For the Tag FitRegion we add only "04_TTTo2L2Nu", "05_ST", "06_WW" backgrounds
    std::cout << "FitRegion==Tag" << std::endl;}
      else if (FitRegion=="NoTag" && bkg==2) {BkgHisto[ch]->Add(ThisBackHisto); // For the NoTag FitRegion we add only "06_WW" backgrounds
    std::cout << "FitRegion==NoTag" << std::endl;}
      else DataHisto[ch]->Add(ThisBackHisto, -1.); // For the Tag FitRegion we subtract "03_VZ", "07_Zjets", "09_TTW", "10_TTZ"; For the NoTag FitRegion we subtract "04_TTTo2L2Nu", "05_ST", "03_VZ", "07_Zjets", "09_TTW", "10_TTZ"

     }
    
    // Fill the DataHisto and BckHisto output histogram in the inclusive channel (ll)
    DataHisto[nChannels] -> Add(DataHisto[ch]); 
    BkgHisto[nChannels]  -> Add(BkgHisto[ch]);

  }
std::cout << "2" << std::endl; 
  // Set the output root file
  TString OutputFileName = OutputDirectory + "/VR1" + FitRegion + "Fit.root";
  if (!FitFileName.Contains("Tag") && FitRegion=="Tag") OutputFileName.ReplaceAll(".root", "_step2.root"); // To denote that mlfitVR1.root in Tag FitRegion is an intermediate step when using this code for its analysis purpose Action=="FitVR1"
  TFile *OutputFile = new TFile(OutputFileName, "recreate");
  
std::cout << "3" << std::endl; 
  /* Loop 2
   * --------
   * Save the BkgHisto output histograms. 
   * Create a new DataHisto[ch] storing the ratio Data/MC in each bin. It is saved and called "Ratio"
   */  
  for (int ch = 0; ch<nChannels+1; ch++)
  {

    DataHisto[ch]->Write();
    BkgHisto[ch]->Write();
    DataHisto[ch]->Divide(BkgHisto[ch]);
    TString HistoName = DataHisto[ch]->GetName();
    HistoName.ReplaceAll("Data", "Ratio");
    DataHisto[ch]->SetName(HistoName);
    DataHisto[ch]->Write();

  }

 /* Loop 3
  * --------
  * Create the shape uncertainty histograms for each channel (ee,mm,em) and each cut 
  *
  * Stop_02_VR1_Tag          Stop_02_SR1_Tag            Stop_02_SR2_Tag            Stop_02_SR3_Tag
  * Stop_02_VR1_NoTag        Stop_02_SR1_NoTag          Stop_02_SR2_NoTag          Stop_02_SR3_NoTag 
  *
  */

std::cout << "4" << std::endl; 
  for (int ct = 0; ct<ncut; ct++)
  {
    TString SRName = scut[ct];
    SRName.ReplaceAll("Stop/02_", ""); // forget inconvienient prefix
    std::cout << SRName << std::endl;     
    if (!SRName.Contains("VR1") && !SRName.Contains("SR")) continue; // do not take into account other cuts of the analysis
    //if (SRName!="VR1_NoTag" && !SRName.Contains("SR")) continue;
    //if (SRName=="VR1_NoTag" && FitRegion=="NoTag") continue;
    
std::cout << "5" << std::endl; 
    for (int ch = 0; ch<nChannels; ch++)
    {
    
std::cout << "6" << std::endl; 
      TString CutName = SRName + "_" + Channel[ch]; // Following the storage criteria of mkdatacards.py

      OutputFile->mkdir(CutName);
      OutputFile->cd(CutName);

      for (int tbg = 0; tbg<=2; tbg++)
      {

std::cout << "7" << std::endl;
 
	if (FitRegion=="Tag" && tbg==2) continue;  // use "04_TTTo2L2Nu", "05_ST" avoid "06_WW" and the rest backgrounds 
	if (FitRegion=="NoTag" && tbg<2) continue; // use "06_WW" avoid "04_TTTo2L2Nu", "05_ST" and the rest background
	
std::cout << "8" << std::endl; 
	TString HistoName = "histo_" + BackgroundSampleName[tbg];
	TH1F *ThisHisto;
	if (!SRName.Contains("SR")) ThisHisto = (TH1F*) ShapeVRFile->Get(CutName + "/MT2ll/" + HistoName); // Take the corresponding background histogram from input shape file: validation region
	else ThisHisto = (TH1F*) ShapeFile->Get(CutName + "/MT2ll/" + HistoName); // Take the corresponding background histograms of SR input shape file: shapeRoot
	ThisHisto->Write();

std::cout << "9" << std::endl; 
        //Create and save the systematic shape uncertainty histograms 	
	TString SystematicName = tbg<2 ? "_TopMT2llShape" : "_WWMT2llShape";
	HistoName += SystematicName;
	if (VRForChannel) HistoName += ("_" + Channel[ch]); 
	ThisHisto->SetName(HistoName + "Down"); // Actually is the nominal one
	ThisHisto->Write();
	if (VRForChannel) ThisHisto->Multiply(DataHisto[ch]); 
	else ThisHisto->Multiply(DataHisto[nChannels]); 
	ThisHisto->SetName(HistoName + "Up"); // Systematic up 
	ThisHisto->Write();

      }      
    }
  }

  OutputFile->Close();
  
std::cout << "fin" << std::endl; 
}

/* FitAnalyzer
 * -------------
 * 
 *  This function call VR1Fit function through the parameter Action 
 *  
 *  @param TString Action 
 *  
 */

void FitAnalyzer(TString Action) {
  
  if (Action.Contains("FitVR1Tag"))  
    VR1Fit("./Datacards/ValidationRegions/mlfitVR1Tag.root", "./Shapes/ShapesVR1_mStop-250to350_Sm300_Xm125.root", "./Datacards", "Tag");
  
  if (Action.Contains("FitVR1NoTag"))
    VR1Fit("./Datacards/ValidationRegions/mlfitVR1NoTag.root", "./Shapes/ShapesVR1_mStop-250to350_Sm300_Xm125.root", "./Datacards", "NoTag");
  
  if (Action=="FitVR1") {
    VR1Fit("./Datacards/ValidationRegions/mlfitVR1.root", "./Shapes/ShapesVR1_mStop-250to350_Sm300_Xm125.root", "./Datacards", "NoTag");
    VR1Fit("./Datacards/ValidationRegions/mlfitVR1.root", "./Shapes/ShapesVR1_mStop-250to350_Sm300_Xm125.root", "./Datacards", "Tag");
  }

}
