void runAnalysisWZ()
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ awz;

  awz.Loop();
}
