#define AnalysisMonoH_cxx
#include "../include/AnalysisMonoH.h"


//------------------------------------------------------------------------------
// AnalysisMonoH
//------------------------------------------------------------------------------
AnalysisMonoH::AnalysisMonoH(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetWriteMinitree(true);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisMonoH::Loop(TString analysis, TString filename, float luminosity)
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

	h_fullpmet         [i][j][k] = new TH1D("h_fullpmet"        + suffix, "", 1000, 0.,  1000);
	h_trkpmet          [i][j][k] = new TH1D("h_trkpmet"         + suffix, "", 1000, 0.,  1000);
        h_deltarl1met      [i][j][k] = new TH1D("h_deltarl1met"     + suffix, "",  100, 0.,     5);
        h_deltarl2met      [i][j][k] = new TH1D("h_deltarl2met"     + suffix, "",  100, 0.,     5);
        h_deltarllmet      [i][j][k] = new TH1D("h_deltarllmet"     + suffix, "",  100, 0.,     5);
	h_deltarjet1met    [i][j][k] = new TH1D("h_deltarjet1met"   + suffix, "",  100, 0.,     5);
	h_deltarjet2met    [i][j][k] = new TH1D("h_deltarjet2met"   + suffix, "",  100, 0.,     5);
	h_deltarjj         [i][j][k] = new TH1D("h_deltarjj"        + suffix, "",  100, 0.,     5);
	h_deltarjjmet      [i][j][k] = new TH1D("h_deltarjjmet"     + suffix, "",  100, 0.,     5);
	h_deltarlep1jet1   [i][j][k] = new TH1D("h_deltarlep1jet1"  + suffix, "",  100, 0.,     5);
	h_deltarlep1jet2   [i][j][k] = new TH1D("h_deltarlep1jet2"  + suffix, "",  100, 0.,     5);
	h_deltarlep2jet1   [i][j][k] = new TH1D("h_deltarlep2jet1"  + suffix, "",  100, 0.,     5);
	h_deltarlep2jet2   [i][j][k] = new TH1D("h_deltarlep2jet2"  + suffix, "",  100, 0.,     5);
       	h_mllstar          [i][j][k] = new TH1D("h_mllstar"         + suffix, "", 3000, 0.,  3000);
       	//h_mnunu            [i][j][k] = new TH1D("h_mnunu"           + suffix, "",   10, 0.,    10);
	h_mr             [i][j][k] = new TH1D("h_mr"              + suffix, "", 2000, 0.,  2000);

	//h_met_m2l         [i][j][k] = new TH2D("h_met_m2l"        + suffix, "", 200,  0,  2000,  100,   0, 200);
	//h_met_deltaphill  [i][j][k] = new TH2D("h_met_deltaphill" + suffix, "", 200,  0,  2000,  100,   0,   5);     
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
    //if (Lepton1.flavour * Lepton2.flavour > 0) continue;
    //if (Lepton1.v.Pt() < 20.) continue;
    //if (Lepton2.v.Pt() < 20.) continue;

    // Let's trust our ntuples
    //--------------------------------------------------------------------------
    if (std_vector_lepton_flavour->at(0)*std_vector_lepton_flavour->at(1) > 0) continue;
    if (std_vector_lepton_pt->at(0) < 20.) continue; 
    if (std_vector_lepton_pt->at(1) < 20.) continue; 
    if (std_vector_lepton_pt->at(2) > 10.) continue; 

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

    // WW cuts   
    FillLevelHistograms(MonoH_00_Has2Leptons, pass);

    pass &= (mll > 12.);
    FillLevelHistograms(MonoH_01_Mll, pass);
    
    pass &= (MET.Et() > 20.);
    FillLevelHistograms(MonoH_02_PfMet, pass);

    bool pass_zveto = (_nelectron == 1 || fabs(mll - Z_MASS) > 15.);
    FillLevelHistograms(MonoH_03_ZVeto, pass && pass_zveto);

    pass &= (mpmet > 45. || (_nelectron == 1 && mpmet > 45.));
    FillLevelHistograms(MonoH_04_MpMet, pass && pass_zveto);

    pass &= (_passdphiveto);
    FillLevelHistograms(MonoH_05_DPhiVeto, pass && pass_zveto);

    pass &= (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);
    FillLevelHistograms(MonoH_06_Ptll, pass && pass_zveto);

    bool passTopCR = pass && _nbjet20cmvav2l == 1;

    pass &= (_nbjet20cmvav2l == 0);
    FillLevelHistograms(MonoH_07_BVeto, pass && pass_zveto);

    if (_writeminitree && pass && pass_zveto) minitree->Fill();

    //ZH->4l Control Region
    if (AnalysisLeptons[2].v.Pt() > 0 || AnalysisLeptons[3].v.Pt() > 0 ){
      //cout<<"I passed! :)"<<endl;
      
      bool passZHCR = ( (fabs(_mll13 - Z_MASS) < 15. || fabs(_mll23 - Z_MASS) < 15. || fabs(_mll14 - Z_MASS) < 15. || fabs(_mll24 - Z_MASS) < 15. || fabs(_mll34 - Z_MASS) < 15.) && AnalysisLeptons[2].v.Pt() > 20. && AnalysisLeptons[3].v.Pt() > 20.);
      
      FillLevelHistograms(MonoH_08_ZHCR, passZHCR);
      // cout<<"Lepton 3 pT = "<<std_vector_lepton_pt->at(2)<<endl;
      // cout<<"Lepton 4 pT = "<<std_vector_lepton_pt->at(3)<<endl;
      // cout<<"-------------------------------------------"<<endl;
    }

    FillLevelHistograms(MonoH_09_TopCR, passTopCR && pass_zveto);

    //    pass &= (!_foundsoftmuon);
    //    FillLevelHistograms(MonoH_08_SoftMu, pass && pass_zveto);

    // monoH cuts                                                         
    // bool pass_monoh = (pass && pass_zveto);
    // bool pass_drll = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);

    // FillLevelHistograms(MonoH_103_CR, pass_monoh && !pass_drll);

    // pass_monoh &= (_mc < 100.);
    // FillLevelHistograms(MonoH_100_Mc, pass_monoh);
   
    // pass_monoh &= pass_drll;                                                                       
    // FillLevelHistograms(MonoH_101_DRll, pass_monoh); 
  
    // pass_monoh &= (mpmet > 60.);
    // FillLevelHistograms(MonoH_102_MpMet, pass_monoh);

    // pass_monoh &= (mpmet > 100.);
    // FillLevelHistograms(MonoH_09_mpmet100, pass_monoh);

    // pass_monoh &= (mth > 200.);
    // FillLevelHistograms(MonoH_10_mth200, pass_monoh);

    // pass_monoh &= (Lepton1.v.DeltaPhi(MET) > 2.6);
    // FillLevelHistograms(MonoH_11_dphil1met, pass_monoh);

    // pass_monoh &= (Lepton2.v.DeltaPhi(MET) > 2.6);
    // FillLevelHistograms(MonoH_12_dphil2met, pass_monoh);

    // pass_monoh &= (drll < 0.8);
    // FillLevelHistograms(MonoH_13_deltarll, pass_monoh);

    // pass_monoh &= (mtw1 > 160.);
    // FillLevelHistograms(MonoH_14_mtw1, pass_monoh);

    // pass_monoh &= (mtw2 > 100.);
    // FillLevelHistograms(MonoH_15_mtw2, pass_monoh);

    // pass_monoh &= (metTtrk > 100.);
    // FillLevelHistograms(MonoH_16_trkmet, pass_monoh);

  }

  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisMonoH::FillAnalysisHistograms(int ichannel,
					   int icut,
					   int ijet)
{

  float m_r = sqrt(pow(Lepton1.v(3)+Lepton2.v(3),2)-pow(Lepton1.v(2)+Lepton2.v(2),2));
  
  h_fullpmet        [ichannel][icut][ijet]->Fill(_fullpmet,         _event_weight);
  h_trkpmet         [ichannel][icut][ijet]->Fill(_trkpmet,          _event_weight);
  h_deltarl1met     [ichannel][icut][ijet]->Fill(_deltarl1met,      _event_weight);
  h_deltarl2met     [ichannel][icut][ijet]->Fill(_deltarl2met,      _event_weight);
  h_deltarllmet     [ichannel][icut][ijet]->Fill(_deltarllmet,      _event_weight);

  if (_njet > 0){
    h_deltarjet1met   [ichannel][icut][ijet]->Fill(_deltarjet1met,    _event_weight);
    h_deltarlep2jet1  [ichannel][icut][ijet]->Fill(_deltarlep2jet1,   _event_weight); 
    h_deltarlep1jet1  [ichannel][icut][ijet]->Fill(_deltarlep1jet1,   _event_weight); 
  }                                                          
  
  if (_njet > 1){
    h_deltarjet2met   [ichannel][icut][ijet]->Fill(_deltarjet2met,    _event_weight);
    h_deltarjj        [ichannel][icut][ijet]->Fill(_deltarjj,         _event_weight);
    h_deltarjjmet     [ichannel][icut][ijet]->Fill(_deltarjjmet,      _event_weight);
    h_deltarlep1jet2  [ichannel][icut][ijet]->Fill(_deltarlep1jet2,   _event_weight); 
    h_deltarlep2jet2  [ichannel][icut][ijet]->Fill(_deltarlep2jet2,   _event_weight);  
  }

  h_mllstar         [ichannel][icut][ijet]->Fill(_mllstar,          _event_weight);
  //h_mnunu           [ichannel][icut][ijet]->Fill(_mnunu,            _event_weight);
  h_mr              [ichannel][icut][ijet]->Fill(m_r,               _event_weight);

  //h_met_m2l        [ichannel][icut][ijet] ->Fill(pfType1Met,   _m2l,        _event_weight); 
  //h_met_deltaphill [ichannel][icut][ijet] ->Fill(pfType1Met, dphill,        _event_weight);  

  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisMonoH::FillLevelHistograms(int  icut,
					bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}
