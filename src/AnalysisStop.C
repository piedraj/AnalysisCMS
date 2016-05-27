#define AnalysisStop_cxx
#include "../include/AnalysisStop.h"
#include "../../BTagSFUtil/BTagSFUtil.C"


//------------------------------------------------------------------------------
// AnalysisStop
//------------------------------------------------------------------------------
AnalysisStop::AnalysisStop(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisStop::Loop(TString analysis, TString filename, float luminosity)
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

	h_2ht[i][j][k] = new TH2F("h_2ht" + suffix, "", 300, 0,  800, 300, 0, 800);
	h_dym[i][j][k] = new TH2F("h_dym" + suffix, "", 200, 0, 1000, 100, 0,   5);

	h_dyll        [i][j][k] = new TH1D("h_dyll"         + suffix, "", 100, 0,     5);
	h_dphimetjet  [i][j][k] = new TH1D("h_dphimetjet"   + suffix, "", 100, 0.,  3.2);
        h_dphimetptbll[i][j][k] = new TH1D("h_dphimetptbll" + suffix, "", 100, 0.,  3.2);
	h_mllbb       [i][j][k] = new TH1D("h_mllbb"        + suffix, "", 200, 0,  1000);
        h_meff        [i][j][k] = new TH1D("h_meff"         + suffix, "", 200, 0,  1000);
        h_ptbll       [i][j][k] = new TH1D("h_ptbll"        + suffix, "", 200, 0,  1000);
	h_mt2ll       [i][j][k] = new TH1D("h_mt2ll"        + suffix, "", 200, 0., 1000);
	h_mt2bb       [i][j][k] = new TH1D("h_mt2bb"        + suffix, "", 200, 0., 1000);
	h_mt2lblb     [i][j][k] = new TH1D("h_mt2lblb"      + suffix, "", 200, 0., 1000);
      }
    }
  }

  root_output->cd();


  BTagSFUtil *BTagSF = new BTagSFUtil("mujets", "CSVv2", "Medium", 0, "_T2tt");


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup(2.4);

    if (filename.Contains("T2tt") || filename.Contains("T2tb") || filename.Contains("T2bW")) {

      float EventBTagSF = 1.;
      for (int ijet = 0; ijet<_njet; ijet++) {
	
	int ThisIndex = AnalysisJets[ijet].index;
	int ThisFlavour = std_vector_jet_HadronFlavour->at(ThisIndex);
	
	float MonteCarloEfficiency = BTagSF->JetTagEfficiency(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
	float DataEfficiency = MonteCarloEfficiency*BTagSF->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
	
	if (AnalysisJets[ijet].csvv2ivf>CSVv2M) 
	  EventBTagSF *= DataEfficiency/MonteCarloEfficiency;
	else 
	  EventBTagSF *= (1. - DataEfficiency)/(1. - MonteCarloEfficiency);
	
      }
      
      float tree_btag_sf = bPogSF_CSVM;

      _event_weight *= EventBTagSF/tree_btag_sf;
    }


    // Analysis
    //--------------------------------------------------------------------------
    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;
    
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

    FillLevelHistograms(Stop_00_Has2Leptons, pass);    

   
    // Basics Stop
    //-------------------------------------------------------------------------
    pass &= mll>20.;
   
    FillLevelHistograms(Stop_00_mll20, pass);

    pass &= fabs(mll - Z_MASS) > 15.;

    FillLevelHistograms(Stop_00_Zveto, pass);

    pass &= (MET.Et() > 40.);
    
    FillLevelHistograms(Stop_00_Met40, pass); 
   
    //-------------------------------------------------------------------------
    // Basics + _ht > 260 + Has2Leptons    

//    bool pass1 = _ht > 260.;
//    bool pass1_jet = _htjets > 100.;   
    bool pass1_jet = _htjets > 100.; 
//    bool pass1_1 = _ht > 300.;   
//    bool pass1_1_jet = _htjets > 125.;
    bool pass1_1_jet = _htjets > 125.; 
//    FillLevelHistograms(Stop_00_ht260, pass && pass1);
//    FillLevelHistograms(Stop_00_ht300, pass && pass1_1);
    FillLevelHistograms( Stop_00_htjets100, pass && pass1_jet);
    FillLevelHistograms( Stop_00_htjets125, pass && pass1_1_jet);

    //    bool pass1_2_jet = _htjets > 150.;
    bool pass1_2_jet = _htjets > 150.;
    FillLevelHistograms( Stop_00_htjets150, pass && pass1_2_jet);

    


    //--------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 50 + Has2Leptons
     
//    pass1 &= MET.Et() > 50.;    
//    pass1_1 &= MET.Et() > 50.;    	
      pass1_jet &= MET.Et() > 50.;
      pass1_1_jet &= MET.Et() > 50.;
  
//    FillLevelHistograms(Stop_00_ht260Met50, pass && pass1);
//    FillLevelHistograms(Stop_00_ht300Met50, pass && pass1_1);
      FillLevelHistograms( Stop_00_htjets100Met50, pass && pass1_jet);
      FillLevelHistograms( Stop_00_htjets125Met50, pass && pass1_1_jet);
	 
      pass1_2_jet &= MET.Et() > 50.;
      FillLevelHistograms( Stop_00_htjets150Met50, pass && pass1_2_jet);

    //-------------------------------------------------------------------------
    // Basics + Has1BJet
   
    bool pass2 = pass && (_njet > 1);

    //pass &= (njet > 1);
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! pass &= (_nbjet20loose > 0);

    FillLevelHistograms(Stop_01_Has2Jets, pass2);

    pass2 &= (_nbjet30csvv2m > 0);

    FillLevelHistograms(Stop_02_Has1BJet, pass2);

    FillLevelHistograms(Stop_02_Has2BJet, pass2 && (_nbjet30csvv2m>=2));

    pass2 &= _htjets > 150.;    

    FillLevelHistograms(Stop_02_Has1BJetHtJets150, pass2);
    
    //--------------------------------------------------------------------------
    // Basics + met > 50 + Has2Leptons

    bool pass_met50 = MET.Et() > 50.;

    bool pass3 = pass && pass_met50;

    FillLevelHistograms(Stop_00_Met50, pass3);
    //--------------------------------------------------------------------------
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillAnalysisHistograms(int ichannel,
					 int icut,
					 int ijet)
{
  h_2ht[ichannel][icut][ijet]->Fill(_ht, _htjets, _event_weight);
  h_dyll[ichannel][icut][ijet]->Fill(_dyll, _event_weight);
  h_mllbb[ichannel][icut][ijet]->Fill(_mllbb, _event_weight);
  h_dym[ichannel][icut][ijet]->Fill(_mllbb, _dyll, _event_weight);
  h_dphimetjet[ichannel][icut][ijet]->Fill(_dphimetjet, _event_weight);
//h_dphilj[ichannel][icut][ijet]->Fill(_dphilj, _event_weight);
  h_meff[ichannel][icut][ijet]->Fill(_meff, _event_weight);
  h_ptbll[ichannel][icut][ijet]->Fill(_ptbll, _event_weight);
  h_dphimetptbll[ichannel][icut][ijet]->Fill(_dphimetptbll, _event_weight);
  h_mt2ll[ichannel][icut][ijet]->Fill(_mt2ll, _event_weight);
  h_mt2bb[ichannel][icut][ijet]->Fill(_mt2bb, _event_weight);
  h_mt2lblb[ichannel][icut][ijet]->Fill(_mt2lblb, _event_weight);
  
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillLevelHistograms(int  icut,
				      bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
