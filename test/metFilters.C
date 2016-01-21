#include "../include/Constants.h"


TH1D* met_Flag[nfilter];


void metFilters()
{
  // Read
  //----------------------------------------------------------------------------
  TFile* file = new TFile("../rootfiles/TTDM/stepB_data_numEvent200.root");

  for (int i=0; i<nfilter; i++)
    met_Flag[i] = (TH1D*)file->Get(Form("met_Flag_%s", sfilter[i].Data()));


  // Print
  //----------------------------------------------------------------------------
  float total = met_Flag[noFilter]->GetEntries();

  printf("\n");
  printf(" MET filters efficiencies\n");
  printf("--------------------------\n");

  for (int i=0; i<nfilter; i++) {

    if (i == noFilter) continue;

    float efficiency = 1e2 * met_Flag[i]->GetEntries() / total;

    printf(" %6.2f %s  %-35s\n", efficiency, "%", sfilter[i].Data());
  }

  printf("\n");
}
