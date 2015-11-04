#include "src/AnalysisCMS.C"


void runAnalysis(TString filename, TString era)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisCMS awz(latino);

  float luminosity = (era.EqualTo("50ns")) ? lumi50ns_fb : lumi25ns_fb;

  awz.Loop(filename, era, luminosity);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  runAnalysis(argv[1], argv[2]);

  return 0;
}
# endif
