//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l ratioExample.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ratioExample()
{
  printf("\n This is a ratio plot example with asymmetric errors, made by Lorenzo Moneta on June 14th, 2016\n\n");

  auto h1 = new TH1D("h1", "h1", 50, 0, 10);
  auto h2 = new TH1D("h2", "h2", 50, 0, 10);

  auto f1 = new TF1("f1", "exp(-x/[0])");

  f1->SetParameter(0, 3);

  h1->FillRandom("f1",  500);
  h2->FillRandom("f1", 2000);

  h2->Sumw2();

  h2->Scale(500./2000.);  // Scale second histogram to same entries as first one

  auto c1 = new TCanvas();

  c1->Divide(1, 2);

  c1->cd(1);

  h1->Draw("e");

  h2->SetLineColor(kBlue); 

  h2->Draw("hist,same");


  // Compute division using C-P errors
  //----------------------------------------------------------------------------
  auto g1 = new TGraphAsymmErrors();

  g1->SetLineColor(kBlue);
  g1->SetLineWidth(2);
  g1->SetMarkerColor(kBlue);
  g1->SetMarkerStyle(20);

  g1->Divide(h1, h2, "pois");


  // Compute division using mid-P errors
  //----------------------------------------------------------------------------
  auto g2 = new TGraphAsymmErrors();

  g2->SetLineColor(kRed);
  g2->SetLineWidth(2);
  g2->SetMarkerColor(kRed);
  g2->SetMarkerStyle(20);

  g2->Divide(h1, h2, "pois midp");


  // Draw ratios
  //----------------------------------------------------------------------------
  c1->cd(2);

  g1->GetXaxis()->SetRangeUser(0,10);

  g1->Draw("ap");
  g2->Draw("p,same");

  g1->SetMaximum(5.);
}
