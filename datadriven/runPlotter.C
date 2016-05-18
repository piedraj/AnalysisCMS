#include "../test/HistogramReader.h"

// Constants
//------------------------------------------------------------------------------
const TString inputdir  = "../rootfiles/nominal/FR";
const TString outputdir = "figures/";

enum {linY, logY};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// runPlotter
//
// option = "hist"         --> all distributions normalized to the luminosity
// option = "nostack,hist" --> signal and top distributions normalized to one
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void runPlotter(TString level,
		TString option = "hist")
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  HistogramReader plotter(inputdir, outputdir);

  float lumi = lumi_fb;

  plotter.SetStackOption(option);
  plotter.SetPublicStyle( false);
  plotter.SetSavePdf    ( false);

  if (option.Contains("nostack"))
    {
      plotter.SetDrawRatio(false);
    }
  else
    {
      plotter.SetLuminosity(lumi);
      plotter.SetDrawRatio (true);
    }

  plotter.AddData("01_Data", "data", color_Data);

  plotter.AddProcess("07_ZJets", "Z+jets", color_ZJets);
  plotter.AddProcess("08_WJets", "W+jets", color_WJets);

  gSystem->mkdir(outputdir + level, kTRUE);

  TString prefix = level + "/h_";

  // Draw distributions
  //----------------------------------------------------------------------------
  if (!option.Contains("nostack")) plotter.SetDrawYield(true);

  plotter.Draw(prefix + "mtw", "W transverse mass", 10, 0, "GeV",  linY, true,  0, 150);

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
  if (argc < 2) {
    
    printf("\n rm -rf %s\n\n", outputdir.Data());

    for (int i=0; i<ncut; i++) printf(" ./runPlotter %s\n", scut[i].Data());

    printf("\n");

    exit(0);
  }

  if (argc == 2)
    runPlotter(argv[1]);
  else
    runPlotter(argv[1], argv[2]);

  return 0;
}
# endif
