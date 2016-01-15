#define AnalysisMonoH_cxx
#include "../include/AnalysisMonoH.h"


//------------------------------------------------------------------------------
// AnalysisMonoH
//------------------------------------------------------------------------------
AnalysisMonoH::AnalysisMonoH(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisMonoH::Loop(TString analysis, TString filename, float luminosity)
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

	h_metvar_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 2000, 0, 200);
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

    pass &= (mll > 12.);
    pass &= (MET.Et() > 20.);
    pass &= (_nelectron == 1 || _nelectron != 1 && _metvar > 45. && fabs(mll - Z_MASS) > 15.);
    pass &= (_mpmet > 20.);
    pass &= (_passdphiveto);
    pass &= (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);
    pass &= (_nbjet15 == 0);
    pass &= (!_foundsoftmuon);

    bool pass_drll = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);
    FillLevelHistograms(MonoH_103_CR, pass && !pass_drll);
    
    pass &= (_mc < 100.);
    FillLevelHistograms(MonoH_100_Mc, pass);
    
    pass &= pass_drll;
    FillLevelHistograms(MonoH_101_DRll, pass);
    
    pass &= (_mpmet > 60.);
    FillLevelHistograms(MonoH_102_MpMet, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisMonoH::FillAnalysisHistograms(int ichannel,
					   int icut,
					   int ijet)
{
  h_metvar_m2l[ichannel][icut][ijet] ->Fill(_metvar, _m2l, _event_weight);

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisMonoH::FillLevelHistograms(int  icut,
					bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
