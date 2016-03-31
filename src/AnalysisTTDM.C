#define AnalysisTTDM_cxx
#include "../include/AnalysisTTDM.h"


//------------------------------------------------------------------------------
// AnalysisTTDM
//------------------------------------------------------------------------------
AnalysisTTDM::AnalysisTTDM(TTree* tree) : AnalysisCMS(tree)
{
  SetSaveMinitree(true);
}


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


  // MET filters histograms and output file
  //----------------------------------------------------------------------------
  ofstream txt_metfilters;

  txt_metfilters.open("txt/" + _analysis + "/" + _sample + "_metfilters.txt");

  for (int j=0; j<nfilter; j++) {

    met_Flag_nocut [j] = new TH1D(Form("met_Flag_nocut_%s",  sfilter[j].Data()), "", 3000, 0, 3000);
    met_Flag_met300[j] = new TH1D(Form("met_Flag_met300_%s", sfilter[j].Data()), "", 3000, 0, 3000);
  }


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
    float truncated_met = (MET.Et() > 2999.5) ? 2999.5 : MET.Et();

    bool pass_all_met_filters;


    // MET filters histograms :: No cut
    //--------------------------------------------------------------------------
    pass_all_met_filters = true;

    met_Flag_nocut[noFilter]->Fill(truncated_met);

    for (int j=HBHENoiseFilter; j<=muonBadTrackFilter; j++) {

      if (std_vector_trigger_special->at(j) > 0) met_Flag_nocut[j]->Fill(truncated_met);
      else pass_all_met_filters = false;
    }
    
    if (pass_all_met_filters) met_Flag_nocut[allFilter]->Fill(truncated_met);


    // MET filters histograms :: MET > 300 GeV
    //--------------------------------------------------------------------------
    if (truncated_met > 300.)
      {
	pass_all_met_filters = true;

	met_Flag_met300[noFilter]->Fill(truncated_met);

	for (int j=HBHENoiseFilter; j<=muonBadTrackFilter; j++) {

	  if (std_vector_trigger_special->at(j) > 0) met_Flag_met300[j]->Fill(truncated_met);
	  else
	    {
	      pass_all_met_filters = false;

	      txt_metfilters << Form("%d:%d:%d:%s\n", run, lumi, event, sfilter[j].Data());
	    }
	}
    
	if (pass_all_met_filters) met_Flag_met300[allFilter]->Fill(truncated_met);
      }


    // Analysis
    //--------------------------------------------------------------------------
    if (!_ismc && run > 258750) continue;  // Luminosity for any blinded analysis

    if (!trigger) continue;

    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 30.) continue;
    if (Lepton2.v.Pt() < 10.) continue;

    if (_nlepton > 2 && AnalysisLeptons[2].v.Pt() > 10.) continue;

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel


    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;

    FillLevelHistograms(TTDM_00_Has2Leptons, pass);

    bool pass_sf = (_nelectron != 1 && fabs(_m2l - Z_MASS) > 15.);
    bool pass_df = (_nelectron == 1);

    pass &= (_m2l > 20.);
    pass &= (pass_sf || pass_df);

    FillLevelHistograms(TTDM_01_ZVeto, pass);

    bool preselection = pass && (njet > 1 && MET.Et() > 50.);
    //    bool preselection = pass && (njet > 0 && MET.Et() > 50.);  // Synchro with Stany

    FillLevelHistograms(TTDM_02_Preselection, preselection);

    if (preselection && _saveminitree) minitree->Fill();

    pass &= (njet > 1);

    FillLevelHistograms(TTDM_03_Has2Jets, pass);

    pass &= (_nbjet20loose > 0);

    FillLevelHistograms(TTDM_04_Has1BJet, pass);

    float _dphillmet = (Lepton1.v + Lepton2.v).DeltaPhi(MET);

    pass &= (fabs(_dphillmet) > 0.6);

    FillLevelHistograms(TTDM_05_DeltaPhi, pass);

    pass &= (MET.Et() > 120.);

    FillLevelHistograms(TTDM_06_MET, pass);
  }


  EndJob();

  txt_metfilters.close();
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
