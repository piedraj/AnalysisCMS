#define AnalysisHWW_cxx
#include "../include/AnalysisHWW.h"

//------------------------------------------------------------------------------
// AnalysisHWW
//------------------------------------------------------------------------------
AnalysisHWW::AnalysisHWW(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisHWW::Loop(TString analysis, TString filename, float luminosity)
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

    // SAME SIGN REGION ANALYSIS
    if((std_vector_lepton_flavour -> at(0) * std_vector_lepton_flavour -> at(1) == 11*13)) continue;

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel

    if(_m2l < 12.) continue;
    if(std_vector_lepton_pt -> at(0) < 20.) continue;
    if(std_vector_lepton_pt -> at(1) < 10.) continue;
    if(std_vector_lepton_pt -> at(2) > 10.) continue;
    if(metPfType1 < 20.) continue;
    if(_pt2l < 30.) continue;

    if(mth < 60.) continue;
    if(mpmet < 20.) continue;
    if(mtw2 < 30.) continue;

    bool pass;

    // Same sign control region (HWW selection)
    //--------------------------------------------------------------------------
    pass = true;

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (std_vector_jet_pt -> at(0) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 

    FillLevelHistograms(HWW_01_hww2l2v_13TeV_of0j, pass);

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (std_vector_jet_pt -> at(0) >= 30.);
    pass &= (std_vector_jet_pt -> at(1) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 

    FillLevelHistograms(HWW_02_hww2l2v_13TeV_of1j, pass);

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (abs(std_vector_lepton_flavour -> at(0)) == 11);
    pass &= (std_vector_jet_pt -> at(0) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 

    FillLevelHistograms(HWW_03_hww2l2v_13TeV_em_0j, pass);

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (abs(std_vector_lepton_flavour -> at(0)) == 11);
    pass &= (std_vector_jet_pt -> at(0) >= 30.);
    pass &= (std_vector_jet_pt -> at(1) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 
    
    FillLevelHistograms(HWW_04_hww2l2v_13TeV_em_1j, pass);

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (abs(std_vector_lepton_flavour -> at(0)) == 13);
    pass &= (std_vector_jet_pt -> at(0) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 

    FillLevelHistograms(HWW_05_hww2l2v_13TeV_me_0j, pass);

    pass = (abs(std_vector_lepton_flavour -> at(1)) == 13 || std_vector_lepton_pt -> at(1) > 13.);
    pass &= (abs(std_vector_lepton_flavour -> at(0)) == 13);
    pass &= (std_vector_jet_pt -> at(0) >= 30.);
    pass &= (std_vector_jet_pt -> at(1) < 30.);

    pass &= (std_vector_jet_pt -> at(0) < 20. || std_vector_jet_cmvav2 -> at(0) < -0.715); 
    pass &= (std_vector_jet_pt -> at(1) < 20. || std_vector_jet_cmvav2 -> at(1) < -0.715); 
    pass &= (std_vector_jet_pt -> at(2) < 20. || std_vector_jet_cmvav2 -> at(2) < -0.715); 
    pass &= (std_vector_jet_pt -> at(3) < 20. || std_vector_jet_cmvav2 -> at(3) < -0.715); 
    pass &= (std_vector_jet_pt -> at(4) < 20. || std_vector_jet_cmvav2 -> at(4) < -0.715); 
    pass &= (std_vector_jet_pt -> at(5) < 20. || std_vector_jet_cmvav2 -> at(5) < -0.715); 
    pass &= (std_vector_jet_pt -> at(6) < 20. || std_vector_jet_cmvav2 -> at(6) < -0.715); 
    pass &= (std_vector_jet_pt -> at(7) < 20. || std_vector_jet_cmvav2 -> at(7) < -0.715); 
    pass &= (std_vector_jet_pt -> at(8) < 20. || std_vector_jet_cmvav2 -> at(8) < -0.715); 
    pass &= (std_vector_jet_pt -> at(9) < 20. || std_vector_jet_cmvav2 -> at(9) < -0.715); 
    
    FillLevelHistograms(HWW_06_hww2l2v_13TeV_me_1j, pass);

    //    if (pass && _njet == 0 && _channel == em) GetRecoWeightsLHE(list_vectors_weights_0jet);
    //    if (pass && _njet == 1 && _channel == em) GetRecoWeightsLHE(list_vectors_weights_1jet);
  
    }

  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisHWW::FillLevelHistograms(int  icut,
				      bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}
