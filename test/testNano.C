#define testNano_cxx
// The class definition in testNano.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("testNano.C")
// root> T->Process("testNano.C","some options")
// root> T->Process("testNano.C+")
//


#include "testNano.h"
#include <TH2.h>
#include <TStyle.h>

Int_t verbosity = 1;

Long64_t nentries             = 0;
Long64_t nentriesMuonFail     = 0;
Long64_t nentriesElectronFail = 0;

Int_t maxentries = 200;  // Set to -1 to consider all the events

void testNano::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void testNano::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

Bool_t testNano::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   //   if (entry > maxentries && maxentries > -1) Abort("Number of events exceeded the maximum given");  // Terminate() is not called
   if (entry > maxentries && maxentries > -1) return 0;                                                   // Terminate() is     called

   nentries++;

   UInt_t nMuonLepton;      // Number of muons     in the Lepton   collection
   UInt_t nElectronLepton;  // Number of electrons in the Lepton   collection
   UInt_t nMuonPass;        // Number of muons     in the Muon     collection that pass the Lepton selection
   UInt_t nElectronPass;    // Number of electrons in the Electron collection that pass the Lepton selection


   if (verbosity > 0) printf("\n------------------------------------------------\n");


   //
   // https://raw.githubusercontent.com/latinos/LatinoAnalysis/master/NanoGardener/python/data/LeptonSel_cfg.py
   //    Study2017
   //       FakeObjWP
   //


   // Lepton
   //--------------------------------------------------------------------------
   nMuonLepton     = 0;
   nElectronLepton = 0;

   for (UInt_t i=0; i<*nLepton; i++) {

     if (Lepton_electronIdx[i] > -1) nElectronLepton++;
     if (Lepton_muonIdx[i]     > -1) nMuonLepton++;
      
     if (verbosity > 0)
       printf(" lep %2d;  pt = %6.2f;  eta = %5.2f;  eIdx = %2d;  mIdx = %2d\n",
	      i,
	      Lepton_pt[i],
	      Lepton_eta[i],
	      Lepton_electronIdx[i],
	      Lepton_muonIdx[i]);
   }


   Bool_t pass;


   // Muon
   //--------------------------------------------------------------------------
   nMuonPass = 0;

   for (UInt_t j=0; j<*nMuon; j++) {

     pass = true;

     pass &= (fabs(Muon_eta[j]) < 2.4);
   //pass &= (Muon_mediumId[j] == 1);  // NOT USED
     pass &= (Muon_pfRelIso03_all[j] < 0.4);

     if (pass) nMuonPass++;

     if (verbosity > 0 && pass)
       printf(" muo %2d;  pt = %6.2f;  eta = %5.2f;  pass = %2d;  mediumId = %d;  pfRelIso03 = %5.2f\n",
	      j,
	      Muon_pt[j],
	      Muon_eta[j],
	      pass,
	      Muon_mediumId[j],
	      Muon_pfRelIso03_all[j]);
   }


   // Electron
   //--------------------------------------------------------------------------
   nElectronPass = 0;

   for (UInt_t k=0; k<*nElectron; k++) {

     pass = true;

   //pass &= (fabs(Electron_eta[k] < 2.5);  // NOT USED
     pass &= (Electron_lostHits[k] < 2);
     pass &= (Electron_convVeto[k] == 1);
     pass &= (Electron_eInvMinusPInv[k] < 0.013);
   //pass &= (Electron_pfRelIso03_all[k] < 0.06);  // NOT USED
     pass &= (Electron_dr03TkSumPt[k] < 0.1);
     pass &= (Electron_dr03HcalDepth1TowerSumEt[k]/Electron_pt[k] < 0.12);

     Bool_t passBarrel = true;

     passBarrel &= (fabs(Electron_eta[k]) <= 1.479);
   //passBarrel &= (fabs(Electron_dxy[k]) < 0.05);  // NOT USED
   //passBarrel &= (fabs(Electron_dz[k]) < 0.1);  // NOT USED
     passBarrel &= (fabs(Electron_deltaEtaSC[k]) < 0.004);
     passBarrel &= (Electron_sieie[k] < 0.011);
     passBarrel &= (Electron_hoe[k] < 0.06);
     passBarrel &= (Electron_dr03EcalRecHitSumEt[k]/Electron_pt[k] < 0.15);

     Bool_t passEndcap = true;

     passEndcap &= (fabs(Electron_eta[k]) > 1.479);
   //passEndcap &= (fabs(Electron_dxy[k]) < 0.1);  // NOT USED
   //passEndcap &= (fabs(Electron_dz[k]) < 0.2);  // NOT USED
     passEndcap &= (Electron_sieie[k] < 0.03);
     passEndcap &= (Electron_hoe[k] < 0.07);
     passEndcap &= (Electron_dr03EcalRecHitSumEt[k]/Electron_pt[k] < 0.13);

     pass &= (passBarrel || passEndcap);

     if (pass) nElectronPass++;

     if (verbosity > 0 && pass)
       printf(" ele %2d;  pt = %6.2f;  eta = %5.2f;  pass = %2d;  dxy = %.3f;  dz = %.3f\n",
	      k,
	      Electron_pt[k],
	      Electron_eta[k],
	      pass,
	      fabs(Electron_dxy[k]),
	      fabs(Electron_dz[k]));
   }


   // Events with some mismatch between Leptons, Electrons and Muons
   //--------------------------------------------------------------------------
   if (nMuonLepton != nMuonPass) {

     nentriesMuonFail++;

     if (verbosity > 0) printf("\n >>>>>>> nMuonLepton(%d) != nMuonPass(%d) <<<<<<<\n", nMuonLepton, nMuonPass);
   }

   if (nElectronLepton != nElectronPass) {

     nentriesElectronFail++;

     if (verbosity > 0) printf("\n +++++++ nElectronLepton(%d) != nElectronPass(%d) +++++++\n", nElectronLepton, nElectronPass);
   }
   
   return kTRUE;
}

void testNano::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
}

void testNano::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   if (verbosity > 0) printf("\n\n");

   printf(" nentries analyzed      = %lld\n", nentries);
   printf(" nentries muon     fail = %lld\n", nentriesMuonFail);
   printf(" nentries electron fail = %lld\n", nentriesElectronFail);
   printf("\n");
}
