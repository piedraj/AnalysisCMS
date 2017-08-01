#include "../test/HistogramReader.h"


// Constants
//------------------------------------------------------------------------------
const TString inputdir  = "../rootfiles/nominal/FR";
const TString outputdir = "figures/";

const TString sll = "#font[12]{ll}";

enum {linY, logY};


// Functions
//------------------------------------------------------------------------------
void DrawLeptonPlots(TString lepton,
		     float   lumi,
		     float   jetet);


TString _level;
TString _option;

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
  _level  = level;
  _option = option;

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  plotter = new HistogramReader(inputdir, outputdir);

  plotter->SetStackOption(option);
  plotter->SetPublicStyle( false);
  plotter->SetSavePdf    ( false);

  (option.Contains("nostack")) ? plotter->SetDrawRatio(false) : plotter->SetDrawRatio (true);

  plotter->AddData("01_Data", "data", color_Data);

  plotter->AddProcess("07_ZJets", "Z+jets", color_ZJets);
  plotter->AddProcess("08_WJets", "W+jets", color_WJets);

  gSystem->mkdir(outputdir + level, kTRUE);

  DrawLeptonPlots("Ele", -1, 10.);
  DrawLeptonPlots("Ele", -1, 15.);
  DrawLeptonPlots("Ele", -1, 20.);
  DrawLeptonPlots("Ele", -1, 25.);
  DrawLeptonPlots("Ele", -1, 30.);
  DrawLeptonPlots("Ele", -1, 35.);
  DrawLeptonPlots("Ele", -1, 45.);

  DrawLeptonPlots("Muon", -1, 25.);
}


//------------------------------------------------------------------------------
// DrawLeptonPlots
//------------------------------------------------------------------------------
void DrawLeptonPlots(TString lepton,
		     float   lumi,
		     float   jetet)
{
  plotter->SetLuminosity(lumi);

  TString prefix = _level + "/h_" + lepton + "_";
  TString suffix = Form("_%.0fGeV", jetet);


  // Draw distributions
  //----------------------------------------------------------------------------
  if (!_option.Contains("nostack")) plotter->SetDrawYield(true);

  if (_level.Contains("Zpeak"))
    {
      plotter->Draw(prefix + "loose_m2l" + suffix, "loose m_{" + sll + "}", 20, 0, "GeV", logY, true, 0, 200);
      plotter->Draw(prefix + "tight_m2l" + suffix, "tight m_{" + sll + "}", 20, 0, "GeV", logY, true, 0, 200);
    }
  else
    {
      plotter->Draw(prefix + "loose_pt_bin"  + suffix, "loose p_{T}", -1, 0,  "GeV", linY, false);
      plotter->Draw(prefix + "tight_pt_bin"  + suffix, "tight p_{T}", -1, 0,  "GeV", linY, false);
      plotter->Draw(prefix + "loose_eta_bin" + suffix, "loose #eta",  -1, 1, "NULL", linY, false);
      plotter->Draw(prefix + "tight_eta_bin" + suffix, "tight #eta",  -1, 1, "NULL", linY, false);
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
