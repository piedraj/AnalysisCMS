
// Constants
//------------------------------------------------------------------------------
const Int_t   njetet = 7;

const Float_t muojetarray[njetet] = {10, 15, 20, 25, 30, 35, 45};
const Float_t elejetarray[njetet] = {10, 15, 20, 25, 30, 35, 45};

const Float_t bigLabelSize = 0.04;


// Data members
//------------------------------------------------------------------------------
TString ele_inputdir;
TString muo_inputdir;


// Member functions
//------------------------------------------------------------------------------
void DrawIt       (TString     filename,
		   TString     hname,
		   TString     cname,
		   TString     title,
		   TString     suffix = "");

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
// root -l -b -q drawTH2D.C
//
//------------------------------------------------------------------------------
void drawTH2D(TString ele_inputdir_name = "/afs/cern.ch/user/p/piedra/public/forXavier/36fb_ele_mva_90p_Iso2016",
	      TString muo_inputdir_name = "/afs/cern.ch/user/p/piedra/public/forXavier/36fb_muon")
{
  ele_inputdir = ele_inputdir_name;
  muo_inputdir = muo_inputdir_name;

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  gSystem->mkdir("png", kTRUE);


  // Prompt rate
  //----------------------------------------------------------------------------
  DrawIt("ElePR_Run2016_HWW36fb",  "h_Ele_signal_pt_eta_bin",  "ElecPR", "electron PR");
  DrawIt("MuonPR_Run2016_HWW36fb", "h_Muon_signal_pt_eta_bin", "MuonPR", "muon PR");


  // Fake rate
  //----------------------------------------------------------------------------
  for (Int_t i=0; i<njetet; i++)
    {
      TString elejetet = Form("jet%.0f", elejetarray[i]);
      TString muojetet = Form("jet%.0f", muojetarray[i]);

      DrawIt("EleFR_Run2016_HWW36fb",  "FR_pT_eta_EWKcorr", "ElecFR_EWKcorr", "electron FR (EWK corrected)", elejetet);
      DrawIt("MuonFR_Run2016_HWW36fb", "FR_pT_eta_EWKcorr", "MuonFR_EWKcorr", "muon FR (EWK corrected)",     muojetet);

      
      // Debug negative fake rates, drawing numerator and denominator
      //------------------------------------------------------------------------
      //      DrawIt("EleFR_Run2016_HWW36fb", "FR_pT_eta_EWKcorr_numerator",   "ElecFR_EWKcorr_numerator",   "electron FR (EWK corrected) numerator",   elejetet);
      //      DrawIt("EleFR_Run2016_HWW36fb", "FR_pT_eta_EWKcorr_denominator", "ElecFR_EWKcorr_denominator", "electron FR (EWK corrected) denominator", elejetet);
    }
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TString filename,
	    TString hname,
	    TString cname,
	    TString title,
	    TString suffix)
{
  // Prepare labels
  //----------------------------------------------------------------------------
  if (suffix.Contains("jet"))
    {
      filename += "_" + suffix;
      cname    += "_" + suffix;
      title    += " " + suffix;
    }


  // Read TH2D
  //----------------------------------------------------------------------------
  TString inputdir = filename.Contains("Ele") ? ele_inputdir : muo_inputdir;

  TFile* inputfile = TFile::Open(inputdir + "/" + filename + ".root");

  TH2D* h = (TH2D*)inputfile->Get(hname)->Clone(cname);

  h->SetDirectory(0);

  inputfile->Close();


  // Draw it
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(cname, cname);

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2DAxisFonts(h, "x", "p_{T} [GeV]");
  TH2DAxisFonts(h, "y", "#eta");

  h->Draw("colz");

  h->SetTitle("");

  DrawLatex(42, 0.940, 0.976, bigLabelSize, 33, title);


  // Print values
  //----------------------------------------------------------------------------
  for (Int_t i=1; i<=h->GetNbinsX(); i++) {
    for (Int_t j=1; j<=h->GetNbinsY(); j++) {

      Double_t value = h->GetBinContent(i,j);

      Double_t ypos = h->GetYaxis()->GetBinCenter(j);
      Double_t xpos = h->GetXaxis()->GetBinCenter(i);
      
      if (gPad->GetLogx()) xpos = h->GetXaxis()->GetBinCenterLog(i);

      Int_t precision = 2;

      if (fabs(value) >  10.) precision = 1;
      if (fabs(value) > 100.) precision = 0;

      TString binvalue = Form("%s.%df", "%", precision);

      TLatex* latex = new TLatex(xpos, ypos, Form(binvalue.Data(), value));
      
      latex->SetTextAlign(   22);
      latex->SetTextFont (   42);
      latex->SetTextSize (0.027);

      if (value < 0.) latex->SetTextColor(kWhite);
	
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

  canvas->SaveAs("png/" + cname + ".png");
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
  axis->SetLabelSize  (bigLabelSize);
  axis->SetTitleSize  (bigLabelSize);

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
