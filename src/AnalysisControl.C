#define AnalysisControl_cxx
#include "../include/AnalysisControl.h"


//------------------------------------------------------------------------------
// AnalysisControl
//------------------------------------------------------------------------------
AnalysisControl::AnalysisControl(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisControl::Loop(TString analysis, TString filename, float luminosity)
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

    bool pass_2l = (Lepton1.flavour * Lepton2.flavour < 0);

    pass_2l &= (Lepton1.v.Pt() > 25.);
    pass_2l &= (Lepton2.v.Pt() > 20.);
    pass_2l &= (std_vector_lepton_pt->at(2) < 10.);
    pass_2l &= (_m2l > 12.);


    bool pass;


    // No cuts
    //--------------------------------------------------------------------------
    pass = true;

    FillLevelHistograms(Control_00_NoCuts, pass);


    // Has 2 medium leptons
    //--------------------------------------------------------------------------
    pass = pass_2l;

    bool isMediumMuon1 = (std_vector_lepton_isMediumMuon->at(0) && abs(Lepton1.flavour) == MUON_FLAVOUR);
    bool isMediumMuon2 = (std_vector_lepton_isMediumMuon->at(1) && abs(Lepton2.flavour) == MUON_FLAVOUR);

    bool isMediumElectron1 = (std_vector_lepton_eleIdMedium->at(0) && abs(Lepton1.flavour) == ELECTRON_FLAVOUR);
    bool isMediumElectron2 = (std_vector_lepton_eleIdMedium->at(1) && abs(Lepton2.flavour) == ELECTRON_FLAVOUR);

    pass &= (isMediumMuon1 || isMediumElectron1);
    pass &= (isMediumMuon2 || isMediumElectron2);

    FillLevelHistograms(Control_01_TwoMediumLeptons, pass);
    

    // Has 2 tight leptons
    //--------------------------------------------------------------------------
    pass = pass_2l;

    bool isTightMuon1 = (std_vector_lepton_isTightMuon->at(0) && abs(Lepton1.flavour) == MUON_FLAVOUR);
    bool isTightMuon2 = (std_vector_lepton_isTightMuon->at(1) && abs(Lepton2.flavour) == MUON_FLAVOUR);

    bool isTightElectron1 = (std_vector_lepton_eleIdTight->at(0) && abs(Lepton1.flavour) == ELECTRON_FLAVOUR);
    bool isTightElectron2 = (std_vector_lepton_eleIdTight->at(1) && abs(Lepton2.flavour) == ELECTRON_FLAVOUR);

    pass &= (isTightMuon1 || isTightElectron1);
    pass &= (isTightMuon2 || isTightElectron2);

    FillLevelHistograms(Control_02_TwoTightLeptons, pass);

    if (_saveminitree && pass) minitree->Fill();


    // R out/in
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (_nbjet20cmvav2l == 0);
    pass &= (_pt2l > 30.);
    pass &= (_channel == em || _pt2l > 45.);

    FillLevelHistograms(Control_03_Routin, pass);


    // WW
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (_nbjet20cmvav2l == 0);
    pass &= (MET.Et() > 20.);
    pass &= (_pt2l > 30.);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (_channel == em || MET.Et() > 45.);
    pass &= (_channel == em || _pt2l > 45.);

    FillLevelHistograms(Control_04_WW, pass);


    // Top
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (_njet > 1);
    pass &= (_nbjet30csvv2m > 0);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (MET.Et() > 45.);

    FillLevelHistograms(Control_05_Top, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisControl::FillLevelHistograms(int  icut,
					  bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}
