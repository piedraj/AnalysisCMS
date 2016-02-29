#define AnalysisTop_cxx
#include "../include/AnalysisTop.h"


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
AnalysisTop::AnalysisTop(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTop::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

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

	h_test[i][j][k] = new TH1D("h_test" + suffix, "", 300, 0, 300);
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
    if (!trigger) continue;

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    // "Third Z-Veto" This requirement should be applied on a loose lepton 
    if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;

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
    bool pass = true;
   
    //-------------------------------------------------------------------------
    // Basics Top

    pass &= mll>20.;

    pass &= fabs(mll - Z_MASS) > 15.;

    pass &= (MET.Et() > 30.); 
   
    //-------------------------------------------------------------------------
    // Basics + _ht > 260 + Has2Leptons    

//    bool pass1 = _ht > 260.;   
//    bool pass1_1 = _ht > 300.;   

//    FillLevelHistograms(Top_00_ht260Has2Leptons, pass && pass1);
//    FillLevelHistograms(Top_00_ht300Has2Leptons, pass && pass1_1);

    //------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 40 + Has2Leptons

//    pass1 &= MET.Et() > 40.;
//    pass1_1 &= MET.Et() > 40.;

//    FillLevelHistograms(Top_00_ht260Met40Has2Leptons, pass && pass1); 
//    FillLevelHistograms(Top_00_ht300Met40Has2Leptons, pass && pass1_1); 

    //--------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 50 + Has2Leptons
     
//    pass1 &= MET.Et() > 50.;    
//    pass1_1 &= MET.Et() > 50.;    
  
//    FillLevelHistograms(Top_00_ht260Met50Has2Leptons, pass && pass1);
//    FillLevelHistograms(Top_00_ht300Met50Has2Leptons, pass && pass1_1);

    //-------------------------------------------------------------------------
    // Basics + met > 40 + Has1BJet

//    bool pass_met40 =  MET.Et() > 40.;
    
//    bool pass2 = pass && pass_met40;
    	

    FillLevelHistograms(Top_00_Has2Leptons, pass);

    pass &= (njet > 1);

    FillLevelHistograms(Top_01_Has2Jets, pass);

    pass &= (_nbjet30medium > 0);

    FillLevelHistograms(Top_02_Has1BJet, pass);
    
    //--------------------------------------------------------------------------
    // Basics + met > 50 + Has2Leptons

//    bool pass_met50 = MET.Et() > 50.;

//    bool pass3 = pass && pass_met50;

//    FillLevelHistograms(Top_00_MET50Has2Leptons, pass3);

    //--------------------------------------------------------------------------
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisTop::FillAnalysisHistograms(int ichannel,
					 int icut,
					 int ijet)
{
  h_test[ichannel][icut][ijet]->Fill(_m2l, _event_weight);

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisTop::FillLevelHistograms(int  icut,
				      bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
