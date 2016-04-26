#include "TFile.h"
#include "TH1D.h"
#include "TString.h"


// Constants
//------------------------------------------------------------------------------
const TString _inputdir = "../rootfiles/nominal/TTDM/";


// Functions
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getPdfQcd
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPdfQcd()
{
  GetPdfQcdSyst("WWTo2L2Nu");
}


//------------------------------------------------------------------------------
// GetPdfQcdSyst
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample)
{
  TFile* gen_file = new TFile("WWTo2L2Nu_lheweights_gen.root", "read");

  TH1D* h_qcdsum_gen = (TH1D*)gen_file->Get("h_qcdsum_gen");
  TH1D* h_pdfsum_gen = (TH1D*)gen_file->Get("h_pdfsum_gen");

  TFile* rec_file = new TFile(_inputdir + sample + ".root", "read");

  TH1D* h_pdfsum_rec = (TH1D*)rec_file->Get("h_pdfsum_rec");
  TH1D* h_qcdsum_rec = (TH1D*)rec_file->Get("h_qcdsum_rec");


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
  printf("\n %s\n", sample.Data());
  printf("---------------------------------------\n");
  printf(" QCD up   -- xs = %5.3f -- acc = %5.3f\n", qcdratio_gen_up,   qcdratio_rec_up   / qcdratio_gen_up);
  printf(" QCD down -- xs = %5.3f -- acc = %5.3f\n", qcdratio_gen_down, qcdratio_rec_down / qcdratio_gen_down);
  printf(" PDF      -- xs = %5.3f -- acc = %5.3f\n", pdf_gen_ratio,     pdf_rec_ratio     / pdf_gen_ratio);
  printf("\n");


  // Alternative PDF approach
  //----------------------------------------------------------------------------
  TH1D* h_pdfratio = new TH1D("h_pdfratio", "", 50, 0.985, 1.015);

  float denominator = h_qcdsum_rec->GetBinContent(1) / h_qcdsum_gen->GetBinContent(1);  // Nominal values

  for (int a=1; a<=nbinpdf; a++)
    {
      float numerator = h_pdfsum_rec->GetBinContent(a) / h_pdfsum_gen->GetBinContent(a);

      float ratio = numerator / denominator;

      h_pdfratio->Fill(ratio);
    }

  TCanvas* c1 = new TCanvas("c1", "c1");

  h_pdfratio->Draw();

  printf(" Alternative PDF acceptance approach RMS = %.2f%s\n\n", 1e2*h_pdfratio->GetRMS(), "%");
}
