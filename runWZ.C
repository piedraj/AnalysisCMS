#include "AnalysisWZ.C"


//TString local_path = "/gpfs/csic_projects/cms/piedra/latino/kbutanov/RunII/15Jul/25ns/";


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
  runWZ("WZ", "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/kbutanov/RunII/15Jul/25ns/latino_WZ.root");

  return 0;
}
# endif
