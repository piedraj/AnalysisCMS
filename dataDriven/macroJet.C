#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TString.h"
#include "TLegend.h"
#include "TObjArray.h"
#include <algorithm>
#include "TCut.h"
#include "TStyle.h"

const int nProcesses = 10;

void domacroJet(TString channel);

void macroJet(){
  domacroJet("ee");
  domacroJet("mm");
}

void domacroJet(TString channel){

  Color_t color[nProcesses];
  
  color[0] = kRed;
  color[1] = kOrange;
  color[2] = kYellow;
  color[3] = kGreen;
  color[4] = kAzure;
  color[5] = kBlue;
  color[6] = kViolet;
  color[7] = kPink;
  color[8] = kOrange+3;
  color[9] = kGray;
  
  TFile* f     = new TFile("../rootfiles/25ns/06_ZJets.root");
  TFile* fData = new TFile("../rootfiles/25ns/01_Data.root");

  TH1D *h[nProcesses];
  char name[80];

  for (int i = 0; i < 10; ++i){
    sprintf(name, "WW/%d_ZWindow%d/h_met_"+channel, 18+i, 25+i); 
    h[i] = (TH1D*) f -> Get(name);
    h[i] -> Rebin(2);
    h[i] -> GetXaxis() -> SetRangeUser(0.,80.);
    h[i] -> SetLineWidth(2);
    h[i] -> SetLineColor(color[i]);
  }

  TH1D* hData = (TH1D*) fData -> Get("WW/23_ZWindow30/h_met_"+channel);
  hData -> Rebin(2);
  hData -> GetXaxis() -> SetRangeUser(0.,80.);
  hData -> SetLineWidth(2);
  hData -> SetLineColor(kBlack);
  hData -> GetYaxis() -> SetRangeUser(0., 2 * hData -> GetMaximum());
  hData -> GetXaxis() -> SetTitle("pfMET [GeV]");
  hData -> GetYaxis() -> SetTitle("entries / 2GeV");
  hData -> GetXaxis() -> SetTitleOffset(1.4);
  hData -> GetYaxis() -> SetTitleOffset(1.8);
  hData -> SetTitle("DY " + channel);

  TCanvas* c = new TCanvas("c","c",600,600);
  c -> cd();

  TPad* pad1 = new TPad("pad1", "pad1", 0., 0., 1.0, 1.0);
  pad1->SetLeftMargin(0.16);
  pad1->SetBottomMargin(0.14);
  pad1->Draw();
  pad1->cd();

  hData -> Draw("ep");
  for (int j = 0; j < nProcesses; ++j){
    if ((25 + j) % 2 != 0 && 25 + j != 30) continue;
    h[j] -> Draw("hist,same");
  }
  
  TLegend* leg = new TLegend(0.5,0.6,0.89,0.89);
  leg -> SetHeader("ptll > 45GeV, |mll - mZ| < 15GeV");
  leg -> AddEntry(hData,"Data - jetpt < 30GeV","lep");
  for (int i = 0; i < nProcesses; ++i){
    if ((25 + i) % 2 != 0 && 25 + i != 30) continue;
    sprintf(name,"jetpt < %d GeV", 25+nProcesses-i);
    leg -> AddEntry(h[nProcesses - i],name,"l");
  }
  leg -> SetLineColor(kWhite);
  leg -> Draw();

  
  c -> Print("DYptNuisanceBis"+channel+".pdf");
  c -> Print("DYptNuisanceBis"+channel+".png");
}
