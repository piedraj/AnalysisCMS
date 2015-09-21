#include "AnalysisWZ.C"


void runWZ(TString filename,
	   TString era,
	   float   luminosity)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisWZ awz(latino);

  awz.Loop(filename, era, luminosity);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  runWZ(argv[1], argv[2], atof(argv[3]));

  return 0;
}
# endif
