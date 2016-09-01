#define AnalysisDY_cxx
#include "../include/AnalysisDY.h"


//------------------------------------------------------------------------------
// AnalysisDY
//------------------------------------------------------------------------------
AnalysisDY::AnalysisDY(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisDY::Loop(TString analysis, TString filename, float luminosity)
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


    // Analysis
    //--------------------------------------------------------------------------
    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel
    
    bool pass_supercut = (Lepton1.v.Pt() > 20.);
    pass_supercut &= (Lepton2.v.Pt() > 10.);
    
    pass_supercut &= (abs(Lepton1.flavour) == 13 || Lepton2.v.Pt() > 13.);
    pass_supercut &= (std_vector_lepton_pt -> at(2) < 10.);

    pass_supercut &= (_m2l > 20.);

    bool pass;

    pass = pass_supercut;

    pass &= (_m2l > 60.);
    pass &= (Lepton1.flavour * Lepton2.flavour == -11 * 11);
    pass &= (Lepton1.v.Pt() > 30. && Lepton2.v.Pt() > 20.);

    FillLevelHistograms(DY_01_DYee, pass);

    pass = pass_supercut;

    pass &= (_m2l > 60.);
    pass &= (Lepton1.flavour * Lepton2.flavour == -13 * 13);

    FillLevelHistograms(DY_02_DYmm, pass);

  }

  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillLevelHistograms(int  icut,
					  bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}
