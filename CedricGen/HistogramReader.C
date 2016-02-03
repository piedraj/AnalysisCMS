#include "HistogramReader.h"


void HistogramReader::Draw(TString hname)
{

  TCanvas* canvas = new TCanvas(hname, hname);
  h_GEN_m2l->Draw();
} 
