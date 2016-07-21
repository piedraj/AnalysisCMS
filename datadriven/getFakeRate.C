
// Constants and data members
//------------------------------------------------------------------------------
const Double_t muonjetarray[] = {10, 15, 20, 25, 30, 35, 45};
const Double_t elejetarray [] = {10, 15, 20, 25, 30, 35, 45};

bool draw         = false;
bool savepng      = true;
bool Wsubtraction = true;
bool Zsubtraction = true;

TFile* data;
TFile* wjets;
TFile* zjets;


// Functions
//------------------------------------------------------------------------------
void DrawFR (TString  flavour,
	     TString  variable,
	     Double_t jetet);

void DrawPR (TString  flavour,
	     TString  variable);

void WriteFR(TString  flavour,
	     Double_t jetet);

void WritePR(TString  flavour);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l getFakeRate.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getFakeRate()
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  if (savepng) gSystem->mkdir("png", kTRUE);

  gSystem->mkdir("rootfiles", kTRUE);

  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root",  "read");
  wjets = new TFile ("../rootfiles/nominal/FR/08_WJets.root", "read");
  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root", "read");


  // Prompt rate
  //----------------------------------------------------------------------------
  WritePR("Ele");
  WritePR("Muon");

  if (draw)
    {
      DrawPR("Ele",  "pt");
      DrawPR("Ele",  "eta");
      DrawPR("Muon", "pt");
      DrawPR("Muon", "eta");
    }


  // Fake rate
  //----------------------------------------------------------------------------
  float elejetet;
  float muonjetet;

  int njetet = (draw) ? 1 : 7;

  for (int i=0; i<njetet; i++) {

    if (draw) {

      elejetet  = elejetarray [5];
      muonjetet = muonjetarray[3];

      DrawFR("Ele",  "pt",  elejetet);
      DrawFR("Ele",  "eta", elejetet);
      DrawFR("Muon", "pt",  muonjetet);
      DrawFR("Muon", "eta", muonjetet);

    } else {

      elejetet  = elejetarray [i];
      muonjetet = muonjetarray[i];
    }

    WriteFR("Ele",  elejetet);
    WriteFR("Muon", muonjetet);
  }
}


//------------------------------------------------------------------------------
// DrawFR
//------------------------------------------------------------------------------
void DrawFR(TString  flavour,
	    TString  variable,
	    Double_t jetet)
{
  TString title  = Form("%s fake rate %s", flavour.Data(), variable.Data());
  TString suffix = Form("%s_bin_%.0fGev", variable.Data(), jetet);

  
  // Read loose and tight histograms
  //----------------------------------------------------------------------------
  TH1D* h_loose_data  = (TH1D*)data ->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_zjets = (TH1D*)zjets->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_loose_wjets = (TH1D*)wjets->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH1D* h_tight_data  = (TH1D*)data ->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_zjets = (TH1D*)zjets->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH1D* h_tight_wjets = (TH1D*)wjets->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR     = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable);
  TH1D* h_FR_EWK = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_" + variable +"_EWK");
      
  h_FR->Divide(h_tight_data, h_loose_data);

  if (Zsubtraction) h_loose_data->Add(h_loose_zjets, -1.);
  if (Wsubtraction) h_loose_data->Add(h_loose_wjets, -1.);
  if (Zsubtraction) h_tight_data->Add(h_tight_zjets, -1.);
  if (Wsubtraction) h_tight_data->Add(h_tight_wjets, -1.);

  h_FR_EWK->Divide(h_tight_data, h_loose_data);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(title, title, 450, 550);
      
  canvas->SetGridx(1);
  canvas->SetGridy(1);

  h_FR->Draw();

  h_FR_EWK->Draw("same");


  // Cosmetics
  //----------------------------------------------------------------------------
  h_FR->SetAxisRange(0, 1, "Y");
  h_FR->SetLineColor(kBlack);
  h_FR->SetLineWidth(2);
  h_FR->SetTitle(title);
  h_FR->SetXTitle(variable);
  h_FR->SetYTitle("FR");

  h_FR_EWK->SetLineColor(kRed+1);
  h_FR_EWK->SetLineWidth(2);


  // Save
  //----------------------------------------------------------------------------
  if (savepng) canvas->SaveAs(Form("png/%s_FR_%s_%.0fGev.png", flavour.Data(), variable.Data(), jetet));
}


//------------------------------------------------------------------------------
// DrawPR
//------------------------------------------------------------------------------
void DrawPR(TString  flavour,
	    TString  variable)
{
  TString title = Form("%s prompt rate %s", flavour.Data(), variable.Data());

  TH1D* h_loose_zjets = (TH1D*)zjets->Get("h_" + flavour + "_loose_" + variable + "_PR");
  TH1D* h_tight_zjets = (TH1D*)zjets->Get("h_" + flavour + "_tight_" + variable + "_PR");

  TH1D* h_PR = (TH1D*)h_tight_zjets->Clone("h_" + flavour + "_PR_" + variable);
      
  h_PR->Divide(h_tight_zjets, h_loose_zjets);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(title, title, 450, 550);
      
  canvas->SetGridx(1);
  canvas->SetGridy(1);

  h_PR->Draw();


  // Cosmetics
  //----------------------------------------------------------------------------
  h_PR->SetAxisRange(0, 1, "Y");
  h_PR->SetLineColor(kBlack);
  h_PR->SetLineWidth(2);
  h_PR->SetTitle(title);
  h_PR->SetXTitle(variable);
  h_PR->SetYTitle("PR");


  // Save
  //----------------------------------------------------------------------------
  if (savepng) canvas->SaveAs(Form("png/%s_PR_%s.png", flavour.Data(), variable.Data()));
}


//------------------------------------------------------------------------------
// WriteFR
//------------------------------------------------------------------------------
void WriteFR(TString  flavour,
	     Double_t jetet)
{
  TString suffix = Form("pt_eta_bin_%.0fGev", jetet);

  
  // Read loose and tight histograms
  //----------------------------------------------------------------------------
  TH2D* h_loose_data  = (TH2D*)data ->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_zjets = (TH2D*)zjets->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_loose_wjets = (TH2D*)wjets->Get("FR/00_QCD/h_" + flavour + "_loose_" + suffix);
  TH2D* h_tight_data  = (TH2D*)data ->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_zjets = (TH2D*)zjets->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);
  TH2D* h_tight_wjets = (TH2D*)wjets->Get("FR/00_QCD/h_" + flavour + "_tight_" + suffix);


  // Prepare fake rate histograms
  //----------------------------------------------------------------------------
  TH1D* h_FR     = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta");
  TH1D* h_FR_EWK = (TH1D*)h_tight_data->Clone("h_" + flavour + "_FR_pt_eta_EWK");
      
  h_FR->Divide(h_tight_data, h_loose_data);

  if (Zsubtraction) h_loose_data->Add(h_loose_zjets, -1.);
  if (Wsubtraction) h_loose_data->Add(h_loose_wjets, -1.);
  if (Zsubtraction) h_tight_data->Add(h_tight_zjets, -1.);
  if (Wsubtraction) h_tight_data->Add(h_tight_wjets, -1.);

  h_FR_EWK->Divide(h_tight_data, h_loose_data);


  // Write
  //----------------------------------------------------------------------------
  TFile *file = new TFile(Form("rootfiles/%sFR_Run2016B_25ns_jet%0.f.root", flavour.Data(), jetet), "recreate");

  h_FR    ->Write("FR_pT_eta");
  h_FR_EWK->Write("FR_pT_eta_EWKcorr");

  file->Close();
}


//------------------------------------------------------------------------------
// WritePR
//------------------------------------------------------------------------------
void WritePR(TString flavour)
{
  TH2D* h_loose_zjets = (TH2D*)zjets->Get("h_" + flavour + "_loose_pt_eta_PR");
  TH2D* h_tight_zjets = (TH2D*)zjets->Get("h_" + flavour + "_tight_pt_eta_PR");

  TH2D* h_PR = (TH2D*)h_tight_zjets->Clone("h_" + flavour + "_signal_pt_eta_bin");
      
  h_PR->Divide(h_tight_zjets, h_loose_zjets);


  // Write
  //----------------------------------------------------------------------------
  TFile* file = new TFile("rootfiles/" + flavour + "PR_Run2016B_25ns.root","recreate");

  h_PR->Write();
  
  file->Close();
}
