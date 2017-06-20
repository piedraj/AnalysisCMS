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


#include "TCanvas.h"
#include "TCut.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1D.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include <fstream>
#include <iostream>
#include <vector>


// Constants
//------------------------------------------------------------------------------
const bool    _savefigures = true;
const TString _filename    = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__l2loose__hadd__l2tightOR__formulasMC/latino_GluGluHToWWTo2L2Nu_M125.root";


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l -b -q getPdfQcd.C+
//
// https://github.com/latinos/LatinoTrees/blob/master/AnalysisStep/src/WeightDumper.cc#L157
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getPdfQcd()
{
  printf("\n Reading %s\n\n", _filename.Data());

  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (_savefigures) gSystem->mkdir("figures", kTRUE);

  TFile* file = TFile::Open(_filename);

  TH1D* h_weights_gen = (TH1D*)file->Get("list_vectors_weights");


  // Fill h_weights_rec
  //----------------------------------------------------------------------------
  TH1D* h_weights_rec = (TH1D*)h_weights_gen->Clone("h_weights_rec");

  h_weights_rec->Reset();

  TTree* tree = (TTree*)file->Get("latino");

  TCut cut_hww2l2v_13TeV_of0j = "mll > 12 && (std_vector_lepton_pt[0] > 20) && (std_vector_lepton_pt[1] > 10) && (std_vector_lepton_pt[2] < 10) && (metPfType1 > 20) && (ptll > 30) && (std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -11*13) && (abs(std_vector_lepton_flavour[1]) == 13 || std_vector_lepton_pt[1] > 13) && (mth >= 60) && (std_vector_jet_pt[0] < 30) && (std_vector_jet_pt[0] < 20 || std_vector_jet_cmvav2[0] < -0.715) && ( std_vector_jet_pt[1] < 20 || std_vector_jet_cmvav2[1] < -0.715) && (std_vector_jet_pt[2] < 20 || std_vector_jet_cmvav2[2] < -0.715) && ( std_vector_jet_pt[3] < 20 || std_vector_jet_cmvav2[3] < -0.715) && (std_vector_jet_pt[4] < 20 || std_vector_jet_cmvav2[4] < -0.715) && (std_vector_jet_pt[5] < 20 || std_vector_jet_cmvav2[5] < -0.715) && (std_vector_jet_pt[6] < 20 || std_vector_jet_cmvav2[6] < -0.715) && (std_vector_jet_pt[7] < 20 || std_vector_jet_cmvav2[7] < -0.715) && (std_vector_jet_pt[8] < 20 || std_vector_jet_cmvav2[8] < -0.715) && (std_vector_jet_pt[9] < 20 || std_vector_jet_cmvav2[9] < -0.715)";

  TCanvas* canvas = new TCanvas("canvas", "canvas");

  if (_filename.Contains("latino"))
    tree->Draw("0*std_vector_LHE_weight+Iteration$+0.5>>h_weights_rec", "std_vector_LHE_weight[Iteration$]/std_vector_LHE_weight[0]"*cut_hww2l2v_13TeV_of0j);
  else
    tree->Draw("0*LHEweight+Iteration$+0.5>>h_weights_rec", "LHEweight[Iteration$]/LHEweight[0]"*cut_hww2l2v_13TeV_of0j);


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
  h_pdfratio->SetFillColor(kRed+1);
  h_pdfratio->SetFillStyle(  1001);
  h_pdfratio->SetLineColor(kRed+1);

  h_pdfratio->Draw("hist");

  h_pdfratio->SetXTitle("#frac{N_{rec}^{PDF} / N_{gen}^{PDF}}{N_{rec}^{nominal} / N_{gen}^{nominal}}");
  h_pdfratio->SetYTitle("entries / bin");

  h_pdfratio->GetXaxis()->SetTitleOffset(2.0);

  canvas->GetFrame()->DrawClone();

  if (_savefigures)
    {
      canvas->SaveAs("figures/pdfacceptance.pdf");
      canvas->SaveAs("figures/pdfacceptance.png");
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
  printf("\n Acceptance uncertainties\n");
  printf("-----------------------------------------\n");
  printf(" nominal acceptance * eff      %4.2f%%\n", 1e2 * h_weights_rec->GetBinContent(1) / h_weights_gen->GetBinContent(1));
  printf(" QCD         mu=0.5 / mu=2.0   %4.2f%% / %4.2f%%\n", qcd_mu05, qcd_mu20);
  printf(" alpha_s     265000 / 266000   %4.2f%% / %4.2f%%\n", alpha_265000, alpha_266000);
  printf(" PDF                           %4.2f%%\n", pdf);
  printf(" PDF+alpha_s                   %4.2f%%\n", pdf_alpha);
  printf("\n");
}
