
void computeFR() {

  TFile*  f         = new TFile  ("../rootfiles/FR/Run2015D_16Dec2015_DoubleMuon_0000__part0.root","read");
  
  TH1D*   h_loose = (TH1D*)f -> Get("FR/00_QCD/h_loose_m");
  TH1D*   h_tight = (TH1D*)f -> Get("FR/00_QCD/h_tight_m");
  
  TH2F*   h_pt_eta = (TH2F*)f -> Get("FR/00_QCD/h_pt_eta_m");

  int loose = h_loose -> Integral();
  int tight = h_tight -> Integral();
  
  float fakeRate = float(tight)/loose;
  
  //  printf("Number of loose muons = %i \n", loose);
  //  printf("Number of tight muons = %i \n", tight);

  //  printf("Fake rate of muons = %f \n", fakeRate);

  h_pt_eta -> Draw();

}
