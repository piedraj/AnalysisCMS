#include "AnalysisWZ.C"


void runWZ(TString sample, TString filename)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  float luminosity = 0.01609;  // fb-1

  awz.Loop(sample, luminosity);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  runWZ(argv[1], argv[2]);

  return 0;
}
# endif
