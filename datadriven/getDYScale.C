const float zmin =  76;  // [GeV]
const float zmax = 106;  // [GeV]


float errAB2  (float a, float err_a, float b, float err_b);
float errRatio(float a, float err_a, float b, float err_b);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getDYScale
//
//     ee data/dy scale = 1.26 +- 0.16
//     mm data/dy scale = 1.47 +- 0.14
//     ll data/dy scale = 1.40 +- 0.11
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getDYScale(float   metcut   = 45,
		TString analysis = "TTDM")
{
  TFile* file_data = new TFile("../rootfiles/nominal/" + analysis + "/01_Data.root",  "read");
  TFile* file_dy   = new TFile("../rootfiles/nominal/" + analysis + "/07_ZJets.root", "read");


  // Get MET (x-axis) vs m2l (y-axis) TH2D histograms
  //----------------------------------------------------------------------------
  TH2D* h2_ee_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_mm");
  TH2D* h2_em_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_em");

  TH2D* h2_ee_dy = (TH2D*)file_dy->Get(analysis + "/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_dy = (TH2D*)file_dy->Get(analysis + "/10_Rinout/h_metPfType1_m2l_mm");


  // Extract m2l without any MET cut
  //----------------------------------------------------------------------------
  TH1D* h_m2l_ee_data = (TH1D*)h2_ee_data->ProjectionY("h_m2l_ee_data");
  TH1D* h_m2l_mm_data = (TH1D*)h2_mm_data->ProjectionY("h_m2l_mm_data");


  // Extract m2l with MET > metcut
  //----------------------------------------------------------------------------
  int bin_metcut = h2_ee_data->GetXaxis()->FindBin(metcut);

  TH1D* h_m2l_ee_data_metcut = (TH1D*)h2_ee_data->ProjectionY("h_m2l_ee_data_metcut", bin_metcut);
  TH1D* h_m2l_mm_data_metcut = (TH1D*)h2_mm_data->ProjectionY("h_m2l_mm_data_metcut", bin_metcut);
  TH1D* h_m2l_em_data_metcut = (TH1D*)h2_em_data->ProjectionY("h_m2l_em_data_metcut", bin_metcut);

  TH1D* h_m2l_ee_dy_metcut = (TH1D*)h2_ee_dy->ProjectionY("h_m2l_ee_dy_metcut", bin_metcut);
  TH1D* h_m2l_mm_dy_metcut = (TH1D*)h2_mm_dy->ProjectionY("h_m2l_mm_dy_metcut", bin_metcut);


  // Initialize counters and errors
  //----------------------------------------------------------------------------
  int bin_zmin = h_m2l_ee_data->FindBin(zmin);
  int bin_zmax = h_m2l_ee_data->FindBin(zmax);

  float n_ee, err_ee = 0;
  float n_mm, err_mm = 0;

  float n_in_ee_data, err_in_ee_data = 0;
  float n_in_mm_data, err_in_mm_data = 0;
  float n_in_em_data, err_in_em_data = 0;

  float n_in_ee_dy, err_in_ee_dy = 0;
  float n_in_mm_dy, err_in_mm_dy = 0;


  // Sum entries and errors^2 from zmin to zmax
  //----------------------------------------------------------------------------
  for (int i=bin_zmin; i<=bin_zmax; i++)
    {
      n_ee += h_m2l_ee_data->GetBinContent(i);
      n_mm += h_m2l_mm_data->GetBinContent(i);

      err_ee += h_m2l_ee_data->GetSumw2()->At(i);
      err_mm += h_m2l_mm_data->GetSumw2()->At(i);

      n_in_ee_data += h_m2l_ee_data_metcut->GetBinContent(i);
      n_in_mm_data += h_m2l_mm_data_metcut->GetBinContent(i);
      n_in_em_data += h_m2l_em_data_metcut->GetBinContent(i);

      err_in_ee_data += h_m2l_ee_data_metcut->GetSumw2()->At(i);
      err_in_mm_data += h_m2l_mm_data_metcut->GetSumw2()->At(i);
      err_in_em_data += h_m2l_em_data_metcut->GetSumw2()->At(i);

      n_in_ee_dy += h_m2l_ee_dy_metcut->GetBinContent(i);
      n_in_mm_dy += h_m2l_mm_dy_metcut->GetBinContent(i);

      err_in_ee_dy += h_m2l_ee_dy_metcut->GetSumw2()->At(i);
      err_in_mm_dy += h_m2l_mm_dy_metcut->GetSumw2()->At(i);
    }


  // Don't forget to sqrt the errors
  //----------------------------------------------------------------------------
  err_ee         = sqrt(err_ee);
  err_mm         = sqrt(err_mm);
  err_in_ee_data = sqrt(err_in_ee_data);
  err_in_mm_data = sqrt(err_in_mm_data);
  err_in_em_data = sqrt(err_in_em_data);
  err_in_ee_dy   = sqrt(err_in_ee_dy);
  err_in_mm_dy   = sqrt(err_in_mm_dy);


  // Compute the scale factors
  //----------------------------------------------------------------------------
  float k_ee = 0.5 * sqrt(n_ee/n_mm);
  float k_mm = 0.5 * sqrt(n_mm/n_ee);

  float err_k_ee = sqrt((1 + n_ee/n_mm) / n_mm) / 4.0;
  float err_k_mm = sqrt((1 + n_mm/n_ee) / n_ee) / 4.0;

  float n_in_ee_dy_est = n_in_ee_data - k_ee * n_in_em_data;
  float n_in_mm_dy_est = n_in_mm_data - k_mm * n_in_em_data;

  float err_in_ee_dy_est = sqrt(err_in_ee_data*err_in_ee_data + errAB2(k_ee, err_k_ee, n_in_em_data, err_in_em_data));
  float err_in_mm_dy_est = sqrt(err_in_mm_data*err_in_mm_data + errAB2(k_mm, err_k_mm, n_in_em_data, err_in_em_data));

  float n_in_ll_dy_est = n_in_ee_dy_est + n_in_mm_dy_est;
  float n_in_ll_dy     = n_in_ee_dy     + n_in_mm_dy;

  float err_in_ll_dy_est = sqrt(err_in_ee_dy_est*err_in_ee_dy_est + err_in_mm_dy_est*err_in_mm_dy_est);
  float err_in_ll_dy     = sqrt(err_in_ee_dy    *err_in_ee_dy     + err_in_mm_dy    *err_in_mm_dy);

  float scale_ee = n_in_ee_dy_est / n_in_ee_dy;
  float scale_mm = n_in_mm_dy_est / n_in_mm_dy;
  float scale_ll = n_in_ll_dy_est / n_in_ll_dy;

  float err_scale_ee = errRatio(n_in_ee_dy_est, err_in_ee_dy_est, n_in_ee_dy, err_in_ee_dy);
  float err_scale_mm = errRatio(n_in_mm_dy_est, err_in_mm_dy_est, n_in_mm_dy, err_in_mm_dy);
  float err_scale_ll = errRatio(n_in_ll_dy_est, err_in_ll_dy_est, n_in_ll_dy, err_in_ll_dy);


  // Print the results
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" ee data/dy scale = %.2f +- %.2f\n", scale_ee, err_scale_ee);
  printf(" mm data/dy scale = %.2f +- %.2f\n", scale_mm, err_scale_mm);
  printf(" ll data/dy scale = %.2f +- %.2f\n", scale_ll, err_scale_ll);
  printf("\n");
}


//------------------------------------------------------------------------------
// errAB2 = err(a*b)^2
//------------------------------------------------------------------------------
float errAB2(float a, float err_a, float b, float err_b)
{
  float err = b*b*err_a*err_a + a*a*err_b*err_b;

  return err;
}


//------------------------------------------------------------------------------
// errRatio = err(a/b)
//------------------------------------------------------------------------------
float errRatio(float a, float err_a, float b, float err_b)
{
  float ratio = a/b;

  float err = (a/b) * sqrt((err_a*err_a)/(a*a) + (err_b*err_b)/(b*b));

  return err;
}
