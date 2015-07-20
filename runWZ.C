#include "AnalysisWZ.C"


void runWZ(TString sample, TString filename)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  awz.Loop(sample);
}


# ifndef __CINT__
int main()
{
//runWZ("WZ",                 "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/kbutanov/RunII/15Jul/25ns/latino_WZ.root");
  runWZ("DYJetsToLL1050",     "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_DYJetsToLL1050.root");
  runWZ("WZTo3LNu",           "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_WZTo3LNu.root");
  runWZ("WJetsToLNu",         "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_WJetsToLNu.root");
  runWZ("ZZ",                 "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_ZZ.root");
  runWZ("ggHWW120",           "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_ggHWW120.root");
  runWZ("topTWantitop",       "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_topTWantitop.root");
  runWZ("topTWtop",           "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_topTWtop.root");
  runWZ("topTchannelAntitop", "/gpfs/csic_projects/cms/piedra/latino/RunII/MC_Spring15/25ns/latino_topTchannelAntitop.root");

  return 0;
}
# endif
