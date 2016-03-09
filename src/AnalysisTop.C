#define AnalysisTop_cxx
#include "../include/AnalysisTop.h"


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
AnalysisTop::AnalysisTop(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTop::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

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

	h_test[i][j][k] = new TH1D("h_test" + suffix, "", 300, 0, 300);
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
    if (!trigger) continue;

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    _nelectron = 0;

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

    FillLevelHistograms(Top_00_Has2Leptons, pass);

    pass &= (njet > 1);

    FillLevelHistograms(Top_01_Has2Jets, pass);

    pass &= (_nbjet20loose > 0);

    FillLevelHistograms(Top_02_Has1BJet, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisTop::FillAnalysisHistograms(int ichannel,
					 int icut,
					 int ijet)
{
  h_test[ichannel][icut][ijet]->Fill(_m2l, _event_weight);

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisTop::FillLevelHistograms(int  icut,
				      bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
