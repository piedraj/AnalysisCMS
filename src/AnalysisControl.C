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

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();


    // Define the channel based on the number of electrons
    //--------------------------------------------------------------------------
    _nelectron = 0;

    if (abs(std_vector_lepton_flavour->at(0)) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(std_vector_lepton_flavour->at(1)) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;


    bool pass;


    // No cuts
    //--------------------------------------------------------------------------
    pass = true;

    FillLevelHistograms(Control_00_NoCuts, pass);


    // Common cuts
    //--------------------------------------------------------------------------
    bool pass_os = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) < 0);
    bool pass_ss = (std_vector_lepton_flavour->at(0) * std_vector_lepton_flavour->at(1) > 0);

    bool pass_pt =
      (std_vector_lepton_pt->at(0) > 25) &&
      (std_vector_lepton_pt->at(1) > 13) &&
      (std_vector_lepton_pt->at(2) < 10);

    bool pass_zveto = (_channel == em || fabs(mll - Z_MASS) > 15);

    bool pass_bveto = true;

    for (int j=0; j<std_vector_jet_pt->size(); j++)
      {
	pass_bveto &= (std_vector_jet_pt->at(j) < 20 || std_vector_jet_cmvav2->at(j) < -0.5884);
      }

    bool pass_btag = false;

    for (int j=0; j<std_vector_jet_pt->size(); j++)
      {
	pass_btag |= (std_vector_jet_pt->at(j) > 20 && std_vector_jet_cmvav2->at(j) > -0.715);
      }


    // DY
    // https://github.com/latinos/PlotsConfigurations/blob/master/Configurations/ControlRegions/DY/Full2016/cuts.py
    //--------------------------------------------------------------------------
    pass = (pass_os && pass_pt && mll > 80 && mll < 100);

    FillLevelHistograms(Control_01_DY, pass);


    // WW
    // https://github.com/latinos/PlotsConfigurations/blob/master/Configurations/ControlRegions/WW/Full2016/cuts.py
    //--------------------------------------------------------------------------
    pass = (pass_os && pass_pt && pass_zveto && pass_bveto && mll > 80 && ptll > 30 && mth > 60 && metPfType1 > 20);

    FillLevelHistograms(Control_02_WW, pass);


    // Top
    //--------------------------------------------------------------------------
    pass = (pass_os && pass_pt && pass_zveto && pass_btag && mll > 80 && ptll > 30 && mth > 60 && metPfType1 > 20);

    FillLevelHistograms(Control_03_Top, pass);


    // SS
    //--------------------------------------------------------------------------
    pass = (pass_ss && pass_pt && pass_zveto && mll > 80 && ptll > 30 && mth > 60 && metPfType1 > 20);

    FillLevelHistograms(Control_04_SS, pass);
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
