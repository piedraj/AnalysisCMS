#define AnalysisBase_cxx
#include "../include/AnalysisBase.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void AnalysisBase::Loop()
{
  printf(" <<< Entering [AnalysisBase::Loop]\n");  // Debug

//   In a ROOT session, you can do:
//      root> .L AnalysisBase.C
//      root> AnalysisBase t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   printf(" nentries = %lld\n", nentries);  // Debug

   if (nentries > 200) nentries = 200;  // Debug. Please remove me when debug has finished

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


      if (nLepton == nMuon)     continue;
      if (nLepton == nElectron) continue;


      printf("\n------------------------------------------------\n");


      // Lepton
      //------------------------------------------------------------------------
      for (UInt_t i=0; i<nLepton; i++)
	printf(" Lepton i   = %d (nLepton   = %d);  pt = %6.2f;  eta = %5.2f;  eIdx = %2d;  mIdx = %2d\n",
	       i, nLepton, Lepton_pt[i], Lepton_eta[i], Lepton_electronIdx[i], Lepton_muonIdx[i]);


      // Muon
      //------------------------------------------------------------------------
      for (UInt_t j=0; j<nMuon; j++)
	printf(" Muon   j   = %d (nMuon     = %d);  pt = %6.2f;  eta = %5.2f\n", j, nMuon, Muon_pt[j], Muon_eta[j]);


      // Electron
      //------------------------------------------------------------------------
      for (UInt_t k=0; k<nElectron; k++)
	printf(" Electron k = %d (nElectron = %d);  pt = %6.2f;  eta = %5.2f\n", k, nElectron, Electron_pt[k], Electron_eta[k]);
   }
}
