//root -l -b -q "Plotter.C(\"hist\", \"ttDM0001scalar00010\")"

#include "../test/HistogramReader.C"

const TString inputdir  = "/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/carlota_blinded/";
const TString outputdir = "/afs/cern.ch/user/j/jgarciaf/www/figures/Analysis_170907_carlota_blinded/";

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

void Plotter(TString option = "hist", TString signal = "ttDM0001scalar00010" ){

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  float lumi = lumi_fb_Full2016/15.;

  Bool_t scale = logY;

  float ymin = ( scale == linY )  ?  0  :  1 ;

  HistogramReader plotter( inputdir + signal + "/", outputdir);

  plotter.SetStackOption  (option);
  plotter.SetPublicStyle  (false);
  plotter.SetMinitreeBased(true);
  plotter.SetSavePdf      (true);

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
  plotter.AddData("01_Data_1outof15", "data", color_Data);
  //plotter.AddData("01_Data_Full2016", "data", color_Data);

  // Add processes
  //----------------------------------------------------------------------------
  plotter.AddProcess("06_WW",             "WW",         color_WW,        roc_background, -999 );
  plotter.AddProcess("02_WZTo3LNu",       "WZ",         color_WZTo3LNu,  roc_background, -999 );
  plotter.AddProcess("03_VZ",             "VZ",         color_VZ,        roc_background, -999 );
  plotter.AddProcess("13_VVV",            "VVV",        color_VVV,       roc_background, -999 );
  plotter.AddProcess("07_ZJets",          "DY",         color_ZJets,     roc_background, -999 );
  plotter.AddProcess("09_TTV",            "ttV",        color_TTV,       roc_background, -999 );
  plotter.AddProcess("05_ST",             "tW",         color_ST,        roc_background, -999 );
  plotter.AddProcess("00_Fakes_Full2016", "non-prompt", color_Fakes,     roc_background, -999 ); // -999 is needed to not scale by luminosity
  plotter.AddProcess("04_TTTo2L2Nu",      "tt",         color_TTTo2L2Nu, roc_background, -999 );
  //plotter.AddProcess("04_TTTo2L2Nu_1",    "tt 1st",     color_TTTo2L2Nu, roc_background, -999 );
  //plotter.AddProcess("04_TTTo2L2Nu_2",    "tt 2nd",     color_TTTo2L2Nu, roc_background, -999 );



  // Add signals
  //----------------------------------------------------------------------------

  ///plotter.AddSignal("ttDM0001scalar00010", "m_{#chi}1 m_{S}10 x20"  ,  color_Signal, roc_signal,20./2.4 );
  ///plotter.AddSignal("ttDM0001scalar00500", "m_{#chi}1 m_{S}500 x1e4",  color_Signal+4, roc_signal,10000./2.4);
  //plotter.AddSignal("ttDM0001scalar00100", "m_{#chi}1 m_{S}100 x200",  color_Signal, roc_signal, 200./2.4 );
  //plotter.AddSignal("ttDM0001pseudo00100", "m_{#chi}1 m_{P}100 x300",  color_Signal, roc_signal, 300./2.4 );

//plotter.AddSignal("ttDM0001scalar00200", "m_{#chi}1 m_{S}200 x400",  color_Signal, roc_signal, 400./2.4  );
//plotter.AddSignal("ttDM0001pseudo00200", "m_{#chi}1 m_{P}200 x400",  color_Signal+4, roc_signal, 400./2.4  );

  if( signal.Contains("ttDM0001scalar00010") )   plotter.AddSignal("ttDM0001scalar00010", "m_{#chi}1 m_{S}10 x20",    color_Signal, roc_signal, 20./2.4   );
  if( signal.Contains("ttDM0001scalar00020") )   plotter.AddSignal("ttDM0001scalar00020", "m_{#chi}1 m_{S}20 x50",    color_Signal, roc_signal, 50./2.4   );
  if( signal.Contains("ttDM0001scalar00050") )   plotter.AddSignal("ttDM0001scalar00050", "m_{#chi}1 m_{S}50 x100",   color_Signal, roc_signal, 100./2.4  );
  if( signal.Contains("ttDM0001scalar00100") )   plotter.AddSignal("ttDM0001scalar00100", "m_{#chi}1 m_{S}100 x200",  color_Signal, roc_signal, 200./2.4  );
  if( signal.Contains("ttDM0001scalar00200") )   plotter.AddSignal("ttDM0001scalar00200", "m_{#chi}1 m_{S}200 x400",  color_Signal, roc_signal, 400./2.4  );
  if( signal.Contains("ttDM0001scalar00300") )   plotter.AddSignal("ttDM0001scalar00300", "m_{#chi}1 m_{S}300 x1000", color_Signal, roc_signal, 1000./2.4 );
  if( signal.Contains("ttDM0001scalar00500") )   plotter.AddSignal("ttDM0001scalar00500", "m_{#chi}1 m_{S}500 x1e4",  color_Signal, roc_signal,10000./2.4 );
  if( signal.Contains("ttDM0001pseudo00010") )   plotter.AddSignal("ttDM0001pseudo00010", "m_{#chi}1 m_{P}10 x100",   color_Signal, roc_signal, 100./2.4  );
  if( signal.Contains("ttDM0001pseudo00020") )   plotter.AddSignal("ttDM0001pseudo00020", "m_{#chi}1 m_{P}20 x150",   color_Signal, roc_signal, 150./2.4  );
  if( signal.Contains("ttDM0001pseudo00050") )   plotter.AddSignal("ttDM0001pseudo00050", "m_{#chi}1 m_{P}50 x200",   color_Signal, roc_signal, 200./2.4  );
  if( signal.Contains("ttDM0001pseudo00100") )   plotter.AddSignal("ttDM0001pseudo00100", "m_{#chi}1 m_{P}100 x300",  color_Signal, roc_signal, 300./2.4  );
  if( signal.Contains("ttDM0001pseudo00200") )   plotter.AddSignal("ttDM0001pseudo00200", "m_{#chi}1 m_{P}200 x400",  color_Signal, roc_signal, 400./2.4  );
  if( signal.Contains("ttDM0001pseudo00300") )   plotter.AddSignal("ttDM0001pseudo00300", "m_{#chi}1 m_{P}300 x1000", color_Signal, roc_signal, 1000./2.4 );
  if( signal.Contains("ttDM0001pseudo00500") )   plotter.AddSignal("ttDM0001pseudo00500", "m_{#chi}1 m_{P}500 x1e4",  color_Signal, roc_signal,10000./2.4 );

  /*if( signal.Contains("ttDM0001scalar00010") )   plotter.AddSignal("ttDM0001scalar00010", "m_{#chi}1 m_{S}10 x2",    color_Signal, roc_signal, 2.  );
  if( signal.Contains("ttDM0001scalar00020") )   plotter.AddSignal("ttDM0001scalar00020", "m_{#chi}1 m_{S}20 x5",    color_Signal, roc_signal, 5.  );
  if( signal.Contains("ttDM0001scalar00050") )   plotter.AddSignal("ttDM0001scalar00050", "m_{#chi}1 m_{S}50 x10",   color_Signal, roc_signal, 10. );
  if( signal.Contains("ttDM0001scalar00100") )   plotter.AddSignal("ttDM0001scalar00100", "m_{#chi}1 m_{S}100 x20",  color_Signal, roc_signal, 20. );
  if( signal.Contains("ttDM0001scalar00200") )   plotter.AddSignal("ttDM0001scalar00200", "m_{#chi}1 m_{S}200 x40",  color_Signal, roc_signal, 40. );
  if( signal.Contains("ttDM0001scalar00300") )   plotter.AddSignal("ttDM0001scalar00300", "m_{#chi}1 m_{S}300 x100", color_Signal, roc_signal, 100.);
  if( signal.Contains("ttDM0001scalar00500") )   plotter.AddSignal("ttDM0001scalar00500", "m_{#chi}1 m_{S}500 x1e3",  color_Signal, roc_signal,1000.);
  if( signal.Contains("ttDM0001pseudo00010") )   plotter.AddSignal("ttDM0001pseudo00010", "m_{#chi}1 m_{P}10 x10",   color_Signal, roc_signal, 10. );
  if( signal.Contains("ttDM0001pseudo00020") )   plotter.AddSignal("ttDM0001pseudo00020", "m_{#chi}1 m_{P}20 x15",   color_Signal, roc_signal, 15. );
  if( signal.Contains("ttDM0001pseudo00050") )   plotter.AddSignal("ttDM0001pseudo00050", "m_{#chi}1 m_{P}50 x20",   color_Signal, roc_signal, 20. );
  if( signal.Contains("ttDM0001pseudo00100") )   plotter.AddSignal("ttDM0001pseudo00100", "m_{#chi}1 m_{P}100 x30",  color_Signal, roc_signal, 30. );
  if( signal.Contains("ttDM0001pseudo00200") )   plotter.AddSignal("ttDM0001pseudo00200", "m_{#chi}1 m_{P}200 x40",  color_Signal, roc_signal, 40. );
  if( signal.Contains("ttDM0001pseudo00300") )   plotter.AddSignal("ttDM0001pseudo00300", "m_{#chi}1 m_{P}300 x100", color_Signal, roc_signal, 100.);
  if( signal.Contains("ttDM0001pseudo00500") )   plotter.AddSignal("ttDM0001pseudo00500", "m_{#chi}1 m_{P}500 x1e3",  color_Signal, roc_signal,1000.);*/

  // Add systematics
  //----------------------------------------------------------------------------
  //plotter.AddSystematic("_Btagup"       );
  //plotter.AddSystematic("_Btagdo"       );
  //plotter.AddSystematic("_Idisoup"      );		
  //plotter.AddSystematic("_Triggerup"    );		
  //plotter.AddSystematic("_METup"        );		
  //plotter.AddSystematic("_JESup"        );		
  //plotter.AddSystematic("_LepElepTCutup");		
  //plotter.AddSystematic("_LepMupTCutup" );
  //plotter.AddSystematic("_QCDup"        );
  //plotter.AddSystematic("_QCDdo"        );
  //plotter.AddSystematic("_PDFup"        );
  //plotter.AddSystematic("_PDFdo"        );
  //plotter.AddSystematic("_toppTrwup"    );
  //plotter.AddSystematic("_toppTrwdo"    );

  // Draw distributions
  //----------------------------------------------------------------------------
  if (!option.Contains("nostack")) plotter.SetDrawYield(true);

  gSystem->mkdir(outputdir, kTRUE);

  TString title = "cms";

  plotter.SetTitle(title);


  //            hname              xtitle                     ngroup precision units  setlogy moveoverflow xmin xmax ymin ymax
  // -------------------------------------------------------------------------------------------------------------------------
plotter.Draw( "darkpt"         , "(reconstructed) mediator p_{T}",      20, 0, "GeV",  scale, false, -3500, 2000, ymin, 1e7);
  //plotter.Draw( "dphijet1met"    , "#Delta#phi(jet1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijet2met"    , "#Delta#phi(jet2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijj"         , "#Delta#phi(jet1,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphijjmet"      , "#Delta#phi(jj,E_{T}^{miss})",        5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep1jet1"   , "#Delta#phi(lep1,jet1)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep1jet2"   , "#Delta#phi(lep1,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep2jet1"   , "#Delta#phi(lep2,jet1)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilep2jet2"   , "#Delta#phi(lep2,jet2)",              5, 2, "rad",  scale, false);
  //plotter.Draw( "dphill"         , "#Delta#phi(lep1,lep2)",              5, 2, "rad",  scale, false);
plotter.Draw( "dphillmet"      , "#Delta#phi(" +sll + "," + sm + ")",  5, 2, "rad",  scale, true, 0, 3.2, ymin, 1e7);
  //plotter.Draw( "dphilmet1"      , "#Delta#phi(lep1,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  //plotter.Draw( "dphilmet2"      , "#Delta#phi(lep2,E_{T}^{miss})",      5, 2, "rad",  scale, false);
  ///plotter.Draw( "ht"             , "H_{T}",                             20, 0, "GeV",  scale, true, 0, 1500, 1);
  //plotter.Draw( "htjets"         , "#sum_{jet} p_{T}",                  20, 0, "GeV",  scale, true, 0, 1500);
  //plotter.Draw( "htnojets"       , "p_{T}^{lep1} + p_{T}^{lep2} + MET", 20, 0, "GeV",  scale, true, 0, 1500);
  //plotter.Draw( "jet1eta"        , "leading jet #eta",                  4, 1, "NULL", scale, false);
  //plotter.Draw( "jet1mass"       , "leading jet mass",                  -1, 0, "GeV",  scale, true, 0,   50);
  //plotter.Draw( "jet1phi"        , "leading jet #phi",                   5, 2, "rad",  scale, false);
  //plotter.Draw( "jet1pt"         , "leading jet p_{T}",                  5, 0, "GeV",  scale, true, 0,  400, ymin);
  //plotter.Draw( "jet2eta"        , "trailing jet #eta",                 4, 1, "NULL", scale, false);
  //plotter.Draw( "jet2mass"       , "trailing jet mass",                 -1, 0, "GeV",  scale, true, 0,   50);
  //plotter.Draw( "jet2phi"        , "trailing jet #phi",                  5, 2, "rad",  scale, false);
  //plotter.Draw( "jet2pt"         , "trailing jet p_{T}",                 5, 0, "GeV",  scale, true, 0,  400, ymin);
  //plotter.Draw( "lep1eta"        , "leading lepton #eta",               -1, 1, "NULL", scale);
  //plotter.Draw( "lep1phi"        , "leading lepton #phi",                5, 2, "rad",  scale);
  //plotter.Draw( "lep1pt"         , "leading lepton p_{T}",               5, 0, "GeV",  scale, true, 0,  150, ymin, 1e6);
  //plotter.Draw( "lep2eta"        , "trailing lepton #eta",              -1, 1, "NULL", scale);
  //plotter.Draw( "lep2phi"        , "trailing lepton #phi",               5, 2, "rad",  scale);
  //plotter.Draw( "lep2pt"         , "trailing lepton p_{T}",              5, 0, "GeV",  scale, true, 0,  150, ymin, 1e6);
  //plotter.Draw( "nlepton"        , "number of leptons",                 -1, 0, "NULL", scale, true, 0,  10, ymin);
  //plotter.Draw( "m2l"            , "m_{" + sll + "}",                   10, 0, "GeV",  scale, true, 0,  300, ymin, 1e6);
plotter.Draw( "metPfType1"     , sm,                                  10, 0, "GeV",  scale, true, 0,  700, ymin, 1e7);
  //plotter.Draw( "mt2lblb"        , "M_{T2}(" + sl + "b" + sl + "b)",    10, 0, "GeV",  scale, false, 0, 600);
plotter.Draw( "mt2ll"          , "M_{T2}(" + sll + ")",               10, 0, "GeV",  scale, true, 0, 200, ymin, 1e7); 
  //plotter.Draw( "mtw1"           , "m_{T}^{W,1}",                       10, 0, "GeV",  scale, true, 0,  400);
  //plotter.Draw( "mtw2"           , "m_{T}^{W,2}",                       10, 0, "GeV",  scale, true, 0,  400);
  //plotter.Draw( "nbjet30csvv2m"  , "number of 30 GeV csvv2m b-jets",    -1, 0, "NULL", scale, true, 0,  6, ymin);
  //plotter.Draw( "njet"           , "number of 30 GeV jets",             -1, 0, "NULL", scale, true, 0,  10, ymin);
  ///plotter.Draw( "nvtx"          , "number of vertices",                -1, 0, "NULL", linY,  true, 0,   30, 1);
plotter.Draw( "ANN_170904_justMET50SF_" + signal, "ANN output", 4, 2, "NULL", scale, true, 0, 1.0, ymin, 1e7);


  //plotter.Draw( "metPfType1"     , sm,                                  10, 0, "GeV",  scale, true, 0,  200, 1);
  //plotter.Roc( "ANN_tanh_mt2ll100_ttDM0001scalar00500", "ANN output"    , 60, "GeV", -0.1, 1.1);
  //plotter.Roc( "ANN_sigm_mt2ll80_regina_ttDM0001scalar00010", "ANN output"    , 60, "GeV", -0.1, 1.1);
  //plotter.Draw( "scale"           , "- u_{||} / q_{T}",                -1, 0, "NULL", scale,  true, -3, 5, 1, 1e8 );
  //plotter.Draw( "uPara"           , "u_{||} + q_{T}",                  -1, 0, "GeV", scale,  true, -200, 200, 1, 1e8 );
  //plotter.Draw( "uPerp"           , "u_{#perp}",                       -1, 0, "GeV", scale,  true, -200, 200, 1, 1e8 );*/

  //plotter.Draw( "mt2ll"          , "M_{T2}(" + sll + ")",               10, 0, "GeV",  scale, false, 0, 200, ymin); 

  //           hname     xtitle                      npoints  units xmin xmax
  //  ---------------------------------------------------------------------------

  //plotter.Roc( "lep1pt", "leading lepton p_{T}",        1000, "GeV", 0, 1000);
  //plotter.Roc( "mt2ll", "M_{T2}(" + sll + ")",            1000, "GeV", 0, 500, "Punzi Eq.6");
  //plotter.Roc( "mt2ll", "M_{T2}(" + sll + ")",            1000, "GeV", 0, 500);

  //plotter.Roc( "dphillmet", "#Delta#phi(" +sll + "," + sm + ")",            1000, "GeV", 2, 5, "Punzi Eq.6");
  //plotter.Roc( "dphillmet", "#Delta#phi(" +sll + "," + sm + ")",            1000, "GeV", 2, 5);
  
  //plotter.Roc( "metPfType1", sm,            1000, "GeV", 0, 200, "Punzi Eq.6");
  //plotter.Roc( "metPfType1", sm,            1000, "GeV", 0, 200);

  //plotter.Roc( "ht", "H_{T}",            1000, "GeV", 0, 1500, "Punzi Eq.6");
  //plotter.Roc( "ht", "H_{T}",            1000, "GeV", 0, 1500);

  //plotter.Roc("topRecoW", "topRecoW", 1000, "NULL", 0, 0.01, "Punzi Eq.6");
  //plotter.Roc("topRecoW", "topRecoW", 1000, "NULL", 0, 0.01);

  //plotter.Roc( "darkpt"    , "(reconstructed) mediator p_{T}"       , 80, "GeV",0, 800);
  //plotter.Roc( "metPfType1", sm                   , 100, "GeV", 0, 200);
  //plotter.Roc( "mt2ll"     , "M_{T2}(" + sll + ")", 50, "GeV", 0, 150);
  //plotter.Roc( "topRecoW"  , "top reco weight"    , 100, "GeV", 0, .01);

  
  // Copy index.php in every directory
  //----------------------------------------------------------------------------
  gSystem->Exec("for dir in $(find ./ -type d); do cp -n ../index.php $dir/; done");
  //gSystem->Exec("rm -f index.php");

}


