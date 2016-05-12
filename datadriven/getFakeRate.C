void getFakeRate()
{
  
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

    TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
    TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
    TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

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

    TH1D* h_Muon_loose_pt_stack = (TH1D*) data -> Get ("h_Muon_loose_pt_stack");
    TH1D* h_Muon_tight_pt_stack = (TH1D*) data -> Get ("h_Muon_tight_pt_stack");
    TH1D* h_Ele_loose_pt_stack = (TH1D*) data -> Get ("h_Ele_loose_pt_stack");
    TH1D* h_Ele_tight_pt_stack = (TH1D*) data -> Get ("h_Ele_tight_pt_stack");

    TH1D* h_zjets_Muon_loose_pt_stack = (TH1D*) zjets -> Get ("h_Muon_loose_pt_stack");
    TH1D* h_zjets_Muon_tight_pt_stack = (TH1D*) zjets -> Get ("h_Muon_tight_pt_stack");
    TH1D* h_zjets_Ele_loose_pt_stack = (TH1D*) zjets -> Get ("h_Ele_loose_pt_stack");
    TH1D* h_zjets_Ele_tight_pt_stack = (TH1D*) zjets -> Get ("h_Ele_tight_pt_stack");

    TH1D* h_wjets_Muon_loose_pt_stack = (TH1D*) wjets -> Get ("h_Muon_loose_pt_stack");
    TH1D* h_wjets_Muon_tight_pt_stack = (TH1D*) wjets -> Get ("h_Muon_tight_pt_stack");
    TH1D* h_wjets_Ele_loose_pt_stack = (TH1D*) wjets -> Get ("h_Ele_loose_pt_stack");
    TH1D* h_wjets_Ele_tight_pt_stack = (TH1D*) wjets -> Get ("h_Ele_tight_pt_stack");
    
    THStack* MC_Muon_loose = new THStack("MC muon loose", "MC muon loose");

    h_zjets_Muon_loose_pt_stack -> SetFillColor(kGreen+2);
    h_zjets_Muon_loose_pt_stack -> SetFillStyle(1001);
    h_zjets_Muon_loose_pt_stack -> Rebin(2);

    h_wjets_Muon_loose_pt_stack -> SetFillColor(kGray+1);
    h_wjets_Muon_loose_pt_stack -> SetFillStyle(1001);
    h_wjets_Muon_loose_pt_stack -> Rebin(2);

    MC_Muon_loose -> Add(h_wjets_Muon_loose_pt_stack);
    MC_Muon_loose -> Add(h_zjets_Muon_loose_pt_stack);

    TCanvas* Muon_Analysis_loose = new TCanvas("Muon loose analysis", "Muon loose analysis");

    Muon_Analysis_loose -> SetLogy();
    h_Muon_loose_pt_stack -> SetMarkerStyle(kFullCircle);

    h_Muon_loose_pt_stack -> Draw("ep");
    h_Muon_loose_pt_stack -> Rebin(2);
    MC_Muon_loose -> Draw("hist, same");
    h_Muon_loose_pt_stack -> Draw("ep, same");

    THStack* MC_Muon_tight = new THStack("MC muon tight", "MC muon tight");

    h_zjets_Muon_tight_pt_stack -> SetFillColor(kGreen+2);
    h_zjets_Muon_tight_pt_stack -> SetFillStyle(1001);
    h_zjets_Muon_tight_pt_stack -> Rebin(2);

    h_wjets_Muon_tight_pt_stack -> SetFillColor(kGray+1);
    h_wjets_Muon_tight_pt_stack -> SetFillStyle(1001);
    h_wjets_Muon_tight_pt_stack -> Rebin(2);

    MC_Muon_tight -> Add(h_wjets_Muon_tight_pt_stack);
    MC_Muon_tight -> Add(h_zjets_Muon_tight_pt_stack);

    TCanvas* Muon_Analysis_tight = new TCanvas("Muon tight analysis", "Muon tight analysis");

    Muon_Analysis_tight -> SetLogy();
    h_Muon_tight_pt_stack -> SetMarkerStyle(kFullCircle);
    h_Muon_tight_pt_stack -> Draw("ep");
    h_Muon_tight_pt_stack -> Rebin(2);
    MC_Muon_tight -> Draw("hist,same");
    h_Muon_tight_pt_stack -> Draw("ep, same");

    THStack* MC_Ele_loose = new THStack("MC ele loose", "MC ele loose");

    h_zjets_Ele_loose_pt_stack -> SetFillColor(kGreen+2);
    h_zjets_Ele_loose_pt_stack -> SetFillStyle(1001);
    h_zjets_Ele_loose_pt_stack -> Rebin(2);

    h_wjets_Ele_loose_pt_stack -> SetFillColor(kGray+1);
    h_wjets_Ele_loose_pt_stack -> SetFillStyle(1001);
    h_wjets_Ele_loose_pt_stack -> Rebin(2);

    MC_Ele_loose -> Add(h_wjets_Ele_loose_pt_stack);
    MC_Ele_loose -> Add(h_zjets_Ele_loose_pt_stack);

    TCanvas* Ele_Analysis_loose = new TCanvas("Ele loose analysis", "Ele loose analysis");

    Ele_Analysis_loose -> SetLogy();
    h_Ele_loose_pt_stack -> SetMarkerStyle(kFullCircle);
    h_Ele_loose_pt_stack -> Draw("ep");
    h_Ele_loose_pt_stack -> Rebin(2);
    MC_Ele_loose -> Draw("hist, same");
    h_Ele_loose_pt_stack -> Draw("ep, same");

    THStack* MC_Ele_tight = new THStack("MC ele tight", "MC ele tight");

    h_zjets_Ele_tight_pt_stack -> SetFillColor(kGreen+2);
    h_zjets_Ele_tight_pt_stack -> SetFillStyle(1001);
    h_zjets_Ele_tight_pt_stack -> Rebin(20);

    h_wjets_Ele_tight_pt_stack -> SetFillColor(kGray+1);
    h_wjets_Ele_tight_pt_stack -> SetFillStyle(1001);
    h_wjets_Ele_tight_pt_stack -> Rebin(20);

    MC_Ele_tight -> Add(h_wjets_Ele_tight_pt_stack);
    MC_Ele_tight -> Add(h_zjets_Ele_tight_pt_stack);

    TCanvas* Ele_Analysis_tight = new TCanvas("Ele tight analysis", "Ele tight analysis");

    Ele_Analysis_tight -> SetLogy();
    h_Ele_tight_pt_stack -> SetMarkerStyle(kFullCircle);
    h_Ele_tight_pt_stack -> Draw("ep");
    h_Ele_tight_pt_stack -> Rebin(20);
    MC_Ele_tight -> Draw("hist,same");
    h_Ele_tight_pt_stack -> Draw("ep, same");

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

}

