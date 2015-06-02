void runAnalysisWZ()
{
  gInterpreter->LoadMacro("AnalysisBase.C+");
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ awz;

  awz.Loop();
}
