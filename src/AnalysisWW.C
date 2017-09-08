#define AnalysisWW_cxx
#include "../include/AnalysisWW.h"


//------------------------------------------------------------------------------
// AnalysisWW
//------------------------------------------------------------------------------
AnalysisWW::AnalysisWW(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWW::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
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

	h_fullpmet[i][j][k] = new TH1D("h_fullpmet" + suffix, "", 3000, 0, 3000);
	h_trkpmet [i][j][k] = new TH1D("h_trkpmet"  + suffix, "", 3000, 0, 3000);
	h_htjets  [i][j][k] = new TH1D("h_htjets"   + suffix, "", 3000, 0, 3000);
	h_htnojets[i][j][k] = new TH1D("h_htnojets" + suffix, "", 3000, 0, 3000);
	h_mllstar [i][j][k] = new TH1D("h_mllstar"  + suffix, "", 3000, 0, 3000);
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

    bool pass_zwindow = (_nelectron == 1 || fabs(mll - Z_MASS) > 15.);
    FillLevelHistograms(WW_03_ZVeto, pass && pass_zwindow);

    pass &= ((_nelectron == 1 && mpmet > 20) || (_nelectron != 1 && mpmet > 45));
    FillLevelHistograms(WW_04_MpMet, pass && pass_zwindow);

    pass &= (_passdphiveto);
    FillLevelHistograms(WW_05_DPhiVeto, pass && pass_zwindow);

    bool pass_ptll = ((_nelectron == 1 && ptll > 30) || (_nelectron != 1 && ptll > 45));

    pass &= pass_ptll;
    FillLevelHistograms(WW_06_Ptll, pass && pass_zwindow);

    bool passBvetoLoose = pass && (_nbjet20cmvav2l == 0);
    FillLevelHistograms(WW_07_BVetoLoose, passBvetoLoose && pass_zwindow);

    bool passHt = passBvetoLoose && (_ht < 200);
    FillLevelHistograms(WW_08_Ht, passHt && pass_zwindow);

    // bool passBvetoMedium = pass && (_nbjet20cmvav2m == 0);
    // FillLevelHistograms(WW_08_BVetoMedium, passBvetoMedium && pass_zwindow);

    // bool passBvetoTight = pass && (_nbjet20cmvav2t == 0);
    // FillLevelHistograms(WW_09_BVetoTight, passBvetoTight && pass_zwindow);

    // bool passBvetoLooseOld = pass && (_nbjet15csvv2l == 0);
    // FillLevelHistograms(WW_10_BVetoLooseOld, passBvetoLooseOld && pass_zwindow);

    // bool passBvetoMediumOld = pass && (_nbjet15csvv2m == 0);
    // FillLevelHistograms(WW_11_BVetoMediumOld, passBvetoMediumOld && pass_zwindow);

    // bool passBvetoTightOld = pass && (_nbjet15csvv2t == 0);
    // FillLevelHistograms(WW_12_BVetoTightOld, passBvetoTightOld && pass_zwindow);

    // bool pass_topCR = (mll > 50 && pass_zwindow && MET.Et() > 20 && _nbjet20cmvav2l > 0);
    // FillLevelHistograms(WW_50_TopCR, pass_topCR);

    // bool pass_dyttCR = (_nbjet20cmvav2l == 0 && mll > 30 && mll < 80 && MET.Et() < 60);
    // FillLevelHistograms(WW_60_DYttCR, pass_dyttCR);

    // //    bool pass_dyCR = (_nbjet20cmvav2l == 0 && !pass_zwindow);
    // bool pass_dyCR = (!pass_zwindow && ptll > 45 && MET.Et() > 20 && mpmet > 20 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(WW_70_DYCR, pass_dyCR);

    // bool pass_dyCR_nompmet = (!pass_zwindow && ptll > 45 && MET.Et() > 20 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(WW_71_DYCR_NoMpMet, pass_dyCR_nompmet);

    // bool pass_dyCR_justptll = (!pass_zwindow && ptll > 45 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(WW_72_DYCR_JustPtll, pass_dyCR_nompmet);

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

    pass_zwindow &= (_nbjet20cmvav2l == 0);
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
  h_fullpmet[ichannel][icut][ijet]->Fill(_fullpmet, _event_weight);
  h_trkpmet [ichannel][icut][ijet]->Fill(_trkpmet,  _event_weight);
  h_htjets  [ichannel][icut][ijet]->Fill(_htjets,   _event_weight);
  h_htnojets[ichannel][icut][ijet]->Fill(_htnojets, _event_weight);
  h_mllstar [ichannel][icut][ijet]->Fill(_mllstar,  _event_weight);

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
