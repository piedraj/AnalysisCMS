void getFakeRate() {

    TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
    TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
    TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

    //    TFile*  jasper = new TFile("MuFR_RunII_25ns_jet20_226pb_76X.root","read");

    // Muon FR pt

    TH1D* h_Muon_loose_pt_bin = (TH1D*) data -> Get ("h_Muon_loose_pt_bin");
    TH1D* h_Muon_tight_pt_bin = (TH1D*) data -> Get ("h_Muon_tight_pt_bin");

    TH1D* h_zjets_Muon_loose_pt_bin = (TH1D*) zjets -> Get ("h_Muon_loose_pt_bin");
    TH1D* h_zjets_Muon_tight_pt_bin = (TH1D*) zjets -> Get ("h_Muon_tight_pt_bin");

    TH1D* h_wjets_Muon_loose_pt_bin = (TH1D*) wjets -> Get ("h_Muon_loose_pt_bin");
    TH1D* h_wjets_Muon_tight_pt_bin = (TH1D*) wjets -> Get ("h_Muon_tight_pt_bin");

    TH1D* h_Muon_FR_pt        = (TH1D*) h_Muon_tight_pt_bin -> Clone();
    TH1D* h_Muon_FR_pt_corrWZ = (TH1D*) h_Muon_tight_pt_bin -> Clone();

    TCanvas* Muon_pt = new TCanvas("Muon pt", "Muon pt", 450, 550);

    Muon_pt->SetGridx(1);
    Muon_pt->SetGridy(1);

    h_Muon_FR_pt -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin, 1., 1., "");

    h_Muon_FR_pt -> GetYaxis() -> SetNdivisions(510);
    
    h_Muon_FR_pt -> Draw();

    h_Muon_FR_pt -> SetLineColor(4);
    h_Muon_FR_pt -> SetAxisRange(0,1,"Y");

    h_Muon_FR_pt -> SetTitle("Muon Fake Rate with and without EWK correction");
    h_Muon_FR_pt -> SetXTitle("Muon pt");
    h_Muon_FR_pt -> SetYTitle("Muon FR");

    h_Muon_loose_pt_bin -> Add(h_zjets_Muon_loose_pt_bin, -1);
    h_Muon_loose_pt_bin -> Add(h_wjets_Muon_loose_pt_bin, -1);

    h_Muon_tight_pt_bin -> Add(h_zjets_Muon_tight_pt_bin, -1);
    h_Muon_tight_pt_bin -> Add(h_wjets_Muon_tight_pt_bin, -1);

    h_Muon_FR_pt_corrWZ -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin, 1., 1., " ");
    h_Muon_FR_pt_corrWZ -> Draw("same");

    h_Muon_FR_pt_corrWZ -> SetLineColor(2);

    // Muon FR eta

    TH1D* h_Muon_loose_eta_bin = (TH1D*) data -> Get ("h_Muon_loose_eta_bin");
    TH1D* h_Muon_tight_eta_bin = (TH1D*) data -> Get ("h_Muon_tight_eta_bin");

    TH1D* h_zjets_Muon_loose_eta_bin = (TH1D*) zjets -> Get ("h_Muon_loose_eta_bin");
    TH1D* h_zjets_Muon_tight_eta_bin = (TH1D*) zjets -> Get ("h_Muon_tight_eta_bin");

    TH1D* h_wjets_Muon_loose_eta_bin = (TH1D*) wjets -> Get ("h_Muon_loose_eta_bin");
    TH1D* h_wjets_Muon_tight_eta_bin = (TH1D*) wjets -> Get ("h_Muon_tight_eta_bin");

    TH1D* h_Muon_FR_eta        = (TH1D*) h_Muon_tight_eta_bin -> Clone();
    TH1D* h_Muon_FR_eta_corrWZ = (TH1D*) h_Muon_tight_eta_bin -> Clone();

    TCanvas* Muon_eta = new TCanvas("Muon eta", "Muon eta");

    h_Muon_FR_eta -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin, 1., 1., "");

    h_Muon_FR_eta -> Draw();
    h_Muon_FR_eta -> SetLineColor(4);
    h_Muon_FR_eta -> SetAxisRange(0,1,"Y");

    h_Muon_FR_eta -> SetTitle("Muon Fake Rate with and without EWK correction");
    h_Muon_FR_eta -> SetXTitle("Muon eta");
    h_Muon_FR_eta -> SetYTitle("Muon FR");

    h_Muon_loose_eta_bin -> Add(h_zjets_Muon_loose_eta_bin, -1);
    h_Muon_loose_eta_bin -> Add(h_wjets_Muon_loose_eta_bin, -1);

    h_Muon_tight_eta_bin -> Add(h_zjets_Muon_tight_eta_bin, -1);
    h_Muon_tight_eta_bin -> Add(h_wjets_Muon_tight_eta_bin, -1);

    h_Muon_FR_eta_corrWZ -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin, 1., 1., " ");
    h_Muon_FR_eta_corrWZ -> Draw("same");

    h_Muon_FR_eta_corrWZ -> SetLineColor(2);

    // Electron FR

    /*

    TH1D* h_Ele_loose_pt_eta_bin = (TH1D*) data -> Get("h_Ele_loose_pt_eta_bin");
    TH1D* h_Ele_tight_pt_eta_bin = (TH1D*) data -> Get("h_Ele_tight_pt_eta_bin");

    TH1D* h_zjets_Ele_loose_pt_eta_bin = (TH1D*) zjets -> Get("h_Ele_loose_pt_eta_bin");
    TH1D* h_zjets_Ele_tight_pt_eta_bin = (TH1D*) zjets -> Get("h_Ele_tight_pt_eta_bin");

    TH1D* h_wjets_Ele_loose_pt_eta_bin = (TH1D*) wjets -> Get("h_Ele_loose_pt_eta_bin");
    TH1D* h_wjets_Ele_tight_pt_eta_bin = (TH1D*) wjets -> Get("h_Ele_tight_pt_eta_bin");

    TH1D* h_Ele_FR_pt_eta = (TH1D*) h_Ele_tight_pt_eta_bin -> Clone();

    h_Ele_loose_pt_eta_bin -> Add(h_zjets_Ele_loose_pt_eta_bin, -1);
    h_Ele_loose_pt_eta_bin -> Add(h_wjets_Ele_loose_pt_eta_bin, -1);

    h_Ele_tight_pt_eta_bin -> Add(h_zjets_Ele_tight_pt_eta_bin, -1);
    h_Ele_tight_pt_eta_bin -> Add(h_wjets_Ele_tight_pt_eta_bin, -1);

    h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");
    h_Ele_FR_pt_eta -> Draw("colztext");

    */

    // Ele FR pt

    
    TH1D* h_Ele_loose_pt_bin = (TH1D*) data -> Get ("h_Ele_loose_pt_bin");
    TH1D* h_Ele_tight_pt_bin = (TH1D*) data -> Get ("h_Ele_tight_pt_bin");

    TH1D* h_Ele_loose_pt_bin_corrWZ = (TH1D*) data -> Get ("h_Ele_loose_pt_bin");
    TH1D* h_Ele_tight_pt_bin_corrWZ = (TH1D*) data -> Get ("h_Ele_tight_pt_bin");

    TH1D* h_zjets_Ele_loose_pt_bin = (TH1D*) zjets -> Get ("h_Ele_loose_pt_bin");
    TH1D* h_zjets_Ele_tight_pt_bin = (TH1D*) zjets -> Get ("h_Ele_tight_pt_bin");

    TH1D* h_wjets_Ele_loose_pt_bin = (TH1D*) wjets -> Get ("h_Ele_loose_pt_bin");
    TH1D* h_wjets_Ele_tight_pt_bin = (TH1D*) wjets -> Get ("h_Ele_tight_pt_bin");

    TH1D* h_Ele_FR_pt        = (TH1D*) h_Ele_tight_pt_bin -> Clone();
    TH1D* h_Ele_FR_pt_corrWZ = (TH1D*) h_Ele_tight_pt_bin -> Clone();

    TCanvas* Ele_pt = new TCanvas("Ele pt", "Ele pt");

    h_Ele_FR_pt -> Divide(h_Ele_tight_pt_bin, h_Ele_loose_pt_bin, 1., 1., "");

    h_Ele_FR_pt -> Draw();
    h_Ele_FR_pt -> SetLineColor(4);
    h_Ele_FR_pt -> SetAxisRange(0,0.5,"Y");

    h_Ele_FR_pt -> SetTitle("Ele Fake Rate with and without EWK correction");
    h_Ele_FR_pt -> SetXTitle("Ele pt");
    h_Ele_FR_pt -> SetYTitle("Ele FR");

    h_Ele_loose_pt_bin_corrWZ -> Add(h_zjets_Ele_loose_pt_bin, -1);
    h_Ele_loose_pt_bin_corrWZ -> Add(h_wjets_Ele_loose_pt_bin, -1);

    h_Ele_tight_pt_bin_corrWZ -> Add(h_zjets_Ele_tight_pt_bin, -1);
    h_Ele_tight_pt_bin_corrWZ -> Add(h_wjets_Ele_tight_pt_bin, -1);

    h_Ele_FR_pt_corrWZ -> Divide(h_Ele_tight_pt_bin_corrWZ, h_Ele_loose_pt_bin_corrWZ, 1., 1., " ");
    h_Ele_FR_pt_corrWZ -> Draw("same");

    h_Ele_FR_pt_corrWZ -> SetLineColor(2);
    
    /*
    
    TH1D* h_loose_relative = (TH1D*) h_Ele_loose_pt_bin -> Clone();
    TH1D* h_tight_relative = (TH1D*) h_Ele_tight_pt_bin -> Clone();

    TCanvas* Ele_loose_pt_relative = new TCanvas("Ele pt loose relative", "Ele pt loose relative");

    h_loose_relative -> Divide(h_Ele_loose_pt_bin, h_Ele_loose_pt_bin_corrWZ, 1., 1., "");
    h_loose_relative -> Draw();

    TCanvas* Ele_tight_pt_relative = new TCanvas("Ele pt tight relative", "Ele pt tight relative");

    h_tight_relative -> Divide(h_Ele_tight_pt_bin, h_Ele_tight_pt_bin_corrWZ, 1., 1., "");
    h_tight_relative -> Draw();

    */
    /*
    THStack* MC_loose = new THStack("MC loose", "MC loose");

    h_zjets_Ele_loose_pt_bin -> SetFillColor(kRed);
    h_zjets_Ele_loose_pt_bin -> SetFillStyle(1001);

    h_wjets_Ele_loose_pt_bin -> SetFillColor(kBlue);
    h_wjets_Ele_loose_pt_bin -> SetFillStyle(1001);

    MC_loose -> Add(h_zjets_Ele_loose_pt_bin);
    MC_loose -> Add(h_wjets_Ele_loose_pt_bin);

    TCanvas* Analysis_loose = new TCanvas("Loose analysis", "loose analysis");

    h_Ele_loose_pt_bin -> Draw();
    MC_loose -> Draw("same");

    THStack* MC_tight = new THStack("MC tight", "MC tight");

    h_zjets_Ele_tight_pt_bin -> SetFillColor(kRed);
    h_wjets_Ele_tight_pt_bin -> SetFillColor(kBlue);

    MC_tight -> Add(h_zjets_Ele_tight_pt_bin);
    MC_tight -> Add(h_wjets_Ele_tight_pt_bin);

    TCanvas* Analysis_tight = new TCanvas("Tight analysis", "tight analysis");

    h_Ele_tight_pt_bin -> Draw();
    MC_tight -> Draw("same");
    */

    // Electron FR eta
    
    TH1D* h_Ele_loose_eta_bin = (TH1D*) data -> Get ("h_Ele_loose_eta_bin");
    TH1D* h_Ele_tight_eta_bin = (TH1D*) data -> Get ("h_Ele_tight_eta_bin");

    TH1D* h_zjets_Ele_loose_eta_bin = (TH1D*) zjets -> Get ("h_Ele_loose_eta_bin");
    TH1D* h_zjets_Ele_tight_eta_bin = (TH1D*) zjets -> Get ("h_Ele_tight_eta_bin");

    TH1D* h_wjets_Ele_loose_eta_bin = (TH1D*) wjets -> Get ("h_Ele_loose_eta_bin");
    TH1D* h_wjets_Ele_tight_eta_bin = (TH1D*) wjets -> Get ("h_Ele_tight_eta_bin");

    TH1D* h_Ele_FR_eta        = (TH1D*) h_Ele_tight_eta_bin -> Clone();
    TH1D* h_Ele_FR_eta_corrWZ = (TH1D*) h_Ele_tight_eta_bin -> Clone();

    TCanvas* Ele_eta = new TCanvas("Ele eta", "Ele eta");

    h_Ele_FR_eta -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin, 1., 1., "");

    h_Ele_FR_eta -> Draw();
    h_Ele_FR_eta -> SetLineColor(4);
    h_Ele_FR_eta -> SetAxisRange(0,0.5,"Y");

    h_Ele_FR_eta -> SetTitle("Ele Fake Rate with and without EWK correction");
    h_Ele_FR_eta -> SetXTitle("Ele eta");
    h_Ele_FR_eta -> SetYTitle("Ele FR");

    h_Ele_loose_eta_bin -> Add(h_zjets_Ele_loose_eta_bin, -1);
    h_Ele_loose_eta_bin -> Add(h_wjets_Ele_loose_eta_bin, -1);

    h_Ele_tight_eta_bin -> Add(h_zjets_Ele_tight_eta_bin, -1);
    h_Ele_tight_eta_bin -> Add(h_wjets_Ele_tight_eta_bin, -1);

    h_Ele_FR_eta_corrWZ -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin, 1., 1., " ");
    h_Ele_FR_eta_corrWZ -> Draw("same");

    h_Ele_FR_eta_corrWZ -> SetLineColor(2);
    


        // Jasper comparison
    /*    
    TCanvas* Comparison = new TCanvas("Comparison", "Comparison");

    //    h_Muon_FR_pt_corrected -> Draw();
    h_Muon_FR_pt_corrWZ -> SetLineColor(2);

    TH2D* h_Muon_FR_jasper = (TH2D*) jasper -> Get ("FR_pT_eta_EWKcorr");
    TH1D* h_Muon_FR_pt_jasper = (TH1D*) h_Muon_FR_jasper ->ProjectionX("h_Muon_FR_pt_jasper");

    h_Muon_FR_pt_jasper -> Draw("same");
    */

}

