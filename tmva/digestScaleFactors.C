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
  float sf_top_av  = 0;
  float sf_top_av2 = 0;
  float sf_ww_av   = 0;
  float sf_ww_av2  = 0;

  for (int i=0; i<ntrials; i++)
    {
      sf_top_av += (sf_top_near[i] + sf_top_far[i]);
      sf_ww_av  += (sf_ww_near [i] + sf_ww_far [i]);
 
      sf_top_av2 += (sf_top_near[i]*sf_top_near[i] + sf_top_far[i]*sf_top_far[i]);
      sf_ww_av2  += (sf_ww_near [i]*sf_ww_near [i] + sf_ww_far [i]*sf_ww_far [i]);
    }


  // Average
  sf_top_av /= (2. * ntrials);
  sf_ww_av  /= (2. * ntrials);


  // Average of squares
  sf_top_av2 /= (2. * ntrials);
  sf_ww_av2  /= (2. * ntrials);


  // Spread taken as systematic
  float rms_top = sqrt(sf_top_av2 - sf_top_av*sf_top_av);
  float rms_ww  = sqrt(sf_ww_av2  - sf_ww_av *sf_ww_av);


  // Weighted average of near and far for the first box
  float er_top = 1./(er_top_near[0]*er_top_near[0]) + 1./(er_top_far[0]*er_top_far[0]);
  float er_ww  = 1./(er_ww_near [0]*er_ww_near [0]) + 1./(er_ww_far [0]*er_ww_far [0]);

  er_top = 1./sqrt(er_top);
  er_ww  = 1./sqrt(er_ww);

  float sf_top = (sf_top_near[0]/(er_top_near[0]*er_top_near[0]) + sf_top_far[0]/(er_top_far[0]*er_top_far[0]))*er_top*er_top;
  float sf_ww  = (sf_ww_near [0]/(er_ww_near [0]*er_ww_near [0]) + sf_ww_far [0]/(er_ww_far [0]*er_ww_far [0]))*er_ww*er_ww;


  // Print
  printf("\n");
  printf(" SF ttbar = %.2f +- %.2f (stat) +- %.2f (syst)\n", sf_top, er_top,rms_top);
  printf(" SF WW    = %.2f +- %.2f (stat) +- %.2f (syst)\n", sf_ww,  er_ww, rms_ww);
  printf("\n");
}
