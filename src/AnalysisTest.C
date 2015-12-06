#define AnalysisTest_cxx
#include "../include/AnalysisTest.h"


//------------------------------------------------------------------------------
// AnalysisTest
//------------------------------------------------------------------------------
AnalysisTest::AnalysisTest(TTree* tree) : AnalysisCMS(tree)
{
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTest::Loop(TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {

    if (scut[j].Contains("TTDM/"))  continue;
    if (scut[j].Contains("WW/"))    continue;
    if (scut[j].Contains("monoH/")) continue;
    if (scut[j].Contains("WZ/"))    continue;

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=0; i<nchannel; i++) {

	TString suffix = "_" + schannel[i];


	// Common TH1 histograms
	//----------------------------------------------------------------------
	h_counterRaw[i][j][k] = new TH1D("h_counterRaw" + suffix, "",    3,    0,    3);
	h_counterLum[i][j][k] = new TH1D("h_counterLum" + suffix, "",    3,    0,    3);
	h_njet30    [i][j][k] = new TH1D("h_njet30"     + suffix, "",    7, -0.5,  6.5);
	h_nbjet15   [i][j][k] = new TH1D("h_nbjet15"    + suffix, "",    7, -0.5,  6.5);
	h_nvtx      [i][j][k] = new TH1D("h_nvtx"       + suffix, "",   50,    0,   50);
	h_deltarll  [i][j][k] = new TH1D("h_deltarll"   + suffix, "",  100,    0,    5);
        h_deltaphill[i][j][k] = new TH1D("h_deltaphill" + suffix, "",  100,    0,    5);
	h_met       [i][j][k] = new TH1D("h_met"        + suffix, "", 3000,    0, 3000);
        h_trkmet    [i][j][k] = new TH1D("h_trkmet"     + suffix, "", 3000,    0, 3000);
        h_mpmet     [i][j][k] = new TH1D("h_mpmet"      + suffix, "", 3000,    0, 3000);
	h_m2l       [i][j][k] = new TH1D("h_m2l"        + suffix, "", 3000,    0, 3000);
        h_mt1       [i][j][k] = new TH1D("h_mt1"        + suffix, "", 3000,    0, 3000);
        h_mt2       [i][j][k] = new TH1D("h_mt2"        + suffix, "", 3000,    0, 3000);
        h_mth       [i][j][k] = new TH1D("h_mth"        + suffix, "", 3000,    0, 3000);
        h_mc        [i][j][k] = new TH1D("h_mc"         + suffix, "", 3000,    0, 3000);
	h_ht        [i][j][k] = new TH1D("h_ht"         + suffix, "", 3000,    0, 3000);
        h_pt1       [i][j][k] = new TH1D("h_pt1"        + suffix, "", 3000,    0, 3000);
        h_pt2       [i][j][k] = new TH1D("h_pt2"        + suffix, "", 3000,    0, 3000);
        h_pt2l      [i][j][k] = new TH1D("h_pt2l"       + suffix, "", 3000,    0, 3000);
        h_ptww      [i][j][k] = new TH1D("h_ptww"       + suffix, "", 3000,    0, 3000);

	
	// Common TH2 histograms
	//----------------------------------------------------------------------
	h_metvar_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 2000, 0, 200);


	// WZ histograms
	//----------------------------------------------------------------------
	h_m3l       [i][j][k] = new TH1D("h_m3l"        + suffix, "", 2000, 0, 2000);
	h_mtw       [i][j][k] = new TH1D("h_mtw"        + suffix, "", 1000, 0, 1000);
	h_zl1pt     [i][j][k] = new TH1D("h_zl1pt"      + suffix, "", 1000, 0, 1000);
	h_zl2pt     [i][j][k] = new TH1D("h_zl2pt"      + suffix, "", 1000, 0, 1000);
	h_wlpt      [i][j][k] = new TH1D("h_wlpt"       + suffix, "", 1000, 0, 1000);
	h_wlzldeltar[i][j][k] = new TH1D("h_wlzldeltar" + suffix, "",  100, 0,    5);
      }
    }
  }


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();


    // Analysis
    //--------------------------------------------------------------------------
    if (!trigger) continue;

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    _nelectron = 0;  // Redefine _nelectron

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;
    _pt2l = ptll;


    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;

    LevelHistograms(Top_00_Has2Leptons, pass);

    pass &= (njet > 1);

    LevelHistograms(Top_01_Has2Jets, pass);

    pass &= (_nbjet15 > 0);

    LevelHistograms(Top_02_Has1BJet, pass);
  }


  EndJob();
}
