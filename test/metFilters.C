#include "../include/Constants.h"


TH1D* met_Flag[nfilter];


void metFilters(TString sample = "Run2015D_16Dec2015_MET_0000__part0")
{
  // Read
  //----------------------------------------------------------------------------
  TFile* file = new TFile("../rootfiles/TTDM/" + sample + ".root");

  for (int i=0; i<nfilter; i++)
    met_Flag[i] = (TH1D*)file->Get(Form("met_Flag_%s", sfilter[i].Data()));


  // Print
  //----------------------------------------------------------------------------
  float total = met_Flag[noFilter]->GetEntries();

  printf("\n");
  printf(" MET filters efficiencies\n");
  printf("   sample: %s\n",   sample.Data());
  printf(" nentries: %.0f\n", total);
  printf("--------------------------\n");

  for (int i=0; i<nfilter; i++) {

    if (i == noFilter) continue;

    float efficiency = 1e2 * met_Flag[i]->GetEntries() / total;

    printf(" %6.2f %s  %-35s\n", efficiency, "%", sfilter[i].Data());
  }

  printf("\n");
}
