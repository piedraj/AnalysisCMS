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

    pass_2l &= (Lepton1.v.Pt() > 20.);
    pass_2l &= (Lepton2.v.Pt() > 20.);
    pass_2l &= (_m2l > 20.);

    bool pass;


    // Has 2 leptons
    //--------------------------------------------------------------------------
    pass = pass_2l;

    FillLevelHistograms(Control_00_Has2Leptons, pass_2l);    


    // Z+jets
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_nbjet30csvv2m == 0);

    FillLevelHistograms(Control_01_ZJets, pass);


    // Top
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_njet > 1);

    bool btag   = (_nbjet30csvv2m > 0);
    bool zveto  = (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    bool metcut = (MET.Et() > 50. && _dphillmet > 1.2);

    FillLevelHistograms(Control_02_Routin,     pass);
    FillLevelHistograms(Control_03_RoutinBtag, pass && btag);
    FillLevelHistograms(Control_04_Top,        pass && zveto && metcut);
    FillLevelHistograms(Control_05_TopBtag,    pass && zveto && metcut && btag);


    // WW
    //--------------------------------------------------------------------------
    pass = pass_2l;

    pass &= (abs(std_vector_lepton_flavour->at(1)) == 13 || std_vector_lepton_pt->at(1) > 13.);
    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_nbjet20cmvav2l == 0);
    pass &= (MET.Et() > 20.);
    pass &= (mpmet > 20.);
    pass &= (_pt2l > 30.);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (_channel == em || MET.Et() > 40.);
    pass &= (_channel == em || mpmet > 40.);
    pass &= (_channel == em || _pt2l > 45.);

    FillLevelHistograms(Control_06_WW, pass);

    if (pass && _njet == 0 && _channel == em) GetRecoWeightsLHE(list_vectors_weights_0jet);
    if (pass && _njet == 1 && _channel == em) GetRecoWeightsLHE(list_vectors_weights_1jet);
    if (pass && _njet >= 2 && _channel == em) GetRecoWeightsLHE(list_vectors_weights_2jet);


    // WH
    //--------------------------------------------------------------------------
    pass = (mllmin3l > 12
	    && std_vector_lepton_pt->at(0) > 20
	    && std_vector_lepton_pt->at(1) > 15
	    && std_vector_lepton_pt->at(2) > 10
	    && std_vector_lepton_pt->at(3) < 10
	    && abs(chlll) == 1);
    
    bool pass_wh3l_ossf = (pass
			   && njet_3l == 0
			   && nbjet_3l == 0
			   && metPfType1 > 40
			   && zveto_3l > 25
			   && mllmin3l < 100
			   && flagOSSF == 1);

    bool pass_wh3l_sssf = (pass
			   && njet_3l == 0
			   && nbjet_3l == 0
			   && metPfType1 > 30
			   && zveto_3l > 25
			   && mllmin3l < 100
			   && flagOSSF == 0);

    bool pass_wh3l = (pass
		      && njet_3l == 0
		      && nbjet_3l == 0
		      && metPfType1 > 40
		      && zveto_3l > 25
		      && mllmin3l < 100);

    if (pass_wh3l)      GetRecoWeightsLHE(list_vectors_weights_wh3l);
    if (pass_wh3l_ossf) GetRecoWeightsLHE(list_vectors_weights_wh3l_ossf);
    if (pass_wh3l_sssf) GetRecoWeightsLHE(list_vectors_weights_wh3l_sssf);
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
