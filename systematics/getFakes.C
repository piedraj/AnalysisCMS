#include"../include/Constants.h"


enum {
  nominal,
  jetUp,
  jetDown,
  statUp,
  statDown,
  nvalue
};

const TString svalue[nvalue] = {
  "nominal yield",
  "jet \\pt up",
  "jet \\pt down",
  "stat. up",
  "stat. down"
};


float _value[nchannel][nvalue];
float _error[nchannel][nvalue];
float _syst [nchannel];


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getFakes
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakes(TString level = "NONE")
{
  if (level.EqualTo("NONE")) {

    printf("\n");

    for (int i=0; i<ncut; i++)
      printf(" root -l -b -q \"getFakes.C(\\\"%s\\\")\"\n", scut[i].Data());
    
    printf("\n");

    return;
  }

  TString tok;

  Ssiz_t from = 0;

  TString analysis= (level.Tokenize(tok, from, "/")) ? tok : "NONE";

  if (analysis.EqualTo("NONE")) return;

  TFile* inputfile = new TFile("../rootfiles/" + analysis + "/00_Fakes.root", "read");

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

      float delta = 0;

      delta += pow((_value[i][jetUp]  - _value[i][jetDown]),  2);
      delta += pow((_value[i][statUp] - _value[i][statDown]), 2);
      
      _syst[i] = 1e2 * sqrt(delta) / _value[i][nominal];
    }


  // Print yields
  //----------------------------------------------------------------------------
  printf("\n");
  
  for (int j=0; j<nvalue; j++)
    {
      printf(" %13s", svalue[j].Data());
      
      for (int i=firstchannel; i<lastchannel; i++)
	{
	  printf(" & %6.2f $\\pm$ %5.2f", _value[i][j], _error[i][j]);
	}

      printf(" \\\\\n");
    }


  // Print systematic uncertainties
  //----------------------------------------------------------------------------
  printf(" %13s", "systematic");

  for (int i=firstchannel; i<lastchannel; i++)
    {
      printf(" & %16.0f\\%%", _syst[i]);
    }

  printf(" \\\\\n");

  printf("\n");
}
