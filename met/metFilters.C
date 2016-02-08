#include "../include/Constants.h"


const Bool_t  _print   = true;
const Bool_t  _savepdf = false;
const Bool_t  _savepng = true;
const Float_t _xoffset = 0.184;
const Float_t _yoffset = 0.043;


void     DrawLatex (Font_t      tfont,
		    Float_t     x,
		    Float_t     y,
		    Float_t     tsize,
		    Short_t     align,
		    const char* text,
		    Bool_t      setndc = true);

TLegend* DrawLegend(Float_t     x1,
		    Float_t     y1,
		    TH1*        hist,
		    TString     label,
		    TString     option  = "p",
		    Float_t     tsize   = 0.025,
		    Float_t     xoffset = _xoffset,
		    Float_t     yoffset = _yoffset);


Color_t cfilter[nfilter];

TH1D* met_Flag[nfilter];


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// metFilters
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void metFilters(TString sample = "NONE",
		TString cut    = "met300")
{
  if (!sample.Contains("Run2015D"))
    {
      printf("\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_DoubleMuon\\\",     \\\"nocut\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_DoubleEG\\\",       \\\"nocut\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_MuonEG\\\",         \\\"nocut\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_SingleElectron\\\", \\\"nocut\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_SinglePhoton\\\",   \\\"nocut\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_MET\\\",            \\\"nocut\\\")\"\n");
      printf("\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_DoubleMuon\\\",     \\\"met300\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_DoubleEG\\\",       \\\"met300\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_MuonEG\\\",         \\\"met300\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_SingleElectron\\\", \\\"met300\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_SinglePhoton\\\",   \\\"met300\\\")\"\n");
      printf(" root -l -b -q \"metFilters.C(\\\"Run2015D_16Dec2015_MET\\\",            \\\"met300\\\")\"\n");
      printf("\n");

      return;
    }

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  cfilter[HBHENoiseFilter]                    = kAzure;
  cfilter[HBHENoiseIsoFilter]                 = kAzure-2;
  cfilter[CSCTightHalo2015Filter]             = kRed-2;
  cfilter[EcalDeadCellTriggerPrimitiveFilter] = kRed-1;
  cfilter[goodVertices]                       = kGreen+1;
  cfilter[eeBadScFilter]                      = kGreen+2;
  cfilter[chargedHadronTrackResolutionFilter] = kRed;
  cfilter[muonBadTrackFilter]                 = kRed+1;
  cfilter[noFilter]                           = kGray;
  cfilter[allFilter]                          = kBlack;


  // Read
  //----------------------------------------------------------------------------
  TFile* file = new TFile("../rootfiles/TTDM/" + sample + ".root");

  for (int i=0; i<nfilter; i++) {

    met_Flag[i] = (TH1D*)file->Get(Form("met_Flag_%s_%s", cut.Data(), sfilter[i].Data()));

    met_Flag[i]->Rebin(50);
  }

  float total = met_Flag[noFilter]->GetEntries();


  // Print
  //----------------------------------------------------------------------------
  if (_print)
    {
      printf("\n");
      printf(" MET filters efficiencies\n");
      printf("   sample: %s\n",   sample.Data());
      printf("      cut: %s\n",   cut.Data());
      printf(" nentries: %.0f\n", total);
      printf("--------------------------\n");

      for (int i=0; i<nfilter; i++) {

	if (i == noFilter) continue;

	float efficiency = 1e2 * met_Flag[i]->GetEntries() / total;

	printf(" %7.3f%s  %-35s\n", efficiency, "%", sfilter[i].Data());
      }

      printf("\n");
    }


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("met-filters", "met-filters", 600, 600);

  c1->SetLogy();

  Float_t x0     = 0.410;
  Float_t y0     = 0.845;
  Float_t xdelta = 0.000;
  Int_t   ny     = 0;

  met_Flag[noFilter]->Draw("hist");

  DrawLegend(x0 + xdelta, y0, met_Flag[noFilter], Form("%s (%.0f events)", sfilter[noFilter].Data(), total), "f");
  ny++;

  float binwidth = met_Flag[noFilter]->GetBinWidth(0);
  float themax   = met_Flag[noFilter]->GetMaximum();

  themax = TMath::Power(10, TMath::Log10(themax) + 1);

  met_Flag[noFilter]->SetMaximum(themax);
  met_Flag[noFilter]->SetLineColor(cfilter[noFilter]);
  met_Flag[noFilter]->SetFillColor(cfilter[noFilter]);
  met_Flag[noFilter]->SetFillStyle(1001);
  met_Flag[noFilter]->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  met_Flag[noFilter]->GetXaxis()->SetTitleOffset(1.5);
  met_Flag[noFilter]->GetYaxis()->SetTitle(Form("events / %.0f GeV", binwidth));
  met_Flag[noFilter]->GetYaxis()->SetTitleOffset(1.7);

  DrawLatex(42, 0.940, 0.945, 0.045, 31, sample + " dataset");


  // Loop
  //----------------------------------------------------------------------------
  for (int i=0; i<nfilter; i++) {

    TString lstyle = (i == allFilter) ? "lp" : "l";

    if (i == noFilter) continue;

    met_Flag[i]->SetLineColor(cfilter[i]);
    met_Flag[i]->SetLineWidth(2);

    if (i == allFilter)
      {
	met_Flag[i]->SetMarkerColor(cfilter[i]);
	met_Flag[i]->SetMarkerStyle(kFullCircle);
	met_Flag[i]->Draw("ep,same");
      }
    else
      {
	met_Flag[i]->Draw("hist,same");
      }

    float efficiency = 1e2 * met_Flag[i]->GetEntries() / total;

    if (met_Flag[i]->GetEntries() == total)
      DrawLegend(x0 + xdelta, y0 - ny*_yoffset, met_Flag[i], Form("(%.0f%s) %s", efficiency, "%", sfilter[i].Data()), lstyle);
    else
      DrawLegend(x0 + xdelta, y0 - ny*_yoffset, met_Flag[i], Form("(%.2f%s) %s", efficiency, "%", sfilter[i].Data()), lstyle);
    ny++;
  }


  // Save the money
  //----------------------------------------------------------------------------
  c1->GetFrame()->DrawClone();

  if (_savepdf || _savepng) gSystem->mkdir("figures", kTRUE);

  if (_savepdf) c1->SaveAs("figures/" + sample + "_" + cut + ".pdf");
  if (_savepng) c1->SaveAs("figures/" + sample + "_" + cut + ".png");
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void DrawLatex(Font_t      tfont,
	       Float_t     x,
	       Float_t     y,
	       Float_t     tsize,
	       Short_t     align,
	       const char* text,
	       Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
TLegend* DrawLegend(Float_t x1,
		    Float_t y1,
		    TH1*    hist,
		    TString label,
		    TString option,
		    Float_t tsize,
		    Float_t xoffset,
		    Float_t yoffset)
{
  TLegend* legend = new TLegend(x1,
				y1,
				x1 + xoffset,
				y1 + yoffset);
  
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (tsize);

  TString final_label = Form(" %s", label.Data());

  legend->AddEntry(hist, final_label.Data(), option.Data());
  legend->Draw();

  return legend;
}
