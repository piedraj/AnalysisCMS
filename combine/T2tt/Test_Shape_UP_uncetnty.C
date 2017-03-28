# include "TFile.h"
# include "TH1F.h"
# include "TSystem.h"
# include "TLegend.h"
# include "TCanvas.h"
# include "TStyle.h"
# include <fstream>
# include <iostream>



//-----------------------------------------
// CheckYields
//-----------------------------------------


TH1F* CheckYields (TH1F* histo)
{     
      int nBins = histo -> GetNbinsX();
      std::cout<<"nbins "<< nBins <<std::endl;
      float histoIntegral = histo -> Integral();
      std::cout<<"Integral " << histoIntegral<<std::endl;
      for (int n = 1; n <= nBins; n++ )
      {
        float yValue = histo -> GetBinContent(n);
        if (yValue <= 0.) histo -> SetBinContent (n, 0.001);
      }
      float correctedIntegral = histo -> Integral();
      if (correctedIntegral != histoIntegral && histoIntegral!= 0) histo -> Scale (correctedIntegral/histoIntegral);
      std::cout << "corrected Integral" << correctedIntegral << std::endl; 
      return histo;

}


//-------------------------------------------------------
// Normalize the Histogram to Unity 
//-------------------------------------------------------


void MakeShape (TH1F *histoName){
    float integral;
    integral = histoName -> Integral(0, histoName -> GetNbinsX()+1);
    cout  << integral << endl; 
    histoName -> Scale(1/integral);
}

//-----------------------------------------------------
// Set Legend 
//-----------------------------------------------------

TLegend *myLegend ( TString title ){

    TLegend *leg1;
    leg1 = new TLegend(0.75, 0.9, 0.9, 0.7);
    leg1->SetFillColor(kWhite); leg1->SetBorderSize(0.);
    leg1->SetTextColor(1); //leg1->SetTextSize(0.020);
    //leg1->SetTextFont(10);
    leg1->AddEntry((TObject*)0, " ", "");
    
    return leg1; 
}

void Smearing_Shapes (TString inputDir = "Datacards/ValidationRegions/")
  
    /*
    *    This function does a "smearing up" of a set of .root histograms. 
    *    It returns a rootfile containing these smeared histos normalized 
    *    to unity.
    */ 

 {   std::cout << "init" << std::endl;
     // set the variables  
     const int nsample = 3;  TString sampleName [nsample] = {"06_WW", "05_ST", "04_TTTo2L2Nu"};
     const int ncut   = 2;  TString cutName [ncut]    = {"VR1_Tag", "VR1_NoTag"};
     enum { ee, mm, em, nchannel}; const TString schannel[nchannel] = {"ee", "mm","em"}; 
   
     // make tho output folder
     gSystem->mkdir("VRTesting/", kTRUE);

     std::cout << "start the loops" << std::endl;

     /* start the loop */
     
     // for each sample
     for (int i = 0; i < nsample; i ++)
     {
       std::cout << sampleName[i] << std::endl;
       // set the output rootfile name. Then recreate it  
       TString outFileName = ""; 
       if (sampleName[i] == "06_WW")          outFileName = "06_WW_smearing.root";
       if (sampleName[i] == "05_ST" )         outFileName = "05_ST_smearing.root";
       if (sampleName[i] == "04_TTTo2L2Nu")   outFileName = "04_TTTo2L2Nu_smearing.root";

       TFile* fileOut =  new TFile ("VRTesting/" + outFileName, "recreate"); 
       
       // for each cut
       for (int j = 0; j < ncut; j ++)
       {       
        std::cout << cutName[j] << std::endl;
        // for each channel
        for ( int k = 0; k < 3 ; k ++)
        {
         std::cout << schannel[k] << std::endl;
         // open the input file
         TString inputFile = inputDir + cutName[j] + "_" + schannel[k] + "/MT2ll/shapes/" + "histos_" +  cutName[j] + "_" + schannel[k] + ".root";
         TFile*  fileIn    = new TFile(inputFile, "read");
//         TH1F* histoOut = HistoTest ( fileIn, sampleName[i], cutName[j], schannel[k]);
         // take the histo
         TH1F*   histoIn   = (TH1F*) fileIn -> Get("histo_" + sampleName[i]);  
         
         // check the bad bins (Bin content < 0 )
         TH1F* histo = CheckYields (histoIn);

         // do the smearing up
         histo->Scale(1.);
         histo->SetBinContent(1, histo->GetBinContent(1)*1.000);
         histo->SetBinContent(2, histo->GetBinContent(2)*1.017);
         histo->SetBinContent(3, histo->GetBinContent(3)*1.032);
         histo->SetBinContent(4, histo->GetBinContent(4)*1.050);
         histo->SetBinContent(5, histo->GetBinContent(5)*1.067);
         histo->SetBinContent(6, histo->GetBinContent(6)*1.082);
         histo->SetBinContent(7, histo->GetBinContent(7)*1.100);
        
         // normalize histo to unity
         MakeShape(histo);
         
         // just clone to be safe
         TH1F* histoOut = (TH1F*)  histo -> Clone();
        
         // write the produced histo 
         fileOut->cd();
         histoOut -> SetName("h_" + sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);
         histoOut -> Write();
         // close the input file
         fileIn -> cd();
         fileIn -> Close();
        }
       }      
       // close the input file
       fileOut->cd();
       fileOut -> Close();
       std::cout << "FIN" << std::endl;
      } 
}      
 

//--------------------------------------------------------------
// Compare_Shapes
//-------------------------------------------------------------- 

void Compare_Shapes(bool Tag = false)
{
  // set the variables 
  enum { ee, mm, em, nchannel}; const TString schannel[nchannel] = {"ee", "mm","em"};
  gSystem->mkdir("VRTesting/VRTest/", kTRUE);

  if (Tag)
  { 
   // Tag Region
   TString cutName = "VR1_Tag";
   const int nsample = 2;  TString sampleName [nsample] = {"05_ST", "04_TTTo2L2Nu"};
   TString inputFile1 = "Datacards/VR1TagFit.root";
   TFile* fileIn1 = new TFile(inputFile1, "read");   

   // for top samples
   for (int i = 0; i < nsample; i ++)
   {
    std::cout << sampleName[i] << std::endl;
    // open the input file2
    TString inputFile2 = "VRTesting/";
    if (sampleName[i] == "05_ST" )        inputFile2  += "05_ST_smearing.root";
    if (sampleName[i] == "04_TTTo2L2Nu")  inputFile2  += "04_TTTo2L2Nu_smearing.root";

    TFile*  fileIn2  = new TFile(inputFile2, "read"); 
 
    // for each channel
    for ( int k = 0; k < 3 ; k ++)
    {
     // take histo1 -> Top uncertainty shape
     TH1F* histo1 = (TH1F*) fileIn1 -> Get(cutName + "_" + schannel[k]  + "/" + "histo_" + sampleName[i] + "_TopMT2llShapeUp");
     // normalize histo1 to unity
     std::cout << histo1 -> Integral() << std::endl; 
     MakeShape(histo1);
     // take histo2 -> Top smeared histo shape
     TH1F* histo2 = (TH1F*) fileIn2 -> Get("h_" + sampleName[i] + "_" + schannel[k] + "_" + cutName);
     // normalize histo2 to unity
     MakeShape(histo2);


     // draw histo1, histo2
     TCanvas* Top_canvas = new TCanvas (sampleName[i] + "_" + schannel[k] + "_" + cutName, "", 1200, 1000);
     gStyle -> SetOptStat("");

     histo1 -> SetLineColor(1);
     histo2 -> SetLineColor(6);
     
     TLegend* _leg = myLegend (sampleName[i] + "_" + schannel[k] + "_" + cutName);
     _leg -> AddEntry(histo1, "Top uncertainty shape", "f");
     _leg -> AddEntry(histo2, "Top smeared histo shape", "f");
   
     histo1 -> SetTitle(sampleName[i] + "_" + schannel[k] + "_" + cutName);
     histo1 -> SetXTitle("MT2ll");  

     histo1 -> Draw();
     histo2 -> Draw("same");
     _leg ->Draw();
   
     Top_canvas -> Print("VRTesting/VRTest/" + sampleName[i] + "_" + schannel[k] + "_" + cutName);
    }
   }
  }

  else
  {
   // No Tag Region
   TString cutName = "VR1_NoTag";
   TString sampleName = "06_WW";
   TString inputFile1 = "Datacards/VR1NoTagFit.root";
   TFile* fileIn1 = new TFile(inputFile1, "read");

   TString inputFile2 = "VRTesting/06_WW_smearing.root";
   TFile*  fileIn2  = new TFile(inputFile2, "read");
std::cout << "1" << std::endl; 
   for ( int k = 0; k < 3 ; k ++)
   {
    // take histo1 -> WW uncertainty shape
    TH1F* histo1 = (TH1F*) fileIn1 -> Get(cutName + "_" + schannel[k] +  "/" + "histo_" + sampleName + "_WWMT2llShapeUp");
std::cout << histo1 -> GetName() << std::endl; 
    // normalize histo1 to unity
    MakeShape(histo1);
std::cout << "3" << std::endl; 
    // take histo2 -> Top smeared histo shape
    TH1F* histo2 = (TH1F*) fileIn2 -> Get("h_" + sampleName + "_" + schannel[k] + "_" + cutName);
std::cout << "4" << std::endl; 
    // normalize histo2 to unity
    MakeShape(histo2);
std::cout << "5" << std::endl; 

    // draw histo1, histo2
    TCanvas* WW_canvas = new TCanvas (sampleName + "_" + schannel[k] + "_" + cutName, "", 1200, 1000);
    gStyle -> SetOptStat("");

    histo1 -> SetLineColor(1);
    histo2 -> SetLineColor(6);

    TLegend* _leg_bis = myLegend (sampleName + "_" + schannel[k] + "_" + cutName);
    _leg_bis -> AddEntry(histo1, "WW uncertainty shape", "f");
    _leg_bis -> AddEntry(histo2, "WW smeared histo shape", "f");

    histo1 -> SetTitle(sampleName + "_" + schannel[k] + "_" + cutName);
    histo1 -> SetXTitle("MT2ll");  

    histo1 -> Draw();
    histo2 -> Draw("same");
    _leg_bis ->Draw();

    WW_canvas -> Print("VRTesting/VRTest/" + sampleName + "_" + schannel[k] + "_" + cutName);
    }
  }

  gSystem->Exec("cp  ./index.php ./VRTesting/VRTest/; done");
  gSystem->Exec("for dir in $(find ./VRTesting/VRTest/ -type d); do cp -n ./index.php $dir/; done"); 
} 
   
/*

//--------------------------------------------------------------
// Compare_Shapes
//-------------------------------------------------------------- 

void Compare_Shapes()
{
  // set the variables 
  const int nsample = 3;  TString sampleName [nsample] = {"06_WW", "05_ST", "04_TTTo2L2Nu"};
  const int ncert   = 2;  TString uncertainty [ncert]  = { "TopMT2llShapeUp", "WWMT2llShapeUp"}; 
  const int ncut    = 2;  TString cutName [ncut]       = {"VR1_Tag", "VR1_NoTag"};
  enum { ee, mm, em, nchannel}; const TString schannel[nchannel] = {"ee", "mm","em"};

   // start the loop 
   
   gSystem->mkdir("VRTesting/VRTest/", kTRUE);
  
  // for each cut
  for (int j = 0; j < ncut; j ++)
  {
    std::cout << cutName[j] << std::endl;

   // open the input file1
   TFile* fileIn1;
   if (cutName[j] == "VR1_NoTag") {TString inputFile1 = "Datacards/VR1notagFit.root"; fileIn1    = new TFile(inputFile1, "read");} 
   if (cutName[j] == "VR1_Tag")   {TString inputFile1 = "Datacards/VR1tagFit.root";   fileIn1    = new TFile(inputFile1, "read");} 
   
   // for each sample
   for (int i = 0; i < nsample; i ++)
   {
    std::cout << sampleName[i] << std::endl;

    // open the input file2 
    TString inputFile2 = "VRTesting/"; 
    if (sampleName[i] == "06_WW")         inputFile2 += "06_WW_smearing.root";
    if (sampleName[i] == "05_ST" )        inputFile2  = "05_ST_smearing.root";
    if (sampleName[i] == "04_TTTo2L2Nu")  inputFile2  = "04_TTTo2L2Nu_smearing.root";

    TFile*  fileIn2  = new TFile(inputFile2, "read"); 
       
           
    // for each channel
    for ( int k = 0; k < 3 ; k ++)
    {
     if (cutName[j] == "VR1_Tag")
     {
      // take histo1 -> Top uncertainty shape
      TH1F* histo1 = (TH1F*) fileIn1 -> Get(cutName[j] + "_" + schannel[k]  + "/" + "histo_" + sampleName[i] + "_TopMT2llShapeUp");
      // normalize histo1 to unity
      MakeShape(histo1);
      // take histo2 -> Top smeared histo shape
      TH1F* histo2 = (TH1F*) fileIn2 -> Get("h_" + sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);
      // normalize histo2 to unity
      MakeShape(histo2);

      // draw histo1, histo2 
      TCanvas Top_canvas = new TCanvas (sampleName[i] + "_" + schannel[k] + "_" + cutName[j], "", 1200, 1000);
      gStyle -> SetOptStat("");
      
      TLegend* _leg = myLegend (sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);      
      _leg -> AddEntry(histo1, "Top uncertainty shape", "f");
      _leg -> AddEntry(histo2, "Top smeared histo shape", "f");
      _leg ->Draw();
      
      histo1 -> Draw();
      histo2 -> Draw("same");
   
      Top_canvas -> Print("VRTesting/VRTest/" + sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);
     }

     if (cutName[j] == "VR1_NoTag" )
     {
      // take histo1 -> WW uncertainty shape
      TH1F* histo1 = (TH1F*) fileIn1 -> Get(cutName[j] + "_" + schannel[k] "/" + "histo_" + sampleName[i] + "_WWMT2llShapeUp");
      // normalize histo1 to unity
      MakeShape(histo1);
      // take histo2 -> Top smeared histo shape
      TH1F* histo2 = (TH1F*) fileIn2 -> Get("h_" + sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);
      // normalize histo2 to unity
      MakeShape(histo2);
     
      // draw histo1, histo2 
      TCanvas WW_canvas = new TCanvas (sampleName[i] + "_" + schannel[k] + "_" + cutName[j], "", 1200, 1000);
      gStyle -> SetOptStat("");
      
      TLegend* _leg_bis = myLegend (sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);      
      _leg_bis -> AddEntry(histo1, "WW uncertainty shape", "f");
      _leg_bis -> AddEntry(histo2, "Top smeared histo shape", "f");
      _leg_bis ->Draw();
 
      histo1 -> Draw();
      histo2 -> Draw("same");
  
      WW_canvas -> Print("VRTesting/VRTest/" + sampleName[i] + "_" + schannel[k] + "_" + cutName[j]);
     }
    }
   }
  } 

  gSystem->Exec("cp  ./index.php ./VRTesting/VRTest/; done");
  gSystem->Exec("for dir in $(find ./VRTesting/VRTest/ -type d); do cp -n ./index.php $dir/; done"); 
*/ 
