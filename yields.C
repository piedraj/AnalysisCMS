//root -l -q 'yields.C("MuMu")'

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

std::ofstream inFile("yields.txt",std::ios::out);

std::vector<float> sigVector;
std::vector<float> dataVector;
std::vector<float> totMC;

//float iData;
float imonoH;
const int nProcesses = 13;
TString process[nProcesses];
TH1F *gSignificance;

void yields(TString flavourChannel = "mm", 
	    TString jetChannel = ""//  0jet/
	    ){
  
  enum{iFakes, iData, iWW, iWZ, iZZ, iTop, iWg, iWJets, iZJets, iTTV, iHWW, iZg, iVVV};

  process[iFakes] = "00_Fakes";
  process[iData]  = "01_Data";
  process[iWW]    = "05_WW";
  process[iWZ]    = "02_WZTo3LNu";
  process[iZZ]    = "03_ZZ";
  process[iTop]   = "04_Top";
  process[iWg]    = "10_Wg";
  process[iWJets] = "07_WJets";
  process[iZJets] = "06_ZJets";
  process[iTTV]   = "08_TTV";
  process[iHWW]   = "09_HWW";
  process[iZg]    = "11_Zg";
  process[iVVV]   = "12_VVV";

  TFile *input[nProcesses];
  TH1D  *histo[nProcesses];

  //  Int_t iSignal = iWW;

  const int nCuts = 8;
  
  //TGraphErrors *gSignificance = new TGraphErrors();
  gSignificance = new TH1F("Significance","Significance", nCuts, 0., nCuts);
  
  //defining cut levels
  TString cutLevel[nCuts];

  cutLevel[0] = "Has2Leptons";
  cutLevel[1] = "Mll > 12GeV";
  cutLevel[2] = "PfMet > 20GeV";
  cutLevel[3] = "ZVeto";
  cutLevel[4] = "MpMet > 20GeV";
  cutLevel[5] = "DPhiVeto";
  cutLevel[6] = "Ptll > 45GeV";
  cutLevel[7] = "BVeto";

  TString cutHisto[nCuts];

  cutHisto[0] = "WW/00_Has2Leptons/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[1] = "WW/01_Mll/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[2] = "WW/02_PfMet/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[3] = "WW/03_ZVeto/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[4] = "WW/04_MpMet/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[5] = "WW/05_DPhiVeto/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[6] = "WW/06_Ptll/" + jetChannel; // + "h_nvtx_" + flavourChannel;
  cutHisto[7] = "WW/07_BVeto/" + jetChannel; // + "h_nvtx_" + flavourChannel;

  Float_t yield[nProcesses][nCuts];
  
  TString path = "rootfiles/WW/";
  
  inFile<<"\\begin{tabular}{cSSSSSSSS}"<<endl;
  cout<<"\\begin{tabular}{cSSSSSSSS}"<<endl;
  inFile<<"\\toprule"<<endl;
  cout<<"\\toprule"<<endl;
  inFile<<"Cut Level";
  cout<<"Cut Level";

  for (int q = 0; q < nProcesses; ++q){
    inFile<<" & {"<<process[q]<<"}";
    cout<<" & {"<<process[q]<<"}";
  }			
  inFile<<" \\\\ "<<endl;	
  cout<<" \\\\ "<<endl;
  inFile<<"\\midrule"<<endl;
  cout<<"\\midrule"<<endl;
  
  for(int c = 0; c < nCuts; ++c){
    
    //signal and background files and histograms
    for (int ip = 0; ip < nProcesses; ++ip){
      TString fileName = path + process[ip] + ".root";
      input[ip] = new TFile(fileName, "read");
      if (!input[ip]) {cout<<"cannot find file "<<ip<<endl; return;}
      
      input[ip] -> ls();
      input[ip] -> cd(cutHisto[c]);
      input[ip] -> ls();

      TString hname = "h_nvtx_" + flavourChannel;
      
      if (input[ip] -> GetListOfKeys() -> Contains(hname) == 0){
	cout<<"I cannot find the histogram named "<<hname<<" in the file "<<ip<<". I'll quit."<<endl;
	return;
      }
      
      histo[ip]  = (TH1D*) input[ip] -> Get(hname);
      if (!histo[ip]) {cout<<"cannot find histo "<<ip<<endl; return;}
    }
    
    //begin the table
    //if (flavourChannel == "OF" || flavourChannel == "EMu" || flavourChannel == "MuE") 
    if (flavourChannel == "em")
    if (c == 6 || c == 4) {
    	sigVector.push_back(0);
    	dataVector.push_back(0);
    	totMC.push_back(0);
    	continue;
    }

    inFile<<cutLevel[c];
    cout<<cutLevel[c];

    Float_t den = 0.;
    
    //number of events
    for(int i = 0; i < nProcesses; ++i){
      
      if (histo[i])
	yield[i][c] = /*TMath::Nint*/(histo[i] -> Integral()); 
      if (i != iData)
	den += yield[i][c];
      else dataVector.push_back(yield[i][c]);
      std::cout << std::fixed;
    
      inFile<<" & "<<setprecision(2)<<yield[i][c];
      inFile<<setprecision(2)<<"("<<yield[i][c]/yield[i][0]<<")";
      cout<<"   & "<<setprecision(2)<<yield[i][c];
      cout<<setprecision(2)<<"("<<yield[i][c]/yield[i][0]<<")";
    }

    gSignificance -> SetBinContent(c+1,yield[iWW][c] / sqrt(den));
    sigVector.push_back(yield[iWW][c] / sqrt(den));
    totMC.push_back(den);
    TString dump = cutLevel[c];
    //dump.Remove(0,8);
    //dump.Remove(dump.Length()-1,1);
    inFile<<"\\\\"<<endl;
    cout<<"\\\\"<<endl;
  }
  
  //end of the table
  inFile<<"\\bottomrule"<<endl;
  cout<<"\\bottomrule"<<endl;
  //inFile<<"Percentage"<<" & "<<DarkNoCuts<<" & "<<ZHNoCuts<<" & "<<HWWNoCuts<<" & "<<WWNoCuts<<"\\"<<endl;
  inFile<<"\\end{tabular}"<<endl;
  cout<<"\\end{tabular}"<<endl;

  //Significance Tabular
  inFile<<"\\begin{center}"<<endl;
  cout<<"\\begin{center}"<<endl;
  inFile<<"\\begin{tabular}{cSSSSSSS}"<<endl;
  cout<<"\\begin{tabular}{cSSSSSSS}"<<endl;
  inFile<<"\\toprule"<<endl;
  cout<<"\\toprule"<<endl;
  inFile<<"Cut Level";
  cout<<"Cut Level";
  inFile<<" & {Significance}";
  cout<<" & {Significance}";
  inFile<<" & {Data2015}";
  cout<<" & {Data2015}";
  inFile<<" & {Total MC}";
  cout<<" & {Total MC}";
  inFile<<" \\\\ "<<endl;	
  cout<<" \\\\ "<<endl;
  inFile<<"\\midrule"<<endl;
  cout<<"\\midrule"<<endl;
  for(int i = 0; i < nCuts; ++i){
    //if (flavourChannel == "OF" || flavourChannel == "EMu" || flavourChannel == "MuE")
    if (flavourChannel == "em")
      if (i == 6 || i == 4) continue;
    
    cout<<cutLevel[i];
    inFile<<cutLevel[i];
    cout<<" & "<<setprecision(4)<<sigVector.at(i);
    inFile<<" & "<<setprecision(4)<<sigVector.at(i);
    cout<<" & "<<setprecision(0)<<dataVector.at(i);
    inFile<<" & "<<setprecision(0)<<dataVector.at(i);
    cout<<" & "<<setprecision(0)<<totMC.at(i);
    inFile<<" & "<<setprecision(0)<<totMC.at(i);
    inFile<<"\\\\"<<endl;
    cout<<"\\\\"<<endl;
  }
  inFile<<"\\bottomrule"<<endl;
  cout<<"\\bottomrule"<<endl;
  inFile<<"\\end{tabular}"<<endl;
  cout<<"\\end{tabular}"<<endl;
  inFile<<"\\end{center}"<<endl;
  cout<<"\\end{center}"<<endl;
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
  //gSignificance -> GetYaxis() -> SetRangeUser(0.,5.);
  gSignificance -> GetXaxis() -> CenterLabels();
  gSignificance -> GetXaxis() -> LabelsOption("h");
  //gSignificance -> SetMarkerStyle(8);
  gSignificance -> SetStats(0);

  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  c1 -> cd();
  gSignificance -> Draw();
  c1 -> Print("Bveto.pdf","pdf");

  //  gSystem -> Exec("mv yields.txt distributions/" + bunch + "/" + flavourChannel + "/" + muonID);
  //gSystem -> Exec("mv Bveto.pdf  distributions/" + bunch + "/" + flavourChannel + "/" + muonID);
}
