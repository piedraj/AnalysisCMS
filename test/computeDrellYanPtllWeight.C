#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "TLegend.h"
#include "TString.h"


// Member functions
//------------------------------------------------------------------------------
void     SetAxis   (TH1*     hist,
		    TString  xtitle,
		    TString  ytitle,
		    Float_t  xoffset,
		    Float_t  yoffset);

TLegend* DrawLegend(Float_t  x1,
		    Float_t  y1,
		    TObject* hist,
		    TString  label,
		    TString  option  = "lp",
		    Float_t  tsize   = 0.030,
		    Float_t  xoffset = 0.200,
		    Float_t  yoffset = 0.050);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l       computeDrellYanPtllWeight.C+
// root -l -b -q computeDrellYanPtllWeight.C+
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void computeDrellYanPtllWeight(TString fname = "h_pt2l_mm")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  TFile* file = TFile::Open("figures/Control/01_DY/" + fname + ".root");

  TH1D* ratio = (TH1D*)file->Get("ratio");


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1");

  ratio->SetMinimum(0.85);
  ratio->SetMaximum(1.20);

  ratio->Draw("ep");

  TString ytitle = Form("data / MC ratio / %.0f GeV", ratio->GetBinWidth(0));

  SetAxis(ratio, ratio->GetXaxis()->GetTitle(), ytitle, 1.7, 1.8);


  // Lorenzo's function
  //----------------------------------------------------------------------------
  TF1* fLo = new TF1("fLo", "(0.876979 + 4.11598e-03*x - 2.35520e-05*x*x) * (1.10211 * (0.958512 - 0.131835*TMath::Erf((x-14.1972)/10.1525)))", 0, 50);

  fLo->SetLineColor  (kGreen+1);
  fLo->SetMarkerColor(kGreen+1);

  fLo->Draw("same");

  //  TF1* fHi = new TF1("fHi", "0.903433", 140, 200);
  //
  //  fHi->SetLineColor  (kGreen+1);
  //  fHi->SetMarkerColor(kGreen+1);
  //
  //  fHi->Draw("same");


  // Old function
  //----------------------------------------------------------------------------
  TF1* fOld = new TF1("fOld", "[2]*(0.95-[3]*TMath::Erf((x-[0])/[1]))", 0, 50);

  fOld->SetLineColor  (kBlue);
  fOld->SetMarkerColor(kBlue);

  // https://github.com/latinos/PlotsConfigurations/blob/master/Configurations/ggH/nuisances_iteos.py#L969-L977
  fOld->SetParameter(0,    14.0);
  fOld->SetParameter(1,     8.8);
  fOld->SetParameter(2, 1.08683);
  fOld->SetParameter(3,     0.1);

  fOld->Draw("same");


  // Error band
  //------------------------------------------------------------------------------
  TF1* fOld_down = (TF1*)fOld->Clone("fOld_down");
  TF1* fOld_up   = (TF1*)fOld->Clone("fOld_up");

  fOld_down->SetParameter(0, 13.6);
  fOld_down->SetParameter(1,  8.6);

  fOld_up->SetParameter(0, 14.4);
  fOld_up->SetParameter(1,  9.0);

  fOld_down->Draw("same");
  fOld_up  ->Draw("same");


  // New function
  //
  //   1  p0           1.35313e+01   4.66851e-02   5.69026e-04  -3.19912e-03
  //   2  p1           7.14458e+00   9.07914e-02   1.12944e-03   1.18232e-03
  //   3  p2           1.04249e+00   3.67360e-04   4.30444e-06   2.31867e-03
  //   4  p3           8.19525e-02   3.84461e-04   4.88893e-06   7.81338e-02
  //
  //----------------------------------------------------------------------------
  TF1* fNew = new TF1("fNew", "[2]*(0.95-[3]*TMath::Erf((x-[0])/[1]))", 0, 50);

  fNew->SetLineColor  (kRed+1);
  fNew->SetMarkerColor(kRed+1);

  fNew->SetParameter(0,  10);
  fNew->SetParameter(1,   1);
  fNew->SetParameter(2,   1);
  fNew->SetParameter(3, 0.1);

  ratio->Fit(fNew, "r");


  // Error band
  //------------------------------------------------------------------------------
  TF1* fNew_down = (TF1*)fNew->Clone("fNew_down");
  TF1* fNew_up   = (TF1*)fNew->Clone("fNew_up");

  fNew_down->SetParameter(0, 0.97 * fNew->GetParameter(0));
  fNew_down->SetParameter(1, 0.97 * fNew->GetParameter(1));

  fNew_up->SetParameter(0, 1.03 * fNew->GetParameter(0));
  fNew_up->SetParameter(1, 1.03 * fNew->GetParameter(1));

  fNew_down->Draw("same");
  fNew_up  ->Draw("same");


  // Legend
  //----------------------------------------------------------------------------
  DrawLegend(0.69, 0.83, (TObject*)fOld, " old fit");
  DrawLegend(0.69, 0.77, (TObject*)fNew, " new fit");
  DrawLegend(0.69, 0.71, (TObject*)fLo,  " Lorenzo's fit");


  // Save
  //----------------------------------------------------------------------------
  ratio->Draw("ep,same");

  c1->SaveAs(fname + "_ratio_fit.png");
}


//------------------------------------------------------------------------------
// SetAxis
//------------------------------------------------------------------------------
void SetAxis(TH1*    hist,
	     TString xtitle,
	     TString ytitle,
	     Float_t xoffset,
	     Float_t yoffset)
{
  gPad->cd();
  gPad->Update();

  // See https://root.cern.ch/doc/master/classTAttText.html#T4
  Float_t padw = gPad->XtoPixel(gPad->GetX2());
  Float_t padh = gPad->YtoPixel(gPad->GetY1());

  Float_t size = (padw < padh) ? padw : padh;

  size = 20. / size;  // Like this label size is always 20 pixels
  
  TAxis* xaxis = (TAxis*)hist->GetXaxis();
  TAxis* yaxis = (TAxis*)hist->GetYaxis();

  xaxis->SetTitleOffset(xoffset);
  yaxis->SetTitleOffset(yoffset);

  xaxis->SetLabelSize(size);
  yaxis->SetLabelSize(size);

  xaxis->SetTitleSize(size);
  yaxis->SetTitleSize(size);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(ytitle);

  yaxis->CenterTitle();

  gPad->GetFrame()->DrawClone();
  gPad->RedrawAxis();
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
TLegend* DrawLegend(Float_t  x1,
		    Float_t  y1,
		    TObject* hist,
		    TString  label,
		    TString  option,
		    Float_t  tsize,
		    Float_t  xoffset,
		    Float_t  yoffset)
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

  legend->AddEntry(hist, label.Data(), option.Data());
  legend->Draw();

  return legend;
}
