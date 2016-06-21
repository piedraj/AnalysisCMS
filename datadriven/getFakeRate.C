void getFakeRate()
{
  
    gInterpreter->ExecuteMacro("../test/PaperStyle.C");
    
    float elejetet  = 35.;
    float muonjetet = 20.;

    TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
    TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
    TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

    // ===================================================================================================
    // Electron Fake Rate
    // ===================================================================================================

    // Electron pt

    TString elesuffix  = Form("_%.0fGev", elejetet);
    /*
    TH1D* h_Ele_loose_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_FR_pt = (TH1D*) h_Ele_tight_pt_bin -> Clone();
    TH1D* h_Ele_FR_pt_EWK = (TH1D*) h_Ele_tight_pt_bin -> Clone();

    TCanvas* Ele_pt = new TCanvas("Ele pt", "Ele pt", 450, 550);

    Ele_pt -> SetGridx(1);
    Ele_pt -> SetGridy(1);

    h_Ele_FR_pt -> Divide(h_Ele_tight_pt_bin , h_Ele_loose_pt_bin , 1., 1., "");

    h_Ele_FR_pt -> Draw();

    h_Ele_FR_pt -> SetLineColor(4);
    h_Ele_FR_pt -> SetAxisRange(0,1,"Y");

    h_Ele_FR_pt -> SetTitle("Ele Fake Rate with and without EWK correction");
    h_Ele_FR_pt -> SetXTitle("Ele pt");
    h_Ele_FR_pt -> SetYTitle("Ele FR");

    h_Ele_loose_pt_bin -> Add(h_Ele_loose_pt_bin_zjets, -1);
    h_Ele_loose_pt_bin -> Add(h_Ele_loose_pt_bin_wjets, -1);

    h_Ele_tight_pt_bin -> Add(h_Ele_tight_pt_bin_zjets, -1);
    h_Ele_tight_pt_bin -> Add(h_Ele_tight_pt_bin_wjets, -1);

    h_Ele_FR_pt_EWK -> Divide(h_Ele_tight_pt_bin, h_Ele_loose_pt_bin, 1., 1., " ");
    h_Ele_FR_pt_EWK -> Draw("same");

    h_Ele_FR_pt_EWK -> SetLineColor(2);    
    */
    // Electron eta    
    /*
    TH1D* h_Ele_loose_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_FR_eta = (TH1D*) h_Ele_tight_eta_bin -> Clone();
    TH1D* h_Ele_FR_eta_EWK = (TH1D*) h_Ele_tight_eta_bin -> Clone();

    TCanvas* Ele_eta = new TCanvas("Ele eta", "Ele eta", 450, 550);

    Ele_eta -> SetGridx(1);
    Ele_eta -> SetGridy(1);

    h_Ele_FR_eta -> Divide(h_Ele_tight_eta_bin , h_Ele_loose_eta_bin , 1., 1., "");

    h_Ele_FR_eta -> Draw();

    h_Ele_FR_eta -> SetLineColor(4);
    h_Ele_FR_eta -> SetAxisRange(0,1,"Y");

    h_Ele_FR_eta -> SetTitle("Ele Fake Rate with and without EWK correction");
    h_Ele_FR_eta -> SetXTitle("Ele eta");
    h_Ele_FR_eta -> SetYTitle("Ele FR");

    h_Ele_loose_eta_bin -> Add(h_Ele_loose_eta_bin_zjets, -1);
    h_Ele_loose_eta_bin -> Add(h_Ele_loose_eta_bin_wjets, -1);

    h_Ele_tight_eta_bin -> Add(h_Ele_tight_eta_bin_zjets, -1);
    h_Ele_tight_eta_bin -> Add(h_Ele_tight_eta_bin_wjets, -1);

    h_Ele_FR_eta_EWK -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin, 1., 1., " ");
    h_Ele_FR_eta_EWK -> Draw("same");

    h_Ele_FR_eta_EWK -> SetLineColor(2);    
    */
    // Electron FR TH2D histograms
    TFile *EleFR = new TFile("EleFR_Run2016B_25ns_jet35_21Jun.root","recreate");

    TH1D* h_Ele_loose_pt_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* FR_pT_eta         = (TH2D*) h_Ele_tight_pt_eta_bin -> Clone();
    TH2D* FR_pT_eta_EWKcorr = (TH2D*) h_Ele_tight_pt_eta_bin -> Clone();

    FR_pT_eta -> Divide(h_Ele_tight_pt_eta_bin , h_Ele_loose_pt_eta_bin , 1., 1., "");
    FR_pT_eta -> Write("FR_pT_eta");

    h_Ele_loose_pt_eta_bin -> Add(h_Ele_loose_pt_eta_bin_zjets, -1);
    h_Ele_loose_pt_eta_bin -> Add(h_Ele_loose_pt_eta_bin_wjets, -1);

    h_Ele_tight_pt_eta_bin -> Add(h_Ele_tight_pt_eta_bin_zjets, -1);
    h_Ele_tight_pt_eta_bin -> Add(h_Ele_tight_pt_eta_bin_wjets, -1);

    FR_pT_eta_EWKcorr -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., " ");
    FR_pT_eta_EWKcorr -> Write("FR_pT_eta_EWKcorr");

    EleFR -> Close();

    // ===================================================================================================
    // Electron Prompt Rate
    // ===================================================================================================

    // Electron PR TH2D histograms
    TFile *ElePR = new TFile("ElePR_Run2016B_25ns_jet35_21Jun.root","recreate");

    TH1D* h_Ele_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_loose_pt_eta_PR");
    TH1D* h_Ele_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_tight_pt_eta_PR");

    TH2D* Ele_PR_pT_eta = (TH2D*) h_Ele_tight_pt_eta_PR -> Clone();

    Ele_PR_pT_eta -> Divide(h_Ele_tight_pt_eta_PR, h_Ele_loose_pt_eta_PR, 1., 1., "");
    Ele_PR_pT_eta -> Write("PR_pT_eta");
    
    ElePR -> Close();

    // ===================================================================================================
    // Muon Fake Rate
    // ===================================================================================================

    // Muon pt
    TString muonsuffix = Form("_%.0fGev", muonjetet);
    /*
    TH1D* h_Muon_loose_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);    

    TH1D* h_Muon_loose_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);

    TH1D* h_Muon_loose_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_loose_pt_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_tight_pt_bin" + muonsuffix);

    TH1D* h_Muon_FR_pt = (TH1D*) h_Muon_tight_pt_bin -> Clone();
    TH1D* h_Muon_FR_pt_EWK = (TH1D*) h_Muon_tight_pt_bin -> Clone();

    TCanvas* Muon_pt = new TCanvas("Muon pt", "Muon pt", 450, 550);

    Muon_pt -> SetGridx(1);
    Muon_pt -> SetGridy(1);

    h_Muon_FR_pt -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin, 1., 1., "");

    h_Muon_FR_pt -> Draw();

    h_Muon_FR_pt -> SetLineColor(4);
    h_Muon_FR_pt -> SetAxisRange(0,1,"Y");

    h_Muon_FR_pt -> SetTitle("Muon Fake Rate with and without EWK correction");
    h_Muon_FR_pt -> SetXTitle("Muon pt");
    h_Muon_FR_pt -> SetYTitle("Muon FR");

    h_Muon_loose_pt_bin -> Add(h_Muon_loose_pt_bin_zjets, -1);
    h_Muon_loose_pt_bin -> Add(h_Muon_loose_pt_bin_wjets, -1);

    h_Muon_tight_pt_bin -> Add(h_Muon_tight_pt_bin_zjets, -1);
    h_Muon_tight_pt_bin -> Add(h_Muon_tight_pt_bin_wjets, -1);

    h_Muon_FR_pt_EWK -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin, 1., 1., " ");
    h_Muon_FR_pt_EWK -> Draw("same");

    h_Muon_FR_pt_EWK -> SetLineColor(2);    
    */
    // Muon eta
    /*
    TH1D* h_Muon_loose_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);    

    TH1D* h_Muon_loose_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);

    TH1D* h_Muon_loose_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_loose_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_tight_eta_bin" + muonsuffix);

    TH1D* h_Muon_FR_eta = (TH1D*) h_Muon_tight_eta_bin -> Clone();
    TH1D* h_Muon_FR_eta_EWK = (TH1D*) h_Muon_tight_eta_bin -> Clone();

    TCanvas* Muon_eta = new TCanvas("Muon eta", "Muon eta", 450, 550);

    Muon_eta -> SetGridx(1);
    Muon_eta -> SetGridy(1);

    h_Muon_FR_eta -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin, 1., 1., "");

    h_Muon_FR_eta -> Draw();

    h_Muon_FR_eta -> SetLineColor(4);
    h_Muon_FR_eta -> SetAxisRange(0,1,"Y");

    h_Muon_FR_eta -> SetTitle("Muon Fake Rate with and without EWK correction");
    h_Muon_FR_eta -> SetXTitle("Muon eta");
    h_Muon_FR_eta -> SetYTitle("Muon FR");

    h_Muon_loose_eta_bin -> Add(h_Muon_loose_eta_bin_zjets, -1);
    h_Muon_loose_eta_bin -> Add(h_Muon_loose_eta_bin_wjets, -1);

    h_Muon_tight_eta_bin -> Add(h_Muon_tight_eta_bin_zjets, -1);
    h_Muon_tight_eta_bin -> Add(h_Muon_tight_eta_bin_wjets, -1);

    h_Muon_FR_eta_EWK -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin, 1., 1., " ");
    h_Muon_FR_eta_EWK -> Draw("same");

    h_Muon_FR_eta_EWK -> SetLineColor(2);    
    */

    // Muon FR TH2D histograms
    TFile *MuFR = new TFile("MuFR_Run2016B_25ns_jet20_21Jun.root","recreate");

    TH1D* h_Muon_loose_pt_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);    

    TH1D* h_Muon_loose_pt_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);

    TH1D* h_Muon_loose_pt_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin" + muonsuffix);
    TH1D* h_Muon_tight_pt_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin" + muonsuffix);

    TH2D* h_Muon_FR_pt_eta     = (TH2D*) h_Muon_tight_pt_eta_bin -> Clone();
    TH2D* h_Muon_FR_pt_eta_EWK = (TH2D*) h_Muon_tight_pt_eta_bin -> Clone();

    h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");
    h_Muon_FR_pt_eta -> Write("FR_pT_eta");

    h_Muon_loose_pt_eta_bin -> Add(h_Muon_loose_pt_eta_bin_zjets, -1);
    h_Muon_loose_pt_eta_bin -> Add(h_Muon_loose_pt_eta_bin_wjets, -1);

    h_Muon_tight_pt_eta_bin -> Add(h_Muon_tight_pt_eta_bin_zjets, -1);
    h_Muon_tight_pt_eta_bin -> Add(h_Muon_tight_pt_eta_bin_wjets, -1);

    h_Muon_FR_pt_eta_EWK -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., " ");
    h_Muon_FR_pt_eta_EWK -> Write("FR_pT_eta_EWKcorr");

    MuFR -> Close();

    // ===================================================================================================
    // Muon Prompt Rate
    // ===================================================================================================

    // Muon PR TH2D histograms
    TFile *MuPR = new TFile("MuPR_Run2016B_25ns_jet35_21Jun.root","recreate");

    TH1D* h_Muon_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_loose_pt_eta_PR");
    TH1D* h_Muon_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_tight_pt_eta_PR");

    TH2D* Muon_PR_pT_eta = (TH2D*) h_Muon_tight_pt_eta_PR -> Clone();

    Muon_PR_pT_eta -> Divide(h_Muon_tight_pt_eta_PR, h_Muon_loose_pt_eta_PR, 1., 1., "");
    Muon_PR_pT_eta -> Write("PR_pT_eta");

    MuPR -> Close();

}


