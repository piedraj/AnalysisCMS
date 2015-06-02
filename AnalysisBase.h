//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun  2 18:15:45 2015 by ROOT version 5.34/18
// from TTree latino/probe_tree
// found on file: ../../latino/latino_WZJets.root
//////////////////////////////////////////////////////////

#ifndef AnalysisBase_h
#define AnalysisBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PxPyPzE4D.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class AnalysisBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         baseW;
   Float_t         bdt1;
   Float_t         bdt2;
   Float_t         bdt3;
   Float_t         bdt4;
   Float_t         ch1;
   Float_t         ch2;
   Float_t         ch3;
   Float_t         ch4;
   Float_t         chSumEt;
   Float_t         channel;
   Float_t         chmet;
   Float_t         chmetphi;
   Float_t         cjeteta1;
   Float_t         cjeteta2;
   Float_t         cjetid1;
   Float_t         cjetid2;
   Float_t         cjetmva1;
   Float_t         cjetmva2;
   Float_t         cjetphi1;
   Float_t         cjetphi2;
   Float_t         cjetpt1;
   Float_t         cjetpt2;
   Float_t         dataset;
   Float_t         detajj;
   Float_t         dphill;
   Float_t         dphilljet;
   Float_t         dphilljetjet;
   Float_t         dphillmet;
   Float_t         dphilmet;
   Float_t         dphilmet1;
   Float_t         dphilmet2;
   Float_t         drll;
   Float_t         dymva0;
   Float_t         dymva1;
   Float_t         effAW;
   Float_t         effBW;
   Float_t         effW;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         eta3;
   Float_t         eta4;
   Float_t         fakeAW;
   Float_t         fakeBW;
   Float_t         fakeW;
   Float_t         gammaMRStar;
   Float_t         hardbjpb;
   Float_t         hardcmva;
   Float_t         hardcsvv2ivf;
   Float_t         hardpfcsv;
   Float_t         hardssvhb;
   Float_t         hardssvhe;
   Float_t         hardtche;
   Float_t         higggsGenpt;
   Float_t         imet;
   Float_t         iso1;
   Float_t         iso2;
   Float_t         iso3;
   Float_t         iso4;
   Float_t         isomva1;
   Float_t         isomva2;
   Float_t         isomva3;
   Float_t         isomva4;
   Float_t         jetCHM1;
   Float_t         jetCHM2;
   Float_t         jetCHM3;
   Float_t         jetCHM4;
   Float_t         jetCHM5;
   Float_t         jetCHM6;
   Float_t         jetCHM7;
   Float_t         jetGenPartoneta1;
   Float_t         jetGenPartoneta2;
   Float_t         jetGenPartoneta3;
   Float_t         jetGenPartonphi1;
   Float_t         jetGenPartonphi2;
   Float_t         jetGenPartonphi3;
   Float_t         jetGenPartonpid1;
   Float_t         jetGenPartonpid2;
   Float_t         jetGenPartonpid3;
   Float_t         jetGenPartonpt1;
   Float_t         jetGenPartonpt2;
   Float_t         jetGenPartonpt3;
   Float_t         jetNChgQC1;
   Float_t         jetNChgQC2;
   Float_t         jetNChgQC3;
   Float_t         jetNChgQC4;
   Float_t         jetNChgQC5;
   Float_t         jetNChgQC6;
   Float_t         jetNChgQC7;
   Float_t         jetNChgptCut1;
   Float_t         jetNChgptCut2;
   Float_t         jetNChgptCut3;
   Float_t         jetNChgptCut4;
   Float_t         jetNChgptCut5;
   Float_t         jetNChgptCut6;
   Float_t         jetNChgptCut7;
   Float_t         jetNHM1;
   Float_t         jetNHM2;
   Float_t         jetNHM3;
   Float_t         jetNHM4;
   Float_t         jetNHM5;
   Float_t         jetNHM6;
   Float_t         jetNHM7;
   Float_t         jetNNeutralptCut1;
   Float_t         jetNNeutralptCut2;
   Float_t         jetNNeutralptCut3;
   Float_t         jetNNeutralptCut4;
   Float_t         jetNNeutralptCut5;
   Float_t         jetNNeutralptCut6;
   Float_t         jetNNeutralptCut7;
   Float_t         jetPhM1;
   Float_t         jetPhM2;
   Float_t         jetPhM3;
   Float_t         jetPhM4;
   Float_t         jetPhM5;
   Float_t         jetPhM6;
   Float_t         jetPhM7;
   Float_t         jetRho;
   Float_t         jetbjpb1;
   Float_t         jetbjpb2;
   Float_t         jetbjpb3;
   Float_t         jetbjpb4;
   Float_t         jetbjpb5;
   Float_t         jetbjpb6;
   Float_t         jetbjpb7;
   Float_t         jetcmva1;
   Float_t         jetcmva2;
   Float_t         jetcsvv2ivf1;
   Float_t         jetcsvv2ivf2;
   Float_t         jeteta1;
   Float_t         jeteta2;
   Float_t         jeteta3;
   Float_t         jeteta4;
   Float_t         jeteta5;
   Float_t         jeteta6;
   Float_t         jeteta7;
   Float_t         jetid1;
   Float_t         jetid2;
   Float_t         jetid3;
   Float_t         jetid4;
   Float_t         jetid5;
   Float_t         jetid6;
   Float_t         jetid7;
   Float_t         jetmass1;
   Float_t         jetmass2;
   Float_t         jetmass3;
   Float_t         jetmass4;
   Float_t         jetmass5;
   Float_t         jetmass6;
   Float_t         jetmass7;
   Float_t         jetmva1;
   Float_t         jetmva2;
   Float_t         jetmva3;
   Float_t         jetmva4;
   Float_t         jetmva5;
   Float_t         jetmva6;
   Float_t         jetmva7;
   Float_t         jetpfcsv1;
   Float_t         jetpfcsv2;
   Float_t         jetphi1;
   Float_t         jetphi2;
   Float_t         jetphi3;
   Float_t         jetphi4;
   Float_t         jetphi5;
   Float_t         jetphi6;
   Float_t         jetphi7;
   Float_t         jetpt1;
   Float_t         jetpt2;
   Float_t         jetpt3;
   Float_t         jetpt4;
   Float_t         jetpt5;
   Float_t         jetpt6;
   Float_t         jetpt7;
   Float_t         jetptd1;
   Float_t         jetptd2;
   Float_t         jetptd3;
   Float_t         jetptd4;
   Float_t         jetptd5;
   Float_t         jetptd6;
   Float_t         jetptd7;
   Float_t         jetssvhb1;
   Float_t         jetssvhb2;
   Float_t         jetssvhe1;
   Float_t         jetssvhe2;
   Float_t         jettche1;
   Float_t         jettche2;
   Float_t         jettche3;
   Float_t         jettche4;
   Float_t         jettche5;
   Float_t         jettche6;
   Float_t         jettche7;
   Float_t         jettchp1;
   Float_t         jettchp2;
   Float_t         jettchp3;
   Float_t         jettchp4;
   Float_t         jettchp5;
   Float_t         jettchp6;
   Float_t         jettchp7;
   Float_t         kfW;
   Float_t         lh1;
   Float_t         lh2;
   Float_t         lh3;
   Float_t         lh4;
   Float_t         mctruth;
   Float_t         metGeneta;
   Float_t         metGenphi;
   Float_t         metGenpt;
   Float_t         mjj;
   Float_t         mll;
   Float_t         mllg;
   Float_t         mllgid;
   Float_t         mpmet;
   Float_t         mth;
   Float_t         mtw1;
   Float_t         mtw2;
   Float_t         nPhos;
   Float_t         nbjet;
   Float_t         nbjettche;
   Float_t         nbrem1;
   Float_t         nbrem2;
   Float_t         nbrem3;
   Float_t         nbrem4;
   Float_t         neutrinoGeneta1;
   Float_t         neutrinoGeneta2;
   Float_t         neutrinoGeneta3;
   Float_t         neutrinoGenphi1;
   Float_t         neutrinoGenphi2;
   Float_t         neutrinoGenphi3;
   Float_t         neutrinoGenpid1;
   Float_t         neutrinoGenpid2;
   Float_t         neutrinoGenpid3;
   Float_t         neutrinoGenpt1;
   Float_t         neutrinoGenpt2;
   Float_t         neutrinoGenpt3;
   Float_t         nextra;
   Float_t         njet;
   Float_t         njetid;
   Float_t         njetvbf;
   Float_t         pchmet;
   Float_t         pdfid1;
   Float_t         pdfid2;
   Float_t         pdfscalePDF;
   Float_t         pdfx1;
   Float_t         pdfx1PDF;
   Float_t         pdfx2;
   Float_t         pdfx2PDF;
   Float_t         pdgid1;
   Float_t         pdgid2;
   Float_t         pdgid3;
   Float_t         pdgid4;
   Float_t         peaking;
   Float_t         pfRawMet;
   Float_t         pfRawMetPhi;
   Float_t         pfRawSumEt;
   Float_t         pfType1Met;
   Float_t         pfType1Metdn;
   Float_t         pfType1Metphi;
   Float_t         pfType1Metup;
   Float_t         pfType1SumEt;
   Float_t         pfmetMEtSig;
   Float_t         pfmetSignificance;
   Float_t         phi1;
   Float_t         phi2;
   Float_t         phi3;
   Float_t         phi4;
   Float_t         pho_n_id;
   Float_t         ppfMet;
   Float_t         predmet;
   Float_t         pt1;
   Float_t         pt2;
   Float_t         pt3;
   Float_t         pt4;
   Float_t         ptll;
   Float_t         puW;
   Float_t         pupMet;
   Float_t         puppijetpt1;
   Float_t         redmet;
   Float_t         sceta1;
   Float_t         sceta2;
   Float_t         sceta3;
   Float_t         sceta4;
   Float_t         softbjpb;
   Float_t         softcmva;
   Float_t         softcsvv2ivf;
   Float_t         softpfcsv;
   Float_t         softssvhb;
   Float_t         softssvhe;
   Float_t         softtche;
   vector<float>   *std_vector_HoE;
   vector<float>   *std_vector_deltaEtaIn;
   vector<float>   *std_vector_deltaPhiIn;
   vector<float>   *std_vector_effectiveArea;
   vector<float>   *std_vector_jetGen_eta;
   vector<float>   *std_vector_jetGen_phi;
   vector<float>   *std_vector_jetGen_pt;
   vector<float>   *std_vector_jet_HadronFlavour;
   vector<float>   *std_vector_jet_PartonFlavour;
   vector<float>   *std_vector_jet_QGRMScand;
   vector<float>   *std_vector_jet_QGRmax;
   vector<float>   *std_vector_jet_QGaxis1;
   vector<float>   *std_vector_jet_QGaxis2;
   vector<float>   *std_vector_jet_QGlikelihood;
   vector<float>   *std_vector_jet_bjpb;
   vector<float>   *std_vector_jet_cmva;
   vector<float>   *std_vector_jet_csvv2ivf;
   vector<float>   *std_vector_jet_eta;
   vector<float>   *std_vector_jet_pfcsv;
   vector<float>   *std_vector_jet_phi;
   vector<float>   *std_vector_jet_pt;
   vector<float>   *std_vector_jet_ssvhb;
   vector<float>   *std_vector_jet_ssvhe;
   vector<float>   *std_vector_jet_tche;
   vector<float>   *std_vector_jet_tchp;
   vector<float>   *std_vector_leptonGen_eta;
   vector<float>   *std_vector_leptonGen_index;
   vector<float>   *std_vector_leptonGen_mpid;
   vector<float>   *std_vector_leptonGen_mstatus;
   vector<float>   *std_vector_leptonGen_phi;
   vector<float>   *std_vector_leptonGen_pid;
   vector<float>   *std_vector_leptonGen_pt;
   vector<float>   *std_vector_leptonGen_status;
   vector<float>   *std_vector_lepton_BestTrackdxy;
   vector<float>   *std_vector_lepton_BestTrackdz;
   vector<float>   *std_vector_lepton_Chi2LocalPos;
   vector<float>   *std_vector_lepton_NTkLayers;
   vector<float>   *std_vector_lepton_NValidFractInTrk;
   vector<float>   *std_vector_lepton_NValidHitsInTrk;
   vector<float>   *std_vector_lepton_NValidHitsSATrk;
   vector<float>   *std_vector_lepton_NValidPixelHitsInTrk;
   vector<float>   *std_vector_lepton_NormChi2GTrk;
   vector<float>   *std_vector_lepton_NumOfMatchedStations;
   vector<float>   *std_vector_lepton_SegCompatibilty;
   vector<float>   *std_vector_lepton_TrkKink;
   vector<float>   *std_vector_lepton_chargedHadronIso;
   vector<float>   *std_vector_lepton_chargedParticleIso;
   vector<float>   *std_vector_lepton_elSIP3D;
   vector<float>   *std_vector_lepton_eta;
   vector<float>   *std_vector_lepton_id;
   vector<float>   *std_vector_lepton_isTightMuon;
   vector<float>   *std_vector_lepton_muSIP3D;
   vector<float>   *std_vector_lepton_neutralHadronIso;
   vector<float>   *std_vector_lepton_phi;
   vector<float>   *std_vector_lepton_photonIso;
   vector<float>   *std_vector_lepton_pt;
   vector<float>   *std_vector_lepton_sumPUPt;
   vector<float>   *std_vector_numHits;
   vector<float>   *std_vector_puppijet_eta;
   vector<float>   *std_vector_puppijet_phi;
   vector<float>   *std_vector_puppijet_pt;
   vector<float>   *std_vector_sigmaIetaIeta;
   vector<float>   *std_vector_tau_eta;
   vector<float>   *std_vector_tau_phi;
   vector<float>   *std_vector_tau_pt;
   Float_t         tightmu;
   Float_t         triggAW;
   Float_t         triggBW;
   Float_t         triggW;
   Float_t         trigger;
   Float_t         trkMet;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet1;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet2;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet3;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet4;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton1;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton2;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton3;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton4;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_photon1;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_photon1id;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_photon2;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
// //ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_photon3;
//   Double_t        fCoordinates_fX;
//   Double_t        fCoordinates_fY;
//   Double_t        fCoordinates_fZ;
//   Double_t        fCoordinates_fT;
   Float_t         worstJetLepPt;
   Float_t         yll;
   Float_t         itpu;
   Float_t         nvtx;
   Float_t         ootpum1;
   Float_t         ootpum2;
   Float_t         ootpum3;
   Float_t         ootpum4;
   Float_t         ootpum5;
   Float_t         ootpup1;
   Float_t         ootpup2;
   Float_t         ootpup3;
   Float_t         trpu;
   Int_t           bveto;
   Int_t           bveto_ip;
   Int_t           bveto_mu;
   Int_t           bveto_munj;
   Int_t           bveto_munj05;
   Int_t           bveto_munj30;
   Int_t           bveto_munj3005;
   Int_t           bveto_nj;
   Int_t           bveto_nj05;
   Int_t           bveto_nj30;
   Int_t           bveto_nj3005;
   Int_t           dphiveto;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          event;

   // List of branches
   TBranch        *b_baseW;   //!
   TBranch        *b_bdt1;   //!
   TBranch        *b_bdt2;   //!
   TBranch        *b_bdt3;   //!
   TBranch        *b_bdt4;   //!
   TBranch        *b_ch1;   //!
   TBranch        *b_ch2;   //!
   TBranch        *b_ch3;   //!
   TBranch        *b_ch4;   //!
   TBranch        *b_chSumEt;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_chmet;   //!
   TBranch        *b_chmetphi;   //!
   TBranch        *b_cjeteta1;   //!
   TBranch        *b_cjeteta2;   //!
   TBranch        *b_cjetid1;   //!
   TBranch        *b_cjetid2;   //!
   TBranch        *b_cjetmva1;   //!
   TBranch        *b_cjetmva2;   //!
   TBranch        *b_cjetphi1;   //!
   TBranch        *b_cjetphi2;   //!
   TBranch        *b_cjetpt1;   //!
   TBranch        *b_cjetpt2;   //!
   TBranch        *b_dataset;   //!
   TBranch        *b_detajj;   //!
   TBranch        *b_dphill;   //!
   TBranch        *b_dphilljet;   //!
   TBranch        *b_dphilljetjet;   //!
   TBranch        *b_dphillmet;   //!
   TBranch        *b_dphilmet;   //!
   TBranch        *b_dphilmet1;   //!
   TBranch        *b_dphilmet2;   //!
   TBranch        *b_drll;   //!
   TBranch        *b_dymva0;   //!
   TBranch        *b_dymva1;   //!
   TBranch        *b_effAW;   //!
   TBranch        *b_effBW;   //!
   TBranch        *b_effW;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_eta3;   //!
   TBranch        *b_eta4;   //!
   TBranch        *b_fakeAW;   //!
   TBranch        *b_fakeBW;   //!
   TBranch        *b_fakeW;   //!
   TBranch        *b_gammaMRStar;   //!
   TBranch        *b_hardbjpb;   //!
   TBranch        *b_hardcmva;   //!
   TBranch        *b_hardcsvv2ivf;   //!
   TBranch        *b_hardpfcsv;   //!
   TBranch        *b_hardssvhb;   //!
   TBranch        *b_hardssvhe;   //!
   TBranch        *b_hardtche;   //!
   TBranch        *b_higggsGenpt;   //!
   TBranch        *b_imet;   //!
   TBranch        *b_iso1;   //!
   TBranch        *b_iso2;   //!
   TBranch        *b_iso3;   //!
   TBranch        *b_iso4;   //!
   TBranch        *b_isomva1;   //!
   TBranch        *b_isomva2;   //!
   TBranch        *b_isomva3;   //!
   TBranch        *b_isomva4;   //!
   TBranch        *b_jetCHM1;   //!
   TBranch        *b_jetCHM2;   //!
   TBranch        *b_jetCHM3;   //!
   TBranch        *b_jetCHM4;   //!
   TBranch        *b_jetCHM5;   //!
   TBranch        *b_jetCHM6;   //!
   TBranch        *b_jetCHM7;   //!
   TBranch        *b_jetGenPartoneta1;   //!
   TBranch        *b_jetGenPartoneta2;   //!
   TBranch        *b_jetGenPartoneta3;   //!
   TBranch        *b_jetGenPartonphi1;   //!
   TBranch        *b_jetGenPartonphi2;   //!
   TBranch        *b_jetGenPartonphi3;   //!
   TBranch        *b_jetGenPartonpid1;   //!
   TBranch        *b_jetGenPartonpid2;   //!
   TBranch        *b_jetGenPartonpid3;   //!
   TBranch        *b_jetGenPartonpt1;   //!
   TBranch        *b_jetGenPartonpt2;   //!
   TBranch        *b_jetGenPartonpt3;   //!
   TBranch        *b_jetNChgQC1;   //!
   TBranch        *b_jetNChgQC2;   //!
   TBranch        *b_jetNChgQC3;   //!
   TBranch        *b_jetNChgQC4;   //!
   TBranch        *b_jetNChgQC5;   //!
   TBranch        *b_jetNChgQC6;   //!
   TBranch        *b_jetNChgQC7;   //!
   TBranch        *b_jetNChgptCut1;   //!
   TBranch        *b_jetNChgptCut2;   //!
   TBranch        *b_jetNChgptCut3;   //!
   TBranch        *b_jetNChgptCut4;   //!
   TBranch        *b_jetNChgptCut5;   //!
   TBranch        *b_jetNChgptCut6;   //!
   TBranch        *b_jetNChgptCut7;   //!
   TBranch        *b_jetNHM1;   //!
   TBranch        *b_jetNHM2;   //!
   TBranch        *b_jetNHM3;   //!
   TBranch        *b_jetNHM4;   //!
   TBranch        *b_jetNHM5;   //!
   TBranch        *b_jetNHM6;   //!
   TBranch        *b_jetNHM7;   //!
   TBranch        *b_jetNNeutralptCut1;   //!
   TBranch        *b_jetNNeutralptCut2;   //!
   TBranch        *b_jetNNeutralptCut3;   //!
   TBranch        *b_jetNNeutralptCut4;   //!
   TBranch        *b_jetNNeutralptCut5;   //!
   TBranch        *b_jetNNeutralptCut6;   //!
   TBranch        *b_jetNNeutralptCut7;   //!
   TBranch        *b_jetPhM1;   //!
   TBranch        *b_jetPhM2;   //!
   TBranch        *b_jetPhM3;   //!
   TBranch        *b_jetPhM4;   //!
   TBranch        *b_jetPhM5;   //!
   TBranch        *b_jetPhM6;   //!
   TBranch        *b_jetPhM7;   //!
   TBranch        *b_jetRho;   //!
   TBranch        *b_jetbjpb1;   //!
   TBranch        *b_jetbjpb2;   //!
   TBranch        *b_jetbjpb3;   //!
   TBranch        *b_jetbjpb4;   //!
   TBranch        *b_jetbjpb5;   //!
   TBranch        *b_jetbjpb6;   //!
   TBranch        *b_jetbjpb7;   //!
   TBranch        *b_jetcmva1;   //!
   TBranch        *b_jetcmva2;   //!
   TBranch        *b_jetcsvv2ivf1;   //!
   TBranch        *b_jetcsvv2ivf2;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_jeteta3;   //!
   TBranch        *b_jeteta4;   //!
   TBranch        *b_jeteta5;   //!
   TBranch        *b_jeteta6;   //!
   TBranch        *b_jeteta7;   //!
   TBranch        *b_jetid1;   //!
   TBranch        *b_jetid2;   //!
   TBranch        *b_jetid3;   //!
   TBranch        *b_jetid4;   //!
   TBranch        *b_jetid5;   //!
   TBranch        *b_jetid6;   //!
   TBranch        *b_jetid7;   //!
   TBranch        *b_jetmass1;   //!
   TBranch        *b_jetmass2;   //!
   TBranch        *b_jetmass3;   //!
   TBranch        *b_jetmass4;   //!
   TBranch        *b_jetmass5;   //!
   TBranch        *b_jetmass6;   //!
   TBranch        *b_jetmass7;   //!
   TBranch        *b_jetmva1;   //!
   TBranch        *b_jetmva2;   //!
   TBranch        *b_jetmva3;   //!
   TBranch        *b_jetmva4;   //!
   TBranch        *b_jetmva5;   //!
   TBranch        *b_jetmva6;   //!
   TBranch        *b_jetmva7;   //!
   TBranch        *b_jetpfcsv1;   //!
   TBranch        *b_jetpfcsv2;   //!
   TBranch        *b_jetphi1;   //!
   TBranch        *b_jetphi2;   //!
   TBranch        *b_jetphi3;   //!
   TBranch        *b_jetphi4;   //!
   TBranch        *b_jetphi5;   //!
   TBranch        *b_jetphi6;   //!
   TBranch        *b_jetphi7;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jetpt3;   //!
   TBranch        *b_jetpt4;   //!
   TBranch        *b_jetpt5;   //!
   TBranch        *b_jetpt6;   //!
   TBranch        *b_jetpt7;   //!
   TBranch        *b_jetptd1;   //!
   TBranch        *b_jetptd2;   //!
   TBranch        *b_jetptd3;   //!
   TBranch        *b_jetptd4;   //!
   TBranch        *b_jetptd5;   //!
   TBranch        *b_jetptd6;   //!
   TBranch        *b_jetptd7;   //!
   TBranch        *b_jetssvhb1;   //!
   TBranch        *b_jetssvhb2;   //!
   TBranch        *b_jetssvhe1;   //!
   TBranch        *b_jetssvhe2;   //!
   TBranch        *b_jettche1;   //!
   TBranch        *b_jettche2;   //!
   TBranch        *b_jettche3;   //!
   TBranch        *b_jettche4;   //!
   TBranch        *b_jettche5;   //!
   TBranch        *b_jettche6;   //!
   TBranch        *b_jettche7;   //!
   TBranch        *b_jettchp1;   //!
   TBranch        *b_jettchp2;   //!
   TBranch        *b_jettchp3;   //!
   TBranch        *b_jettchp4;   //!
   TBranch        *b_jettchp5;   //!
   TBranch        *b_jettchp6;   //!
   TBranch        *b_jettchp7;   //!
   TBranch        *b_kfW;   //!
   TBranch        *b_lh1;   //!
   TBranch        *b_lh2;   //!
   TBranch        *b_lh3;   //!
   TBranch        *b_lh4;   //!
   TBranch        *b_mctruth;   //!
   TBranch        *b_metGeneta;   //!
   TBranch        *b_metGenphi;   //!
   TBranch        *b_metGenpt;   //!
   TBranch        *b_mjj;   //!
   TBranch        *b_mll;   //!
   TBranch        *b_mllg;   //!
   TBranch        *b_mllgid;   //!
   TBranch        *b_mpmet;   //!
   TBranch        *b_mth;   //!
   TBranch        *b_mtw1;   //!
   TBranch        *b_mtw2;   //!
   TBranch        *b_nPhos;   //!
   TBranch        *b_nbjet;   //!
   TBranch        *b_nbjettche;   //!
   TBranch        *b_nbrem1;   //!
   TBranch        *b_nbrem2;   //!
   TBranch        *b_nbrem3;   //!
   TBranch        *b_nbrem4;   //!
   TBranch        *b_neutrinoGeneta1;   //!
   TBranch        *b_neutrinoGeneta2;   //!
   TBranch        *b_neutrinoGeneta3;   //!
   TBranch        *b_neutrinoGenphi1;   //!
   TBranch        *b_neutrinoGenphi2;   //!
   TBranch        *b_neutrinoGenphi3;   //!
   TBranch        *b_neutrinoGenpid1;   //!
   TBranch        *b_neutrinoGenpid2;   //!
   TBranch        *b_neutrinoGenpid3;   //!
   TBranch        *b_neutrinoGenpt1;   //!
   TBranch        *b_neutrinoGenpt2;   //!
   TBranch        *b_neutrinoGenpt3;   //!
   TBranch        *b_nextra;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_njetid;   //!
   TBranch        *b_njetvbf;   //!
   TBranch        *b_pchmet;   //!
   TBranch        *b_pdfid1;   //!
   TBranch        *b_pdfid2;   //!
   TBranch        *b_pdfscalePDF;   //!
   TBranch        *b_pdfx1;   //!
   TBranch        *b_pdfx1PDF;   //!
   TBranch        *b_pdfx2;   //!
   TBranch        *b_pdfx2PDF;   //!
   TBranch        *b_pdgid1;   //!
   TBranch        *b_pdgid2;   //!
   TBranch        *b_pdgid3;   //!
   TBranch        *b_pdgid4;   //!
   TBranch        *b_peaking;   //!
   TBranch        *b_pfRawMet;   //!
   TBranch        *b_pfRawMetPhi;   //!
   TBranch        *b_pfRawSumEt;   //!
   TBranch        *b_pfType1Met;   //!
   TBranch        *b_pfType1Metdn;   //!
   TBranch        *b_pfType1Metphi;   //!
   TBranch        *b_pfType1Metup;   //!
   TBranch        *b_pfType1SumEt;   //!
   TBranch        *b_pfmetMEtSig;   //!
   TBranch        *b_pfmetSignificance;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_phi3;   //!
   TBranch        *b_phi4;   //!
   TBranch        *b_pho_n_id;   //!
   TBranch        *b_ppfMet;   //!
   TBranch        *b_predmet;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_pt3;   //!
   TBranch        *b_pt4;   //!
   TBranch        *b_ptll;   //!
   TBranch        *b_puW;   //!
   TBranch        *b_pupMet;   //!
   TBranch        *b_puppijetpt1;   //!
   TBranch        *b_redmet;   //!
   TBranch        *b_sceta1;   //!
   TBranch        *b_sceta2;   //!
   TBranch        *b_sceta3;   //!
   TBranch        *b_sceta4;   //!
   TBranch        *b_softbjpb;   //!
   TBranch        *b_softcmva;   //!
   TBranch        *b_softcsvv2ivf;   //!
   TBranch        *b_softpfcsv;   //!
   TBranch        *b_softssvhb;   //!
   TBranch        *b_softssvhe;   //!
   TBranch        *b_softtche;   //!
   TBranch        *b_std_vector_HoE;   //!
   TBranch        *b_std_vector_deltaEtaIn;   //!
   TBranch        *b_std_vector_deltaPhiIn;   //!
   TBranch        *b_std_vector_effectiveArea;   //!
   TBranch        *b_std_vector_jetGen_eta;   //!
   TBranch        *b_std_vector_jetGen_phi;   //!
   TBranch        *b_std_vector_jetGen_pt;   //!
   TBranch        *b_std_vector_jet_HadronFlavour;   //!
   TBranch        *b_std_vector_jet_PartonFlavour;   //!
   TBranch        *b_std_vector_jet_QGRMScand;   //!
   TBranch        *b_std_vector_jet_QGRmax;   //!
   TBranch        *b_std_vector_jet_QGaxis1;   //!
   TBranch        *b_std_vector_jet_QGaxis2;   //!
   TBranch        *b_std_vector_jet_QGlikelihood;   //!
   TBranch        *b_std_vector_jet_bjpb;   //!
   TBranch        *b_std_vector_jet_cmva;   //!
   TBranch        *b_std_vector_jet_csvv2ivf;   //!
   TBranch        *b_std_vector_jet_eta;   //!
   TBranch        *b_std_vector_jet_pfcsv;   //!
   TBranch        *b_std_vector_jet_phi;   //!
   TBranch        *b_std_vector_jet_pt;   //!
   TBranch        *b_std_vector_jet_ssvhb;   //!
   TBranch        *b_std_vector_jet_ssvhe;   //!
   TBranch        *b_std_vector_jet_tche;   //!
   TBranch        *b_std_vector_jet_tchp;   //!
   TBranch        *b_std_vector_leptonGen_eta;   //!
   TBranch        *b_std_vector_leptonGen_index;   //!
   TBranch        *b_std_vector_leptonGen_mpid;   //!
   TBranch        *b_std_vector_leptonGen_mstatus;   //!
   TBranch        *b_std_vector_leptonGen_phi;   //!
   TBranch        *b_std_vector_leptonGen_pid;   //!
   TBranch        *b_std_vector_leptonGen_pt;   //!
   TBranch        *b_std_vector_leptonGen_status;   //!
   TBranch        *b_std_vector_lepton_BestTrackdxy;   //!
   TBranch        *b_std_vector_lepton_BestTrackdz;   //!
   TBranch        *b_std_vector_lepton_Chi2LocalPos;   //!
   TBranch        *b_std_vector_lepton_NTkLayers;   //!
   TBranch        *b_std_vector_lepton_NValidFractInTrk;   //!
   TBranch        *b_std_vector_lepton_NValidHitsInTrk;   //!
   TBranch        *b_std_vector_lepton_NValidHitsSATrk;   //!
   TBranch        *b_std_vector_lepton_NValidPixelHitsInTrk;   //!
   TBranch        *b_std_vector_lepton_NormChi2GTrk;   //!
   TBranch        *b_std_vector_lepton_NumOfMatchedStations;   //!
   TBranch        *b_std_vector_lepton_SegCompatibilty;   //!
   TBranch        *b_std_vector_lepton_TrkKink;   //!
   TBranch        *b_std_vector_lepton_chargedHadronIso;   //!
   TBranch        *b_std_vector_lepton_chargedParticleIso;   //!
   TBranch        *b_std_vector_lepton_elSIP3D;   //!
   TBranch        *b_std_vector_lepton_eta;   //!
   TBranch        *b_std_vector_lepton_id;   //!
   TBranch        *b_std_vector_lepton_isTightMuon;   //!
   TBranch        *b_std_vector_lepton_muSIP3D;   //!
   TBranch        *b_std_vector_lepton_neutralHadronIso;   //!
   TBranch        *b_std_vector_lepton_phi;   //!
   TBranch        *b_std_vector_lepton_photonIso;   //!
   TBranch        *b_std_vector_lepton_pt;   //!
   TBranch        *b_std_vector_lepton_sumPUPt;   //!
   TBranch        *b_std_vector_numHits;   //!
   TBranch        *b_std_vector_puppijet_eta;   //!
   TBranch        *b_std_vector_puppijet_phi;   //!
   TBranch        *b_std_vector_puppijet_pt;   //!
   TBranch        *b_std_vector_sigmaIetaIeta;   //!
   TBranch        *b_std_vector_tau_eta;   //!
   TBranch        *b_std_vector_tau_phi;   //!
   TBranch        *b_std_vector_tau_pt;   //!
   TBranch        *b_tightmu;   //!
   TBranch        *b_triggAW;   //!
   TBranch        *b_triggBW;   //!
   TBranch        *b_triggW;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_trkMet;   //!
//   TBranch        *b_v_jet1_fCoordinates_fX;   //!
//   TBranch        *b_v_jet1_fCoordinates_fY;   //!
//   TBranch        *b_v_jet1_fCoordinates_fZ;   //!
//   TBranch        *b_v_jet1_fCoordinates_fT;   //!
//   TBranch        *b_v_jet2_fCoordinates_fX;   //!
//   TBranch        *b_v_jet2_fCoordinates_fY;   //!
//   TBranch        *b_v_jet2_fCoordinates_fZ;   //!
//   TBranch        *b_v_jet2_fCoordinates_fT;   //!
//   TBranch        *b_v_jet3_fCoordinates_fX;   //!
//   TBranch        *b_v_jet3_fCoordinates_fY;   //!
//   TBranch        *b_v_jet3_fCoordinates_fZ;   //!
//   TBranch        *b_v_jet3_fCoordinates_fT;   //!
//   TBranch        *b_v_jet4_fCoordinates_fX;   //!
//   TBranch        *b_v_jet4_fCoordinates_fY;   //!
//   TBranch        *b_v_jet4_fCoordinates_fZ;   //!
//   TBranch        *b_v_jet4_fCoordinates_fT;   //!
//   TBranch        *b_v_lepton1_fCoordinates_fX;   //!
//   TBranch        *b_v_lepton1_fCoordinates_fY;   //!
//   TBranch        *b_v_lepton1_fCoordinates_fZ;   //!
//   TBranch        *b_v_lepton1_fCoordinates_fT;   //!
//   TBranch        *b_v_lepton2_fCoordinates_fX;   //!
//   TBranch        *b_v_lepton2_fCoordinates_fY;   //!
//   TBranch        *b_v_lepton2_fCoordinates_fZ;   //!
//   TBranch        *b_v_lepton2_fCoordinates_fT;   //!
//   TBranch        *b_v_lepton3_fCoordinates_fX;   //!
//   TBranch        *b_v_lepton3_fCoordinates_fY;   //!
//   TBranch        *b_v_lepton3_fCoordinates_fZ;   //!
//   TBranch        *b_v_lepton3_fCoordinates_fT;   //!
//   TBranch        *b_v_lepton4_fCoordinates_fX;   //!
//   TBranch        *b_v_lepton4_fCoordinates_fY;   //!
//   TBranch        *b_v_lepton4_fCoordinates_fZ;   //!
//   TBranch        *b_v_lepton4_fCoordinates_fT;   //!
//   TBranch        *b_v_photon1_fCoordinates_fX;   //!
//   TBranch        *b_v_photon1_fCoordinates_fY;   //!
//   TBranch        *b_v_photon1_fCoordinates_fZ;   //!
//   TBranch        *b_v_photon1_fCoordinates_fT;   //!
//   TBranch        *b_v_photon1id_fCoordinates_fX;   //!
//   TBranch        *b_v_photon1id_fCoordinates_fY;   //!
//   TBranch        *b_v_photon1id_fCoordinates_fZ;   //!
//   TBranch        *b_v_photon1id_fCoordinates_fT;   //!
//   TBranch        *b_v_photon2_fCoordinates_fX;   //!
//   TBranch        *b_v_photon2_fCoordinates_fY;   //!
//   TBranch        *b_v_photon2_fCoordinates_fZ;   //!
//   TBranch        *b_v_photon2_fCoordinates_fT;   //!
//   TBranch        *b_v_photon3_fCoordinates_fX;   //!
//   TBranch        *b_v_photon3_fCoordinates_fY;   //!
//   TBranch        *b_v_photon3_fCoordinates_fZ;   //!
//   TBranch        *b_v_photon3_fCoordinates_fT;   //!
   TBranch        *b_worstJetLepPt;   //!
   TBranch        *b_yll;   //!
   TBranch        *b_itpu;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_ootpum1;   //!
   TBranch        *b_ootpum2;   //!
   TBranch        *b_ootpum3;   //!
   TBranch        *b_ootpum4;   //!
   TBranch        *b_ootpum5;   //!
   TBranch        *b_ootpup1;   //!
   TBranch        *b_ootpup2;   //!
   TBranch        *b_ootpup3;   //!
   TBranch        *b_trpu;   //!
   TBranch        *b_bveto;   //!
   TBranch        *b_bveto_ip;   //!
   TBranch        *b_bveto_mu;   //!
   TBranch        *b_bveto_munj;   //!
   TBranch        *b_bveto_munj05;   //!
   TBranch        *b_bveto_munj30;   //!
   TBranch        *b_bveto_munj3005;   //!
   TBranch        *b_bveto_nj;   //!
   TBranch        *b_bveto_nj05;   //!
   TBranch        *b_bveto_nj30;   //!
   TBranch        *b_bveto_nj3005;   //!
   TBranch        *b_dphiveto;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!

   AnalysisBase(TTree *tree=0);
   virtual ~AnalysisBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef AnalysisBase_cxx
AnalysisBase::AnalysisBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../latino/latino_WZJets.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../latino/latino_WZJets.root");
      }
      f->GetObject("latino",tree);

   }
   Init(tree);
}

AnalysisBase::~AnalysisBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnalysisBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnalysisBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnalysisBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   std_vector_HoE = 0;
   std_vector_deltaEtaIn = 0;
   std_vector_deltaPhiIn = 0;
   std_vector_effectiveArea = 0;
   std_vector_jetGen_eta = 0;
   std_vector_jetGen_phi = 0;
   std_vector_jetGen_pt = 0;
   std_vector_jet_HadronFlavour = 0;
   std_vector_jet_PartonFlavour = 0;
   std_vector_jet_QGRMScand = 0;
   std_vector_jet_QGRmax = 0;
   std_vector_jet_QGaxis1 = 0;
   std_vector_jet_QGaxis2 = 0;
   std_vector_jet_QGlikelihood = 0;
   std_vector_jet_bjpb = 0;
   std_vector_jet_cmva = 0;
   std_vector_jet_csvv2ivf = 0;
   std_vector_jet_eta = 0;
   std_vector_jet_pfcsv = 0;
   std_vector_jet_phi = 0;
   std_vector_jet_pt = 0;
   std_vector_jet_ssvhb = 0;
   std_vector_jet_ssvhe = 0;
   std_vector_jet_tche = 0;
   std_vector_jet_tchp = 0;
   std_vector_leptonGen_eta = 0;
   std_vector_leptonGen_index = 0;
   std_vector_leptonGen_mpid = 0;
   std_vector_leptonGen_mstatus = 0;
   std_vector_leptonGen_phi = 0;
   std_vector_leptonGen_pid = 0;
   std_vector_leptonGen_pt = 0;
   std_vector_leptonGen_status = 0;
   std_vector_lepton_BestTrackdxy = 0;
   std_vector_lepton_BestTrackdz = 0;
   std_vector_lepton_Chi2LocalPos = 0;
   std_vector_lepton_NTkLayers = 0;
   std_vector_lepton_NValidFractInTrk = 0;
   std_vector_lepton_NValidHitsInTrk = 0;
   std_vector_lepton_NValidHitsSATrk = 0;
   std_vector_lepton_NValidPixelHitsInTrk = 0;
   std_vector_lepton_NormChi2GTrk = 0;
   std_vector_lepton_NumOfMatchedStations = 0;
   std_vector_lepton_SegCompatibilty = 0;
   std_vector_lepton_TrkKink = 0;
   std_vector_lepton_chargedHadronIso = 0;
   std_vector_lepton_chargedParticleIso = 0;
   std_vector_lepton_elSIP3D = 0;
   std_vector_lepton_eta = 0;
   std_vector_lepton_id = 0;
   std_vector_lepton_isTightMuon = 0;
   std_vector_lepton_muSIP3D = 0;
   std_vector_lepton_neutralHadronIso = 0;
   std_vector_lepton_phi = 0;
   std_vector_lepton_photonIso = 0;
   std_vector_lepton_pt = 0;
   std_vector_lepton_sumPUPt = 0;
   std_vector_numHits = 0;
   std_vector_puppijet_eta = 0;
   std_vector_puppijet_phi = 0;
   std_vector_puppijet_pt = 0;
   std_vector_sigmaIetaIeta = 0;
   std_vector_tau_eta = 0;
   std_vector_tau_phi = 0;
   std_vector_tau_pt = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("baseW", &baseW, &b_baseW);
   fChain->SetBranchAddress("bdt1", &bdt1, &b_bdt1);
   fChain->SetBranchAddress("bdt2", &bdt2, &b_bdt2);
   fChain->SetBranchAddress("bdt3", &bdt3, &b_bdt3);
   fChain->SetBranchAddress("bdt4", &bdt4, &b_bdt4);
   fChain->SetBranchAddress("ch1", &ch1, &b_ch1);
   fChain->SetBranchAddress("ch2", &ch2, &b_ch2);
   fChain->SetBranchAddress("ch3", &ch3, &b_ch3);
   fChain->SetBranchAddress("ch4", &ch4, &b_ch4);
   fChain->SetBranchAddress("chSumEt", &chSumEt, &b_chSumEt);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("chmet", &chmet, &b_chmet);
   fChain->SetBranchAddress("chmetphi", &chmetphi, &b_chmetphi);
   fChain->SetBranchAddress("cjeteta1", &cjeteta1, &b_cjeteta1);
   fChain->SetBranchAddress("cjeteta2", &cjeteta2, &b_cjeteta2);
   fChain->SetBranchAddress("cjetid1", &cjetid1, &b_cjetid1);
   fChain->SetBranchAddress("cjetid2", &cjetid2, &b_cjetid2);
   fChain->SetBranchAddress("cjetmva1", &cjetmva1, &b_cjetmva1);
   fChain->SetBranchAddress("cjetmva2", &cjetmva2, &b_cjetmva2);
   fChain->SetBranchAddress("cjetphi1", &cjetphi1, &b_cjetphi1);
   fChain->SetBranchAddress("cjetphi2", &cjetphi2, &b_cjetphi2);
   fChain->SetBranchAddress("cjetpt1", &cjetpt1, &b_cjetpt1);
   fChain->SetBranchAddress("cjetpt2", &cjetpt2, &b_cjetpt2);
   fChain->SetBranchAddress("dataset", &dataset, &b_dataset);
   fChain->SetBranchAddress("detajj", &detajj, &b_detajj);
   fChain->SetBranchAddress("dphill", &dphill, &b_dphill);
   fChain->SetBranchAddress("dphilljet", &dphilljet, &b_dphilljet);
   fChain->SetBranchAddress("dphilljetjet", &dphilljetjet, &b_dphilljetjet);
   fChain->SetBranchAddress("dphillmet", &dphillmet, &b_dphillmet);
   fChain->SetBranchAddress("dphilmet", &dphilmet, &b_dphilmet);
   fChain->SetBranchAddress("dphilmet1", &dphilmet1, &b_dphilmet1);
   fChain->SetBranchAddress("dphilmet2", &dphilmet2, &b_dphilmet2);
   fChain->SetBranchAddress("drll", &drll, &b_drll);
   fChain->SetBranchAddress("dymva0", &dymva0, &b_dymva0);
   fChain->SetBranchAddress("dymva1", &dymva1, &b_dymva1);
   fChain->SetBranchAddress("effAW", &effAW, &b_effAW);
   fChain->SetBranchAddress("effBW", &effBW, &b_effBW);
   fChain->SetBranchAddress("effW", &effW, &b_effW);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("eta3", &eta3, &b_eta3);
   fChain->SetBranchAddress("eta4", &eta4, &b_eta4);
   fChain->SetBranchAddress("fakeAW", &fakeAW, &b_fakeAW);
   fChain->SetBranchAddress("fakeBW", &fakeBW, &b_fakeBW);
   fChain->SetBranchAddress("fakeW", &fakeW, &b_fakeW);
   fChain->SetBranchAddress("gammaMRStar", &gammaMRStar, &b_gammaMRStar);
   fChain->SetBranchAddress("hardbjpb", &hardbjpb, &b_hardbjpb);
   fChain->SetBranchAddress("hardcmva", &hardcmva, &b_hardcmva);
   fChain->SetBranchAddress("hardcsvv2ivf", &hardcsvv2ivf, &b_hardcsvv2ivf);
   fChain->SetBranchAddress("hardpfcsv", &hardpfcsv, &b_hardpfcsv);
   fChain->SetBranchAddress("hardssvhb", &hardssvhb, &b_hardssvhb);
   fChain->SetBranchAddress("hardssvhe", &hardssvhe, &b_hardssvhe);
   fChain->SetBranchAddress("hardtche", &hardtche, &b_hardtche);
   fChain->SetBranchAddress("higggsGenpt", &higggsGenpt, &b_higggsGenpt);
   fChain->SetBranchAddress("imet", &imet, &b_imet);
   fChain->SetBranchAddress("iso1", &iso1, &b_iso1);
   fChain->SetBranchAddress("iso2", &iso2, &b_iso2);
   fChain->SetBranchAddress("iso3", &iso3, &b_iso3);
   fChain->SetBranchAddress("iso4", &iso4, &b_iso4);
   fChain->SetBranchAddress("isomva1", &isomva1, &b_isomva1);
   fChain->SetBranchAddress("isomva2", &isomva2, &b_isomva2);
   fChain->SetBranchAddress("isomva3", &isomva3, &b_isomva3);
   fChain->SetBranchAddress("isomva4", &isomva4, &b_isomva4);
   fChain->SetBranchAddress("jetCHM1", &jetCHM1, &b_jetCHM1);
   fChain->SetBranchAddress("jetCHM2", &jetCHM2, &b_jetCHM2);
   fChain->SetBranchAddress("jetCHM3", &jetCHM3, &b_jetCHM3);
   fChain->SetBranchAddress("jetCHM4", &jetCHM4, &b_jetCHM4);
   fChain->SetBranchAddress("jetCHM5", &jetCHM5, &b_jetCHM5);
   fChain->SetBranchAddress("jetCHM6", &jetCHM6, &b_jetCHM6);
   fChain->SetBranchAddress("jetCHM7", &jetCHM7, &b_jetCHM7);
   fChain->SetBranchAddress("jetGenPartoneta1", &jetGenPartoneta1, &b_jetGenPartoneta1);
   fChain->SetBranchAddress("jetGenPartoneta2", &jetGenPartoneta2, &b_jetGenPartoneta2);
   fChain->SetBranchAddress("jetGenPartoneta3", &jetGenPartoneta3, &b_jetGenPartoneta3);
   fChain->SetBranchAddress("jetGenPartonphi1", &jetGenPartonphi1, &b_jetGenPartonphi1);
   fChain->SetBranchAddress("jetGenPartonphi2", &jetGenPartonphi2, &b_jetGenPartonphi2);
   fChain->SetBranchAddress("jetGenPartonphi3", &jetGenPartonphi3, &b_jetGenPartonphi3);
   fChain->SetBranchAddress("jetGenPartonpid1", &jetGenPartonpid1, &b_jetGenPartonpid1);
   fChain->SetBranchAddress("jetGenPartonpid2", &jetGenPartonpid2, &b_jetGenPartonpid2);
   fChain->SetBranchAddress("jetGenPartonpid3", &jetGenPartonpid3, &b_jetGenPartonpid3);
   fChain->SetBranchAddress("jetGenPartonpt1", &jetGenPartonpt1, &b_jetGenPartonpt1);
   fChain->SetBranchAddress("jetGenPartonpt2", &jetGenPartonpt2, &b_jetGenPartonpt2);
   fChain->SetBranchAddress("jetGenPartonpt3", &jetGenPartonpt3, &b_jetGenPartonpt3);
   fChain->SetBranchAddress("jetNChgQC1", &jetNChgQC1, &b_jetNChgQC1);
   fChain->SetBranchAddress("jetNChgQC2", &jetNChgQC2, &b_jetNChgQC2);
   fChain->SetBranchAddress("jetNChgQC3", &jetNChgQC3, &b_jetNChgQC3);
   fChain->SetBranchAddress("jetNChgQC4", &jetNChgQC4, &b_jetNChgQC4);
   fChain->SetBranchAddress("jetNChgQC5", &jetNChgQC5, &b_jetNChgQC5);
   fChain->SetBranchAddress("jetNChgQC6", &jetNChgQC6, &b_jetNChgQC6);
   fChain->SetBranchAddress("jetNChgQC7", &jetNChgQC7, &b_jetNChgQC7);
   fChain->SetBranchAddress("jetNChgptCut1", &jetNChgptCut1, &b_jetNChgptCut1);
   fChain->SetBranchAddress("jetNChgptCut2", &jetNChgptCut2, &b_jetNChgptCut2);
   fChain->SetBranchAddress("jetNChgptCut3", &jetNChgptCut3, &b_jetNChgptCut3);
   fChain->SetBranchAddress("jetNChgptCut4", &jetNChgptCut4, &b_jetNChgptCut4);
   fChain->SetBranchAddress("jetNChgptCut5", &jetNChgptCut5, &b_jetNChgptCut5);
   fChain->SetBranchAddress("jetNChgptCut6", &jetNChgptCut6, &b_jetNChgptCut6);
   fChain->SetBranchAddress("jetNChgptCut7", &jetNChgptCut7, &b_jetNChgptCut7);
   fChain->SetBranchAddress("jetNHM1", &jetNHM1, &b_jetNHM1);
   fChain->SetBranchAddress("jetNHM2", &jetNHM2, &b_jetNHM2);
   fChain->SetBranchAddress("jetNHM3", &jetNHM3, &b_jetNHM3);
   fChain->SetBranchAddress("jetNHM4", &jetNHM4, &b_jetNHM4);
   fChain->SetBranchAddress("jetNHM5", &jetNHM5, &b_jetNHM5);
   fChain->SetBranchAddress("jetNHM6", &jetNHM6, &b_jetNHM6);
   fChain->SetBranchAddress("jetNHM7", &jetNHM7, &b_jetNHM7);
   fChain->SetBranchAddress("jetNNeutralptCut1", &jetNNeutralptCut1, &b_jetNNeutralptCut1);
   fChain->SetBranchAddress("jetNNeutralptCut2", &jetNNeutralptCut2, &b_jetNNeutralptCut2);
   fChain->SetBranchAddress("jetNNeutralptCut3", &jetNNeutralptCut3, &b_jetNNeutralptCut3);
   fChain->SetBranchAddress("jetNNeutralptCut4", &jetNNeutralptCut4, &b_jetNNeutralptCut4);
   fChain->SetBranchAddress("jetNNeutralptCut5", &jetNNeutralptCut5, &b_jetNNeutralptCut5);
   fChain->SetBranchAddress("jetNNeutralptCut6", &jetNNeutralptCut6, &b_jetNNeutralptCut6);
   fChain->SetBranchAddress("jetNNeutralptCut7", &jetNNeutralptCut7, &b_jetNNeutralptCut7);
   fChain->SetBranchAddress("jetPhM1", &jetPhM1, &b_jetPhM1);
   fChain->SetBranchAddress("jetPhM2", &jetPhM2, &b_jetPhM2);
   fChain->SetBranchAddress("jetPhM3", &jetPhM3, &b_jetPhM3);
   fChain->SetBranchAddress("jetPhM4", &jetPhM4, &b_jetPhM4);
   fChain->SetBranchAddress("jetPhM5", &jetPhM5, &b_jetPhM5);
   fChain->SetBranchAddress("jetPhM6", &jetPhM6, &b_jetPhM6);
   fChain->SetBranchAddress("jetPhM7", &jetPhM7, &b_jetPhM7);
   fChain->SetBranchAddress("jetRho", &jetRho, &b_jetRho);
   fChain->SetBranchAddress("jetbjpb1", &jetbjpb1, &b_jetbjpb1);
   fChain->SetBranchAddress("jetbjpb2", &jetbjpb2, &b_jetbjpb2);
   fChain->SetBranchAddress("jetbjpb3", &jetbjpb3, &b_jetbjpb3);
   fChain->SetBranchAddress("jetbjpb4", &jetbjpb4, &b_jetbjpb4);
   fChain->SetBranchAddress("jetbjpb5", &jetbjpb5, &b_jetbjpb5);
   fChain->SetBranchAddress("jetbjpb6", &jetbjpb6, &b_jetbjpb6);
   fChain->SetBranchAddress("jetbjpb7", &jetbjpb7, &b_jetbjpb7);
   fChain->SetBranchAddress("jetcmva1", &jetcmva1, &b_jetcmva1);
   fChain->SetBranchAddress("jetcmva2", &jetcmva2, &b_jetcmva2);
   fChain->SetBranchAddress("jetcsvv2ivf1", &jetcsvv2ivf1, &b_jetcsvv2ivf1);
   fChain->SetBranchAddress("jetcsvv2ivf2", &jetcsvv2ivf2, &b_jetcsvv2ivf2);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("jeteta3", &jeteta3, &b_jeteta3);
   fChain->SetBranchAddress("jeteta4", &jeteta4, &b_jeteta4);
   fChain->SetBranchAddress("jeteta5", &jeteta5, &b_jeteta5);
   fChain->SetBranchAddress("jeteta6", &jeteta6, &b_jeteta6);
   fChain->SetBranchAddress("jeteta7", &jeteta7, &b_jeteta7);
   fChain->SetBranchAddress("jetid1", &jetid1, &b_jetid1);
   fChain->SetBranchAddress("jetid2", &jetid2, &b_jetid2);
   fChain->SetBranchAddress("jetid3", &jetid3, &b_jetid3);
   fChain->SetBranchAddress("jetid4", &jetid4, &b_jetid4);
   fChain->SetBranchAddress("jetid5", &jetid5, &b_jetid5);
   fChain->SetBranchAddress("jetid6", &jetid6, &b_jetid6);
   fChain->SetBranchAddress("jetid7", &jetid7, &b_jetid7);
   fChain->SetBranchAddress("jetmass1", &jetmass1, &b_jetmass1);
   fChain->SetBranchAddress("jetmass2", &jetmass2, &b_jetmass2);
   fChain->SetBranchAddress("jetmass3", &jetmass3, &b_jetmass3);
   fChain->SetBranchAddress("jetmass4", &jetmass4, &b_jetmass4);
   fChain->SetBranchAddress("jetmass5", &jetmass5, &b_jetmass5);
   fChain->SetBranchAddress("jetmass6", &jetmass6, &b_jetmass6);
   fChain->SetBranchAddress("jetmass7", &jetmass7, &b_jetmass7);
   fChain->SetBranchAddress("jetmva1", &jetmva1, &b_jetmva1);
   fChain->SetBranchAddress("jetmva2", &jetmva2, &b_jetmva2);
   fChain->SetBranchAddress("jetmva3", &jetmva3, &b_jetmva3);
   fChain->SetBranchAddress("jetmva4", &jetmva4, &b_jetmva4);
   fChain->SetBranchAddress("jetmva5", &jetmva5, &b_jetmva5);
   fChain->SetBranchAddress("jetmva6", &jetmva6, &b_jetmva6);
   fChain->SetBranchAddress("jetmva7", &jetmva7, &b_jetmva7);
   fChain->SetBranchAddress("jetpfcsv1", &jetpfcsv1, &b_jetpfcsv1);
   fChain->SetBranchAddress("jetpfcsv2", &jetpfcsv2, &b_jetpfcsv2);
   fChain->SetBranchAddress("jetphi1", &jetphi1, &b_jetphi1);
   fChain->SetBranchAddress("jetphi2", &jetphi2, &b_jetphi2);
   fChain->SetBranchAddress("jetphi3", &jetphi3, &b_jetphi3);
   fChain->SetBranchAddress("jetphi4", &jetphi4, &b_jetphi4);
   fChain->SetBranchAddress("jetphi5", &jetphi5, &b_jetphi5);
   fChain->SetBranchAddress("jetphi6", &jetphi6, &b_jetphi6);
   fChain->SetBranchAddress("jetphi7", &jetphi7, &b_jetphi7);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jetpt3", &jetpt3, &b_jetpt3);
   fChain->SetBranchAddress("jetpt4", &jetpt4, &b_jetpt4);
   fChain->SetBranchAddress("jetpt5", &jetpt5, &b_jetpt5);
   fChain->SetBranchAddress("jetpt6", &jetpt6, &b_jetpt6);
   fChain->SetBranchAddress("jetpt7", &jetpt7, &b_jetpt7);
   fChain->SetBranchAddress("jetptd1", &jetptd1, &b_jetptd1);
   fChain->SetBranchAddress("jetptd2", &jetptd2, &b_jetptd2);
   fChain->SetBranchAddress("jetptd3", &jetptd3, &b_jetptd3);
   fChain->SetBranchAddress("jetptd4", &jetptd4, &b_jetptd4);
   fChain->SetBranchAddress("jetptd5", &jetptd5, &b_jetptd5);
   fChain->SetBranchAddress("jetptd6", &jetptd6, &b_jetptd6);
   fChain->SetBranchAddress("jetptd7", &jetptd7, &b_jetptd7);
   fChain->SetBranchAddress("jetssvhb1", &jetssvhb1, &b_jetssvhb1);
   fChain->SetBranchAddress("jetssvhb2", &jetssvhb2, &b_jetssvhb2);
   fChain->SetBranchAddress("jetssvhe1", &jetssvhe1, &b_jetssvhe1);
   fChain->SetBranchAddress("jetssvhe2", &jetssvhe2, &b_jetssvhe2);
   fChain->SetBranchAddress("jettche1", &jettche1, &b_jettche1);
   fChain->SetBranchAddress("jettche2", &jettche2, &b_jettche2);
   fChain->SetBranchAddress("jettche3", &jettche3, &b_jettche3);
   fChain->SetBranchAddress("jettche4", &jettche4, &b_jettche4);
   fChain->SetBranchAddress("jettche5", &jettche5, &b_jettche5);
   fChain->SetBranchAddress("jettche6", &jettche6, &b_jettche6);
   fChain->SetBranchAddress("jettche7", &jettche7, &b_jettche7);
   fChain->SetBranchAddress("jettchp1", &jettchp1, &b_jettchp1);
   fChain->SetBranchAddress("jettchp2", &jettchp2, &b_jettchp2);
   fChain->SetBranchAddress("jettchp3", &jettchp3, &b_jettchp3);
   fChain->SetBranchAddress("jettchp4", &jettchp4, &b_jettchp4);
   fChain->SetBranchAddress("jettchp5", &jettchp5, &b_jettchp5);
   fChain->SetBranchAddress("jettchp6", &jettchp6, &b_jettchp6);
   fChain->SetBranchAddress("jettchp7", &jettchp7, &b_jettchp7);
   fChain->SetBranchAddress("kfW", &kfW, &b_kfW);
   fChain->SetBranchAddress("lh1", &lh1, &b_lh1);
   fChain->SetBranchAddress("lh2", &lh2, &b_lh2);
   fChain->SetBranchAddress("lh3", &lh3, &b_lh3);
   fChain->SetBranchAddress("lh4", &lh4, &b_lh4);
   fChain->SetBranchAddress("mctruth", &mctruth, &b_mctruth);
   fChain->SetBranchAddress("metGeneta", &metGeneta, &b_metGeneta);
   fChain->SetBranchAddress("metGenphi", &metGenphi, &b_metGenphi);
   fChain->SetBranchAddress("metGenpt", &metGenpt, &b_metGenpt);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("mllg", &mllg, &b_mllg);
   fChain->SetBranchAddress("mllgid", &mllgid, &b_mllgid);
   fChain->SetBranchAddress("mpmet", &mpmet, &b_mpmet);
   fChain->SetBranchAddress("mth", &mth, &b_mth);
   fChain->SetBranchAddress("mtw1", &mtw1, &b_mtw1);
   fChain->SetBranchAddress("mtw2", &mtw2, &b_mtw2);
   fChain->SetBranchAddress("nPhos", &nPhos, &b_nPhos);
   fChain->SetBranchAddress("nbjet", &nbjet, &b_nbjet);
   fChain->SetBranchAddress("nbjettche", &nbjettche, &b_nbjettche);
   fChain->SetBranchAddress("nbrem1", &nbrem1, &b_nbrem1);
   fChain->SetBranchAddress("nbrem2", &nbrem2, &b_nbrem2);
   fChain->SetBranchAddress("nbrem3", &nbrem3, &b_nbrem3);
   fChain->SetBranchAddress("nbrem4", &nbrem4, &b_nbrem4);
   fChain->SetBranchAddress("neutrinoGeneta1", &neutrinoGeneta1, &b_neutrinoGeneta1);
   fChain->SetBranchAddress("neutrinoGeneta2", &neutrinoGeneta2, &b_neutrinoGeneta2);
   fChain->SetBranchAddress("neutrinoGeneta3", &neutrinoGeneta3, &b_neutrinoGeneta3);
   fChain->SetBranchAddress("neutrinoGenphi1", &neutrinoGenphi1, &b_neutrinoGenphi1);
   fChain->SetBranchAddress("neutrinoGenphi2", &neutrinoGenphi2, &b_neutrinoGenphi2);
   fChain->SetBranchAddress("neutrinoGenphi3", &neutrinoGenphi3, &b_neutrinoGenphi3);
   fChain->SetBranchAddress("neutrinoGenpid1", &neutrinoGenpid1, &b_neutrinoGenpid1);
   fChain->SetBranchAddress("neutrinoGenpid2", &neutrinoGenpid2, &b_neutrinoGenpid2);
   fChain->SetBranchAddress("neutrinoGenpid3", &neutrinoGenpid3, &b_neutrinoGenpid3);
   fChain->SetBranchAddress("neutrinoGenpt1", &neutrinoGenpt1, &b_neutrinoGenpt1);
   fChain->SetBranchAddress("neutrinoGenpt2", &neutrinoGenpt2, &b_neutrinoGenpt2);
   fChain->SetBranchAddress("neutrinoGenpt3", &neutrinoGenpt3, &b_neutrinoGenpt3);
   fChain->SetBranchAddress("nextra", &nextra, &b_nextra);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("njetid", &njetid, &b_njetid);
   fChain->SetBranchAddress("njetvbf", &njetvbf, &b_njetvbf);
   fChain->SetBranchAddress("pchmet", &pchmet, &b_pchmet);
   fChain->SetBranchAddress("pdfid1", &pdfid1, &b_pdfid1);
   fChain->SetBranchAddress("pdfid2", &pdfid2, &b_pdfid2);
   fChain->SetBranchAddress("pdfscalePDF", &pdfscalePDF, &b_pdfscalePDF);
   fChain->SetBranchAddress("pdfx1", &pdfx1, &b_pdfx1);
   fChain->SetBranchAddress("pdfx1PDF", &pdfx1PDF, &b_pdfx1PDF);
   fChain->SetBranchAddress("pdfx2", &pdfx2, &b_pdfx2);
   fChain->SetBranchAddress("pdfx2PDF", &pdfx2PDF, &b_pdfx2PDF);
   fChain->SetBranchAddress("pdgid1", &pdgid1, &b_pdgid1);
   fChain->SetBranchAddress("pdgid2", &pdgid2, &b_pdgid2);
   fChain->SetBranchAddress("pdgid3", &pdgid3, &b_pdgid3);
   fChain->SetBranchAddress("pdgid4", &pdgid4, &b_pdgid4);
   fChain->SetBranchAddress("peaking", &peaking, &b_peaking);
   fChain->SetBranchAddress("pfRawMet", &pfRawMet, &b_pfRawMet);
   fChain->SetBranchAddress("pfRawMetPhi", &pfRawMetPhi, &b_pfRawMetPhi);
   fChain->SetBranchAddress("pfRawSumEt", &pfRawSumEt, &b_pfRawSumEt);
   fChain->SetBranchAddress("pfType1Met", &pfType1Met, &b_pfType1Met);
   fChain->SetBranchAddress("pfType1Metdn", &pfType1Metdn, &b_pfType1Metdn);
   fChain->SetBranchAddress("pfType1Metphi", &pfType1Metphi, &b_pfType1Metphi);
   fChain->SetBranchAddress("pfType1Metup", &pfType1Metup, &b_pfType1Metup);
   fChain->SetBranchAddress("pfType1SumEt", &pfType1SumEt, &b_pfType1SumEt);
   fChain->SetBranchAddress("pfmetMEtSig", &pfmetMEtSig, &b_pfmetMEtSig);
   fChain->SetBranchAddress("pfmetSignificance", &pfmetSignificance, &b_pfmetSignificance);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("phi3", &phi3, &b_phi3);
   fChain->SetBranchAddress("phi4", &phi4, &b_phi4);
   fChain->SetBranchAddress("pho_n_id", &pho_n_id, &b_pho_n_id);
   fChain->SetBranchAddress("ppfMet", &ppfMet, &b_ppfMet);
   fChain->SetBranchAddress("predmet", &predmet, &b_predmet);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("pt3", &pt3, &b_pt3);
   fChain->SetBranchAddress("pt4", &pt4, &b_pt4);
   fChain->SetBranchAddress("ptll", &ptll, &b_ptll);
   fChain->SetBranchAddress("puW", &puW, &b_puW);
   fChain->SetBranchAddress("pupMet", &pupMet, &b_pupMet);
   fChain->SetBranchAddress("puppijetpt1", &puppijetpt1, &b_puppijetpt1);
   fChain->SetBranchAddress("redmet", &redmet, &b_redmet);
   fChain->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
   fChain->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
   fChain->SetBranchAddress("sceta3", &sceta3, &b_sceta3);
   fChain->SetBranchAddress("sceta4", &sceta4, &b_sceta4);
   fChain->SetBranchAddress("softbjpb", &softbjpb, &b_softbjpb);
   fChain->SetBranchAddress("softcmva", &softcmva, &b_softcmva);
   fChain->SetBranchAddress("softcsvv2ivf", &softcsvv2ivf, &b_softcsvv2ivf);
   fChain->SetBranchAddress("softpfcsv", &softpfcsv, &b_softpfcsv);
   fChain->SetBranchAddress("softssvhb", &softssvhb, &b_softssvhb);
   fChain->SetBranchAddress("softssvhe", &softssvhe, &b_softssvhe);
   fChain->SetBranchAddress("softtche", &softtche, &b_softtche);
   fChain->SetBranchAddress("std_vector_HoE", &std_vector_HoE, &b_std_vector_HoE);
   fChain->SetBranchAddress("std_vector_deltaEtaIn", &std_vector_deltaEtaIn, &b_std_vector_deltaEtaIn);
   fChain->SetBranchAddress("std_vector_deltaPhiIn", &std_vector_deltaPhiIn, &b_std_vector_deltaPhiIn);
   fChain->SetBranchAddress("std_vector_effectiveArea", &std_vector_effectiveArea, &b_std_vector_effectiveArea);
   fChain->SetBranchAddress("std_vector_jetGen_eta", &std_vector_jetGen_eta, &b_std_vector_jetGen_eta);
   fChain->SetBranchAddress("std_vector_jetGen_phi", &std_vector_jetGen_phi, &b_std_vector_jetGen_phi);
   fChain->SetBranchAddress("std_vector_jetGen_pt", &std_vector_jetGen_pt, &b_std_vector_jetGen_pt);
   fChain->SetBranchAddress("std_vector_jet_HadronFlavour", &std_vector_jet_HadronFlavour, &b_std_vector_jet_HadronFlavour);
   fChain->SetBranchAddress("std_vector_jet_PartonFlavour", &std_vector_jet_PartonFlavour, &b_std_vector_jet_PartonFlavour);
   fChain->SetBranchAddress("std_vector_jet_QGRMScand", &std_vector_jet_QGRMScand, &b_std_vector_jet_QGRMScand);
   fChain->SetBranchAddress("std_vector_jet_QGRmax", &std_vector_jet_QGRmax, &b_std_vector_jet_QGRmax);
   fChain->SetBranchAddress("std_vector_jet_QGaxis1", &std_vector_jet_QGaxis1, &b_std_vector_jet_QGaxis1);
   fChain->SetBranchAddress("std_vector_jet_QGaxis2", &std_vector_jet_QGaxis2, &b_std_vector_jet_QGaxis2);
   fChain->SetBranchAddress("std_vector_jet_QGlikelihood", &std_vector_jet_QGlikelihood, &b_std_vector_jet_QGlikelihood);
   fChain->SetBranchAddress("std_vector_jet_bjpb", &std_vector_jet_bjpb, &b_std_vector_jet_bjpb);
   fChain->SetBranchAddress("std_vector_jet_cmva", &std_vector_jet_cmva, &b_std_vector_jet_cmva);
   fChain->SetBranchAddress("std_vector_jet_csvv2ivf", &std_vector_jet_csvv2ivf, &b_std_vector_jet_csvv2ivf);
   fChain->SetBranchAddress("std_vector_jet_eta", &std_vector_jet_eta, &b_std_vector_jet_eta);
   fChain->SetBranchAddress("std_vector_jet_pfcsv", &std_vector_jet_pfcsv, &b_std_vector_jet_pfcsv);
   fChain->SetBranchAddress("std_vector_jet_phi", &std_vector_jet_phi, &b_std_vector_jet_phi);
   fChain->SetBranchAddress("std_vector_jet_pt", &std_vector_jet_pt, &b_std_vector_jet_pt);
   fChain->SetBranchAddress("std_vector_jet_ssvhb", &std_vector_jet_ssvhb, &b_std_vector_jet_ssvhb);
   fChain->SetBranchAddress("std_vector_jet_ssvhe", &std_vector_jet_ssvhe, &b_std_vector_jet_ssvhe);
   fChain->SetBranchAddress("std_vector_jet_tche", &std_vector_jet_tche, &b_std_vector_jet_tche);
   fChain->SetBranchAddress("std_vector_jet_tchp", &std_vector_jet_tchp, &b_std_vector_jet_tchp);
   fChain->SetBranchAddress("std_vector_leptonGen_eta", &std_vector_leptonGen_eta, &b_std_vector_leptonGen_eta);
   fChain->SetBranchAddress("std_vector_leptonGen_index", &std_vector_leptonGen_index, &b_std_vector_leptonGen_index);
   fChain->SetBranchAddress("std_vector_leptonGen_mpid", &std_vector_leptonGen_mpid, &b_std_vector_leptonGen_mpid);
   fChain->SetBranchAddress("std_vector_leptonGen_mstatus", &std_vector_leptonGen_mstatus, &b_std_vector_leptonGen_mstatus);
   fChain->SetBranchAddress("std_vector_leptonGen_phi", &std_vector_leptonGen_phi, &b_std_vector_leptonGen_phi);
   fChain->SetBranchAddress("std_vector_leptonGen_pid", &std_vector_leptonGen_pid, &b_std_vector_leptonGen_pid);
   fChain->SetBranchAddress("std_vector_leptonGen_pt", &std_vector_leptonGen_pt, &b_std_vector_leptonGen_pt);
   fChain->SetBranchAddress("std_vector_leptonGen_status", &std_vector_leptonGen_status, &b_std_vector_leptonGen_status);
   fChain->SetBranchAddress("std_vector_lepton_BestTrackdxy", &std_vector_lepton_BestTrackdxy, &b_std_vector_lepton_BestTrackdxy);
   fChain->SetBranchAddress("std_vector_lepton_BestTrackdz", &std_vector_lepton_BestTrackdz, &b_std_vector_lepton_BestTrackdz);
   fChain->SetBranchAddress("std_vector_lepton_Chi2LocalPos", &std_vector_lepton_Chi2LocalPos, &b_std_vector_lepton_Chi2LocalPos);
   fChain->SetBranchAddress("std_vector_lepton_NTkLayers", &std_vector_lepton_NTkLayers, &b_std_vector_lepton_NTkLayers);
   fChain->SetBranchAddress("std_vector_lepton_NValidFractInTrk", &std_vector_lepton_NValidFractInTrk, &b_std_vector_lepton_NValidFractInTrk);
   fChain->SetBranchAddress("std_vector_lepton_NValidHitsInTrk", &std_vector_lepton_NValidHitsInTrk, &b_std_vector_lepton_NValidHitsInTrk);
   fChain->SetBranchAddress("std_vector_lepton_NValidHitsSATrk", &std_vector_lepton_NValidHitsSATrk, &b_std_vector_lepton_NValidHitsSATrk);
   fChain->SetBranchAddress("std_vector_lepton_NValidPixelHitsInTrk", &std_vector_lepton_NValidPixelHitsInTrk, &b_std_vector_lepton_NValidPixelHitsInTrk);
   fChain->SetBranchAddress("std_vector_lepton_NormChi2GTrk", &std_vector_lepton_NormChi2GTrk, &b_std_vector_lepton_NormChi2GTrk);
   fChain->SetBranchAddress("std_vector_lepton_NumOfMatchedStations", &std_vector_lepton_NumOfMatchedStations, &b_std_vector_lepton_NumOfMatchedStations);
   fChain->SetBranchAddress("std_vector_lepton_SegCompatibilty", &std_vector_lepton_SegCompatibilty, &b_std_vector_lepton_SegCompatibilty);
   fChain->SetBranchAddress("std_vector_lepton_TrkKink", &std_vector_lepton_TrkKink, &b_std_vector_lepton_TrkKink);
   fChain->SetBranchAddress("std_vector_lepton_chargedHadronIso", &std_vector_lepton_chargedHadronIso, &b_std_vector_lepton_chargedHadronIso);
   fChain->SetBranchAddress("std_vector_lepton_chargedParticleIso", &std_vector_lepton_chargedParticleIso, &b_std_vector_lepton_chargedParticleIso);
   fChain->SetBranchAddress("std_vector_lepton_elSIP3D", &std_vector_lepton_elSIP3D, &b_std_vector_lepton_elSIP3D);
   fChain->SetBranchAddress("std_vector_lepton_eta", &std_vector_lepton_eta, &b_std_vector_lepton_eta);
   fChain->SetBranchAddress("std_vector_lepton_id", &std_vector_lepton_id, &b_std_vector_lepton_id);
   fChain->SetBranchAddress("std_vector_lepton_isTightMuon", &std_vector_lepton_isTightMuon, &b_std_vector_lepton_isTightMuon);
   fChain->SetBranchAddress("std_vector_lepton_muSIP3D", &std_vector_lepton_muSIP3D, &b_std_vector_lepton_muSIP3D);
   fChain->SetBranchAddress("std_vector_lepton_neutralHadronIso", &std_vector_lepton_neutralHadronIso, &b_std_vector_lepton_neutralHadronIso);
   fChain->SetBranchAddress("std_vector_lepton_phi", &std_vector_lepton_phi, &b_std_vector_lepton_phi);
   fChain->SetBranchAddress("std_vector_lepton_photonIso", &std_vector_lepton_photonIso, &b_std_vector_lepton_photonIso);
   fChain->SetBranchAddress("std_vector_lepton_pt", &std_vector_lepton_pt, &b_std_vector_lepton_pt);
   fChain->SetBranchAddress("std_vector_lepton_sumPUPt", &std_vector_lepton_sumPUPt, &b_std_vector_lepton_sumPUPt);
   fChain->SetBranchAddress("std_vector_numHits", &std_vector_numHits, &b_std_vector_numHits);
   fChain->SetBranchAddress("std_vector_puppijet_eta", &std_vector_puppijet_eta, &b_std_vector_puppijet_eta);
   fChain->SetBranchAddress("std_vector_puppijet_phi", &std_vector_puppijet_phi, &b_std_vector_puppijet_phi);
   fChain->SetBranchAddress("std_vector_puppijet_pt", &std_vector_puppijet_pt, &b_std_vector_puppijet_pt);
   fChain->SetBranchAddress("std_vector_sigmaIetaIeta", &std_vector_sigmaIetaIeta, &b_std_vector_sigmaIetaIeta);
   fChain->SetBranchAddress("std_vector_tau_eta", &std_vector_tau_eta, &b_std_vector_tau_eta);
   fChain->SetBranchAddress("std_vector_tau_phi", &std_vector_tau_phi, &b_std_vector_tau_phi);
   fChain->SetBranchAddress("std_vector_tau_pt", &std_vector_tau_pt, &b_std_vector_tau_pt);
   fChain->SetBranchAddress("tightmu", &tightmu, &b_tightmu);
   fChain->SetBranchAddress("triggAW", &triggAW, &b_triggAW);
   fChain->SetBranchAddress("triggBW", &triggBW, &b_triggBW);
   fChain->SetBranchAddress("triggW", &triggW, &b_triggW);
   fChain->SetBranchAddress("trigger", &trigger, &b_trigger);
   fChain->SetBranchAddress("trkMet", &trkMet, &b_trkMet);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_jet1_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_jet1_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_jet1_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_jet1_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_jet2_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_jet2_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_jet2_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_jet2_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_jet3_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_jet3_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_jet3_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_jet3_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_jet4_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_jet4_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_jet4_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_jet4_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_lepton1_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_lepton1_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_lepton1_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_lepton1_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_lepton2_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_lepton2_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_lepton2_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_lepton2_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_lepton3_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_lepton3_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_lepton3_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_lepton3_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_lepton4_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_lepton4_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_lepton4_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_lepton4_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_photon1_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_photon1_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_photon1_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_photon1_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_photon1id_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_photon1id_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_photon1id_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_photon1id_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_photon2_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_photon2_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_photon2_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_photon2_fCoordinates_fT);
//    fChain->SetBranchAddress("fCoordinates.fX", &fCoordinates_fX, &b_v_photon3_fCoordinates_fX);
//    fChain->SetBranchAddress("fCoordinates.fY", &fCoordinates_fY, &b_v_photon3_fCoordinates_fY);
//    fChain->SetBranchAddress("fCoordinates.fZ", &fCoordinates_fZ, &b_v_photon3_fCoordinates_fZ);
//    fChain->SetBranchAddress("fCoordinates.fT", &fCoordinates_fT, &b_v_photon3_fCoordinates_fT);
   fChain->SetBranchAddress("worstJetLepPt", &worstJetLepPt, &b_worstJetLepPt);
   fChain->SetBranchAddress("yll", &yll, &b_yll);
   fChain->SetBranchAddress("itpu", &itpu, &b_itpu);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("ootpum1", &ootpum1, &b_ootpum1);
   fChain->SetBranchAddress("ootpum2", &ootpum2, &b_ootpum2);
   fChain->SetBranchAddress("ootpum3", &ootpum3, &b_ootpum3);
   fChain->SetBranchAddress("ootpum4", &ootpum4, &b_ootpum4);
   fChain->SetBranchAddress("ootpum5", &ootpum5, &b_ootpum5);
   fChain->SetBranchAddress("ootpup1", &ootpup1, &b_ootpup1);
   fChain->SetBranchAddress("ootpup2", &ootpup2, &b_ootpup2);
   fChain->SetBranchAddress("ootpup3", &ootpup3, &b_ootpup3);
   fChain->SetBranchAddress("trpu", &trpu, &b_trpu);
   fChain->SetBranchAddress("bveto", &bveto, &b_bveto);
   fChain->SetBranchAddress("bveto_ip", &bveto_ip, &b_bveto_ip);
   fChain->SetBranchAddress("bveto_mu", &bveto_mu, &b_bveto_mu);
   fChain->SetBranchAddress("bveto_munj", &bveto_munj, &b_bveto_munj);
   fChain->SetBranchAddress("bveto_munj05", &bveto_munj05, &b_bveto_munj05);
   fChain->SetBranchAddress("bveto_munj30", &bveto_munj30, &b_bveto_munj30);
   fChain->SetBranchAddress("bveto_munj3005", &bveto_munj3005, &b_bveto_munj3005);
   fChain->SetBranchAddress("bveto_nj", &bveto_nj, &b_bveto_nj);
   fChain->SetBranchAddress("bveto_nj05", &bveto_nj05, &b_bveto_nj05);
   fChain->SetBranchAddress("bveto_nj30", &bveto_nj30, &b_bveto_nj30);
   fChain->SetBranchAddress("bveto_nj3005", &bveto_nj3005, &b_bveto_nj3005);
   fChain->SetBranchAddress("dphiveto", &dphiveto, &b_dphiveto);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   Notify();
}

Bool_t AnalysisBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnalysisBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnalysisBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   fChain->Show(entry);
   return 1;
}
#endif // #ifdef AnalysisBase_cxx
