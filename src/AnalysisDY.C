#define AnalysisDY_cxx
#include "../include/AnalysisDY.h"


//------------------------------------------------------------------------------
// AnalysisDY
//------------------------------------------------------------------------------
AnalysisDY::AnalysisDY(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisDY::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
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
  for (Long64_t jentry=0; jentry<_nentries; jentry++) {

    Long64_t ientry = LoadTree(jentry);
   
    met_Over_pt2l = MET.Et() / ptll;
    pt2l_Over_met = ptll / MET.Et();
    
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
    
    _m2l  = mll;
    _pt2l = ptll;
    
    bool pass_2l = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_2l &= (Lepton1.v.Pt() > 25.);
    pass_2l &= (Lepton2.v.Pt() > 20.);
    pass_2l &= (std_vector_lepton_pt->at(2) < 10.);
    pass_2l &= (mll>12.);



    // Fill histograms. WW selection: CMS AN-15-325, pag.7 with pt1 > 25 (trigger)
    //--------------------------------------------------------------------------
    
    //No cuts
    //---------------------------------------------------------------------------
    FillLevelHistograms(DY_00_noCuts, true);    
    
    //Has 2 Leptons                                                                                                                           
    //---------------------------------------------------------------------------     
    FillLevelHistograms(DY_01_Has2Leptons, pass_2l);

    if (_saveminitree && pass_2l) minitree->Fill();
    
    // DY-Control Region  
    //---------------------------------------------------------------------------  
    bool pass_Zwindow = fabs(_m2l - Z_MASS) < 15.;
    FillLevelHistograms(DY_02_DYControl, pass_2l && pass_Zwindow);

    // B Veto                                                                                                                                
    //---------------------------------------------------------------------------    
    pass_2l &= (_nbjet20cmvav2l == 0);
    FillLevelHistograms(DY_03_BVetoLoose, pass_2l);

    //Ptll cut                                                                                                                                                                                           
    //---------------------------------------------------------------------------                                                                                                                         
    pass_2l &= (ptll>30.);
    pass_2l &= (_channel == em || ptll > 45.);
    FillLevelHistograms(DY_04_Ptll, pass_2l);

    // Cortes adicionales en variables: dphillmet, MET/ptll                                                                                                                                              
    //---------------------------------------------------------------------------                                                                                                                 
                                                                                                                                                                                                         
    pass_2l &= (_dphillmet>=0);
    pass_2l &= (_channel == em || _dphillmet > 2.5);
    FillLevelHistograms(DY_05_dphillmet, pass_2l);

    // MET cut
    //---------------------------------------------------------------------------    
    pass_2l &= (MET.Et() > 20.);
    FillLevelHistograms(DY_06_PfMet, pass_2l);

    // mpMET cut                                                                                                           
    //---------------------------------------------------------------------------                                                            
    pass_2l &= (mpmet > 20.);
    pass_2l &= (_channel == em || mpmet > 45.);
    FillLevelHistograms(DY_07_mpMet, pass_2l);
    
    //DY-Control region
    //---------------------------------------------------------------------------
    FillLevelHistograms(DY_08_Zwindow, pass_2l && pass_Zwindow);

    // Z peak Veto
    //---------------------------------------------------------------------------    
    pass_2l &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    FillLevelHistograms(DY_09_ZVeto, pass_2l);
    
    //top-enriched control region (CMS AN-16-182, pag.57, adapted to WW selection)
    //---------------------------------------------------------------------------                        
    bool pass_tcontrol = (Lepton1.flavour * Lepton2.flavour < 0);
    pass_tcontrol &= (Lepton1.v.Pt() > 25.);
    pass_tcontrol &= (Lepton2.v.Pt() > 20.);
    pass_tcontrol &= (std_vector_lepton_pt->at(2) < 10.);
    pass_tcontrol &= (mll > 12.);
    pass_tcontrol &= (ptll > 30.);
    pass_tcontrol &= (_channel == em || ptll > 45.);
    pass_tcontrol &= (MET.Et() > 20.);
    pass_tcontrol &= (mpmet > 20.);
    pass_tcontrol &= (_channel == em || mpmet > 45.);
    pass_tcontrol &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);
    pass_tcontrol &= (_njet==0 ? _nbjet20cmvav2l > 0 : _nbjet30cmvav2l > 0);
    FillLevelHistograms(DY_10_TopControl, pass_tcontrol);
    
    
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisDY::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

    FillAnalysisHistograms(_channel, icut, _jetbin);
    FillAnalysisHistograms(_channel, icut, njetbin);
}
