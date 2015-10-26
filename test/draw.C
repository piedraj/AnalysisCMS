#include "DrawFunctions.h"
#include "../Constants.h"


#include <fstream>
#include <iostream>
#include <vector>


// Data members
//------------------------------------------------------------------------------
const UInt_t nprocess = 10;

enum {
  Data,
  WZ,
  ZZ,
  Top,
  SingleTop,
  WW,
  ZJets,
  WJets,
  TTW,
  TTZ
};

Color_t cprocess[nprocess];
TFile*  input   [nprocess];
TString sprocess[nprocess];


enum {linY, logY};


// Settings
//------------------------------------------------------------------------------
Bool_t          _savepdf  = kFALSE;
Bool_t          _savepng  = kTRUE;
TString         _datapath = "../rootfiles";
TString         _era      = "50ns";
Bool_t          _batch;
Double_t        _luminosity;
Int_t           _cut;
Int_t           _jetbin;
UInt_t          _firstchannel;
UInt_t          _lastchannel;
TString         _analysis;
TString         _directory;
TString         _sjetbin;

vector<UInt_t>   vprocess;


// Member functions
//------------------------------------------------------------------------------
void     PrintHelp                ();

Int_t    SetParameters            (Int_t         cut,
				   Int_t         jetbin);

Int_t    ReadInputFiles           ();

void     DrawHistogram            (TString       hname,
				   TString       xtitle,
				   Int_t         ngroup       = -1,
				   Int_t         precision    =  0,
				   TString       units        = "NULL",
				   Bool_t        drawratio    = false,
				   Bool_t        setlogy      = false,
				   Bool_t        moveoverflow = true,
				   Double_t      xmin         = -999,
				   Double_t      xmax         = -999,
				   Double_t      ymin         = -999,
				   Double_t      ymax         = -999);

void     MakeOutputDirectory      (TString       format);

TString  GetName                  (TString       prefix,
				   UInt_t        channel);


//------------------------------------------------------------------------------
// draw
//------------------------------------------------------------------------------
void draw(Int_t  cut       = -1,
	  Int_t  jetbin    = njetbin,
	  Bool_t drawratio = false)
{
  if (SetParameters(cut, jetbin) < 0) return;


  // Loop over channels
  //----------------------------------------------------------------------------
  for (UInt_t channel=_firstchannel; channel<=_lastchannel; channel++) {

    if (!_batch && channel != _lastchannel) continue;

    if (_analysis.EqualTo("WW"))
      {
	DrawHistogram(GetName("h_m2l", channel), "m_{#font[12]{ll}}", 8, 0, "GeV", drawratio, logY);
      }
    else
      {
	DrawHistogram(GetName("h_m2l", channel), "m_{#font[12]{ll}}", 4, 0, "GeV", drawratio, linY, true, 60, 120);
	DrawHistogram(GetName("h_m3l", channel), "m_{#font[12]{3l}}", 5, 0, "GeV", drawratio, linY, true, 60, 120);
      }

    DrawHistogram(GetName("h_counterLum", channel), "yield",                                   -1, 0, "NULL", drawratio, linY);
    DrawHistogram(GetName("h_pfType1Met", channel), "E_{T}^{miss}",                             5, 0, "GeV",  drawratio, linY);
    DrawHistogram(GetName("h_ht",         channel), "H_{T}",                                    5, 0, "GeV",  drawratio, linY);
    DrawHistogram(GetName("h_nvtx",       channel), "number of vertices",                      -1, 0, "NULL", drawratio, linY);
    DrawHistogram(GetName("h_njet",       channel), "number of jets (p_{T}^{jet} > 30 GeV)",   -1, 0, "NULL", drawratio, logY);
    DrawHistogram(GetName("h_nbjet",      channel), "number of b-jets (p_{T}^{jet} > 30 GeV)", -1, 0, "NULL", drawratio, logY);
  }
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   TString  xtitle,
		   Int_t    ngroup,
		   Int_t    precision,
		   TString  units,
		   Bool_t   drawratio,
		   Bool_t   setlogy,
		   Bool_t   moveoverflow,
		   Double_t xmin,
		   Double_t xmax,
		   Double_t ymin,
		   Double_t ymax)
{
  TCanvas* canvas = NULL;

  TPad* pad1 = NULL;
  TPad* pad2 = NULL;

  if (drawratio)
    {
      canvas = new TCanvas(hname, hname, 550, 720);

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
      canvas = new TCanvas(hname, hname);

      pad1 = new TPad("pad1", "pad1", 0, 0, 1, 1);
      pad1->Draw();
    }


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(setlogy);

  THStack* hstack = new THStack(hname, hname);

  TH1D* hist[nprocess];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1D*)input[j]->Get(hname);

    if (xmin == -999) xmin = hist[j]->GetXaxis()->GetXmin();
    if (xmax == -999) xmax = hist[j]->GetXaxis()->GetXmax();

    if (ngroup > 0) hist[j]->Rebin(ngroup);

    if (moveoverflow) MoveOverflowBins(hist[j], xmin, xmax);

    hist[j]->SetFillColor(cprocess[j]);
    hist[j]->SetLineColor(cprocess[j]);

    if (j == Data)
      {
	hist[j]->SetMarkerStyle(kFullCircle);
	hist[j]->SetTitle("");
      }
    else
      {
	hist[j]->SetFillStyle(1001);
	hstack->Add(hist[j]);
      }
  }


  // All MC
  //----------------------------------------------------------------------------
  TH1D* allmc = (TH1D*)hist[Data]->Clone("allmc");

  allmc->SetFillColor  (kGray+1);
  allmc->SetFillStyle  (   3345);
  allmc->SetLineColor  (kGray+1);
  allmc->SetMarkerColor(kGray+1);
  allmc->SetMarkerSize (      0);

  for (Int_t ibin=0; ibin<=allmc->GetNbinsX()+1; ibin++) {

    Double_t binValue = 0.;
    Double_t binError = 0.;

    for (UInt_t i=0; i<vprocess.size(); i++) {

      UInt_t j = vprocess.at(i);

      if (j == Data) continue;

      Double_t binContent   = hist[j]->GetBinContent(ibin);
      Double_t binStatError = hist[j]->GetBinError(ibin);
      Double_t binSystError = 0;  // To be updated

      binValue += binContent;
      binError += (binStatError * binStatError);
      binError += (binSystError * binSystError);
    }
    
    binError = sqrt(binError);

    allmc->SetBinContent(ibin, binValue);
    allmc->SetBinError  (ibin, binError);
  }


  // Draw
  //----------------------------------------------------------------------------
  hist[Data]->Draw("ep");
  hstack    ->Draw("hist,same");
  allmc     ->Draw("e2,same");
  hist[Data]->Draw("ep,same");


  // Set xtitle and ytitle
  //----------------------------------------------------------------------------
  TString ytitle = Form("events / %s.%df", "%", precision);

  ytitle = Form(ytitle.Data(), hist[Data]->GetBinWidth(0));

  if (!units.Contains("NULL")) {
    xtitle = Form("%s [%s]", xtitle.Data(), units.Data());
    ytitle = Form("%s %s",   ytitle.Data(), units.Data());
  }


  // Adjust xaxis and yaxis
  //----------------------------------------------------------------------------
  hist[Data]->GetXaxis()->SetRangeUser(xmin, xmax);

  Double_t theMin   = 0.0;
  Double_t theMax   = GetMaximumIncludingErrors(hist[Data], xmin, xmax);
  Double_t theMaxMC = GetMaximumIncludingErrors(allmc,      xmin, xmax);

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (pad1->GetLogy())
    {
      theMin = 1e-1;
      theMax = 2. * TMath::Power(10, TMath::Log10(theMax) + 2);
    }
  else
    {
      theMax *= 1.45;
    }

  hist[Data]->SetMinimum(theMin);
  hist[Data]->SetMaximum(theMax);

  if (ymin != -999) hist[Data]->SetMinimum(ymin);
  if (ymax != -999) hist[Data]->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.760;
  Double_t y0     = 0.834;
  Double_t delta  = 0.048 + 0.001;
  Double_t ndelta = 0;
  Double_t xdelta = 0.535;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[Data], Form(" data (%.0f)", Yield(hist[Data])), "lp"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)allmc,      Form(" all (%.0f)",  Yield(allmc)),      "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WZ],   Form(" WZ (%.0f)",   Yield(hist[WZ])),   "f");  ndelta += delta;

  ndelta = 0;
  xdelta = 0.31;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[Top],       Form(" tt+jets (%.0f)",    Yield(hist[Top])),       "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[SingleTop], Form(" single top (%.0f)", Yield(hist[SingleTop])), "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZJets],     Form(" Z+jets (%.0f)",     Yield(hist[ZJets])),     "f");  ndelta += delta;

  ndelta = 0;
  xdelta = 0.05;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZZ],    Form(" ZZ (%.0f)",     Yield(hist[ZZ])),    "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WW],    Form(" WW (%.0f)",     Yield(hist[WW])),    "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WJets], Form(" W+jets (%.0f)", Yield(hist[WJets])), "f"); ndelta += delta;

  if (_era.EqualTo("25ns"))
    {
      DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[TTW], Form(" ttW (%.0f)", Yield(hist[TTW])), "f"); ndelta += delta;
      DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[TTZ], Form(" ttZ (%.0f)", Yield(hist[TTZ])), "f"); ndelta += delta;
    }


  // Titles
  //----------------------------------------------------------------------------
  Double_t xprelim = (drawratio) ? 0.288 : 0.300;

  DrawTLatex(61, 0.190,   0.945, 0.050, 11, "CMS");
  DrawTLatex(52, xprelim, 0.945, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940,   0.945, 0.050, 31, Form("%.2f pb^{-1} (13TeV)", 1e3*_luminosity));

  SetAxis(hist[Data], xtitle, ytitle, 0.045, 1.5, 1.7);


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  if (drawratio)
    {
      pad2->cd();
    
      TH1D* ratio       = (TH1D*)hist[Data]->Clone("ratio");
      TH1D* uncertainty = (TH1D*)allmc->Clone("uncertainty");

      for (Int_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {

	Double_t mcValue = allmc->GetBinContent(ibin);
	Double_t mcError = allmc->GetBinError  (ibin);
    
	Double_t dtValue = ratio->GetBinContent(ibin);
	Double_t dtError = ratio->GetBinError  (ibin);

	Double_t ratioVal         = 0.0;
	Double_t ratioErr         = 1e-9;
	Double_t uncertaintyError = 1e-9;

	if (mcValue > 0 && dtValue > 0)
	  {
	    ratioVal         = dtValue / mcValue - 1.0;
	    ratioErr         = dtError / mcValue;
	    uncertaintyError = ratioVal * mcError / mcValue;
	  }
	
	ratio->SetBinContent(ibin, ratioVal);
	ratio->SetBinError  (ibin, ratioErr);
	
	uncertainty->SetBinContent(ibin, 0.0);
	uncertainty->SetBinError  (ibin, uncertaintyError);
      }

      ratio->Draw("ep");

      ratio->GetYaxis()->SetRangeUser(-2, 2);

      uncertainty->Draw("e2,same");

      ratio->Draw("ep,same");

      SetAxis(ratio, xtitle, "data / MC - 1", 0.105, 1.4, 0.75);
    }


  //----------------------------------------------------------------------------
  // Save it
  //----------------------------------------------------------------------------
  canvas->cd();

  if (_batch)
    {
      TString cname = _analysis + "/" + _era + "/" + hname;

      if (setlogy) cname += "_log";

      if (_savepdf) canvas->SaveAs(Form("pdf/%s.pdf", cname.Data()));
      if (_savepng) canvas->SaveAs(Form("png/%s.png", cname.Data()));
    }
}


//------------------------------------------------------------------------------
// SetParameters
//------------------------------------------------------------------------------
Int_t SetParameters(Int_t cut, Int_t jetbin)
{
  if (cut < 0)
    {
      PrintHelp();
      return -1;
    }

  _cut          = cut;
  _jetbin       = jetbin;
  _batch        = gROOT->IsBatch();
  _luminosity   = (_era.EqualTo("50ns")) ? lumi50ns_fb : lumi25ns_fb;
  _analysis     = (_cut < WZ00_Exactly3Leptons) ? "WW" : "WZ";
  _sjetbin      = (_jetbin < njetbin) ? Form("/%djet", _jetbin) : "";
  _directory    = scut[_cut] + _sjetbin + "/";
  _firstchannel = (_analysis.EqualTo("WZ")) ? eee : ee;
  _lastchannel  = (_analysis.EqualTo("WZ")) ? lll : ll;

  if (_batch)
    {
      if (_savepdf) MakeOutputDirectory("pdf");
      if (_savepng) MakeOutputDirectory("png");
    }

  sprocess[Data]      = "01_Data";
  sprocess[WZ]        = "02_WZ";
  sprocess[ZZ]        = "03_ZZ";
  sprocess[Top]       = "04_Top";
  sprocess[SingleTop] = "05_SingleTop";
  sprocess[WW]        = "06_WW";
  sprocess[ZJets]     = "07_ZJets";
  sprocess[WJets]     = "08_WJets";
  sprocess[TTW]       = "09_TTW";
  sprocess[TTZ]       = "10_TTZ";

  cprocess[Data]      = kBlack;
  cprocess[WZ]        = kOrange-2;
  cprocess[ZZ]        = kRed+3;
  cprocess[Top]       = kYellow;
  cprocess[SingleTop] = kYellow-6;
  cprocess[WW]        = kAzure-7;
  cprocess[ZJets]     = kGreen+2;
  cprocess[WJets]     = kAzure-9;
  cprocess[TTW]       = kAzure-9;  // Temporary
  cprocess[TTZ]       = kAzure-9;  // Temporary

  vprocess.clear();
  vprocess.push_back(Data);
  vprocess.push_back(WJets);
  vprocess.push_back(WW);
  vprocess.push_back(SingleTop);
  vprocess.push_back(Top);
  vprocess.push_back(ZZ);
  vprocess.push_back(ZJets);
  vprocess.push_back(WZ);

  if (_era.EqualTo("25ns"))
    {
      vprocess.push_back(TTW);
      vprocess.push_back(TTZ);
    }

  if (ReadInputFiles() < 0) return -1;

  return 0;
}


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles()
{
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TString fname = _datapath + "/" + _era + "/" + sprocess[j] + ".root";

    TString hname = _directory + "h_counterRaw_" + schannel[_lastchannel];

    input[j] = new TFile(fname);

    TH1D* dummy = (TH1D*)input[j]->Get(hname);

    if (!dummy)
      {
	printf("\n");
	printf(" [ReadInputFiles] Something went wrong reading a test histogram.\n\n");
	printf(" \t Option1: the problem is in the input file %s.\n", fname.Data());
	printf(" \t Option2: the problem is in the test histogram %s.\n", hname.Data());
	printf("\n");
	
	return -1;
      }
  }

  return 0;
}


//------------------------------------------------------------------------------
// MakeOutputDirectory
//------------------------------------------------------------------------------
void MakeOutputDirectory(TString format)
{
  gSystem->mkdir(format + "/" + _analysis + "/" + _era + "/" + _directory, kTRUE);

  TString path = format;

  gSystem->Exec(Form("cp index.php %s/.", path.Data()));

  path += "/" + _analysis;

  gSystem->Exec(Form("cp index.php %s/.", path.Data()));

  path += "/" + _era;

  gSystem->Exec(Form("cp index.php %s/.", path.Data()));

  path += "/" + scut[_cut];

  gSystem->Exec(Form("cp index.php %s/.", path.Data()));

  path += _sjetbin;

  gSystem->Exec(Form("cp index.php %s/.", path.Data()));
}


//------------------------------------------------------------------------------
// PrintHelp
//------------------------------------------------------------------------------
void PrintHelp()
{
  printf("\n");
      
  if (_savepdf) printf(" rm -rf pdf\n");
  if (_savepng) printf(" rm -rf png\n");

  if (_savepdf || _savepng) printf("\n");

  for (UInt_t i=0; i<WZ00_Exactly3Leptons; i++)
    for (UInt_t j=0; j<=njetbin; j++) {

      TString sj = (j < njetbin) ? Form(",%d", j) : "";

      printf(" root -l -b -q \'draw.C+(%s%s)\'\n", scut[i].Data(), sj.Data());
    }
     
  printf("\n");

  for (UInt_t i=WZ00_Exactly3Leptons; i<ncut; i++)
    printf(" root -l -b -q \'draw.C+(%s)\'\n", scut[i].Data());

  printf("\n");
}


//------------------------------------------------------------------------------
// GetName
//------------------------------------------------------------------------------
TString GetName(TString prefix, UInt_t channel)
{
  TString hname = _directory + prefix + "_" + schannel[channel];

  return hname;
}
