#define AnalysisTTDM_cxx
#include "../include/AnalysisTTDM.h"


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
AnalysisTTDM::AnalysisTTDM(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTTDM::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  root_output->cd();

  for (int j=0; j<ncut; j++) {

    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=ee; i<=ll; i++) {

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);
      }
    }
  }

  root_output->cd();


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();


    // LHE weights for QCD and PDF systematic uncertainties
    //--------------------------------------------------------------------------
    GetSumOfWeightsLHE();


    // Analysis
    //--------------------------------------------------------------------------
    if (!_ismc && run > 258750) continue;  // Luminosity for any blinded analysis

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 10.) continue;
    if (Lepton2.v.Pt() < 10.) continue;

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel

    bool pass;


    // AN-15-305 analysis
    //--------------------------------------------------------------------------
    pass = true;

    pass &= (Lepton1.v.Pt() > 20.);
    pass &= (Lepton2.v.Pt() > 20.);
    pass &= (_m2l > 20.);
    pass &= (njet > 1);
    pass &= (_nbjet30medium > 0);
    pass &= (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);
    pass &= (_nelectron == 1 || MET.Et() > 40.);

    FillLevelHistograms(TTDM_04_AN15305, pass);


    // IFCA ttDM analysis
    //--------------------------------------------------------------------------
    if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;

    pass = true;

    pass &= (Lepton1.v.Pt() > 30.);

    FillLevelHistograms(TTDM_00_Has2Leptons, pass);

    pass &= (_m2l > 20.);
    pass &= (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);

    FillLevelHistograms(TTDM_01_ZVeto, pass);

    pass &= (MET.Et() > 50.);

    FillLevelHistograms(TTDM_02_MET50, pass);

    if (_saveminitree && pass) minitree->Fill();

    pass &= (njet > 1);

    FillLevelHistograms(TTDM_03_Preselection, pass);

    
    // AN-16-105 analysis
    //--------------------------------------------------------------------------
    pass &= (_nbjet30medium > 0);
    pass &= (_dphillmet > 1.2);

    FillLevelHistograms(TTDM_05_AN16105, pass);
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

  //  FillAnalysisHistograms(_channel, icut, _jetbin);
  //  FillAnalysisHistograms(_channel, icut, njetbin);
}
