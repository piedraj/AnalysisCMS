#define AnalysisWZ_cxx
#include "../include/AnalysisWZ.h"


//------------------------------------------------------------------------------
// AnalysisWZ
//------------------------------------------------------------------------------
AnalysisWZ::AnalysisWZ(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisWZ::Loop(TString analysis, TString filename, float luminosity)
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

      for (int i=eee; i<=lll; i++) {

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);

	h_m3l       [i][j][k] = new TH1D("h_m3l"        + suffix, "", 2000, 0, 2000);
	h_mtw       [i][j][k] = new TH1D("h_mtw"        + suffix, "", 1000, 0, 1000);
	h_zl1pt     [i][j][k] = new TH1D("h_zl1pt"      + suffix, "", 1000, 0, 1000);
	h_zl2pt     [i][j][k] = new TH1D("h_zl2pt"      + suffix, "", 1000, 0, 1000);
	h_wlpt      [i][j][k] = new TH1D("h_wlpt"       + suffix, "", 1000, 0, 1000);
	h_wlzldeltar[i][j][k] = new TH1D("h_wlzldeltar" + suffix, "",  100, 0,    5);
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
    if (_nlepton < 3) continue;

    if (AnalysisLeptons[0].v.Pt() < 10.) continue;
    if (AnalysisLeptons[1].v.Pt() < 10.) continue;
    if (AnalysisLeptons[2].v.Pt() < 10.) continue;

    // This requirement should be applied on a loose lepton
    if (_nlepton > 3 && AnalysisLeptons[3].v.Pt() > 10.) continue;

    _nelectron = 0;

    for (int i=0; i<3; i++)
      {
	if (abs(AnalysisLeptons[i].flavour) == ELECTRON_FLAVOUR) _nelectron++;
      }

    if      (_nelectron == 3) _channel = eee;
    else if (_nelectron == 2) _channel = eem;
    else if (_nelectron == 1) _channel = emm;
    else if (_nelectron == 0) _channel = mmm;


    // Make Z and W candidates
    //--------------------------------------------------------------------------
    _m2l = -999;

    for (UInt_t i=0; i<3; i++) {
    
      for (UInt_t j=i+1; j<3; j++) {

	if (AnalysisLeptons[i].flavour + AnalysisLeptons[j].flavour != 0) continue;

	float inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	if (_m2l < 0 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

	  _m2l = inv_mass;

	  ZLepton1 = AnalysisLeptons[i];
	  ZLepton2 = AnalysisLeptons[j];
	
	  for (UInt_t k=0; k<3; k++) {
	  
	    if (k == i) continue;
	    if (k == j) continue;

	    WLepton = AnalysisLeptons[k];
	  }
	}
      }
    }


    // WZ selection
    //--------------------------------------------------------------------------
    if (_m2l < 0) continue;

    _m3l  = (ZLepton1.v + ZLepton2.v + WLepton.v).M();
    _pt2l = (ZLepton1.v + ZLepton2.v).Pt();
  
    GetMt(WLepton, _mtw);

    bool pass = true;

    FillLevelHistograms(WZ_00_Exactly3Leptons, pass);

    pass &= (_m2l > 76. && _m2l < 106.);
    pass &= (ZLepton1.v.Pt() > 20.);

    FillLevelHistograms(WZ_01_HasZ, pass);

    pass &= (WLepton.v.Pt() >  20.);
    pass &= (MET.Et()       >  30.);
    pass &= (_m3l           > 100.);
    
    pass &= ((WLepton.v + ZLepton1.v).M() > 4.);
    pass &= ((WLepton.v + ZLepton2.v).M() > 4.);

    FillLevelHistograms(WZ_02_HasW, pass);

    pass &= (_nbjet20cmvav2t == 0);
    
    FillLevelHistograms(WZ_03_BVeto, pass);


    // Z+jets enriched region
    //--------------------------------------------------------------------------
    pass = (_m3l > 100.);

    pass &= ((WLepton.v  + ZLepton1.v).M() > 4.);
    pass &= ((WLepton.v  + ZLepton2.v).M() > 4.);
    pass &= ((ZLepton1.v + ZLepton2.v).M() > 4.);

    pass &= (_mtw < 50.);
    pass &= (MET.Et() < 30.);

    FillLevelHistograms(WZ_04_ZRegion, pass);


    // Top enriched region
    //--------------------------------------------------------------------------
    pass = (_m3l > 100.);

    pass &= ((WLepton.v  + ZLepton1.v).M() > 4.);
    pass &= ((WLepton.v  + ZLepton2.v).M() > 4.);
    pass &= ((ZLepton1.v + ZLepton2.v).M() > 4.);

    pass &= (_m2l < 89. || _m2l > 93.);
    pass &= (_nbjet20cmvav2l > 0.);

    FillLevelHistograms(WZ_05_TopRegion, pass);

    if (pass && _writeminitree) minitree->Fill();
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{
  float wlzl1deltar = WLepton.v.DeltaR(ZLepton1.v);
  float wlzl2deltar = WLepton.v.DeltaR(ZLepton2.v);
  float wlzldeltar  = min(wlzl1deltar, wlzl2deltar);

  h_m3l       [ichannel][icut][ijet]->Fill(_m3l,            _event_weight);
  h_mtw       [ichannel][icut][ijet]->Fill(_mtw,            _event_weight);
  h_zl1pt     [ichannel][icut][ijet]->Fill(ZLepton1.v.Pt(), _event_weight);
  h_zl2pt     [ichannel][icut][ijet]->Fill(ZLepton2.v.Pt(), _event_weight);
  h_wlpt      [ichannel][icut][ijet]->Fill(WLepton.v.Pt(),  _event_weight);
  h_wlzldeltar[ichannel][icut][ijet]->Fill(wlzldeltar,      _event_weight);

  if (ichannel != lll) FillAnalysisHistograms(lll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
