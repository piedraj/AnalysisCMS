const int ntrials = 6;

float sf_top_near[ntrials] = { 0.96,  0.93,  0.96,  0.93,  0.96,  0.93};
float er_top_near[ntrials] = { 0.06,  0.04,  0.06,  0.04,  0.06,  0.04};
float sf_ww_near [ntrials] = { 0.84,  0.86,  1.00,  1.06,  0.93,  0.97};
float er_ww_near [ntrials] = { 0.47,  0.47,  0.34,  0.34,  0.29,  0.28};
float cov_near   [ntrials] = {-0.14, -0.22, -0.33, -0.32, -0.27, -0.26};

float sf_top_far[ntrials] = { 1.10,  1.00,  1.10,  1.00,  1.10,  1.00};
float er_top_far[ntrials] = { 0.05,  0.04,  0.05,  0.04,  0.05,  0.04};
float sf_ww_far [ntrials] = { 0.53,  0.56,  0.20,  0.36,  0.46,  0.52};
float er_ww_far [ntrials] = { 0.17,  0.17,  0.32,  0.32,  0.16,  0.15};
float cov_far   [ntrials] = {-0.13, -0.14, -0.30, -0.31, -0.22, -0.21};


//------------------------------------------------------------------------------
//
// digestScaleFactors
//
//   SF ttbar = 1.04 +- 0.04 (stat) +- 0.06 (syst) = 1.04 +- 0.07
//   SF WW    = 0.57 +- 0.16 (stat) +- 0.27 (syst) = 0.57 +- 0.32
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
  printf(" SF ttbar = %.2f +- %.2f (stat) +- %.2f (syst) = %.2f +- %.2f\n", sf_top, er_top, rms_top, sf_top, sqrt(er_top*er_top + rms_top*rms_top));
  printf(" SF WW    = %.2f +- %.2f (stat) +- %.2f (syst) = %.2f +- %.2f\n", sf_ww,  er_ww,  rms_ww,  sf_ww,  sqrt(er_ww *er_ww  + rms_ww *rms_ww));
  printf("\n");
}
