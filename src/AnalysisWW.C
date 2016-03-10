#define AnalysisWW_cxx
#include "../include/AnalysisWW.h"


//------------------------------------------------------------------------------
// AnalysisWW
//------------------------------------------------------------------------------
AnalysisWW::AnalysisWW(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWW::Loop(TString analysis, TString filename, float luminosity)
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

	h_dphilmet1[i][j][k] = new TH1D("h_dphilmet1"   + suffix, "", 1000, 0,   10);
	h_dphilmet2[i][j][k] = new TH1D("h_dphilmet2"   + suffix, "", 1000, 0,   10);
	h_metphi   [i][j][k] = new TH1D("h_metphi"      + suffix, "", 1000, 0,   10);
	h_lepphi1  [i][j][k] = new TH1D("h_lepphi1"     + suffix, "", 1000, 0,   10);
	h_lepphi2  [i][j][k] = new TH1D("h_lepphi2"     + suffix, "", 1000, 0,   10);
	h_fullpmet [i][j][k] = new TH1D("h_fullpmet"    + suffix, "", 1000, 0, 1000);
	h_trkpmet  [i][j][k] = new TH1D("h_trkpmet"     + suffix, "", 1000, 0, 1000);
	h_jetpt1   [i][j][k] = new TH1D("h_jetpt1"      + suffix, "", 1000, 0, 1000);

	h_metvar_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 2000, 0, 200);
      }
    }
  }

  root_output->cd();
  

  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries; jentry++) {

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

    // This requirement should be applied on a loose lepton
    if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;

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

    FillLevelHistograms(WW_00_Has2Leptons, pass);

    pass &= (mll > 12.);
    FillLevelHistograms(WW_01_Mll, pass);

    pass &= (MET.Et() > 20.);
    FillLevelHistograms(WW_02_PfMet, pass);

    bool pass_zwindow = (fabs(mll - Z_MASS) > 15.);
    bool pass_zveto   = (_nelectron == 1 || _nelectron != 1 && _metvar > 45. && pass_zwindow);

    FillLevelHistograms(WW_03_ZVeto, pass && pass_zveto);

    pass &= (mpmet > 20.);
    FillLevelHistograms(WW_04_MpMet, pass && pass_zveto);

    pass &= (_passdphiveto);
    FillLevelHistograms(WW_05_DPhiVeto, pass && pass_zveto);

    bool pass_ptll = (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);

    pass &= pass_ptll;
    FillLevelHistograms(WW_06_Ptll, pass && pass_zveto);

    pass &= (_nbjet20loose == 0);
    FillLevelHistograms(WW_07_BVeto, pass && pass_zveto);

    bool pass_topCR = (mll > 50 && pass_zveto && MET.Et() > 20 && nbjet > 0);
    FillLevelHistograms(WW_50_TopCR, pass_topCR);

    bool pass_dyttCR = (nbjet == 0 && mll > 30 && mll < 80 && MET.Et() < 60);
    FillLevelHistograms(WW_60_DYttCR, pass_dyttCR);

    bool pass_dyCR = (nbjet == 0 && !pass_zwindow);
    FillLevelHistograms(WW_70_DYCR, pass_dyCR);


    //    bool pass_ht = (_ht < 250.);
    //    FillLevelHistograms(WW_09_Ht, pass && pass_zveto && pass_ht);
    
    //    FillLevelHistograms(WW_10_DY, pass);  // Data-driven DY


    // DY control region
    //--------------------------------------------------------------------------
    /*
    FillLevelHistograms(WW_11_ZWindow, pass_zwindow);

    pass_zwindow &= (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);
    FillLevelHistograms(WW_12_ZWindowPtll, pass_zwindow);

    for (UInt_t j=0; j<12; ++j)
      {
	bool jet = (std_vector_jet_pt->at(0) < 25. + j && fabs(std_vector_jet_eta->at(0)) < 4.7);

	FillLevelHistograms(WW_18_ZWindow25 + j, pass_zwindow && jet);
      }

    //    pass_zwindow &= (mpmet > 20.);
    //    FillLevelHistograms(WW_13_ZWindowMpMet, pass_zwindow);

    pass_zwindow &= (MET.Et() > 20.);
    FillLevelHistograms(WW_14_ZWindowPfMet, pass_zwindow);

    pass_zwindow &= (_nbjet20loose == 0);
    FillLevelHistograms(WW_15_ZWindowBVeto, pass_zwindow);

    pass_zwindow &= (!_foundsoftmuon);
    FillLevelHistograms(WW_16_ZWindowSoftMu, pass_zwindow);

    pass_zwindow &= (mpmet > 20.);
    FillLevelHistograms(WW_17_ZCR, pass_zwindow);
    */


    // monoH selection - on top of WW (excluding Ht selection)
    //--------------------------------------------------------------------------
    /*
    bool pass_monoh = (pass && pass_zveto);
    bool pass_drll  = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);

    FillLevelHistograms(WW_103_MonoH_CR, pass_monoh && !pass_drll);
    
    pass_monoh &= (_mc < 100.);
    FillLevelHistograms(WW_100_MonoH_Mc, pass_monoh);
    
    pass_monoh &= pass_drll;
    FillLevelHistograms(WW_101_MonoH_DRll, pass_monoh);
    
    pass_monoh &= (mpmet > 60.);
    FillLevelHistograms(WW_102_MonoH_MpMet, pass_monoh);
    */
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisWW::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{
  h_dphilmet1[ichannel][icut][ijet]->Fill(dphilmet1,                    _event_weight);
  h_dphilmet2[ichannel][icut][ijet]->Fill(dphilmet2,                    _event_weight);
  h_metphi   [ichannel][icut][ijet]->Fill(MET.Phi(),                    _event_weight);
  h_fullpmet [ichannel][icut][ijet]->Fill(_fullpmet,                    _event_weight);
  h_trkpmet  [ichannel][icut][ijet]->Fill(_trkpmet,                     _event_weight);
  h_jetpt1   [ichannel][icut][ijet]->Fill(std_vector_jet_pt->at(0),     _event_weight);
  h_lepphi1  [ichannel][icut][ijet]->Fill(std_vector_lepton_phi->at(0), _event_weight);
  h_lepphi2  [ichannel][icut][ijet]->Fill(std_vector_lepton_phi->at(1), _event_weight);

  h_metvar_m2l[ichannel][icut][ijet]->Fill(_metvar, _m2l, _event_weight);

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisWW::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
