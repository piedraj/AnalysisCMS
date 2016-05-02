
void computeFR() {
  
  TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
  TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
  TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

  // ========================
  // Fill histograms
  // ========================

  // Muons without correction

  TH2F* h_Muon_loose_pt_eta_bin = (TH2F*) data -> Get("h_Muon_loose_pt_eta_bin");
  TH2F* h_Muon_tight_pt_eta_bin = (TH2F*) data -> Get("h_Muon_tight_pt_eta_bin");

  TH2F* h_Muon_FR_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  TH1F* h_Muon_loose_pt_bin = (TH1F*) data -> Get ("h_Muon_loose_pt_bin");
  TH1F* h_Muon_tight_pt_bin = (TH1F*) data -> Get ("h_Muon_tight_pt_bin");

  TH1F* h_Muon_FR_pt = (TH1F*) h_Muon_tight_pt_bin -> Clone();

  h_Muon_FR_pt -> Divide(h_Muon_tight_pt_bin, h_Muon_loose_pt_bin, 1., 1., "");

  TH1F* h_Muon_loose_eta_bin = (TH1F*) data -> Get ("h_Muon_loose_eta_bin");
  TH1F* h_Muon_tight_eta_bin = (TH1F*) data -> Get ("h_Muon_tight_eta_bin");

  TH1F* h_Muon_FR_eta = (TH1F*) h_Muon_tight_eta_bin -> Clone();

  h_Muon_FR_eta -> Divide(h_Muon_tight_eta_bin, h_Muon_loose_eta_bin, 1., 1., "");
  
  // Electrons without correction

  TH2F* h_Ele_loose_pt_eta_bin = (TH2F*) data -> Get("h_Ele_loose_pt_eta_bin");
  TH2F* h_Ele_tight_pt_eta_bin = (TH2F*) data -> Get("h_Ele_tight_pt_eta_bin");

  TH2F* h_Ele_FR_pt_eta = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  TH1F* h_Ele_loose_pt_bin = (TH1F*) data -> Get ("h_Ele_loose_pt_bin");
  TH1F* h_Ele_tight_pt_bin = (TH1F*) data -> Get ("h_Ele_tight_pt_bin");

  TH1F* h_Ele_FR_pt = (TH1F*) h_Ele_tight_pt_bin -> Clone();

  h_Ele_FR_pt -> Divide(h_Ele_tight_pt_bin, h_Ele_loose_pt_bin, 1., 1., "");

  TH1F* h_Ele_loose_eta_bin = (TH1F*) data -> Get ("h_Ele_loose_eta_bin");
  TH1F* h_Ele_tight_eta_bin = (TH1F*) data -> Get ("h_Ele_tight_eta_bin");

  TH1F* h_Ele_FR_eta = (TH1F*) h_Ele_tight_eta_bin -> Clone();

  h_Ele_FR_eta -> Divide(h_Ele_tight_eta_bin, h_Ele_loose_eta_bin, 1., 1., "");

  // Muons with correction

  // Electrons with correction


  // ========================
  // Draw histograms
  // ========================  

  // Muons without correction

  TCanvas* Muon_pt_eta = new TCanvas("Muon pt eta", "Muon pt eta");

  h_Muon_FR_pt_eta -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_pt_eta -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta -> SetYTitle("Muon eta");

  h_Muon_FR_pt_eta -> Draw();

  TCanvas* Muon_pt = new TCanvas("Muon pt", "Muon pt");

  h_Muon_FR_pt -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_pt -> SetXTitle("Muon pt");

  h_Muon_FR_pt -> Draw();

  TCanvas* Muon_eta = new TCanvas("Muon eta", "Muon eta");

  h_Muon_FR_eta -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_eta -> SetXTitle("Muon eta");

  h_Muon_FR_eta -> Draw();

  // Electrons without correction

  TCanvas* Electron_pt_eta = new TCanvas("Electron pt eta", "Electron pt eta");

  h_Ele_FR_pt_eta -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_pt_eta -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta -> SetYTitle("Electron eta");

  h_Ele_FR_pt_eta -> Draw();

  TCanvas* Ele_pt = new TCanvas("Electron pt", "Electron pt");

  h_Ele_FR_pt -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_pt -> SetXTitle("Electron pt");

  h_Ele_FR_pt -> Draw();

  TCanvas* Ele_eta = new TCanvas("Electron eta", "Electron eta");

  h_Ele_FR_eta -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_eta -> SetXTitle("Electron eta");

  h_Ele_FR_eta -> Draw();
  
  // Muons with correction

  // Electrons with correction






  /*

  TH2F*   h_Muon_loose_pt_eta_bin = (TH2F*) data -> Get("h_Muon_loose_pt_eta_bin");
  TH2F*   h_Muon_tight_pt_eta_bin = (TH2F*) data -> Get("h_Muon_tight_pt_eta_bin");
  TH2F*   h_Muon_FR_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
 
  h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");
   h_Muon_FR_pt_eta -> Draw();
   return;

  TH1F*   h_Muon_tight_pt = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX() -> Clone();
  TH1F*   h_Muon_loose_pt = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionX() -> Clone();
  TH1F*   h_Muon_FR_pt = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX() -> Clone();;

  h_Muon_FR_pt -> Divide(h_Muon_tight_pt, h_Muon_loose_pt, 1., 1., ""); 
  //  h_Muon_FR_pt -> Draw();
  //  return;

  TH1F*   h_Muon_tight_eta = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionY() -> Clone();
  TH1F*   h_Muon_loose_eta = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionY() -> Clone();
  TH1F*   h_Muon_FR_eta = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionY() -> Clone();;

  h_Muon_FR_eta -> Divide(h_Muon_tight_eta, h_Muon_loose_eta, 1., 1., ""); 
  
//  h_Muon_FR_eta -> Draw();
  // return;

  // Muons with Z correction
  
  TH2F*   h_zjets_Muon_loose_pt_eta_bin = (TH2F*) zjets -> Get("h_Muon_loose_pt_eta_bin");
  TH2F*   h_zjets_Muon_tight_pt_eta_bin = (TH2F*) zjets -> Get("h_Muon_tight_pt_eta_bin");

  TH2F*   h_Muon_loose_pt_eta_bin_corrZ = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_Muon_tight_pt_eta_bin_corrZ = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_loose_pt_eta_bin_corrZ -> Add(h_zjets_Muon_loose_pt_eta_bin, -1);  
  h_Muon_tight_pt_eta_bin_corrZ -> Add(h_zjets_Muon_tight_pt_eta_bin, -1);

  TH2F*   h_Muon_FR_pt_eta_corrZ = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
  
  h_Muon_FR_pt_eta_corrZ -> Divide(h_Muon_tight_pt_eta_bin_corrZ, h_Muon_loose_pt_eta_bin_corrZ, 1., 1., "");
  */
  /*
    h_Muon_tight_pt_eta_bin_corrZ -> Draw();
  return;
  h_Muon_loose_pt_eta_bin_corrZ -> Draw();
  return;
  */

  //h_Muon_FR_pt_eta_corrZ -> Draw("colz");
  //    h_Muon_FR_pt_eta_corrZ -> ProjectionX() -> Draw();
  














  /*

  // Without EWK correction
  // =======================================

  // Muons
  
  TH2F*   h_Muon_loose_pt_eta_bin = (TH2F*) data  -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_Muon_tight_pt_eta_bin = (TH2F*) data  -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

    
  TH1F*   h_Muon_tight_pt = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX();
  TH1F*   h_Muon_loose_pt = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionX();
  TH1F*   h_Muon_FR_pt = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX();

  h_Muon_FR_pt -> Divide(h_Muon_tight_pt, h_Muon_loose_pt, 1., 1., ""); 
  
  TH1F*   h_Muon_tight_eta = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionY();
  TH1F*   h_Muon_loose_eta = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionY();
  TH1F*   h_Muon_FR_eta = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionY();
  
  h_Muon_FR_eta -> Divide(h_Muon_tight_eta, h_Muon_loose_eta, 1., 1., "");
  */
  /*

  // Electrons

  TH2F*   h_Ele_loose_pt_eta_bin  = (TH2F*) data  -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_Ele_tight_pt_eta_bin  = (TH2F*) data  -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");
  TH2F*   h_Ele_FR_pt_eta = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  //   h_Ele_FR_pt_eta -> Draw();

  TH1F*   h_Ele_tight_pt = (TH1F*) h_Ele_tight_pt_eta_bin -> ProjectionX();
  TH1F*   h_Ele_loose_pt = (TH1F*) h_Ele_loose_pt_eta_bin -> ProjectionX();
  TH1F*   h_Ele_FR_pt = (TH1F*) h_Ele_tight_pt_eta_bin -> ProjectionX();

  h_Ele_FR_pt -> Divide(h_Ele_tight_pt, h_Ele_loose_pt, 1., 1., ""); 

  TH1F*   h_Ele_tight_eta = (TH1F*) h_Ele_tight_pt_eta_bin -> ProjectionY();
  TH1F*   h_Ele_loose_eta = (TH1F*) h_Ele_loose_pt_eta_bin -> ProjectionY();
  TH1F*   h_Ele_FR_eta = (TH1F*) h_Ele_tight_pt_eta_bin -> ProjectionY();
  
  h_Ele_FR_eta -> Divide(h_Ele_tight_eta, h_Ele_loose_eta, 1., 1., "");

  */
  // With EWK correction  
  // =======================================
  /*
  // Z+jets
  TH2F*   h_zjets_Muon_loose_pt_eta = (TH2F*) zjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_zjets_Muon_tight_pt_eta = (TH2F*) zjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

  TH2F*   h_zjets_Ele_loose_pt_eta  = (TH2F*) zjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_zjets_Ele_tight_pt_eta  = (TH2F*) zjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");

  // W+jets
  TH2F*   h_wjets_Muon_loose_pt_eta = (TH2F*) wjets -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_wjets_Muon_tight_pt_eta = (TH2F*) wjets -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

  TH2F*   h_wjets_Ele_loose_pt_eta  = (TH2F*) wjets -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_wjets_Ele_tight_pt_eta  = (TH2F*) wjets -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");
  
  // Muons
  TH2F*   h_Muon_loose_pt_eta_corrZ  = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_Muon_tight_pt_eta_corrZ  = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  TH2F*   h_Muon_FR_pt_eta_corrZ     = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_tight_pt_eta_corrZ -> Add(h_zjets_Muon_tight_pt_eta, -1);
  h_Muon_loose_pt_eta_corrZ -> Add(h_zjets_Muon_loose_pt_eta, -1);

  h_Muon_tight_pt_eta_corrZ -> Add(h_wjets_Muon_tight_pt_eta, -1);
  h_Muon_loose_pt_eta_corrZ -> Add(h_wjets_Muon_loose_pt_eta, -1);
    
  h_Muon_FR_pt_eta_corrZ -> Divide(h_Muon_tight_pt_eta_corrZ, h_Muon_loose_pt_eta_corrZ, 1., 1., "");
  
  TH1F*  h_Muon_tight_pt_corrZ = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX();
  TH1F*  h_Muon_loose_pt_corrZ = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionX();
  TH1F*  h_Muon_FR_pt_corrZ = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX();

  h_Muon_tight_pt_corrZ -> Add(h_zjets_Muon_tight_pt_eta -> ProjectionX(), -1);
  h_Muon_loose_pt_corrZ -> Add(h_zjets_Muon_loose_pt_eta -> ProjectionX(), -1);

  h_Muon_tight_pt_corrZ -> Add(h_wjets_Muon_tight_pt_eta -> ProjectionX(), -1);
  h_Muon_loose_pt_corrZ -> Add(h_wjets_Muon_loose_pt_eta -> ProjectionX(), -1);
  h_Muon_loose_pt_corrZ -> Draw();
  return;

  h_Muon_FR_pt_corrZ -> Divide(h_Muon_tight_pt_corrZ, h_Muon_loose_pt_corrZ, 1., 1., "");
  h_Muon_FR_pt_corrZ  -> Draw();
  */













  /*
  // Electrons
  TH2F*   h_Ele_loose_pt_eta_corrZ  = (TH2F*) h_Ele_loose_pt_eta_bin -> Clone();
  TH2F*   h_Ele_tight_pt_eta_corrZ  = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  TH2F*   h_Ele_FR_pt_eta_corrZ     = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_loose_pt_eta_corrZ -> Add(h_zjets_Ele_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_zjets_Ele_tight_pt_eta, -1);

  h_Ele_loose_pt_eta_corrZ -> Add(h_wjets_Ele_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_wjets_Ele_tight_pt_eta, -1);
    
  h_Ele_FR_pt_eta_corrZ -> Divide(h_Ele_tight_pt_eta_corrZ, h_Ele_loose_pt_eta_corrZ, 1., 1., "");
  */
  //    h_Ele_FR_pt_eta_corrZ -> Draw();


  // Draw histograms
  // =======================================
  /*     
  TCanvas* Muon_pt_eta = new TCanvas("Muon pt eta", "Muon pt eta");

  h_Muon_FR_pt_eta -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_pt_eta -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta -> SetYTitle("Muon eta");

  h_Muon_FR_pt_eta -> Draw();

  TCanvas* Muon_pt = new TCanvas("Muon pt", "Muon pt");
  
  h_Muon_FR_pt -> SetLineColor(4);
  h_Muon_FR_pt -> SetTitle("Muon FR pt");
  h_Muon_FR_pt -> SetXTitle("Muon pt");
  h_Muon_FR_pt -> SetYTitle("Muon FR");

  h_Muon_FR_pt -> Draw();
  */
  //  h_Muon_FR_pt_corrZ -> SetLineColor(2);
  //  h_Muon_FR_pt_corrZ -> Draw();
  
  /*
  TCanvas* Muon_eta = new TCanvas("Muon eta", "Muon eta");

  h_Muon_FR_eta -> SetLineColor(4);
  h_Muon_FR_eta -> SetTitle("Muon FR eta");
  h_Muon_FR_eta -> SetXTitle("Muon eta");
  h_Muon_FR_eta -> SetYTitle("Muon FR");
  h_Muon_FR_eta -> Draw();

  h_Muon_FR_eta_corrZ -> SetLineColor(2);
  h_Muon_FR_eta_corrZ -> Draw();
  
  TCanvas* Ele_pt_eta = new TCanvas("Ele pt eta", "Ele pt eta");

  h_Ele_FR_pt_eta -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_pt_eta -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta -> SetYTitle("Electron eta");

  //  h_Ele_FR_pt_eta -> Draw();

  
  TCanvas* Ele_pt = new TCanvas("Ele pt", "Ele pt");

  h_Ele_FR_pt -> SetLineColor(4);
  h_Ele_FR_pt -> SetTitle("Ele FR pt");
  h_Ele_FR_pt -> SetXTitle("Ele pt");
  h_Ele_FR_pt -> SetYTitle("Ele FR");
  h_Ele_FR_pt -> Draw();

  h_Ele_FR_pt_corrZ -> SetLineColor(2);
  h_Ele_FR_pt_corrZ -> Draw();

  TCanvas *Ele_eta = new TCanvas("Ele eta", "Ele eta");

  h_Ele_FR_eta -> SetLineColor(4);
  h_Ele_FR_eta -> SetTitle("Ele FR eta");
  h_Ele_FR_eta -> SetXTitle("Ele eta");
  h_Ele_FR_eta -> SetYTitle("Ele FR");
  h_Ele_FR_eta -> Draw();

  h_Ele_FR_eta_corrZ -> SetLineColor(2);
  h_Ele_FR_eta_corrZ -> Draw();
  */

}

