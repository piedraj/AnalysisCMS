#include "src/AnalysisControl.C"
//#include "src/AnalysisDY.C"
//#include "src/AnalysisFR.C"
//#include "src/AnalysisMET.C"
//#include "src/AnalysisMonoH.C"
//#include "src/AnalysisPR.C"
//#include "src/AnalysisShape.C"
//#include "src/AnalysisStop.C"
//#include "src/AnalysisTop.C"
//#include "src/AnalysisTTDM.C"
//#include "src/AnalysisWW.C"
//#include "src/AnalysisWZ.C"


void runAnalysis(TString filename,
		 TString systematic,
                 float   StopMass       = -1.,
                 float   NeutralinoMass = -1.)
{
  gInterpreter->ExecuteMacro("test/PaperStyle.C");

  TFile* file = TFile::Open(filename);

  TTree* latino = (TTree*)file->Get("latino");

  float baseW_lumi_fb = 1.0;  // baseW has been computed for 1.0 fb-1

  AnalysisControl analysis(latino, systematic); analysis.Loop("Control", filename, baseW_lumi_fb);
  //  AnalysisDY      analysis(latino, systematic); analysis.Loop("DY",      filename, baseW_lumi_fb);
  //  AnalysisFR      analysis(latino, systematic); analysis.Loop("FR",      filename, baseW_lumi_fb);
  //  AnalysisMET     analysis(latino, systematic); analysis.Loop("MET",     filename, baseW_lumi_fb);
  //  AnalysisMonoH   analysis(latino, systematic); analysis.Loop("MonoH",   filename, baseW_lumi_fb);
  //  AnalysisPR      analysis(latino, systematic); analysis.Loop("PR",      filename, baseW_lumi_fb);
  //  AnalysisShape   analysis(latino, systematic); analysis.Loop("Shape",   filename, baseW_lumi_fb);
  //  AnalysisStop    analysis(latino, systematic); analysis.Loop("Stop",    filename, baseW_lumi_fb);
  //  AnalysisStop    analysis(latino, systematic); analysis.Loop("Stop",    filename, baseW_lumi_fb, StopMass, NeutralinoMass);
  //  AnalysisTop     analysis(latino, systematic); analysis.Loop("Top",     filename, baseW_lumi_fb);
  //  AnalysisTTDM    analysis(latino, systematic); analysis.Loop("TTDM",    filename, baseW_lumi_fb);
  //  AnalysisWW      analysis(latino, systematic); analysis.Loop("WW",      filename, baseW_lumi_fb);
  //  AnalysisWZ      analysis(latino, systematic); analysis.Loop("WZ",      filename, baseW_lumi_fb);
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  if (argc < 3 )
    {
      printf("\n ./runAnalysis <filename> <systematic>\n");
      printf("\n The output will be saved in\n\n");
      printf("            minitrees/<systematic>\n");
      printf("            rootfiles/<systematic>\n");
      printf("            txt/<systematic>\n\n");
      
      return -1;
    }

  if      (argc == 3) runAnalysis(argv[1], argv[2]);
  else if (argc == 4) runAnalysis(argv[1], argv[2],  atof(argv[3]));
  else if (argc == 5) runAnalysis(argv[1], argv[2],  atof(argv[3]), atof(argv[4]));

  return 0;
}
# endif
