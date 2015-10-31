#include "HistogramReader.h"


void runPlotter(TString cut)
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  HistogramReader plotter("../rootfiles/50ns/");
    
  plotter.AddProcess("01_Data",      " data",       kBlack);
  plotter.AddProcess("02_WZ",        " WZ",         kOrange-2);
  plotter.AddProcess("03_ZZ",        " ZZ",         kRed+3);
  plotter.AddProcess("04_Top",       " tt+jets",    kYellow);
  plotter.AddProcess("05_SingleTop", " single top", kYellow-6);
  plotter.AddProcess("06_WW",        " WW",         kAzure-7);
  plotter.AddProcess("07_ZJets",     " Z+jets",     kGreen+2);
  plotter.AddProcess("08_WJets",     " W+jets",     kAzure-9);
  //  plotter.AddProcess("09_TTW",       " ttW",        kAzure-9);
  //  plotter.AddProcess("10_TTZ",       " ttZ",        kAzure-9);
  
  plotter.Draw(cut + "/h_m2l_ll",  "m_{#font[12]{ll}}",                      5, 0, "GeV", 0, true, 60, 120);
  plotter.Draw(cut + "/h_njet_ll", "number of jets (p_{T}^{jet} > 30 GeV)", -1, 0, "NULL");
}


# ifndef __CINT__
int main(int argc, char ** argv)
{
  runPlotter(argv[1]);

  return 0;
}
# endif
