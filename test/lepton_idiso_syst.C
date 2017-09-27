//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l lepton_idiso_syst.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Source code
//
// https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/python/variables/multiIdisoScaleFactors.py#L556-L559
// https://github.com/latinos/LatinoAnalysis/blob/master/Gardener/python/data/formulasToAdd_MC.py#L80-L90 
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const TString directory = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC/";


void lepton_idiso_syst(TString filename = "latino_WWTo2L2Nu.root")
{
  // Read
  //----------------------------------------------------------------------------
  TFile* _file0 = TFile::Open(directory + filename);

  TTree* latino = (TTree*)_file0->Get("latino");


  // Selection
  //----------------------------------------------------------------------------
  TCut lepton_pt = "std_vector_lepton_pt[0] > 25. && std_vector_lepton_pt[1] > 20. && std_vector_lepton_pt[2] < 10.";
  TCut electrons = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -121";
  TCut muons     = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -169";


  // Draw ele
  //----------------------------------------------------------------------------
  TCanvas* c_ele = new TCanvas("c_ele", "electrons");

  TH1F* h_ele = new TH1F("h_ele", "electron idiso syst (%)", 50, 0, 5);

  h_ele->SetFillColor(kAzure);
  h_ele->SetLineColor(kAzure);

  latino->Draw("0.5*1e2*(LepSF2l__ele_cut_WP_Tight80X__Up-LepSF2l__ele_cut_WP_Tight80X__Do)>>h_ele", lepton_pt && electrons);

  c_ele->SaveAs("electron_idiso_syst.png");


  // Draw mu
  //----------------------------------------------------------------------------
  TCanvas* c_mu = new TCanvas("c_mu", "muons");

  TH1F* h_mu = new TH1F("h_mu", "muon idiso syst (%)", 50, 0, 5);

  h_mu->SetFillColor(kAzure);
  h_mu->SetLineColor(kAzure);

  latino->Draw("0.5*1e2*(LepSF2l__mu_cut_Tight80x__Up-LepSF2l__mu_cut_Tight80x__Do)>>h_mu", lepton_pt && muons);

  c_mu->SaveAs("muon_idiso_syst.png");


  // Print
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" <electron idiso syst> = %.2f%%\n", h_ele->GetMean());
  printf(" <muon     idiso syst> = %.2f%%\n", h_mu ->GetMean());
  printf("\n");
}
