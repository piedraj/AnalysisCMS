#include "HistogramReader.h"


// Constants
//------------------------------------------------------------------------------
const Bool_t datadriven = true;
const Bool_t allplots   = false;

const TString inputdir  = "../rootfiles/nominal/";
const TString outputdir = "figures/";

const TString sl  = "#font[12]{l}";
const TString sll = "#font[12]{ll}";
const TString sm  = "E_{T}^{miss}";

enum {linY, logY};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// runPlotter
//
// option = "hist"         --> all distributions normalized to the luminosity
// option = "nostack,hist" --> signal and top distributions normalized to one
//
//   Draw(TString  hname,                  Name of the histogram.
//        TString  xtitle       = "",      Title of the x-axis.
//        Int_t    ngroup       = -1,      Number of bins to be merged into one bin.
//        Int_t    precision    = 0,       Number of decimal digits.
//        TString  units        = "NULL",  Units of the histogram.
//        Bool_t   setlogy      = false,   Set it to true (false) for logarithmic (linear) scale.
//        Bool_t   moveoverflow = true,    Set it to true to plot the events out of range.
//        Float_t  xmin         = -999,
//        Float_t  xmax         = -999,
//        Float_t  ymin         = -999,
//        Float_t  ymax         = -999);
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

  float lumi = lumi_fb_2016;

  if (analysis.EqualTo("Shape")) lumi = lumi_fb_Run2016B;

  Bool_t scale = linY;

  if (analysis.EqualTo("MonoH")) scale = logY;
  if (analysis.EqualTo("Stop"))  scale = logY;
  if (analysis.EqualTo("Top"))   scale = logY;

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  HistogramReader plotter(inputdir + analysis, outputdir);

  plotter.SetStackOption(option);
  plotter.SetPublicStyle(false);
  plotter.SetSavePdf    (false);

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
      plotter.AddProcess("02_WZTo3LNu", "WZ",       color_WZTo3LNu);
      plotter.AddProcess("06_WW",       "WW",       color_WW);
      plotter.AddProcess("11_Wg",       "W#gamma",  color_Wg);
      plotter.AddProcess("15_WgStat",   "W#gamma*", color_WgStar);
      plotter.AddProcess("03_VZ",       "VZ",       color_VZ);
      plotter.AddProcess("09_TTV",      "ttV",      color_TTV);
      plotter.AddProcess("13_VVV",      "VVV",      color_VVV);

      if (datadriven)
	{
	  // -999 is needed to not scale by luminosity
	  plotter.AddProcess("00_Fakes", "non-prompt", color_Fakes, roc_background, -999);
	  plotter.AddProcess("12_Zg",    "Z#gamma",    color_Zg);
	}
      else
	{
	  plotter.AddProcess("07_ZJets",     "Z+jets", color_ZJets);
	  plotter.AddProcess("04_TTTo2L2Nu", "tt",     color_TTTo2L2Nu);
	  plotter.AddProcess("05_ST",        "tW",     color_ST);
	}
    }
  else
    {
      plotter.AddProcess("14_HZ",        "HZ",       color_HZ);
      plotter.AddProcess("10_HWW",       "HWW",      color_HWW);
      plotter.AddProcess("06_WW",        "WW",       color_WW, roc_signal);
      plotter.AddProcess("02_WZTo3LNu",  "WZ",       color_WZTo3LNu);
      plotter.AddProcess("03_VZ",        "VZ",       color_VZ);
      plotter.AddProcess("11_Wg",        "W#gamma",  color_Wg);
      plotter.AddProcess("15_WgStar",    "W#gamma*", color_WgStar);
      plotter.AddProcess("07_ZJets",     "Z+jets",   color_ZJets);
      plotter.AddProcess("09_TTV",       "ttV",      color_TTV);
      plotter.AddProcess("04_TTTo2L2Nu", "tt",       color_TTTo2L2Nu);
      plotter.AddProcess("05_ST",        "tW",       color_ST);

      if (datadriven)
	{
	  // -999 is needed to not scale by luminosity
	  plotter.AddProcess("00_Fakes", "non-prompt", color_Fakes, roc_background, -999);
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
      plotter.AddSignal("monoH_2HDM_MZp-600_MA0-400",  "m_{Z'} 600",  color_Signal-4);
      plotter.AddSignal("monoH_2HDM_MZp-800_MA0-400",  "m_{Z'} 800",  color_Signal-3);
      plotter.AddSignal("monoH_2HDM_MZp-1200_MA0-400", "m_{Z'} 1200", color_Signal-1);
      plotter.AddSignal("monoH_2HDM_MZp-1700_MA0-400", "m_{Z'} 1700", color_Signal+1);
      plotter.AddSignal("monoH_2HDM_MZp-2000_MA0-400", "m_{Z'} 2000", color_Signal+2);
      plotter.AddSignal("monoH_2HDM_MZp-2500_MA0-400", "m_{Z'} 2500", color_Signal+3);
    }


  if (analysis.EqualTo("TTDM"))
    {
      plotter.AddSignal("ttDM0001scalar00010", "m_{#chi}1 m_{S}10",  color_Signal);
      plotter.AddSignal("ttDM0001scalar00500", "m_{#chi}1 m_{S}500", color_Signal+2);
    }


  if (analysis.EqualTo("Stop"))
    {
      plotter.AddSignal("T2tt_mStop??", "m_{Stop}350-400",  color_Signal);  
      plotter.AddSignal("T2tt_mStop??", "m_{Stop}400-1200", color_Signal+2);  
    }


  // Draw events by cut
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  gSystem->mkdir(outputdir + level, kTRUE);

  for (int i=firstchannel; i<=lastchannel; i++)
    {
      plotter.LoopEventsByCut(analysis, "h_counterLum_" + schannel[i]);

      TString title = (i < lastchannel) ? lchannel[i] : "inclusive";

      plotter.SetTitle(title);

      plotter.Draw(analysis + "/h_counterLum_" + schannel[i] + "_evolution", "", -1, 0, "NULL", logY, false);
    }


  // Draw events by channel
  //----------------------------------------------------------------------------
  plotter.SetDrawYield(false);

  for (int j=0; j<=njetbin; j++)
    {
      if (!analysis.EqualTo("Top")  &&
	  !analysis.EqualTo("Stop") &&
	  !analysis.EqualTo("WW")   &&
	  j != njetbin) continue;
      
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      plotter.LoopEventsByChannel(level + jetbin);

      plotter.Draw(level + jetbin + "/h_counterLum_evolution", "", -1, 0, "NULL", scale, false);
    }


  // Draw distributions
  //----------------------------------------------------------------------------
  if (!option.Contains("nostack")) plotter.SetDrawYield(true);
  if (analysis.EqualTo("MonoH"))   plotter.SetDrawYield(false);

  float m2l_xmin   = (level.Contains("WZ")) ?  60 :   0;  // [GeV]
  float m2l_xmax   = (level.Contains("WZ")) ? 120 : 300;  // [GeV]
  int   m2l_ngroup = (level.Contains("WZ")) ?   2 :   5;
  
  for (int j=0; j<=njetbin; j++)
    {
      if (!analysis.EqualTo("Top")  &&
	  !analysis.EqualTo("Stop") &&
	  !analysis.EqualTo("WW")   &&
	  j != njetbin) continue;   
         
      TString jetbin = (j < njetbin) ? Form("/%djet", j) : "";

      gSystem->mkdir(outputdir + level + jetbin, kTRUE);

      TString prefix = level + jetbin + "/h_";

      for (int i=firstchannel; i<=lastchannel; i++)
	{
	  TString suffix = "_" + schannel[i];
	  
	  TString title = (i < lastchannel) ? lchannel[i] : "inclusive";

	  plotter.SetTitle(title);


	  // Common histograms
	  //--------------------------------------------------------------------
	  plotter.Draw(prefix + "m2l" + suffix, "m_{" + sll + "}", m2l_ngroup, 0, "GeV", logY, true, m2l_xmin, m2l_xmax);
	  plotter.Draw(prefix + "m2l" + suffix, "m_{" + sll + "}", m2l_ngroup, 0, "GeV", linY, true, m2l_xmin, m2l_xmax);

	  plotter.Draw(prefix + "njet"           + suffix, "number of 30 GeV jets",             -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet20cmvav2l" + suffix, "number of 20 GeV cmvav2l b-jets",   -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "nbjet30csvv2m"  + suffix, "number of 30 GeV csvv2m b-jets",    -1, 0, "NULL", scale);
	  plotter.Draw(prefix + "dphillmet"      + suffix, "#Delta#phi(" +sll + "," + sm + ")",  5, 2, "rad",  scale);
	  plotter.Draw(prefix + "metPfType1Phi"  + suffix, sm + " #phi",                         5, 2, "rad",  scale);
	  plotter.Draw(prefix + "metPfType1"     + suffix, sm,                                  10, 0, "GeV",  scale, true, 0,  200);
	  plotter.Draw(prefix + "nvtx"           + suffix, "number of vertices",                -1, 0, "NULL", scale, true, 0,   30);
	  plotter.Draw(prefix + "lep1pt"         + suffix, "leading lepton p_{T}",               5, 0, "GeV",  scale, true, 0,  150);
	  plotter.Draw(prefix + "lep2pt"         + suffix, "trailing lepton p_{T}",              5, 0, "GeV",  scale, true, 0,  150);
	  plotter.Draw(prefix + "lep1eta"        + suffix, "leading lepton #eta",               -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "lep2eta"        + suffix, "trailing lepton #eta",              -1, 1, "NULL", scale);
	  plotter.Draw(prefix + "lep1phi"        + suffix, "leading lepton #phi",                5, 2, "rad",  scale);
	  plotter.Draw(prefix + "lep2phi"        + suffix, "trailing lepton #phi",               5, 2, "rad",  scale);
	  plotter.Draw(prefix + "jet1eta"        + suffix, "leading jet #eta",                  -1, 1, "NULL", scale, false);
	  plotter.Draw(prefix + "jet2eta"        + suffix, "trailing jet #eta",                 -1, 1, "NULL", scale, false);
	  plotter.Draw(prefix + "jet1phi"        + suffix, "leading jet #phi",                   5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "jet2phi"        + suffix, "trailing jet #phi",                  5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "jet1pt"         + suffix, "leading jet p_{T}",                  5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "jet2pt"         + suffix, "trailing jet p_{T}",                 5, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "dphill"         + suffix, "#Delta#phi(lep1,lep2)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "detall"         + suffix, "#Delta#eta(lep1,lep2)",              5, 2, "rad",  scale, true, 0, 5);
	  plotter.Draw(prefix + "topReco"        + suffix, "number of tt reco solutions",       -1, 0, "NULL", scale);


	  // ROC
	  //--------------------------------------------------------------------
	  // S / #sqrt{B}
	  // S / #sqrt{S+B}
	  // S / B
	  // Punzi Eq.6 (https://arxiv.org/pdf/physics/0308063v2.pdf)
	  // Punzi Eq.7 (https://arxiv.org/pdf/physics/0308063v2.pdf)
	  plotter.Roc(prefix + "ht"    + suffix, "H_{T}",         1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "pt2l"  + suffix, "p_{T}^{ll}",    1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "mth"   + suffix, "m_{T}^{ll}",    1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "mtw1"  + suffix, "m_{T}^{W1}",    1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "mtw2"  + suffix, "m_{T}^{W2}",    1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "mt2ll" + suffix, "m_{T2}^{ll}",   1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "m2l"   + suffix, "m_{ll}",        1000, "GeV", 0, 1000, "Punzi Eq.6");
	  plotter.Roc(prefix + "drll"  + suffix, "#Delta R_{ll}",   50, "rad", 0,    5, "Punzi Eq.6");


	  if (!allplots) continue;


	  plotter.Draw(prefix + "dyll"         + suffix, "lepton #Delta#eta",                 -1, 3, "NULL", scale);
	  plotter.Draw(prefix + "dphimetjet"   + suffix, "min #Delta#phi(jet," + sm + ")",     5, 2, "rad",  scale);
	  plotter.Draw(prefix + "dphimetptbll" + suffix, "#Delta#phi(llmet," + sm + ")",       5, 2, "rad",  scale);
	  plotter.Draw(prefix + "mllbb"        + suffix, "m_{" + sll + "bb}",                 10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "meff"         + suffix, "m_{eff}",                           10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "ptbll"        + suffix, "p_{T}^{llmet}",                     10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "mt2ll"        + suffix, "M_{T2}(" + sll + ")",               10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "mt2bb"        + suffix, "M_{T2}(bb)" ,                       10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "mt2lblb"      + suffix, "M_{T2}(" + sl + "b" + sl + "b)",    10, 0, "GeV",  scale, false, 0, 600);
	  plotter.Draw(prefix + "dphijet1met"  + suffix, "#Delta#phi(jet1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphijet2met"  + suffix, "#Delta#phi(jet2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphijj"       + suffix, "#Delta#phi(jet1,jet2)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphijjmet"    + suffix, "#Delta#phi(jj,E_{T}^{miss})",        5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilep1jet1" + suffix, "#Delta#phi(lep1,jet1)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilep1jet2" + suffix, "#Delta#phi(lep1,jet2)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilep2jet1" + suffix, "#Delta#phi(lep2,jet1)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilep2jet2" + suffix, "#Delta#phi(lep2,jet2)",              5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphillstar"   + suffix, "#Delta#phi*(lep1,lep2)",             5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilmet1"    + suffix, "#Delta#phi(lep1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "dphilmet2"    + suffix, "#Delta#phi(lep2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "metTtrkPhi"   + suffix, "track E_{T}^{miss} #phi",            5, 2, "rad",  scale, false);
	  plotter.Draw(prefix + "drll"         + suffix, "#DeltaR(lep1,lep2)",                 5, 1, "NULL", scale, false);
	  plotter.Draw(prefix + "jet1mass"     + suffix, "leading jet mass",                  -1, 0, "GeV",  scale, true, 0,   50);
	  plotter.Draw(prefix + "jet2mass"     + suffix, "trailing jet mass",                 -1, 0, "GeV",  scale, true, 0,   50);
	  plotter.Draw(prefix + "mc"           + suffix, "m_{c}",                             10, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "metTtrk"      + suffix, "track E_{T}^{miss}",                10, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "mpmet"        + suffix, "min projected E_{T}^{miss}",        10, 0, "GeV",  logY,  true, 0,  200);
	  plotter.Draw(prefix + "mth"          + suffix, "m_{T}^{H}",                         10, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "mtw1"         + suffix, "m_{T}^{W,1}",                       10, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "mtw2"         + suffix, "m_{T}^{W,2}",                       10, 0, "GeV",  scale, true, 0,  400);
	  plotter.Draw(prefix + "ht"           + suffix, "H_{T}",                             20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "htjets"       + suffix, "#sum_{jet} p_{T}",                  20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "htnojets"     + suffix, "p_{T}^{lep1} + p_{T}^{lep2} + MET", 20, 0, "GeV",  scale, true, 0, 1500);
	  plotter.Draw(prefix + "pt2l"         + suffix, "p_{T}^{#font[12]{ll}}",             10, 0, "GeV",  scale, true, 0,  300);
	  plotter.Draw(prefix + "ptww"         + suffix, "p_{T}^{WW}",                        10, 0, "GeV",  scale, true, 0,  600);
	  plotter.Draw(prefix + "sumjpt12"     + suffix, "p_{T}^{jet1} + p_{T}^{jet2}",       10, 0, "GeV",  scale, true, 0,  600);
	  plotter.Draw(prefix + "sumpt12"      + suffix, "p_{T}^{lep1} + p_{T}^{lep2}",       10, 0, "GeV",  scale, true, 0,  600);


	  // WW and MonoH histograms
	  //--------------------------------------------------------------------
	  if (analysis.EqualTo("WW") || analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "fullpmet" + suffix, "projected E_{T}^{miss}",       10, 0, "GeV", scale, false, 0, 100);
	      plotter.Draw(prefix + "trkpmet"  + suffix, "projected track E_{T}^{miss}",  2, 0, "GeV", scale, false, 0, 100);
	      plotter.Draw(prefix + "mllstar"  + suffix, "m2l^{*}",                      10, 0, "GeV", scale, false, 0, 300);
	      plotter.Draw(prefix + "metPuppi" + suffix, "PUPPI E_{T}^{miss}",           10, 0, "GeV", scale, true,  0, 400);
	    }

	  if (analysis.EqualTo("MonoH"))
	    {
	      plotter.Draw(prefix + "deltarl1met"    + suffix, "#DeltaR(lep1,E_{T}^{miss})", 2, 1, "NULL", scale, false, 0, 4);
	      plotter.Draw(prefix + "deltarl2met"    + suffix, "#DeltaR(lep2,E_{T}^{miss})", 2, 1, "NULL", scale, false, 0, 4);
	      plotter.Draw(prefix + "deltarllmet"    + suffix, "#DeltaR(ll,E_{T}^{miss})",   2, 1, "NULL", scale, false, 0, 4);
              plotter.Draw(prefix + "deltarjet1met"  + suffix, "#DeltaR(jet1,E_{T}^{miss})", 5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarjet2met"  + suffix, "#DeltaR(jet2,E_{T}^{miss})", 5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarjj"       + suffix, "#DeltaR(jet1,jet2)",         5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarjjmet"    + suffix, "#DeltaR(jj,E_{T}^{miss})",   5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarlep1jet1" + suffix, "#DeltaR(lep1,jet1)",         5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarlep1jet2" + suffix, "#DeltaR(lep1,jet2)",         5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarlep2jet1" + suffix, "#DeltaR(lep2,jet1)",         5, 2, "NULL", scale, false);
              plotter.Draw(prefix + "deltarlep2jet2" + suffix, "#DeltaR(lep2,jet2)",         5, 2, "NULL", scale, false);
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


  // Cross section
  //----------------------------------------------------------------------------
  //  root -l eos/cms/store/group/phys_higgs/cmshww/amassiro/HWW12fb_v2/07Jun2016_spring16_mAODv2_12pXfbm1/MCl2loose__hadd__bSFL2pTEff__l2tight/latino_WWTo2L2Nu.root as _file0...
  //  mcWeightPos->GetEntries() - mcWeightNeg->GetEntries()
  //  1.96718e+06
  //
  if (analysis.EqualTo("Control") && level.Contains("WW"))
    {
      printf("\n Cross section\n");
      printf("---------------\n\n");

      for (int i=firstchannel; i<=lastchannel; i++)
	plotter.CrossSection(level,
			     schannel[i],
			     "WW",
			     WW2lnu,
			     "WWTo2L2Nu",            12.1780, 1967180,
			     "GluGluWWTo2L2Nu_MCFM",  0.5905,  481600);
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
