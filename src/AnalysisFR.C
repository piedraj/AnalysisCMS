#define AnalysisFR_cxx
#include "../include/AnalysisFR.h"

//------------------------------------------------------------------------------
// AnalysisFR
//------------------------------------------------------------------------------

AnalysisFR::AnalysisFR(TTree* tree) : AnalysisCMS(tree)
{
  SetSaveMinitree(false);
}

//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------

void AnalysisFR::Loop(TString analysis, TString filename, float luminosity)
{
  printf(" Debug [0]\n");
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

      for (int i=e; i<=m; i++) { 

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
  printf(" Debug [1]\n");

  //  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
  for (Long64_t jentry=0; jentry<100;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    // Analysis
    //--------------------------------------------------------------------------

    //    if (!std_vector_trigger_special[27]) continue;
  printf(" Debug [2]\n");

    if (_nlepton != 1) continue;
  printf(" Debug [3]\n");

    if (njet < 1) continue;
  printf(" Debug [4]\n");

    if (AnalysisJets[0].v.DeltaR(Lepton1.v) < 1) continue;
  printf(" Debug [5]\n");

    _nelectron = 0;
    
    if (abs(Lepton1.flavour == ELECTRON_FLAVOUR)) _nelectron++;
  printf(" Debug [6]\n");

    if (_nelectron == 0) _channel = m;
    else if (_nelectron == 1) _channel = e;
    
    // Selection
    //--------------------------------------------------------------------------
    
    GetMt(Lepton1, _mtw);
  printf(" Debug [7]\n");
    
    bool pass = true;
    
    pass &= (MET.Et() < 20.);
    pass &= (_mtw < 20.);
  printf(" Debug [8]\n");
    
      FillLevelHistograms(FR_00_QCD, pass);
  printf(" Debug [9]\n");
    
    if (pass && _saveminitree) minitree->Fill();
    printf(" Debug [10]\n");
  }
  printf(" Debug [11]\n");
  EndJob();
  printf(" Debug [12]\n");
}

//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillAnalysisHistograms(int ichannel,
					int icut,
					int ijet)
{

  //  if (Lepton1.type == 0) {
    h_loose       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight);
    //} else if (Lepton1.type == 1) {
    h_tight       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight);
    //}

  //  if (ichannel != l) FillAnalysisHistograms(l, icut, ijet);
}

//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisFR::FillLevelHistograms(int  icut,
				     bool pass)
{
  if (!pass) return;
  
  //  FillHistograms(_channel, icut, _jetbin);
  //  FillHistograms(_channel, icut, njetbin);
  
    FillAnalysisHistograms(_channel, icut, _jetbin);
    FillAnalysisHistograms(_channel, icut, njetbin);
}


