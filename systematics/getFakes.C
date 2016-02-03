#include"../include/Constants.h"

void PrintFakes(TString analysis,
                TString schannel,
                TString scut);

void getFakes(TString cut = "NONE"){

  if (cut.EqualTo("NONE")) {
    printf("\n");
    for (int i=0; i<ncut; i++)
      printf(" root -l -b -q \"getFakes.C(\\\"%s\\\")\"\n", scut[i].Data());
    printf("\n");

    return;
  }

  TString analysis="WZ";
  
  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  for (int j=firstchannel; j<=lastchannel; j++){
    PrintFakes(analysis, cut, schannel[j]);
  }
  
}



void PrintFakes(TString analysis, 
	        TString scut,
		TString schannel)
{


  float nominal, nominalError;
  float up, upError;
  float down, downError;
  float statUp, statUpError;
  float statDown, statDownError;

  float delta;
  float systematic;

  TFile*  f             =   new TFile  ("../rootfiles/WZ/00_Fakes.root","read");
  TH1D*   h_fakes       =   (TH1D*)f   ->  Get(Form("%s/h_fakes_%s", scut.Data(), schannel.Data()));
  TH1D*   h_fakesError  =   (TH1D*)f   ->  Get(Form("%s/h_fakesError_%s", scut.Data(), schannel.Data()));


 printf("CANAL : %s \n", schannel.Data());
 printf("CORTE : %s \n", scut.Data());
 printf("--------------------- \n");

 nominal    =   h_fakes -> GetBinContent(1);
 up         =   h_fakes -> GetBinContent(2);
 down       =   h_fakes -> GetBinContent(3);
 statUp     =   h_fakes -> GetBinContent(4);
 statDown   =   h_fakes -> GetBinContent(5);

 nominalError    = sqrt(h_fakesError -> GetBinContent(1));
 upError         = sqrt(h_fakesError -> GetBinContent(2));
 downError       = sqrt(h_fakesError -> GetBinContent(3));
 statUpError     = sqrt(h_fakesError -> GetBinContent(4));
 statDownError   = sqrt(h_fakesError -> GetBinContent(5));
 
 printf("Value of nominal :        (%.3f +/- %.3f) \n", nominal, nominalError);
 printf("Value of Up :             (%.3f +/- %.3f) \n", up, upError);
 printf("Value of Down :           (%.3f +/- %.3f) \n", down, downError);
 printf("Value of Stat Up :        (%.3f +/- %.3f) \n", statUp, statUpError);
 printf("Value of Stat Down :      (%.3f +/- %.3f) \n", statDown, statDownError);

 delta = sqrt(pow((up-down),2) + pow((statUp-statDown),2));   // This error might be too large
 systematic = 100*(delta)/nominal;

 printf("The systematic error associated with the fakes for this canal %s is... %.3f %% \n \n", schannel.Data(), systematic); 
}
