#include "HistogramReader.h"


const TString inputdir  = "../rootfiles/";
const TString outputdir = "figures/";

enum {linY, logY};


void runPlotter(TString level)
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  TString tok;

  Ssiz_t from = 0;

  TString analysis = (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  Bool_t scale = (analysis.EqualTo("WZ")) ? linY : logY;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  HistogramReader plotter(inputdir + analysis, outputdir);

  plotter.SetLuminosity(lumi25ns_fb);
  plotter.SetStackOption("hist");
  plotter.SetDrawRatio(true);


  // Get the data
  //----------------------------------------------------------------------------
  plotter.AddData("01_Data", "data", kBlack);


  // Add processes
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("WZ"))
    {
      plotter.AddProcess("02_WZ",    "WZ",      kOrange-2);
      plotter.AddProcess("06_ZJets", "Z+jets",  kGreen+2);
      plotter.AddProcess("07_WJets", "W+jets",  kGray+1);
      plotter.AddProcess("05_WW",    "WW",      kAzure-9);
      plotter.AddProcess("09_HWW",   "HWW",     kRed);
      plotter.AddProcess("10_Vg",    "V#gamma", kBlue);
      plotter.AddProcess("03_ZZ",    "ZZ",      kRed+3);
      plotter.AddProcess("04_Top",   "top",     kYellow);
      plotter.AddProcess("08_TTV",   "ttV",     kGreen-6);
      plotter.AddProcess("11_VVV",   "VVV",     kYellow-6);
    }
  else
    {
      plotter.AddProcess("05_WW",    "WW",      kAzure-9);
      plotter.AddProcess("02_WZ",    "WZ",      kOrange-2);
      plotter.AddProcess("03_ZZ",    "ZZ",      kRed+3);
      plotter.AddProcess("04_Top",   "top",     kYellow);
      plotter.AddProcess("10_Vg",    "V#gamma", kBlue);
      plotter.AddProcess("07_WJets", "W+jets",  kGray+1);
      plotter.AddProcess("06_ZJets", "Z+jets",  kGreen+2);
      plotter.AddProcess("08_TTV",   "ttV",     kGreen-6);
      plotter.AddProcess("09_HWW",   "HWW",     kRed);
      plotter.AddProcess("11_VVV",   "VVV",     kYellow-6);
    }


  // Add signals
  //----------------------------------------------------------------------------
  //  if (analysis.EqualTo("TTDM"))
  //    {
  //      plotter.AddSignal("ttDM1scalar20__part0",    "m_{#phi}1 m_{#chi}20",    kRed-4);
  //      plotter.AddSignal("ttDM1scalar50__part0",    "m_{#phi}1 m_{#chi}50",    kRed-3);
  //      plotter.AddSignal("ttDM1scalar500__part0",   "m_{#phi}1 m_{#chi}500",   kRed-2);
  //      plotter.AddSignal("ttDM10scalar10__part0",   "m_{#phi}10 m_{#chi}10",   kRed-1);
  //      plotter.AddSignal("ttDM50scalar50__part0",   "m_{#phi}50 m_{#chi}50",   kRed);
  //      plotter.AddSignal("ttDM50scalar200__part0",  "m_{#phi}50 m_{#chi}200",  kRed+1);
  //      plotter.AddSignal("ttDM50scalar300__part0",  "m_{#phi}50 m_{#chi}300",  kRed+2);
  //      plotter.AddSignal("ttDM150scalar200__part0", "m_{#phi}150 m_{#chi}200", kRed+3);
  //    }


  // Draw events by cut
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  gSystem->mkdir(outputdir + level, kTRUE);

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEventsByCut(analysis, "h_counterLum_" + schannel[i]);

      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", scale);
    }


  // Draw events by channel
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  for (int j=0; j<=njetbin; j++)
    {
      if (!level.Contains("WW") && j != njetbin) continue;
      
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      plotter.LoopEventsByChannel(level + jetbin);

      plotter.Draw(level + jetbin + "/h_counterLum_evolution", "", -1, 0, "NULL", scale);
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

	  TString title = (i < lastchannel) ? lchannel[i] : "cms";

	  float xmin = (level.Contains("WZ")) ?  60 :   0;
	  float xmax = (level.Contains("WZ")) ? 120 : 300;


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.SetTitle(title);

	  plotter.Draw(prefix + "njet30"     + suffix, "number of jets (p_{T}^{jet} > 30 GeV)",   -1, 0, "NULL",  logY);
	  plotter.Draw(prefix + "nbjet15"    + suffix, "number of b-jets (p_{T}^{jet} > 15 GeV)", -1, 0, "NULL",  logY);
	  plotter.Draw(prefix + "nvtx"       + suffix, "number of vertices",                      -1, 0, "NULL",  linY, true,    0,   30);
	  plotter.Draw(prefix + "drll"       + suffix, "#DeltaR_{#font[12]{ll}}",                  5, 1, "NULL", scale, true,    0,    4);
	  plotter.Draw(prefix + "deltaphill" + suffix, "#Delta#phi_{#font[12]{ll}}",               5, 1, "rad",  scale, true,    0, 3.15);
	  plotter.Draw(prefix + "met"        + suffix, "E_{T}^{miss}",                            10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "trkmet"     + suffix, "track E_{T}^{miss}",                      10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "mpmet"      + suffix, "min projected E_{T}^{miss}",              10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "m2l"        + suffix, "m_{#font[12]{ll}}",                        2, 0, "GeV",  scale, true, xmin, xmax);
	  plotter.Draw(prefix + "mtw1"       + suffix, "m_{T}^{W,1}",                             20, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "mtw2"       + suffix, "m_{T}^{W,2}",                             20, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "mth"        + suffix, "m_{T}^{H}",                               10, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "mc"         + suffix, "m_{c}",                                   20, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "ht"         + suffix, "H_{T}",                                   20, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "pt1"        + suffix, "leading lepton p_{T}",                    10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "pt2"        + suffix, "trailing lepton p_{T}",                   10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "pt2l"       + suffix, "p_{T}^{#font[12]{ll}}",                   10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "ptww"       + suffix, "p_{T}^{WW}",                              10, 0, "GeV",  scale, true,    0,  300);


	  // WZ histograms
	  //--------------------------------------------------------------------
	  if (!level.Contains("WZ")) continue;

	  plotter.Draw(prefix + "m3l"        + suffix, "m_{#font[12]{3l}}",                10, 0, "GeV",  scale, true, 60, 360);
  	  plotter.Draw(prefix + "mtw"        + suffix, "W transverse mass",                10, 0, "GeV",  scale, true,  0, 150);
  	  plotter.Draw(prefix + "zl1pt"      + suffix, "Z leading lepton p_{T}",           10, 0, "GeV",  scale, true,  0, 150);
  	  plotter.Draw(prefix + "zl2pt"      + suffix, "Z trailing lepton p_{T}",          10, 0, "GeV",  scale, true,  0, 150);
  	  plotter.Draw(prefix + "wlpt"       + suffix, "W lepton p_{T}",                   10, 0, "GeV",  scale, true,  0, 150);
  	  plotter.Draw(prefix + "wlzldeltar" + suffix, "min #DeltaR(W lepton, Z leptons)",  5, 1, "NULL", scale);
	}
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
