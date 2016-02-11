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


float _value   [nchannel][nvalue];
float _error   [nchannel][nvalue];
float _jetSyst [nchannel];
float _statSyst[nchannel];
float _fakeSyst[nchannel];


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

      _jetSyst[i] = 1e2 * fabs(_value[i][jetUp] - _value[i][jetDown]) / (2. * _value[i][nominal]);

      _statSyst[i] = 1e2 * fabs(_value[i][statUp] - _value[i][statDown]) / (2. * _value[i][nominal]);

      _fakeSyst[i] = sqrt(_jetSyst[i]*_jetSyst[i] + _statSyst[i]*_statSyst[i]);
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
  printf("\\hline\n");
  printf(" %13s", "jet \\pt syst."); for (int i=firstchannel; i<lastchannel; i++) printf(" & %16.0f\\%%", _jetSyst[i]);  printf(" \\\\\n");
  printf(" %13s", "stat. syst.");    for (int i=firstchannel; i<lastchannel; i++) printf(" & %16.0f\\%%", _statSyst[i]); printf(" \\\\\n");
  printf(" %13s", "total syst.");    for (int i=firstchannel; i<lastchannel; i++) printf(" & %16.0f\\%%", _fakeSyst[i]); printf(" \\\\\n");

  printf("\n");
}
