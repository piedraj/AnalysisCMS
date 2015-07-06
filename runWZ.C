#include "AnalysisWZ.C"


void runWZ()
{
  AnalysisWZ awz;

  awz.Loop();
}


# ifndef __CINT__
int main()
{
  runWZ();

  return 0;
}
# endif
