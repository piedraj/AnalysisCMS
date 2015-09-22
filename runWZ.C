#include "AnalysisWZ.C"


void runWZ(TString filename, TString era)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  float luminosity = (era.EqualTo("50ns")) ? 0.04003 : 0.01547;  // fb-1

  awz.Loop(filename, era, luminosity);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  runWZ(argv[1], argv[2]);

  return 0;
}
# endif
