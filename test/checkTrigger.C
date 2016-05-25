// Constants and data members
//------------------------------------------------------------------------------
const TCut SingleElectron = "std_vector_trigger[1]";                             // HLT_Ele23_WPLoose_Gsf_v*
const TCut SingleMuon     = "std_vector_trigger[40] || std_vector_trigger[17]";  // HLT_IsoMu18, HLT_IsoTkMu20
const TCut DoubleEG       = "std_vector_trigger[5]";                             // HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
const TCut DoubleMuon     = "std_vector_trigger[10] || std_vector_trigger[12]";  // HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ
const TCut MuonEG         = "std_vector_trigger[7] || std_vector_trigger[9]";    // HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL, HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL

const TString path2015D = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox_76x/03Mar_Run2015D_16Dec2015__l2loose__hadd__EpTCorr__l2tight/latino_";
const TString path2016B = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/20May2016_Run2016B_PromptReco/l2loose16__hadd__l2tight/latino_";

TString path;
TCut    json;


// Functions
//------------------------------------------------------------------------------
void GetTriggerEfficiency(TString sample);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// checkTrigger
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void checkTrigger(TString data = "2015D")
{
  if (data.Contains("2015D"))
    {
      path = path2015D;
      json = "metPfType1 > -999";

      GetTriggerEfficiency("Run2015D_16Dec2015_SingleElectron");
      GetTriggerEfficiency("Run2015D_16Dec2015_SingleMuon");
      GetTriggerEfficiency("Run2015D_16Dec2015_DoubleEG");
      GetTriggerEfficiency("Run2015D_16Dec2015_DoubleMuon");
      GetTriggerEfficiency("Run2015D_16Dec2015_MuonEG");
    }
  else if (data.Contains("2016B"))
    {
      path = path2016B;
      json = "isJsonOk > 0";

      GetTriggerEfficiency("Run2016B_PromptReco_SingleElectron");
      GetTriggerEfficiency("Run2016B_PromptReco_SingleMuon");
      GetTriggerEfficiency("Run2016B_PromptReco_DoubleEG");
      GetTriggerEfficiency("Run2016B_PromptReco_DoubleMuon");
      GetTriggerEfficiency("Run2016B_PromptReco_MuonEG");
    }
}


//------------------------------------------------------------------------------
// GetTriggerEfficiency
//------------------------------------------------------------------------------
void GetTriggerEfficiency(TString sample)
{
  TFile* file = new TFile(path + sample + ".root");

  TCanvas* canvas = new TCanvas("canvas " + sample, "canvas " + sample);
  
  TCut pass = json;

  if (sample.Contains("SingleMuon"))     pass = json && SingleMuon;
  if (sample.Contains("DoubleMuon"))     pass = json && DoubleMuon;
  if (sample.Contains("DoubleEG"))       pass = json && DoubleEG;
  if (sample.Contains("MuonEG"))         pass = json && MuonEG;
  if (sample.Contains("SingleElectron")) pass = json && SingleElectron;

  TTree* latino = (TTree*)file->Get("latino");

  Long64_t numerator   = latino->Draw("metPfType1", pass);
  Long64_t denominator = latino->Draw("metPfType1", json);

  printf(" %-34s efficiency = %6.2f%s (%lli / %lli)\n",
	 sample.Data(),
	 1e2 * numerator / denominator,
	 "%",
	 numerator,
	 denominator);
}
