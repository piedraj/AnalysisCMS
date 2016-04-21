
void computeFR() {

  //  TFile*  f = new TFile ("../rootfiles/nominal/FR/Run2015D_16Dec2015_DoubleMuon_0000__part0.root","read");
  TFile*  f = new TFile ("../rootfiles/nominal/FR/DYJetsToLL_M-50_0000__part0.root","read");
    
  TH2F*   h_Muon_loose_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Muon_loose_pt_eta_bin");
  TH2F*   h_Muon_tight_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Muon_tight_pt_eta_bin");

  TH2F*   h_Muon_FR_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_FR_pt_eta -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  TH2F*   h_zjets_loose_pt_eta = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_zjets_tight_pt_eta = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
  
  TH2F*   h_Muon_loose_pt_eta_corrZ  = (TH2F*) h_Muon_loose_pt_eta_bin -> Clone();
  TH2F*   h_Muon_tight_pt_eta_corrZ  = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();
  TH2F*   h_Muon_FR_pt_eta_corrZ     = (TH2F*) h_Muon_tight_pt_eta_bin -> Clone();

  h_Muon_loose_pt_eta_corrZ -> Add(h_zjets_loose_pt_eta, -1);
  h_Muon_tight_pt_eta_corrZ -> Add(h_zjets_tight_pt_eta, -1);
    
  h_Muon_FR_pt_eta_corrZ -> Divide(h_Muon_tight_pt_eta_bin, h_Muon_loose_pt_eta_bin, 1., 1., "");

  h_Muon_FR_pt_eta_corrZ -> Draw();


 //  TFile*  f = new TFile ("../rootfiles/nominal/FR/Run2015D_16Dec2015_DoubleMuon_0000__part0.root","read");
  TFile*  f = new TFile ("../rootfiles/nominal/FR/DYJetsToLL_M-50_0000__part0.root","read");
    
  TH2F*   h_Ele_loose_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Ele_loose_pt_eta_bin");
  TH2F*   h_Ele_tight_pt_eta_bin = (TH2F*)f -> Get("FR/00_QCD/h_Ele_tight_pt_eta_bin");

  TH2F*   h_Ele_FR_pt_eta = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();

  h_Ele_FR_pt_eta -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  TH2F*   h_Ele_loose_pt_eta_corrZ  = (TH2F*) h_Ele_loose_pt_eta_bin -> Clone();
  TH2F*   h_Ele_tight_pt_eta_corrZ  = (TH2F*) h_Ele_tight_pt_eta_bin -> Clone();
  TH2F*   h_Ele_FR_pt_eta_corrZ     = (TH2F*) h_ELe_tight_pt_eta_bin -> Clone();

  h_Ele_loose_pt_eta_corrZ -> Add(h_zjets_loose_pt_eta, -1);
  h_Ele_tight_pt_eta_corrZ -> Add(h_zjets_tight_pt_eta, -1);
    
  h_Ele_FR_pt_eta_corrZ -> Divide(h_Ele_tight_pt_eta_bin, h_Ele_loose_pt_eta_bin, 1., 1., "");

  h_Ele_FR_pt_eta_corrZ -> Draw();

 

}


