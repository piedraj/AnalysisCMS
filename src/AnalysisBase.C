#define AnalysisBase_cxx
#include "../include/AnalysisBase.h"


Int_t verbosity = 1;


void AnalysisBase::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nentriesMuonFail     = 0;
  Long64_t nentriesElectronFail = 0;

  printf("\n");
  printf(" nentries total         = %lld\n", nentries);

  if (nentries > 200) nentries = 200;  // Please remove me when debug has finished

  UInt_t nMuonLepton;      // Number of muons     in the Lepton   collection
  UInt_t nElectronLepton;  // Number of electrons in the Lepton   collection
  UInt_t nMuonPass;        // Number of muons     in the Muon     collection that pass the Lepton selection
  UInt_t nElectronPass;    // Number of electrons in the Electron collection that pass the Lepton selection

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);


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

    for (UInt_t i=0; i<nLepton; i++) {

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

    for (UInt_t j=0; j<nMuon; j++) {

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

    for (UInt_t k=0; k<nElectron; k++) {

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
  }

  if (verbosity > 0) printf("\n\n");

  printf(" nentries analyzed      = %lld\n", nentries);
  printf(" nentries muon     fail = %lld\n", nentriesMuonFail);
  printf(" nentries electron fail = %lld\n", nentriesElectronFail);
  printf("\n");
}
