const Double_t muonjetarray[] = {10, 15, 20, 25, 30, 35, 45};
const Double_t elejetarray [] = {10, 15, 20, 25, 30, 35, 45};

TFile* data;
TFile* wjets;
TFile* zjets;

bool Wsubtraction = true;
bool Zsubtraction = true;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l getFakeRate.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakeRate(bool draw    = true,
		 bool savepng = true)
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root",  "read");
  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root", "read");
  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root", "read");

  float elejetet;
  float muonjetet;

  int njetet = (draw) ? 1 : 8;

  for (int i=0; i<njetet; i++) {

    if (draw) {
      elejetet  = elejetarray [5];
      muonjetet = muonjetarray[3];
    } else {
      elejetet  = elejetarray [i];
      muonjetet = muonjetarray[i];
    }


    // Electron fake rate
    //--------------------------------------------------------------------------
    TString elesuffix = Form("_%.0fGev", elejetet);

    TH1D* h_Ele_loose_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_FR_pt     = (TH1D*) h_Ele_tight_pt_bin -> Clone("h_Ele_FR_pt");
    TH1D* h_Ele_FR_pt_EWK = (TH1D*) h_Ele_tight_pt_bin -> Clone("h_Ele_FR_pt_EWK");
      
    if (draw) {

      TCanvas* Ele_pt = new TCanvas("Electron FR pt", "Electron FR pt", 450, 550);
  
      Ele_pt -> SetGridx(1);
      Ele_pt -> SetGridy(1);

      h_Ele_FR_pt -> Divide(h_Ele_tight_pt_bin, h_Ele_loose_pt_bin);

      h_Ele_FR_pt -> Draw();

      h_Ele_FR_pt -> SetLineColor(4);
      h_Ele_FR_pt -> SetAxisRange(0, 1, "Y");

      h_Ele_FR_pt -> SetTitle("Electron fake rate");
      h_Ele_FR_pt -> SetXTitle("pt");
      h_Ele_FR_pt -> SetYTitle("FR");

      if (Zsubtraction) h_Ele_loose_pt_bin -> Add(h_Ele_loose_pt_bin_zjets, -1);
      if (Wsubtraction) h_Ele_loose_pt_bin -> Add(h_Ele_loose_pt_bin_wjets, -1);

      if (Zsubtraction) h_Ele_tight_pt_bin -> Add(h_Ele_tight_pt_bin_zjets, -1);
      if (Wsubtraction) h_Ele_tight_pt_bin -> Add(h_Ele_tight_pt_bin_wjets, -1);

      h_Ele_FR_pt_EWK -> Divide(h_Ele_tight_pt_bin, h_Ele_loose_pt_bin);
      h_Ele_FR_pt_EWK -> Draw("same");
    
      h_Ele_FR_pt_EWK -> SetLineColor(2);    
    
      if (savepng) Ele_pt -> SaveAs(Form("Ele_FR_pt_%.0fGev.png", elejetet));
    
    }

    // Electron eta    

    TH1D* h_Ele_loose_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_FR_eta     = (TH1D*) h_Ele_tight_eta_bin -> Clone("h_Ele_FR_eta");
    TH1D* h_Ele_FR_eta_EWK = (TH1D*) h_Ele_tight_eta_bin -> Clone("h_Ele_FR_eta_EWK");

    if (draw) {

      TCanvas* Ele_eta = new TCanvas("Electron FR eta", "Electron FR eta", 450, 550);

      Ele_eta -> SetGridx(1);
      Ele_eta -> SetGridy(1);

      h_Ele_FR_eta -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin);

      h_Ele_FR_eta -> Draw();

      h_Ele_FR_eta -> SetLineColor(4);
      h_Ele_FR_eta -> SetAxisRange(0,1,"Y");

      h_Ele_FR_eta -> SetTitle("Electron fake rate");
      h_Ele_FR_eta -> SetXTitle("eta");
      h_Ele_FR_eta -> SetYTitle("FR");

      if (Zsubtraction) h_Ele_loose_eta_bin -> Add(h_Ele_loose_eta_bin_zjets, -1);
      if (Wsubtraction) h_Ele_loose_eta_bin -> Add(h_Ele_loose_eta_bin_wjets, -1);

      if (Zsubtraction) h_Ele_tight_eta_bin -> Add(h_Ele_tight_eta_bin_zjets, -1);
      if (Wsubtraction) h_Ele_tight_eta_bin -> Add(h_Ele_tight_eta_bin_wjets, -1);

      h_Ele_FR_eta_EWK -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin);
      h_Ele_FR_eta_EWK -> Draw("same");

      h_Ele_FR_eta_EWK -> SetLineColor(2);    

      if (savepng) Ele_eta -> SaveAs(Form("Ele_FR_eta_%.0fGev.png", elejetet));

    }

    // Electron FR TH2D histograms

    TFile *EleFR = new TFile(Form("EleFR_Run2016B_25ns_jet%0.f_21Jun.root", elejetet),"recreate");

    TH2D* h_Ele_loose_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* h_Ele_loose_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* h_Ele_loose_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* h_Ele_FR_pT_eta         = (TH2D*) h_Ele_tight_pt_eta_bin -> Clone("h_Ele_FR_pT_eta");
    TH2D* h_Ele_FR_pT_eta_EWKcorr = (TH2D*) h_Ele_tight_pt_eta_bin -> Clone("h_Ele_FR_pT_eta_EWKcorr");

    h_Ele_FR_pT_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin);
    h_Ele_FR_pT_eta -> Write("FR_pT_eta");

    if (Zsubtraction) h_Ele_loose_pt_eta_bin -> Add(h_Ele_loose_pt_eta_bin_zjets, -1);
    if (Wsubtraction) h_Ele_loose_pt_eta_bin -> Add(h_Ele_loose_pt_eta_bin_wjets, -1);

    if (Zsubtraction) h_Ele_tight_pt_eta_bin -> Add(h_Ele_tight_pt_eta_bin_zjets, -1);
    if (Wsubtraction) h_Ele_tight_pt_eta_bin -> Add(h_Ele_tight_pt_eta_bin_wjets, -1);

    h_Ele_FR_pT_eta_EWKcorr -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin);
    h_Ele_FR_pT_eta_EWKcorr -> Write("FR_pT_eta_EWKcorr");

    EleFR -> Close();

    // ===================================================================================================
    // Electron prompt rate
    // ===================================================================================================

    if (draw) {

      // Electron PR pt
      TCanvas* Ele_PR_pt = new TCanvas("Electron PR pt", "Electron PR pt", 450, 550);
    
      TH1D* h_Ele_loose_pt_PR = (TH1D*) zjets -> Get("h_Ele_loose_pt_PR");
      TH1D* h_Ele_tight_pt_PR = (TH1D*) zjets -> Get("h_Ele_tight_pt_PR");

      TH1D* h_Ele_PR_pt = (TH1D*) h_Ele_tight_pt_PR -> Clone("h_Ele_PR_pt");
    
      h_Ele_PR_pt -> SetTitle("Electron prompt rate");
      h_Ele_PR_pt -> SetXTitle("pt");
      h_Ele_PR_pt -> SetYTitle("PR");
      
      h_Ele_PR_pt -> Divide(h_Ele_tight_pt_PR, h_Ele_loose_pt_PR);
      h_Ele_PR_pt -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Ele_PR_pt -> Draw();
      
      if(savepng) Ele_PR_pt -> SaveAs(Form("Ele_PR_pt_%.0fGev.png", elejetet));

    }

    if (draw) {

      // Electron PR eta
      TCanvas* Ele_PR_eta = new TCanvas("Electron PR eta", "Electron PR eta", 450, 550);

      TH1D* h_Ele_loose_eta_PR = (TH1D*) zjets -> Get("h_Ele_loose_eta_PR");
      TH1D* h_Ele_tight_eta_PR = (TH1D*) zjets -> Get("h_Ele_tight_eta_PR");
    
      TH1D* h_Ele_PR_eta = (TH1D*) h_Ele_tight_eta_PR -> Clone("h_Ele_PR_eta");

      h_Ele_PR_eta -> SetTitle("Electron prompt rate");
      h_Ele_PR_eta -> SetXTitle("eta");
      h_Ele_PR_eta -> SetYTitle("PR");
      
      h_Ele_PR_eta -> Divide(h_Ele_tight_eta_PR, h_Ele_loose_eta_PR);
      h_Ele_PR_eta -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Ele_PR_eta -> Draw();

      Ele_PR_eta->Update();

      TLine* line = new TLine(1.479, Ele_PR_eta -> GetUymin(), 1.479, Ele_PR_eta -> GetUymax());
  
      line -> SetLineWidth(2);
      line -> SetLineStyle(kDotted);
      line -> Draw("same");

      if(savepng) Ele_PR_eta -> SaveAs(Form("Ele_PR_eta_%.0fGev.png", elejetet));

    }

    // Electron PR TH2D histograms
    TFile* ElePR = new TFile("ElePR_Run2016B_25ns_21Jun.root","recreate");

    TH1D* h_Ele_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_loose_pt_eta_PR");
    TH1D* h_Ele_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_tight_pt_eta_PR");

    TH2D* Ele_PR_pt_eta = (TH2D*) h_Ele_tight_pt_eta_PR -> Clone("h_Ele_signal_pt_eta_bin");

    Ele_PR_pt_eta -> Divide(h_Ele_tight_pt_eta_PR, h_Ele_loose_pt_eta_PR);
    Ele_PR_pt_eta -> Write();

    ElePR -> Close();

    // ===================================================================================================
    // Muon fake rate
    // ===================================================================================================

    // Muon pt
    TString muonsuffix = Form("_%.0fGev", muonjetet);
    
    if (draw) {

      TH1D* h_Muon_loose_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
      TH1D* h_Muon_tight_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);    

      TH1D* h_Muon_loose_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
      TH1D* h_Muon_tight_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);

      TH1D* h_Muon_loose_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
      TH1D* h_Muon_tight_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);
    
      TH1D* h_Muon_FR_pt     = (TH1D*) h_Muon_tight_pt_bin -> Clone("h_Muon_FR_pt");
      TH1D* h_Muon_FR_pt_EWK = (TH1D*) h_Muon_tight_pt_bin -> Clone("h_Muon_FR_pt_EWK");

      TCanvas* Muon_pt = new TCanvas("Muon FR pt", "Muon FR pt", 450, 550);

      Muon_pt -> SetGridx(1);
      Muon_pt -> SetGridy(1);

      h_Muon_FR_pt -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin);

      h_Muon_FR_pt -> Draw();
      
      h_Muon_FR_pt -> SetLineColor(4);
      h_Muon_FR_pt -> SetAxisRange(0,1,"Y");

      h_Muon_FR_pt -> SetTitle("Muon fake rate");
      h_Muon_FR_pt -> SetXTitle("pt");
      h_Muon_FR_pt -> SetYTitle("FR");

      if (Zsubtraction) h_Muon_loose_pt_bin -> Add(h_Muon_loose_pt_bin_zjets, -1);
      if (Wsubtraction) h_Muon_loose_pt_bin -> Add(h_Muon_loose_pt_bin_wjets, -1);

      if (Zsubtraction) h_Muon_tight_pt_bin -> Add(h_Muon_tight_pt_bin_zjets, -1);
      if (Wsubtraction) h_Muon_tight_pt_bin -> Add(h_Muon_tight_pt_bin_wjets, -1);
      
      h_Muon_FR_pt_EWK -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin);
      h_Muon_FR_pt_EWK -> Draw("same");

      h_Muon_FR_pt_EWK -> SetLineColor(2);    
    
      if (savepng) Muon_pt -> SaveAs(Form("Muon_FR_pt_%.0fGev.png", muonjetet));
    }

    // Muon eta
    
    if(draw) {

      TH1D* h_Muon_loose_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
      TH1D* h_Muon_tight_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);    

      TH1D* h_Muon_loose_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
      TH1D* h_Muon_tight_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);

      TH1D* h_Muon_loose_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
      TH1D* h_Muon_tight_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);

      TH1D* h_Muon_FR_eta     = (TH1D*) h_Muon_tight_eta_bin -> Clone("h_Muon_FR_eta");
      TH1D* h_Muon_FR_eta_EWK = (TH1D*) h_Muon_tight_eta_bin -> Clone("h_Muon_FR_eta_EWK");

      TCanvas* Muon_eta = new TCanvas("Muon FR eta", "Muon FR eta", 450, 550);

      Muon_eta -> SetGridx(1);
      Muon_eta -> SetGridy(1);

      h_Muon_FR_eta -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin);

      h_Muon_FR_eta -> Draw();

      h_Muon_FR_eta -> SetLineColor(4);
      h_Muon_FR_eta -> SetAxisRange(0,1,"Y");

      h_Muon_FR_eta -> SetTitle("Muon fake rate");
      h_Muon_FR_eta -> SetXTitle("eta");
      h_Muon_FR_eta -> SetYTitle("FR");

      if (Zsubtraction) h_Muon_loose_eta_bin -> Add(h_Muon_loose_eta_bin_zjets, -1);
      if (Wsubtraction) h_Muon_loose_eta_bin -> Add(h_Muon_loose_eta_bin_wjets, -1);

      if (Zsubtraction) h_Muon_tight_eta_bin -> Add(h_Muon_tight_eta_bin_zjets, -1);
      if (Wsubtraction) h_Muon_tight_eta_bin -> Add(h_Muon_tight_eta_bin_wjets, -1);

      h_Muon_FR_eta_EWK -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin);
      h_Muon_FR_eta_EWK -> Draw("same");
      
      h_Muon_FR_eta_EWK -> SetLineColor(2);    
      
      if(savepng) Muon_eta -> SaveAs(Form("Muon_FR_eta_%.0fGev.png", muonjetet));

    }

    // Muon FR TH2D histograms
    TFile *MuFR = new TFile(Form("MuFR_Run2016B_25ns_jet%.0f_21Jun.root", muonjetet),"recreate");

    TH2D* h_Muon_loose_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH2D* h_Muon_tight_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);    

    TH2D* h_Muon_loose_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH2D* h_Muon_tight_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);

    TH2D* h_Muon_loose_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH2D* h_Muon_tight_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);

    TH2D* h_Muon_FR_pt_eta     = (TH2D*) h_Muon_tight_pt_eta_bin -> Clone("h_Muon_FR_pt_eta");
    TH2D* h_Muon_FR_pt_eta_EWK = (TH2D*) h_Muon_tight_pt_eta_bin -> Clone("h_Muon_FR_pt_eta_EWK");

    h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin);
    h_Muon_FR_pt_eta -> Write("FR_pT_eta");

    if (Zsubtraction) h_Muon_loose_pt_eta_bin -> Add(h_Muon_loose_pt_eta_bin_zjets, -1);
    if (Wsubtraction) h_Muon_loose_pt_eta_bin -> Add(h_Muon_loose_pt_eta_bin_wjets, -1);

    if (Zsubtraction) h_Muon_tight_pt_eta_bin -> Add(h_Muon_tight_pt_eta_bin_zjets, -1);
    if (Wsubtraction) h_Muon_tight_pt_eta_bin -> Add(h_Muon_tight_pt_eta_bin_wjets, -1);

    h_Muon_FR_pt_eta_EWK -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin);
    h_Muon_FR_pt_eta_EWK -> Write("FR_pT_eta_EWKcorr");

    MuFR -> Close();

    // ===================================================================================================
    // Muon prompt rate
    // ===================================================================================================

    if (draw) {

      // Muon PR pt
      TCanvas* Muon_PR_pt = new TCanvas("Muon PR pt", "Muon PR pt", 450, 550);

      TH1D* h_Muon_loose_pt_PR = (TH1D*) zjets -> Get("h_Muon_loose_pt_PR");
      TH1D* h_Muon_tight_pt_PR = (TH1D*) zjets -> Get("h_Muon_tight_pt_PR");
      
      TH1D* h_Muon_PR_pt = (TH1D*) h_Muon_tight_pt_PR -> Clone("h_Muon_PR_pt");
    
      h_Muon_PR_pt -> SetTitle("Muon prompt rate");
      h_Muon_PR_pt -> SetXTitle("pt");
      h_Muon_PR_pt -> SetYTitle("PR");

      h_Muon_PR_pt -> Divide(h_Muon_tight_pt_PR, h_Muon_loose_pt_PR);
      h_Muon_PR_pt -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Muon_PR_pt -> Draw();

      if(savepng) Muon_PR_pt -> SaveAs(Form("Muon_PR_pt_%.0fGev.png", muonjetet));

    }


    if (draw) {

      // Muon PR eta
      TCanvas* Muon_PR_eta = new TCanvas("Muon PR eta", "Muon PR eta", 450, 550);

      TH1D* h_Muon_loose_eta_PR = (TH1D*) zjets -> Get("h_Muon_loose_eta_PR");
      TH1D* h_Muon_tight_eta_PR = (TH1D*) zjets -> Get("h_Muon_tight_eta_PR");

      TH1D* h_Muon_PR_eta = (TH1D*) h_Muon_tight_eta_PR -> Clone("h_Muon_PR_eta");

      h_Muon_PR_eta -> SetTitle("Muon prompt rate");
      h_Muon_PR_eta -> SetXTitle("eta");
      h_Muon_PR_eta -> SetYTitle("PR");

      h_Muon_PR_eta -> Divide(h_Muon_tight_eta_PR, h_Muon_loose_eta_PR);
      h_Muon_PR_eta -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Muon_PR_eta -> Draw();

      if(savepng) Muon_PR_eta -> SaveAs(Form("Muon_PR_eta_%.0fGev.png", muonjetet));

    }

    // Muon PR TH2D histograms
    TFile *MuPR = new TFile("MuPR_Run2016B_25ns_21Jun.root","recreate");

    TH1D* h_Muon_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_loose_pt_eta_PR");
    TH1D* h_Muon_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_tight_pt_eta_PR");

    TH2D* Muon_PR_pT_eta = (TH2D*) h_Muon_tight_pt_eta_PR -> Clone("h_Muon_signal_pt_eta_bin");

    Muon_PR_pT_eta -> Divide(h_Muon_tight_pt_eta_PR, h_Muon_loose_pt_eta_PR);
    Muon_PR_pT_eta -> Write();

    MuPR -> Close();
  }
}
