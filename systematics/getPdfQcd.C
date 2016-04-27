/*
  
  From mcWeightExplainedOrdered

  >>> 9 QCD variations
  [bin    1]   <weight id="1001"> muR=1 muF=1 </weight>
  [bin    2]   <weight id="1002"> muR=1 muF=2 </weight>
  [bin    3]   <weight id="1003"> muR=1 muF=0.5 </weight>
  [bin    4]   <weight id="1004"> muR=2 muF=1 </weight>
  [bin    5]   <weight id="1005"> muR=2 muF=2 </weight>
  [bin    6]   <weight id="1006"> muR=2 muF=0.5 </weight>
  [bin    7]   <weight id="1007"> muR=0.5 muF=1 </weight>
  [bin    8]   <weight id="1008"> muR=0.5 muF=2 </weight>
  [bin    9]   <weight id="1009"> muR=0.5 muF=0.5 </weight>

  >>> 100 PDF variations
  [bin   10]   <weight id="2001"> PDF set = 260001 </weight>
  [bin   11]   <weight id="2002"> PDF set = 260002 </weight>
  [bin   12]   <weight id="2003"> PDF set = 260003 </weight>
  ...
  [bin  107]   <weight id="2098"> PDF set = 260098 </weight>
  [bin  108]   <weight id="2099"> PDF set = 260099 </weight>
  [bin  109]   <weight id="2100"> PDF set = 260100 </weight>

  >>> 2 alpha_s variations
  [bin  110]   <weight id="2101"> PDF set = 265000 </weight>
  [bin  111]   <weight id="2102"> PDF set = 266000 </weight>

*/


#include "TFile.h"
#include "TH1D.h"
#include "TString.h"


// Constants
//------------------------------------------------------------------------------
const int _nqcd = 9;
const int _npdf = 100;

const TString _gendir = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight/";
const TString _recdir = "../rootfiles/nominal/TTDM/";


// Functions
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample, TString jetbin);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getPdfQcd
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPdfQcd()
{
  GetPdfQcdSyst("WWTo2L2Nu", "0jet");
  GetPdfQcdSyst("WWTo2L2Nu", "1jet");

  GetPdfQcdSyst("VBFHToWWTo2L2Nu_M125", "0jet");
  GetPdfQcdSyst("VBFHToWWTo2L2Nu_M125", "1jet");

  GetPdfQcdSyst("GluGluHToWWTo2L2Nu_M125", "0jet");
  GetPdfQcdSyst("GluGluHToWWTo2L2Nu_M125", "1jet");
}


//------------------------------------------------------------------------------
// GetPdfQcdSyst
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample, TString jetbin)
{
  TFile* genfile = new TFile(_gendir + "latino_" + sample + ".root", "read");
  TFile* recfile = new TFile(_recdir + sample + ".root", "read");

  TH1D* h_weights_gen = (TH1D*)genfile->Get("list_vectors_weights");
  TH1D* h_weights_rec = (TH1D*)recfile->Get("h_weights_rec_" + jetbin);


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  float qcdratio_gen_up = h_weights_gen->GetBinContent(9) / h_weights_gen->GetBinContent(1);
  float qcdratio_gen_do = h_weights_gen->GetBinContent(5) / h_weights_gen->GetBinContent(1);

  float qcdratio_rec_up = h_weights_rec->GetBinContent(9) / h_weights_rec->GetBinContent(1);
  float qcdratio_rec_do = h_weights_rec->GetBinContent(5) / h_weights_rec->GetBinContent(1);


  // Produce the alpha_s uncertainties
  //----------------------------------------------------------------------------
  float alpharatio_gen_up = h_weights_gen->GetBinContent(110) / h_weights_gen->GetBinContent(1);
  float alpharatio_gen_do = h_weights_gen->GetBinContent(111) / h_weights_gen->GetBinContent(1);

  float alpharatio_rec_up = h_weights_rec->GetBinContent(110) / h_weights_rec->GetBinContent(1);
  float alpharatio_rec_do = h_weights_rec->GetBinContent(111) / h_weights_rec->GetBinContent(1);


  // Produce the PDF uncertainties
  //----------------------------------------------------------------------------
  float pdf_gen_mean = 0;
  float pdf_rec_mean = 0;

  float pdf_gen_mean_sq = 0;
  float pdf_rec_mean_sq = 0;

  for (int i=10; i<110; i++)
    {
      pdf_gen_mean += (h_weights_gen->GetBinContent(i) / _npdf);
      pdf_rec_mean += (h_weights_rec->GetBinContent(i) / _npdf);

      pdf_gen_mean_sq += (pow(h_weights_gen->GetBinContent(i), 2) / _npdf);
      pdf_rec_mean_sq += (pow(h_weights_rec->GetBinContent(i), 2) / _npdf);
    }

  float pdf_gen_sd = sqrt(pdf_gen_mean_sq - pow(pdf_gen_mean, 2)); 
  float pdf_rec_sd = sqrt(pdf_rec_mean_sq - pow(pdf_rec_mean, 2)); 

  float pdf_gen_ratio = 1 + (pdf_gen_sd / pdf_gen_mean);
  float pdf_rec_ratio = 1 + (pdf_rec_sd / pdf_rec_mean);


  // Print the uncertainties
  //----------------------------------------------------------------------------
  printf("\n %s %s\n", sample.Data(), jetbin.Data());
  printf("--------------------------------------\n");
  printf(" QCD up        xs = %6.2f%%,  acc = %4.2f%%\n", 1e2 *     (1. - qcdratio_gen_up),   1e2 * fabs(1. - qcdratio_rec_up   / qcdratio_gen_up));
  printf(" QCD down      xs = %6.2f%%,  acc = %4.2f%%\n", 1e2 *     (1. - qcdratio_gen_do),   1e2 * fabs(1. - qcdratio_rec_do   / qcdratio_gen_do));
  printf(" alpha_s up    xs = %6.2f%%,  acc = %4.2f%%\n", 1e2 *     (1. - alpharatio_gen_up), 1e2 * fabs(1. - alpharatio_rec_up / alpharatio_gen_up));
  printf(" alpha_s down  xs = %6.2f%%,  acc = %4.2f%%\n", 1e2 *     (1. - alpharatio_gen_do), 1e2 * fabs(1. - alpharatio_rec_do / alpharatio_gen_do));
//printf(" PDF           xs = %6.2f%%,  acc = %4.2f%%\n", 1e2 * fabs(1. - pdf_gen_ratio),     1e2 * fabs(1. - pdf_rec_ratio     / pdf_gen_ratio));


  // Alternative PDF approach
  //----------------------------------------------------------------------------
  TH1D* h_pdfratio = new TH1D("h_pdfratio", "", 100, 0.97, 1.03);

  float denominator = h_weights_rec->GetBinContent(1) / h_weights_gen->GetBinContent(1);  // Nominal values

  for (int i=10; i<110; i++)
    {
      float numerator = h_weights_rec->GetBinContent(i) / h_weights_gen->GetBinContent(i);

      float ratio = numerator / denominator;

      h_pdfratio->Fill(ratio);
    }

  TCanvas* canvas = new TCanvas(sample + "_" + jetbin, sample + "_" + jetbin);

  h_pdfratio->Draw();

  printf(" PDF %24s acc = %4.2f%% (Andrea's way)\n\n", " ", 1e2*h_pdfratio->GetRMS());
}
