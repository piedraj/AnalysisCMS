#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"


void GetSumOfWeightsLHE(TH1D* h_pdf, TH1D* h_qcd);


vector<float> *std_vector_LHE_weight;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// getPdfQcdGenSum
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void getPdfQcdGenSum(TString inputname = "WWTo2L2Nu__part0", TString outputname = "WWTo2L2Nu")
void getPdfQcdGenSum(TString inputname = "VBFHToWWTo2L2Nu_M125", TString outputname = "VBFHToWWTo2L2Nu_M125")
{
  TFile* output = new TFile(outputname + "_lheweights.root", "recreate");

  TH1D* h_pdfsum_gen = new TH1D("h_pdfsum_gen", "", 100, 0, 100);
  TH1D* h_qcdsum_gen = new TH1D("h_qcdsum_gen", "",   9, 0,   9);

  TFile* file = new TFile("eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/22Jan/MC/25ns/LatinoTrees/latino_" + inputname + ".root", "read");

  TTree* tree = (TTree*)file->Get("myTree");

  std_vector_LHE_weight = 0;

  tree->SetBranchAddress("weightsLHE", &std_vector_LHE_weight);


  // Loop over the tree
  //----------------------------------------------------------------------------
  Long64_t nentries = tree->GetEntries();

  for (Long64_t i=0; i<nentries; i++) {

    tree->GetEntry(i);

    GetSumOfWeightsLHE(h_pdfsum_gen, h_qcdsum_gen);
  }


  // Save the money
  //----------------------------------------------------------------------------
  output->cd();

  h_pdfsum_gen->Write();
  h_qcdsum_gen->Write();

  output->Close();
}


void GetSumOfWeightsLHE(TH1D* h_pdf, TH1D* h_qcd)
{
  if (!std_vector_LHE_weight) return;

  for (int i=0; i<h_pdf->GetNbinsX(); i++)
    {
      h_pdf->Fill(i, std_vector_LHE_weight->at(i+9));
    }

  for (int i=0; i<h_qcd->GetNbinsX(); i++)
    {
      h_qcd->Fill(i, std_vector_LHE_weight->at(i));
    }
}
