//------------------------------------------------------------------------------
//
// Simple and generic ROOT macro to compare two histograms
//
//------------------------------------------------------------------------------
void compareHistograms(TString histname1 = "Control/04_Top/h_dphill_ll",
		       TString histname2 = "Control/04_Top/h_dphill_ll",
		       TString filename1 = "/afs/cern.ch/user/p/piedra/work/CMSSW_projects/CMSSW_8_0_5/src/AnalysisCMS/rootfiles/nominal/Control/05_ST.root",
		       TString filename2 = "/afs/cern.ch/user/p/piedra/work/CMSSW_projects/CMSSW_8_0_5/src/AnalysisCMS/rootfiles/nominal/Control/04_TTTo2L2Nu.root",
		       TString xtitle    = "#Delta#phi_{#font[12]{ll}}",
		       Bool_t  scale     = true)
{
  gInterpreter->ExecuteMacro("PaperStyle.C");


  // Talk to me
  //----------------------------------------------------------------------------
  printf("\n");
  printf("  hist1: %s (histogram)\n", histname1.Data());
  printf("  hist2: %s (points)\n",    histname2.Data());
  printf("  file1: %s\n",             filename1.Data());
  printf("  file2: %s\n",             filename2.Data());
  printf(" xtitle: %s\n",             xtitle.Data());
  printf("  scale: %s\n", (scale) ? "yes" : "no");
  printf("\n");


  // Do the input files exist?
  //----------------------------------------------------------------------------
  if (gSystem->AccessPathName(filename1))
    {
      printf(" [compareHistograms.C] Cannot access %s\n\n", filename1.Data());
      return;
    }

  if (gSystem->AccessPathName(filename2))
    {
      printf(" [compareHistograms.C] Cannot access %s\n\n", filename2.Data());
      return;
    }


  // All right, get them
  //----------------------------------------------------------------------------
  TFile* file1 = TFile::Open(filename1, "read");
  TFile* file2 = TFile::Open(filename2, "read");


  // Get the histograms and verify that they exist
  //----------------------------------------------------------------------------
  TH1D* hist1 = (TH1D*)file1->Get(histname1);
  TH1D* hist2 = (TH1D*)file2->Get(histname2);

  if (!hist1)
    {
      printf(" [compareHistograms.C] Cannot access %s\n\n", histname1.Data());
      return;
    }

  if (!hist2)
    {
      printf(" [compareHistograms.C] Cannot access %s\n\n", histname2.Data());
      return;
    }


  // Scale histograms
  //----------------------------------------------------------------------------
  if (scale)
    {
      hist1->Scale(1. / hist1->Integral());
      hist2->Scale(1. / hist2->Integral());
    }


  // Cosmetics
  //----------------------------------------------------------------------------
  hist1->SetFillColor(kAzure-9);
  hist1->SetFillStyle(1001);
  hist1->SetLineColor(kAzure-9);
  hist1->SetLineWidth(0);
  
  hist2->SetLineWidth(2);
  hist2->SetMarkerStyle(kFullCircle);


  // Draw time
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1");

  TH1D* hfirst = (TH1D*)hist1->Clone("hfirst");

  hfirst->Reset();
  hfirst->SetTitle("");
  hfirst->Draw();

  hist1->Draw("hist,same");
  hist2->Draw("ep,same");


  // Vertical size
  //----------------------------------------------------------------------------
  Float_t ymin = (hist1->GetMinimum() < hist2->GetMinimum()) ? hist1->GetMinimum() : hist2->GetMinimum();
  Float_t ymax = (hist1->GetMaximum() > hist2->GetMaximum()) ? hist1->GetMaximum() : hist2->GetMaximum();

  ymax *= 1.1;

  hfirst->SetMinimum(ymin);
  hfirst->SetMaximum(ymax);

  
  // Axis and labels
  //----------------------------------------------------------------------------
  hfirst->SetXTitle(xtitle);
  hfirst->SetYTitle("entries / bin");

  hfirst->GetXaxis()->SetTitleOffset(1.5);
  hfirst->GetYaxis()->SetTitleOffset(2.2);

  hfirst->GetYaxis()->CenterTitle();


  // Round up
  //----------------------------------------------------------------------------
  c1->RedrawAxis();
  c1->GetFrame()->DrawClone();
}
