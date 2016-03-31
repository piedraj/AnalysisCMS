#include "../test/HistogramReader.h"



// Constants
//------------------------------------------------------------------------------
const int     ngroup    = 2;
const TString inputdir  = "output/application/";
const TString outputdir = "figures/";

enum {linY, logY};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// runPlotter
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void runPlotter(TString signal)
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  HistogramReader plotter(inputdir, outputdir);

  plotter.SetLuminosity (lumi_fb);
  plotter.SetStackOption( "hist");
  plotter.SetDrawRatio  (   true);
  plotter.SetDrawYield  (   true);
  plotter.SetPublicStyle(  false);
  plotter.SetSavePdf    (   true);


  // Data
  //----------------------------------------------------------------------------
  plotter.AddData(signal + "__01_Data", "data", color_Data);


  // Backgrounds
  //----------------------------------------------------------------------------
  plotter.AddProcess(signal + "__14_HZ",        "HZ",         color_HZ);
  plotter.AddProcess(signal + "__10_HWW",       "HWW",        color_HWW);
  plotter.AddProcess(signal + "__06_WW",        "WW",         color_WW);
  plotter.AddProcess(signal + "__02_WZTo3LNu",  "WZ",         color_WZTo3LNu);
  plotter.AddProcess(signal + "__03_ZZ",        "ZZ",         color_ZZ);
  plotter.AddProcess(signal + "__11_Wg",        "W#gamma",    color_Wg);
  plotter.AddProcess(signal + "__07_ZJets",     "Z+jets",     color_ZJets);
  plotter.AddProcess(signal + "__09_TTV",       "ttV",        color_TTV);
  plotter.AddProcess(signal + "__04_TTTo2L2Nu", "tt",         color_TTTo2L2Nu);
  plotter.AddProcess(signal + "__05_ST",        "tW",         color_ST);
  plotter.AddProcess(signal + "__00_Fakes",     "non-prompt", color_Fakes);


  // Signal
  //----------------------------------------------------------------------------
  plotter.AddSignal(signal + "__" + signal, "m_{#chi}1 m_{P}10", color_Signal);


  // Draw
  //----------------------------------------------------------------------------
  gSystem->mkdir(outputdir, kTRUE);

  plotter.Draw("h_mva", "MVA output", ngroup, 2, "NULL", linY);
  plotter.Draw("h_mva", "MVA output", ngroup, 2, "NULL", logY);


  // Optimization
  //----------------------------------------------------------------------------
  int score = plotter.GetBestSignalScore("h_mva", "S/sqrt(B)", ngroup);


  // Copy index.php in every directory
  //----------------------------------------------------------------------------
  gSystem->Exec("for dir in $(find ./ -type d); do cp -n ../index.php $dir/; done");
  gSystem->Exec("rm -f index.php");
}


//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
# ifndef __CINT__
int main(int argc, char ** argv)
{
  if(argc < 2) {
    
    printf("\n rm -rf %s\n\n", outputdir.Data());

    printf(" ./runPlotter <signal>\n\n");

    exit(0);
  }

  runPlotter(argv[1]);

  return 0;
}
# endif
