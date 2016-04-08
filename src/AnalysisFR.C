#define AnalysisFR_cxx
#include "../include/AnalysisFR.h"

    float _event_weight_fr = 1.;


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

    for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    // Analysis
    //--------------------------------------------------------------------------

    // Trigger analysis

    bool passTrigger = false;

    printf("Lepton1 : %f    MUON : %f \n", Lepton1.flavour, MUON_FLAVOUR);

    if (abs(Lepton1.flavour == MUON_FLAVOUR)) {
          printf("2 \n");
      if (Lepton1.v.Pt() > 10. && Lepton1.v.Pt() <= 20. && std_vector_trigger->at(22) == 1) { //Lumi HLT_Mu8_TrkIsoVVL: 1.386 pb
	    printf("3 \n");
	passTrigger= true;
	_event_weight_fr = (_luminosity*1000 / 1.386);

      }
      else if (Lepton1.v.Pt() > 20. && std_vector_trigger->at(23) == 1) { //Lumi HLT_Mu17_TrkIsoVVL: 201.951 pb

	passTrigger = true;
	_event_weight_fr = (_luminosity*1000 / 201.951);

      }
    
    } else if (abs(Lepton1.flavour == ELECTRON_FLAVOUR)) {
      
      if (Lepton1.v.Pt() > 13. && Lepton1.v.Pt() <= 25. && std_vector_trigger->at(31) == 1) { //Lumi HLT_Ele8_Iso: 11.204 pb;

	passTrigger = true;
	_event_weight_fr = (_luminosity*1000 / 11.204);

      } else if (Lepton1.v.Pt() > 25. && std_vector_trigger->at(33) == 1) { //Lumi HLT_Ele23_Iso: 3.201

	passTrigger = true;
	_event_weight_fr = (_luminosity*1000 / 3.201);
      
      }
 
    }

    if (!passTrigger) continue;


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
     
    if (pass && _saveminitree) minitree->Fill();
 
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
    h_loose       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight_fr);
  } else if (Lepton1.type == 1) {
    h_tight       [ichannel][icut][ijet]->Fill(Lepton1.v.Pt(),            _event_weight_fr);
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
  
  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);

}


