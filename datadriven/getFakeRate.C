
// Constants and data members
//------------------------------------------------------------------------------
const Int_t   njetet = 7;

const Float_t muonjetarray[njetet] = {10, 15, 20, 25, 30, 35, 45};
const Float_t elejetarray [njetet] = {10, 15, 20, 25, 30, 35, 45};

const Float_t muonscale = -1.;
const Float_t elescale  = -1.;

bool draw         = true;
bool savepng      = true;
bool setgrid      = true;
bool Wsubtraction = true;
bool Zsubtraction = true;

TFile* dataFR;
TFile* wjetsFR;
TFile* zjetsFR;

TFile* zjetsPR;


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
// root -l getFakeRate.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakeRate()
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  if (savepng) gSystem->mkdir("png", kTRUE);

  gSystem->mkdir("rootfilesFR", kTRUE);
  gSystem->mkdir("rootfilesPR", kTRUE);

  dataFR  = new TFile ("../rootfiles/nominal/FR/01_Data.root",  "read");
  wjetsFR = new TFile ("../rootfiles/nominal/FR/08_WJets.root", "read");
  zjetsFR = new TFile ("../rootfiles/nominal/FR/07_ZJets.root", "read");
  zjetsPR = new TFile ("../rootfiles/nominal/PR/07_ZJets.root", "read");


  // Prompt rate
  //----------------------------------------------------------------------------
  WritePR("Ele");
  WritePR("Muon");

  if (draw)
    {
      DrawPR("Ele",  "pt" , "p_{T} [GeV]");
      DrawPR("Muon", "pt" , "p_{T} [GeV]");
      DrawPR("Ele",  "eta", "|#eta|");
      DrawPR("Muon", "eta", "|#eta|");
    }


  // Fake rate
  //----------------------------------------------------------------------------
  Float_t elejetet;
  Float_t muonjetet;

  for (Int_t i=0; i<njetet; i++) {
    for (Int_t j=0; j<njetet; j++) {

      elejetet  = elejetarray [i];
      muonjetet = muonjetarray[j];

      // jetet = 35 GeV for electrons
      // jetet = 25 GeV for muons
      if (draw && i == 5 && j == 3) {

	DrawFR("Ele",  "pt",  "p_{T} [GeV]", elescale,  elejetet);
	DrawFR("Muon", "pt",  "p_{T} [GeV]", muonscale, muonjetet);
	DrawFR("Ele",  "eta", "|#eta|",      elescale,  elejetet);
	DrawFR("Muon", "eta", "|#eta|",      muonscale, muonjetet);
      }

      WriteFR("Ele",  elescale,  elejetet);
      WriteFR("Muon", muonscale, muonjetet);
    }
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


  // Make fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR     = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_FR_EWK = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable +"_EWK");

  h_FR->Divide(h_tight_data, h_loose_data);

  if (Zsubtraction) h_loose_data->Add(h_loose_zjets, lepscale);
  if (Wsubtraction) h_loose_data->Add(h_loose_wjets, lepscale);
  if (Zsubtraction) h_tight_data->Add(h_tight_zjets, lepscale);
  if (Wsubtraction) h_tight_data->Add(h_tight_wjets, lepscale);

  h_FR_EWK->Divide(h_tight_data, h_loose_data);


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


  // Save and write
  //----------------------------------------------------------------------------
  if (savepng) canvas1->SaveAs(Form("png/%s_FR_%s_%.0fGeV.png",           flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas2->SaveAs(Form("png/%s_EWKrel_tight_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas3->SaveAs(Form("png/%s_EWKrel_loose_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));

  TFile* file = new TFile(Form("rootfilesFR/%s_FR_%s_%.0fGeV.root", flavour.Data(), variable.Data(), jetet), "recreate");

  h_FR    ->Write();
  h_FR_EWK->Write();
  
  file->Close();
}


//------------------------------------------------------------------------------
// DrawPR
//------------------------------------------------------------------------------
void DrawPR(TString  flavour,
	    TString  variable,
	    TString  xtitle)
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
  TH2D* h_FR     = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta");
  TH2D* h_FR_EWK = (TH2D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta_EWK");
      
  h_FR->Divide(h_tight_data, h_loose_data);

  if (Zsubtraction) h_loose_data->Add(h_loose_zjets, lepscale);
  if (Wsubtraction) h_loose_data->Add(h_loose_wjets, lepscale);
  if (Zsubtraction) h_tight_data->Add(h_tight_zjets, lepscale);
  if (Wsubtraction) h_tight_data->Add(h_tight_wjets, lepscale);

  h_FR_EWK->Divide(h_tight_data, h_loose_data);


  // Write
  //----------------------------------------------------------------------------
  TFile *file = new TFile(Form("rootfilesFR/%sFR_Run2016_HWW36fb_jet%0.f.root", flavour.Data(), jetet), "recreate");

  h_FR    ->Write("FR_pT_eta");
  h_FR_EWK->Write("FR_pT_eta_EWKcorr");

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
  TFile* file = new TFile("rootfilesPR/" + flavour + "PR_Run2016_HWW36fb.root","recreate");

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
