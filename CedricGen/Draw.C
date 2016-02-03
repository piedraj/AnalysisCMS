#include "TString.h"

void Draw() {

  TFile*      f = new TFile ("output.root","read");

  // Draw the m2l related histograms

  TH1D*       h_GEN_m2l         =   (TH1D*)f   ->   Get("h_GEN_m2l");
  TH1D*       h_RECO_m2l        =   (TH1D*)f   ->   Get("h_RECO_m2l");
  TH1D*       h_GEN_m2l_DeltaR  =   (TH1D*)f   ->   Get("h_GEN_m2l_DeltaR");

  TCanvas*    canvasm2l = new TCanvas("Mass of the two Z generated leptons","Mass of the two Z generated leptons");


  h_GEN_m2l->Draw();
  h_GEN_m2l->SetLineWidth(2);  
  h_GEN_m2l->SetTitle("Mass of the two Z leptons in different scenarios");

  h_GEN_m2l_DeltaR->Draw("same");
  h_GEN_m2l_DeltaR->SetLineWidth(2);
  h_GEN_m2l_DeltaR->SetLineColor(2);

  h_RECO_m2l->Draw("same");
  h_RECO_m2l->SetLineWidth(2);
  h_RECO_m2l->SetLineColor(8);

  //  TCanvas*   canvasm2lbis = new TCvans("Invariant mass of the two Z and W generated leptons", "Invariant mass of the two Z and W generated leptons");

  // Draw the pt related histograms

  TH1D*        h_GEN_zl1pt      =   (TH1D*)f   ->   Get("h_GEN_zl1pt");
  TH1D*        h_GEN_zl2pt      =   (TH1D*)f   ->   Get("h_GEN_zl2pt");
  TH1D*        h_GEN_wlpt       =   (TH1D*)f   ->   Get("h_GEN_wlpt");

  TCanvas*     canvaspt = new TCanvas("Pt of the 3 generated leptons", "Pt of the 3 generated leptons");

  h_GEN_zl2pt->Draw();
  h_GEN_zl2pt->SetTitle("Pf of the 3 generated leptons");
  h_GEN_zl2pt->SetLineWidth(2);

  h_GEN_zl1pt->Draw("same");
  h_GEN_zl1pt->SetLineWidth(2);  
  h_GEN_zl1pt->SetLineColor(2);    

  h_GEN_wlpt->Draw("same");
  h_GEN_wlpt->SetLineWidth(2);  
  h_GEN_wlpt->SetLineColor(8);

  
  // Draw the met related histograms
  
  TH1D*        h_GEN_met        =   (TH1D*)f   ->   Get("h_GEN_met");
  TH1D*        h_RECO_met       =   (TH1D*)f   ->   Get("h_RECO_met");
  TH1D*        h_met_comparison =   (TH1D*)f   ->   Get("h_met_comparison");

  TCanvas*     canvasmet = new TCanvas("Met of different cases", "Met of different cases");

  h_GEN_met->Draw();
  h_GEN_met->SetTitle("Met of different cases");
  h_GEN_met->SetLineWidth(2);

  h_RECO_met->Draw("same");
  h_RECO_met->SetLineWidth(2);  
  h_RECO_met->SetLineColor(2);    

  TCanvas*    canvasmetcomparison = new TCanvas("Met comparison", "Met comparison");

  h_met_comparison->Draw();
  h_met_comparison->SetLineWidth(2);  

  // Draw the DeltaR related histograms

  TH1D*        h_GEN_wlzl1deltar       =   (TH1D*)f   ->   Get("h_GEN_wlzl1deltar");
  TH1D*        h_GEN_wlzl2deltar       =   (TH1D*)f   ->   Get("h_GEN_wlzl2deltar");
  TH1D*        h_GEN_zl1zl2deltar      =   (TH1D*)f   ->   Get("h_GEN_zl1zl2deltar");

  TCanvas*     canvasdeltar = new TCanvas("Delta R between leptons", "Delta R between leptons");

  h_GEN_wlzl1deltar -> Draw();
  h_GEN_wlzl1deltar -> SetTitle("Delta R between leptons");
  h_GEN_wlzl1deltar -> SetLineWidth(2);

  h_GEN_wlzl2deltar -> Draw("same");
  h_GEN_wlzl2deltar -> SetLineWidth(2);  
  h_GEN_wlzl2deltar -> SetLineColor(2);    

  h_GEN_zl1zl2deltar -> Draw("same");
  h_GEN_zl1zl2deltar -> SetLineWidth(2);  
  h_GEN_zl1zl2deltar -> SetLineColor(8);

  
}








