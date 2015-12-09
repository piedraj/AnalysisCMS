#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TString.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TObjArray.h"
#include <algorithm>
#include "TCut.h"
#include "TStyle.h"

const int nProcesses = 10;
const int nCuts      = 12;

enum {iWZ, iZZ, iTop, iWW, iZjet, iWjet, iTTV, iHWW, iWg, iVVV};

void domacroJet(TString channel, Int_t loop);

void DrawTLatex(Double_t x, Double_t y, Double_t tsize, const char* text);

void DrawTLatex(Double_t x, Double_t y, Double_t tsize, const char* text)
{
  TLatex* tl = new TLatex(x, y, text);
  tl->SetNDC();
  tl->SetTextAlign(   32);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);
  tl->Draw("same");
}

void macroJet(){

  for (int aa = 0; aa < nCuts; ++aa){
    domacroJet("ee",aa);
    domacroJet("mm",aa);
  }
}

void domacroJet(TString channel, Int_t loop){

  TString process[nProcesses];
  process[iWZ]   = "02_WZ";
  process[iZZ]   = "03_ZZ";
  process[iTop]  = "04_Top";
  process[iWW]   = "05_WW";
  process[iZjet] = "06_ZJets";
  process[iWjet] = "07_WJets";
  process[iTTV]  = "08_TTV";
  process[iHWW]  = "09_HWW";
  process[iWg]   = "10_Vg";
  process[iVVV]  = "11_VVV";
 
  Color_t color[nProcesses];
  color[iWZ]     = kOrange;
  color[iZZ]     = kAzure - 2;
  color[iTop]    = kYellow + 1;
  color[iWW]     = kAzure - 9;
  color[iZjet]   = kGreen + 2;
  color[iWjet]   = kGray + 1;
  color[iTTV]    = kYellow +1;
  color[iHWW]    = kRed + 2;
  color[iWg]     = kViolet;
  color[iVVV]    = kAzure - 2;
  
  TFile* f[nProcesses];

  for (int i = 0; i < nProcesses; ++i)
    f[i] = new TFile("../rootfiles/WW/" + process[i] + ".root");
  TFile* fData = new TFile("../rootfiles/WW/01_Data.root");

  TH1D *h[nProcesses];
  char name[80];

  for (int i = 0; i < nProcesses; ++i){
    sprintf(name, "WW/%d_ZWindow%d/h_met_"+channel, 18+loop, 25+loop); 
    h[i] = (TH1D*) f[i] -> Get(name);
    h[i] -> Rebin(2);
    h[i] -> GetXaxis() -> SetRangeUser(0.,100.);
    h[i] -> SetLineWidth(2);
    h[i] -> SetFillStyle(1001);
    h[i] -> SetFillColor(color[i]);
  }
  
  TH1D* hData = (TH1D*) fData -> Get("WW/23_ZWindow30/h_met_"+channel);
  hData -> Rebin(2);
  hData -> GetXaxis() -> SetRangeUser(0.,100.);
  hData -> SetLineWidth(2);
  hData -> SetLineColor(kBlack);
  hData -> GetYaxis() -> SetRangeUser(0.1, 2 * hData -> GetMaximum());
  hData -> GetXaxis() -> SetTitle("pfMET [GeV]");
  hData -> GetYaxis() -> SetTitle("entries / 2GeV");
  hData -> GetXaxis() -> SetTitleOffset(1.4);
  hData -> GetXaxis() -> SetLabelOffset(4.0);
  hData -> GetYaxis() -> SetTitleOffset(1.8);
  hData -> SetTitle("DY " + channel);
  
  char title[80];
  sprintf(title,"DY " + channel + " - jetp_{T} < %d", 25 + loop);
  
  THStack* hstack = new THStack(title,title);
  
  float max = 10000.;
  //  if (channel == "mm") max = 1400.;

  hstack -> SetMinimum(0.1);
  hstack -> SetMaximum(max);
  for (int w = 0; w < nProcesses-1; ++w)
    hstack -> Add(h[w]);
  
  TCanvas *c = new TCanvas("c","c",600,600);
  c -> cd();
  
  TPad* pad1 = new TPad("pad1", "pad1", 0., 0.2, 1.0, 1.0);
  TPad* pad2 = new TPad("pad2", "pad2", 0., 0.0, 1.0, 0.3);
  pad1->SetLeftMargin(0.16);
  pad1->SetBottomMargin(0.12);
  pad1->Draw();
  pad1->cd();
  
  pad1->SetLogy();

  hstack -> Draw("hist"); 
  hstack -> GetXaxis() -> SetRangeUser(0.,100.);
  hData  -> Draw("ep,same");
  hstack -> Draw("hist,same"); 
  hData  -> Draw("ep,same");
  
  TLegend* leg = new TLegend(0.6,0.6,0.89,0.8);
  leg -> SetNColumns(2);
  leg -> AddEntry(hData,"Data - jetpt < 30GeV","lep");
  sprintf(name,"jetpt < %d GeV", 25+loop);
  for (int i = 0; i < nProcesses; ++i)
    leg -> AddEntry(h[i],process[i],"f");
  leg -> SetLineColor(kWhite);
  leg -> Draw();
  
  DrawTLatex(0.89, 0.88, 0.03, "ptll > 45GeV \n, |mll - mZ| < 15GeV");
  DrawTLatex(0.89, 0.84, 0.03, "mpmet > 20GeV\n, pfmet > 20GeV");

  c -> Update();
  c -> cd();
  
  pad2->SetLeftMargin(0.16);
  pad2->SetBottomMargin(0.40);
  pad2->Draw();
  pad2->cd();
  pad2->SetGridy(1);
  
  TH1D *ratio = new TH1D("ratio","",100,0.,200.);
  for (int b = 0; b < 100; ++b)
    if (((TH1*)(hstack->GetStack()->Last()))->GetBinContent(b) != 0)
      ratio -> SetBinContent(b,hData->GetBinContent(b) / ((TH1*)(hstack->GetStack()->Last()))->GetBinContent(b));
    else ratio -> SetBinContent(b,0.);
  ratio -> SetStats(0);
  ratio -> GetXaxis() -> SetLabelSize(0.12);
  ratio -> GetYaxis() -> SetLabelSize(0.12);
  ratio -> GetYaxis() -> SetRangeUser(0.,2.);
  ratio -> GetXaxis() -> SetRangeUser(0.,100.);
  ratio -> GetXaxis() -> SetTitle("pfMET [GeV]");
  ratio -> GetXaxis() -> SetTitleSize(0.12);
  ratio -> GetYaxis() -> SetNdivisions(306);
  
  ratio->Draw("ep");
  
  sprintf(name,"DYptNuisanceBis"+channel+"_%d.pdf",loop);
  c -> Print(name);
  sprintf(name,"DYptNuisanceBis"+channel+"_%d.png",loop);
  c -> Print(name);
  
  delete pad1;
  delete pad2;
  delete c;
}

