#define AnalysisTop_cxx
#include "../include/AnalysisTop.h"
using namespace std;


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
AnalysisTop::AnalysisTop(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTop::Loop(TString analysis, TString filename, float luminosity)
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

	h_2ht    [i][j][k] = new TH2F("h_2ht" + suffix, "", 300, 0, 800, 300, 0, 800);
	h_met_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 3000, 0, 3000);
	h_metm2l [i][j][k] = new TH2D("h_metm2l" + suffix, "", 3000, 0,3000, 3000, 0, 3000);

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
    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    // "Third Z-Veto" This requirement should be applied on a loose lepton 
//    if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;
  //  if (_nlepton > 2) continue;

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
    bool pass;
    bool passZveto;
    bool passZwindow;
    bool passL;
    bool passM;
    bool passT;

  	 // AN-15-305 analysis
  	 //--------------------------------------------------------------------------
  	 
		pass = true;
    	
	        FillLevelHistograms(Top_00_Has2Leptons, pass);    

    		pass &= (Lepton1.v.Pt() > 20.);
    		pass &= (Lepton2.v.Pt() > 20.);
    		pass &= (_m2l > 20.);

		
		FillLevelHistograms(Top_02_Restability, pass);

    		pass &= (_nelectron == 1 || MET.Et() > 45.);

                FillLevelHistograms(Top_02_Met45, pass); 
		
   		pass &= (njet > 1);
                passZveto = pass && (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);

	        FillLevelHistograms(Top_03_Zveto, passZveto);


		passL = pass && (_nbjet30loose > 0);
                passM = pass && (_nbjet30medium > 0);
                passT = pass && (_nbjet30tight > 0);

                passZwindow = passM && (_nelectron == 1 || fabs(_m2l - Z_MASS) < 15.);

		
    		FillLevelHistograms(Top_04_AN15305MZwindow, passZwindow);
    		FillLevelHistograms(Top_04_AN15305L, passL);
    		FillLevelHistograms(Top_04_AN15305M, passM);
    		FillLevelHistograms(Top_04_AN15305T, passT);


    //  FillLevelHistograms(Top_00_Has2Leptons, pass);    
   
    //-------------------------------------------------------------------------
    // Basics Top

    

   // bool pass_sf = (_nelectron != 1 && fabs(_m2l - Z_MASS) > 15.);
   // bool pass_df = (_nelectron == 1);

   // pass &= mll>20.;
   //
   // FillLevelHistograms(Top_00_mll20, pass);

   // pass &= fabs(mll - Z_MASS) > 15.;

   // FillLevelHistograms(Top_00_Zveto, pass);

   // pass &= (MET.Et() > 40.);

   // FillLevelHistograms(Top_00_Met40, pass); 


    //--------------------------------------------------------------------------
    // Basics + met > 50 + Has2Leptons
 
   //  bool pass_met50 = MET.Et() > 50.;

   //  bool pass3 = pass && pass_met50;
 
   //  FillLevelHistograms(Top_00_Met50, pass3);
    


   
    //-------------------------------------------------------------------------
    // Basics + _ht > 260 + Has2Leptons    

//    bool pass1 = _ht > 260.;
//    bool pass1_jet = _htjets > 100.;   
//    bool pass1_1 = _ht > 300.;   
//    bool pass1_1_jet = _htjets > 125.;
//
//    FillLevelHistograms(Top_00_ht260, pass && pass1);
//    FillLevelHistograms(Top_00_ht300, pass && pass1_1);
//    FillLevelHistograms( Top_00_htjets100, pass && pass1_jet);
//    FillLevelHistograms( Top_00_htjets125, pass && pass1_1_jet);
//
//    bool pass1_2_jet = _htjets > 150.;
//
//    FillLevelHistograms( Top_00_htjets150, pass && pass1_2_jet);

    


    //--------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 50 + Has2Leptons
     
//    pass1 &= MET.Et() > 50.;    
//    pass1_1 &= MET.Et() > 50.;    	
//    pass1_jet &= MET.Et() > 50.;
//    pass1_1_jet &= MET.Et() > 50.;
// 
//    FillLevelHistograms(Top_00_ht260Met50, pass && pass1);
//    FillLevelHistograms(Top_00_ht300Met50, pass && pass1_1);
//    FillLevelHistograms( Top_00_htjets100Met50, pass && pass1_jet);
//    FillLevelHistograms( Top_00_htjets125Met50, pass && pass1_1_jet);
//	 
//    pass1_2_jet &= MET.Et() > 50.;
//
//    FillLevelHistograms( Top_00_htjets150Met50, pass && pass1_2_jet);

    //-------------------------------------------------------------------------
    // Basics + Has1BJet
   
  //  pass &= (njet > 1);
  //
  //  FillLevelHistograms(Top_01_Has2Jets, pass);


  //  pass &= (_nbjet30tight > 0);

  //  FillLevelHistograms(Top_02_Has1BJet, pass);

  //  
  //  bool pass_htjets = _htjets > 150.;
  //  bool pass_mpmet  = (_nelectron == 1 || _mpmet > 30.);
  //  

  //  pass_htjets &= pass;    

  //  FillLevelHistograms(Top_02_Has1BJetHtJets150, pass_htjets);


  //  pass_mpmet &= pass;

  //  FillLevelHistograms(Top_02_Has1BJetSFmpMet30, pass_mpmet);


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
  h_2ht[ichannel][icut][ijet]->Fill(_ht, _htjets, _event_weight);
  h_met_m2l[ichannel][icut][ijet]->Fill(metPfType1, _m2l, _event_weight);
  h_metm2l [ichannel][icut][ijet]->Fill(metPfType1, _m2l, _event_weight);

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
