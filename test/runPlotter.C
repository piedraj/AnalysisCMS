#include "HistogramReader.h"


// Constants
//------------------------------------------------------------------------------
const Bool_t datadriven = true;

const TString inputdir  = "../rootfiles/nominal/";
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
  gInterpreter->ExecuteMacro("PaperStyle.C");

  TString tok;

  Ssiz_t from = 0;

  TString analysis = (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  float lumi = lumi_fb;

  if (analysis.EqualTo("TTDM")) lumi = lumi_fb_blind;

  Bool_t scale = logY;

  if (analysis.EqualTo("MonoH")) scale = logY;
  if (analysis.EqualTo("Top"))   scale = linY;
  if (analysis.EqualTo("TTDM"))  scale = linY;
  if (analysis.EqualTo("WW"))    scale = linY;
  if (analysis.EqualTo("WZ"))    scale = linY;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  HistogramReader plotter(inputdir + analysis, outputdir);

  plotter.SetStackOption(option);
  plotter.SetPublicStyle( false);
  plotter.SetSavePdf    (  true);

  if (option.Contains("nostack"))
    {
      plotter.SetDrawRatio(false);
    }
  else
    {
      plotter.SetLuminosity(lumi);
      plotter.SetDrawRatio (true);
    }


  // Get the data
  //----------------------------------------------------------------------------
  plotter.AddData("01_Data", "data", color_Data);


  // Add processes
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("WZ"))
    {
      plotter.AddProcess("02_WZTo3LNu", "WZ",      color_WZTo3LNu);
      plotter.AddProcess("06_WW",       "WW",      color_WW);
      plotter.AddProcess("11_Wg",       "W#gamma", color_Wg);
      plotter.AddProcess("03_ZZ",       "ZZ",      color_ZZ);
      plotter.AddProcess("09_TTV",      "ttV",     color_TTV);
      plotter.AddProcess("13_VVV",      "VVV",     color_VVV);

      if (datadriven)
	{
	  plotter.AddProcess("00_Fakes", "non-prompt", color_Fakes);
	  plotter.AddProcess("12_Zg",    "Z#gamma",    color_Zg);
	}
      else
	{
	  plotter.AddProcess("07_ZJets",    "Z+jets", color_ZJets);
	  plotter.AddProcess("04_TTTo2L2Nu", "tt",    color_TTTo2L2Nu);
	  plotter.AddProcess("05_ST",       "tW",     color_ST);
	}
    }
  else
    {
      plotter.AddProcess("14_HZ",       "HZ",      color_HZ);
      plotter.AddProcess("10_HWW",      "HWW",     color_HWW);
      plotter.AddProcess("06_WW",       "WW",      color_WW);
      plotter.AddProcess("02_WZTo3LNu", "WZ",      color_WZTo3LNu);
      plotter.AddProcess("03_ZZ",       "ZZ",      color_ZZ);
      plotter.AddProcess("11_Wg",       "W#gamma", color_Wg);
      plotter.AddProcess("07_ZJets",    "Z+jets",  color_ZJets);
      plotter.AddProcess("09_TTV",      "ttV",     color_TTV);
      plotter.AddProcess("04_TTTo2L2Nu", "tt",     color_TTTo2L2Nu, 1.00);
      plotter.AddProcess("05_ST",        "tW",     color_ST);

      if (datadriven)
	{
	  plotter.AddProcess("00_Fakes", "non-prompt", color_Fakes);

	}
      else
	{
	  plotter.AddProcess("08_WJets", "W+jets", color_WJets);
	}
    }


  // Add signals
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("MonoH"))
    {
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP600_MA0300_13TeV",  "m_{Z'} 600",  color_Signal-4);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP800_MA0300_13TeV",  "m_{Z'} 800",  color_Signal-3);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1000_MA0300_13TeV", "m_{Z'} 1000", color_Signal-2);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1200_MA0300_13TeV", "m_{Z'} 1200", color_Signal-1);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1400_MA0300_13TeV", "m_{Z'} 1400", color_Signal);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP1700_MA0300_13TeV", "m_{Z'} 1700", color_Signal+1);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP2000_MA0300_13TeV", "m_{Z'} 2000", color_Signal+2);
      //plotter.AddSignal("Higgs_Zp2HDM_ww_MZP2500_MA0300_13TeV", "m_{Z'} 2500", color_Signal+3);
    }

  if (analysis.EqualTo("TTDM"))
    {
      //plotter.AddSignal("ttDM0001scalar0010", "m_{#chi}1 m_{S}10",  color_Signal);
      //plotter.AddSignal("ttDM0001scalar0500", "m_{#chi}1 m_{S}500", color_Signal+2);
    }


  // Draw events by cut
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  gSystem->mkdir(outputdir + level, kTRUE);

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEventsByCut(analysis, "h_counterLum_" + schannel[i]);

      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", logY, false);
    }


  // Draw events by channel
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  for (int j=0; j<=njetbin; j++)
    {
      if (!analysis.EqualTo("Top") && !analysis.EqualTo("WW") && j != njetbin) continue;
      
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      plotter.LoopEventsByChannel(level + jetbin);

      plotter.Draw(level + jetbin + "/h_counterLum_evolution", "", -1, 0, "NULL", scale, false);
    }


  // Draw distributions
  //----------------------------------------------------------------------------
  if (!option.Contains("nostack")) plotter.SetDrawYield(true);

  float m2l_xmin   = (level.Contains("WZ")) ?  60 :   0;  // [GeV]
  float m2l_xmax   = (level.Contains("WZ")) ? 120 : 300;  // [GeV]
  int   m2l_ngroup = (level.Contains("WZ")) ?   2 :   5;
  
  for (int j=0; j<=njetbin; j++)
    {
      if (!analysis.EqualTo("Top") && !analysis.EqualTo("WW") && j != njetbin) continue;   
         
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      TString prefix = level + jetbin + "/h_";

      for (int i=firstchannel; i<=lastchannel; i++)
	{
	  TString suffix = "_" + schannel[i];

	  TString title = (i < lastchannel) ? lchannel[i] : "cms";

	  plotter.SetTitle(title);


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.Draw(prefix + "nvtx"     + suffix, "number of vertices",          -1, 0, "NULL", linY,  true, 0,  30);
	  plotter.Draw(prefix + "sumjpt12" + suffix, "p_{T}^{jet1} + p_{T}^{jet2}", 10, 0, "GeV",  scale, true, 0, 600);
	  plotter.Draw(prefix + "sumpt12"  + suffix, "p_{T}^{lep1} + p_{T}^{lep2}", 10, 0, "GeV",  scale, true, 0, 600);
	  plotter.Draw(prefix + "ptww"     + suffix, "p_{T}^{WW}",                  10, 0, "GeV",  scale, true, 0, 600);
	  plotter.Draw(prefix + "pt2l"     + suffix, "p_{T}^{#font[12]{ll}}",       10, 0, "GeV",  scale, true, 0, 600);


	  // Common histograms with minitree variables
	  //--------------------------------------------------------------------
	  plotter.Draw(prefix + "m2l" + suffix, "m_{#font[12]{ll}}", m2l_ngroup, 0, "GeV", linY, true, m2l_xmin, m2l_xmax);
	  plotter.Draw(prefix + "m2l" + suffix, "m_{#font[12]{ll}}", m2l_ngroup, 0, "GeV", logY, true, m2l_xmin, m2l_xmax);

	  plotter.Draw(prefix + "dphijet1met"   + suffix, "#Delta#phi(jet1,E_{T}^{miss})",      5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphijet2met"   + suffix, "#Delta#phi(jet2,E_{T}^{miss})",      5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphijj"        + suffix, "#Delta#phi(jet1,jet2)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphijjmet"     + suffix, "#Delta#phi(jj,E_{T}^{miss})",        5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilep1jet1"  + suffix, "#Delta#phi(lep1,jet1)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilep1jet2"  + suffix, "#Delta#phi(lep1,jet2)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilep2jet1"  + suffix, "#Delta#phi(lep2,jet1)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilep2jet2"  + suffix, "#Delta#phi(lep2,jet2)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphill"        + suffix, "#Delta#phi(lep1,lep2)",              5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphillmet"     + suffix, "#Delta#phi(ll,E_{T}^{miss})",        5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphillstar"    + suffix, "#Delta#phi*(lep1,lep2)",             5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilmet1"     + suffix, "#Delta#phi(lep1,E_{T}^{miss})",      5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphilmet2"     + suffix, "#Delta#phi(lep2,E_{T}^{miss})",      5, 2, "rad",  scale);
	  plotter.Draw(prefix + "drll"          + suffix, "#DeltaR(lep1,lep2)",                 5, 1, "NULL", scale);
	  plotter.Draw(prefix + "lep1eta"       + suffix, "leading lepton #eta",               -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "lep2eta"       + suffix, "trailing lepton #eta",              -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "jet1eta"       + suffix, "leading jet #eta",                  -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "jet2eta"       + suffix, "trailing jet #eta",                 -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "lep1phi"       + suffix, "leading lepton #phi",                5, 2, "rad",  scale);
	  plotter.Draw(prefix + "lep2phi"       + suffix, "trailing lepton #phi",               5, 2, "rad",  scale);
	  plotter.Draw(prefix + "jet1phi"       + suffix, "leading jet #phi",                   5, 2, "rad",  scale);
	  plotter.Draw(prefix + "jet2phi"       + suffix, "trailing jet #phi",                  5, 2, "rad",  scale);
	  plotter.Draw(prefix + "metPfType1Phi" + suffix, "E_{T}^{miss} #phi",                  5, 2, "rad",  scale);
	  plotter.Draw(prefix + "metTtrkPhi"    + suffix, "track E_{T}^{miss} #phi",            5, 2, "rad",  scale);
	  plotter.Draw(prefix + "lep1pt"        + suffix, "leading lepton p_{T}",               5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "lep2pt"        + suffix, "trailing lepton p_{T}",              5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "jet1pt"        + suffix, "leading jet p_{T}",                  5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "jet2pt"        + suffix, "trailing jet p_{T}",                 5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "jet1mass"      + suffix, "leading jet mass",                  -1, 0, "GeV",  scale, true, 0,   50);
	  plotter.Draw(prefix + "jet2mass"      + suffix, "trailing jet mass",                 -1, 0, "GeV",  scale, true, 0,   50);
	  plotter.Draw(prefix + "mc"            + suffix, "m_{c}",                             10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "metPfType1"    + suffix, "E_{T}^{miss}",                      10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "metTtrk"       + suffix, "track E_{T}^{miss}",                10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "mpmet"         + suffix, "min projected E_{T}^{miss}",        10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "mth"           + suffix, "m_{T}^{H}",                         10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "mtw1"          + suffix, "m_{T}^{W,1}",                       10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "mtw2"          + suffix, "m_{T}^{W,2}",                       10, 0, "GeV",  scale, true, 0,  500);
	  plotter.Draw(prefix + "ht"            + suffix, "H_{T}",                             20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "htjets"        + suffix, "#sum_{jet} p_{T}",                  20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "htnojets"      + suffix, "p_{T}^{lep1} + p_{T}^{lep2} + MET", 20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "nbjet15loose"  + suffix, "number of (15 GeV) loose b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet15medium" + suffix, "number of (15 GeV) medium b-jets",  -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet15tight"  + suffix, "number of (15 GeV) tight b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20loose"  + suffix, "number of (20 GeV) loose b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20medium" + suffix, "number of (20 GeV) medium b-jets",  -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20tight"  + suffix, "number of (20 GeV) tight b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet30loose"  + suffix, "number of (30 GeV) loose b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet30medium" + suffix, "number of (30 GeV) medium b-jets",  -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet30tight"  + suffix, "number of (30 GeV) tight b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "njet"          + suffix, "number of (30 GeV) jets",           -1, 0, "NULL", scale);


	  // WW and MonoH histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("WW") || analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "fullpmet" + suffix, "projected E_{T}^{miss}",       10, 0, "GeV", scale, false, 0, 100);
	      plotter.Draw(prefix + "trkpmet"  + suffix, "projected track E_{T}^{miss}",  2, 0, "GeV", scale, false, 0, 100);
	      plotter.Draw(prefix + "mllstar"  + suffix, "m2l^{*}",                      10, 0, "GeV", scale, false, 0, 300);
	    }


	  // MonoH histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "deltarl1met"    + suffix, "#DeltaR(lep1,E_{T}^{miss})",  2, 1, "NULL", scale, false, 0, 4);
	      plotter.Draw(prefix + "deltarl2met"    + suffix, "#DeltaR(lep2,E_{T}^{miss})",  2, 1, "NULL", scale, false, 0, 4);
	      plotter.Draw(prefix + "deltarllmet"    + suffix, "#DeltaR(ll,E_{T}^{miss})",    2, 1, "NULL", scale, false, 0, 4);
              plotter.Draw(prefix + "deltarjet1met"  + suffix, "#DeltaR(jet1,E_{T}^{miss})",  5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarjet2met"  + suffix, "#DeltaR(jet2,E_{T}^{miss})",  5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarjj"       + suffix, "#DeltaR(jet1,jet2)",          5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarjjmet"    + suffix, "#DeltaR(jj,E_{T}^{miss})",    5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarlep1jet1" + suffix, "#DeltaR(lep1,jet1)",          5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarlep1jet2" + suffix, "#DeltaR(lep1,jet2)",          5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarlep2jet1" + suffix, "#DeltaR(lep2,jet1)",          5, 2, "NULL", scale);
              plotter.Draw(prefix + "deltarlep2jet2" + suffix, "#DeltaR(lep2,jet2)",          5, 2, "NULL", scale);
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


  // Cross-section
  //----------------------------------------------------------------------------
  if (analysis.EqualTo("WZ") && level.Contains("BVeto"))
    {
      printf("\n Cross section mu\n");
      printf("------------------\n");

      for (int i=firstchannel; i<=lastchannel; i++)
	plotter.CrossSection(level, schannel[i], analysis);

      printf("\n");
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
