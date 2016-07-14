void getFakeRate()
{
  
    gInterpreter->ExecuteMacro("../test/PaperStyle.C");

    int njetet;
    const Double_t muonjetarray[8] = {10., 15., 20., 25., 30., 35., 45.};
    const Double_t elejetarray[8] = {10., 15., 20., 25., 30., 35., 45.};

    float elejetet;
    float muonjetet;

    bool draw = true;
    bool savepng = false;

    if (draw) {
      njetet = 1;
    } else {
      njetet = 8;
    }

    for (int i=0; i < njetet; i++) {

      if(draw) {
	elejetet = 35.;
	muonjetet = 25.;
      } else {
	elejetet = elejetarray[i];
	muonjetet = muonjetarray[i];
      }

    TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
    TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
    TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

    // ===================================================================================================
    // Electron Fake Rate
    // ===================================================================================================

    // Electron pt    
    TString elesuffix  = Form("_%.0fGev", elejetet);

    TH1D* h_Ele_loose_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_loose_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_bin" + elesuffix);
    TH1D* h_Ele_tight_pt_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_bin" + elesuffix);

    TH1D* h_Ele_FR_pt = (TH1D*) h_Ele_tight_pt_bin -> Clone();
    TH1D* h_Ele_FR_pt_EWK = (TH1D*) h_Ele_tight_pt_bin -> Clone();
      
    if(draw) {

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
    
      if (savepng) Ele_pt -> SaveAs(Form("Ele_FR_pt_%.0fGev.png", elejetet));
    
    }

    // Electron eta    

    TH1D* h_Ele_loose_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin = (TH1D*) data -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_zjets = (TH1D*) zjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_loose_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_loose_eta_bin" + elesuffix);
    TH1D* h_Ele_tight_eta_bin_wjets = (TH1D*) wjets -> Get("FR/00_QCD/h_Ele_tight_eta_bin" + elesuffix);

    TH1D* h_Ele_FR_eta = (TH1D*) h_Ele_tight_eta_bin -> Clone();
    TH1D* h_Ele_FR_eta_EWK = (TH1D*) h_Ele_tight_eta_bin -> Clone();

    if(draw) {

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

      if(savepng) Ele_eta -> SaveAs(Form("Ele_FR_eta_%.0fGev.png", elejetet));

    }

    // Electron FR TH2D histograms

    TFile *EleFR = new TFile(Form("EleFR_Run2016B_25ns_jet%0.f_21Jun.root", elejetet),"recreate");

    TH2D* h_Ele_loose_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin = (TH2D*) data -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* h_Ele_loose_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin_zjets = (TH2D*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

    TH2D* h_Ele_loose_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin" + elesuffix);
    TH2D* h_Ele_tight_pt_eta_bin_wjets = (TH2D*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin" + elesuffix);

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

    if(draw) {

      // Electron PR pt
      TCanvas* Ele_PR_pt = new TCanvas("Ele PR pt", "Ele PR pt", 450, 550);
    
      TH1D* h_Ele_loose_pt_PR = (TH1D*) zjets -> Get("h_Ele_loose_pt_PR");
      TH1D* h_Ele_tight_pt_PR = (TH1D*) zjets -> Get("h_Ele_tight_pt_PR");

      TH1D* h_Ele_PR_pt = (TH1D*) h_Ele_tight_pt_PR -> Clone();
    
      h_Ele_PR_pt -> SetTitle("Ele Prompt Rate");
      h_Ele_PR_pt -> SetXTitle("Ele pt");
      h_Ele_PR_pt -> SetYTitle("Ele PR");
      
      h_Ele_PR_pt -> Divide(h_Ele_tight_pt_PR, h_Ele_loose_pt_PR, 1., 1., "");
      h_Ele_PR_pt -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Ele_PR_pt -> Draw();
      
      if(savepng) Ele_PR_pt -> SaveAs(Form("Ele_PR_pt_%.0fGev.png", elejetet));

    }

    if(draw) {

      // Electron PR eta
      TCanvas* Ele_PR_eta = new TCanvas("Ele PR eta", "Ele PR eta", 450, 550);

      TH1D* h_Ele_loose_eta_PR = (TH1D*) zjets -> Get("h_Ele_loose_eta_PR");
      TH1D* h_Ele_tight_eta_PR = (TH1D*) zjets -> Get("h_Ele_tight_eta_PR");
    
      TH1D* h_Ele_PR_eta = (TH1D*) h_Ele_tight_eta_PR -> Clone();

      h_Ele_PR_eta -> SetTitle("Ele Prompt Rate");
      h_Ele_PR_eta -> SetXTitle("Ele eta");
      h_Ele_PR_eta -> SetYTitle("Ele PR");
      
      h_Ele_PR_eta -> Divide(h_Ele_tight_eta_PR, h_Ele_loose_eta_PR, 1., 1., "");
      h_Ele_PR_eta -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Ele_PR_eta -> Draw();

      TLine* line = new TLine(1.479, Ele_PR_eta -> GetUymin(), 1.479, Ele_PR_eta -> GetUymax());
  
      line -> SetLineWidth(2);
      line -> SetLineStyle(kDotted);
      line -> Draw("same");

      if(savepng) Ele_PR_eta -> SaveAs(Form("Ele_PR_eta_%.0fGev.png", elejetet));

    }

    // Electron PR TH2D histograms
    TFile *ElePR = new TFile("ElePR_Run2016B_25ns_21Jun.root","recreate");

    TH1D* h_Ele_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_loose_pt_eta_PR");
    TH1D* h_Ele_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Ele_tight_pt_eta_PR");

    TH2D* Ele_PR_pt_eta = (TH2D*) h_Ele_tight_pt_eta_PR -> Clone();

    Ele_PR_pt_eta -> Divide(h_Ele_tight_pt_eta_PR, h_Ele_loose_pt_eta_PR, 1., 1., "");
    Ele_PR_pt_eta -> Write("h_Ele_signal_pt_eta_bin");

    ElePR -> Close();

    // ===================================================================================================
    // Muon Fake Rate
    // ===================================================================================================

    // Muon pt
    TString muonsuffix = Form("_%.0fGev", muonjetet);
    
    if(draw) {

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
    
      if(savepng) Muon_pt -> SaveAs(Form("Muon_FR_pt_%.0fGev.png", muonjetet));

    }

    // Muon eta
    
    if(draw) {

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

    if(draw) {

      // Muon PR pt
      TCanvas* Muon_PR_pt = new TCanvas("Muon PR pt", "Muon PR pt", 450, 550);

      TH1D* h_Muon_loose_pt_PR = (TH1D*) zjets -> Get("h_Muon_loose_pt_PR");
      TH1D* h_Muon_tight_pt_PR = (TH1D*) zjets -> Get("h_Muon_tight_pt_PR");
      
      TH1D* h_Muon_PR_pt = (TH1D*) h_Muon_tight_pt_PR -> Clone();
    
      h_Muon_PR_pt -> SetTitle("Muon Prompt Rate");
      h_Muon_PR_pt -> SetXTitle("Muon pt");
      h_Muon_PR_pt -> SetYTitle("Muon PR");

      h_Muon_PR_pt -> Divide(h_Muon_tight_pt_PR, h_Muon_loose_pt_PR, 1., 1., "");
      h_Muon_PR_pt -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Muon_PR_pt -> Draw();

      if(savepng) Muon_PR_pt -> SaveAs(Form("Muon_PR_pt_%.0fGev.png", muonjetet));

    }


    if(draw) {

      // Muon PR eta
      TCanvas* Muon_PR_eta = new TCanvas("Muon PR eta", "Muon PR eta", 450, 550);

      TH1D* h_Muon_loose_eta_PR = (TH1D*) zjets -> Get("h_Muon_loose_eta_PR");
      TH1D* h_Muon_tight_eta_PR = (TH1D*) zjets -> Get("h_Muon_tight_eta_PR");

      TH1D* h_Muon_PR_eta = (TH1D*) h_Muon_tight_eta_PR -> Clone();

      h_Muon_PR_eta -> SetTitle("Muon Prompt Rate");
      h_Muon_PR_eta -> SetXTitle("Muon eta");
      h_Muon_PR_eta -> SetYTitle("Muon PR");

      h_Muon_PR_eta -> Divide(h_Muon_tight_eta_PR, h_Muon_loose_eta_PR, 1., 1., "");
      h_Muon_PR_eta -> GetYaxis() -> SetRangeUser(0.5, 1.1);
      h_Muon_PR_eta -> Draw();

      if(savepng) Muon_PR_eta -> SaveAs(Form("Muon_PR_eta_%.0fGev.png", muonjetet));

    }

    // Muon PR TH2D histograms
    TFile *MuPR = new TFile("MuPR_Run2016B_25ns_21Jun.root","recreate");

    TH1D* h_Muon_loose_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_loose_pt_eta_PR");
    TH1D* h_Muon_tight_pt_eta_PR = (TH1D*) zjets -> Get("h_Muon_tight_pt_eta_PR");

    TH2D* Muon_PR_pT_eta = (TH2D*) h_Muon_tight_pt_eta_PR -> Clone();

    Muon_PR_pT_eta -> Divide(h_Muon_tight_pt_eta_PR, h_Muon_loose_pt_eta_PR, 1., 1., "");
    Muon_PR_pT_eta -> Write("h_Muon_signal_pt_eta_bin");

    MuPR -> Close();

    }

}


