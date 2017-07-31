
// Constants
//------------------------------------------------------------------------------
const Int_t   njetet = 7;

const Float_t muojetarray[njetet] = {10, 15, 20, 25, 30, 35, 45};
const Float_t elejetarray[njetet] = {10, 15, 20, 25, 30, 35, 45};

const Float_t muoscale = -1.;
const Float_t elescale = -1.;


// Data members
//------------------------------------------------------------------------------
bool    draw         = false;
bool    savepng      = true;
bool    setgrid      = true;
bool    Wsubtraction = true;
bool    Zsubtraction = true;

TFile*  dataFR;
TFile*  wjetsFR;
TFile*  zjetsFR;
TFile*  zjetsPR;

TString inputdir;
TString outputdir;


// Functions
//------------------------------------------------------------------------------
void     Cosmetics (TH1D*       hist,
		    Option_t*   option,
		    TString     xtitle,
		    TString     ytitle,
		    Color_t     color);

void     DrawFR    (TString     flavour,
		    TString     variable,
		    TString     xtitle,
		    Float_t     lepscale,
		    Float_t     jetet);

void     DrawPR    (TString     flavour,
		    TString     variable,
		    TString     xtitle);

void     WriteFR   (TString     flavour,
		    Float_t     lepscale,
		    Float_t     jetet);

void     WritePR   (TString     flavour);

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
		    Float_t     tsize   = 0.035,
		    Float_t     xoffset = 0.184,
		    Float_t     yoffset = 0.043);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l -b -q getFakeRate.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakeRate(TString inputdir_name  = "../rootfiles/nominal",
		 TString outputdir_name = "test")
{
  inputdir  = inputdir_name;
  outputdir = outputdir_name;

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  if (savepng) gSystem->mkdir("png", kTRUE);

  gSystem->mkdir(outputdir, kTRUE);

  dataFR  = new TFile (inputdir + "/FR/01_Data.root",  "read");
  wjetsFR = new TFile (inputdir + "/FR/08_WJets.root", "read");
  zjetsFR = new TFile (inputdir + "/FR/07_ZJets.root", "read");
  zjetsPR = new TFile (inputdir + "/PR/07_ZJets.root", "read");


  // Prompt rate
  //----------------------------------------------------------------------------
  WritePR("Ele");
  WritePR("Muon");

  if (draw)
    {
      DrawPR("Ele",  "pt",  "p_{T} [GeV]");
      DrawPR("Muon", "pt",  "p_{T} [GeV]");
      DrawPR("Ele",  "eta", "|#eta|");
      DrawPR("Muon", "eta", "|#eta|");
    }


  // Fake rate
  //----------------------------------------------------------------------------
  Float_t elejetet;
  Float_t muojetet;

  for (Int_t i=0; i<njetet; i++) {

    elejetet = elejetarray[i];
    muojetet = muojetarray[i];

    WriteFR("Ele",  elescale, elejetet);
    WriteFR("Muon", muoscale, muojetet);

    if (!draw) continue;

    DrawFR("Ele",  "pt",  "p_{T} [GeV]", elescale, elejetet);
    DrawFR("Ele",  "eta", "|#eta|",      elescale, elejetet);

    DrawFR("Muon", "pt",  "p_{T} [GeV]", muoscale, muojetet);
    DrawFR("Muon", "eta", "|#eta|",      muoscale, muojetet);
  }
}


//------------------------------------------------------------------------------
// DrawFR
//------------------------------------------------------------------------------
void DrawFR(TString flavour,
	    TString variable,
	    TString xtitle,
	    Float_t lepscale,
	    Float_t jetet)
{
  TString suffix = Form("%s_bin_%.0fGeV", variable.Data(), jetet);


  // Read loose and tight histograms
  //----------------------------------------------------------------------------
  TH1D* h_loose_data  = (TH1D*)dataFR ->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_zjets = (TH1D*)zjetsFR->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_wjets = (TH1D*)wjetsFR->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_tight_data  = (TH1D*)dataFR ->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_zjets = (TH1D*)zjetsFR->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_wjets = (TH1D*)wjetsFR->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Make EWK correction histograms
  //----------------------------------------------------------------------------
  TH1D* h_tight_correction = (TH1D*)h_tight_zjets->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_loose_correction = (TH1D*)h_loose_zjets->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_EWKrel_tight     = (TH1D*)h_tight_data ->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_EWKrel_loose     = (TH1D*)h_loose_data ->Clone("h_" + flavour + "_FR_" + variable);
  
  h_tight_correction->Add(h_tight_wjets, 1);
  h_loose_correction->Add(h_loose_wjets, 1);

  h_EWKrel_tight->Divide(h_tight_correction, h_tight_data);    
  h_EWKrel_loose->Divide(h_loose_correction, h_loose_data);    


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR                 = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_"                 + variable);
  TH1D* h_FR_denominator     = (TH1D*)h_loose_data->Clone("h_" + flavour + "_FR_denominator_"     + variable);
  TH1D* h_FR_EWK             = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_EWK_"             + variable);
  TH1D* h_FR_EWK_denominator = (TH1D*)h_loose_data->Clone("h_" + flavour + "_FR_EWK_denominator_" + variable);


  // Do the math
  //----------------------------------------------------------------------------
  h_FR->Divide(h_FR_denominator);

  if (Zsubtraction) h_FR_EWK->Add(h_tight_zjets, lepscale);
  if (Wsubtraction) h_FR_EWK->Add(h_tight_wjets, lepscale);

  if (Zsubtraction) h_FR_EWK_denominator->Add(h_loose_zjets, lepscale);
  if (Wsubtraction) h_FR_EWK_denominator->Add(h_loose_wjets, lepscale);

  h_FR_EWK->Divide(h_FR_EWK_denominator);


  // Draw fake rate
  //----------------------------------------------------------------------------
  TString title1 = Form("%s fake rate when jet p_{T} > %.0f GeV", flavour.Data(), jetet);

  TCanvas* canvas1 = new TCanvas(title1 + " " + variable, title1 + " " +  variable);
      
  canvas1->SetGridx(setgrid);
  canvas1->SetGridy(setgrid);
  
  Cosmetics(h_FR,     "ep",      xtitle, title1, kBlack);
  Cosmetics(h_FR_EWK, "ep,same", xtitle, title1, kRed+1);

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");

  DrawLegend(0.22, 0.845, h_FR,     "Without EWK correction");
  DrawLegend(0.22, 0.800, h_FR_EWK, "With EWK correction");


  // Draw tight EWK correction
  //----------------------------------------------------------------------------
  TString title2 = Form("tight %s EWK correction when jet p_{T} > %.0f GeV", flavour.Data(), jetet);

  TCanvas* canvas2 = new TCanvas(title2 + " " + variable, title2 + " " + variable);

  canvas2->SetGridx(setgrid);
  canvas2->SetGridy(setgrid);

  Cosmetics(h_EWKrel_tight, "ep", xtitle, title2, kBlack);

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");


  // Draw loose EWK correction
  //----------------------------------------------------------------------------
  TString title3 = Form("loose %s EWK correction when jet p_{T} > %.0f GeV", flavour.Data(), jetet);

  TCanvas* canvas3 = new TCanvas(title3 + " " + variable, title3 + " " + variable);

  canvas3->SetGridx(setgrid);
  canvas3->SetGridy(setgrid);

  Cosmetics(h_EWKrel_loose, "ep", xtitle, title3, kBlack);

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");


  // Save
  //----------------------------------------------------------------------------
  if (savepng) canvas1->SaveAs(Form("png/%s_FR_%s_%.0fGeV.png",           flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas2->SaveAs(Form("png/%s_EWKrel_tight_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas3->SaveAs(Form("png/%s_EWKrel_loose_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));
}


//------------------------------------------------------------------------------
// DrawPR
//------------------------------------------------------------------------------
void DrawPR(TString flavour,
	    TString variable,
	    TString xtitle)
{
  TH1D* h_loose_zjets = (TH1D*)zjetsPR -> Get("h_" + flavour + "_loose_" + variable + "_PR");
  TH1D* h_tight_zjets = (TH1D*)zjetsPR -> Get("h_" + flavour + "_tight_" + variable + "_PR");

  TH1D* h_PR = (TH1D*)h_tight_zjets->Clone("h_" + flavour + "_PR_" + variable);
      
  h_PR->Divide(h_tight_zjets, h_loose_zjets);


  // Draw
  //----------------------------------------------------------------------------
  TString title = flavour + " prompt rate";

  TCanvas* canvas = new TCanvas(title + " " + variable, title + " " + variable);
      
  canvas->SetGridx(setgrid);
  canvas->SetGridy(setgrid);

  Cosmetics(h_PR, "ep", xtitle, title, kBlack);


  // Save
  //----------------------------------------------------------------------------
  if (savepng) canvas->SaveAs(Form("png/%s_PR_%s.png", flavour.Data(), variable.Data()));
}


//------------------------------------------------------------------------------
// WriteFR
//------------------------------------------------------------------------------
void WriteFR(TString flavour,
	     Float_t lepscale,
	     Float_t jetet)
{
  TString suffix = Form("pt_eta_bin_%.0fGeV", jetet);

  
  // Read loose and tight histograms
  //----------------------------------------------------------------------------
  TH2D* h_loose_data  = (TH2D*)dataFR ->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_zjets = (TH2D*)zjetsFR->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_wjets = (TH2D*)wjetsFR->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_tight_data  = (TH2D*)dataFR ->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_zjets = (TH2D*)zjetsFR->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_wjets = (TH2D*)wjetsFR->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH2D* h_FR                 = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR");
  TH2D* h_FR_numerator       = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR_numerator");
  TH2D* h_FR_denominator     = (TH2D*)h_loose_data->Clone("h_" + flavour + "_FR_denominator");
  TH2D* h_FR_EWK             = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR_EWK");
  TH2D* h_FR_EWK_numerator   = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR_EWK_numerator");
  TH2D* h_FR_EWK_denominator = (TH2D*)h_loose_data->Clone("h_" + flavour + "_FR_EWK_denominator");


  // Do the math
  //----------------------------------------------------------------------------
  h_FR->Divide(h_FR_numerator, h_FR_denominator);

  if (Zsubtraction) h_FR_EWK_numerator->Add(h_tight_zjets, lepscale);
  if (Wsubtraction) h_FR_EWK_numerator->Add(h_tight_wjets, lepscale);

  if (Zsubtraction) h_FR_EWK_denominator->Add(h_loose_zjets, lepscale);
  if (Wsubtraction) h_FR_EWK_denominator->Add(h_loose_wjets, lepscale);

  h_FR_EWK->Divide(h_FR_EWK_numerator, h_FR_EWK_denominator);


  // Write
  //----------------------------------------------------------------------------
  TFile *file = new TFile(Form("%s/%sFR_Run2016_HWW36fb_jet%0.f.root",
			       outputdir.Data(),
			       flavour.Data(),
			       jetet),
			  "recreate");

  h_FR            ->Write("FR_pT_eta");
  h_FR_numerator  ->Write("FR_pT_eta_numerator");
  h_FR_denominator->Write("FR_pT_eta_denominator");

  h_FR_EWK            ->Write("FR_pT_eta_EWKcorr");
  h_FR_EWK_numerator  ->Write("FR_pT_eta_EWKcorr_numerator");
  h_FR_EWK_denominator->Write("FR_pT_eta_EWKcorr_denominator");

  file->Close();
}


//------------------------------------------------------------------------------
// WritePR
//------------------------------------------------------------------------------
void WritePR(TString flavour)
{
  TH2D* h_loose_zjets = (TH2D*)zjetsPR->Get("h_" + flavour + "_loose_pt_eta_PR");
  TH2D* h_tight_zjets = (TH2D*)zjetsPR->Get("h_" + flavour + "_tight_pt_eta_PR");

  TH2D* h_PR = (TH2D*)h_tight_zjets->Clone("h_" + flavour + "_signal_pt_eta_bin");

  h_PR->Divide(h_tight_zjets, h_loose_zjets);


  // Write
  //----------------------------------------------------------------------------
  TFile* file = new TFile(Form("%s/%sPR_Run2016_HWW36fb.root",
			       outputdir.Data(),
			       flavour.Data()),
			  "recreate");

  h_PR->Write();
  
  file->Close();
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


//------------------------------------------------------------------------------
// Cosmetics
//------------------------------------------------------------------------------
void Cosmetics(TH1D*     hist,
	       Option_t* option,
	       TString   xtitle,
	       TString   ytitle,
	       Color_t   color)
{
  hist->Draw(option);

  hist->SetLineColor  (color);
  hist->SetLineWidth  (2);
  hist->SetMarkerColor(color);
  hist->SetMarkerStyle(kFullCircle);
  hist->SetTitle      ("");
  hist->SetXTitle     (xtitle);
  hist->SetYTitle     (ytitle);

  hist->SetAxisRange(0, 1, "Y");

  hist->GetXaxis()->SetTitleOffset(1.5);
  hist->GetYaxis()->SetTitleOffset(1.8);
}
