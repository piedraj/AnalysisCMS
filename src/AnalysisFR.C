#define AnalysisFR_cxx
#include "../include/AnalysisFR.h"

AnalysisFR::AnalysisFR(TTree* tree) : AnalysisCMS(tree)
{
  SetSaveMinitree(false);
}

void AnalysisFR::Loop(TString analysis, TString filename, float luminosity)
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

      for (int i=e; i<=m; i++) { // ?

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);

	h_loose [i][j][k] = new TH1D("h_loose" + suffix, "", 100, 0, 100); 
	h_tight [i][j][k] = new TH1D("h_tight" + suffix, "", 100, 0, 100); 

      }
    }
  }

  root_output->cd();

  // Loop over events
  //--------------------------------------------------------------------------

  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    // Analysis
    //--------------------------------------------------------------------------

    //    if (!std_vector_trigger_special[27]) continue;

    if (_nlepton != 1) continue;

    if (njet < 1) continue;

    if (AnalysisJets[0].v.DeltaR(Lepton1.v) < 1) continue;

    _nelectron = 0;
    
    if (abs(Lepton1.flavour == ELECTRON_FLAVOUR)) _nelectron++;

    if (_nelectron == 0) _channel = m;
    else if (_nelectron == 1) _channel = e;
    
    // Selection
    //--------------------------------------------------------------------------
    
    GetMt(Lepton1, _mtw);
    
    bool pass = true;
    
    pass &= (MET.Et() < 20.);
    pass &= (_mtw < 20.);
    
    FillLevelHistograms(FR_00_QCD, pass);
    
  }

  EndJob();

}

//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{

  if (Lepton1.type == 0) {
    h_loose       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight);
  } else if (Lepton1.type == 1) {
    h_tight       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight);
  }

  //  if (ichannel != l) FillAnalysisHistograms(l, icut, ijet);
}

//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;
  
  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
  
  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}


