//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Get the binomial errors for efficiencies
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const int nbins = 1;


void efficiency()
{
  TH1D* h_num = new TH1D("h_num", "h_num", nbins, 0, nbins);
  TH1D* h_den = new TH1D("h_den", "h_den", nbins, 0, nbins);

  h_num->SetBinContent(1,  14000.);
  h_den->SetBinContent(1, 100000.);

  TGraphAsymmErrors* graph = new TGraphAsymmErrors();

  graph->Divide(h_num, h_den, "cl=0.683 b(1,1) mode");

  TCanvas* canvas = new TCanvas("canvas", "canvas");

  graph->SetMarkerStyle(kFullCircle);

  graph->Draw("apz");

  graph->Print();
}
