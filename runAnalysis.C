#include "src/AnalysisTop.C"
#include "src/AnalysisWW.C"
#include "src/AnalysisWZ.C"


void runAnalysis(TString filename)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  //  AnalysisTop analysis(latino);
  AnalysisWW analysis(latino);
  //  AnalysisWZ analysis(latino);

  analysis.Loop(filename, lumi25ns_fb);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  if (argc != 2)
    {
      printf("\n ./runAnalysis <filename>\n\n");
      
      return -1;
    }

  runAnalysis(argv[1]);

  return 0;
}
# endif
