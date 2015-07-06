#include "AnalysisWZ.C"


TString local_path = "/gpfs/csic_projects/cms/piedra/latino/";


void runWZ(TString sample)
{
  TFile* file = new TFile(local_path + "latino_" + sample + ".root");

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  awz.Loop(sample);
}


# ifndef __CINT__
int main()
{
  runWZ("WZJets_PHYS14");
  runWZ("WZTo3LNu_25ns");

  return 0;
}
# endif
