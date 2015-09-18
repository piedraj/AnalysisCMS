#include "DrawFunctions.h"


#include <fstream>
#include <iostream>
#include <vector>


// Data members
//------------------------------------------------------------------------------
const UInt_t nchannel = 5;

enum {
  eee,
  eem,
  emm,
  mmm,
  all
};

TString schannel[nchannel] = {
  "eee",
  "eem",
  "emm",
  "mmm",
  "all"
};

TString lchannel[nchannel] = {
  "eee",
  "ee#mu",
  "e#mu#mu",
  "#mu#mu#mu",
  "all"
};


const UInt_t ncut = 5;

enum {
  Exactly3Leptons,
  HasZ,
  HasW,
  OneBJet,
  NoBJets
};

TString scut[ncut] = {
  "Exactly3Leptons",
  "HasZ",
  "HasW",
  "OneBJet",
  "NoBJets"
};


const UInt_t nprocess = 6;

enum {
  Data,
  WZ,
  ZZ,
  Top,
  ZJets,
  VVV
};

Color_t cprocess[nprocess];
TFile*  input   [nprocess];
TString sprocess[nprocess];


enum {linY, logY};


// Settings
//------------------------------------------------------------------------------
Double_t        _luminosity = 16.09; // pb
TString         _datapath   = "rootfiles";
Bool_t          _batch;
UInt_t          _cut;

vector<UInt_t>   vprocess;


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            ();

Int_t    ReadInputFiles           ();

void     DrawHistogram            (TString       hname,
				   UInt_t        channel,
				   UInt_t        cut,
				   TString       xtitle,
				   Int_t         ngroup       = -1,
				   Int_t         precision    =  0,
				   TString       units        = "NULL",
				   Bool_t        setLogy      = false,
				   Double_t      xmin         = -999,
				   Double_t      xmax         = -999,
				   Double_t      ymin         = -999,
				   Double_t      ymax         = -999,
				   Bool_t        moveOverflow = false);

Double_t Yield                    (TH1*          h);

void     MakeOutputDirectory      (TString       format);


//------------------------------------------------------------------------------
// draw
//------------------------------------------------------------------------------
void draw(UInt_t cut   = Exactly3Leptons,
	  Bool_t batch = kTRUE)
{
  _cut   = cut;
  _batch = batch;

  SetParameters();

  if (ReadInputFiles() < 0) return;

  for (UInt_t channel=0; channel<nchannel; channel++) {

    DrawHistogram("h_counter_lum", channel, cut, "yield",            -1, 0, "NULL", linY);
    DrawHistogram("h_invMass2Lep", channel, cut, "m_{#font[12]{ll}}", 4, 0, "GeV",  linY, 60, 120);
  }
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   UInt_t   channel,
		   UInt_t   cut,
		   TString  xtitle,
		   Int_t    ngroup,
		   Int_t    precision,
		   TString  units,
		   Bool_t   setLogy,
		   Double_t xmin,
		   Double_t xmax,
		   Double_t ymin,
		   Double_t ymax,
		   Bool_t   moveOverflow)
{
  hname += "_" + schannel[channel] + "_" + scut[cut];

  TCanvas* canvas = new TCanvas(hname, hname, 550, 720);

  TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);

  pad1->SetTopMargin   (0.08);
  pad1->SetBottomMargin(0.02);
  pad1->Draw();

  pad2->SetTopMargin   (0.08);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(setLogy);

  THStack* hstack = new THStack(hname, hname);

  TH1D* hist[nprocess];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1D*)input[j]->Get(hname);

    hist[j]->SetName(hname + "_" + sprocess[j]);

    if (moveOverflow) MoveOverflowBins(hist[j], xmin, xmax);

    if (ngroup > 0) hist[j]->Rebin(ngroup);

    if (j != Data)
      {
	hist[j]->SetFillColor(cprocess[j]);
	hist[j]->SetFillStyle(1001);
	hist[j]->SetLineColor(cprocess[j]);

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

  for (Int_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

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


  // Poisson errors for data
  //----------------------------------------------------------------------------
  TH1D* hdata = new TH1D(hname + "_data", "",
			 hist[Data]->GetNbinsX(),
			 hist[Data]->GetBinLowEdge(1),
			 hist[Data]->GetBinLowEdge(hist[Data]->GetNbinsX())+hist[Data]->GetBinWidth(1));

  hdata->SetBinErrorOption(TH1::kPoisson);  // Do not set Sumw2()

  hdata->SetLineColor  (cprocess[Data]);
  hdata->SetMarkerColor(cprocess[Data]);
  hdata->SetMarkerStyle(kFullCircle);
  hdata->SetTitle("");

  for (int ibin=1; ibin<=hist[Data]->GetNbinsX(); ++ibin)
    {
      for (int f=0; f<hist[Data]->GetBinContent(ibin); f++)
	{
	  hdata->Fill(hist[Data]->GetBinCenter(ibin));
	}
    }


  // Axis labels
  //----------------------------------------------------------------------------
  TAxis* xaxis = hdata->GetXaxis();
  TAxis* yaxis = hdata->GetYaxis();

  TString ytitle = Form("Events / %s.%df", "%", precision);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(Form(ytitle.Data(), hdata->GetBinWidth(0)));

  if (!units.Contains("NULL")) {
    
    xaxis->SetTitle(Form("%s [%s]", xaxis->GetTitle(), units.Data()));
    yaxis->SetTitle(Form("%s %s",   yaxis->GetTitle(), units.Data()));
  }


  // Draw
  //----------------------------------------------------------------------------
  xaxis->SetRangeUser(xmin, xmax);

  hdata ->Draw("ep");
  hstack->Draw("hist,same");
  allmc ->Draw("e2,same");
  hdata ->Draw("ep,same");


  // Adjust scale
  //----------------------------------------------------------------------------
  Double_t theMax   = GetMaximumIncludingErrors(hdata, xmin, xmax);
  Double_t theMaxMC = GetMaximumIncludingErrors(allmc, xmin, xmax);

  if (theMaxMC > theMax)
    {
      theMax = theMaxMC;
    }

  if (pad1->GetLogy())
    theMax = TMath::Power(10, TMath::Log10(theMax) + 2);
  else
    theMax *= 1.55;

  hdata->SetMinimum(0.0);
  hdata->SetMaximum(theMax);

  if (ymin != -999) hdata->SetMinimum(ymin);
  if (ymax != -999) hdata->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.760;
  Double_t y0     = 0.834;
  Double_t delta  = 0.048 + 0.001;
  Double_t ndelta = 0;
  Double_t xdelta = 0.535;

  TString sData = Form(" data (%.0f)", Yield(hdata));

  if (channel != all) sData = Form(" %s%s", lchannel[channel].Data(), sData.Data());

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hdata,    sData,                                "lp"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)allmc,    Form(" all (%.0f)", Yield(allmc)),    "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WZ], Form(" WZ (%.0f)",  Yield(hist[WZ])), "f");  ndelta += delta;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[Top],   Form(" top (%.0f)",    Yield(hist[Top])),   "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZJets], Form(" Z+jets (%.0f)", Yield(hist[ZJets])), "f"); ndelta += delta;

  ndelta = 0;
  xdelta = 0.22;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZZ],  Form(" ZZ (%.0f)",       Yield(hist[ZZ])),  "f"); ndelta += delta;
  //  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZG],  Form(" Z#gamma (%.0f)",  Yield(hist[ZG])),  "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[VVV], Form(" VVV (%.0f)",      Yield(hist[VVV])), "f"); ndelta += delta;
  //  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WV],  Form(" W#gamma* (%.0f)", Yield(hist[WV])),  "f"); ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(61, 0.190, 0.945, 0.050, 11, "CMS"); // 42
  DrawTLatex(52, 0.288, 0.945, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940, 0.945, 0.050, 31, Form("%.2f pb^{-1} (13TeV)", _luminosity));


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  pad2->cd();
    
  TH1D* ratio       = (TH1D*)hdata->Clone("ratio");
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


  // Save
  //----------------------------------------------------------------------------
  pad2->cd(); SetAxis(ratio, hdata->GetXaxis()->GetTitle(), "data / MC - 1", 0.105, 0.75);
  pad1->cd(); SetAxis(hdata, "", hdata->GetYaxis()->GetTitle(), 0.045, 1.7);

  canvas->cd();

  if (_batch)
    {
      TString cname = scut[_cut] + "/" + hname;

      if (setLogy) cname += "_log";

      canvas->SaveAs(Form("pdf/%s.pdf", cname.Data()));
      canvas->SaveAs(Form("png/%s.png", cname.Data()));
    }
}


//------------------------------------------------------------------------------
// SetParameters
//------------------------------------------------------------------------------
void SetParameters()
{
  if (_batch) gROOT->SetBatch();

  sprocess[Data]  = "WZTo3LNu";
  sprocess[WZ]    = "WZTo3LNu";
  sprocess[ZZ]    = "ggZZ";
  sprocess[Top]   = "TTTo2L2Nu";
  sprocess[ZJets] = "DYJetsToLL_M-50";
  sprocess[VVV]   = "TTZToLLNuNu_M-10";

  cprocess[Data]  = kBlack;
  //  cprocess[Fakes] = kAzure-7;
  cprocess[WZ]    = kOrange-2;
  cprocess[ZZ]    = kRed+3;
  //  cprocess[ZG]    = kGreen-5;
  cprocess[Top]   = kYellow;
  cprocess[ZJets] = kGreen+2;
  cprocess[VVV]   = kAzure-9;
  //  cprocess[WV]    = kGray+1;

  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  vprocess.clear();

  vprocess.push_back(Data);
  //  vprocess.push_back(WV);
  vprocess.push_back(VVV);
  vprocess.push_back(Top);
  vprocess.push_back(ZJets);
  vprocess.push_back(ZZ);
  //  vprocess.push_back(ZG);
  vprocess.push_back(WZ);
}


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles()
{
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TString fname = _datapath + "/" + sprocess[j] + ".root";

    input[j] = new TFile(fname);

    TH1D* dummy = (TH1D*)input[j]->Get("h_counter_raw_all_" + scut[_cut]);

    if (!dummy)
      {
	printf(" [ReadInputFiles] The %s file is broken or it does not exist.\n",
	       sprocess[j].Data());
	
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
  gSystem->mkdir(format, kTRUE);

  gSystem->mkdir(format + "/" + scut[_cut], kTRUE);
}
