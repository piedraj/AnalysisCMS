#include "AnalysisWZ.C"


void runWZ(TString sample, TString filename)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  // https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2015Analysis
  //
  float luminosity = 0.01609;  // 25ns

  awz.Loop(sample, luminosity);
}


# ifndef __CINT__
int main()
{
  runWZ("WZTo3LNu",            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_WZTo3LNu.root");
  runWZ("DYJetsToLL_M-10to50", "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_DYJetsToLL_M-10to50.root");
  runWZ("DYJetsToLL_M-50",     "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_DYJetsToLL_M-50.root");
  runWZ("ST_tW_top",           "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ST_tW_top.root");
  runWZ("TTTo2L2Nu",           "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTTo2L2Nu.root");
  runWZ("TTWJetsToLNu",        "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTWJetsToLNu.root");
  runWZ("TTZToLLNuNu_M-10",    "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTZToLLNuNu_M-10.root");
  runWZ("ggZZ2e2m",            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2e2m.root");
  runWZ("ggZZ2e2t",            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2e2t.root");
  runWZ("ggZZ2m2t",            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2m2t.root");
  runWZ("ggZZ4e",              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4e.root");
  runWZ("ggZZ4m",              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4m.root");
  runWZ("ggZZ4t",              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4t.root");

  // Input files can be accessed directly from eos when working from lxplus
  // runWZ("WZ", "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/kbutanov/RunII/15Jul/25ns/latino_WZ.root");

  return 0;
}
# endif
