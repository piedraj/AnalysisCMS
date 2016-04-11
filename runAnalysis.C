//#include "src/AnalysisMonoH.C"
#include "src/AnalysisTop.C"
//#include "src/AnalysisTTDM.C"
//#include "src/AnalysisWW.C"
//#include "src/AnalysisWZ.C"


void runAnalysis(TString filename)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  //  AnalysisMonoH mh(latino); mh.Loop("MonoH", filename, lumi_fb);
    AnalysisTop   tt(latino); tt.Loop("Top",   filename, lumi_fb);
  //AnalysisTTDM  dm(latino); dm.Loop("TTDM",  filename, lumi_fb);
  //  AnalysisWW    ww(latino); ww.Loop("WW",    filename, lumi_fb);
  //  AnalysisWZ    wz(latino); wz.Loop("WZ",    filename, lumi_fb);
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
