#include"../include/Constants.h"


enum {
  nominalYield,
  elUp,
  elDown,
  elStatUp,
  elStatDown,
  muUp,
  muDown,
  muStatUp,
  muStatDown,
  nvalue
};

const TString svalue[nvalue] = {
  "nominal yield",
  "electron jet \\pt up",
  "electron jet \\pt down",
  "electron stat. up",
  "electron stat. down",
  "muon jet \\pt up",
  "muon jet \\pt down",
  "muon stat. up",
  "muon stat. down"
};


float _value     [nchannel][nvalue];
float _error     [nchannel][nvalue];
float _elSyst    [nchannel];
float _muSyst    [nchannel];
float _elStatSyst[nchannel];
float _muStatSyst[nchannel];


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getFakes
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakes(TString level = "NONE")
{
  if (level.EqualTo("NONE")) {
    
    printf("\n");

    for (int i=0; i<ncut; i++) {
            printf(" root -l -b -q \"getFakes.C(\\\"%s\\\")\"\n", scut[i].Data());
    }
    
    printf("\n");
    
    return;
 }

  TString tok;

  Ssiz_t from = 0;

  TString analysis= (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  TFile* inputfile = new TFile("../rootfiles/nominal/" + analysis + "/00_Fakes.root", "read");

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  // Get yields
  //----------------------------------------------------------------------------
  for (int i=firstchannel; i<lastchannel; i++)
    {
      TH1D* h_fakes = (TH1D*)inputfile->Get(Form("%s/h_fakes_%s", level.Data(), schannel[i].Data()));

      for (int j=0; j<nvalue; j++)
	{
	  _value[i][j] = h_fakes->GetBinContent(j+1);
	  _error[i][j] = sqrt(h_fakes->GetSumw2()->At(j+1));
	}

      _elSyst[i] = 1e2 * fabs(_value[i][elUp] - _value[i][elDown]) / (2. * _value[i][nominalYield]);
      _muSyst[i] = 1e2 * fabs(_value[i][muUp] - _value[i][muDown]) / (2. * _value[i][nominalYield]);

      _elStatSyst[i] = 1e2 * fabs(_value[i][elStatUp] - _value[i][elStatDown]) / (2. * _value[i][nominalYield]);
      _muStatSyst[i] = 1e2 * fabs(_value[i][muStatUp] - _value[i][muStatDown]) / (2. * _value[i][nominalYield]);
      }


  // Print yields
  //----------------------------------------------------------------------------
  printf("\n %22s", " ");

  for (int i=firstchannel; i<lastchannel; i++) printf(" & %19s", schannel[i].Data());

  printf(" \\\\\n");

  for (int j=0; j<nvalue; j++)
    {
      printf(" %22s", svalue[j].Data());

      for (int i=firstchannel; i<lastchannel; i++)
        {
          printf(" & %7.2f $\\pm$ %5.2f", _value[i][j], _error[i][j]);
        }

      printf(" \\\\\n");
    }

  // Print systematic uncertainties
  //----------------------------------------------------------------------------
  printf("\\hline\n");
  printf(" %22s", " ");                       for (int i=firstchannel; i<lastchannel; i++) printf(" & %19s", schannel[i].Data());   printf(" \\\\\n");
  printf(" %22s", "electron jet \\pt syst."); for (int i=firstchannel; i<lastchannel; i++) printf(" & %17.0f\\%%", _elSyst[i]);     printf(" \\\\\n");
  printf(" %22s", "electron stat. syst.");    for (int i=firstchannel; i<lastchannel; i++) printf(" & %17.0f\\%%", _elStatSyst[i]); printf(" \\\\\n");
  printf(" %22s", "muon jet \\pt syst.");     for (int i=firstchannel; i<lastchannel; i++) printf(" & %17.0f\\%%", _muSyst[i]);     printf(" \\\\\n");
  printf(" %22s", "muon stat. syst.");        for (int i=firstchannel; i<lastchannel; i++) printf(" & %17.0f\\%%", _muStatSyst[i]); printf(" \\\\\n");

  printf("\n");

}
