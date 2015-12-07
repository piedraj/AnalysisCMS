#define AnalysisTTDM_cxx
#include "../include/AnalysisTTDM.h"


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
AnalysisTTDM::AnalysisTTDM(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTTDM::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {

    if (scut[j].Contains("Top/"))   continue;
    if (scut[j].Contains("WW/"))    continue;
    if (scut[j].Contains("monoH/")) continue;
    if (scut[j].Contains("WZ/"))    continue;

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=ee; i<=ll; i++) {

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);

	h_test[i][j][k] = new TH1D("h_test" + suffix, "", 300, 0, 300);
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

    _nlepton   = 2;  // Redefine _nlepton
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

    FillLevelHistograms(TTDM_00_Has2Leptons, pass);

    bool pass_sf = (_nelectron != 1 && fabs(mll - Z_MASS) > 15.);
    bool pass_df = (_nelectron == 1);

    pass &= (mll > 20.);
    pass &= (pass_sf || pass_df);

    FillLevelHistograms(TTDM_01_ZVeto, pass);

    pass &= (njet > 1);

    if (njet < 2) continue;

    FillLevelHistograms(TTDM_02_Has2Jets, pass);

    pass &= (Lepton1.v.Pt() + Lepton2.v.Pt() > 120.);

    FillLevelHistograms(TTDM_03_LepPtSum, pass);

    pass &= (AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt() < 400.);

    FillLevelHistograms(TTDM_04_JetPtSum, pass);

    pass &= (Lepton1.v.DeltaPhi(Lepton2.v) < 2.);

    FillLevelHistograms(TTDM_05_LepDeltaPhi, pass);

    pass &= (MET.Et() > 320.);

    FillLevelHistograms(TTDM_06_MET, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillAnalysisHistograms(int ichannel,
					  int icut,
					  int ijet)
{
  h_test[ichannel][icut][ijet]->Fill(_m2l, _event_weight);

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillLevelHistograms(int  icut,
				       bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
