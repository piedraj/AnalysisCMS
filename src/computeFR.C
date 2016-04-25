
void computeFR() {
  
  TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
  TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
  TFile*  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");


  // Without EWK correction
  // =======================================
  TH2F*   h_Muon_loose_pt_eta_bin = (TH2F*) data  -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_Muon_tight_pt_eta_bin = (TH2F*) data  -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

  //  TCanvas *c0 = new TCanvas("c0","c0");
  //  h_Muon_tight_pt_eta_bin -> Draw();

    for (int j=1; j<=h_Muon_tight_pt_eta_bin -> GetNbinsY(); j++)
      {
	Int_t binx = h_Muon_tight_pt_eta_bin ->GetXaxis()->FindBin(17.);
	Int_t biny = j;
	Int_t bin = h_Muon_tight_pt_eta_bin ->GetBin(binx,biny,0); //see doc of TH1::GetBin

      printf(" i: %d j: %d %f\n", binx, biny, h_Muon_loose_pt_eta_bin -> GetBinContent(bin));
      }

    //TCanvas *c1 = new TCanvas("c1","c1");
  TH1F*   h_kk = (TH1F*) h_Muon_tight_pt_eta_bin -> ProjectionX();
  //h_kk->Draw();
  //return;

  TH2F*   h_Ele_loose_pt_eta_bin  = (TH2F*) data  -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_Ele_tight_pt_eta_bin  = (TH2F*) data  -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");

  // Muons
  TH2F*   h_Muon_FR_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
 
  h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  h_Muon_FR_pt_eta -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_pt_eta -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta -> SetYTitle("Muon eta");

  // h_Muon_FR_pt_eta -> Draw();
  
  TH1F*   h_Muon_loose_pt = (TH1F*) h_Muon_loose_pt_eta_bin -> ProjectionX();
  //  h_Muon_loose_pt -> Draw();

  TH1F*   h_Muon_FR_eta = (TH1F*) h_Muon_FR_pt_eta -> ProjectionY();
  //  h_Muon_FR_eta -> Draw();

  // Electrons
  TH2F*   h_Ele_FR_pt_eta = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  h_Ele_FR_pt_eta -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_pt_eta -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta -> SetYTitle("Electron eta");

  //       h_Ele_FR_pt_eta -> Draw("colz");
       h_Ele_FR_pt_eta -> Draw();


  // With EWK correction  
  // =======================================

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

  h_Muon_loose_pt_eta_corrZ -> Add(h_zjets_Muon_loose_pt_eta, -1);
  h_Muon_tight_pt_eta_corrZ -> Add(h_zjets_Muon_tight_pt_eta, -1);

  h_Muon_loose_pt_eta_corrZ -> Add(h_wjets_Muon_loose_pt_eta, -1);
  h_Muon_tight_pt_eta_corrZ -> Add(h_wjets_Muon_tight_pt_eta, -1);
    
  h_Muon_FR_pt_eta_corrZ -> Divide(h_Muon_tight_pt_eta_corrZ, h_Muon_loose_pt_eta_corrZ, 1., 1., "");

  h_Muon_FR_pt_eta_corrZ -> SetTitle("Muon Fake Rate with EWK correction");
  h_Muon_FR_pt_eta_corrZ -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta_corrZ -> SetYTitle("Muon eta");

   h_Muon_FR_pt_eta_corrZ -> Draw();

  // Electrons
  TH2F*   h_Ele_loose_pt_eta_corrZ  = (TH2F*) h_Ele_loose_pt_eta_bin -> Clone();
  TH2F*   h_Ele_tight_pt_eta_corrZ  = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  TH2F*   h_Ele_FR_pt_eta_corrZ     = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_loose_pt_eta_corrZ -> Add(h_zjets_Ele_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_zjets_Ele_tight_pt_eta, -1);

  h_Ele_loose_pt_eta_corrZ -> Add(h_wjets_Ele_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_wjets_Ele_tight_pt_eta, -1);
    
  h_Ele_FR_pt_eta_corrZ -> Divide(h_Ele_tight_pt_eta_corrZ, h_Ele_loose_pt_eta_corrZ, 1., 1., "");

  h_Ele_FR_pt_eta_corrZ -> SetTitle("Electron Fake Rate with EWK correction");
  h_Ele_FR_pt_eta_corrZ -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta_corrZ -> SetYTitle("Electron eta");

  //    h_Ele_FR_pt_eta_corrZ -> Draw();

}

 


















/*
void computeFR() {

  TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
  TFile*  Zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
  TFile*  Wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");
    
  Th2f*   h_Muon_loose_pt_eta_bin = (TH2F*) data -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_Muon_tight_pt_eta_bin = (TH2F*) data -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

  

  TH2F*   h_Muon_FR_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  h_Muon_FR_pt_eta -> SetTitle("Muon Fake Rate without EWK correction");
  h_Muon_FR_pt_eta -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta -> SetYTitle("Muon eta");

  h_Muon_FR_pt_eta -> Draw();

  TH2F*   h_zjets_loose_pt_eta = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_zjets_tight_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
  
  TH2F*   h_Muon_loose_pt_eta_corrZ  = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_Muon_tight_pt_eta_corrZ  = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
  TH2F*   h_Muon_FR_pt_eta_corrZ     = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_loose_pt_eta_corrZ -> Add(h_zjets_loose_pt_eta, -1);
  h_Muon_tight_pt_eta_corrZ -> Add(h_zjets_tight_pt_eta, -1);
    
  h_Muon_FR_pt_eta_corrZ -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  h_Muon_FR_pt_eta_corrZ -> SetTitle("Muon Fake Rate with EWK correction");
  h_Muon_FR_pt_eta_corrZ -> SetXTitle("Muon pt");
  h_Muon_FR_pt_eta_corrZ -> SetYTitle("Muon eta");

  h_Muon_FR_pt_eta_corrZ -> Draw();




  TH2F*   h_Ele_loose_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_Ele_tight_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");

  TH2F*   h_Ele_FR_pt_eta = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  h_Ele_FR_pt_eta -> SetTitle("Electron Fake Rate without EWK correction");
  h_Ele_FR_pt_eta -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta -> SetYTitle("Electron eta");

  h_Ele_FR_pt_eta -> Draw();

  TH2F*   h_Ele_loose_pt_eta_corrZ  = (TH2F*) h_Ele_loose_pt_eta_bin -> Clone();
  TH2F*   h_Ele_tight_pt_eta_corrZ  = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();
  TH2F*   h_Ele_FR_pt_eta_corrZ     = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_loose_pt_eta_corrZ -> Add(h_zjets_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_zjets_tight_pt_eta, -1);
    
  h_Ele_FR_pt_eta_corrZ -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  h_Ele_FR_pt_eta_corrZ -> SetTitle("Electron Fake Rate with EWK correction");
  h_Ele_FR_pt_eta_corrZ -> SetXTitle("Electron pt");
  h_Ele_FR_pt_eta_corrZ -> SetYTitle("Electron eta");

  h_Ele_FR_pt_eta_corrZ -> Draw();

 

}

*/





