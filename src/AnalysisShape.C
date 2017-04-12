#define AnalysisShape_cxx
#include "../include/AnalysisShape.h"

//------------------------------------------------------------------------------
// AnalysisShape
//------------------------------------------------------------------------------
AnalysisShape::AnalysisShape(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisShape::Loop(TString analysis, TString filename, float luminosity)
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
    
    if (Lepton1.v.Pt() < 30.) continue;
    if (Lepton2.v.Pt() < 15.) continue;

    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel

    bool pass;

    // Has 2 "tight" leptons
    //--------------------------------------------------------------------------
    //    pass = (std_vector_lepton_isTightLepton -> at(0) == 1 && std_vector_lepton_isTightLepton -> at(1) == 1);
    pass = true;
    
    FillLevelHistograms(Shape_01_Has2Leptons, pass);

    // No additional lepton
    //--------------------------------------------------------------------------
    pass &= (std_vector_lepton_pt -> at(2) < 10.);

    FillLevelHistograms(Shape_02_HasOnly2Leptons, pass);

    // At least two jets
    //--------------------------------------------------------------------------
    pass &= (_njet > 1);
    pass &= (_nbjet30csvv2m > 0);
    pass &= (AnalysisJets[0].v.Pt() > 30. && fabs(AnalysisJets[0].v.Eta()) < 4.);
    pass &= (AnalysisJets[1].v.Pt() > 30. && fabs(AnalysisJets[1].v.Eta()) < 4.);

    FillLevelHistograms(Shape_03_Has2Jets, pass);

    // MET
    //--------------------------------------------------------------------------
    pass &= (MET.Et() >= 50.);

    FillLevelHistograms(Shape_04_MET, pass);

    // Deltaphi
    //--------------------------------------------------------------------------
    pass &= (_dphillmet > 1.2);

    FillLevelHistograms(Shape_05_DeltaPhi, pass);

    // mll
    //--------------------------------------------------------------------------
    pass &= (mll >= 20.);

    FillLevelHistograms(Shape_06_mll, pass);

    // Zveto
    //--------------------------------------------------------------------------
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);

    FillLevelHistograms(Shape_07_Zveto, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisShape::FillLevelHistograms(int  icut,
					  bool pass)
{
  if (!pass) return;
  
  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}



