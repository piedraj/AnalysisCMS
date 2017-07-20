const Float_t _bigLabelSize = 0.04;


// Member functions
//------------------------------------------------------------------------------
void DrawIt       (TString     filename,
		   TString     hname,
		   TString     cname,
		   TString     title);

void AxisFonts    (TAxis*      axis,
		   TString     title);

void TH2DAxisFonts(TH2D*       h,
		   TString     coordinate,
		   TString     title);

void DrawLatex    (Font_t      tfont,
		   Float_t     x,
		   Float_t     y,
		   Float_t     tsize,
		   Short_t     align,
		   const char* text,
		   Bool_t      setndc = true);


//------------------------------------------------------------------------------
//
// drawTH2D
//
//------------------------------------------------------------------------------
void drawTH2D()
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  gSystem->mkdir("png", kTRUE);

  DrawIt("rootfilesFR/EleFR_Run2016_HWW36fb_jet35",  "FR_pT_eta_EWKcorr",        "ElecFR_Jet35", "electron FR jet35 (13 TeV)");
  DrawIt("rootfilesFR/MuonFR_Run2016_HWW36fb_jet25", "FR_pT_eta_EWKcorr",        "MuonFR_Jet25", "muon FR jet25 (13 TeV)");
  DrawIt("rootfilesPR/ElePR_Run2016_HWW36fb",        "h_Ele_signal_pt_eta_bin",  "ElecPR",       "electron PR (13 TeV)");
  DrawIt("rootfilesPR/MuonPR_Run2016_HWW36fb",       "h_Muon_signal_pt_eta_bin", "MuonPR",       "muon PR (13 TeV)");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TString filename,
	    TString hname,
	    TString cname,
	    TString title)
{
  TFile* inputfile = TFile::Open(filename + ".root");

  TH2D* h = (TH2D*)inputfile->Get(hname)->Clone(cname);

  h->SetDirectory(0);

  inputfile->Close();

  TString name = h->GetName();

  TCanvas* canvas = new TCanvas(name, name);

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2DAxisFonts(h, "x", "p_{T} [GeV]");
  TH2DAxisFonts(h, "y", "#eta");

  h->Draw("colz");

  h->SetTitle("");

  DrawLatex(42, 0.940, 0.976, _bigLabelSize, 33, title);


  // Print values
  //----------------------------------------------------------------------------
  Double_t hmin = h->GetMinimum();
  Double_t hmax = h->GetMaximum();
    
  for (Int_t i=1; i<=h->GetNbinsX(); i++) {
    for (Int_t j=1; j<=h->GetNbinsY(); j++) {

      Double_t value = h->GetBinContent(i,j);

      Double_t ypos = h->GetYaxis()->GetBinCenter(j);
      Double_t xpos = h->GetXaxis()->GetBinCenter(i);
      
      if (gPad->GetLogx()) xpos = h->GetXaxis()->GetBinCenterLog(i);

      TLatex* latex = new TLatex(xpos, ypos, Form("%.2f", value));
      
      latex->SetTextAlign(   22);
      latex->SetTextFont (   42);
      latex->SetTextSize (0.027);

      if (value < hmin + 0.3*(hmax - hmin)) latex->SetTextColor(kWhite);
	
      latex->Draw();
    }
  }


  // Set the palette font
  //----------------------------------------------------------------------------
  canvas->Update();

  TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");

  palette->SetLabelFont(42);


  // Save the plot
  //----------------------------------------------------------------------------
  canvas->Update();
  
  canvas->Modified();

  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("png/" + name + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
{
  axis->SetLabelFont  (   42);
  axis->SetLabelOffset(0.015);
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetLabelSize  (_bigLabelSize);
  axis->SetTitleSize  (_bigLabelSize);

  axis->SetTitle(title);
}


//------------------------------------------------------------------------------
// TH2DAxisFonts
//------------------------------------------------------------------------------
void TH2DAxisFonts(TH2D*   h,
		   TString coordinate,
		   TString title)
{
  TAxis* axis = NULL;

  if (coordinate.Contains("x")) axis = h->GetXaxis();
  if (coordinate.Contains("y")) axis = h->GetYaxis();

  AxisFonts(axis, title);
}


//-----------------------------------------------------------------------------
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
