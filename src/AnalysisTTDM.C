#define AnalysisTTDM_cxx
#include "../include/AnalysisTTDM.h"


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
AnalysisTTDM::AnalysisTTDM(TTree* tree) : AnalysisCMS(tree) {}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisTTDM::Loop(TString analysis, TString filename, float luminosity)
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
      }
    }
  }

  root_output->cd();


  // MET filters histograms
  //----------------------------------------------------------------------------
  met_Flag_none                               = new TH1D("met_Flag_none",                               "", 3000, 0, 3000);
  met_Flag_all                                = new TH1D("met_Flag_all",                                "", 3000, 0, 3000);
  met_Flag_trackingFailureFilter              = new TH1D("met_Flag_trackingFailureFilter",              "", 3000, 0, 3000);
  met_Flag_goodVertices                       = new TH1D("met_Flag_goodVertices",                       "", 3000, 0, 3000);
  met_Flag_CSCTightHaloFilter                 = new TH1D("met_Flag_CSCTightHaloFilter",                 "", 3000, 0, 3000);
  met_Flag_trkPOGFilters                      = new TH1D("met_Flag_trkPOGFilters",                      "", 3000, 0, 3000);
  met_Flag_trkPOG_logErrorTooManyClusters     = new TH1D("met_Flag_trkPOG_logErrorTooManyClusters",     "", 3000, 0, 3000);
  met_Flag_EcalDeadCellTriggerPrimitiveFilter = new TH1D("met_Flag_EcalDeadCellTriggerPrimitiveFilter", "", 3000, 0, 3000);
  met_Flag_ecalLaserCorrFilter                = new TH1D("met_Flag_ecalLaserCorrFilter",                "", 3000, 0, 3000);
  met_Flag_trkPOG_manystripclus53X            = new TH1D("met_Flag_trkPOG_manystripclus53X",            "", 3000, 0, 3000);
  met_Flag_eeBadScFilter                      = new TH1D("met_Flag_eeBadScFilter",                      "", 3000, 0, 3000);
  met_Flag_METFilters                         = new TH1D("met_Flag_METFilters",                         "", 3000, 0, 3000);
  met_Flag_HBHENoiseFilter                    = new TH1D("met_Flag_HBHENoiseFilter",                    "", 3000, 0, 3000);
  met_Flag_trkPOG_toomanystripclus53X         = new TH1D("met_Flag_trkPOG_toomanystripclus53X",         "", 3000, 0, 3000);
  met_Flag_hcalLaserEventFilter               = new TH1D("met_Flag_hcalLaserEventFilter",               "", 3000, 0, 3000);


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();


    // MET filters histograms
    //--------------------------------------------------------------------------
    met_Flag_none->Fill(MET.Et());

    if (std_vector_trigger_special->at(0)  > 0) met_Flag_trackingFailureFilter             ->Fill(MET.Et());
    if (std_vector_trigger_special->at(1)  > 0) met_Flag_goodVertices                      ->Fill(MET.Et());
    if (std_vector_trigger_special->at(2)  > 0) met_Flag_CSCTightHaloFilter                ->Fill(MET.Et());
    if (std_vector_trigger_special->at(3)  > 0) met_Flag_trkPOGFilters                     ->Fill(MET.Et());
    if (std_vector_trigger_special->at(4)  > 0) met_Flag_trkPOG_logErrorTooManyClusters    ->Fill(MET.Et());
    if (std_vector_trigger_special->at(5)  > 0) met_Flag_EcalDeadCellTriggerPrimitiveFilter->Fill(MET.Et());
    if (std_vector_trigger_special->at(6)  > 0) met_Flag_ecalLaserCorrFilter               ->Fill(MET.Et());
    if (std_vector_trigger_special->at(7)  > 0) met_Flag_trkPOG_manystripclus53X           ->Fill(MET.Et());
    if (std_vector_trigger_special->at(8)  > 0) met_Flag_eeBadScFilter                     ->Fill(MET.Et());
    if (std_vector_trigger_special->at(9)  > 0) met_Flag_METFilters                        ->Fill(MET.Et());
    if (std_vector_trigger_special->at(10) > 0) met_Flag_HBHENoiseFilter                   ->Fill(MET.Et());
    if (std_vector_trigger_special->at(11) > 0) met_Flag_trkPOG_toomanystripclus53X        ->Fill(MET.Et());
    if (std_vector_trigger_special->at(12) > 0) met_Flag_hcalLaserEventFilter              ->Fill(MET.Et());

    met_Flag_all->Fill(MET.Et());


    // Analysis
    //--------------------------------------------------------------------------
    if (!trigger) continue;

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;

    _nlepton   = 2;  // Redefine _nlepton
    _nelectron = 0;  // Redefine _nelectron

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    //    _m2l  = mll;   // [Work in progress] Needs l2sel
    //    _pt2l = ptll;  // [Work in progress] Needs l2sel
    _m2l  = 999;
    _pt2l = 999;


    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;

    FillLevelHistograms(TTDM_00_Has2Leptons, pass);

    bool pass_sf = (_nelectron != 1 && fabs(_m2l - Z_MASS) > 15.);
    bool pass_df = (_nelectron == 1);

    pass &= (_m2l > 20.);
    pass &= (pass_sf || pass_df);

    FillLevelHistograms(TTDM_01_ZVeto, pass);

    pass &= (njet > 1);

    if (njet < 2) continue;

    FillLevelHistograms(TTDM_02_Has2Jets, pass);

    pass &= (Lepton1.v.Pt() + Lepton2.v.Pt() > 120.);

    FillLevelHistograms(TTDM_03_LepPtSum, pass);

    pass &= (AnalysisJets[0].v.Pt() + AnalysisJets[1].v.Pt() < 400.);

    FillLevelHistograms(TTDM_04_JetPtSum, pass);

    pass &= (Lepton1.v.DeltaPhi(Lepton2.v) < 2.);

    FillLevelHistograms(TTDM_05_LepDeltaPhi, pass);

    pass &= (MET.Et() > 320.);

    FillLevelHistograms(TTDM_06_MET, pass);
  }


  EndJob();
}


//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillAnalysisHistograms(int ichannel,
					  int icut,
					  int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisTTDM::FillLevelHistograms(int  icut,
				       bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  //  FillAnalysisHistograms(_channel, icut, _jetbin);
  //  FillAnalysisHistograms(_channel, icut, njetbin);
}
