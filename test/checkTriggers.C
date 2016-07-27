//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l -b -q checkTriggers.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const TString _gridui = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/";
const TString _lxplus = "eos/cms/store/group/phys_higgs/cmshww/amassiro/HWW6p3/";


std::vector<TString> triggers;


void checkTriggers()
{
  triggers.clear();

  triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v*");
  triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*");
  triggers.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_v*");
  triggers.push_back("HLT_IsoMu22_v*");
  triggers.push_back("HLT_IsoTkMu22_v*");
  triggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*");
  triggers.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v*");
  triggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*");

  TFile* file = new TFile(_lxplus + "21Jun2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr__l2tight/latino_Run2016B_PromptReco_MuonEG.root");

  TH1F* selectedTriggers = (TH1F*)file->Get("selectedTriggers");

  TAxis* xaxis = (TAxis*)selectedTriggers->GetXaxis();

  printf("\n Checking %d triggers\n\n", (int)triggers.size());

  for (int i=0; i<triggers.size(); i++)
    {
      for (int j=0; j<=selectedTriggers->GetNbinsX(); j++)
	{
	  TString label = (TString)xaxis->GetBinLabel(j);

	  if (label.Length() < 1) continue;

	  if (triggers[i].EqualTo(label)) printf(" found [%d] %s\n", i, label.Data());
	}
    }

  printf("\n");
}
