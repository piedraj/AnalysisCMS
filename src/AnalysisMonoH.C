#define AnalysisMonoH_cxx
#include "../include/AnalysisMonoH.h"


//------------------------------------------------------------------------------
// AnalysisMonoH
//------------------------------------------------------------------------------
AnalysisMonoH::AnalysisMonoH(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisMonoH::Loop(TString analysis, TString filename, float luminosity)
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

	h_metvar_m2l[i][j][k] = new TH2D("h_metvar_m2l" + suffix, "", 4, metvar_bins, 2000, 0, 200);
	h_dphilmet1[i][j][k]  = new TH1D("h_dphilmet1"  + suffix, "", 1000, 0., 10.0);
	h_dphilmet2[i][j][k]  = new TH1D("h_dphilmet2"  + suffix, "", 1000, 0., 10.0);
	h_fullpmet[i][j][k]   = new TH1D("h_fullpmet"   + suffix, "", 1000, 0., 1000);
	h_trkpmet[i][j][k]    = new TH1D("h_trkpmet"    + suffix, "", 1000, 0., 1000);
	h_jetpt1[i][j][k]     = new TH1D("h_jetpt1"     + suffix, "", 1000, 0., 1000);
	// h_metphi[i][j][k]     = new TH1D("h_metphi"     + suffix, "", 1000, 0., 10.0);
	h_lepphi1[i][j][k]    = new TH1D("h_lepphi1"    + suffix, "", 1000, 0., 10.0);
	h_lepphi2[i][j][k]    = new TH1D("h_lepphi2"    + suffix, "", 1000, 0., 10.0);
	h_pt1_pdfUp[i][j][k]  = new TH1D("h_pt1_pdfUp"  + suffix, "", 1000, 0., 1000);
	h_pt1_pdfDown[i][j][k]= new TH1D("h_pt1_pdfDown"+ suffix, "", 1000, 0., 1000);

        // Puesto por Alberto Manjon
        // h_njet       [i][j][k] = new TH1D("h_njet"       + suffix, "",    4, 0,    4);
        // h_nbjet      [i][j][k] = new TH1D("h_nbjet"      + suffix, "",    4, 0,    4);
        // h_pfType1Met    [i][j][k] = new TH1D("h_pfType1Met" + suffix, "", 2000, 0, 2000);
        h_deltarl1met   [i][j][k] = new TH1D("h_deltarl1met"+ suffix, "",  100, 0,    5);
        h_deltarl2met   [i][j][k] = new TH1D("h_deltarl2met"+ suffix, "",  100, 0,    5);
        h_deltarllmet   [i][j][k] = new TH1D("h_deltarllmet"+ suffix, "",  100, 0,    5);
        h_deltaphil1met [i][j][k] = new TH1D("h_deltaphil1met"+ suffix, "",  100, 0,    5);
        h_deltaphil2met [i][j][k] = new TH1D("h_deltaphil2met"+ suffix, "",  100, 0,    5);
        h_deltaphillmet [i][j][k] = new TH1D("h_deltaphillmet"+ suffix, "",  100, 0,    5);
        // h_deltaR_jet_met [i][j][k] = new TH1D("h_deltaR_jet_met"+ suffix, "",  100, 0,  5);
        // Razor Variable
	h_mr            [i][j][k] = new TH1D("h_mr"+ suffix, "",  2000, 0, 2000);          
        // h_met_m2l       [i][j][k] = new TH2D("h_met_m2l" + suffix, "", 200, 0, 2000, 100, 0, 200);
        // h_met_deltaphill[i][j][k] = new TH2D("h_met_deltaphill" + suffix, "", 200, 0, 2000, 100, 0, 5);

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

    // Modificado por Alberto Manjon

    // WW cuts   
    FillLevelHistograms(MonoH_00_Has2Leptons, pass);

    pass &= (mll > 12.);
    FillLevelHistograms(MonoH_01_Mll, pass);
    
    pass &= (MET.Et() > 20.);
    FillLevelHistograms(MonoH_02_PfMet, pass);

    bool pass_zveto = (_nelectron == 1 || fabs(mll - Z_MASS) > 15.);
    FillLevelHistograms(MonoH_03_ZVeto, pass && pass_zveto);

    pass &= (_mpmet > 20. || (_nelectron == 1 && _mpmet > 45.));
    FillLevelHistograms(MonoH_04_MpMet, pass && pass_zveto);

    pass &= (_passdphiveto);
    FillLevelHistograms(MonoH_05_DPhiVeto, pass && pass_zveto);

    pass &= (_nelectron == 1 && ptll > 30. || _nelectron != 1 && ptll > 45.);
    FillLevelHistograms(MonoH_06_Ptll, pass && pass_zveto);

    pass &= (_nbjet20loose == 0);
    FillLevelHistograms(MonoH_07_BVeto, pass && pass_zveto);

    pass &= (!_foundsoftmuon);
    FillLevelHistograms(MonoH_08_SoftMu, pass && pass_zveto);

    // monoH cuts                                                         
    bool pass_monoh = (pass && pass_zveto);
    bool pass_drll = (Lepton1.v.DeltaR(Lepton2.v) < 1.5);

    // FillLevelHistograms(MonoH_103_CR, pass_monoh && !pass_drll);

    // pass_monoh &= (_mc < 100.);
    // FillLevelHistograms(MonoH_100_Mc, pass_monoh);
   
    // pass_monoh &= pass_drll;                                                                       
    // FillLevelHistograms(MonoH_101_DRll, pass_monoh); 
  
    // pass_monoh &= (mpmet > 60.);
    // FillLevelHistograms(MonoH_102_MpMet, pass_monoh);

    // Cortes Puestos por Alberto
    // bool pass_mpmet100 = (mpmet > 100.);
    pass_monoh &= (mpmet > 100.);
    FillLevelHistograms(MonoH_09_mpmet100, pass_monoh);

    // bool pass_mth200 = (mth > 200.);
    pass_monoh &= (mth > 200.);
    FillLevelHistograms(MonoH_10_mth200, pass_monoh);

    // bool pass_dphil1met = (Lepton1.v.DeltaPhi(MET) > 2.6);
    pass_monoh &= (Lepton1.v.DeltaPhi(MET) > 2.6);
    FillLevelHistograms(MonoH_11_dphil1met, pass_monoh);

    // bool pass_dphil2met = (Lepton2.v.DeltaPhi(MET) > 2.6);
    pass_monoh &= (Lepton2.v.DeltaPhi(MET) > 2.6);
    FillLevelHistograms(MonoH_12_dphil2met, pass_monoh);

    // bool pass_deltarll = (drll < 0.8);
    pass_monoh &= (drll < 0.8);
    FillLevelHistograms(MonoH_13_deltarll, pass_monoh);

    // bool pass_mtw1 = (mtw1 > 160.);
    pass_monoh &= (mtw1 > 160.);
    FillLevelHistograms(MonoH_14_mtw1, pass_monoh);

    // bool pass_mtw2 = (mtw2 > 100.);   
    pass_monoh &= (mtw2 > 100.);
    FillLevelHistograms(MonoH_15_mtw2, pass_monoh);

    // bool pass_trkmet = (trkMet > 100.);   
    pass_monoh &= (metTtrk > 100.);
    FillLevelHistograms(MonoH_16_trkmet, pass_monoh);

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

  // Puesto por Alberto Manjon   
  float deltarl1met = fabs(Lepton1.v.DeltaR(MET));
  float deltarl2met = fabs(Lepton2.v.DeltaR(MET));
  float deltarllmet = fabs((Lepton1.v+Lepton2.v).DeltaR(MET));
  float deltaphil1met = fabs(Lepton1.v.DeltaPhi(MET));
  float deltaphil2met = fabs(Lepton2.v.DeltaPhi(MET));
  float deltaphillmet = fabs((Lepton1.v+Lepton2.v).DeltaPhi(MET));

  // Puesto por Alberto Manjon - Razor Variable
  float m_r  = sqrt(pow(Lepton1.v(3)+Lepton2.v(3),2)-pow(Lepton1.v(2)+Lepton2.v(2),2));

  // Puesto por Alberto Manjon
  // h_njet       [ichannel][icut][ijet]->Fill(_njet,       _event_weight);
  // h_nbjet      [ichannel][icut][ijet]->Fill(_nbjet,      _event_weight);
  // h_pfType1Met    [ichannel][icut][ijet]->Fill(pfType1Met, _event_weight);
  h_deltarl1met   [ichannel][icut][ijet]->Fill(deltarl1met, _event_weight);
  h_deltarl2met   [ichannel][icut][ijet]->Fill(deltarl2met, _event_weight);
  h_deltarllmet   [ichannel][icut][ijet]->Fill(deltarllmet, _event_weight);
  h_deltaphil1met [ichannel][icut][ijet]->Fill(deltaphil1met, _event_weight);
  h_deltaphil2met [ichannel][icut][ijet]->Fill(deltaphil2met, _event_weight);
  h_deltaphillmet [ichannel][icut][ijet]->Fill(deltaphillmet, _event_weight);
  // h_deltaR_jet_met [ichannel][icut][ijet]->Fill(deltaR_jet_met, _event_weight);    
  // Razor Variable                                                                                                                  
  h_mr            [ichannel][icut][ijet]->Fill(m_r, _event_weight);  
  // h_met_m2l[ichannel][icut][ijet] ->Fill(pfType1Met, _m2l, _event_weight);
  // h_met_deltaphill[ichannel][icut][ijet] ->Fill(pfType1Met, dphill, _event_weight);

  h_metvar_m2l[ichannel][icut][ijet] ->Fill(_metvar, _m2l, _event_weight);
  h_dphilmet1[ichannel][icut][ijet]  ->Fill(dphilmet1,                   _event_weight);
  h_dphilmet2[ichannel][icut][ijet]  ->Fill(dphilmet2,                   _event_weight);
  h_fullpmet[ichannel][icut][ijet]   ->Fill(_fullpmet,                    _event_weight);
  h_trkpmet[ichannel][icut][ijet]    ->Fill(_trkpmet,                     _event_weight);
  h_jetpt1[ichannel][icut][ijet]     ->Fill(std_vector_jet_pt->at(0),     _event_weight);
  // h_metphi[ichannel][icut][ijet]     ->Fill(pfType1Metphi,                _event_weight);
  h_lepphi1[ichannel][icut][ijet]    ->Fill(std_vector_lepton_phi->at(0), _event_weight);
  h_lepphi2[ichannel][icut][ijet]    ->Fill(std_vector_lepton_phi->at(1), _event_weight);
  h_pt1_pdfUp[ichannel][icut][ijet]  ->Fill(std_vector_lepton_pt->at(0),  _event_weight);
  h_pt1_pdfDown[ichannel][icut][ijet]->Fill(std_vector_lepton_pt->at(0),  _event_weight);

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
