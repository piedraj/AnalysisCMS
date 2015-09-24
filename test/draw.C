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
  WZ00_Exactly3Leptons,
  WZ01_HasZ,
  WZ02_HasW,
  WZ03_OneBJet,
  WZ04_NoBJets
};

TString scut[ncut] = {
  "WZ00_Exactly3Leptons",
  "WZ01_HasZ",
  "WZ02_HasW",
  "WZ03_OneBJet",
  "WZ04_NoBJets"
};


const UInt_t nprocess = 9;

enum {
  Data,
  WZ,
  ZZ,
  Top,
  SingleTop,
  WW,
  ZJets,
  WJets,
  QCD
};

Color_t cprocess[nprocess];
TFile*  input   [nprocess];
TString sprocess[nprocess];


enum {linY, logY};


// Settings
//------------------------------------------------------------------------------
Double_t        _luminosity = 40.03; // pb
TString         _datapath   = "../rootfiles";
TString         _era        = "50ns";
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

void     MakeOutputDirectory      (TString       format);


//------------------------------------------------------------------------------
// draw
//------------------------------------------------------------------------------
void draw(UInt_t cut   = WZ00_Exactly3Leptons,
	  Bool_t batch = kTRUE)
{
  _cut   = cut;
  _batch = batch;

  SetParameters();

  if (ReadInputFiles() < 0) return;

  for (UInt_t channel=0; channel<nchannel; channel++) {

    if (channel != all) continue;

    DrawHistogram("h_counter_lum", channel, cut, "yield",                                   -1, 0, "NULL", linY);
    DrawHistogram("h_pfType1Met",  channel, cut, "E_{T}^{miss}",                             5, 0, "GeV",  linY);
    DrawHistogram("h_nvtx",        channel, cut, "number of vertices",                      -1, 0, "NULL", linY, 0, 40);
    DrawHistogram("h_njet",        channel, cut, "number of jets (p_{T}^{jet} > 30 GeV)",   -1, 0, "NULL", linY, 0, 4);
    DrawHistogram("h_nbjet",       channel, cut, "number of b-jets (p_{T}^{jet} > 30 GeV)", -1, 0, "NULL", linY, 0, 4);
    DrawHistogram("h_m2l",         channel, cut, "m_{#font[12]{ll}}",                        4, 0, "GeV",  linY, 60, 120);
    DrawHistogram("h_m3l",         channel, cut, "m_{#font[12]{3l}}",                        5, 0, "GeV",  linY, 60, 350);
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

  TH1F* hist[nprocess];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1F*)input[j]->Get(hname);

    hist[j]->SetName(hname + "_" + sprocess[j]);

    if (moveOverflow) MoveOverflowBins(hist[j], xmin, xmax);

    if (ngroup > 0) hist[j]->Rebin(ngroup);

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
  TH1F* allmc = (TH1F*)hist[Data]->Clone("allmc");

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


  // Adjust axes
  //----------------------------------------------------------------------------
  TAxis* xaxis = hist[Data]->GetXaxis();
  TAxis* yaxis = hist[Data]->GetYaxis();

  xaxis->SetRangeUser(xmin, xmax);

  TString ytitle = Form("Events / %s.%df", "%", precision);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(Form(ytitle.Data(), hist[Data]->GetBinWidth(0)));

  if (!units.Contains("NULL")) {
    
    xaxis->SetTitle(Form("%s [%s]", xaxis->GetTitle(), units.Data()));
    yaxis->SetTitle(Form("%s %s",   yaxis->GetTitle(), units.Data()));
  }


  // Adjust scale
  //----------------------------------------------------------------------------
  Double_t theMax   = GetMaximumIncludingErrors(hist[Data], xmin, xmax);
  Double_t theMaxMC = GetMaximumIncludingErrors(allmc,      xmin, xmax);

  if (theMaxMC > theMax)
    {
      theMax = theMaxMC;
    }

  if (pad1->GetLogy())
    theMax = TMath::Power(10, TMath::Log10(theMax) + 2);
  else
    theMax *= 1.4;  // 1.55

  hist[Data]->SetMinimum(0.0);
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

  TString sData = Form(" data (%.0f)", Yield(hist[Data]));

  if (channel != all) sData = Form(" %s%s", lchannel[channel].Data(), sData.Data());

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[Data], sData,                                "lp"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)allmc,      Form(" all (%.0f)", Yield(allmc)),    "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WZ],   Form(" WZ (%.0f)",  Yield(hist[WZ])), "f");  ndelta += delta;

  ndelta = 0;
  xdelta = 0.31;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[Top],       Form(" top (%.0f)",        Yield(hist[Top])),       "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[SingleTop], Form(" single top (%.0f)", Yield(hist[SingleTop])), "f");  ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZJets],     Form(" Z+jets (%.0f)",     Yield(hist[ZJets])),     "f");  ndelta += delta;

  ndelta = 0;
  xdelta = 0.05;

  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[ZZ],    Form(" ZZ (%.0f)",     Yield(hist[ZZ])),    "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WW],    Form(" WW (%.0f)",     Yield(hist[WW])),    "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[WJets], Form(" W+jets (%.0f)", Yield(hist[WJets])), "f"); ndelta += delta;
  DrawLegend(x0 - xdelta, y0 - ndelta, (TObject*)hist[QCD],   Form(" QCD (%.0f)",    Yield(hist[QCD])),   "f"); ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(61, 0.190, 0.945, 0.050, 11, "CMS");
  DrawTLatex(52, 0.288, 0.945, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940, 0.945, 0.050, 31, Form("%.2f pb^{-1} (13TeV)", _luminosity));


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  pad2->cd();
    
  TH1F* ratio       = (TH1F*)hist[Data]->Clone("ratio");
  TH1F* uncertainty = (TH1F*)allmc->Clone("uncertainty");

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
  pad2->cd(); SetAxis(ratio, hist[Data]->GetXaxis()->GetTitle(), "data / MC - 1", 0.105, 0.75);
  pad1->cd(); SetAxis(hist[Data], "", hist[Data]->GetYaxis()->GetTitle(), 0.045, 1.7);

  canvas->cd();

  if (_batch)
    {
      TString cname = scut[_cut] + "/" + hname;

      if (setLogy) cname += "_log";

      canvas->SaveAs(Form("pdf/%s/%s.pdf", _era.Data(), cname.Data()));
      canvas->SaveAs(Form("png/%s/%s.png", _era.Data(), cname.Data()));
    }
}


//------------------------------------------------------------------------------
// SetParameters
//------------------------------------------------------------------------------
void SetParameters()
{
  if (_batch)
    {
      gROOT->SetBatch();
      MakeOutputDirectory("pdf");
      MakeOutputDirectory("png");
    }

  sprocess[Data]      = "01_Data";
  sprocess[WZ]        = "02_WZ";
  sprocess[ZZ]        = "03_ZZ";
  sprocess[Top]       = "04_Top";
  sprocess[SingleTop] = "05_SingleTop";
  sprocess[WW]        = "06_WW";
  sprocess[ZJets]     = "07_ZJets";
  sprocess[WJets]     = "08_WJets";
  sprocess[QCD]       = "09_QCD";

  cprocess[Data]      = kBlack;
  cprocess[WZ]        = kOrange-2;
  cprocess[ZZ]        = kRed+3;
  cprocess[Top]       = kYellow;
  cprocess[SingleTop] = kYellow-6;
  cprocess[WW]        = kAzure-7;
  cprocess[ZJets]     = kGreen+2;
  cprocess[WJets]     = kAzure-9;
  cprocess[QCD]       = kGray+1;

  vprocess.clear();
  vprocess.push_back(Data);
  vprocess.push_back(QCD);
  vprocess.push_back(WJets);
  vprocess.push_back(ZJets);
  vprocess.push_back(WW);
  vprocess.push_back(SingleTop);
  vprocess.push_back(Top);
  vprocess.push_back(ZZ);
  vprocess.push_back(WZ);
}


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles()
{
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TString fname = _datapath + "/" + _era + "/" + sprocess[j] + ".root";

    input[j] = new TFile(fname);

    TH1F* dummy = (TH1F*)input[j]->Get("h_counter_raw_all_" + scut[_cut]);

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
  gSystem->mkdir(format + "/" + _era + "/" + scut[_cut], kTRUE);

  gSystem->Exec(Form("cp index.php %s/.",       format.Data()));
  gSystem->Exec(Form("cp index.php %s/%s/.",    format.Data(), _era.Data()));
  gSystem->Exec(Form("cp index.php %s/%s/%s/.", format.Data(), _era.Data(), scut[_cut].Data()));
}
