#include"../include/Constants.h"


void PrintFakes(TString schannel, TString scut);


TFile* _inputfile;


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

  _inputfile = new TFile("../rootfiles/" + analysis + "/00_Fakes.root", "read");

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  for (int j=firstchannel; j<=lastchannel; j++)
    PrintFakes(level, schannel[j]);

  printf("\n");
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PrintFakes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PrintFakes(TString scut, TString schannel)
{
  TH1D* h_fakes = (TH1D*)_inputfile->Get(Form("%s/h_fakes_%s", scut.Data(), schannel.Data()));

  float nominal  = h_fakes->GetBinContent(1);
  float up       = h_fakes->GetBinContent(2);
  float down     = h_fakes->GetBinContent(3);
  float statUp   = h_fakes->GetBinContent(4);
  float statDown = h_fakes->GetBinContent(5);

  float nominalError  = sqrt(h_fakes->GetSumw2()->At(1));
  float upError       = sqrt(h_fakes->GetSumw2()->At(2));
  float downError     = sqrt(h_fakes->GetSumw2()->At(3));
  float statUpError   = sqrt(h_fakes->GetSumw2()->At(4));
  float statDownError = sqrt(h_fakes->GetSumw2()->At(5));

  // This error estimation might be too large
  float delta      = sqrt(pow((up-down),2) + pow((statUp-statDown),2));
  float systematic = 1e2 * delta / nominal;

  printf("\n %s (%s)\n", scut.Data(), schannel.Data());
  printf("-----------------------------\n");
  printf("    nominal yield: %.2f +- %.2f\n", nominal,  nominalError);
  printf("      jet pt   up: %.2f +- %.2f\n", up,       upError);
  printf("      jet pt down: %.2f +- %.2f\n", down,     downError);
  printf("        stat   up: %.2f +- %.2f\n", statUp,   statUpError);
  printf("        stat down: %.2f +- %.2f\n", statDown, statDownError);
  printf(" systematic error: %.2f%%\n", systematic); 
}
