#define AnalysisTop_cxx
#include "../include/AnalysisTop.h"


//------------------------------------------------------------------------------
// AnalysisTop
//------------------------------------------------------------------------------
AnalysisTop::AnalysisTop(TTree* tree) : AnalysisCMS(tree)
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

	h_test		[i][j][k] = new TH1D("h_test" + suffix, "", 300, 0, 300);
        h_htjets	[i][j][k] = new TH1D("h_htjets" + suffix, "", 300, 0, 800);
	h_2ht 		[i][j][k] = new TH2F("h_2ht" + suffix, "", 300, 0, 800,300,0,800);
        h_dphilmet1	[i][j][k] = new TH1D("h_dphilmet1"   + suffix, "", 1000, 0,   10);
	h_dphilmet2	[i][j][k] = new TH1D("h_dphilmet2"   + suffix, "", 1000, 0,   10);        
        h_jetpt1   	[i][j][k] = new TH1D("h_jetpt1"      + suffix, "", 1000, 0, 1000);
        h_jetpt2   	[i][j][k] = new TH1D("h_jetpt2"      + suffix, "", 1000, 0, 1000);
	
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
    //if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;
    if (_nlepton > 2) continue;

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

    FillLevelHistograms(Top_00_Has2Leptons, pass);    
   
    //-------------------------------------------------------------------------
    // Basics Top

    pass &= mll>20.;
   
    FillLevelHistograms(Top_00_mll20, pass);

    pass &= fabs(mll - Z_MASS) > 15.;

    FillLevelHistograms(Top_00_Zveto, pass);

    pass &= (MET.Et() > 40.);

    FillLevelHistograms(Top_00_Met40, pass); 
   
    //-------------------------------------------------------------------------
    // Basics + _ht > 260 + Has2Leptons    

//    bool pass1 = _ht > 260.;
    bool pass1_jet = _htjets > 100.;   
//    bool pass1_1 = _ht > 300.;   
    bool pass1_1_jet = _htjets > 125.;
//    FillLevelHistograms(Top_00_ht260, pass && pass1);
//    FillLevelHistograms(Top_00_ht300, pass && pass1_1);
    FillLevelHistograms( Top_00_htjets100, pass && pass1_jet);
    FillLevelHistograms( Top_00_htjets125, pass && pass1_1_jet);

    bool pass1_2_jet = _htjets > 150.;
    FillLevelHistograms( Top_00_htjets150, pass && pass1_2_jet);

    


    //--------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 50 + Has2Leptons
     
//    pass1 &= MET.Et() > 50.;    
//    pass1_1 &= MET.Et() > 50.;    	
      pass1_jet &= MET.Et() > 50.;
      pass1_1_jet &= MET.Et() > 50.;
  
//    FillLevelHistograms(Top_00_ht260Met50, pass && pass1);
//    FillLevelHistograms(Top_00_ht300Met50, pass && pass1_1);
      FillLevelHistograms( Top_00_htjets100Met50, pass && pass1_jet);
      FillLevelHistograms( Top_00_htjets125Met50, pass && pass1_1_jet);
	 
      pass1_2_jet &= MET.Et() > 50.;
      FillLevelHistograms( Top_00_htjets150Met50, pass && pass1_2_jet);

    //-------------------------------------------------------------------------
    // Basics + Has1BJet
   
    bool pass2 = pass && (njet > 1);

    //pass &= (njet > 1);
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! pass &= (_nbjet20loose > 0);

    FillLevelHistograms(Top_01_Has2Jets, pass2);

    pass2 &= (_nbjet30tight > 0);

    FillLevelHistograms(Top_02_Has1BJet, pass2);

    pass2 &= _htjets > 150.;    

    FillLevelHistograms(Top_02_Has1BJetHtJets150, pass2);
    
    //--------------------------------------------------------------------------
    // Basics + met > 50 + Has2Leptons

    bool pass_met50 = MET.Et() > 50.;

    bool pass3 = pass && pass_met50;

    FillLevelHistograms(Top_00_Met50, pass3);

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
  h_test	[ichannel][icut][ijet]->Fill(_m2l, _event_weight);
  h_htjets	[ichannel][icut][ijet]->Fill(_htjets, _event_weight);
  h_2ht 	[ichannel][icut][ijet]->Fill(_ht, _htjets, _event_weight);
  h_dphilmet1	[ichannel][icut][ijet]->Fill(dphilmet1,                    _event_weight);
  h_dphilmet2	[ichannel][icut][ijet]->Fill(dphilmet2,                    _event_weight);  
  h_jetpt1   	[ichannel][icut][ijet]->Fill(std_vector_jet_pt->at(0),     _event_weight);
  h_jetpt2   	[ichannel][icut][ijet]->Fill(std_vector_jet_pt->at(1),     _event_weight);

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
