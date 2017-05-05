
// Constants and data members
//------------------------------------------------------------------------------
const Bool_t verbose = false;

const Float_t muonjetarray[] = {10, 15, 20, 25, 30, 35, 45};
const Float_t elejetarray [] = {10, 15, 20, 25, 30, 35, 45};

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
void     DrawFR     (TString    flavour,
		     TString    variable,
		     TString    xtitle,
		     Float_t    lepscale,
		     Float_t    jetet);

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

  //dataFR  = new TFile ("../rootfilesNewFakes/nominal/FR/01_Data.root",  "read");
  //wjetsFR = new TFile ("../rootfilesNewFakes/nominal/FR/08_WJets.root", "read");
  //zjetsFR = new TFile ("../rootfilesNewFakes/nominal/FR/07_ZJets.root", "read");

  dataFR  = new TFile ("../rootfiles/newFakes2/FR/01_Data.root",  "read");
  wjetsFR = new TFile ("../rootfiles/newFakes2/FR/08_WJets.root", "read");
  zjetsFR = new TFile ("../rootfiles/newFakes2/FR/07_ZJets.root", "read");

  zjetsPR = new TFile ("../rootfilesPR/nominal/PR/07_ZJets.root", "read");

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

  //int njetet = (draw) ? 1 : 7;
  int njetet = 7;

  for (int i=0; i<njetet; i++) {

    if (draw) {

      //elejetet  = elejetarray [5];
      //muonjetet = muonjetarray[3];

      elejetet  = elejetarray [i];
      muonjetet = muonjetarray[i];

      DrawFR("Ele",  "pt",  "p_{T} [GeV]", elescale,  elejetet);
      DrawFR("Muon", "pt",  "p_{T} [GeV]", muonscale, muonjetet);
      DrawFR("Ele",  "eta", "|#eta|",      elescale,  elejetet);
      DrawFR("Muon", "eta", "|#eta|",      muonscale, muonjetet);

    } else {

      elejetet  = elejetarray [i];
      muonjetet = muonjetarray[i];
    }

    WriteFR("Ele",  elescale,  elejetet);
    WriteFR("Muon", muonscale, muonjetet);
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
  TString title  = Form("%s fake rate %s", flavour.Data(), variable.Data());
  TString title_EWKrel_tight = Form("%s EWK relative correction (tight) %s", flavour.Data(), variable.Data());
  TString title_EWKrel_loose = Form("%s EWK relative correction (loose) %s", flavour.Data(), variable.Data());
  TString suffix = Form("%s_bin_%.0fGeV", variable.Data(), jetet);

  
  // Read loose and tight histograms
  //----------------------------------------------------------------------------
  TH1D* h_loose_data  = (TH1D*)dataFR  -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_zjets = (TH1D*)zjetsFR -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_wjets = (TH1D*)wjetsFR -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_tight_data  = (TH1D*)dataFR  -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_zjets = (TH1D*)zjetsFR -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_wjets = (TH1D*)wjetsFR -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR     = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_FR_EWK = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable +"_EWK");

  h_FR->Divide(h_tight_data, h_loose_data);

  TH1D* h_tight_correction = (TH1D*)h_tight_zjets->Clone("h_" + flavour + "_FR_" + variable);;
  TH1D* h_loose_correction = (TH1D*)h_loose_zjets->Clone("h_" + flavour + "_FR_" + variable);;
  TH1D* h_EWKrel_tight = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_EWKrel_loose = (TH1D*)h_loose_data->Clone("h_" + flavour + "_FR_" + variable);
  
  h_tight_correction->Add(h_tight_wjets, 1);
  h_loose_correction->Add(h_loose_wjets, 1);

  h_EWKrel_tight->Divide(h_tight_correction, h_tight_data);    
  h_EWKrel_loose->Divide(h_loose_correction, h_loose_data);    

  if (Zsubtraction) h_loose_data->Add(h_loose_zjets, lepscale);
  if (Wsubtraction) h_loose_data->Add(h_loose_wjets, lepscale);
  if (Zsubtraction) h_tight_data->Add(h_tight_zjets, lepscale);
  if (Wsubtraction) h_tight_data->Add(h_tight_wjets, lepscale);

  h_FR_EWK->Divide(h_tight_data, h_loose_data);

  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(title, title, 450, 550);
      
  canvas->SetGridx(setgrid);
  canvas->SetGridy(setgrid);

  h_FR->Draw("ep");
  h_FR_EWK->Draw("ep,same");

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");
  DrawLegend(0.22, 0.83, h_FR, "Without EWK correction");
  DrawLegend(0.22, 0.80, h_FR_EWK, "With EWK correction");

  TCanvas* canvas2 = new TCanvas(title_EWKrel_tight, title_EWKrel_tight, 450, 550);

  canvas2->SetGridx(setgrid);
  canvas2->SetGridy(setgrid);

  h_EWKrel_tight -> Draw("ep");

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");

  TCanvas* canvas3 = new TCanvas(title_EWKrel_loose, title_EWKrel_loose, 450, 550);

  canvas3->SetGridx(setgrid);
  canvas3->SetGridy(setgrid);

  h_EWKrel_loose -> Draw("ep");

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");

  // Print bin values and errors
  //----------------------------------------------------------------------------
  if (variable.EqualTo("pt") && verbose)
    {
      printf("\n");

      for (int i=1; i<=h_FR_EWK->GetNbinsX(); i++)
	{
	  float ptvalue = h_FR_EWK->GetBinLowEdge(i);
	
	  printf(" bin[%d]   pt: %.0f GeV   fr: %.3f +- %.3f\n",
		 i,
		 ptvalue,
		 h_FR_EWK->GetBinContent(i),
		 h_FR_EWK->GetBinError(i));
	}

      printf("\n");
    }


  // Cosmetics
  //----------------------------------------------------------------------------

  h_FR->SetAxisRange(0, 1, "Y");
  h_FR->SetLineColor(kBlack);
  h_FR->SetLineWidth(2);
  h_FR->SetMarkerColor(kBlack);
  h_FR->SetMarkerStyle(kFullCircle);
  h_FR->SetTitle("");
  h_FR->SetXTitle(xtitle);
  h_FR->SetYTitle(Form("%s fake rate", flavour.Data()));

  h_FR->GetXaxis()->SetTitleOffset(1.5);
  h_FR->GetYaxis()->SetTitleOffset(1.8);

  h_FR_EWK->SetLineColor(kRed+1);
  h_FR_EWK->SetLineWidth(2);
  h_FR_EWK->SetMarkerColor(kRed+1);
  h_FR_EWK->SetMarkerStyle(kFullCircle);

  h_EWKrel_tight->SetTitle("");
  h_EWKrel_tight->SetXTitle(xtitle);
  h_EWKrel_tight->SetYTitle("EWK relative correction (tight)");
  h_EWKrel_tight->GetXaxis()->SetTitleOffset(1.5);
  h_EWKrel_tight->GetYaxis()->SetTitleOffset(1.8);
  h_EWKrel_tight->SetLineWidth(2);
  h_EWKrel_tight->SetMarkerStyle(kFullCircle);

  h_EWKrel_loose->SetTitle("");
  h_EWKrel_loose->SetXTitle(xtitle);
  h_EWKrel_loose->SetYTitle("EWK relative correction (loose)");
  h_EWKrel_loose->GetXaxis()->SetTitleOffset(1.5);
  h_EWKrel_loose->GetYaxis()->SetTitleOffset(1.8);
  h_EWKrel_loose->SetLineWidth(2);
  h_EWKrel_loose->SetMarkerStyle(kFullCircle);

  // Save and write
  //----------------------------------------------------------------------------
  if (savepng) canvas->SaveAs(Form("png/%s_FR_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas2->SaveAs(Form("png/%s_EWKrel_tight_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));
  if (savepng) canvas3->SaveAs(Form("png/%s_EWKrel_loose_%s_%.0fGeV.png", flavour.Data(), variable.Data(), jetet));

  TFile* file = new TFile(Form("rootfilesFR/%s_FR_%s_%.0fGeV.root", flavour.Data(), variable.Data(), jetet), "recreate");

  h_FR->Write();
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
  TString title = Form("%s prompt rate %s", flavour.Data(), variable.Data());

  TH1D* h_loose_zjets = (TH1D*)zjetsPR -> Get("h_" + flavour + "_loose_" + variable + "_PR");
  TH1D* h_tight_zjets = (TH1D*)zjetsPR -> Get("h_" + flavour + "_tight_" + variable + "_PR");

  TH1D* h_PR = (TH1D*)h_tight_zjets->Clone("h_" + flavour + "_PR_" + variable);
      
  h_PR->Divide(h_tight_zjets, h_loose_zjets);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(title, title, 450, 550);
      
  canvas->SetGridx(setgrid);
  canvas->SetGridy(setgrid);

  h_PR->Draw("ep");

  // Cosmetics
  //----------------------------------------------------------------------------
  h_PR->SetAxisRange(0, 1, "Y");
  h_PR->SetLineColor(kBlack);
  h_PR->SetLineWidth(2);
  h_PR->SetMarkerColor(kBlack);
  h_PR->SetMarkerStyle(kFullCircle);
  h_PR->SetTitle("");
  h_PR->SetXTitle(xtitle);
  h_PR->SetYTitle(Form("%s prompt rate", flavour.Data()));

  h_PR->GetXaxis()->SetTitleOffset(1.5);
  h_PR->GetYaxis()->SetTitleOffset(1.8);

  DrawLatex(42, 0.940, 0.945, 0.045, 31, "35.9 fb^{-1} (13 TeV)");


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
  TH2D* h_loose_data  = (TH2D*)dataFR  -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_zjets = (TH2D*)zjetsFR -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_wjets = (TH2D*)wjetsFR -> Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_tight_data  = (TH2D*)dataFR  -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_zjets = (TH2D*)zjetsFR -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_wjets = (TH2D*)wjetsFR -> Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR     = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta");
  TH1D* h_FR_EWK = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta_EWK");
      
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

  TH2D* h_loose_zjets = (TH2D*)zjetsPR -> Get("h_" + flavour + "_loose_pt_eta_PR");
  TH2D* h_tight_zjets = (TH2D*)zjetsPR -> Get("h_" + flavour + "_tight_pt_eta_PR");

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
