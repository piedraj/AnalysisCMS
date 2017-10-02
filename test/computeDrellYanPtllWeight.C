// Member functions
//------------------------------------------------------------------------------
void SetAxis(TH1*    hist,
	     TString xtitle,
	     TString ytitle,
	     Float_t xoffset,
	     Float_t yoffset);

TLegend* DrawLegend(Float_t       x1,
		    Float_t       y1,
		    TObject*      hist,
		    TString       label,
		    TString       option  = "lp",
		    Float_t       tsize   = 0.030,
		    Float_t       xoffset = 0.200,
		    Float_t       yoffset = 0.050);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// computeDrellYanPtllWeight
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

  ratio->SetMinimum(0.8);
  ratio->SetMaximum(1.2);

  ratio->Draw("ep");

  SetAxis(ratio, ratio->GetXaxis()->GetTitle(), "data / MC", 1.7, 1.8);


  // Old function
  //----------------------------------------------------------------------------
  TF1* fOld = new TF1("fOld", "[2]*(0.95-[3]*TMath::Erf((x-[0])/[1]))", 0, 100);

  fOld->SetLineColor  (kBlue);
  fOld->SetMarkerColor(kBlue);

  fOld->SetParameter(0,   12.5151);
  fOld->SetParameter(1,   5.51582);
  fOld->SetParameter(2,   1.08683);
  fOld->SetParameter(3, 0.0657370);

  fOld->Draw("same");


  // New function
  //
  //   1  p0           1.29841e+01   4.19572e-02   1.06607e-03  -3.11461e-02
  //   2  p1           6.01778e+00   7.72143e-02   2.06097e-03  -9.89485e-03
  //   3  p2           1.04796e+00   3.21517e-04   7.66763e-06  -3.54511e+00
  //   4  p3           7.40024e-02   3.07885e-04   8.06164e-06  -1.70240e+00
  //  
  //----------------------------------------------------------------------------
  TF1* fNew = new TF1("fNew", "[2]*(0.95-[3]*TMath::Erf((x-[0])/[1]))", 0, 100);

  fNew->SetLineColor  (kRed+1);
  fNew->SetMarkerColor(kRed+1);

  fNew->SetParameter(0,  10);
  fNew->SetParameter(1,   1);
  fNew->SetParameter(2,   1);
  fNew->SetParameter(3, 0.1);

  ratio->Fit(fNew);


  // Legend
  //----------------------------------------------------------------------------
  DrawLegend(0.7, 0.83, (TObject*)fOld, " old fit");
  DrawLegend(0.7, 0.77, (TObject*)fNew, " new fit");


  // Save
  //----------------------------------------------------------------------------
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
