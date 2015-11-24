#include "HistogramReader.h"


const TString inputdir  = "../rootfiles/25ns/";
const TString outputdir = "figures/25ns/";

enum {linY, logY};


void runPlotter(TString level)
{
  TString tok;

  Ssiz_t from = 0;

  TString analysis = (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir(outputdir + level, kTRUE);

  HistogramReader plotter(inputdir, outputdir);

  plotter.SetLuminosity(lumi25ns_fb);
  plotter.SetStackOption("hist");
  plotter.SetDrawRatio(true);

  plotter.AddData("01_Data", "data", kBlack);


  // Stack predicted histograms in different order for different analyses
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("WZ"))
    {
      plotter.AddProcess("02_WZ",        "WZ",      kOrange-2);
      plotter.AddProcess("07_ZJets",     "Z+jets",  kGreen+2);
      plotter.AddProcess("05_SingleTop", "top",     kYellow-6);
      plotter.AddProcess("08_WJets",     "W+jets",  kGray+1);
      plotter.AddProcess("06_WW",        "WW",      kAzure-9);
      plotter.AddProcess("11_HWW",       "HWW",     kRed);
      plotter.AddProcess("03_ZZ",        "ZZ",      kRed+3);
      plotter.AddProcess("04_Top",       "tt+jets", kYellow);
      plotter.AddProcess("09_TTW",       "ttW",     kGreen-6);
    }
  else
    {
      plotter.AddProcess("06_WW",        "WW",      kAzure-9);
      plotter.AddProcess("02_WZ",        "WZ",      kOrange-2);
      plotter.AddProcess("03_ZZ",        "ZZ",      kRed+3);
      plotter.AddProcess("04_Top",       "tt+jets", kYellow);
      plotter.AddProcess("05_SingleTop", "top",     kYellow-6);
      plotter.AddProcess("08_WJets",     "W+jets",  kGray+1);
      plotter.AddProcess("07_ZJets",     "Z+jets",  kGreen+2);
      plotter.AddProcess("09_TTW",       "ttW",     kGreen-6);
      plotter.AddProcess("11_HWW",       "HWW",     kRed);
    }


  // Draw cut evolution
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEvolution(analysis, "h_counterLum_" + schannel[i]);

      bool setlogy = (analysis.EqualTo("WZ")) ? linY : logY;
      
      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", setlogy);
    }


  // Draw distributions
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(true);

  for (int j=0; j<=njetbin; j++)
    {
      if (!level.Contains("WW") && j != njetbin) continue;
      
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      TString prefix = level + jetbin + "/h_";

      for (int i=firstchannel; i<=lastchannel; i++)
	{
	  TString suffix = "_" + schannel[i];

	  float xmin = (level.Contains("WZ")) ?  60 :   0;
	  float xmax = (level.Contains("WZ")) ? 120 : 300;


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.Draw(prefix + "counterLum" + suffix, "yield",                                   -1, 0, "NULL", linY);
	  plotter.Draw(prefix + "njet"       + suffix, "number of jets (p_{T}^{jet} > 30 GeV)",   -1, 0, "NULL", logY);
	  plotter.Draw(prefix + "nbjet"      + suffix, "number of b-jets (p_{T}^{jet} > 20 GeV)", -1, 0, "NULL", logY);
	  plotter.Draw(prefix + "nvtx"       + suffix, "number of vertices",                      -1, 0, "NULL", linY, true,    0,   30);
	  plotter.Draw(prefix + "deltarll"   + suffix, "#DeltaR_{#font[12]{ll}}",                  2, 1, "NULL", logY, true,    0,    4);
	  plotter.Draw(prefix + "deltaphill" + suffix, "#Delta#phi_{#font[12]{ll}}",               2, 1, "rad",  logY, true,    0, 3.15);
	  plotter.Draw(prefix + "trkmet"     + suffix, "track E_{T}^{miss}",                      10, 0, "GeV",  logY, true,    0,  400);
	  plotter.Draw(prefix + "met"        + suffix, "E_{T}^{miss}",                            10, 0, "GeV",  logY, true,    0,  400);
	  plotter.Draw(prefix + "mpmet"      + suffix, "min projected E_{T}^{miss}",              10, 0, "GeV",  logY, true,    0,  400);
	  plotter.Draw(prefix + "m2l"        + suffix, "m_{#font[12]{ll}}",                        2, 0, "GeV",  logY, true, xmin, xmax);
	  plotter.Draw(prefix + "m2l"        + suffix, "m_{#font[12]{ll}}",                        2, 0, "GeV",  linY, true, xmin, xmax);
	  plotter.Draw(prefix + "mt1"        + suffix, "m_{T,1}",                                 10, 0, "GeV",  logY, true,    0,  500);
	  plotter.Draw(prefix + "mt2"        + suffix, "m_{T,2}",                                 10, 0, "GeV",  logY, true,    0,  500);
	  plotter.Draw(prefix + "mth"        + suffix, "m_{T}^{H}",                               10, 0, "GeV",  logY, true,    0,  500);
	  plotter.Draw(prefix + "mc"         + suffix, "m_{c}",                                   10, 0, "GeV",  logY, true,    0,  500);
	  plotter.Draw(prefix + "ht"         + suffix, "H_{T}",                                   10, 0, "GeV",  logY, true,    0,  500);
	  plotter.Draw(prefix + "pt1"        + suffix, "leading lepton p_{T}",                    10, 0, "GeV",  logY, true,    0,  300);
	  plotter.Draw(prefix + "pt2"        + suffix, "trailing lepton p_{T}",                   10, 0, "GeV",  logY, true,    0,  300);
	  plotter.Draw(prefix + "pt2l"       + suffix, "p_{T}^{#font[12]{ll}}",                   10, 0, "GeV",  logY, true,    0,  300);
	  plotter.Draw(prefix + "ptww"       + suffix, "p_{T}^{WW}",                              10, 0, "GeV",  logY, true,    0,  300);


	  // WZ histograms
	  //--------------------------------------------------------------------
	  if (!level.Contains("WZ")) continue;

	  plotter.Draw(prefix + "m3l"         + suffix, "m_{#font[12]{3l}}",                   10, 0, "GeV",  linY, true, 60, 360);
  	  plotter.Draw(prefix + "mtw"         + suffix, "W transverse mass",                   10, 0, "GeV",  linY, true,  0, 150);
	  plotter.Draw(prefix + "zl1pt"       + suffix, "Z leading lepton p_{T}",              10, 0, "GeV",  linY, true,  0, 150);
	  plotter.Draw(prefix + "zl2pt"       + suffix, "Z trailing lepton p_{T}",             10, 0, "GeV",  linY, true,  0, 150);
	  plotter.Draw(prefix + "wlpt"        + suffix, "W lepton p_{T}",                      10, 0, "GeV",  linY, true,  0, 150);
	  plotter.Draw(prefix + "zl1eta"      + suffix, "Z leading lepton #eta",                6, 1, "NULL", linY);
	  plotter.Draw(prefix + "zl2eta"      + suffix, "Z trailing lepton #eta",               6, 1, "NULL", linY);
	  plotter.Draw(prefix + "wleta"       + suffix, "W lepton #eta",                        6, 1, "NULL", linY);
	  plotter.Draw(prefix + "wlzl1deltar" + suffix, "#DeltaR(W lepton, Z leading lepton)",  5, 1, "NULL", linY);
	  plotter.Draw(prefix + "wlzl2deltar" + suffix, "#DeltaR(W lepton, Z leading lepton)",  5, 1, "NULL", linY);
  	  plotter.Draw(prefix + "wldxy"       + suffix, "W lepton dxy",                        20, 3, "cm",   logY);
  	  plotter.Draw(prefix + "wldz"        + suffix, "W lepton dz",                         20, 3, "cm",   logY);
  	  plotter.Draw(prefix + "zl1dxy"      + suffix, "Z leading lepton dxy",                20, 3, "cm",   logY);
  	  plotter.Draw(prefix + "zl1dz"       + suffix, "Z leading lepton dz",                 20, 3, "cm",   logY);
  	  plotter.Draw(prefix + "zl2dxy"      + suffix, "Z trailing lepton dxy",               20, 3, "cm",   logY);
  	  plotter.Draw(prefix + "zl2dz"       + suffix, "Z trailing lepton dz",                20, 3, "cm",   logY);
	}
    }


  // Copy index.php in every directory
  //----------------------------------------------------------------------------
  gSystem->Exec("for dir in $(find ./ -type d); do cp -n ../index.php $dir/; done");
  gSystem->Exec("rm index.php");
}


//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
# ifndef __CINT__
int main(int argc, char ** argv)
{
  if(argc < 2) {
    
    printf("\n rm -rf %s\n\n", outputdir.Data());

    for (int i=0; i<ncut; i++) printf(" ./runPlotter %s\n", scut[i].Data());

    printf("\n");

    exit(0);
  }

  runPlotter(argv[1]);

  return 0;
}
# endif
