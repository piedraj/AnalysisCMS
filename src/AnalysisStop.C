#define AnalysisStop_cxx
#include "../include/AnalysisStop.h"
#include "../include/lester_mt2_bisect.h"

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

        h_htjets[i][j][k] = new TH1D("h_htjets" + suffix, "", 300, 0, 800);
	
	h_2ht[i][j][k] = new TH2F("h_2ht" + suffix, "", 300, 0, 800, 300, 0, 800);

	//h_dphill[i][j][k] = new TH1D("h_phill" + suffix, "", 100, 0., 3.2);
	//h_mll[i][j][k] = new TH1D("h_mll" + suffix, "", 200, 0., 1000.);
	h_dyll[i][j][k] = new TH1D("h_dyll" + suffix, "", 100, 0, 5);
	h_mllbb[i][j][k] = new TH1D("h_mllbb" + suffix, "", 200, 0, 1000);
	h_dym[i][j][k] = new TH2F("h_dym" + suffix, "", 200, 0, 1000, 100, 0, 5);
	h_dphimetjet[i][j][k] = new TH1D("h_dphimetjet" + suffix, "", 100, 0., 3.2);
        h_dphimetlep1[i][j][k] = new TH1D("h_dphimetlep1" + suffix, "", 100, 0., 3.2);
        //h_dphilj[i][j][k] = new TH1D("h_dphilj" + suffix, "", 100, 0., 3.2);
        h_meff[i][j][k] = new TH1D("h_meff" + suffix, "", 200, 0, 1000);
        h_ptbll[i][j][k] = new TH1D("h_ptbll" + suffix, "", 200, 0, 1000);
        h_dphimetptbll[i][j][k] = new TH1D("h_dphimetptbll" + suffix, "", 100, 0., 3.2);
	h_mt2ll[i][j][k] = new TH1D("h_mt2ll" + suffix, "", 200, 0., 1000);
	h_mt2bb[i][j][k] = new TH1D("h_mt2bb" + suffix, "", 200, 0., 1000);
	h_mt2lblb[i][j][k] = new TH1D("h_mt2lblb" + suffix, "", 200, 0., 1000);

      }
    }
  }

  root_output->cd();

  asymm_mt2_lester_bisect::disableCopyrightMessage();

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

    _dyll = fabs(_lep1eta - _lep2eta);

    _ptbll = (Lepton1.v + Lepton2.v + MET).Pt();
    _dphimetptbll = fabs((Lepton1.v + Lepton2.v + MET).DeltaPhi(MET));

    float MaxLeptonPt = _lep1pt;
    if (_lep2pt>MaxLeptonPt) cout << "Wrong lepton ordering " << _lep1pt << " " << _lep2pt << endl;
    _dphimetlep1 = fabs(Lepton1.v.DeltaPhi(MET)); 

    _mt2ll = ComputeMT2(Lepton1.v, Lepton2.v, MET);

    int _njet = AnalysisJets.size();
    
    _dphimetjet = -0.1; double MinDeltaPhiMetJet = 999.;
    for (int ijet = 0; ijet<_njet; ijet++) {
      
      double ThisDeltaPhiMetJet = fabs(AnalysisJets[ijet].v.DeltaPhi(MET));
      if (ThisDeltaPhiMetJet<MinDeltaPhiMetJet) {
	
	MinDeltaPhiMetJet = ThisDeltaPhiMetJet;
	_dphimetjet = ThisDeltaPhiMetJet;
	
      }
      
    }

    _meff = -0.1;
    _mllbb = -0.1;
    _mt2bb = -0.1;
    _mt2lblb = -0.1;
    if (_njet>=2) {
      
      _meff = MET.Pt() + Lepton1.v.Pt() + Lepton2.v.Pt() + AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt();

      int bjetindex[2] = {-1, -1};
      if (_nbjet30csvv2m>=1) {
	
	int nbjetfound = 0, nbjetfromleading = 0;
	for (int ijet = 0; ijet<_njet; ijet++) {
	  if (nbjetfound<2) {
	    if (AnalysisJets[ijet].csvv2ivf>CSVv2M) {
	      bjetindex[1] = bjetindex[0];
	      bjetindex[nbjetfound] = ijet;
	      nbjetfound++;
	    } else if (nbjetfromleading<1) {
	      bjetindex[nbjetfound] = ijet;
	      nbjetfromleading++;
	    }  
	  }
	}
	if (bjetindex[0]>=0 && bjetindex[1]>=0) {
	  
	  _mllbb = (Lepton1.v + Lepton2.v + AnalysisJets[bjetindex[0]].v + AnalysisJets[bjetindex[1]].v).M();

	  _mt2bb = ComputeMT2(AnalysisJets[bjetindex[0]].v, AnalysisJets[bjetindex[1]].v, Lepton1.v + Lepton2.v + MET, 1);
	    
	  _mt2lblb = ComputeMT2(AnalysisJets[bjetindex[0]].v + Lepton1.v, AnalysisJets[bjetindex[1]].v + Lepton2.v, MET);
	  double CombinatorialMT2lblb = ComputeMT2(AnalysisJets[bjetindex[0]].v + Lepton2.v, AnalysisJets[bjetindex[1]].v + Lepton1.v, MET, 2);
	  if (CombinatorialMT2lblb<_mt2lblb) _mt2lblb = CombinatorialMT2lblb;
	  
	}
	
      }
      
    }
    
    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;

    FillLevelHistograms(Stop_00_Has2Leptons, pass);    
   
    //-------------------------------------------------------------------------
    // Basics Stop

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

    pass2 &= (_nbjet30medium > 0);

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
  h_htjets[ichannel][icut][ijet]->Fill(_htjets, _event_weight);

  h_2ht[ichannel][icut][ijet]->Fill(_ht, _htjets, _event_weight);

  //h_dphill[ichannel][icut][ijet]->Fill(_dphill, _event_weight);
  //h_mll[ichannel][icut][ijet]->Fill(_m2l, _event_weight);
  h_dyll[ichannel][icut][ijet]->Fill(_dyll, _event_weight);
  h_mllbb[ichannel][icut][ijet]->Fill(_mllbb, _event_weight);
  h_dym[ichannel][icut][ijet]->Fill(_mllbb, _dyll, _event_weight);
  h_dphimetjet[ichannel][icut][ijet]->Fill(_dphimetjet, _event_weight);
  h_dphimetlep1[ichannel][icut][ijet]->Fill(_dphimetlep1, _event_weight);
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


//------------------------------------------------------------------------------
// ComputeMT2
//------------------------------------------------------------------------------
double AnalysisStop::ComputeMT2(TLorentzVector VisibleA, TLorentzVector VisibleB, 
				TLorentzVector Invisible, int MT2Type, double MT2Precision) 
{
  
  double mVisA = fabs(VisibleA.M()); // mass of visible object on side A.  Must be >=0.
  double mVisB = fabs(VisibleB.M()); // mass of visible object on side B.  Must be >=0.

  double chiA = 0.; // hypothesised mass of invisible on side A.  Must be >=0.
  double chiB = 0.; // hypothesised mass of invisible on side B.  Must be >=0.
  
  if (MT2Type==1) {

    mVisA = mVisB = 5.;
    chiA = chiB = 80.;

  }

  double pxA = VisibleA.Px(); // x momentum of visible object on side A.
  double pyA = VisibleA.Py(); // y momentum of visible object on side A.
  
  double pxB = VisibleB.Px(); // x momentum of visible object on side B.
  double pyB = VisibleB.Py(); // y momentum of visible object on side B.
  
  double pxMiss = Invisible.Px(); // x component of missing transverse momentum.
  double pyMiss = Invisible.Py(); // y component of missing transverse momentum.
  
  double desiredPrecisionOnMt2 = MT2Precision; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.
  
  // asymm_mt2_lester_bisect::disableCopyrightMessage();
  
  double MT2 =  asymm_mt2_lester_bisect::get_mT2(
						 mVisA, pxA, pyA,
						 mVisB, pxB, pyB,
						 pxMiss, pyMiss,
						 chiA, chiB,
						 desiredPrecisionOnMt2);

  return MT2;
  
}
