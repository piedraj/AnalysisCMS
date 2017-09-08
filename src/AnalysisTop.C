#define AnalysisTop_cxx
#include "../include/AnalysisTop.h"
using namespace std;


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
AnalysisTop::AnalysisTop(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTop::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //---------------------------------------------------------------------------

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

	h_2ht    [i][j][k] = new TH2F("h_2ht" + suffix, "", 300, 0, 800, 300, 0, 800);
	h_met_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 3000, 0, 3000);
      }
    }
  }

  root_output->cd();


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries; jentry++) {

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
    bool pass0;
    bool passAN15305;
    bool passZvetoL;
    bool passZvetoT;
    bool passZwindow;
    bool passMet45;
    bool passL;
    bool passM;
    bool passT;
    bool passRoutin; 

  	 // AN-15-305 analysis
  	 //--------------------------------------------------------------------------
		
                pass0 = true;
                FillLevelHistograms(Top_00_Has2Leptons, pass0);

    		pass = pass0 && (Lepton1.v.Pt() > 20.);
    		pass &= (Lepton2.v.Pt() > 20.);
    		pass &= (_m2l > 20.);
                FillLevelHistograms(Top_01_Restability, pass);

    		passMet45 = pass && (_nelectron == 1 || MET.Et() > 45.);
 //             FillLevelHistograms(Top_02_Met45, passMet45);
 
                passRoutin = (_njet > 1) && pass && (_nbjet30cmvav2m > 0);
		FillLevelHistograms(Top_02_Routin, passRoutin);

  		passL = pass && (_njet > 1) && (_nbjet30cmvav2l > 0);
                passM = pass && (_njet > 1) && (_nbjet30cmvav2m > 0);
                passT = pass && (_njet > 1) && (_nbjet30cmvav2t > 0);
//		FillLevelHistograms(Top_04_bL, passL);
//                FillLevelHistograms(Top_04_bM, passM);
//                FillLevelHistograms(Top_04_bT, passT);
              
//                passZwindowM = passM && (_nelectron == 1 || fabs(_m2l - Z_MASS) < 15.);
// 	        FillLevelHistograms(Top_05_AN15305MZwindow, passZwindowM);
//	                
		passAN15305 = passMet45 && passM && (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);
//		passZvetoL = passL && (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);
//		passZvetoT = passT && (_nelectron == 1 || fabs(_m2l - Z_MASS) > 15.);
		FillLevelHistograms(Top_03_AN15305M, passAN15305);
//		FillLevelHistograms(Top_06_AN15305L, passZvetoL);
//		FillLevelHistograms(Top_06_AN15305T, passZvetoT);
		


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
   
  //  pass &= (_njet > 1);
  //
  //  FillLevelHistograms(Top_01_Has2Jets, pass);
//    pass &= (_njet > 1);
  
//    FillLevelHistograms(Top_01_Has2Jets, pass);


//    pass &= (_nbjet30cmvav2t > 0);


  //  pass &= (_nbjet30tight > 0);

  //  FillLevelHistograms(Top_02_Has1BJet, pass);

  //  
  //  bool pass_htjets = _htjets > 150.;
  //  bool pass_mpmet  = (_nelectron == 1 || mpmet > 30.);
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
