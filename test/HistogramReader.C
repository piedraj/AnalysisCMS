#include "HistogramReader.h"

using namespace std;


//------------------------------------------------------------------------------
// HistogramReader
//
// The following is valid for both _mcscale and _signalscale
//
//    If the scale value for a process is equal to -999 then that process is
//    not scaled by luminosity, as it is done for fakes
//
//    If the scale value for a process is greater than zero, then the process
//    is also scaled by such scale value
//------------------------------------------------------------------------------
HistogramReader::HistogramReader(const TString& inputdir,
				 const TString& outputdir) :
  _inputdir        (inputdir),
  _outputdir       (outputdir),
  _stackoption     ("nostack,hist"),
  _title           ("inclusive"),
  _luminosity_fb   (-1),
  _changebinlabel  (false),
  _datanorm        (false),
  _drawratio       (false),
  _drawsignificance(false),
  _drawyield       (false),
  _minitreebased   (false),
  _publicstyle     (false),
  _savepdf         (false),
  _savepng         (true),
  _saveratio       (true)
{
  _datafile   = NULL;
  _datahist   = NULL;
  _allmchist  = NULL;
  _prefitfile = NULL;
  _prefithist = NULL;

  _mcfile.clear();
  _mccolor.clear();
  _mclabel.clear();
  _mcscale.clear();

  _signalfile.clear();
  _signalcolor.clear();
  _signallabel.clear();
  _signalscale.clear();

  TH1::SetDefaultSumw2();
}


//------------------------------------------------------------------------------
// AddData
//------------------------------------------------------------------------------
void HistogramReader::AddData(const TString& filename,
			      const TString& label,
			      Color_t        color)
{
  TString fullname = _inputdir + "/" + filename + ".root";

  if (gSystem->AccessPathName(fullname))
    {
      printf(" [HistogramReader::AddData] Cannot access %s\n", fullname.Data());
      return;
    }

  TFile* file = new TFile(fullname, "update");

  _datacolor    = color;
  _datafile     = file;
  _datafilename = filename;
  _datalabel    = label;
}


//------------------------------------------------------------------------------
// AddPrefit
//------------------------------------------------------------------------------
void HistogramReader::AddPrefit(const TString& filename,
				const TString& label,
				Color_t        color)
{
  TString fullname = _inputdir + "/" + filename + ".root";

  if (gSystem->AccessPathName(fullname))
    {
      printf(" [HistogramReader::AddPrefit] Cannot access %s\n", fullname.Data());
      return;
    }

  TFile* file = new TFile(fullname, "update");

  _prefitcolor    = color;
  _prefitfile     = file;
  _prefitfilename = filename;
  _prefitlabel    = label;
}


//------------------------------------------------------------------------------
// AddProcess
//------------------------------------------------------------------------------
void HistogramReader::AddProcess(const TString& filename,
				 const TString& label,
				 Color_t        color,
				 Int_t          kind,
				 Float_t        scale)
{
  TString fullname = _inputdir + "/" + filename + ".root";

  if (gSystem->AccessPathName(fullname))
    {
      printf(" [HistogramReader::AddProcess] Cannot access %s\n", fullname.Data());
      return;
    }

  TFile* file = new TFile(fullname, "update");

  _mccolor.push_back(color);
  _mcfile.push_back(file);
  _mcfilename.push_back(filename); 
  _mclabel.push_back(label);
  _mcscale.push_back(scale);
  
  if (scale > 0. && scale != 1.)
    printf("\n [HistogramReader::AddProcess] Process %s will be scaled by %.2f\n\n", label.Data(), scale);

  if (kind == roc_signal)
    {
      _roc_signalfile.push_back(file);
      _roc_signalscale.push_back(scale);
    }
  else if (kind == roc_background)
    {
      _roc_backgroundfile.push_back(file);
      _roc_backgroundscale.push_back(scale);
    }
}


//------------------------------------------------------------------------------
// AddSignal
//------------------------------------------------------------------------------
void HistogramReader::AddSignal(const TString& filename,
				const TString& label,
				Color_t        color,
				Int_t          kind,
				Float_t        scale)
{
  TString fullname = _inputdir + "/" + filename + ".root";
  
  if (gSystem->AccessPathName(fullname))
    {
      printf(" [HistogramReader::AddSignal] Cannot access %s\n", fullname.Data());
      return;
    }

  TFile* file = new TFile(fullname, "update");

  _signalcolor.push_back(color);
  _signalfile.push_back(file);
  _signalfilename.push_back(filename);
  _signallabel.push_back(label);
  _signalscale.push_back(scale);

  if (scale > 0. && scale != 1.)
    printf("\n [HistogramReader::AddSignal] Process %s will be scaled by %.2f\n\n", label.Data(), scale);

  if (kind == roc_signal)
    {
      _roc_signalfile.push_back(file);
      _roc_signalscale.push_back(scale);
    }
  else if (kind == roc_background)
    {
      _roc_backgroundfile.push_back(file);
      _roc_backgroundscale.push_back(scale);
    }
}


//------------------------------------------------------------------------------
// AddSystematic
//------------------------------------------------------------------------------
void HistogramReader::AddSystematic(TString systematic)
{
  _systematics.push_back(systematic);
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
  _xmin = xmin;
  _xmax = xmax;

  TString cname = hname;

  if (_stackoption.Contains("nostack")) cname += "_nostack";

  if (setlogy) cname += "_log";

  _writeyields = (hname.Contains("_evolution")) ? true : false;

  if (_writeyields)
    {
      _yields_table.open(_outputdir + "/" + cname + ".txt");

      _writelabels = true;
    }


  TCanvas* canvas = NULL;

  TPad* pad1 = NULL;
  TPad* pad2 = NULL;


  // Set drawsignificance to false if drawratio is true
  if (_drawratio && _datafile) _drawsignificance = false;


  if ((_drawratio && _datafile) || _drawsignificance)
    {
      Int_t ww = (_publicstyle) ? 800 : 550;
      Int_t wh = (_publicstyle) ? 800 : 720;

      canvas = new TCanvas(cname, cname, ww, wh);

      pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
      pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);

      pad1->SetTopMargin   (0.08);
      pad1->SetBottomMargin(0.01);  // 0.02

      pad2->SetTopMargin   (0.05);  // 0.08
      pad2->SetBottomMargin(0.45);  // 0.35

      pad1->Draw();
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


  // Stack processes
  //----------------------------------------------------------------------------
  _mchist.clear();

  THStack* mcstack = new THStack(hname + "_mcstack", hname + "_mcstack");

  for (UInt_t i=0; i<_mcfile.size(); i++) {

    _mcfile[i]->cd();

    TH1D* dummy = (TH1D*)_mcfile[i]->Get(hname);

    _mchist.push_back((TH1D*)dummy->Clone());

    if (_luminosity_fb > 0 && _mcscale[i] > -999) _mchist[i]->Scale(_luminosity_fb);

    if (_mcscale[i] > 0) _mchist[i]->Scale(_mcscale[i]);

    SetHistogram(_mchist[i], _mccolor[i], 1001, kDot, kSolid, 0, ngroup, moveoverflow, xmin, xmax);
    
    mcstack->Add(_mchist[i]);
  }


  // Stack signals
  //----------------------------------------------------------------------------
  _signalhist.clear();

  THStack* signalstack = new THStack(hname + "_signalstack", hname + "_signalstack");

  for (UInt_t i=0; i<_signalfile.size(); i++) {

    _signalfile[i]->cd();

    TH1D* dummy = (TH1D*)_signalfile[i]->Get(hname);

    _signalhist.push_back((TH1D*)dummy->Clone());

    if (_luminosity_fb > 0 && _signalscale[i] > -999) _signalhist[i]->Scale(_luminosity_fb);

    if (_signalscale[i] > 0) _signalhist[i]->Scale(_signalscale[i]);

    SetHistogram(_signalhist[i], _signalcolor[i], 0, kDot, kSolid, 4, ngroup, moveoverflow, xmin, xmax);
    
    signalstack->Add(_signalhist[i], "][");
  }


  // Get the data
  //----------------------------------------------------------------------------
  if (_datafile)
    {
      _datafile->cd();

      TH1D* dummy = (TH1D*)_datafile->Get(hname);

      _datahist = (TH1D*)dummy->Clone();
      
      SetHistogram(_datahist, kBlack, 0, kFullCircle, kSolid, 1, ngroup, moveoverflow, xmin, xmax);
    }


  // Get the prefit
  //----------------------------------------------------------------------------
  if (_prefitfile)
    {
      _prefitfile->cd();

      TH1D* dummy = (TH1D*)_prefitfile->Get(hname);

      _prefithist = (TH1D*)dummy->Clone();
      
      if (_luminosity_fb) _prefithist->Scale(_luminosity_fb);
      
      SetHistogram(_prefithist, _prefitcolor, 0, kDot, 7, 4, ngroup, moveoverflow, xmin, xmax);
    }


  // Normalize MC to data
  //----------------------------------------------------------------------------
  if (_datahist && _datanorm)
    {
      Float_t mcnorm   = Yield((TH1D*)(mcstack->GetStack()->Last()));
      Float_t datanorm = Yield(_datahist);

      for (UInt_t i=0; i<_mchist.size(); i++)
	{
	  _mchist[i]->Scale(datanorm / mcnorm);
	}

      mcstack->Modified();
    }


  // hfirst will contain the axis settings
  //----------------------------------------------------------------------------
  TH1D* hfirst = (TH1D*)_mchist[0]->Clone("hfirst");

  hfirst->Reset();

  hfirst->SetTitle("");


  // All MC
  //----------------------------------------------------------------------------
  _allmchist = (TH1D*)_mchist[0]->Clone("allmchist");

  _allmchist->SetName(_mchist[0]->GetName());

  // Possible modification (how to deal with systematic uncertainties?)
  //  _allmchist = (TH1D*)(mcstack->GetStack()->Last());


  // Include systematics
  //----------------------------------------------------------------------------
  if (_systematics.size() > 0) IncludeSystematics(hname);

  
  for (Int_t ibin=0; ibin<=_allmchist->GetNbinsX(); ibin++) {

    Float_t binValue = 0.;
    Float_t binError = 0.;

    for (UInt_t i=0; i<_mchist.size(); i++) {

      Float_t binContent   = _mchist[i]->GetBinContent(ibin);
      Float_t binStatError = sqrt(_mchist[i]->GetSumw2()->At(ibin));
      Float_t binSystError = (_mchist_syst.size() > 0) ? _mchist_syst[i]->GetBinContent(ibin) : 0.;
      Float_t binLumiError = binContent * lumi_error_percent / 1e2; 

      binValue += binContent;
      binError += (binStatError * binStatError);
      binError += (binSystError * binSystError);
      binError += (binLumiError * binLumiError); 
    }

    binError = sqrt(binError);

    _allmchist->SetBinContent(ibin, binValue);
    _allmchist->SetBinError  (ibin, binError);

  }

  _allmclabel = "Bkg. unc.";

  _allmchist->SetFillColor  (kGray+2);  // kGray+1
  _allmchist->SetFillStyle  (   3345);
  _allmchist->SetLineColor  (kGray+2);  // kGray+1
  _allmchist->SetMarkerColor(kGray+2);  // KGray+1
  _allmchist->SetMarkerSize (      0);


  // Draw
  //----------------------------------------------------------------------------
  hfirst->Draw();

  mcstack->Draw(_stackoption + ",same");

  if (!_stackoption.Contains("nostack")) _allmchist->Draw("e2,same");

  if (_prefitfile) _prefithist->Draw("hist,][,same");

  if (_signalfile.size() > 0) signalstack->Draw("nostack,hist,][,same");

  if (_datahist) _datahist->Draw("ep,same");


  // Set xtitle and ytitle
  //----------------------------------------------------------------------------
  TString ytitle = "Events / bin";

  if (precision > -1) ytitle = Form("Events / %s.%df", "%", precision);

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
      theMaxMC = 0.0;

      for (UInt_t i=0; i<_mcfile.size(); i++)
	{
	  Float_t mchist_i_max = GetMaximum(_mchist[i], xmin, xmax, false);

	  if (mchist_i_max > theMaxMC) theMaxMC = mchist_i_max;
	}
    }

  if (theMaxMC > theMax) theMax = theMaxMC;

  Float_t theMaxSignal = 0.0;

  if (_signalfile.size() > 0)
    {
      for (UInt_t i=0; i<_signalfile.size(); i++)
	{
	  Float_t signalhist_i_max = GetMaximum(_signalhist[i], xmin, xmax, false);

	  if (signalhist_i_max > theMaxSignal) theMaxSignal = signalhist_i_max;
	}
    }

  if (theMaxSignal > theMax) theMax = theMaxSignal;

  if (pad1->GetLogy())
    {
      theMin = 0.2;
      theMax = TMath::Power(10, TMath::Log10(theMax) + 4);
    }
  else
    {
      theMax *= 1.7;
    }

  hfirst->SetMinimum(theMin);
  hfirst->SetMaximum(theMax);

  if (ymin != -999) hfirst->SetMinimum(ymin);
  if (ymax != -999) hfirst->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Float_t tsize  = 0.030;                         // text size
  Float_t x0     = 0.218;                         // x position of the data on the top left
  Float_t y0     = 0.843;                         // y position of the data on the top left
  Float_t xdelta = (_drawyield) ? 0.228 : 0.170;  // x width between columns
  Float_t ydelta = 0.050;                         // y width between rows
  Int_t   nx     = 0;                             // column number
  Int_t   ny     = 0;                             // row    number

  TString opt = (_stackoption.Contains("nostack")) ? "l" : "f";

  if (_publicstyle)
    {
      tsize  = 0.035;
      xdelta = 0.282;
      ydelta = 0.049;
    }


  // Data legend
  //----------------------------------------------------------------------------
  if (_datahist)
    {
      DrawLegend(x0, y0, _datahist, _datalabel.Data(), "elp", true, tsize);
      ny++;
    }


  // All MC legend
  //----------------------------------------------------------------------------
  if (!_stackoption.Contains("nostack") && !_publicstyle)
    {
      DrawLegend(x0, y0 - ny*ydelta, _allmchist, _allmclabel.Data(), opt, true, tsize);
      ny++;
    }


  // Standard Model processes legend
  //----------------------------------------------------------------------------
  Int_t nrow = (_mchist.size() > 10) ? 5 : 4;

  if (_publicstyle) nrow = 5;

  for (int i=0; i<_mchist.size(); i++)
    {
      int j = (_publicstyle) ? _mchist.size()-i-1 : i;

      if (ny == nrow)
	{
	  ny = 0;
	  nx++;
	}

      DrawLegend(x0 + nx*xdelta, y0 - ny*ydelta, _mchist[j], _mclabel[j].Data(), opt, true, tsize);
      ny++;
    }


  // All MC legend
  //----------------------------------------------------------------------------
  if (!_stackoption.Contains("nostack") && _publicstyle)
    {
      DrawLegend(x0 + nx*xdelta, y0 - ny*ydelta, _allmchist, _allmclabel.Data(), opt, true, tsize);
      ny++;
    }


  // Prefit legend
  //----------------------------------------------------------------------------
  if (_prefithist)
    {
      DrawLegend(x0 + nx*xdelta, y0 - ny*ydelta, _prefithist, _prefitlabel.Data(), "l", true, tsize);
      ny++;
    }


  // Search signals legend
  //----------------------------------------------------------------------------
  for (int i=0; i<_signalhist.size(); i++)
    {
      DrawLegend(x0 + nx*xdelta, y0 - ny*ydelta, _signalhist[i], _signallabel[i].Data(), "l", true, tsize);
      ny++;
    }


  // CMS title
  //----------------------------------------------------------------------------
  if (_publicstyle)
    {
      DrawLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
      DrawLatex(52, 0.268, 0.945, 0.035, 11, "Preliminary");
    }
  else
    {
      Float_t xprelim = ((_drawratio && _datafile) || _drawsignificance) ? 0.288 : 0.300;

      if (_title.EqualTo("inclusive"))
	{
	  DrawLatex(61, 0.190,   0.945, 0.050, 11, "CMS");
	  DrawLatex(52, xprelim, 0.945, 0.030, 11, "Preliminary");
	}
      else
	{
	  DrawLatex(42, 0.190, 0.945, 0.050, 11, _title);
	}
    }


  // Luminosity title
  //----------------------------------------------------------------------------
  if (_luminosity_fb > 0)
    DrawLatex(42, 0.940, 0.945, 0.050, 31, Form("%.1f fb^{-1} (13TeV)", _luminosity_fb));
  else
    DrawLatex(42, 0.940, 0.945, 0.050, 31, "(13TeV)");

  SetAxis(hfirst, xtitle, ytitle);


  //----------------------------------------------------------------------------
  // Set the bin label to the bin number
  //----------------------------------------------------------------------------
  if (_changebinlabel)
    {
      for (Int_t ibin=1; ibin<=hfirst->GetNbinsX(); ibin++) {
	
	hfirst->GetXaxis()->SetBinLabel(ibin, Form("%d", ibin));
      }
    }


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  if (_drawratio && _datafile)
    {
      pad2->cd();

      // This approach isn't yet working
      //      TGraphAsymmErrors* g = new TGraphAsymmErrors();
      //      g->Divide(_datahist, _allmchist, "cl=0.683 b(1,1) mode");
      //      g->SetMarkerStyle(kFullCircle);
      //      g->Draw("ap");

      TH1D* ratio       = (TH1D*)_datahist->Clone("ratio");
      TH1D* uncertainty = (TH1D*)_allmchist->Clone("uncertainty");
      TH1D* prefitratio = (_prefithist) ? (TH1D*)_prefithist->Clone("prefitratio") : NULL;

      for (Int_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {

	if (_changebinlabel) ratio->GetXaxis()->SetBinLabel(ibin, Form("%d", ibin));

	Float_t dtValue = _datahist->GetBinContent(ibin);
	Float_t dtError = _datahist->GetBinError(ibin);

	Float_t mcValue = _allmchist->GetBinContent(ibin);
	Float_t mcError = _allmchist->GetBinError(ibin);

	Float_t ratioVal       = -999;
	Float_t ratioErr       = -999;
	Float_t uncertaintyErr = -999;

	if (mcValue > 0)
	  {
	    ratioVal       = dtValue / mcValue;
	    ratioErr       = dtError / mcValue;
	    uncertaintyErr = ratioVal * mcError / mcValue;
	  }

	ratio->SetBinContent(ibin, ratioVal);
	ratio->SetBinError  (ibin, ratioErr);
	
	uncertainty->SetBinContent(ibin, 1.);
	uncertainty->SetBinError  (ibin, uncertaintyErr);


	// Prefit part
	//----------------------------------------------------------------------
	if (_prefithist)
	  {
	    Float_t prefitValue = _prefithist->GetBinContent(ibin);

	    Float_t prefitratioVal = (prefitValue > 0) ? dtValue / prefitValue : -999;

	    prefitratio->SetBinContent(ibin, prefitratioVal);
	  }
      }

      ratio->SetTitle("");

      ratio->Draw("ep");

      ratio->GetXaxis()->SetRangeUser(xmin, xmax);

      ratio->GetYaxis()->SetRangeUser(0.5, 1.5);

      uncertainty->Draw("e2,same");

      ratio->Draw("ep,same");

      if (_prefithist) prefitratio->Draw("hist,][,same");

      SetAxis(ratio, xtitle, "Data / Bkg");


      // Save the ratio histogram
      //------------------------------------------------------------------------
      if (_saveratio && hname.Contains("pt2l_mm"))
	{
	  TFile* ratiofile = new TFile(_outputdir + "/" + hname + ".root", "recreate");
	  
	  ratio->Write();
	  
	  ratiofile->Close();
	}
    }
  //  else if (_drawsignificance)
  //    {
  //      // Barbara's stuff
  //    }


  //----------------------------------------------------------------------------
  // Save it
  //----------------------------------------------------------------------------
  canvas->cd();

  if (_savepdf) canvas->SaveAs(_outputdir + cname + ".pdf");
  if (_savepng) canvas->SaveAs(_outputdir + cname + ".png");

  if (_writeyields)
    {
      _yields_table << std::endl;
      
      _yields_table.close();
    }
}


//------------------------------------------------------------------------------
// CrossSection
//------------------------------------------------------------------------------
void HistogramReader::CrossSection(TString level,
				   TString channel,
				   TString process,
				   Float_t branchingratio,
				   TString signal1_filename,
				   Float_t signal1_xs,
				   Float_t signal1_ngen,
				   TString signal2_filename,
				   Float_t signal2_xs,
				   Float_t signal2_ngen)
{
  if (_luminosity_fb < 0)
    {
      printf("\n [HistogramReader::CrossSection] Warning: reading negative luminosity\n\n");
    }


  // Get the signal (example qqWW)
  //----------------------------------------------------------------------------
  TFile* signal1_file = new TFile(_inputdir + "/" + signal1_filename + ".root");

  float signal1_counterLum = Yield((TH1D*)signal1_file->Get(level + "/h_counterLum_" + channel));
  float signal1_counterRaw = Yield((TH1D*)signal1_file->Get(level + "/h_counterRaw_" + channel));

  float counterSignal = signal1_counterLum * _luminosity_fb;

  float efficiency = signal1_counterRaw / signal1_ngen;


  // Get the second signal (example ggWW)
  //----------------------------------------------------------------------------
  if (!signal2_filename.Contains("NULL"))
    {
      TFile* signal2_file = new TFile(_inputdir + "/" + signal2_filename + ".root");

      float signal2_counterLum = Yield((TH1D*)signal2_file->Get(level + "/h_counterLum_" + channel));
      float signal2_counterRaw = Yield((TH1D*)signal2_file->Get(level + "/h_counterRaw_" + channel));

      counterSignal += (signal2_counterLum * _luminosity_fb);

      float signal1_fraction = signal1_xs / (signal1_xs + signal2_xs);
      float signal2_fraction = 1. - signal1_fraction;

      float signal1_efficiency = signal1_counterRaw / signal1_ngen;
      float signal2_efficiency = signal2_counterRaw / signal2_ngen;

      efficiency = signal1_fraction*signal1_efficiency + signal2_fraction*signal2_efficiency;
    }


  // Get the backgrounds
  //----------------------------------------------------------------------------
  float counterBackground = 0;

  for (UInt_t i=0; i<_mcfile.size(); i++) {

    if (_mclabel[i].EqualTo(process)) continue;

    _mcfile[i]->cd();

    TH1D* dummy = (TH1D*)_mcfile[i]->Get(level + "/h_counterLum_" + channel);

    float counterDummy = Yield(dummy);

    if (_luminosity_fb > 0 && _mcscale[i] > -999) counterDummy *= _luminosity_fb;

    if (_mcscale[i] > 0) counterDummy *= _mcscale[i];

    counterBackground += counterDummy;
  }


  // Get the data
  //----------------------------------------------------------------------------
  if (_datafile)
    {
      _datafile->cd();

      TH1D* dummy = (TH1D*)_datafile->Get(level + "/h_counterLum_" + channel);

      _datahist = (TH1D*)dummy->Clone();      
    }

  float counterData = Yield(_datahist);


  // Cross-section calculation
  //----------------------------------------------------------------------------  
  float xs = (counterData - counterBackground) / (1e3 * _luminosity_fb * efficiency * branchingratio);
  float mu = (counterData - counterBackground) / (counterSignal);


  // Statistical error
  //----------------------------------------------------------------------------  
  float xsErrorStat = sqrt(counterData) / (1e3 * _luminosity_fb * efficiency * branchingratio);
  float muErrorStat = sqrt(counterData) / (counterSignal); 

 
  // Print
  //----------------------------------------------------------------------------  
  printf("      channel = %s\n", channel.Data());
  printf("        ndata = %.0f\n", counterData);
  printf("         nbkg = %.2f\n", counterBackground);
  printf(" ndata - nbkg = %.2f\n", counterData - counterBackground);
  printf("      nsignal = %.2f\n", counterSignal);
  printf("           mu = (ndata - nbkg) / nsignal = %.2f +- %.2f (stat) +- %.2f (lumi)\n", mu, muErrorStat, mu * lumi_error_percent / 1e2);
  printf("         lumi = %.0f pb\n", 1e3 * _luminosity_fb);
  printf("           br = %f\n", branchingratio);
  printf("          eff = %.4f\n", efficiency);
  printf("           xs = (ndata - nbkg) / (lumi * eff * br) = %.2f +- %.2f (stat) +- %.2f (lumi) pb\n\n", xs, xsErrorStat, xs * lumi_error_percent / 1e2);
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
				     Bool_t  drawyield,
				     Float_t tsize,
				     Float_t xoffset,
				     Float_t yoffset)
{
  drawyield &= (_drawyield && !_publicstyle);

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

  if (drawyield)
    final_label = Form("%s (%.0f)", final_label.Data(), Yield(hist));

  if (Yield(hist) < 0)
    printf("\n [HistogramReader::DrawLegend] Warning: %s %s yield = %f\n\n",
	   label.Data(),
	   hist->GetName(),
	   Yield(hist));

  legend->AddEntry(hist, final_label.Data(), option.Data());
  legend->Draw();

  WriteYields(hist, label);

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
  
  Int_t firstBin = (xmin > -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax > -999) ? axis->FindBin(xmax) : nbins;

  if (firstBin < 1)     firstBin = 1;
  if (lastBin  > nbins) lastBin  = nbins;

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
  int nbins    = hist->GetNbinsX();
  
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
			      Float_t xoffset,
			      Float_t yoffset)
{
  TString hname = hist->GetName();

  gPad->cd();
  gPad->Update();

  TAxis* xaxis = (TAxis*)hist->GetXaxis();
  TAxis* yaxis = (TAxis*)hist->GetYaxis();

  xaxis->SetLabelFont(43);  // Text font code = 10*fontnumber + precision
  yaxis->SetLabelFont(43);  // Text font code = 10*fontnumber + precision
  xaxis->SetTitleFont(43);  // Text font code = 10*fontnumber + precision
  yaxis->SetTitleFont(43);  // Text font code = 10*fontnumber + precision

  xaxis->SetLabelSize(26);  // precision = 3 scalable and rotatable hardware fonts. Text size is given in pixels
  yaxis->SetLabelSize(26);  // precision = 3 scalable and rotatable hardware fonts. Text size is given in pixels
  xaxis->SetTitleSize(26);  // precision = 3 scalable and rotatable hardware fonts. Text size is given in pixels
  yaxis->SetTitleSize(26);  // precision = 3 scalable and rotatable hardware fonts. Text size is given in pixels

  xaxis->SetTitleOffset(xoffset);
  yaxis->SetTitleOffset(yoffset);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(ytitle);

  if (_minitreebased)
    {
      xaxis->SetLabelOffset(5.*xaxis->GetLabelOffset());
      yaxis->SetLabelOffset(3.*yaxis->GetLabelOffset());
    }

  if (_publicstyle)
    {
      xaxis->SetNdivisions(510);
      yaxis->SetNdivisions(709);
    }

  if (hname.Contains("ratio"))
    {
      yaxis->CenterTitle();
      yaxis->SetNdivisions(104);
    }

  gPad->GetFrame()->DrawClone();
  gPad->RedrawAxis();
}


//------------------------------------------------------------------------------
// SetHistogram
//------------------------------------------------------------------------------
void HistogramReader::SetHistogram(TH1*     hist,
				   Color_t  color,
				   Style_t  fstyle,
				   Style_t  mstyle,
				   Style_t  lstyle,
				   Width_t  lwidth,
				   Int_t    ngroup,
				   Bool_t   moveoverflow,
				   Float_t& xmin,
				   Float_t& xmax)
{
  if (!hist)
    {
      printf("\n [HistogramReader::SetHistogram] Error: histogram does not exist\n\n");
      return;
    }

  if (xmin == -999) xmin = hist->GetXaxis()->GetXmin();
  if (xmax == -999) xmax = hist->GetXaxis()->GetXmax();

  hist->SetFillColor(color );
  hist->SetFillStyle(fstyle);

  hist->SetLineColor(color );
  hist->SetLineStyle(lstyle);
  hist->SetLineWidth(lwidth);

  hist->SetMarkerColor(color );
  hist->SetMarkerStyle(mstyle);

  if (_stackoption.Contains("nostack") && Yield(hist) > 0)
    {
      hist->SetFillStyle(0);
      hist->SetLineWidth(2);

      hist->Scale(1. / Yield(hist));
    }


  // Rebin and move overflow bins
  //----------------------------------------------------------------------------
  if (ngroup > 0) hist->Rebin(ngroup);
  
  if (moveoverflow) MoveOverflows(hist, xmin, xmax);
}


//------------------------------------------------------------------------------
// Yield
//------------------------------------------------------------------------------
Float_t HistogramReader::Yield(TH1* hist)
{
  if (!hist) return 0;

  Float_t hist_yield = hist->Integral(-1, -1);

  return hist_yield;
}


//------------------------------------------------------------------------------
// Error
//------------------------------------------------------------------------------
Float_t HistogramReader::Error(TH1* hist)
{
  if (!hist) return 0;

  Float_t hist_error = sqrt(hist->GetSumw2()->GetSum());

  return hist_error;
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

      bin++;

      hist->SetBinContent(bin, Yield(dummy));
      hist->SetBinError  (bin, Error(dummy));


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

  for (UInt_t i=0; i<_signalfile.size(); i++) EventsByCut(_signalfile[i], analysis, hname);
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
  Int_t firstchannel = (level.Contains("WZ/")) ? eee : ee;
  Int_t lastchannel  = (level.Contains("WZ/")) ? lll : ll;
  
  Int_t nbins = 0;
  
  for (Int_t i=firstchannel; i<=lastchannel; i++) nbins++;


  // Create and fill the evolution histogram
  file->cd(level);

  TH1D* hist = new TH1D("h_counterLum_evolution", "", nbins, -0.5, nbins-0.5);

  for (Int_t i=firstchannel, bin=0; i<=lastchannel; i++)
    {
      TH1D* dummy = (TH1D*)file->Get(level + "/h_counterLum_" + schannel[i]);

      bin++;

      hist->SetBinContent(bin, Yield(dummy));
      hist->SetBinError  (bin, Error(dummy));

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

  for (UInt_t i=0; i<_signalfile.size(); i++) EventsByChannel(_signalfile[i], level);
}


//------------------------------------------------------------------------------
// GetBestScoreX
//------------------------------------------------------------------------------
Float_t HistogramReader::GetBestScoreX(TH1*    sig_hist,
				       TH1*    bkg_hist,
				       TString fom)
{
  Int_t nbins = sig_hist->GetNbinsX();

  Float_t score_value = 0;
  Float_t score_x     = 0;
  Float_t sig_total   = Yield(sig_hist);


  // For The Punzi Effect
  // http://arxiv.org/pdf/physics/0308063v2.pdf
  Float_t a = 5.;
  Float_t b = 1.645;  // Corresponds to a p-value equal to 0.05


  for (UInt_t k=0; k<nbins+1; k++) {

    Float_t sig_yield = sig_hist->Integral(k, nbins+1);
    Float_t bkg_yield = bkg_hist->Integral(k, nbins+1);

    Float_t sig_eff = (sig_total > 0.) ? sig_yield / sig_total : -999;

    if (sig_yield > 0. && bkg_yield > 0.)
      {
	Float_t score = -999;

	if (fom.EqualTo("S / #sqrt{B}"))   score = sig_yield / sqrt(bkg_yield);
	if (fom.EqualTo("S / #sqrt{S+B}")) score = sig_yield / sqrt(sig_yield + bkg_yield);
	if (fom.EqualTo("S / B"))          score = sig_yield / bkg_yield;
	if (fom.EqualTo("Punzi Eq.6"))     score =   sig_eff / (b*b + 2*a*sqrt(bkg_yield) + b*sqrt(b*b + 4*a*sqrt(b) + 4*bkg_yield)); 
	if (fom.EqualTo("Punzi Eq.7"))     score =   sig_eff / (a/2 + sqrt(bkg_yield));

	if (score > score_value)
	  {
	    score_value = score;
	    score_x     = sig_hist->GetBinCenter(k);
	  }
      }
  }


  printf("\n [HistogramReader::GetBestScoreX] x = %.2f (%.2f < x < %.2f) has the best %s (%f)\n\n",
  	 score_x,
  	 sig_hist->GetXaxis()->GetXmin(),
  	 sig_hist->GetXaxis()->GetXmax(),
   	 fom.Data(),
  	 score_value);


  return score_x;
}


//------------------------------------------------------------------------------
// GetBestSignalScoreX
//------------------------------------------------------------------------------
Float_t HistogramReader::GetBestSignalScoreX(TString hname,
					     TString fom,
					     Int_t   ngroup)
{
  printf("\n [HistogramReader::GetBestSignalScoreX] Warning: reading only the first signal\n");


  // Get the signals
  //----------------------------------------------------------------------------
  _signalhist.clear();

  for (UInt_t i=0; i<_signalfile.size(); i++) {

    _signalfile[i]->cd();

    TH1D* dummy = (TH1D*)_signalfile[i]->Get(hname);

    _signalhist.push_back((TH1D*)dummy->Clone());

    if (_luminosity_fb > 0) _signalhist[i]->Scale(_luminosity_fb);

    if (ngroup > 0) _signalhist[i]->Rebin(ngroup);
  }

  
  // Get the backgrounds
  //----------------------------------------------------------------------------
  _mchist.clear();

  THStack* mcstack = new THStack(hname + "_mcstack", hname + "_mcstack");

  for (UInt_t i=0; i<_mcfile.size(); i++) {

    _mcfile[i]->cd();

    TH1D* dummy = (TH1D*)_mcfile[i]->Get(hname);

    _mchist.push_back((TH1D*)dummy->Clone());

    if (_luminosity_fb > 0 && _mcscale[i] > -999) _mchist[i]->Scale(_luminosity_fb);

    if (_mcscale[i] > 0) _mchist[i]->Scale(_mcscale[i]);

    if (ngroup > 0) _mchist[i]->Rebin(ngroup);

    mcstack->Add(_mchist[i]);
  }


  // Get the best score
  //----------------------------------------------------------------------------
  TH1D* backgroundhist = (TH1D*)(mcstack->GetStack()->Last());

  return GetBestScoreX(_signalhist[0], backgroundhist, fom);
}


//------------------------------------------------------------------------------
// WriteYields
//------------------------------------------------------------------------------
void HistogramReader::WriteYields(TH1*    hist,
				  TString label)
{
  if (!_writeyields) return;

  TString hname = hist->GetName();

  UInt_t nbins = hist->GetNbinsX();

  TAxis* axis = (TAxis*)hist->GetXaxis();

  Int_t firstBin = (_xmin > -999) ? axis->FindBin(_xmin) : 1;
  Int_t lastBin  = (_xmax > -999) ? axis->FindBin(_xmax) : nbins;

  if (firstBin < 1)     firstBin = 1;
  if (lastBin  > nbins) lastBin  = nbins;

  if (_xmax > -999 && hist->GetBinLowEdge(lastBin) >= _xmax) lastBin -= 1;

  if (_writelabels)
    {
      _writelabels = false;

      _yields_table << Form("\n %15s", " ");

      for (int i=firstBin; i<=lastBin; i++) {

	TString binlabel = (TString)hist->GetXaxis()->GetBinLabel(i);

	if (!hname.Contains("evolution")) binlabel = Form("%d", i);
	    
	_yields_table << Form(" | %-24s", binlabel.Data());
      }

      _yields_table << Form("\n");
    }

  _yields_table << Form(" %15s", label.Data());

  for (int i=firstBin; i<=lastBin; i++) {

    float process_yield = hist->GetBinContent(i);
    float process_error = sqrt(hist->GetSumw2()->At(i));

    if (label.EqualTo("data"))
      {
	_yields_table << Form(" | %8.0f %15s", process_yield, " ");
      }
    else
      {
	_yields_table << Form(" | %11.2f +/- %8.2f", process_yield, process_error);
      }
  }

  _yields_table << Form("\n");
}


//------------------------------------------------------------------------------
// Roc
//------------------------------------------------------------------------------
void HistogramReader::Roc(TString hname,
			  TString xtitle,
			  Int_t   npoints,
			  TString units,
			  Float_t xmin,
			  Float_t xmax,
			  TString fom)
{
  // Get the signal
  //----------------------------------------------------------------------------
  THStack* stack_sig = new THStack(hname + "_stack_sig", hname + "_stack_sig");

  for (int i=0; i<_roc_signalfile.size(); ++i)
    {
      _roc_signalfile[i]->cd();

      TH1D* dummy = (TH1D*)(_roc_signalfile[i]->Get(hname))->Clone();

      if (_luminosity_fb > 0 && _roc_signalscale[i] > -999) dummy->Scale(_luminosity_fb);
      
      stack_sig->Add(dummy);
    }

  TH1D* hSig = (TH1D*)(stack_sig->GetStack()->Last());


  // Get the backgrounds
  //----------------------------------------------------------------------------
  THStack* stack_bkg = new THStack(hname + "_stack_bkg", hname + "_stack_bkg");

  for (int j=0; j<_roc_backgroundfile.size(); ++j)
    {
      _roc_backgroundfile[j]->cd();

      TH1D* dummy = (TH1D*)(_roc_backgroundfile[j]->Get(hname))->Clone();

      if (_luminosity_fb > 0 && _roc_backgroundscale[j] > -999) dummy->Scale(_luminosity_fb);

      stack_bkg->Add(dummy);
    }

  TH1D* hBkg = (TH1D*)(stack_bkg->GetStack()->Last());


  // For The Punzi Effect
  // http://arxiv.org/pdf/physics/0308063v2.pdf
  Float_t a = 5.;
  Float_t b = 1.645;  // Corresponds to a p-value equal to 0.05


  // Compute ROC and significance
  //----------------------------------------------------------------------------
  float step = (xmax - xmin) / npoints;

  TGraph* rocGraph_min = new TGraph();
  TGraph* rocGraph_max = new TGraph();
  TGraph* sigGraph_min = new TGraph();
  TGraph* sigGraph_max = new TGraph();

  Float_t score_value_min = 0;
  Float_t score_value_max = 0;
  Float_t score_x_min     = 0;
  Float_t score_x_max     = 0;

  Float_t sigEff_score_x_min = -999;
  Float_t bkgEff_score_x_min = -999;
  Float_t sigEff_score_x_max = -999;
  Float_t bkgEff_score_x_max = -999;

  Float_t sigTotal = hSig->Integral(-1, -1);
  Float_t bkgTotal = hBkg->Integral(-1, -1);

  for (int s=0; s<=npoints; ++s) {

    Float_t sigYield_min = 0;
    Float_t sigYield_max = 0;
    Float_t bkgYield_min = 0;
    Float_t bkgYield_max = 0;

    sigYield_max += hSig->Integral(-1, hSig->FindBin(xmin + s*step));
    bkgYield_max += hBkg->Integral(-1, hBkg->FindBin(xmin + s*step));

    sigYield_min += hSig->Integral(hSig->FindBin(xmin + s*step), -1);
    bkgYield_min += hBkg->Integral(hBkg->FindBin(xmin + s*step), -1);

    Float_t sigEff_max = (sigTotal != 0) ? sigYield_max / sigTotal : -999;
    Float_t bkgEff_max = (bkgTotal != 0) ? bkgYield_max / bkgTotal : -999;

    Float_t sigEff_min = (sigTotal != 0) ? sigYield_min / sigTotal : -999;
    Float_t bkgEff_min = (bkgTotal != 0) ? bkgYield_min / bkgTotal : -999;

    Float_t score_min = -999;

    if (sigYield_min > 0. && bkgYield_min > 0.)
      {
        if (fom.EqualTo("S / #sqrt{B}"))   score_min = sigYield_min / sqrt(bkgYield_min);  
        if (fom.EqualTo("S / #sqrt{S+B}")) score_min = sigYield_min / sqrt(bkgYield_min + sigYield_min);
        if (fom.EqualTo("S / B"))          score_min = sigYield_min / bkgYield_min;
        if (fom.EqualTo("Punzi Eq.6"))     score_min =   sigEff_min / (b*b + 2*a*sqrt(bkgYield_min) + b*sqrt(b*b + 4*a*sqrt(b) + 4*bkgYield_min));
        if (fom.EqualTo("Punzi Eq.7"))     score_min =   sigEff_min / (a/2 + sqrt(bkgYield_min));
      }

    Float_t score_max = -999;

    if (sigYield_max > 0. && bkgYield_max > 0.)
      {
        if (fom.EqualTo("S / #sqrt{B}"))   score_max = sigYield_max / sqrt(bkgYield_max);
        if (fom.EqualTo("S / #sqrt{S+B}")) score_max = sigYield_max / sqrt(bkgYield_max + sigYield_max);
        if (fom.EqualTo("S / B"))          score_max = sigYield_max / bkgYield_max;
        if (fom.EqualTo("Punzi Eq.6"))     score_max =   sigEff_max / (b*b + 2*a*sqrt(bkgYield_max) + b*sqrt(b*b + 4*a*sqrt(b) + 4*bkgYield_max));
        if (fom.EqualTo("Punzi Eq.7"))     score_max =   sigEff_max / (a/2 + sqrt(bkgYield_max));
      }

    if (score_min > score_value_min) {
      score_value_min    = score_min;
      score_x_min        = xmin + s*step;
      sigEff_score_x_min = sigEff_min;
      bkgEff_score_x_min = bkgEff_min;
    }

    if (score_max > score_value_max) {
      score_value_max    = score_max;
      score_x_max        = xmin + s*step;
      sigEff_score_x_max = sigEff_max;
      bkgEff_score_x_max = bkgEff_max;
    }

    rocGraph_min->SetPoint(s, sigEff_min, 1 - bkgEff_min);
    rocGraph_max->SetPoint(s, sigEff_max, 1 - bkgEff_max);

    sigGraph_min->SetPoint(s, xmin + s*step, score_min);
    sigGraph_max->SetPoint(s, xmin + s*step, score_max);
  }


  printf("\n");
  printf(" [HistogramReader::Roc] Reading %s from %.2f to %.2f\n\n", hname.Data(), xmin, xmax);
  printf(" The best %s (%f) corresponds to x > %7.2f %s (S_eff = %6.2f\%, B_eff = %6.2f\%)\n",
	 fom.Data(),
	 score_value_min,
	 score_x_min,
	 units.Data(),
	 1e2 * sigEff_score_x_min,
	 1e2 * bkgEff_score_x_min);
  printf(" The best %s (%f) corresponds to x < %7.2f %s (S_eff = %6.2f\%, B_eff = %6.2f\%)\n",
	 fom.Data(),
	 score_value_max,
	 score_x_max,
	 units.Data(),
	 1e2 * sigEff_score_x_max,
	 1e2 * bkgEff_score_x_max);
  printf("\n");
  

  // Draw and save ROC
  //----------------------------------------------------------------------------
  Color_t color_min = kRed+1;
  Color_t color_max = kBlack;

  Style_t style_min = kFullCircle;
  Style_t style_max = kOpenCircle;

  TCanvas* rocCanvas = new TCanvas(hname + " ROC", hname + " ROC");

  rocGraph_min->SetMarkerColor(color_min);
  rocGraph_min->SetMarkerStyle(style_min);
  rocGraph_min->SetMarkerSize(0.5);

  rocGraph_max->SetMarkerColor(color_max);
  rocGraph_max->SetMarkerStyle(style_max);
  rocGraph_max->SetMarkerSize(0.5);

  rocGraph_min->Draw("ap");
  rocGraph_max->Draw("psame");

  rocGraph_min->GetXaxis()->SetRangeUser(0, 1);
  rocGraph_min->GetYaxis()->SetRangeUser(0, 1);

  DrawLatex(42, 0.190, 0.945, 0.050, 11, _title);

  SetAxis(rocGraph_min->GetHistogram(), xtitle + " signal efficiency", xtitle + " background rejection", 1.5, 1.8);

  if (_savepdf) rocCanvas->SaveAs(_outputdir + hname + "_ROC.pdf");
  if (_savepng) rocCanvas->SaveAs(_outputdir + hname + "_ROC.png");


  // Draw and save significance
  //----------------------------------------------------------------------------
  TCanvas *sigCanvas = new TCanvas(hname + " significance", hname + " significance");

  TString myxtitle = (!units.Contains("NULL")) ? xtitle + " [" + units + "]" : xtitle;

  sigGraph_min->SetMarkerColor(color_min);
  sigGraph_min->SetMarkerStyle(style_min);
  sigGraph_min->SetMarkerSize(0.5);

  sigGraph_max->SetMarkerColor(color_max);
  sigGraph_max->SetMarkerStyle(style_max);
  sigGraph_max->SetMarkerSize(0.5);

  sigGraph_min->Draw("ap");
  sigGraph_max->Draw("psame");

  Float_t ymax = (score_value_min > score_value_max) ? score_value_min : score_value_max;

  ymax *= 1.5;

  sigGraph_min->GetXaxis()->SetRangeUser(xmin, xmax);
  sigGraph_min->GetYaxis()->SetRangeUser(   0, ymax);

  DrawLatex(42, 0.190, 0.945, 0.050, 11, _title);

  TH1F* dummy_min = new TH1F("dummy_min", "", 1, 0, 1);
  TH1F* dummy_max = new TH1F("dummy_max", "", 1, 0, 1);

  dummy_min->SetLineColor  (color_min);
  dummy_min->SetMarkerColor(color_min);
  dummy_min->SetMarkerStyle(style_min);

  dummy_max->SetLineColor  (color_max);
  dummy_max->SetMarkerColor(color_max);
  dummy_max->SetMarkerStyle(style_max);

  DrawLegend(0.22, 0.84, dummy_min, Form("%s > x", xtitle.Data()), "lp", false);
  DrawLegend(0.22, 0.77, dummy_max, Form("%s < x", xtitle.Data()), "lp", false);

  SetAxis(sigGraph_min->GetHistogram(), myxtitle, fom);

  if (_savepdf) sigCanvas->SaveAs(_outputdir + hname + "_significance.pdf");
  if (_savepng) sigCanvas->SaveAs(_outputdir + hname + "_significance.png");

  dummy_min->Delete();
  dummy_max->Delete();
}


//------------------------------------------------------------------------------
// IncludeSystematics
//------------------------------------------------------------------------------
void HistogramReader::IncludeSystematics(TString hname)
{
  int nbins = _mchist[0]->GetNbinsX();

  int nsystematics = _systematics.size(); 


  // Loop over all processes
  //----------------------------------------------------------------------------
  for (int i=0; i<_mchist.size(); i++) {

    TH1D* myhisto = (TH1D*)_mchist[0]->Clone("myhisto");

    float suma[nbins+1]; 

    for (int k=0; k<=nbins; k++) suma[k] = 0;


    // Data-driven systematics
    //--------------------------------------------------------------------------
    if (_mcfilename.at(i) == "04_TTTo2L2Nu" ||
	_mcfilename.at(i) == "07_ZJets"     ||
	_mcfilename.at(i) == "00_Fakes_Full2016")
      {
	// Loop over all bins
	//----------------------------------------------------------------------
	for (int k=0; k<=nbins; k++) {

	  float diff = _mchist[i]->GetBinContent(k);

	  if (_mcfilename.at(i) == "04_TTTo2L2Nu")      diff *= 0.073; 
	  if (_mcfilename.at(i) == "07_ZJets")          diff *= 0.04;
	  if (_mcfilename.at(i) == "00_Fakes_Full2016") diff *= 0.30;

	  suma[k] += diff;
	}
      }


    // Other systematics
    //--------------------------------------------------------------------------
    else
      {
    	for (int j=0; j<nsystematics; j++) {

	  if (j%2 != 0) continue;

	  TFile* myfile1;
	  TFile* myfile2;

	  if (_minitreebased)
	    {
	      myfile1 = new TFile(_inputdir + "/" + _mcfilename.at(i) + _systematics.at(j)   + ".root", "read");
	      myfile2 = new TFile(_inputdir + "/" + _mcfilename.at(i) + _systematics.at(j+1) + ".root", "read");
	    }
	  else
	    {
	      myfile1 = new TFile(_systematics.at(j)   + "/" + _mcfilename.at(i) + ".root", "read");
	      myfile2 = new TFile(_systematics.at(j+1) + "/" + _mcfilename.at(i) + ".root", "read");
	    }
	      

	  TH1D* dummy1 = (TH1D*)myfile1->Get(hname);
	  TH1D* dummy2 = (TH1D*)myfile2->Get(hname);

	  if (_luminosity_fb > 0 && _mcscale[i] > -999)
	    {
	      dummy1->Scale(_luminosity_fb);
	      dummy2->Scale(_luminosity_fb);
	    }
		
	  if (_mcscale[i] > 0)
	    {
	      dummy1->Scale(_mcscale[i]);
	      dummy2->Scale(_mcscale[i]);
	    }


	  // Loop over all bins
	  //--------------------------------------------------------------------
	  for (int k=0; k<=nbins; k++) {

	    float diff = (dummy1->GetBinContent(k) - dummy2->GetBinContent(k)) / 2.;  // (up-down) / 2.
	    
	    suma[k] += diff;
	  }

	  myfile1->Close();
	  myfile2->Close();
	}
      }
   
  
    // Save the sum of systematic uncertainties per bin
    //--------------------------------------------------------------------------
    for (int k=0; k<=nbins; k++) myhisto->SetBinContent(k, suma[k]);
     
    _mchist_syst.push_back(myhisto);
  }
}
