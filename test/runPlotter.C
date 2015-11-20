#include "HistogramReader.h"


const TString inputdir  = "../rootfiles/25ns/";
const TString outputdir = "figures/25ns/";


void runPlotter(TString level)
{
  enum {linY, logY};

  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir(outputdir + level, kTRUE);

  HistogramReader plotter(inputdir, outputdir);

  plotter.SetLuminosity (lumi25ns_fb);
  plotter.SetStackOption("hist");
  plotter.SetDrawRatio(true);

  plotter.AddData   ("01_Data",      "data",    kBlack);
  plotter.AddProcess("08_WJets",     "W+jets",  kAzure-9);
  plotter.AddProcess("09_TTW",       "ttW",     kGreen-6);
  plotter.AddProcess("11_HWW",       "HWW",     kRed);
  plotter.AddProcess("06_WW",        "WW",      kAzure-7);
  plotter.AddProcess("05_SingleTop", "top",     kYellow-6);
  plotter.AddProcess("04_Top",       "tt+jets", kYellow);
  plotter.AddProcess("03_ZZ",        "ZZ",      kRed+3);
  plotter.AddProcess("07_ZJets",     "Z+jets",  kGreen+2);
  plotter.AddProcess("02_WZ",        "WZ",      kOrange-2);


  // Draw cut evolution
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  TString tok;

  Ssiz_t from = 0;

  TString analysis = (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEvolution(analysis, "h_counterLum_" + schannel[i]);

      bool setlogy = (analysis.EqualTo("WZ")) ? linY : logY;
      
      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", setlogy);
    }


  // Draw distributions
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(true);

  firstchannel = (level.Contains("WZ")) ? eee : ee;
  lastchannel  = (level.Contains("WZ")) ? lll : ll;

  for (int j=0; j<=njetbin; j++)
    {
      if (!level.Contains("WW") && j != njetbin) continue;
      
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      TString prefix = level + jetbin + "/h_";

      for (int i=firstchannel; i<=lastchannel; i++)
	{
	  TString suffix = "_" + schannel[i];

	  float m2lmin = (level.Contains("WZ")) ?  50. : -999.;
	  float m2lmax = (level.Contains("WZ")) ? 130. : -999.;


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.Draw(prefix + "m2l"        + suffix, "m_{#font[12]{ll}}",                        5, 0, "GeV",  logY, true, m2lmin, m2lmax);
	  plotter.Draw(prefix + "m2l"        + suffix, "m_{#font[12]{ll}}",                        5, 0, "GeV",  linY, true, m2lmin, m2lmax);
	  plotter.Draw(prefix + "met"        + suffix, "E_{T}^{miss}",                            10, 0, "GeV",  linY, true,      0,    200);
	  plotter.Draw(prefix + "counterLum" + suffix, "yield",                                   -1, 0, "NULL", linY);
	  plotter.Draw(prefix + "ht"         + suffix, "H_{T}",                                   10, 0, "GeV",  linY);
	  plotter.Draw(prefix + "nvtx"       + suffix, "number of vertices",                      -1, 0, "NULL", linY);
	  plotter.Draw(prefix + "njet"       + suffix, "number of jets (p_{T}^{jet} > 30 GeV)",   -1, 0, "NULL", logY);
	  plotter.Draw(prefix + "nbjet"      + suffix, "number of b-jets (p_{T}^{jet} > 30 GeV)", -1, 0, "NULL", logY);
	  plotter.Draw(prefix + "deltarll"   + suffix, "#DeltaR_{#font[12]{ll}}",                  2, 1, "NULL", linY, true, 0, 4);
	  plotter.Draw(prefix + "deltarll"   + suffix, "#DeltaR_{#font[12]{ll}}",                  2, 1, "NULL", logY, true, 0, 4);


	  // WZ histograms
	  //--------------------------------------------------------------------
	  if (!level.Contains("WZ")) continue;

	  plotter.Draw(prefix + "m3l"   + suffix, "m_{#font[12]{3l}}",       10, 0, "GeV", linY, true, 60, 300);
	  plotter.Draw(prefix + "zl1pt" + suffix, "Z leading lepton p_{T}",   5, 0, "GeV", linY, true,  0, 150);
	  plotter.Draw(prefix + "zl2pt" + suffix, "Z trailing lepton p_{T}",  5, 0, "GeV", linY, true,  0, 150);
	  plotter.Draw(prefix + "wlpt"  + suffix, "W lepton p_{T}",           5, 0, "GeV", linY, true,  0, 150);
  	  plotter.Draw(prefix + "mtW"   + suffix, "W transverse mass",       10, 0, "GeV", linY);

	  if (1) continue;

	  plotter.Draw(prefix + "zl1eta"      + suffix, "Z leading lepton #eta",                6, 1, "NULL", linY);
	  plotter.Draw(prefix + "zl2eta"      + suffix, "Z trailing lepton #eta",               6, 1, "NULL", linY);
	  plotter.Draw(prefix + "wleta"       + suffix, "W lepton #eta",                        6, 1, "NULL", linY);
	  plotter.Draw(prefix + "wlzl1deltar" + suffix, "#DeltaR(W lepton, Z leading lepton)",  6, 1, "NULL", linY);
	  plotter.Draw(prefix + "wlzl2deltar" + suffix, "#DeltaR(W lepton, Z leading lepton)",  6, 1, "NULL", linY);
  	  plotter.Draw(prefix + "wldxy"       + suffix, "W lepton dxy",                        20, 0, "cm",   logY);
  	  plotter.Draw(prefix + "wldz"        + suffix, "W lepton dz",                         20, 0, "cm",   logY);
  	  plotter.Draw(prefix + "zl1dxy"      + suffix, "Z leading lepton dxy",                20, 0, "cm",   logY);
  	  plotter.Draw(prefix + "zl1dz"       + suffix, "Z leading lepton dz",                 20, 0, "cm",   logY);
  	  plotter.Draw(prefix + "zl2dxy"      + suffix, "Z trailing lepton dxy",               20, 0, "cm",   logY);
  	  plotter.Draw(prefix + "zl2dz"       + suffix, "Z trailing lepton dz",                20, 0, "cm",   logY);
	}
    }

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
