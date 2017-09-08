#define AnalysisTTDM_cxx
#include "../include/AnalysisTTDM.h"



//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
AnalysisTTDM::AnalysisTTDM(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteHistograms(false);
  SetWriteMinitree(true);
  SetMinitreePath("/eos/user/j/jgarciaf/");
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTTDM::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  if (_writehistograms)
    {
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
    }


  // Loop over events
  //----------------------------------------------------------------------------

    //if ( _nentries > 100000 )  _nentries = 100000;  
    for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    //if (jentry%15!=0) continue; 

    //cout << "\n" << jentry << endl;

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup(2.4);  // We consider only jets up to |eta| < 4.0


    // Analysis
    //--------------------------------------------------------------------------
    //if (!_ismc && run > 258750) continue;  // Luminosity for any blinded analysis


    //if (_writeminitree) minitree->Fill();   // the most primitive pruning

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 25.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel


    // AN-16-011, IFCA ¿?
    //--------------------------------------------------------------------------
    bool pass   = true;

//FillLevelHistograms(step_1, pass);
//FillLevelHistograms(step_2, pass);
//FillLevelHistograms(step_3, pass);

    pass &= (std_vector_lepton_pt->at(2) < 10.);






    pass &= ( _m2l > 20.                                    );

    pass &= ( _channel == em  ||  fabs(_m2l - Z_MASS) > 15. );

    pass &= ( _njet > 1                                     );

    ///pass &= ( metPfType1  > 50.                             );
//FillLevelHistograms(step_7, pass);
    pass &= ( _nbjet30csvv2m > 0                            );
//FillLevelHistograms(step_8, pass);

//FillLevelHistograms(TTDM_01_NewPresel, pass);

    if (_writeminitree && pass ) minitree->Fill();

    // TT Control Region
    //--------------------------------------------------------------------------
    /*pass  = true; 
    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_m2l > 20.);
    pass &= (_njet > 1);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (MET.Et() > 25 && MET.Et() < 50.); 

    FillLevelHistograms(TTDM_04_tt, pass);

    pass &= (_nbjet30csvv2m > 0);

    //if ( _writeminitree && pass ) minitree->Fill();

    FillLevelHistograms(TTDM_05_tt, pass);

    pass &= (_nbjet30csvv2m > 1);

    FillLevelHistograms(TTDM_06_tt, pass);*/


    // WW Control Region
    //--------------------------------------------------------------------------
    /*pass  = true;

    pass &= (Lepton1.v.Pt() > 30.);
    pass &= (Lepton2.v.Pt() > 10.);
    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_m2l > 20.);
    pass &= (MET.Et() > 50.);
    pass &= (mpmet > 20.);
    pass &= (_pt2l > 30.);
    pass &= (_nbjet30csvv2m == 0);

    if (_channel != em)
      {
	pass &= (fabs(_m2l - Z_MASS) > 15.);
	pass &= (mpmet > 40.);
	pass &= (_pt2l > 45.);
      }

    FillLevelHistograms(TTDM_WW0jet, pass && _njet == 0);
    FillLevelHistograms(TTDM_WW1jet, pass && _njet == 1);

    //if ( _writeminitree && pass && _njet == 0 ) minitree->Fill();*/


    // Zjets Control Region
    //--------------------------------------------------------------------------
    /*pass  = true;

    pass &= (std_vector_lepton_pt->at(2) < 10.);
    pass &= (_m2l > 20.);
    pass &= (_njet > 1);
    pass &= (_nbjet30csvv2m > 0);
    pass &= (_channel != em && fabs(_m2l - Z_MASS) < 15.);
    pass &= (MET.Et() > 20.); 
    
    //if ( _writeminitree && pass ) minitree->Fill();
    FillLevelHistograms(TTDM_Zjets, pass);*/  


    // AN-16-105, Northwestern University
    //--------------------------------------------------------------------------
    /*pass = true; 

    pass &= (std_vector_lepton_pt->at(2) < 10.);

    // Cut applied in AN-16-105 but not in AN-16-011
    // At least one lepton passes a single lepton trigger

    pass &= (_m2l > 20.);
    pass &= (_njet > 1);
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass &= (MET.Et() > 50.); 
    //pass &= (_nbjet30csvv2m > 0);
    pass &= (_dphillmet > 1.2);

    FillLevelHistograms(TTDM_03_AN16105, pass);*/

  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillAnalysisHistograms(int ichannel,
					  int icut,
					  int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillLevelHistograms(int  icut,
				       bool pass)
{
  if (!_writehistograms) return;

  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  //  FillAnalysisHistograms(_channel, icut, _jetbin);
  //  FillAnalysisHistograms(_channel, icut, njetbin);
}
