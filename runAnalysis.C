#include "src/AnalysisFR.C"
//#include "src/AnalysisMonoH.C"
//#include "src/AnalysisTop.C"
//#include "src/AnalysisTTDM.C"
//#include "src/AnalysisWW.C"
//#include "src/AnalysisWZ.C"


void runAnalysis(TString filename,
		 TString systematic)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

    AnalysisFR    fr(latino); fr.Loop("FR", filename, lumi_fb);
  //  AnalysisMonoH mh(latino); mh.Loop("MonoH", filename, lumi_fb);
  //  AnalysisTop   tt(latino); tt.Loop("Top",   filename, lumi_fb);
  //  AnalysisTTDM  dm(latino); dm.Loop("TTDM",  filename, lumi_fb);
  //  AnalysisWW    ww(latino); ww.Loop("WW",    filename, lumi_fb);
  //  AnalysisWZ    wz(latino); wz.Loop("WZ",    filename, lumi_fb);

}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  if (argc != 3)
    {
      printf("\n ./runAnalysis <filename> <systematic>\n");
      printf("\n The output will be saved in\n\n");
      printf("            minitrees/<systematic>\n");
      printf("            rootfiles/<systematic>\n");
      printf("            txt/<systematic>\n\n");
      
      return -1;
    }

  runAnalysis(argv[1], argv[2]);

  return 0;
}
# endif
