const float zmin =  76;  // [GeV]
const float zmax = 106;  // [GeV]


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getDYScale
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getDYScale()
{
  TFile* file_data = new TFile("../rootfiles/nominal/TTDM/01_Data.root",  "read");
  TFile* file_dy   = new TFile("../rootfiles/nominal/TTDM/07_ZJets.root", "read");


  // Get MET (x-axis) vs m2l (y-axis) TH2D histograms
  //----------------------------------------------------------------------------
  TH2D* h2_ee_data = (TH2D*)file_data->Get("TTDM/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_data = (TH2D*)file_data->Get("TTDM/10_Rinout/h_metPfType1_m2l_mm");
  TH2D* h2_em_data = (TH2D*)file_data->Get("TTDM/10_Rinout/h_metPfType1_m2l_em");

  TH2D* h2_ee_dy = (TH2D*)file_dy->Get("TTDM/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_dy = (TH2D*)file_dy->Get("TTDM/10_Rinout/h_metPfType1_m2l_mm");


  // Projections on the y-axis
  //----------------------------------------------------------------------------
  TH1D* h_m2l_ee_data = (TH1D*)h2_ee_data->ProjectionY("h_m2l_ee_data");
  TH1D* h_m2l_mm_data = (TH1D*)h2_mm_data->ProjectionY("h_m2l_mm_data");

  int nbinsx = h2_ee_data->GetXaxis()->GetNbins();

  TH1D* h_m2l_ee_data_metcut = (TH1D*)h2_ee_data->ProjectionY("h_m2l_ee_data_metcut", nbinsx, nbinsx+1);
  TH1D* h_m2l_mm_data_metcut = (TH1D*)h2_mm_data->ProjectionY("h_m2l_mm_data_metcut", nbinsx, nbinsx+1);
  TH1D* h_m2l_em_data_metcut = (TH1D*)h2_em_data->ProjectionY("h_m2l_em_data_metcut", nbinsx, nbinsx+1);

  TH1D* h_m2l_ee_dy_metcut = (TH1D*)h2_ee_dy->ProjectionY("h_m2l_ee_dy_metcut", nbinsx, nbinsx+1);
  TH1D* h_m2l_mm_dy_metcut = (TH1D*)h2_mm_dy->ProjectionY("h_m2l_mm_dy_metcut", nbinsx, nbinsx+1);


  // Get the numbers
  //----------------------------------------------------------------------------
  int bin_zmin = h_m2l_ee_data->FindBin(zmin);
  int bin_zmax = h_m2l_ee_data->FindBin(zmax);

  float n_ee = h_m2l_ee_data->Integral(bin_zmin, bin_zmax);
  float n_mm = h_m2l_mm_data->Integral(bin_zmin, bin_zmax);

  float n_in_ee_data = h_m2l_ee_data_metcut->Integral(bin_zmin, bin_zmax);
  float n_in_mm_data = h_m2l_mm_data_metcut->Integral(bin_zmin, bin_zmax);
  float n_in_em_data = h_m2l_em_data_metcut->Integral(bin_zmin, bin_zmax);

  float n_in_ee_dy = h_m2l_ee_dy_metcut->Integral(bin_zmin, bin_zmax);
  float n_in_mm_dy = h_m2l_mm_dy_metcut->Integral(bin_zmin, bin_zmax);

  float scale_ee = (n_in_ee_data - 0.5 * sqrt(n_ee/n_mm) * n_in_em_data) / n_in_ee_dy;
  float scale_mm = (n_in_mm_data - 0.5 * sqrt(n_mm/n_ee) * n_in_em_data) / n_in_mm_dy;

  printf("\n");
  printf(" ee data/dy scale = %.2f +- XX\n", scale_ee);
  printf(" mm data/dy scale = %.2f +- XX\n", scale_mm);
  printf("\n");
}
