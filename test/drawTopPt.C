#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "THStack.h"
#include "TString.h"
#include "TSystem.h"


const TString filename = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC/latino_TTTo2L2Nu__part7.root";


//------------------------------------------------------------------------------
//
//  root -l drawTopPt.C+
//
//------------------------------------------------------------------------------
void drawTopPt(bool write = true,
	       bool read  = true)
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  TH1::SetDefaultSumw2();


  // Write
  //----------------------------------------------------------------------------
  if (write) {

    TChain* chain = new TChain("latino", "latino");

    chain->Add(filename);

    TH1D* hist = new TH1D("toppt", "toppt", 160, 0.7, 1.1);

    TCanvas* canvas = new TCanvas("toppt", "toppt");

    TCut leppt   = "std_vector_lepton_pt[0] > 25 && std_vector_lepton_pt[1] > 20 && std_vector_lepton_pt[2] < 10";
    TCut twojets = "std_vector_jet_pt[0] > 30 && std_vector_jet_pt[1] > 30 && fabs(std_vector_jet_eta[0]) < 2.4 && fabs(std_vector_jet_eta[1]) < 2.4";
    TCut onebjet = "(std_vector_jet_csvv2ivf[0] > 0.8484 || std_vector_jet_csvv2ivf[1] > 0.8484)";
    TCut mm      = "(std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -169) && std_vector_muon_isTightLepton_cut_Tight80x[0] > 0.5 && std_vector_muon_isTightLepton_cut_Tight80x[1] > 0.5";
    TCut ee      = "(std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -121) && std_vector_electron_isTightLepton_cut_WP_Tight80X[0] > 0.5 && std_vector_electron_isTightLepton_cut_WP_Tight80X[1] > 0.5";

    TCut selection = ee && leppt && twojets && onebjet && "mll > 20 && fabs(mll - 91.188) > 15 && metPfType1 > 80";

    printf("\n Reading %s\n", filename.Data());
    printf("\n Applying the selection %s\n", selection.GetTitle());
    printf("\n");

    chain->Draw("sqrt(exp(0.123 - 0.0005 * (topLHEpt + antitopLHEpt)))>>toppt", selection);
    
    TFile* output = new TFile("toppt.root", "recreate");

    hist->Write();

    output->Close();

    canvas->Close();
  }


  // Read
  //----------------------------------------------------------------------------
  if (read) {

    gSystem->mkdir("png", kTRUE);

    TFile* input = new TFile("toppt.root", "read");

    TH1D* hist = (TH1D*)input->Get("toppt");

    TCanvas* canvas = new TCanvas("toppt", "toppt");

    hist->Draw("hist");

    hist->GetXaxis()->SetTitle("weight = #sqrt{exp(0.123 - 0.0005#upoint(p_{T}^{top} + p_{T}^{antitop}))}");
    hist->GetXaxis()->SetTitleOffset(1.7);
    hist->SetTitle("");

    hist->SetFillStyle(1001);
    hist->SetFillColor(kGray+1);
    hist->SetLineColor(kGray+1);

    canvas->GetFrame()->DrawClone();

    canvas->SaveAs("png/toppt.png");
  }
}
