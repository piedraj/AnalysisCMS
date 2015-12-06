#include "src/AnalysisTest.C"


void runAnalysisTest(TString filename)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  AnalysisTest analysis(latino);

  analysis.AddAnalysis("Top");

  analysis.Loop(filename, lumi25ns_fb);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  if (argc != 2)
    {
      printf("\n ./runAnalysisTest <filename>\n\n");
      
      return -1;
    }

  runAnalysisTest(argv[1]);

  return 0;
}
# endif
