#define AnalysisControl_cxx
#include "../include/AnalysisControl.h"


//------------------------------------------------------------------------------
// AnalysisControl
//------------------------------------------------------------------------------
AnalysisControl::AnalysisControl(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
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


    ////////////////////////////////////////////////////////////////////////////
    //    fChain->SetBranchStatus("*", 0);
    //    fChain->SetBranchStatus("GEN_weight_SM", 1);
    //    fChain->SetBranchStatus("Gen_ZGstar_mass", 1);
    //    fChain->SetBranchStatus("Gen_ZGstar_MomId", 1);
    //    ...
    //    ...
    ////////////////////////////////////////////////////////////////////////////


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
    pass_2l &= (_m2l > 20.);


    bool pass;


    // No cuts
    //--------------------------------------------------------------------------
    pass = true;

    FillLevelHistograms(Control_00_NoCuts, pass);


    // Has 2 tight leptons
    //--------------------------------------------------------------------------
    pass = pass_2l;

    FillLevelHistograms(Control_01_TwoLeptons, pass);

    if (_saveminitree && pass) minitree->Fill();


    // R out/in
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (_nbjet30csvv2m > 0);

    FillLevelHistograms(Control_02_Routin, pass);


    // WW
    // https://github.com/latinos/PlotsConfigurations/blob/master/Configurations/ControlRegions/WW/Full2016/cuts.py
    //--------------------------------------------------------------------------
    pass =
      mll > 80                         &&
      std_vector_lepton_pt->at(0) > 25 &&
      std_vector_lepton_pt->at(1) > 13 &&
      std_vector_lepton_pt->at(2) < 10 &&
      metPfType1 > 20                  &&
      ptll > 30                        &&
      mth >= 60	                       &&
      (std_vector_jet_pt->at(0) < 20 || std_vector_jet_cmvav2->at(0) < -0.5884) &&
      (std_vector_jet_pt->at(1) < 20 || std_vector_jet_cmvav2->at(1) < -0.5884) &&
      (std_vector_jet_pt->at(2) < 20 || std_vector_jet_cmvav2->at(2) < -0.5884) &&
      (std_vector_jet_pt->at(3) < 20 || std_vector_jet_cmvav2->at(3) < -0.5884) &&
      (std_vector_jet_pt->at(4) < 20 || std_vector_jet_cmvav2->at(4) < -0.5884) &&
      (std_vector_jet_pt->at(5) < 20 || std_vector_jet_cmvav2->at(5) < -0.5884) &&
      (std_vector_jet_pt->at(6) < 20 || std_vector_jet_cmvav2->at(6) < -0.5884) &&
      (std_vector_jet_pt->at(7) < 20 || std_vector_jet_cmvav2->at(7) < -0.5884) &&
      (std_vector_jet_pt->at(8) < 20 || std_vector_jet_cmvav2->at(8) < -0.5884) &&
      (std_vector_jet_pt->at(9) < 20 || std_vector_jet_cmvav2->at(9) < -0.5884);

    FillLevelHistograms(Control_03_WW, pass);

    if (pass) EventDump();


    // Top
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (_njet > 1);
    pass &= (_nbjet30csvv2m > 0);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (MET.Et() > 45.);

    FillLevelHistograms(Control_04_Top, pass);
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
