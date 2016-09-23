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
  for (Long64_t jentry=0; jentry<_nentries; jentry++) {

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
    
    _m2l  = mll;
    _pt2l = ptll;
    

    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass_2l = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_2l &= (Lepton1.v.Pt() > 20.);
    pass_2l &= (Lepton2.v.Pt() > 20.);
    pass_2l &= (mll > 20.);
   
    FillLevelHistograms(DY_00_noCuts, true);

    FillLevelHistograms(DY_01_Has2Leptons, pass_2l);

    pass_2l &= (abs(mll>70.) && abs(mll<120.));
    FillLevelHistograms(DY_02_Zwindow, pass_2l);
   
    pass_2l &= (metPfType1 < 60. && pfmetMEtSig < 15.);
    FillLevelHistograms(DY_03_phiMETcorrected, pass_2l);
   
   

   
   

   
   

   
   

   

   
    // bool passBvetoMedium = pass && (_nbjet20cmvav2m == 0);
    // FillLevelHistograms(DY_08_BVetoMedium, passBvetoMedium && pass_zwindow);

    // bool passBvetoTight = pass && (_nbjet20cmvav2t == 0);
    // FillLevelHistograms(DY_09_BVetoTight, passBvetoTight && pass_zwindow);

    // bool passBvetoLooseOld = pass && (_nbjet15csvv2l == 0);
    // FillLevelHistograms(DY_10_BVetoLooseOld, passBvetoLooseOld && pass_zwindow);

    // bool passBvetoMediumOld = pass && (_nbjet15csvv2m == 0);
    // FillLevelHistograms(DY_11_BVetoMediumOld, passBvetoMediumOld && pass_zwindow);

    // bool passBvetoTightOld = pass && (_nbjet15csvv2t == 0);
    // FillLevelHistograms(DY_12_BVetoTightOld, passBvetoTightOld && pass_zwindow);

    // bool pass_topCR = (mll > 50 && pass_zwindow && MET.Et() > 20 && _nbjet20cmvav2l > 0);
    // FillLevelHistograms(DY_50_TopCR, pass_topCR);

    // bool pass_dyttCR = (_nbjet20cmvav2l == 0 && mll > 30 && mll < 80 && MET.Et() < 60);
    // FillLevelHistograms(DY_60_DYttCR, pass_dyttCR);

    // //    bool pass_dyCR = (_nbjet20cmvav2l == 0 && !pass_zwindow);
    // bool pass_dyCR = (!pass_zwindow && ptll > 45 && MET.Et() > 20 && _mpmet > 20 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(DY_70_DYCR, pass_dyCR);

    // bool pass_dyCR_nompmet = (!pass_zwindow && ptll > 45 && MET.Et() > 20 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(DY_71_DYCR_NoMpMet, pass_dyCR_nompmet);

    // bool pass_dyCR_justptll = (!pass_zwindow && ptll > 45 && _nbjet20cmvav2l == 0);
    // FillLevelHistograms(DY_72_DYCR_JustPtll, pass_dyCR_nompmet);

    //    bool pass_ht = (_ht < 250.);
    //    FillLevelHistograms(DY_09_Ht, pass && pass_zveto && pass_ht);
    
    //    FillLevelHistograms(DY_10_DY, pass);  // Data-driven DY


    // DY control region
    //--------------------------------------------------------------------------
    /*
    FillLevelHistograms(DY_11_ZWindow, pass_zwindow);

    pass_zwindow &= (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);
    FillLevelHistograms(DY_12_ZWindowPtll, pass_zwindow);

    for (UInt_t j=0; j<12; ++j)
      {
	bool jet = (std_vector_jet_pt->at(0) < 25. + j && fabs(std_vector_jet_eta->at(0)) < 4.7);

	FillLevelHistograms(DY_18_ZWindow25 + j, pass_zwindow && jet);
      }

    //    pass_zwindow &= (mpmet > 20.);
    //    FillLevelHistograms(DY_13_ZWindowMpMet, pass_zwindow);

    pass_zwindow &= (MET.Et() > 20.);
    FillLevelHistograms(DY_14_ZWindowPfMet, pass_zwindow);

    pass_zwindow &= (_nbjet20cmvav2l == 0);
    FillLevelHistograms(DY_15_ZWindowBVeto, pass_zwindow);

    pass_zwindow &= (!_foundsoftmuon);
    FillLevelHistograms(DY_16_ZWindowSoftMu, pass_zwindow);

    pass_zwindow &= (mpmet > 20.);
    FillLevelHistograms(DY_17_ZCR, pass_zwindow);
    */


    // monoH selection - on top of WW (excluding Ht selection)
    //--------------------------------------------------------------------------
    /*
    bool pass_monoh = (pass && pass_zveto);
    bool pass_drll  = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);

    FillLevelHistograms(DY_103_MonoH_CR, pass_monoh && !pass_drll);
    
    pass_monoh &= (_mc < 100.);
    FillLevelHistograms(DY_100_MonoH_Mc, pass_monoh);
    
    pass_monoh &= pass_drll;
    FillLevelHistograms(DY_101_MonoH_DRll, pass_monoh);
    
    pass_monoh &= (mpmet > 60.);
    FillLevelHistograms(DY_102_MonoH_MpMet, pass_monoh);
    */
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
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

  //  FillAnalysisHistograms(_channel, icut, _jetbin);
  //  FillAnalysisHistograms(_channel, icut, njetbin);
}
