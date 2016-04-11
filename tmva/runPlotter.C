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
// option = "hist"         --> all distributions normalized to the luminosity
// option = "nostack,hist" --> signal and top distributions normalized to one
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void runPlotter(TString signal,
		TString label,
		TString option = "hist")
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  HistogramReader plotter(inputdir, outputdir);

  plotter.SetStackOption(option);
  plotter.SetDrawRatio  ( false);
  plotter.SetDrawYield  ( false);
  plotter.SetPublicStyle( false);
  plotter.SetSavePdf    (  true);
  
  if (option.Contains("nostack"))
    {
      plotter.AddSignal (signal + "__" + signal,    label, color_Signal);
      plotter.AddProcess(signal + "__04_TTTo2L2Nu", "tt",  kBlack);
    }
  else
    {
      plotter.SetLuminosity(lumi_fb);

      plotter.AddSignal (signal + "__" + signal,    label,        color_Signal);
      plotter.AddData   (signal + "__01_Data",      "data",       color_Data);
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
    }


  // Draw
  //----------------------------------------------------------------------------
  gSystem->mkdir(outputdir, kTRUE);

  plotter.Draw("h_mva_" + signal, "MVA output", ngroup, 2, "NULL", linY);
  plotter.Draw("h_mva_" + signal, "MVA output", ngroup, 2, "NULL", logY);


  // Optimization
  //----------------------------------------------------------------------------
  float score_x = plotter.GetBestSignalScoreX("h_mva_" + signal, "S/sqrt(B)", ngroup);


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
  if (argc < 3) {
    
    printf("\n rm -rf %s\n\n", outputdir.Data());

    printf(" ./runPlotter ttDM0001pseudo0010 \"m_{#chi}1 m_{P}10\"\n");
    printf(" ./runPlotter ttDM0001pseudo0020 \"m_{#chi}1 m_{P}20\"\n");
    printf(" ./runPlotter ttDM0001pseudo0050 \"m_{#chi}1 m_{P}50\"\n");
    printf(" ./runPlotter ttDM0001pseudo0100 \"m_{#chi}1 m_{P}100\"\n");
    printf(" ./runPlotter ttDM0001pseudo0200 \"m_{#chi}1 m_{P}200\"\n");
    printf(" ./runPlotter ttDM0001pseudo0500 \"m_{#chi}1 m_{P}500\"\n");
    printf(" ./runPlotter ttDM0010pseudo0100 \"m_{#chi}10 m_{P}100\"\n");
    printf(" ./runPlotter ttDM0050pseudo0200 \"m_{#chi}50 m_{P}200\"\n");
    printf(" ./runPlotter ttDM0150pseudo0200 \"m_{#chi}150 m_{P}200\"\n");
    printf(" ./runPlotter ttDM0150pseudo0500 \"m_{#chi}150 m_{P}500\"\n");

    printf("\n");

    printf(" ./runPlotter ttDM0001scalar0010 \"m_{#chi}1 m_{S}10\"\n");
    printf(" ./runPlotter ttDM0001scalar0050 \"m_{#chi}1 m_{S}50\"\n");
    printf(" ./runPlotter ttDM0001scalar0100 \"m_{#chi}1 m_{S}100\"\n");
    printf(" ./runPlotter ttDM0001scalar0200 \"m_{#chi}1 m_{S}200\"\n");
    printf(" ./runPlotter ttDM0001scalar0300 \"m_{#chi}1 m_{S}300\"\n");
    printf(" ./runPlotter ttDM0001scalar0500 \"m_{#chi}1 m_{S}500\"\n");
    printf(" ./runPlotter ttDM0010scalar0010 \"m_{#chi}10 m_{S}10\"\n");
    printf(" ./runPlotter ttDM0010scalar0050 \"m_{#chi}10 m_{S}50\"\n");
    printf(" ./runPlotter ttDM0010scalar0100 \"m_{#chi}10 m_{S}100\"\n");
    printf(" ./runPlotter ttDM0050scalar0050 \"m_{#chi}50 m_{S}50\"\n");
    printf(" ./runPlotter ttDM0050scalar0200 \"m_{#chi}50 m_{S}200\"\n");
    printf(" ./runPlotter ttDM0050scalar0300 \"m_{#chi}50 m_{S}300\"\n");
    printf(" ./runPlotter ttDM0150scalar0200 \"m_{#chi}150 m_{S}200\"\n");
    printf(" ./runPlotter ttDM0500scalar0500 \"m_{#chi}500 m_{S}500\"\n");

    printf("\n");

    exit(0);
  }

  if (argc == 3)
    runPlotter(argv[1], argv[2]);
  else
    runPlotter(argv[1], argv[2], argv[3]);

  return 0;
}
# endif
