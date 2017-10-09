#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "TLegend.h"
#include "TString.h"


const bool errorband = false;


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


  // Draw the ratio
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1");

  ratio->SetMinimum(0.85);
  ratio->SetMaximum(1.20);

  ratio->Draw("ep");

  TString ytitle = Form("data / MC ratio / %.0f GeV", ratio->GetBinWidth(0));

  SetAxis(ratio, ratio->GetXaxis()->GetTitle(), ytitle, 1.7, 1.8);


  // Draw the old function
  //----------------------------------------------------------------------------
  TF1* fOld = new TF1("fOld", "[3]*(0.95 - [0]*TMath::Erf((x-[1])/[2]))", 0, 90);

  fOld->SetLineColor  (kGreen+2);
  fOld->SetMarkerColor(kGreen+2);

  // https://github.com/latinos/PlotsConfigurations/blob/master/Configurations/ggH/nuisances_iteos.py#L969-L977
  fOld->SetParameter(0,     0.1);
  fOld->SetParameter(1,    14.0);
  fOld->SetParameter(2,     8.8);
  fOld->SetParameter(3, 1.08683);

  fOld->Draw("same");


  // Draw the old function error band
  //------------------------------------------------------------------------------
  if (errorband)
    {
      TF1* fOld_down = (TF1*)fOld->Clone("fOld_down");
      TF1* fOld_up   = (TF1*)fOld->Clone("fOld_up");

      fOld_down->SetParameter(0, 13.6);
      fOld_down->SetParameter(1,  8.6);

      fOld_up->SetParameter(0, 14.4);
      fOld_up->SetParameter(1,  9.0);

      fOld_down->Draw("same");
      fOld_up  ->Draw("same");
    }


  // Update the fit parameters of the old function
  //
  //   1  p0           6.85257e-02   2.99341e-01   1.42129e-05   4.16760e-04
  //   2  p1           1.24518e+01   4.76906e+01   3.55312e-03   6.41625e-07
  //   3  p2           5.40627e+00   7.83907e+01   6.49546e-03  -1.96376e-06
  //   4  p3           1.05396e+00   3.48880e-01   1.58779e-05  -3.97115e-05
  //
  //----------------------------------------------------------------------------
  TF1* fNew = new TF1("fNew", "[3]*(0.95 - [0]*TMath::Erf((x-[1])/[2]))", 0, 90);

  fNew->SetLineColor  (kRed+1);
  fNew->SetMarkerColor(kRed+1);

  fNew->SetParameter(0, 0.1);
  fNew->SetParameter(1,  10);
  fNew->SetParameter(2,   1);
  fNew->SetParameter(3,   1);

  ratio->Fit(fNew, "mlr0");

  fNew->Draw("same");


  // Draw the updated error band
  //------------------------------------------------------------------------------
  if (errorband)
    {
      TF1* fNew_down = (TF1*)fNew->Clone("fNew_down");
      TF1* fNew_up   = (TF1*)fNew->Clone("fNew_up");

      fNew_down->SetParameter(0, 0.97 * fNew->GetParameter(0));
      fNew_down->SetParameter(1, 0.97 * fNew->GetParameter(1));

      fNew_up->SetParameter(0, 1.03 * fNew->GetParameter(0));
      fNew_up->SetParameter(1, 1.03 * fNew->GetParameter(1));

      fNew_down->Draw("same");
      fNew_up  ->Draw("same");
    }


  // Fit Lorenzo's function
  //
  //   1  p0           1.17864e-01   7.44869e-01   1.03478e-05   1.79528e-03
  //   2  p1           1.34231e+01   6.10789e+01   2.78074e-03   6.38697e-06
  //   3  p2           9.76801e+00   1.03947e+02   5.51661e-03  -2.28998e-06
  //   4  p3           1.01367e+00   6.56154e-01   1.29644e-05  -1.36428e-04
  //   5  p4           2.50141e-03   2.62347e-02   1.53669e-07  -1.11290e-01
  //   6  p5           1.10637e-05   1.42769e-04   1.32442e-09   1.17953e+01
  //
  //----------------------------------------------------------------------------
  TF1* fLo = new TF1("fLo", "([3] + [4]*x - [5]*x*x) * (0.95 - [0]*TMath::Erf((x-[1])/[2]))", 0, 150);

  fLo->SetLineColor  (kBlue);
  fLo->SetMarkerColor(kBlue);

  fLo->SetParameter(0,    0.131835);
  fLo->SetParameter(1,     14.1972);
  fLo->SetParameter(2,     10.1525);
  fLo->SetParameter(3,    0.876979);
  fLo->SetParameter(4, 4.11598e-03);
  fLo->SetParameter(5, 2.35520e-05);

  ratio->Fit(fLo, "mlr0");


  // Get the point where the first derivative is closest to zero
  //
  //   fLo = 0.9608 and d(fLo)/d(ptll) = -0.000110 for ptll = 119 GeV
  //
  //----------------------------------------------------------------------------
  float smallest_derivative_value = 999;
  float smallest_derivative_x     = 999;

  for (int x=90; x<120; x++)
    {
      if (fLo->Derivative(x) < smallest_derivative_value)
	{
	  smallest_derivative_value = fLo->Derivative(x);
	  smallest_derivative_x     = x;
	}
    }

  printf("\n fLo = %.4f and d(fLo)/d(ptll) = %f for ptll = %.0f GeV\n\n",
	 fLo->Eval(smallest_derivative_x),
	 fLo->Derivative(smallest_derivative_x),
	 smallest_derivative_x);


  // Draw Lorenzo's function in two ranges
  //----------------------------------------------------------------------------
  fLo->SetRange(0, smallest_derivative_x);

  fLo->Draw("same");

  TF1* fHi = new TF1("fHi", "[0]", smallest_derivative_x, 150);

  fHi->SetLineColor  (kBlue);
  fHi->SetMarkerColor(kBlue);

  fHi->SetParameter(0, fLo->Eval(smallest_derivative_x));

  fHi->Draw("same");


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
