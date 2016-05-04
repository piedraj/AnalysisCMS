const float zmin =  76;  // [GeV]
const float zmax = 106;  // [GeV]


float errAB2  (float a, float err_a, float b, float err_b);
float errRatio(float a, float err_a, float b, float err_b);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// This macro computes the DY scale factor for the ee and mm channels. It needs
// as input a set of two-dimensional histograms, with MET in the x-axis and m2l
// in the y-axis. These histograms, for both data and DY, have all the analysis
// cuts (taken from AN-15-305) with the exception of |m2l - mZ| > 15 GeV and
// MET > 40 GeV. The scale factor for the ee channel (the procedure is identical
// for the mm channel) is computed from equations (1) and (2). We have assumed
// negligible the peaking processes (WZ and ZZ).
//
//    k_ee     = 0.5 * sqrt(n_ee / n_mm);                            (1)
//    scale_ee = (n_in_ee_data - k_ee * n_in_em_data) / n_in_ee_dy;  (2)
//
// And these are the results that we obtain for MET > 40 GeV.
//   
//                |         ee               mm
//   -------------+-----------------------------------
//    n(data)     |    2998 +- 55       6323 +- 80   
//    k           |   0.344 +- 0.004   0.726 +- 0.008
//    Nin(ZZ)     |    3.92 +- 0.06     7.25 +- 0.08 
//    Nin(DY)     |   282.0 +- 18.3    612.0 +- 26.0 
//    Nin(est)    |   383.4 +- 30.3    812.9 +- 47.9 
//    SF(est/DY)  |   1.360 +- 0.139   1.328 +- 0.096
//   -------------+-----------------------------------
//    Nout(DY)    |    31.5 +- 4.3      54.6 +- 5.1  
//    Nout(est)   |    42.8 +- 7.3      72.5 +- 8.6  
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getDYScale(float   metcut   = 40,
		TString analysis = "TTDM")
{
  TFile* file_data = new TFile("../rootfiles/nominal/" + analysis + "/01_Data.root",  "read");
  TFile* file_dy   = new TFile("../rootfiles/nominal/" + analysis + "/07_ZJets.root", "read");
  TFile* file_zz   = new TFile("../rootfiles/nominal/" + analysis + "/03_ZZ.root",    "read");


  // Get MET (x-axis) vs m2l (y-axis) TH2D histograms
  //----------------------------------------------------------------------------
  TH2D* h2_ee_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_mm");
  TH2D* h2_em_data = (TH2D*)file_data->Get(analysis + "/10_Rinout/h_metPfType1_m2l_em");

  TH2D* h2_ee_dy = (TH2D*)file_dy->Get(analysis + "/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_dy = (TH2D*)file_dy->Get(analysis + "/10_Rinout/h_metPfType1_m2l_mm");

  TH2D* h2_ee_zz = (TH2D*)file_zz->Get(analysis + "/10_Rinout/h_metPfType1_m2l_ee");
  TH2D* h2_mm_zz = (TH2D*)file_zz->Get(analysis + "/10_Rinout/h_metPfType1_m2l_mm");


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

  TH1D* h_m2l_ee_zz_metcut = (TH1D*)h2_ee_zz->ProjectionY("h_m2l_ee_zz_metcut", bin_metcut);
  TH1D* h_m2l_mm_zz_metcut = (TH1D*)h2_mm_zz->ProjectionY("h_m2l_mm_zz_metcut", bin_metcut);


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

  float n_in_ee_zz, err_in_ee_zz = 0;
  float n_in_mm_zz, err_in_mm_zz = 0;

  float n_out_ee_dy, err_out_ee_dy = 0;
  float n_out_mm_dy, err_out_mm_dy = 0;


  // Sum entries and errors^2
  //----------------------------------------------------------------------------
  for (int i=0; i<=h_m2l_ee_data->GetNbinsX()+1; i++)
    {
      // Inside the Z-peak
      if (i >= bin_zmin && i <= bin_zmax)
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

	  n_in_ee_zz += h_m2l_ee_zz_metcut->GetBinContent(i);
	  n_in_mm_zz += h_m2l_mm_zz_metcut->GetBinContent(i);

	  err_in_ee_zz += h_m2l_ee_zz_metcut->GetSumw2()->At(i);
	  err_in_mm_zz += h_m2l_mm_zz_metcut->GetSumw2()->At(i);
	}
      // Outside the Z-peak
      else
	{
	  n_out_ee_dy += h_m2l_ee_dy_metcut->GetBinContent(i);
	  n_out_mm_dy += h_m2l_mm_dy_metcut->GetBinContent(i);

	  err_out_ee_dy += h_m2l_ee_dy_metcut->GetSumw2()->At(i);
	  err_out_mm_dy += h_m2l_mm_dy_metcut->GetSumw2()->At(i);
	}
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
  err_in_ee_zz   = sqrt(err_in_ee_zz);
  err_in_mm_zz   = sqrt(err_in_mm_zz);
  err_out_ee_dy  = sqrt(err_in_ee_dy);
  err_out_mm_dy  = sqrt(err_in_mm_dy);


  // Compute the scale factors
  //----------------------------------------------------------------------------
  float k_ee = 0.5 * sqrt(n_ee / n_mm);
  float k_mm = 0.5 * sqrt(n_mm / n_ee);

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

  float n_out_ee_dy_est = scale_ee * n_out_ee_dy;
  float n_out_mm_dy_est = scale_mm * n_out_mm_dy;

  float err_out_ee_dy_est =sqrt(errAB2(scale_ee, err_scale_ee, n_out_ee_dy, err_out_ee_dy));
  float err_out_mm_dy_est =sqrt(errAB2(scale_mm, err_scale_mm, n_out_mm_dy, err_out_mm_dy));


  // Print the results
  //----------------------------------------------------------------------------
  printf("\n Results for MET > %.0f GeV\n\n", metcut);
  printf("             |         ee               mm\n");
  printf("-------------+-----------------------------------\n");
  printf(" n(data)     |  %6.0f +- %-5.0f  %6.0f +- %-5.0f\n", n_ee,            err_ee,            n_mm,            err_mm);
  printf(" k           |  %6.3f +- %-5.3f  %6.3f +- %-5.3f\n", k_ee,            err_k_ee,          k_mm,            err_k_mm);
  printf(" Nin(ZZ)     |  %6.2f +- %-5.2f  %6.2f +- %-5.2f\n", n_in_ee_zz,      err_in_ee_zz,      n_in_mm_zz,      err_in_mm_zz);
  printf(" Nin(DY)     |  %6.1f +- %-5.1f  %6.1f +- %-5.1f\n", n_in_ee_dy,      err_in_ee_dy,      n_in_mm_dy,      err_in_mm_dy);
  printf(" Nin(est)    |  %6.1f +- %-5.1f  %6.1f +- %-5.1f\n", n_in_ee_dy_est,  err_in_ee_dy_est,  n_in_mm_dy_est,  err_in_mm_dy_est);
  printf(" SF(est/DY)  |  %6.3f +- %-5.3f  %6.3f +- %-5.3f\n", scale_ee,        err_scale_ee,      scale_mm,        err_scale_mm);
  printf("-------------+-----------------------------------\n");
  printf(" Nout(DY)    |  %6.1f +- %-5.1f  %6.1f +- %-5.1f\n", n_out_ee_dy,     err_out_ee_dy,     n_out_mm_dy,     err_out_mm_dy);
  printf(" Nout(est)   |  %6.1f +- %-5.1f  %6.1f +- %-5.1f\n", n_out_ee_dy_est, err_out_ee_dy_est, n_out_mm_dy_est, err_out_mm_dy_est);
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
