
//#include "src/AnalysisControl.C"
#include "src/AnalysisFR.C"
//#include "src/AnalysisMonoH.C"
//#include "src/AnalysisStop.C"
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

<<<<<<< HEAD

  //  AnalysisControl analysis(latino, systematic); analysis.Loop("Control", filename, lumi_fb);
  AnalysisFR      analysis(latino, systematic); analysis.Loop("FR",      filename, lumi_fb);
=======
  //  AnalysisControl analysis(latino, systematic); analysis.Loop("Control", filename, lumi_fb);
  //  AnalysisFR      analysis(latino, systematic); analysis.Loop("FR",      filename, lumi_fb);
>>>>>>> 5fd9c6a3e68c9ca3688b1d82edafba4d623f55b1
  //  AnalysisMonoH   analysis(latino, systematic); analysis.Loop("MonoH",   filename, lumi_fb);
  //  AnalysisStop    analysis(latino, systematic); analysis.Loop("Stop",    filename, lumi_fb);
  //  AnalysisTop     analysis(latino, systematic); analysis.Loop("Top",     filename, lumi_fb);
  AnalysisTTDM    analysis(latino, systematic); analysis.Loop("TTDM",    filename, lumi_fb_blind);
  //  AnalysisWW      analysis(latino, systematic); analysis.Loop("WW",      filename, lumi_fb);
  //  AnalysisWZ      analysis(latino, systematic); analysis.Loop("WZ",      filename, lumi_fb);

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
