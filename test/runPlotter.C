#include "HistogramReader.h"


const Bool_t datadriven = true;

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

  Bool_t scale = logY;

  if (analysis.EqualTo("MonoH")) scale = logY;
  if (analysis.EqualTo("WW"))    scale = linY;
  if (analysis.EqualTo("WZ"))    scale = linY;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  HistogramReader plotter(inputdir + analysis, outputdir);

  plotter.SetLuminosity (lumi_fb);
  plotter.SetStackOption( "hist");
  plotter.SetDrawRatio  (   true);
  plotter.SetPublicStyle(  false);


  // Get the data
  //----------------------------------------------------------------------------
  plotter.AddData("01_Data", "data", kBlack);


  // Add processes
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("WZ"))
    {
      plotter.AddProcess("02_WZTo3LNu", "WZ",      kOrange-2);
      plotter.AddProcess("05_WW",       "WW",      kAzure-9);
      plotter.AddProcess("10_Wg",       "W#gamma", kBlue);
      plotter.AddProcess("03_ZZ",       "ZZ",      kRed+3);
      plotter.AddProcess("08_TTV",      "ttV",     kGreen-6);
      plotter.AddProcess("12_VVV",      "VVV",     kYellow-6);

      if (datadriven)
	{
	  plotter.AddProcess("00_Fakes", "non-prompt", kGreen+2);
	  plotter.AddProcess("11_Zg",    "Z#gamma",    kTeal);
	}
      else
	{
	  plotter.AddProcess("06_ZJets", "Z+jets", kGreen+2);
	  plotter.AddProcess("04_Top",   "top",    kYellow);
	}
    }
  else
    {
      plotter.AddProcess("05_WW",       "WW",      kAzure-9);
      plotter.AddProcess("02_WZTo3LNu", "WZ",      kOrange-2);
      plotter.AddProcess("03_ZZ",       "ZZ",      kRed+3);
      plotter.AddProcess("10_Wg",       "W#gamma", kBlue);
      plotter.AddProcess("06_ZJets",    "Z+jets",  kGreen+2);
      plotter.AddProcess("08_TTV",      "ttV",     kGreen-6);
      plotter.AddProcess("09_HWW",      "HWW",     kRed);
      plotter.AddProcess("13_HZ",       "HZ",      kRed+1);
      plotter.AddProcess("12_VVV",      "VVV",     kYellow-6);
      plotter.AddProcess("04_Top",      "top",     kYellow);

      if (datadriven)
	{
	  plotter.AddProcess("00_Fakes", "non-prompt", kGray+1);
	}
      else
	{
	  plotter.AddProcess("07_WJets", "W+jets", kGray+1);
	}
    }


  // Add signals
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("MonoH"))
    {
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP600_MA0300_13TeV",  "m_{Z'} 600",  kRed-4);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP800_MA0300_13TeV",  "m_{Z'} 800",  kRed-3);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1000_MA0300_13TeV", "m_{Z'} 1000", kRed-2);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1200_MA0300_13TeV", "m_{Z'} 1200", kRed-1);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1400_MA0300_13TeV", "m_{Z'} 1400", kRed);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1700_MA0300_13TeV", "m_{Z'} 1700", kRed+1);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP2000_MA0300_13TeV", "m_{Z'} 2000", kRed+2);
      plotter.AddSignal("Higgs_Zp2HDM_ww_MZP2500_MA0300_13TeV", "m_{Z'} 2500", kRed+3);
    }

  if (analysis.EqualTo("TTDM"))
    {
      plotter.AddSignal("ttDM1scalar20",    "m_{#chi}1 m_{#phi}20",    kRed-4);
      //      plotter.AddSignal("ttDM1scalar50",    "m_{#chi}1 m_{#phi}50",    kRed-3);
      //      plotter.AddSignal("ttDM1scalar500",   "m_{#chi}1 m_{#phi}500",   kRed-2);
      //      plotter.AddSignal("ttDM10scalar10",   "m_{#chi}10 m_{#phi}10",   kRed-1);
      //      plotter.AddSignal("ttDM50scalar50",   "m_{#chi}50 m_{#phi}50",   kRed);
      //      plotter.AddSignal("ttDM50scalar200",  "m_{#chi}50 m_{#phi}200",  kRed+1);
      //      plotter.AddSignal("ttDM50scalar300",  "m_{#chi}50 m_{#phi}300",  kRed+2);
      //      plotter.AddSignal("ttDM150scalar200", "m_{#chi}150 m_{#phi}200", kRed+3);
    }


  // Draw events by cut
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  gSystem->mkdir(outputdir + level, kTRUE);

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEventsByCut(analysis, "h_counterLum_" + schannel[i]);

      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", scale, false);
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

      plotter.Draw(level + jetbin + "/h_counterLum_evolution", "", -1, 0, "NULL", scale, false);
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
	  float xmax = (level.Contains("WZ")) ? 120 : 500;


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.SetTitle(title);

	  plotter.Draw(prefix + "njet"         + suffix, "number of (30 GeV) jets",         -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20loose" + suffix, "number of (20 GeV) loose b-jets", -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20tight" + suffix, "number of (20 GeV) tight b-jets", -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet30tight" + suffix, "number of (30 GeV) tight b-jets", -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nvtx"         + suffix, "number of vertices",              -1, 0, "NULL",  linY, true,    0,   30);
	  plotter.Draw(prefix + "drll"         + suffix, "#DeltaR_{#font[12]{ll}}",          5, 1, "NULL", scale, true,    0,    4);
	  plotter.Draw(prefix + "dphill"       + suffix, "#Delta#phi_{#font[12]{ll}}",       5, 1, "rad",  scale, true,    0, 3.15);
	  plotter.Draw(prefix + "met"          + suffix, "E_{T}^{miss}",                    10, 0, "GeV",  scale, true,    0,  400);
	  plotter.Draw(prefix + "trkmet"       + suffix, "track E_{T}^{miss}",              10, 0, "GeV",  scale, true,    0,  400);
	  plotter.Draw(prefix + "mpmet"        + suffix, "min projected E_{T}^{miss}",      10, 0, "GeV",  scale, true,    0,  400);
	  plotter.Draw(prefix + "m2l"          + suffix, "m_{#font[12]{ll}}",               10, 0, "GeV",  scale, true, xmin, xmax);
	  plotter.Draw(prefix + "mtw1"         + suffix, "m_{T}^{W,1}",                     10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "mtw2"         + suffix, "m_{T}^{W,2}",                     10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "mth"          + suffix, "m_{T}^{H}",                       10, 0, "GeV",  scale, true,    0,  300);
	  plotter.Draw(prefix + "mc"           + suffix, "m_{c}",                           10, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "ht"           + suffix, "H_{T}",                           10, 0, "GeV",  scale, true,    0,  500);
	  plotter.Draw(prefix + "pt1"          + suffix, "leading lepton p_{T}",             2, 0, "GeV",  scale, true,    0,  200);
	  plotter.Draw(prefix + "pt2"          + suffix, "trailing lepton p_{T}",            2, 0, "GeV",  scale, true,    0,  200);
	  plotter.Draw(prefix + "pt2l"         + suffix, "p_{T}^{#font[12]{ll}}",            2, 0, "GeV",  scale, true,    0,  200);
	  plotter.Draw(prefix + "ptww"         + suffix, "p_{T}^{WW}",                       2, 0, "GeV",  scale, true,    0,  200);
	  plotter.Draw(prefix + "sumpt12"      + suffix, "p_{T}^{lep1} + p_{T}^{lep2}",     10, 0, "GeV",  scale, true,    0,  600);
	  plotter.Draw(prefix + "sumjpt12"     + suffix, "p_{T}^{jet1} + p_{T}^{jet2}",     10, 0, "GeV",  scale, true,    0,  600);


	  // WW and MonoH histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("WW") || analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "jetpt1"    + suffix, "leading jet p_{T}",                  2, 0, "GeV", linY, false, 0, 100);
	      plotter.Draw(prefix + "htjets"    + suffix, "#sum_{jet} p_{T}",                  10, 0, "GeV", logY, false, 0, 300);
	      plotter.Draw(prefix + "htjets"    + suffix, "#sum_{jet} p_{T}",                  10, 0, "GeV", linY, false, 0, 300);
	      plotter.Draw(prefix + "htnojets"  + suffix, "p_{T}^{lep1} + p_{T}^{lep2} + MET", 10, 0, "GeV", linY, false, 0, 300);
	      plotter.Draw(prefix + "dphilmet1" + suffix, "#Delta#phi(lep1,E_{T}^{miss})",     10, 1, "rad", linY, false, 0, 3.2);
	      plotter.Draw(prefix + "dphilmet2" + suffix, "#Delta#phi(lep2,E_{T}^{miss})",     10, 1, "rad", linY, false, 0, 3.2);
	      plotter.Draw(prefix + "fullpmet"  + suffix, "projected E_{T}^{miss}",            10, 0, "GeV", linY, false, 0, 100);
	      plotter.Draw(prefix + "trkpmet"   + suffix, "projected track E_{T}^{miss}",       2, 0, "GeV", linY, false, 0, 100);
	      plotter.Draw(prefix + "metphi"    + suffix, "E_{T}^{miss} #phi",                 10, 0, "rad", linY, false, 0, 3.2);
	      plotter.Draw(prefix + "lepphi1"   + suffix, "leading lep #phi",                  10, 0, "rad", linY, false, 0, 3.2);
	      plotter.Draw(prefix + "lepphi2"   + suffix, "trailing lep #phi",                 10, 0, "rad", linY, false, 0, 3.2);

	      plotter.Draw(prefix + "mllstar"   + suffix, "m2l^{*}",                           10, 0, "GeV", linY, false, 0, 300);
	      plotter.Draw(prefix + "dphillstar"+ suffix, "#Delta #phi_{ll}^{*}",              10, 0, "rad", linY, false, 0, 3.2);
	    }


	  // MonoH histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "deltarl1met"   + suffix, "#DeltaR(lep1,E_{T}^{miss})",     2, 1, "NULL", logY, false, 0,   4);
	      plotter.Draw(prefix + "deltarl2met"   + suffix, "#DeltaR(lep2,E_{T}^{miss})",     2, 1, "NULL", logY, false, 0,   4);
	      plotter.Draw(prefix + "deltarllmet"   + suffix, "#DeltaR(ll,E_{T}^{miss})",       2, 1, "NULL", logY, false, 0,   4);
	      plotter.Draw(prefix + "deltaphil1met" + suffix, "#Delta#phi(lep1,E_{T}^{miss})",  2, 1, "rad",  logY, false, 0, 3.2);
	      plotter.Draw(prefix + "deltaphil2met" + suffix, "#Delta#phi(lep2,E_{T}^{miss})",  2, 1, "rad",  logY, false, 0, 3.2);
	      plotter.Draw(prefix + "deltaphillmet" + suffix, "#Delta#phi(ll,E_{T}^{miss})",    2, 1, "rad",  logY, false, 0, 3.2);
	      plotter.Draw(prefix + "m_r"           + suffix, "M_{R}",                         10, 0, "GeV",  logY, false, 0, 300);
	    }


	  // WZ histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("WZ"))
	    {
	      plotter.Draw(prefix + "m3l"        + suffix, "m_{#font[12]{3l}}",                10, 0, "GeV",  scale, true, 60, 360);
	      plotter.Draw(prefix + "mtw"        + suffix, "W transverse mass",                10, 0, "GeV",  scale, true,  0, 150);
	      plotter.Draw(prefix + "zl1pt"      + suffix, "Z leading lepton p_{T}",           10, 0, "GeV",  scale, true,  0, 150);
	      plotter.Draw(prefix + "zl2pt"      + suffix, "Z trailing lepton p_{T}",          10, 0, "GeV",  scale, true,  0, 150);
	      plotter.Draw(prefix + "wlpt"       + suffix, "W lepton p_{T}",                   10, 0, "GeV",  scale, true,  0, 150);
	      plotter.Draw(prefix + "wlzldeltar" + suffix, "min #DeltaR(W lepton, Z leptons)",  5, 1, "NULL", scale);
	    }
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
