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
void GetPdfQcdSyst(TString     sample,
		   TString     label,
		   TString     jetbin);

void DrawLatex    (Font_t      tfont,
		   Float_t     x,
		   Float_t     y,
		   Float_t     tsize,
		   Short_t     align,
		   const char* text,
		   Bool_t      setndc = true);


// Data members
//------------------------------------------------------------------------------
bool _savefigures;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getPdfQcd
//
//    WWTo2L2Nu 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      2.25%
//    QCD         mu=0.5 / mu=2.0   0.53% / 0.52%
//    alpha_s     265000 / 266000   0.02% / 0.02%
//    PDF                           0.25%
//    PDF+alpha_s                   0.25%
//   
//   
//    WWTo2L2Nu 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.85%
//    QCD         mu=0.5 / mu=2.0   1.54% / 1.38%
//    alpha_s     265000 / 266000   0.01% / 0.01%
//    PDF                           0.27%
//    PDF+alpha_s                   0.27%
//   
//   
//    VBFHToWWTo2L2Nu_M125 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.19%
//    QCD         mu=0.5 / mu=2.0   0.68% / 0.60%
//    alpha_s     265000 / 266000   1.14% / 0.82%
//    PDF                           0.51%
//    PDF+alpha_s                   1.12%
//   
//   
//    VBFHToWWTo2L2Nu_M125 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.80%
//    QCD         mu=0.5 / mu=2.0   0.11% / 0.01%
//    alpha_s     265000 / 266000   0.22% / 0.29%
//    PDF                           0.31%
//    PDF+alpha_s                   0.40%
//   
//   
//    GluGluHToWWTo2L2Nu_M125 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      1.14%
//    QCD         mu=0.5 / mu=2.0   2.71% / 2.26%
//    alpha_s     265000 / 266000   0.11% / 0.04%
//    PDF                           0.56%
//    PDF+alpha_s                   0.57%
//   
//   
//    GluGluHToWWTo2L2Nu_M125 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.54%
//    QCD         mu=0.5 / mu=2.0   2.00% / 1.66%
//    alpha_s     265000 / 266000   0.29% / 0.31%
//    PDF                           0.37%
//    PDF+alpha_s                   0.48%
//   
//   
//    WZTo3LNu 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.15%
//    QCD         mu=0.5 / mu=2.0   1.70% / 1.47%
//    alpha_s     265000 / 266000   0.15% / 0.26%
//    PDF                           0.48%
//    PDF+alpha_s                   0.52%
//   
//   
//    WZTo3LNu 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.14%
//    QCD         mu=0.5 / mu=2.0   2.94% / 2.57%
//    alpha_s     265000 / 266000   0.21% / 0.27%
//    PDF                           0.48%
//    PDF+alpha_s                   0.54%
//   
//   
//    HWminusJ_HToWW_M125 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.12%
//    QCD         mu=0.5 / mu=2.0   5.99% / 4.42%
//    alpha_s     265000 / 266000   0.29% / 0.28%
//    PDF                           0.55%
//    PDF+alpha_s                   0.62%
//   
//   
//    HWminusJ_HToWW_M125 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.12%
//    QCD         mu=0.5 / mu=2.0   0.99% / 1.36%
//    alpha_s     265000 / 266000   0.05% / 0.02%
//    PDF                           0.47%
//    PDF+alpha_s                   0.48%
//   
//   
//    HWplusJ_HToWW_M125 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.12%
//    QCD         mu=0.5 / mu=2.0   2.17% / 3.38%
//    alpha_s     265000 / 266000   0.20% / 0.13%
//    PDF                           0.47%
//    PDF+alpha_s                   0.50%
//   
//   
//    HWplusJ_HToWW_M125 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.12%
//    QCD         mu=0.5 / mu=2.0   4.71% / 3.58%
//    alpha_s     265000 / 266000   0.10% / 0.03%
//    PDF                           0.36%
//    PDF+alpha_s                   0.36%
//   
//   
//    HZJ_HToWW_M125 0jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.06%
//    QCD         mu=0.5 / mu=2.0   2.16% / 3.30%
//    alpha_s     265000 / 266000   0.06% / 0.30%
//    PDF                           1.29%
//    PDF+alpha_s                   1.31%
//   
//   
//    HZJ_HToWW_M125 1jet acceptance uncertainties
//   -----------------------------------------
//    nominal acceptance * eff      0.06%
//    QCD         mu=0.5 / mu=2.0   3.74% / 1.18%
//    alpha_s     265000 / 266000   0.12% / 0.03%
//    PDF                           0.88%
//    PDF+alpha_s                   0.89
//   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPdfQcd(bool savefigures = false)
{
  _savefigures = savefigures;
  
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  if (_savefigures) gSystem->mkdir("figures", kTRUE);

  GetPdfQcdSyst("WWTo2L2Nu", "WW", "0jet");
  GetPdfQcdSyst("WWTo2L2Nu", "WW", "1jet");

  GetPdfQcdSyst("VBFHToWWTo2L2Nu_M125", "qqH", "0jet");
  GetPdfQcdSyst("VBFHToWWTo2L2Nu_M125", "qqH", "1jet");

  GetPdfQcdSyst("GluGluHToWWTo2L2Nu_M125", "ggH", "0jet");
  GetPdfQcdSyst("GluGluHToWWTo2L2Nu_M125", "ggH", "1jet");

  GetPdfQcdSyst("WZTo3LNu", "WZ", "0jet");
  GetPdfQcdSyst("WZTo3LNu", "WZ", "1jet");

  GetPdfQcdSyst("HWminusJ_HToWW_M125", "HW-", "0jet");
  GetPdfQcdSyst("HWminusJ_HToWW_M125", "HW-", "1jet");

  GetPdfQcdSyst("HWplusJ_HToWW_M125", "HW+", "0jet");
  GetPdfQcdSyst("HWplusJ_HToWW_M125", "HW+", "1jet");

  GetPdfQcdSyst("HZJ_HToWW_M125", "HZ", "0jet");
  GetPdfQcdSyst("HZJ_HToWW_M125", "HZ", "1jet");
}


//------------------------------------------------------------------------------
// GetPdfQcdSyst
//------------------------------------------------------------------------------
void GetPdfQcdSyst(TString sample,
		   TString label,
		   TString jetbin)
{
  TFile* file = new TFile(_recdir + sample + ".root", "read");

  TH1F* h_weights_gen = (TH1F*)file->Get("list_vectors_weights_gen");
  TH1F* h_weights_rec = (TH1F*)file->Get("list_vectors_weights_" + jetbin);


  // Produce the QCD uncertainties
  //----------------------------------------------------------------------------
  float qcd_gen_mu05 = h_weights_gen->GetBinContent(9) / h_weights_gen->GetBinContent(1);
  float qcd_gen_mu20 = h_weights_gen->GetBinContent(5) / h_weights_gen->GetBinContent(1);

  float qcd_rec_mu05 = h_weights_rec->GetBinContent(9) / h_weights_rec->GetBinContent(1);
  float qcd_rec_mu20 = h_weights_rec->GetBinContent(5) / h_weights_rec->GetBinContent(1);


  // Produce the PDF uncertainties
  //----------------------------------------------------------------------------
  TH1D* h_pdfratio = new TH1D("h_pdfratio", "", 100, 0.965, 1.035);

  float denominator = h_weights_rec->GetBinContent(1) / h_weights_gen->GetBinContent(1);  // Nominal values

  for (int i=10; i<110; i++)
    {
      float numerator = h_weights_rec->GetBinContent(i) / h_weights_gen->GetBinContent(i);

      float ratio = numerator / denominator;

      h_pdfratio->Fill(ratio);
    }


  // Draw the PDF distribution
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(sample + "_" + jetbin, sample + "_" + jetbin);

  h_pdfratio->SetFillColor(kRed+1);
  h_pdfratio->SetFillStyle(  1001);
  h_pdfratio->SetLineColor(kRed+1);

  h_pdfratio->Draw("hist");

  h_pdfratio->SetXTitle("#frac{N_{rec}^{PDF} / N_{gen}^{PDF}}{N_{rec}^{nominal} / N_{gen}^{nominal}}");
  h_pdfratio->SetYTitle("entries / bin");

  h_pdfratio->GetXaxis()->SetTitleOffset(2.0);

  DrawLatex(42, 0.940, 0.945, 0.050, 31, label + " " + jetbin);

  canvas->GetFrame()->DrawClone();

  if (_savefigures)
    {
      canvas->SaveAs("figures/pdfacceptance_" + sample + "_" + jetbin + ".pdf");
      canvas->SaveAs("figures/pdfacceptance_" + sample + "_" + jetbin + ".png");
    }


  // Produce the alpha_s uncertainties
  //----------------------------------------------------------------------------
  float alpha_gen_265000 = h_weights_gen->GetBinContent(110) / h_weights_gen->GetBinContent(1);
  float alpha_gen_266000 = h_weights_gen->GetBinContent(111) / h_weights_gen->GetBinContent(1);

  float alpha_rec_265000 = h_weights_rec->GetBinContent(110) / h_weights_rec->GetBinContent(1);
  float alpha_rec_266000 = h_weights_rec->GetBinContent(111) / h_weights_rec->GetBinContent(1);


  // Prepare the final uncertainties
  //----------------------------------------------------------------------------
  float qcd_mu05 = 1e2 * fabs(1. - qcd_rec_mu05 / qcd_gen_mu05);
  float qcd_mu20 = 1e2 * fabs(1. - qcd_rec_mu20 / qcd_gen_mu20);

  float alpha_265000 = 1e2 * fabs(1. - alpha_rec_265000 / alpha_gen_265000);
  float alpha_266000 = 1e2 * fabs(1. - alpha_rec_266000 / alpha_gen_266000);

  float pdf = 1e2 * h_pdfratio->GetRMS();

  float pdf_alpha = sqrt(pdf*pdf + (alpha_265000*alpha_265000 + alpha_266000*alpha_266000)/2.);


  // Print the final uncertainties
  //----------------------------------------------------------------------------
  printf("\n %s %s acceptance uncertainties\n", sample.Data(), jetbin.Data());
  printf("-----------------------------------------\n");
  printf(" nominal acceptance * eff      %4.2f%%\n", 1e2 * h_weights_rec->GetBinContent(1) / h_weights_gen->GetBinContent(1));
  printf(" QCD         mu=0.5 / mu=2.0   %4.2f%% / %4.2f%%\n", qcd_mu05, qcd_mu20);
  printf(" alpha_s     265000 / 266000   %4.2f%% / %4.2f%%\n", alpha_265000, alpha_266000);
  printf(" PDF                           %4.2f%%\n", pdf);
  printf(" PDF+alpha_s                   %4.2f%%\n", pdf_alpha);
  printf("\n");
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
