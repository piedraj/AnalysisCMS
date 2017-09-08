#define AnalysisMET_cxx
#include "../include/AnalysisMET.h"



//------------------------------------------------------------------------------
// AnalysisMET
//------------------------------------------------------------------------------
AnalysisMET::AnalysisMET(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisMET::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Loop over events
  //----------------------------------------------------------------------------

    //if ( _nentries > 10000 )  _nentries = 10000;  
    for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    //cout << "\n" << jentry << endl;

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup(4.0);  // We consider only jets up to |eta| < 4.0


    // Analysis
    //--------------------------------------------------------------------------
    //if (!_ismc && run > 258750) continue;  // Luminosity for any blinded analysis

    //if (_writeminitree) minitree->Fill();   // the most primitive pruning

    //if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    //if (Lepton1.v.Pt() < 25.) continue;
    //if (Lepton2.v.Pt() < 20.) continue;

    //_nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    //_pt2l = ptll;  // Needs l2Sel



    bool pass   = true;

    //pass &= ( nLepGood20 == 2 );

    pass &= ( abs(std_vector_lepton_eta->at(0)) < 2.4 && abs(std_vector_lepton_eta->at(1)) < 2.4 ); 

    pass &= (  abs( abs(std_vector_lepton_eta->at(0))-1.5 ) > 0.1  &&  abs( abs(std_vector_lepton_eta->at(1))-1.5 ) > 0.1  ); 

    pass &= ( abs(_m2l - Z_MASS) < 10. );

    pass &= (std_vector_lepton_pt->at(0) > 25.);

    pass &= (std_vector_lepton_pt->at(1) > 20.);


    if (_writeminitree && pass ) minitree->Fill();

 

  }


  EndJob();
}

