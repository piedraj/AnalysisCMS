#define AnalysisMET_cxx
#include "../include/AnalysisMET.h"



//------------------------------------------------------------------------------
// AnalysisMET
//------------------------------------------------------------------------------
AnalysisMET::AnalysisMET(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(true);
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

    //if ( _nentries > 1000 )  _nentries = 1000;  
    for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    //cout << "\n" << jentry << endl;

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    //EventSetup(4.0);  // We consider only jets up to |eta| < 4.0


    // Analysis
    //--------------------------------------------------------------------------
    //if (!_ismc && run > 258750) continue;  // Luminosity for any blinded analysis

    //if (_saveminitree) minitree->Fill();   // the most primitive pruning

    //if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    //if (Lepton1.v.Pt() < 25.) continue;
    //if (Lepton2.v.Pt() < 20.) continue;

    //_nelectron = 0;

    //if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    //if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    //if      (_nelectron == 2) _channel = ee;
    //else if (_nelectron == 1) _channel = em;
    //else if (_nelectron == 0) _channel = mm;
    
    //_m2l  = mll;   // Needs l2Sel
    //_pt2l = ptll;  // Needs l2Sel



    //bool pass   = true;

    //pass &= (std_vector_lepton_pt->at(2) < 10.);

    //pass &= ( _m2l > 20.                                    );

    //pass &= ( _channel == em  ||  fabs(_m2l - Z_MASS) > 15. );

    //pass &= ( _njet > 1                                     );

    //pass &= ( _nbjet30csvv2m > 0                            );


    if (_saveminitree && pass ) minitree->Fill();

 

  }


  EndJob();
}

