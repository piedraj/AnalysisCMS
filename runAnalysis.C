//#include "src/AnalysisMonoH.C"
//#include "src/AnalysisTop.C"
#include "src/AnalysisTTDM.C"
//#include "src/AnalysisWW.C"
//#include "src/AnalysisWZ.C"


void runAnalysis(TString filename)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  //  AnalysisMonoH analysis(latino); analysis.Loop("MonoH", filename, lumi_fb);
  //  AnalysisTop   analysis(latino); analysis.Loop("Top",   filename, lumi_fb);
  AnalysisTTDM  analysis(latino); analysis.Loop("TTDM",  filename, lumi_fb);
  //  AnalysisWW    analysis(latino); analysis.Loop("WW",    filename, lumi_fb);
  //  AnalysisWZ    analysis(latino); analysis.Loop("WZ",    filename, lumi_fb);
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
