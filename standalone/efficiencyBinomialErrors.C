//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l efficiencyBinomialErrors.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const int nbins = 7;

float n_num[] = {  9847, 12193,  12774,  9940,  7060,  5540,   4080};
float n_den[] = {100000, 99999, 100000, 97999, 99400, 98800, 100000};


void efficiencyBinomialErrors()
{
  printf("\n Get the binomial errors for efficiencies\n\n");

  auto h_num = new TH1D("h_num", "h_num", nbins, 0, nbins);
  auto h_den = new TH1D("h_den", "h_den", nbins, 0, nbins);

  for (int i=0; i<nbins; i++)
    {
      h_num->SetBinContent(i+1, n_num[i]);
      h_den->SetBinContent(i+1, n_den[i]);
    }

  auto graph = new TGraphAsymmErrors();

  graph->Divide(h_num, h_den, "cl=0.683 b(1,1) mode");

  auto canvas = new TCanvas("canvas", "canvas");

  graph->SetMarkerStyle(kFullCircle);

  graph->Draw("apz");

  graph->Print();
}
