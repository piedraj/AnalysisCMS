// root -l -b -q Plotter.C 

#include "../test/HistogramReader.C"

const TString inputdir  = "histos/";
const TString outputdir = "figures/";

const TString sl  = "#font[12]{l}";
const TString sll = "#font[12]{ll}";
const TString sm  = "E_{T}^{miss}";

enum {linY, logY};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Plotter
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

void Plotter(TString option = "hist"){

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  float lumi = lumi_fb_2016;

  Bool_t scale = logY;

  HistogramReader plotter(inputdir, outputdir);

  plotter.SetStackOption(option);
  plotter.SetPublicStyle(false);
  plotter.SetSavePdf    (true);

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
  plotter.AddProcess("14_HZ",        "HZ",         color_HZ);
  plotter.AddProcess("10_HWW",       "HWW",        color_HWW);
  plotter.AddProcess("06_WW",        "WW",         color_WW);
  plotter.AddProcess("02_WZTo3LNu",  "WZ",         color_WZTo3LNu);
  plotter.AddProcess("03_VZ",        "VZ",         color_VZ);
  plotter.AddProcess("11_Wg",        "W#gamma",    color_Wg);
  plotter.AddProcess("15_WgStar",    "W#gamma*",   color_WgStar);
  plotter.AddProcess("07_ZJets",     "Z+jets",     color_ZJets);
  plotter.AddProcess("09_TTV",       "ttV",        color_TTV);
  plotter.AddProcess("04_TTTo2L2Nu", "tt",         color_TTTo2L2Nu);
  plotter.AddProcess("05_ST",        "tW",         color_ST);
  plotter.AddProcess("00_Fakes",     "non-prompt", color_Fakes, roc_background, -999); // -999 is needed to not scale by luminosity

  // Add signals
  //----------------------------------------------------------------------------
  plotter.AddSignal("ttDM0001scalar00010", "m_{#chi}1 m_{S}10",  color_Signal, roc_signal);
  plotter.AddSignal("ttDM0001scalar00500", "m_{#chi}1 m_{S}500", color_Signal+4);
 

  // ROC curve inputs
  //----------------------------------------------------------------------------                 
  //   plotter.AddRocSignal("ttDM-signal"); 

  //   plotter.AddRocBackground("06_WW");
  //   plotter.AddRocBackground("04_TTTo2L2Nu");
  //   plotter.AddRocBackground("00_Fakes");
  //   plotter.AddRocBackground("02_WZTo3LNu");
  //   plotter.AddRocBackground("03_VZ");
  //   plotter.AddRocBackground("05_ST");
  //   plotter.AddRocBackground("07_ZJets");
  //   plotter.AddRocBackground("09_TTV");
  //   plotter.AddRocBackground("10_HWW");
  //   plotter.AddRocBackground("11_Wg");
  //   plotter.AddRocBackground("12_Zg");
  //   plotter.AddRocBackground("13_VVV");
  //   plotter.AddRocBackground("14_HZ");
  //   plotter.AddRocBackground("15_WgStar");



  // Draw distributions
  //----------------------------------------------------------------------------
  if (!option.Contains("nostack")) plotter.SetDrawYield(true);

  gSystem->mkdir(outputdir, kTRUE);

  TString title = "cms";

  plotter.SetTitle(title);


  //            hname              xtitle                     ngroup precision units  setlogy moveoverflow xmin xmax ymin ymax
  // -------------------------------------------------------------------------------------------------------------------------
  //plotter.Draw( "dphijet1met"    , "#Delta#phi(jet1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijet2met"    , "#Delta#phi(jet2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijj"         , "#Delta#phi(jet1,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijjmet"      , "#Delta#phi(jj,E_{T}^{miss})",        5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep1jet1"   , "#Delta#phi(lep1,jet1)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep1jet2"   , "#Delta#phi(lep1,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep2jet1"   , "#Delta#phi(lep2,jet1)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep2jet2"   , "#Delta#phi(lep2,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphill"         , "#Delta#phi(lep1,lep2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphillmet"      , "#Delta#phi(" +sll + "," + sm + ")",  5, 2, "rad",  scale);
  //plotter.Draw( "dphilmet1"      , "#Delta#phi(lep1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilmet2"      , "#Delta#phi(lep2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "ht"             , "H_{T}",                             20, 0, "GeV",  scale, true, 0, 1500);
  //plotter.Draw( "htjets"         , "#sum_{jet} p_{T}",                  20, 0, "GeV",  scale, true, 0, 1500);
  //plotter.Draw( "htnojets"       , "p_{T}^{lep1} + p_{T}^{lep2} + MET", 20, 0, "GeV",  scale, true, 0, 1500);
  //plotter.Draw( "jet1eta"        , "leading jet #eta",                  -1, 1, "NULL", scale, false);
  //plotter.Draw( "jet1mass"       , "leading jet mass",                  -1, 0, "GeV",  scale, true, 0,   50);
  //plotter.Draw( "jet1phi"        , "leading jet #phi",                   5, 2, "rad",  scale, false);
  //plotter.Draw( "jet1pt"         , "leading jet p_{T}",                  5, 0, "GeV",  scale, true, 0,  400);
  //plotter.Draw( "jet2eta"        , "trailing jet #eta",                 -1, 1, "NULL", scale, false);
  //plotter.Draw( "jet2mass"       , "trailing jet mass",                 -1, 0, "GeV",  scale, true, 0,   50);
  //plotter.Draw( "jet2phi"        , "trailing jet #phi",                  5, 2, "rad",  scale, false);
  //plotter.Draw( "jet2pt"         , "trailing jet p_{T}",                 5, 0, "GeV",  scale, true, 0,  400);
  //plotter.Draw( "lep1eta"        , "leading lepton #eta",               -1, 1, "NULL", scale);
  //plotter.Draw( "lep1phi"        , "leading lepton #phi",                5, 2, "rad",  scale);
  //plotter.Draw( "lep1pt"         , "leading lepton p_{T}",               5, 0, "GeV",  scale, true, 0,  150);
  //plotter.Draw( "lep2eta"        , "trailing lepton #eta",              -1, 1, "NULL", scale);
  //plotter.Draw( "lep2phi"        , "trailing lepton #phi",               5, 2, "rad",  scale);
  //plotter.Draw( "lep2pt"         , "trailing lepton p_{T}",              5, 0, "GeV",  scale, true, 0,  150);
  plotter.Draw( "m2l"            , "m_{" + sll + "}",                   10, 0, "GeV",  scale, true, 0,  300);
  plotter.Draw( "metPfType1"     , sm,                                  10, 0, "GeV",  scale, true, 0,  200);
  //plotter.Draw( "mt2lblb"        , "M_{T2}(" + sl + "b" + sl + "b)",    10, 0, "GeV",  scale, false, 0, 600);
  //plotter.Draw( "mt2ll"          , "M_{T2}(" + sll + ")",               10, 0, "GeV",  scale, false, 0, 600); 
  //plotter.Draw( "mtw1"           , "m_{T}^{W,1}",                       10, 0, "GeV",  scale, true, 0,  400);
  //plotter.Draw( "mtw2"           , "m_{T}^{W,2}",                       10, 0, "GeV",  scale, true, 0,  400);
  plotter.Draw( "nbjet30csvv2m"  , "number of 30 GeV csvv2m b-jets",    -1, 0, "NULL", scale, true, 0,  6);
  plotter.Draw( "njet"           , "number of 30 GeV jets",             -1, 0, "NULL", scale, true, 0,  6);
  //plotter.Draw( "nvtx"           , "number of vertices",                -1, 0, "NULL", linY,  true, 0,   30);




  //           hname     xtitle                      npoints  units xmin xmax
  //  ---------------------------------------------------------------------------
  //plotter.Roc( "lep1pt", "leading lepton p_{T}",        1000, "GeV", 0, 1000);



  // Copy index.php in every directory
  //----------------------------------------------------------------------------
  gSystem->Exec("for dir in $(find ./ -type d); do cp -n ../index.php $dir/; done");
  gSystem->Exec("rm -f index.php");

}

