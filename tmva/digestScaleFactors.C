const int ntrials = 6;

float sf_top_near[ntrials] = { 0.90,  0.84,  0.89,  0.83,  0.89,  0.83};
float sf_ww_near [ntrials] = { 0.41,  0.43,  0.87,  0.96,  0.70,  0.77};
float er_top_near[ntrials] = { 0.06,  0.04,  0.06,  0.04,  0.06,  0.04};
float er_ww_near [ntrials] = { 0.57,  0.57,  0.32,  0.32,  0.29,  0.29};
float cov_near   [ntrials] = {-0.16, -0.26, -0.33, -0.32, -0.28, -0.27};

float sf_top_far[ntrials] = { 1.00,  0.90,  1.00,  0.90,  1.00,  0.90};
float er_top_far[ntrials] = { 0.05,  0.04,  0.05,  0.04,  0.05,  0.04};
float sf_ww_far [ntrials] = { 0.25,  0.29,  0.02,  0.17,  0.20,  0.26};
float er_ww_far [ntrials] = { 0.17,  0.17,  0.30,  0.30,  0.15,  0.15};
float cov_far   [ntrials] = {-0.13, -0.15, -0.30, -0.31, -0.22, -0.21};


//------------------------------------------------------------------------------
//
// digestScaleFactors
//
//------------------------------------------------------------------------------
void digestScaleFactors()
{
  float sf_top = 0;
  float sf_ww  = 0;
  float er_top = 0;
  float er_ww  = 0;

  for (int i=0; i<ntrials; i++)
    {
      sf_top += sf_top_near[i];
      sf_ww  += sf_ww_near [i];

      er_top += (er_top_near[i]*er_top_near[i]);
      er_top += (er_top_far [i]*er_top_far [i]);

      er_ww += (er_ww_near[i]*er_ww_near[i]);
      er_ww += (er_ww_far [i]*er_ww_far [i]);
    }

  sf_top /= ntrials;
  sf_ww  /= ntrials;
  
  er_top = sqrt(er_top) / ntrials;
  er_ww  = sqrt(er_ww)  / ntrials;

  printf("\n");
  printf(" SF ttbar = %.2f +- %.2f\n", sf_top, er_top);
  printf(" SF WW    = %.2f +- %.2f\n", sf_ww,  er_ww);
  printf("\n");
}
