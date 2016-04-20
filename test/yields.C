/*
root -l -b -q 'yields.C("em","0jet/")'
*/

#include "TROOT.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TString.h"
#include "TMath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <vector>

//#include "include/CutsMonoH.h"
//#include "include/CutsTop.h"
//#include "include/CutsTTDM.h"
#include "include/CutsWW.h"
//#include "include/CutsWZ.h"

std::ofstream inFile("outyields.txt",std::ios::out);

std::vector<float> sigVector;
std::vector<float> dataVector;
std::vector<float> totMC;

TH1F *gSignificance;

void yields(TString flavourChannel = "em", 
	    TString jetChannel     = "0jet/",//"0jet/", //"",
	    bool    percentage     = false,
	    TString analysis       = "WW" 
	    ){
  
  //  enum{iFakes, iData, iWZ, iZZ, iTTTo2L2Nu, iST, iWW, iZJets, iWJets, iTTV, iHWW, iWg, iZg, iVVV, iHZ, nProcesses};
  enum{iData, iTTTo2L2Nu, iST, iWW, iZJets, nProcesses};

  TString process[nProcesses];
  //  process[iFakes]     = "00_Fakes";
  process[iData]      = "01_Data";
  //  process[iWZ]        = "02_WZTo3LNu";
  //  process[iZZ]        = "03_ZZ";
  process[iTTTo2L2Nu] = "04_TTTo2L2Nu";
  process[iST]        = "05_ST";
  process[iWW]        = "WWTo2L2Nu";
  process[iZJets]     = "07_ZJets";
  //  process[iWJets]     = "08_WJets";
  //  process[iTTV]       = "09_TTV";
  //  process[iHWW]       = "10_HWW";
  //  process[iWg]        = "11_Wg";
  //  process[iZg]        = "12_Zg";
  //  process[iVVV]       = "13_VVV";
  //  process[iHZ]        = "14_HZ";

  TFile *input[nProcesses];
  TH1D  *histo[nProcesses];

  const int nCuts = ncut;
  
  gSignificance = new TH1F("Significance","Significance", nCuts, 0., nCuts);

   
  //defining cut levels - from CutsWW.h
  TString cutLevel[nCuts];
  TString cutHisto[nCuts];

  for (Int_t iCut = 0; iCut < ncut; ++iCut){
    cutHisto[iCut] = scut[iCut] + "/" + jetChannel + "h_nvtx_" + flavourChannel;
    cutLevel[iCut] = scut[iCut]; 
    cutLevel[iCut].Remove(0,6);
  } 

  Float_t yield[nProcesses][nCuts];
  
  TString path = "rootfiles/nominal" + analysis + "/";
  
  inFile<<"\\begin{tabular}{cSSSSSSSSSSSSSSSSSSSS}"<<endl;
  cout  <<"\\begin{tabular}{cSSSSSSSSSSSSSSSSSSSS}"<<endl;
  inFile<<"\\toprule"<<endl;
  cout  <<"\\toprule"<<endl;
  inFile<<"Cut Level";
  cout  <<"Cut Level";

  for (int q = 0; q < nProcesses; ++q){
    TString title = process[q];
    TString titleShort = title.Remove(0,3);
    inFile<<" & {"<<titleShort<<"}";
    cout  <<" & {"<<titleShort<<"}";
  }			
  inFile<<" \\\\ "<<endl;	
  cout  <<" \\\\ "<<endl;
  inFile<<"\\midrule"<<endl;
  cout  <<"\\midrule"<<endl;
  
  for(int c = 0; c < nCuts; ++c){
    
    //signal and background files and histograms
    for (int ip = 0; ip < nProcesses; ++ip){
      TString fileName = path + process[ip] + ".root";
      input[ip] = new TFile(fileName, "read");
      if (!input[ip]) {cout  <<"cannot find file "<<ip<<endl; return;}
      
      histo[ip]  = (TH1D*)input[ip] -> Get(cutHisto[c]);
      if (!histo[ip]) {cout  <<"cannot find histo "<<ip<<"in file "<<input[ip]<<endl; return;}

    }
    
    inFile<<cutLevel[c];
    cout  <<cutLevel[c];

    Float_t den = 0.;
    
    //number of events
    for(int i = 0; i < nProcesses; ++i){
      
      if (histo[i]){
	yield[i][c] = (histo[i] -> Integral()); 
      }

      if (i != iData)
	den += yield[i][c];
      else dataVector.push_back(yield[i][c]);
      std::cout << std::fixed;
    
      inFile<<" & "<<setprecision(2)<<yield[i][c];
      cout  <<" & "<<setprecision(2)<<yield[i][c];
      if (percentage){
	inFile<<setprecision(2)<<"("<<yield[i][c]/yield[i][0]<<")";
	cout  <<setprecision(2)<<"("<<yield[i][c]/yield[i][0]<<")";
      }      
    }

    if (den != 0){
      gSignificance -> SetBinContent(c+1,yield[iWW][c] / sqrt(den));
      sigVector.push_back(yield[iWW][c] / sqrt(den));
    }
    else{
      gSignificance -> SetBinContent(c+1, 0.);
      sigVector.push_back(0.);
    }

    totMC.push_back(den);
    TString dump = cutLevel[c];
    inFile<<"\\\\"<<endl;
    cout  <<"\\\\"<<endl;
  }
  
  //end of the table
  inFile<<"\\bottomrule"<<endl;
  cout  <<"\\bottomrule"<<endl;
  inFile<<"\\end{tabular}"<<endl;
  cout  <<"\\end{tabular}"<<endl;

  //Significance Tabular
  inFile<<"\\begin{center}"<<endl;
  cout  <<"\\begin{center}"<<endl;
  inFile<<"\\begin{tabular}{cSSSSSSS}"<<endl;
  cout  <<"\\begin{tabular}{cSSSSSSS}"<<endl;
  inFile<<"\\toprule"<<endl;
  cout  <<"\\toprule"<<endl;
  inFile<<"Cut Level";
  cout  <<"Cut Level";
  inFile<<" & {Significance}";
  cout  <<" & {Significance}";
  inFile<<" & {Data2015}";
  cout  <<" & {Data2015}";
  inFile<<" & {Total MC}";
  cout  <<" & {Total MC}";
  inFile<<" \\\\ "<<endl;	
  cout  <<" \\\\ "<<endl;
  inFile<<"\\midrule"<<endl;
  cout  <<"\\midrule"<<endl;

  for(int i = 0; i < nCuts; ++i){
    cout  <<cutLevel[i];
    inFile<<cutLevel[i];
    cout  <<" & "<<setprecision(4)<<sigVector.at(i);
    inFile<<" & "<<setprecision(4)<<sigVector.at(i);
    cout  <<" & "<<setprecision(0)<<dataVector.at(i);
    inFile<<" & "<<setprecision(0)<<dataVector.at(i);
    cout  <<" & "<<setprecision(0)<<totMC.at(i);
    inFile<<" & "<<setprecision(0)<<totMC.at(i);
    inFile<<"\\\\"<<endl;
    cout  <<"\\\\"<<endl;
  }

  inFile<<"\\bottomrule"<<endl;
  cout  <<"\\bottomrule"<<endl;
  inFile<<"\\end{tabular}"<<endl;
  cout  <<"\\end{tabular}"<<endl;
  inFile<<"\\end{center}"<<endl;
  cout  <<"\\end{center}"<<endl;
  inFile.close();


  //Significance Graph Cosmetics
  gSignificance -> GetXaxis() -> SetNdivisions(-414);
  for (int c = 0; c < nCuts; ++c)
    gSignificance -> GetXaxis() -> SetBinLabel(c+1, cutLevel[c]);
  gSignificance -> GetXaxis() -> SetLabelSize(0.04);
  gSignificance -> GetXaxis() -> SetTitle("Cut Level");
  gSignificance -> GetXaxis() -> SetLabelSize(0.04);
  gSignificance -> GetYaxis() -> SetLabelSize(0.04);
  gSignificance -> GetYaxis() -> SetTitle("Significance");
  gSignificance -> GetXaxis() -> SetRangeUser(0,nCuts);
  gSignificance -> GetXaxis() -> CenterLabels();
  gSignificance -> GetXaxis() -> LabelsOption("h");
  gSignificance -> SetStats(0);

  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  c1 -> cd();
  gSignificance -> Draw();
  c1 -> Print("Significance.pdf","pdf");
}
