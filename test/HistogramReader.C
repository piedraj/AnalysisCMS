#include "HistogramReader.h"


//------------------------------------------------------------------------------
// HistogramReader
//------------------------------------------------------------------------------
HistogramReader::HistogramReader(TString const &inputdir,
				 TString const &outputdir) :
  _inputdir     (inputdir),
  _outputdir    (outputdir),
  _stackoption  ("nostack,hist"),
  _luminosity_fb(0),
  _drawratio    (false),
  _drawyield    (false),
  _savepdf      (false),
  _savepng      (true)
{
  _mcfile.clear();
  _mccolor.clear();
  _mclabel.clear();

  _datafile  = NULL;
  _datahist  = NULL;
  _allmchist = NULL;
}


//------------------------------------------------------------------------------
// AddData
//------------------------------------------------------------------------------
void HistogramReader::AddData(TString const &filename,
			      TString const &label,
			      Color_t        color)
{
  TFile *file = new TFile(_inputdir + filename + ".root", "update");

  _datafile  = file;
  _datalabel = label;
  _datacolor = color;
}


//------------------------------------------------------------------------------
// AddProcess
//------------------------------------------------------------------------------
void HistogramReader::AddProcess(TString const &filename,
				 TString const &label,
				 Color_t        color)
{
  TFile *file = new TFile(_inputdir + filename + ".root", "update");

  _mcfile.push_back(file);
  _mclabel.push_back(label);
  _mccolor.push_back(color);
}


//------------------------------------------------------------------------------
// Draw
//------------------------------------------------------------------------------
void HistogramReader::Draw(TString hname,
			   TString xtitle,
			   Int_t   ngroup,
			   Int_t   precision,
			   TString units,
			   Bool_t  setlogy,
			   Bool_t  moveoverflow,
			   Float_t xmin,
			   Float_t xmax,
			   Float_t ymin,
			   Float_t ymax)
{
  TString cname = hname;

  if (setlogy) cname += "_log";

  TCanvas* canvas = NULL;

  TPad* pad1 = NULL;
  TPad* pad2 = NULL;

  if (_drawratio && _datafile)
    {
      canvas = new TCanvas(cname, cname, 550, 720);

      pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
      pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);

      pad1->SetTopMargin   (0.08);
      pad1->SetBottomMargin(0.02);
      pad1->Draw();

      pad2->SetTopMargin   (0.08);
      pad2->SetBottomMargin(0.35);
      pad2->Draw();
    }
  else
    {
      canvas = new TCanvas(cname, cname, 550, 600);

      pad1 = new TPad("pad1", "pad1", 0, 0, 1, 1);

      pad1->Draw();
    }


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(setlogy);

  _mchist.clear();

  THStack* hstack = new THStack(hname, hname);

  for (UInt_t i=0; i<_mcfile.size(); i++) {

    TH1D* dummy = (TH1D*)_mcfile[i]->Get(hname);

    if (!dummy)
      {
	printf("\n Error: %s does not exist\n\n", hname.Data());
	return;
      }

    if (xmin == -999) xmin = dummy->GetXaxis()->GetXmin();
    if (xmax == -999) xmax = dummy->GetXaxis()->GetXmax();

    _mchist.push_back((TH1D*)dummy->Clone());

    _mchist[i]->SetFillColor(_mccolor[i]);
    _mchist[i]->SetLineColor(_mccolor[i]);
    _mchist[i]->SetLineWidth(0);
    _mchist[i]->SetFillStyle(1001);

    if (_stackoption.Contains("nostack") && Yield(_mchist[i]) > 0)
      {
	_mchist[i]->SetFillStyle(0);
	_mchist[i]->SetLineWidth(2);
	_mchist[i]->Scale(1.0/Yield(_mchist[i]));
      }

    if (ngroup > 0) _mchist[i]->Rebin(ngroup);

    if (moveoverflow) MoveOverflows(_mchist[i], xmin, xmax);

    hstack->Add(_mchist[i]);
  }


  SetData(hname, ngroup, moveoverflow, xmin, xmax);


  // hfirst will contain the axis settings
  //----------------------------------------------------------------------------
  TH1D* hfirst = (TH1D*)_mchist[0]->Clone("hfirst");

  hfirst->Reset();


  // All MC
  //----------------------------------------------------------------------------
  _allmchist = (TH1D*)_mchist[0]->Clone("allmchist");
  
  for (Int_t ibin=0; ibin<=_allmchist->GetNbinsX(); ibin++) {

    Float_t binValue = 0.;
    Float_t binError = 0.;

    for (UInt_t i=0; i<_mchist.size(); i++) {

      Float_t binContent   = _mchist[i]->GetBinContent(ibin);
      Float_t binStatError = _mchist[i]->GetBinError(ibin);
      Float_t binSystError = 0;  // To be updated

      binValue += binContent;
      binError += (binStatError * binStatError);
      binError += (binSystError * binSystError);
    }
    
    binError = sqrt(binError);

    _allmchist->SetBinContent(ibin, binValue);
    _allmchist->SetBinError  (ibin, binError);
  }

  _allmclabel = "stat";

  _allmchist->SetFillColor  (kGray+1);
  _allmchist->SetFillStyle  (   3345);
  _allmchist->SetLineColor  (kGray+1);
  _allmchist->SetMarkerColor(kGray+1);
  _allmchist->SetMarkerSize (      0);

  if (_stackoption.Contains("nostack") && Yield(_allmchist) > 0)
    {
      _allmchist->SetLineWidth(2);
      _allmchist->Scale(1.0/Yield(_allmchist));
    }


  // Draw
  //----------------------------------------------------------------------------
  hfirst->Draw();

  hstack->Draw(_stackoption + ",same");

  _allmchist->Draw("e2,same");

  if (_datahist) _datahist->Draw("ep,same");


  // Set xtitle and ytitle
  //----------------------------------------------------------------------------
  TString ytitle = Form("events / %s.%df", "%", precision);

  ytitle = Form(ytitle.Data(), hfirst->GetBinWidth(0));

  if (!units.Contains("NULL")) {
    xtitle = Form("%s [%s]", xtitle.Data(), units.Data());
    ytitle = Form("%s %s",   ytitle.Data(), units.Data());
  }


  // Adjust xaxis and yaxis
  //----------------------------------------------------------------------------
  hfirst->GetXaxis()->SetRangeUser(xmin, xmax);

  Float_t theMin = 0.0;

  Float_t theMax = (_datahist) ? GetMaximum(_datahist, xmin, xmax) : 0.0;

  Float_t theMaxMC = GetMaximum(_allmchist, xmin, xmax);

  if (_stackoption.Contains("nostack"))
    {
      for (UInt_t i=0; i<_mcfile.size(); i++)
	{
	  Float_t mchist_i_max = GetMaximum(_mchist[i], xmin, xmax, false);

	  if (mchist_i_max > theMaxMC) theMaxMC = mchist_i_max;
	}
    }

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (pad1->GetLogy())
    {
      theMin = 3e-1;
      theMax = TMath::Power(10, TMath::Log10(theMax) + 3);
    }
  else
    {
      theMax *= 1.45;
    }

  hfirst->SetMinimum(theMin);
  hfirst->SetMaximum(theMax);

  if (ymin != -999) hfirst->SetMinimum(ymin);
  if (ymax != -999) hfirst->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Float_t x0     = 0.220;
  Float_t y0     = 0.834;
  Float_t xdelta = 0.0;
  Float_t ydelta = _yoffset + 0.001;
  Int_t   ny     = 0;

  TString opt = (_stackoption.Contains("nostack")) ? "l" : "f";

  if (_datahist)
    {
      DrawLegend(x0 + xdelta, y0 - ny*ydelta, _datahist, _datalabel.Data(), "lp");
      ny++;
    }

  DrawLegend(x0 + xdelta, y0 - ny*ydelta, _allmchist, _allmclabel.Data(), opt);
  ny++;

  for (int i=0; i<_mchist.size(); i++)
    {
      if (ny == 4)
	{
	  ny = 0;
	  xdelta += 0.228;
	}

      DrawLegend(x0 + xdelta, y0 - ny*ydelta, _mchist[i], _mclabel[i].Data(), opt);
      ny++;
    }


  // Titles
  //----------------------------------------------------------------------------
  Float_t xprelim = (_drawratio && _datafile) ? 0.288 : 0.300;

  DrawLatex(61, 0.190,   0.945, 0.050, 11, "CMS");
  DrawLatex(52, xprelim, 0.945, 0.030, 11, "Preliminary");
  DrawLatex(42, 0.940,   0.945, 0.050, 31, Form("%.3f fb^{-1} (13TeV)", _luminosity_fb));

  SetAxis(hfirst, xtitle, ytitle, 0.045, 1.5, 1.7);


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  if (_drawratio && _datafile)
    {
      pad2->cd();
    
      TH1D* ratio       = (TH1D*)_datahist ->Clone("ratio");
      TH1D* uncertainty = (TH1D*)_allmchist->Clone("uncertainty");

      for (Int_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {

	Float_t dtValue = _datahist->GetBinContent(ibin);
	Float_t dtError = _datahist->GetBinError  (ibin);

	Float_t mcValue = _allmchist->GetBinContent(ibin);
	Float_t mcError = _allmchist->GetBinError  (ibin);

	Float_t ratioVal         = 999;
	Float_t ratioErr         = 999;
	Float_t uncertaintyError = 999;

	if (mcValue > 0)
	  {
	    ratioVal         = dtValue / mcValue;
	    ratioErr         = dtError / mcValue;
	    uncertaintyError = ratioVal * mcError / mcValue;
	  }
	
	ratio->SetBinContent(ibin, ratioVal);
	ratio->SetBinError  (ibin, ratioErr);
	
	uncertainty->SetBinContent(ibin, 1.);
	uncertainty->SetBinError  (ibin, uncertaintyError);
      }

      ratio->Draw("ep");

      ratio->GetXaxis()->SetRangeUser(xmin, xmax);
      ratio->GetYaxis()->SetRangeUser(0, 2);

      uncertainty->Draw("e2,same");

      ratio->Draw("ep,same");

      SetAxis(ratio, xtitle, "data / MC", 0.105, 1.4, 0.75);
    }


  //----------------------------------------------------------------------------
  // Save it
  //----------------------------------------------------------------------------
  canvas->cd();

  if (_savepdf) canvas->SaveAs(_outputdir + cname + ".pdf");
  if (_savepng) canvas->SaveAs(_outputdir + cname + ".png");
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void HistogramReader::DrawLatex(Font_t      tfont,
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
TLegend* HistogramReader::DrawLegend(Float_t x1,
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

  if (_drawyield) final_label = Form("%s (%.0f)", final_label.Data(), Yield(hist));

  legend->AddEntry(hist, final_label.Data(), option.Data());
  legend->Draw();

  return legend;
}


//------------------------------------------------------------------------------
// GetMaximum
//------------------------------------------------------------------------------
Float_t HistogramReader::GetMaximum(TH1*    hist,
				    Float_t xmin,
				    Float_t xmax,
				    Bool_t  binError)
{
  UInt_t nbins = hist->GetNbinsX();

  TAxis* axis = (TAxis*)hist->GetXaxis();
  
  Int_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

  Float_t hmax = 0;

  for (Int_t i=firstBin; i<=lastBin; i++) {

    Float_t binHeight = hist->GetBinContent(i);

    if (binError) binHeight += hist->GetBinError(i);

    if (binHeight > hmax) hmax = binHeight;
  }

  return hmax;
}


//------------------------------------------------------------------------------
// MoveOverflows
//
// For all histogram types: nbins, xlow, xup
//
//   bin = 0;       underflow bin
//   bin = 1;       first bin with low-edge xlow INCLUDED
//   bin = nbins;   last bin with upper-edge xup EXCLUDED
//   bin = nbins+1; overflow bin
//
//------------------------------------------------------------------------------
void HistogramReader::MoveOverflows(TH1*    hist,
				    Float_t xmin,
				    Float_t xmax)
{
  int nentries = hist->GetEntries();

  int nbins = hist->GetNbinsX();
  
  TAxis* xaxis = (TAxis*)hist->GetXaxis();


  // Underflow
  //----------------------------------------------------------------------------
  if (xmin != -999)
    {
      Int_t   firstBin = -1;
      Float_t firstVal = 0;
      Float_t firstErr = 0;
      
      for (Int_t i=0; i<=nbins+1; i++)
	{
	  if (xaxis->GetBinLowEdge(i) < xmin)
	    {
	      firstVal += hist->GetBinContent(i);
	      firstErr += (hist->GetBinError(i)*hist->GetBinError(i));
	      hist->SetBinContent(i, 0);
	      hist->SetBinError  (i, 0);
	    }
	  else if (firstBin == -1)
	    {
	      firstVal += hist->GetBinContent(i);
	      firstErr += (hist->GetBinError(i)*hist->GetBinError(i));
	      firstBin = i;
	    }
	}

      firstErr = sqrt(firstErr);
  
      hist->SetBinContent(firstBin, firstVal);
      hist->SetBinError  (firstBin, firstErr);
    }


  // Overflow
  //----------------------------------------------------------------------------
  if (xmax != -999)
    {
      Int_t   lastBin = -1;
      Float_t lastVal = 0;
      Float_t lastErr = 0;
      
      for (Int_t i=nbins+1; i>=0; i--)
	{
	  Float_t lowEdge = xaxis->GetBinLowEdge(i);
      
	  if (lowEdge >= xmax)
	    {
	      lastVal += hist->GetBinContent(i);
	      lastErr += (hist->GetBinError(i)*hist->GetBinError(i));
	      hist->SetBinContent(i, 0);
	      hist->SetBinError  (i, 0);
	    }
	  else if (lastBin == -1)
	    {
	      lastVal += hist->GetBinContent(i);
	      lastErr += (hist->GetBinError(i)*hist->GetBinError(i));
	      lastBin = i;
	    }
	}

      lastErr = sqrt(lastErr);
  
      hist->SetBinContent(lastBin, lastVal);
      hist->SetBinError  (lastBin, lastErr);
    }

  hist->SetEntries(nentries);
}


//------------------------------------------------------------------------------
// SetAxis
//------------------------------------------------------------------------------
void HistogramReader::SetAxis(TH1*    hist,
			      TString xtitle,
			      TString ytitle,
			      Float_t size,
			      Float_t xoffset,
			      Float_t yoffset)
{
  gPad->cd();
  gPad->Update();

  TAxis* xaxis = (TAxis*)hist->GetXaxis();
  TAxis* yaxis = (TAxis*)hist->GetYaxis();

  xaxis->SetLabelFont(42);
  yaxis->SetLabelFont(42);
  xaxis->SetTitleFont(42);
  yaxis->SetTitleFont(42);

  xaxis->SetLabelOffset(0.025);
  yaxis->SetLabelOffset(0.025);
  xaxis->SetTitleOffset(xoffset);
  yaxis->SetTitleOffset(yoffset);

  xaxis->SetLabelSize(size);
  yaxis->SetLabelSize(size);
  xaxis->SetTitleSize(size);
  yaxis->SetTitleSize(size);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(ytitle);

  xaxis->SetNdivisions(505);
  yaxis->SetNdivisions(505);

  yaxis->CenterTitle();

  gPad->GetFrame()->DrawClone();
  gPad->RedrawAxis();
}


//------------------------------------------------------------------------------
// SetData
//------------------------------------------------------------------------------
Int_t HistogramReader::SetData(TString hname,
			       Int_t   ngroup,
			       Bool_t  moveoverflow,
			       Float_t xmin,
			       Float_t xmax)
{
  if (!_datafile) return -1;

  TH1D* dummy = (TH1D*)_datafile->Get(hname);

  if (!dummy)
    {
      printf(" [HistogramReader::SetData] Histogram %s not found\n", hname.Data());

      return -1;
    }

  _datahist = (TH1D*)dummy->Clone();

  _datahist->SetFillColor  ( _datacolor);
  _datahist->SetLineColor  ( _datacolor);
  _datahist->SetMarkerStyle(kFullCircle);

  if (_stackoption.Contains("nostack") && Yield(_datahist) > 0)
    {
      _datahist->Scale(1.0/Yield(_datahist));
    }

  if (ngroup > 0) _datahist->Rebin(ngroup);
  
  if (moveoverflow) MoveOverflows(_datahist, xmin, xmax);

  return 0;
}


//------------------------------------------------------------------------------
// Yield
//------------------------------------------------------------------------------
Float_t HistogramReader::Yield(TH1* hist)
{
  if (!hist) return 0;

  Int_t nbins = hist->GetNbinsX();
      
  return hist->Integral(0, nbins+1);
}


//------------------------------------------------------------------------------
// EventsByCut
//------------------------------------------------------------------------------
void HistogramReader::EventsByCut(TFile*  file,
				  TString analysis,
				  TString hname)
{
  // Check if the evolution histogram already exists
  TH1D* test_hist = (TH1D*)file->Get(analysis + "/" + hname + "_evolution");

  if (test_hist) return;


  // Get the number of bins
  Int_t nbins = 0;
  
  for (Int_t i=0; i<ncut; i++)
    {
      if (!scut[i].Contains(analysis + "/")) continue;

      nbins++;
    }


  // Create and fill the evolution histogram
  file->cd(analysis);

  TH1D* hist = new TH1D(hname + "_evolution", "", nbins, -0.5, nbins-0.5);

  for (Int_t i=0, bin=0; i<ncut; i++)
    {
      if (!scut[i].Contains(analysis + "/")) continue;

      TH1D* dummy = (TH1D*)file->Get(scut[i] + "/" + hname);

      hist->SetBinContent(++bin, Yield(dummy));


      // Change the evolution histogram x-axis labels
      TString tok, icut;

      Ssiz_t from = 0;

      while (scut[i].Tokenize(tok, from, "_")) icut = tok;

      hist->GetXaxis()->SetBinLabel(bin, icut);
    }


  // Write the evolution histogram
  hist->Write();
  file->cd();
}


//------------------------------------------------------------------------------
// LoopEventsByCut
//------------------------------------------------------------------------------
void HistogramReader::LoopEventsByCut(TString analysis, TString hname)
{
  if (_datafile) EventsByCut(_datafile, analysis, hname);

  for (UInt_t i=0; i<_mcfile.size(); i++) EventsByCut(_mcfile[i], analysis, hname);
}


//------------------------------------------------------------------------------
// EventsByChannel
//------------------------------------------------------------------------------
void HistogramReader::EventsByChannel(TFile*  file,
				      TString level)
{
  // Check if the evolution histogram already exists
  TH1D* test_hist = (TH1D*)file->Get(level + "/h_counterLum_evolution");

  if (test_hist) return;


  // Get the number of bins
  int firstchannel = (level.Contains("WZ/")) ? eee : ee;
  int lastchannel  = (level.Contains("WZ/")) ? lll : ll;
  
  Int_t nbins = 0;
  
  for (Int_t i=firstchannel; i<=lastchannel; i++) nbins++;


  // Create and fill the evolution histogram
  file->cd(level);

  TH1D* hist = new TH1D("h_counterLum_evolution", "", nbins, -0.5, nbins-0.5);

  for (Int_t i=firstchannel, bin=0; i<=lastchannel; i++)
    {
      TH1D* dummy = (TH1D*)file->Get(level + "/h_counterLum_" + schannel[i]);

      hist->SetBinContent(++bin, Yield(dummy));

      hist->GetXaxis()->SetBinLabel(bin, lchannel[i]);
    }


  // Write the evolution histogram
  hist->Write();
  file->cd();
}


//------------------------------------------------------------------------------
// LoopEventsByChannel
//------------------------------------------------------------------------------
void HistogramReader::LoopEventsByChannel(TString level)
{
  if (_datafile) EventsByChannel(_datafile, level);

  for (UInt_t i=0; i<_mcfile.size(); i++) EventsByChannel(_mcfile[i], level);
}
