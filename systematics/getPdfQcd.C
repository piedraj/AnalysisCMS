/*

  The QCD, PDF and alpha_s variations are documented in the following slides,

  https://indico.cern.ch/event/459797/contributions/1961581/attachments/1181555/1800214/mcaod-Feb15-2016.pdf
  
  The correspondence with latinos is documented in the mcWeightExplainedOrdered histogram,

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

  TH1F* h_weights_gen = (TH1F*)genfile->Get("list_vectors_weights");
  TH1F* h_weights_rec = (TH1F*)recfile->Get("list_vectors_weights_" + jetbin);


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  float qcd_gen_mu05 = h_weights_gen->GetBinContent(9) / h_weights_gen->GetBinContent(1);
  float qcd_gen_mu20 = h_weights_gen->GetBinContent(5) / h_weights_gen->GetBinContent(1);

  float qcd_rec_mu05 = h_weights_rec->GetBinContent(9) / h_weights_rec->GetBinContent(1);
  float qcd_rec_mu20 = h_weights_rec->GetBinContent(5) / h_weights_rec->GetBinContent(1);


  // Produce the PDF uncertainties
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


  // Produce the alpha_s uncertainties
  //----------------------------------------------------------------------------
  float alpha_gen_265000 = h_weights_gen->GetBinContent(110) / h_weights_gen->GetBinContent(1);
  float alpha_gen_266000 = h_weights_gen->GetBinContent(111) / h_weights_gen->GetBinContent(1);

  float alpha_rec_265000 = h_weights_rec->GetBinContent(110) / h_weights_rec->GetBinContent(1);
  float alpha_rec_266000 = h_weights_rec->GetBinContent(111) / h_weights_rec->GetBinContent(1);


  // Print the uncertainties
  //----------------------------------------------------------------------------
  float qcd_mu05 = 1e2 * fabs(1. - qcd_rec_mu05 / qcd_gen_mu05);
  float qcd_mu20 = 1e2 * fabs(1. - qcd_rec_mu20 / qcd_gen_mu20);

  float alpha_265000 = 1e2 * fabs(1. - alpha_rec_265000 / alpha_gen_265000);
  float alpha_266000 = 1e2 * fabs(1. - alpha_rec_266000 / alpha_gen_266000);

  float pdf = 1e2 * h_pdfratio->GetRMS();

  float pdf_alpha = sqrt(pdf*pdf + (alpha_265000*alpha_265000 + alpha_266000*alpha_266000)/2.);


  printf("\n %s %s acceptance uncertainties\n", sample.Data(), jetbin.Data());
  printf("-----------------------------------------\n");
  printf(" QCD         mu=0.5 / mu=2.0   %4.2f%% / %4.2f%%\n", qcd_mu05, qcd_mu20);
  printf(" alpha_s     265000 / 266000   %4.2f%% / %4.2f%%\n", alpha_265000, alpha_266000);
  printf(" PDF                           %4.2f%%\n", pdf);
  printf(" PDF+alpha_s                   %4.2f%%\n", pdf_alpha);
  printf("\n");
}
