#include "src/AnalysisCMS.C"


void runAnalysis(TString filename, TString era)
{
  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisCMS awz(latino);

  awz.AddAnalysis("Top");
  awz.AddAnalysis("TTDM");
  awz.AddAnalysis("WW");
  awz.AddAnalysis("WZ");

  float luminosity = (era.EqualTo("50ns")) ? lumi50ns_fb : lumi25ns_fb;

  awz.Loop(filename, era, luminosity);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  if (argc != 3)
    {
      printf("\n ./runAnalysis <filename> <era>\n\n");
      
      return -1;
    }

  runAnalysis(argv[1], argv[2]);

  return 0;
}
# endif
