#include "../test/HistogramReader.h"


// Constants
//------------------------------------------------------------------------------
const TString inputdir  = "../rootfiles/nominal/FR";
const TString outputdir = "figures/";

enum {linY, logY};


// Functions
//------------------------------------------------------------------------------
void DrawLeptonPlots(
		     TString lepton,
		     float   lumi,
		     float   jetet);


TString         _level;
TString         _option;

HistogramReader* plotter;


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
  _level = level;
  _option = option;

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  plotter = new HistogramReader(inputdir, outputdir);

  plotter->SetStackOption(option);
  plotter->SetPublicStyle( false);
  plotter->SetSavePdf    ( false);

  if (option.Contains("nostack"))
    {
      plotter->SetDrawRatio(false);
    }
  else
    {
      plotter->SetDrawRatio (true);
    }

  plotter->AddData("01_Data", "data", color_Data);

  plotter->AddProcess("06_TT", "tt", color_TTTo2L2Nu);
  plotter->AddProcess("07_ZJets", "Z+jets", color_ZJets);
  plotter->AddProcess("08_WJets", "W+jets", color_WJets);

  gSystem->mkdir(outputdir + level, kTRUE);


  DrawLeptonPlots("Ele",  0.00314, 35.);
  DrawLeptonPlots("Muon", 0.19465,   20.);
 }

void DrawLeptonPlots(
		     TString lepton,
		     float   lumi,
		     float   jetet)
{
  plotter->SetLuminosity(lumi);

  TString prefix = _level + "/h_" + lepton + "_";
  TString suffix = Form("_%.0fGev", jetet);

  // Draw distributions
  //----------------------------------------------------------------------------
  if (!_option.Contains("nostack")) plotter->SetDrawYield(true);

  if (_level.Contains("ZRegion")) {

    plotter->Draw(prefix + "loose_m2l" + suffix, "Loose m2l",     20, 0, "GeV", linY, true, 0, 200);
    plotter->Draw(prefix + "tight_m2l" + suffix, "Tight m2l",     20, 0, "GeV", linY, true, 0, 200);

    plotter->Draw(prefix + "loose_m2l" + suffix, "Loose m2l log",     20, 0, "GeV", logY, true, 0, 200);
    plotter->Draw(prefix + "tight_m2l" + suffix, "Tight m2l log",     20, 0, "GeV", logY, true, 0, 200);

  } else {

    plotter->Draw(prefix + "loose_pt" + suffix, "Loose pt",     20, 0, "GeV", linY, true, 35., 200);
    plotter->Draw(prefix + "tight_pt" + suffix, "Tight pt",     20, 0, "GeV", linY, true, 0, 200);

    plotter->Draw(prefix + "loose_pt" + suffix, "Loose pt log",     20, 0, "GeV", logY, true, 0, 200);
    plotter->Draw(prefix + "tight_pt" + suffix, "Tight p logt",     20, 0, "GeV", logY, true, 0, 200);

    plotter->Draw(prefix + "loose_mtw" + suffix, "Loose mtw",     20, 0, "GeV", linY, true, 0, 200);
    plotter->Draw(prefix + "tight_mtw" + suffix, "Tight mtw",     20, 0, "GeV", linY, true, 0, 200);

    plotter->Draw(prefix + "loose_mtw" + suffix, "Loose mtw log",     20, 0, "GeV", logY, true, 0, 200);
    plotter->Draw(prefix + "tight_mtw" + suffix, "Tight mtw log",     20, 0, "GeV", logY, true, 0, 200);
  }

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
