#include"../include/Constants.h"
#include"../include/CutsWZ.h"

void PrintFakes(TString analysis,
                TString schannel,
                TString scut);

void getFakes(){

  TString analysis="WZ";

  int firstchannel = (analysis.EqualTo("WZ")) ? eee : ee;
  int lastchannel  = (analysis.EqualTo("WZ")) ? lll : ll;

  //  if(analysis.EqualTo("WZ")){
  // int firstcut = ncut[0];
  // int lastcut  = ncut[4];
  //    }

//  for (int i=firstcut; i<=lastcut; i++){
    for (int j=firstchannel; j<=lastchannel; j++){
      PrintFakes(analysis, "WZ/02_HasW", schannel[j]);
    }
//}
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

 printf("========================================== \n");
 printf("CANAL : %s \n", schannel.Data());

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

 printf("Valor nominal :   (%f +/- %f) \n", nominal, nominalError);
 printf("Valor Up :        (%f +/- %f) \n", up, upError);
 printf("Valor Down :      (%f +/- %f) \n", down, downError);
 printf("Valor Stat Up :   (%f +/- %f) \n", statUp, statUpError);
 printf("Valor Stat Down : (%f +/- %f) \n", statDown, statDownError);

 delta = sqrt( pow((up-down),2) + pow((statUp-statDown),2));
 systematic = 100*(delta)/nominal;

 printf("The systematic error asociated with the fakes for this canal %s is... %f % \n ", schannel.Data(), systematic); 

}
