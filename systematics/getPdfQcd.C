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
  TFile* file = new TFile(_inputdir + sample + ".root", "read");


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  TH1D* h_qcdsum_gen = (TH1D*)file->Get("h_qcdsum_gen");
  TH1D* h_qcdsum_rec = (TH1D*)file->Get("h_qcdsum_rec");

  float qcdratio_gen_up   = h_qcdsum_gen->GetBinContent(9) / h_qcdsum_gen->GetBinContent(1);
  float qcdratio_gen_down = h_qcdsum_gen->GetBinContent(5) / h_qcdsum_gen->GetBinContent(1);

  float qcdratio_rec_up   = h_qcdsum_rec->GetBinContent(9) / h_qcdsum_rec->GetBinContent(1);
  float qcdratio_rec_down = h_qcdsum_rec->GetBinContent(5) / h_qcdsum_rec->GetBinContent(1);


  // Produce the PDF uncertainties
  //----------------------------------------------------------------------------
  TH1D* h_pdfsum_gen = (TH1D*)file->Get("h_pdfsum_gen");
  TH1D* h_pdfsum_rec = (TH1D*)file->Get("h_pdfsum_rec");

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
}  
