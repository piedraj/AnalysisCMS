#include "TFile.h"
#include "TH1D.h"
#include "TString.h"


// Constants
//------------------------------------------------------------------------------
const TString _inputdir = "../rootfiles/nominal/TTDM/";


// Functions
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample,
		   TString jetbin);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getPdfQcd
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPdfQcd()
{
  GetPdfQcdSyst("WWTo2L2Nu", "0jet");
  GetPdfQcdSyst("WWTo2L2Nu", "1jet");
}


//------------------------------------------------------------------------------
// GetPdfQcdSyst
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample, TString jetbin)
{
  TFile* gen_file = new TFile("WWTo2L2Nu_lheweights_gen.root", "read");

  TH1D* h_qcdsum_gen = (TH1D*)gen_file->Get("h_qcdsum_gen");
  TH1D* h_pdfsum_gen = (TH1D*)gen_file->Get("h_pdfsum_gen");

  TFile* rec_file = new TFile(_inputdir + sample + ".root", "read");

  TH1D* h_pdfsum_rec = (TH1D*)rec_file->Get("h_pdfsum_rec_" + jetbin);
  TH1D* h_qcdsum_rec = (TH1D*)rec_file->Get("h_qcdsum_rec_" + jetbin);


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  float qcdratio_gen_up   = h_qcdsum_gen->GetBinContent(9) / h_qcdsum_gen->GetBinContent(1);
  float qcdratio_gen_down = h_qcdsum_gen->GetBinContent(5) / h_qcdsum_gen->GetBinContent(1);

  float qcdratio_rec_up   = h_qcdsum_rec->GetBinContent(9) / h_qcdsum_rec->GetBinContent(1);
  float qcdratio_rec_down = h_qcdsum_rec->GetBinContent(5) / h_qcdsum_rec->GetBinContent(1);


  // Produce the PDF uncertainties
  //----------------------------------------------------------------------------
  int nbinpdf = h_pdfsum_gen->GetNbinsX();

  float pdf_gen_mean = h_pdfsum_gen->Integral() / nbinpdf;
  float pdf_rec_mean = h_pdfsum_rec->Integral() / nbinpdf;

  float pdf_gen_mean_sq = 0;
  float pdf_rec_mean_sq = 0;

  for (int a=1; a<=nbinpdf; a++)
    {
      pdf_gen_mean_sq += (pow(h_pdfsum_gen->GetBinContent(a), 2) / nbinpdf);
      pdf_rec_mean_sq += (pow(h_pdfsum_rec->GetBinContent(a), 2) / nbinpdf);
    }

  float pdf_gen_sd = sqrt(pdf_gen_mean_sq - pow(pdf_gen_mean, 2)); 
  float pdf_rec_sd = sqrt(pdf_rec_mean_sq - pow(pdf_rec_mean, 2)); 

  float pdf_gen_ratio = 1 + (pdf_gen_sd / pdf_gen_mean);
  float pdf_rec_ratio = 1 + (pdf_rec_sd / pdf_rec_mean);


  // Print the uncertainties
  //----------------------------------------------------------------------------
  printf("\n %s %s\n", sample.Data(), jetbin.Data());
  printf("-------------------------------------\n");
  printf(" QCD up   xs = %5.2f%%, \t acc = %4.2f%%\n", 1e2 *     (1. - qcdratio_gen_up),   1e2 * fabs(1. - qcdratio_rec_up   / qcdratio_gen_up));
  printf(" QCD down xs = %5.2f%%, \t acc = %4.2f%%\n", 1e2 *     (1. - qcdratio_gen_down), 1e2 * fabs(1. - qcdratio_rec_down / qcdratio_gen_down));
  printf(" PDF      xs = %5.2f%%, \t acc = %4.2f%%\n", 1e2 * fabs(1. - pdf_gen_ratio),     1e2 * fabs(1. - pdf_rec_ratio     / pdf_gen_ratio));


  // Alternative PDF approach
  //----------------------------------------------------------------------------
  TH1D* h_pdfratio = new TH1D("h_pdfratio", "", 100, 0.97, 1.03);

  float denominator = h_qcdsum_rec->GetBinContent(1) / h_qcdsum_gen->GetBinContent(1);  // Nominal values

  for (int a=1; a<=nbinpdf; a++)
    {
      float numerator = h_pdfsum_rec->GetBinContent(a) / h_pdfsum_gen->GetBinContent(a);

      float ratio = numerator / denominator;

      h_pdfratio->Fill(ratio);
    }

  TCanvas* canvas = new TCanvas(sample + "_" + jetbin, sample + "_" + jetbin);

  h_pdfratio->Draw();

  printf(" PDF                     acc = %4.2f%% (Andrea's way)\n\n", 1e2*h_pdfratio->GetRMS());
}
