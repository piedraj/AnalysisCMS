/////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 29 12:02:32 2016 by ROOT version 6.02/13
// from TTree latino/probe_tree
// found on file: /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox_76x/22Jan_25ns_mAODv2_MC__l2loose__hadd__bSFL2pTEff__l2tight/latino_WWTo2L2Nu.root
//////////////////////////////////////////////////////////

#ifndef AnalysisBase_h
#define AnalysisBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

using namespace std;

class AnalysisBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         GEN_weight_SM;
   //   Float_t         Gen_ZGstar_deltaR;
   Float_t         Gen_ZGstar_mass;
   Float_t         Gen_ZGstar_MomId;
   //   Float_t         Gen_ZGstar_mu1_eta;
   //   Float_t         Gen_ZGstar_mu1_phi;
   //   Float_t         Gen_ZGstar_mu1_pt;
   //   Float_t         Gen_ZGstar_mu2_eta;
   //   Float_t         Gen_ZGstar_mu2_phi;
   //   Float_t         Gen_ZGstar_mu2_pt;
   Float_t         topLHEpt;
   Float_t         antitopLHEpt;
   //   Float_t         LHE_weight_SM;
   //   Float_t         chSumEt;
   //   Float_t         chmet;
   //   Float_t         chmetphi;
   //   Float_t         dataset;
   //   Float_t         effW;
   //   Float_t         fakeW;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleUp;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleDown;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleUp;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleDown;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuUp;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuDown;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuUp;
   Float_t         fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuDown;
   Float_t         fakeW2l0j;
   Float_t         fakeW2l0jElUp;
   Float_t         fakeW2l0jElDown;
   Float_t         fakeW2l0jstatElUp;
   Float_t         fakeW2l0jstatElDown;
   Float_t         fakeW2l0jMuUp;
   Float_t         fakeW2l0jMuDown;
   Float_t         fakeW2l0jstatMuUp;
   Float_t         fakeW2l0jstatMuDown;
   Float_t         fakeW2l1j;
   Float_t         fakeW2l1jElUp;
   Float_t         fakeW2l1jElDown;
   Float_t         fakeW2l1jstatElUp;
   Float_t         fakeW2l1jstatElDown;
   Float_t         fakeW2l1jMuUp;
   Float_t         fakeW2l1jMuDown;
   Float_t         fakeW2l1jstatMuUp;
   Float_t         fakeW2l1jstatMuDown;
   Float_t         fakeW2l2j;
   Float_t         fakeW2l2jElUp;
   Float_t         fakeW2l2jElDown;
   Float_t         fakeW2l2jstatElUp;
   Float_t         fakeW2l2jstatElDown;
   Float_t         fakeW2l2jMuUp;
   Float_t         fakeW2l2jMuDown;
   Float_t         fakeW2l2jstatMuUp;
   Float_t         fakeW2l2jstatMuDown;
   Float_t         fakeW3l;
   Float_t         fakeW3lElUp;
   Float_t         fakeW3lElDown;
   Float_t         fakeW3lstatElUp;
   Float_t         fakeW3lstatElDown;
   Float_t         fakeW3lMuUp;
   Float_t         fakeW3lMuDown;
   Float_t         fakeW3lstatMuUp;
   Float_t         fakeW3lstatMuDown;
   //   Float_t         hardbjpb;
   //   Float_t         hardcmvav2;
   //   Float_t         hardcsvv2ivf;
   //   Float_t         hardpfcsv;
   //   Float_t         hardssvhb;
   //   Float_t         hardssvhe;
   //   Float_t         hardtche;
   //   Float_t         higgsGeneta;
   //   Float_t         higgsGenmass;
   //   Float_t         higgsGenphi;
   //   Float_t         higgsGenpt;
   //   Float_t         higgsLHEeta;
   //   Float_t         higgsLHEmass;
   //   Float_t         higgsLHEphi;
   //   Float_t         higgsLHEpt;
   //   Float_t         imet;
   Float_t         jetRho;
   //   Float_t         jetcmvav21;
   //   Float_t         jetcmvav22;
   //   Float_t         jetcsvv2ivf1;
   //   Float_t         jetcsvv2ivf2;
   //   Float_t         jetpfcsv1;
   //   Float_t         jetpfcsv2;
   //   Float_t         jetssvhb1;
   //   Float_t         jetssvhb2;
   //   Float_t         jetssvhe1;
   //   Float_t         jetssvhe2;
   //   Float_t         kfW;
   //   Float_t         mctruth;
   Float_t         metGeneta;
   Float_t         metGenphi;
   Float_t         metGenpt;
   //   Float_t         metLHEeta;
   //   Float_t         metLHEphi;
   //   Float_t         metLHEpt;
   //   Float_t         metPfNoHf;
   //   Float_t         metPfProj;
   //   Float_t         metPfProjElecEnDn;
   //   Float_t         metPfProjElecEnUp;
   //   Float_t         metPfProjJetEnDn;
   //   Float_t         metPfProjJetEnUp;
   //   Float_t         metPfProjJetResDn;
   //   Float_t         metPfProjJetResUp;
   //   Float_t         metPfProjMuonEnDn;
   //   Float_t         metPfProjMuonEnUp;
   //   Float_t         metPfProjUnclEnDn;
   //   Float_t         metPfProjUnclEnUp;
   //   Float_t         metPfRaw;
   //   Float_t         metPfRawElecEnDn;
   //   Float_t         metPfRawElecEnUp;
   //   Float_t         metPfRawJetEnDn;
   //   Float_t         metPfRawJetEnUp;
   //   Float_t         metPfRawJetResDn;
   //   Float_t         metPfRawJetResUp;
   //   Float_t         metPfRawMuonEnDn;
   //   Float_t         metPfRawMuonEnUp;
   //   Float_t         metPfRawPhi;
   //   Float_t         metPfRawPhiElecEnDn;
   //   Float_t         metPfRawPhiElecEnUp;
   //   Float_t         metPfRawPhiJetEnDn;
   //   Float_t         metPfRawPhiJetEnUp;
   //   Float_t         metPfRawPhiJetResDn;
   //   Float_t         metPfRawPhiJetResUp;
   //   Float_t         metPfRawPhiMuonEnDn;
   //   Float_t         metPfRawPhiMuonEnUp;
   //   Float_t         metPfRawPhiUnclEnDn;
   //   Float_t         metPfRawPhiUnclEnUp;
   //   Float_t         metPfRawSumEt;
   //   Float_t         metPfRawUnclEnDn;
   //   Float_t         metPfRawUnclEnUp;
   //   Float_t         metPfType1ElecEnDn;
   //   Float_t         metPfType1ElecEnUp;
   //   Float_t         metPfType1JetEnDn;
   //   Float_t         metPfType1JetEnUp;
   //   Float_t         metPfType1JetResDn;
   //   Float_t         metPfType1JetResUp;
   //   Float_t         metPfType1MuonEnDn;
   //   Float_t         metPfType1MuonEnUp;
   //   Float_t         metPfType1SumEt;
   //   Float_t         metPfType1UnclEnDn;
   //   Float_t         metPfType1UnclEnUp;
   Float_t         metPuppi;
   Float_t         metTtrk;
   Float_t         metTtrkPhi;
   //   Float_t         mllg;
   //   Float_t         mllgid;
   Float_t         mpmet;
   //   Float_t         nPhos;
   //   Float_t         nbjet;
   //   Float_t         nbjettche;
   //   Float_t         nextra;
   //   Float_t         njetid;
   //   Float_t         pchmet;
   //   Float_t         pdfid1;
   //   Float_t         pdfid2;
   //   Float_t         pdfscalePDF;
   //   Float_t         pdfx1;
   //   Float_t         pdfx1PDF;
   //   Float_t         pdfx2;
   //   Float_t         pdfx2PDF;
   //   Float_t         peaking;
   Float_t         pfmetMEtSig;
   Float_t         pfmetSignificance;
   //   Float_t         pho_HoE;
   //   Float_t         pho_chIso;
   //   Float_t         pho_hasPixelSeed;
   //   Float_t         pho_n_id;
   //   Float_t         pho_nhIso;
   //   Float_t         pho_passElecVeto;
   //   Float_t         pho_phIso;
   //   Float_t         pho_sietaieta;
   //   Float_t         predmet;
   //   Float_t         redmet;   
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          event;
   //   Float_t         softbjpb;
   //   Float_t         softcmvav2;
   //   Float_t         softcsvv2ivf;
   //   Float_t         softpfcsv;
   //   Float_t         softssvhb;
   //   Float_t         softssvhe;
   //   Float_t         softtche;


   // 3lepton variables
   Float_t         njet_3l;
   Float_t         flagOSSF;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         eta3;
   Float_t         zveto_3l;
   Float_t         pfmet;
   Float_t         chlll;
   Float_t         drllmin3l;
   Float_t         mlll;
   Float_t         mllmin3l;
   Float_t         phi3;
   Float_t         phi2;
   Float_t         phi1;
   Float_t         pt3;
   Float_t         nbjet_3l;


   // Full2016_Apr17 electron working points
   Float_t         LepCut2l__ele_cut_WP_Tight80X__mu_cut_Tight80x;
   Float_t         LepSF2l__ele_cut_WP_Tight80X__mu_cut_Tight80x;
   Float_t         LepSF2l__mu_cut_Tight80x__Do;
   Float_t         LepSF2l__mu_cut_Tight80x__Up;
   Float_t         LepSF2l__ele_cut_WP_Tight80X__Do;
   Float_t         LepSF2l__ele_cut_WP_Tight80X__Up;
   Float_t         METFilter_DATA;
   Float_t         METFilter_MC;

   vector<float>   *std_vector_muon_isTightLepton_cut_Tight80x;

   vector<float>   *std_vector_muon_idisoW_cut_Tight80x;
   vector<float>   *std_vector_muon_idisoW_cut_Tight80x_Down;
   vector<float>   *std_vector_muon_idisoW_cut_Tight80x_Up;
   
   vector<float>   *std_vector_electron_isTightLepton_cut_WP_Tight80X;
   vector<float>   *std_vector_electron_isTightLepton_cut_WP_Tight80X_SS;
   vector<float>   *std_vector_electron_isTightLepton_mva_80p_Iso2015;
   vector<float>   *std_vector_electron_isTightLepton_mva_80p_Iso2016;
   vector<float>   *std_vector_electron_isTightLepton_mva_90p_Iso2015;
   vector<float>   *std_vector_electron_isTightLepton_mva_90p_Iso2016;

   vector<float>   *std_vector_electron_idisoW_cut_WP_Tight80X;
   vector<float>   *std_vector_electron_idisoW_cut_WP_Tight80X_Down;
   vector<float>   *std_vector_electron_idisoW_cut_WP_Tight80X_Up;

   vector<float>   *std_vector_electron_idisoW_cut_WP_Tight80X_SS;
   vector<float>   *std_vector_electron_idisoW_mva_80p_Iso2015;
   vector<float>   *std_vector_electron_idisoW_mva_80p_Iso2016;
   vector<float>   *std_vector_electron_idisoW_mva_90p_Iso2015;
   vector<float>   *std_vector_electron_idisoW_mva_90p_Iso2016;

   //   vector<float>   *std_vector_GEN_weight;
   vector<float>   *std_vector_LHE_weight;
   //   vector<float>   *std_vector_LHElepton_eta;
   //   vector<float>   *std_vector_LHElepton_id;
   //   vector<float>   *std_vector_LHElepton_phi;
   //   vector<float>   *std_vector_LHElepton_pt;
   //   vector<float>   *std_vector_LHEneutrino_eta;
   //   vector<float>   *std_vector_LHEneutrino_id;
   //   vector<float>   *std_vector_LHEneutrino_phi;
   //   vector<float>   *std_vector_LHEneutrino_pt;
   //   vector<float>   *std_vector_LHEparton_eta;
   //   vector<float>   *std_vector_LHEparton_id;
   //   vector<float>   *std_vector_LHEparton_phi;
   vector<float>   *std_vector_LHEparton_pt;
   vector<float>   *std_vector_VBoson_eta;
   vector<float>   *std_vector_VBoson_mass;
   vector<float>   *std_vector_VBoson_phi;
   vector<float>   *std_vector_VBoson_pid;
   vector<float>   *std_vector_VBoson_pt;
   //   vector<float>   *std_vector_dressedLeptonGen_eta;
   //   vector<float>   *std_vector_dressedLeptonGen_phi;
   //   vector<float>   *std_vector_dressedLeptonGen_pid;
   //   vector<float>   *std_vector_dressedLeptonGen_pt;
   vector<float>   *std_vector_jetGen_eta;
   vector<float>   *std_vector_jetGen_phi;
   vector<float>   *std_vector_jetGen_pt;
   vector<float>   *std_vector_leptonGen_eta;
   //   vector<float>   *std_vector_leptonGen_fromHardProcess;
   //   vector<float>   *std_vector_leptonGen_isDirectHadronDecayProduct;
   vector<float>   *std_vector_leptonGen_isDirectPromptTauDecayProduct;
   vector<float>   *std_vector_leptonGen_isPrompt;
   //   vector<float>   *std_vector_leptonGen_isTauDecayProduct;
   vector<float>   *std_vector_leptonGen_phi;
   vector<float>   *std_vector_leptonGen_pid;
   vector<float>   *std_vector_leptonGen_pt;
   vector<float>   *std_vector_leptonGen_status;
   vector<float>   *std_vector_leptonGen_MotherPID;
   vector<float>   *std_vector_DarkMatterGen_pt;
   vector<float>   *std_vector_neutrinoGen_eta;
   vector<float>   *std_vector_neutrinoGen_isDirectPromptTauDecayProduct;
   vector<float>   *std_vector_neutrinoGen_isPrompt;
   vector<float>   *std_vector_neutrinoGen_phi;
   vector<float>   *std_vector_neutrinoGen_pid;
   vector<float>   *std_vector_neutrinoGen_MotherPID;
   vector<float>   *std_vector_neutrinoGen_pt;
   vector<float>   *std_vector_partonGen_eta;
   vector<float>   *std_vector_partonGen_isHardProcess;
   //   vector<float>   *std_vector_partonGen_isPrompt;
   vector<float>   *std_vector_partonGen_phi;
   vector<float>   *std_vector_partonGen_pid;
   vector<float>   *std_vector_partonGen_pt;
   //   vector<float>   *std_vector_photon_eta;
   //   vector<float>   *std_vector_photon_phi;
   //   vector<float>   *std_vector_photon_pt;
   //   vector<float>   *std_vector_photonid_eta;
   //   vector<float>   *std_vector_photonid_phi;
   //   vector<float>   *std_vector_photonid_pt;
   vector<float>   *std_vector_softMuD0;
   vector<float>   *std_vector_softMuDz;
   //   vector<float>   *std_vector_softMuEta;
   vector<float>   *std_vector_softMuIsTrackerMuon;
   vector<float>   *std_vector_softMuIso;
   //   vector<float>   *std_vector_softMuPhi;
   vector<float>   *std_vector_softMuPt;
   vector<float>   *std_vector_softMuTMLastStationAngTight;
   //   vector<float>   *std_vector_tau_eta;
   //   vector<float>   *std_vector_tau_phi;
   //   vector<float>   *std_vector_tau_pt;
   //   vector<float>   *std_vector_trackjet_eta;
   //   vector<float>   *std_vector_trackjet_phi;
   //   vector<float>   *std_vector_trackjet_probabilityB;
   //   vector<float>   *std_vector_trackjet_pt;
   vector<float>   *std_vector_trigger_special;
   vector<float>   *std_vector_trigger;
   //   Float_t         tightmu;
   Float_t         triggW;
   Float_t         trigger;
   Float_t         trig_DbleEle;
   Float_t         trig_SnglEle;
   Float_t         trig_DbleMu;
   Float_t         trig_SnglMu;
   Float_t         trig_EleMu;
   Float_t         veto_EMTFBug;
   //   Float_t         triggerFakeRate;
   //   Float_t         itpu;
   Float_t         nvtx;
   Float_t         nGoodVtx;
   //   Float_t         ootpum1;
   //   Float_t         ootpum2;
   //   Float_t         ootpum3;
   //   Float_t         ootpum4;
   //   Float_t         ootpum5;
   //   Float_t         ootpup1;
   //   Float_t         ootpup2;
   //   Float_t         ootpup3;
   //   Float_t         trpu;
   //   Int_t           bveto_ip;
   //   vector<float>   *std_vector_electron_ooEmooP;
   //   vector<float>   *std_vector_jet_puid;
   //   vector<float>   *std_vector_muon_NTkLayers;
   vector<float>   *std_vector_jet_cmvav2;
   //   vector<float>   *std_vector_muon_Chi2LocalPos;
   vector<float>   *std_vector_jet_phi;
   vector<float>   *std_vector_jet_mass;
   vector<float>   *std_vector_jet_chargedHadronFraction;
   //   vector<float>   *std_vector_muon_NValidPixelHitsInTrk;
   //   vector<float>   *std_vector_electron_passConversionVeto;
   //   vector<float>   *std_vector_puppijet_eta;
   //   vector<float>   *std_vector_muon_NValidHitsSATrk;
   //   vector<float>   *std_vector_electron_ecalPFClusterIso;
   //   vector<float>   *std_vector_jet_tche;
   //   vector<float>   *std_vector_muon_NValidHitsInTrk;
   vector<float>   *std_vector_jet_csvv2ivf;
   //   vector<float>   *std_vector_jet_tchp;
   //   vector<float>   *std_vector_electron_dEtaIn;
   //   vector<float>   *std_vector_muon_TrkKink;
   //   vector<float>   *std_vector_muon_NormChi2GTrk;
   //   vector<float>   *std_vector_electron_full5x5_sigmaIetaIeta;
   //   vector<float>   *std_vector_jet_ssvhe;
   //   vector<float>   *std_vector_puppijet_phi;
   vector<float>   *std_vector_jet_eta;
   vector<float>   *std_vector_jet_pt;
   vector<float>   *std_vector_jet_isFromISR;
   //   vector<float>   *std_vector_muon_NValidFractInTrk;
   //   vector<float>   *std_vector_jet_bjp;
   //   vector<float>   *std_vector_puppijet_pt;
   //   vector<float>   *std_vector_muon_SegCompatibilty;
   vector<float>   *std_vector_electron_effectiveArea;
   vector<float>   *std_vector_jet_PartonFlavour;
   //   vector<float>   *std_vector_jet_softMuDz;
   //   vector<float>   *std_vector_jet_softMuIso;
   //   vector<float>   *std_vector_jet_softMuEta;
   //   vector<float>   *std_vector_jet_softMuPt;
   //   vector<float>   *std_vector_jet_ssvhb;
   //   vector<float>   *std_vector_jet_softMuD0;
   //   vector<float>   *std_vector_jet_QGlikelihood;
   //   vector<float>   *std_vector_muon_NumOfMatchedStations;
   //   vector<float>   *std_vector_jet_softMuPhi;
   //   vector<float>   *std_vector_electron_hcalPFClusterIso;
   //   vector<float>   *std_vector_jet_bjpb;
   //   vector<float>   *std_vector_jet_pfcsv;
   //   vector<float>   *std_vector_electron_dPhiIn;
   //   vector<float>   *std_vector_electron_hOverE;
   //   vector<float>   *std_vector_jet_NumberSoftMu;
   vector<float>   *std_vector_jet_HadronFlavour;
   //   vector<float>   *std_vector_electron_scEta;
   //   vector<float>   *std_vector_electron_expectedMissingInnerHits;
   Float_t         metFilter;
   Float_t         jetpt1;
   Float_t         jetpt2;
   Float_t         jeteta1;
   Float_t         jeteta2;
   Float_t         jetphi1;
   Float_t         jetphi2;
   Float_t         jetmass1;
   Float_t         jetmass2;
   //   Float_t         jettche1;
   //   Float_t         jettche2;
   Float_t         puW;
   //   Float_t         puW6p3;
   //   Float_t         Xsec;
   Float_t         baseW;
   //   Float_t         nllnnloW;
   Float_t         nllW;
   //   Float_t         nllW_Rup;
   //   Float_t         nllW_Qup;
   //   Float_t         nllW_Rdown;
   //   Float_t         nllW_Qdown;
   //   Float_t         gen_mww;
   //   Float_t         gen_ptww;
   Float_t         gen_ptll;
   vector<float>   *std_vector_lepton_chargedHadronIso;
   vector<float>   *std_vector_lepton_photonIso;
   //   vector<float>   *std_vector_lepton_closejet_PartonFlavour;
   //   vector<float>   *std_vector_lepton_closejet_pt;
   vector<float>   *std_vector_lepton_neutralHadronIso;
   vector<float>   *std_vector_lepton_eleIdMedium;
   //   vector<float>   *std_vector_lepton_chargedParticleIso;
   vector<float>   *std_vector_lepton_phi;
   //   vector<float>   *std_vector_lepton_eleIdLoose;
   //   vector<float>   *std_vector_lepton_dz;
   //   vector<float>   *std_vector_lepton_eleIdVeto;
   vector<float>   *std_vector_lepton_isLooseLepton;
   vector<float>   *std_vector_lepton_pt;
   vector<float>   *std_vector_lepton_sumPUPt;
   vector<float>   *std_vector_lepton_eta;
   vector<float>   *std_vector_lepton_isMediumMuon;
   //   vector<float>   *std_vector_lepton_PfIsoDeltaBeta;
   //   vector<float>   *std_vector_lepton_closejet_phi;
   vector<float>   *std_vector_lepton_trackIso;
   //   vector<float>   *std_vector_lepton_isWgsLepton;
   //   vector<float>   *std_vector_lepton_closejet_eta;
   //   vector<float>   *std_vector_lepton_SIP3D;
   vector<float>   *std_vector_lepton_isTightLepton;
   vector<float>   *std_vector_lepton_ch;
   vector<float>   *std_vector_lepton_flavour;
   //   vector<float>   *std_vector_lepton_closejet_drlj;
   vector<float>   *std_vector_lepton_eleIdTight;
   vector<float>   *std_vector_lepton_d0;
   vector<float>   *std_vector_lepton_genmatched;
   vector<float>   *std_vector_lepton_promptgenmatched;
   vector<float>   *std_vector_lepton_isTightMuon;
   Float_t         susyMstop;
   Float_t         susyMLSP;
   Float_t         metPfType1Phi;
   Float_t         metPfType1;
   Float_t         bPogSF;
   Float_t         bPogSFUp;
   Float_t         bPogSFDown;
   Float_t         bPogSF_CMVAL;
   Float_t         bPogSF_CMVAL_up;
   Float_t         bPogSF_CMVAL_down;
   Float_t         bPogSF_CMVAM;
   Float_t         bPogSF_CMVAM_up;
   Float_t         bPogSF_CMVAM_down;
   Float_t         bPogSF_CMVAT;
   Float_t         bPogSF_CMVAT_up;
   Float_t         bPogSF_CMVAT_down;
   Float_t         bPogSF_CSVL;
   Float_t         bPogSF_CSVL_up;
   Float_t         bPogSF_CSVL_down;
   Float_t         bPogSF_CSVM;
   Float_t         bPogSF_CSVM_up;
   Float_t         bPogSF_CSVM_down;
   Float_t         bPogSF_CSVT;
   Float_t         bPogSF_CSVT_up;
   Float_t         bPogSF_CSVT_down;
   //   Float_t         bTPSF;
   //   Float_t         bTPSFUp;
   //   Float_t         bTPSFDown;
   //   Float_t         bPogSF1Jet;
   //   Float_t         bPogSF1JetUp;
   //   Float_t         bPogSF1JetDown;
   //   Float_t         bTPSF1Jet;
   //   Float_t         bTPSF1JetUp;
   //   Float_t         bTPSF1JetDown;
   //   Float_t         bPogSF2Jet;
   //   Float_t         bPogSF2JetUp;
   //   Float_t         bPogSF2JetDown;
   //   Float_t         bTPSF2Jet;
   //   Float_t         bTPSF2JetUp;
   //   Float_t         bTPSF2JetDown;
   Float_t         effTrigW_Down;
   Float_t         effTrigW_Up;
   Float_t         effTrigW;
   vector<float>   *std_vector_lepton_idisoWcut_WP_Tight80X;
   vector<float>   *std_vector_lepton_idisoWcut_WP_Tight80X_Up;
   vector<float>   *std_vector_lepton_idisoWcut_WP_Tight80X_Down;
   vector<float>   *std_vector_lepton_idisoW;
   vector<float>   *std_vector_lepton_idisoW_Up;
   vector<float>   *std_vector_lepton_idisoW_Down;
   //   vector<float>   *std_vector_lepton_idisoW_Syst;
   //   vector<float>   *std_vector_lepton_idisoLooseW;
   //   vector<float>   *std_vector_lepton_idisoLooseW_Up;
   //   vector<float>   *std_vector_lepton_idisoLooseW_Down;
   //   vector<float>   *std_vector_lepton_idisoLooseW_Syst;
   vector<float>   *std_vector_lepton_recoW;
   vector<float>   *std_vector_lepton_recoW_Up;
   vector<float>   *std_vector_lepton_recoW_Down;
   vector<float>   *std_vector_lepton_fastsimW;
   vector<float>   *std_vector_lepton_fastsimW_Up;
   vector<float>   *std_vector_lepton_fastsimW_Down;
   //   Float_t         mcoll;
   //   Float_t         mcollWW;
   //   Float_t         vht_pt;
   //   Float_t         mjj;
   //   Float_t         mllThird;
   Float_t         dphilljetjet;
   //   Float_t         vht_phi;
   Float_t         drll;
   Float_t         channel;
   Float_t         njet;
   Float_t         dphilljet;
   Float_t         dphilmet1;
   Float_t         dphilmet2;
   //   Float_t         projpfmet;
   //   Float_t         mTi;
   Float_t         ht;
   Float_t         dphill;
   Float_t         pt2;
   Float_t         pt1;
   Float_t         detajj;
   Float_t         ptll;
   Float_t         mll;
   Float_t         dphilmet;
   Float_t         mth;
   Float_t         yll;
   Float_t         mtw2;
   Float_t         mtw1;
   //   Float_t         isJsonOk;

   // List of branches
   TBranch        *b_GEN_weight_SM;   //!
   //   TBranch        *b_Gen_ZGstar_deltaR;   //!
   TBranch        *b_Gen_ZGstar_mass;   //!
   TBranch        *b_Gen_ZGstar_MomId;   //!
   //   TBranch        *b_Gen_ZGstar_mu1_eta;   //!
   //   TBranch        *b_Gen_ZGstar_mu1_phi;   //!
   //   TBranch        *b_Gen_ZGstar_mu1_pt;   //!
   //   TBranch        *b_Gen_ZGstar_mu2_eta;   //!
   //   TBranch        *b_Gen_ZGstar_mu2_phi;   //!
   //   TBranch        *b_Gen_ZGstar_mu2_pt;   //!
   TBranch        *b_topLHEpt;   //!
   TBranch        *b_antitopLHEpt;   //!
   //   TBranch        *b_LHE_weight_SM;   //!
   //   TBranch        *b_chSumEt;   //!
   //   TBranch        *b_chmet;   //!
   //   TBranch        *b_chmetphi;   //!
   //   TBranch        *b_dataset;   //!
   //   TBranch        *b_effW;   //!
   //   TBranch        *b_fakeW;   //!
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleUp;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleDown;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleUp;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleDown;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuUp;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuDown;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuUp;
   TBranch        *b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuDown;
   TBranch        *b_fakeW3l;
   TBranch        *b_fakeW3lElUp;
   TBranch        *b_fakeW3lElDown;
   TBranch        *b_fakeW3lstatElUp;
   TBranch        *b_fakeW3lstatElDown;
   TBranch        *b_fakeW3lMuUp;
   TBranch        *b_fakeW3lMuDown;
   TBranch        *b_fakeW3lstatMuUp;
   TBranch        *b_fakeW3lstatMuDown;
   TBranch        *b_fakeW2l0j;
   TBranch        *b_fakeW2l0jElUp;
   TBranch        *b_fakeW2l0jElDown;
   TBranch        *b_fakeW2l0jstatElUp;
   TBranch        *b_fakeW2l0jstatElDown;
   TBranch        *b_fakeW2l0jMuUp;
   TBranch        *b_fakeW2l0jMuDown;
   TBranch        *b_fakeW2l0jstatMuUp;
   TBranch        *b_fakeW2l0jstatMuDown;
   TBranch        *b_fakeW2l1j;
   TBranch        *b_fakeW2l1jElUp;
   TBranch        *b_fakeW2l1jElDown;
   TBranch        *b_fakeW2l1jstatElUp;
   TBranch        *b_fakeW2l1jstatElDown;
   TBranch        *b_fakeW2l1jMuUp;
   TBranch        *b_fakeW2l1jMuDown;
   TBranch        *b_fakeW2l1jstatMuUp;
   TBranch        *b_fakeW2l1jstatMuDown;
   TBranch        *b_fakeW2l2j;
   TBranch        *b_fakeW2l2jElUp;
   TBranch        *b_fakeW2l2jElDown;
   TBranch        *b_fakeW2l2jstatElUp;
   TBranch        *b_fakeW2l2jstatElDown;
   TBranch        *b_fakeW2l2jMuUp;
   TBranch        *b_fakeW2l2jMuDown;
   TBranch        *b_fakeW2l2jstatMuUp;
   TBranch        *b_fakeW2l2jstatMuDown;
   //   TBranch        *b_hardbjpb;   //!
   //   TBranch        *b_hardcmvav2;   //!
   //   TBranch        *b_hardcsvv2ivf;   //!
   //   TBranch        *b_hardpfcsv;   //!
   //   TBranch        *b_hardssvhb;   //!
   //   TBranch        *b_hardssvhe;   //!
   //   TBranch        *b_hardtche;   //!
   //   TBranch        *b_higgsGeneta;   //!
   //   TBranch        *b_higgsGenmass;   //!
   //   TBranch        *b_higgsGenphi;   //!
   //   TBranch        *b_higgsGenpt;   //!
   //   TBranch        *b_higgsLHEeta;   //!
   //   TBranch        *b_higgsLHEmass;   //!
   //   TBranch        *b_higgsLHEphi;   //!
   //   TBranch        *b_higgsLHEpt;   //!
   //   TBranch        *b_imet;   //!
   TBranch        *b_jetRho;   //!
   //   TBranch        *b_jetcmvav21;   //!
   //   TBranch        *b_jetcmvav22;   //!
   //   TBranch        *b_jetcsvv2ivf1;   //!
   //   TBranch        *b_jetcsvv2ivf2;   //!
   //   TBranch        *b_jetpfcsv1;   //!
   //   TBranch        *b_jetpfcsv2;   //!
   //   TBranch        *b_jetssvhb1;   //!
   //   TBranch        *b_jetssvhb2;   //!
   //   TBranch        *b_jetssvhe1;   //!
   //   TBranch        *b_jetssvhe2;   //!
   //   TBranch        *b_kfW;   //!
   //   TBranch        *b_mctruth;   //!
   TBranch        *b_metGeneta;   //!
   TBranch        *b_metGenphi;   //!
   TBranch        *b_metGenpt;   //!
   //   TBranch        *b_metLHEeta;   //!
   //   TBranch        *b_metLHEphi;   //!
   //   TBranch        *b_metLHEpt;   //!
   //   TBranch        *b_metPfNoHf;   //!
   //   TBranch        *b_metPfProj;   //!
   //   TBranch        *b_metPfProjElecEnDn;   //!
   //   TBranch        *b_metPfProjElecEnUp;   //!
   //   TBranch        *b_metPfProjJetEnDn;   //!
   //   TBranch        *b_metPfProjJetEnUp;   //!
   //   TBranch        *b_metPfProjJetResDn;   //!
   //   TBranch        *b_metPfProjJetResUp;   //!
   //   TBranch        *b_metPfProjMuonEnDn;   //!
   //   TBranch        *b_metPfProjMuonEnUp;   //!
   //   TBranch        *b_metPfProjUnclEnDn;   //!
   //   TBranch        *b_metPfProjUnclEnUp;   //!
   //   TBranch        *b_metPfRaw;   //!
   //   TBranch        *b_metPfRawElecEnDn;   //!
   //   TBranch        *b_metPfRawElecEnUp;   //!
   //   TBranch        *b_metPfRawJetEnDn;   //!
   //   TBranch        *b_metPfRawJetEnUp;   //!
   //   TBranch        *b_metPfRawJetResDn;   //!
   //   TBranch        *b_metPfRawJetResUp;   //!
   //   TBranch        *b_metPfRawMuonEnDn;   //!
   //   TBranch        *b_metPfRawMuonEnUp;   //!
   //   TBranch        *b_metPfRawPhi;   //!
   //   TBranch        *b_metPfRawPhiElecEnDn;   //!
   //   TBranch        *b_metPfRawPhiElecEnUp;   //!
   //   TBranch        *b_metPfRawPhiJetEnDn;   //!
   //   TBranch        *b_metPfRawPhiJetEnUp;   //!
   //   TBranch        *b_metPfRawPhiJetResDn;   //!
   //   TBranch        *b_metPfRawPhiJetResUp;   //!
   //   TBranch        *b_metPfRawPhiMuonEnDn;   //!
   //   TBranch        *b_metPfRawPhiMuonEnUp;   //!
   //   TBranch        *b_metPfRawPhiUnclEnDn;   //!
   //   TBranch        *b_metPfRawPhiUnclEnUp;   //!
   //   TBranch        *b_metPfRawSumEt;   //!
   //   TBranch        *b_metPfRawUnclEnDn;   //!
   //   TBranch        *b_metPfRawUnclEnUp;   //!
   //   TBranch        *b_metPfType1ElecEnDn;   //!
   //   TBranch        *b_metPfType1ElecEnUp;   //!
   //   TBranch        *b_metPfType1JetEnDn;   //!
   //   TBranch        *b_metPfType1JetEnUp;   //!
   //   TBranch        *b_metPfType1JetResDn;   //!
   //   TBranch        *b_metPfType1JetResUp;   //!
   //   TBranch        *b_metPfType1MuonEnDn;   //!
   //   TBranch        *b_metPfType1MuonEnUp;   //!
   //   TBranch        *b_metPfType1SumEt;   //!
   //   TBranch        *b_metPfType1UnclEnDn;   //!
   //   TBranch        *b_metPfType1UnclEnUp;   //!
   TBranch        *b_metPuppi;   //!
   TBranch        *b_metTtrk;   //!
   TBranch        *b_metTtrkPhi;   //!
   //   TBranch        *b_mllg;   //!
   //   TBranch        *b_mllgid;   //!
   TBranch        *b_mpmet;   //!
   //   TBranch        *b_nPhos;   //!
   //   TBranch        *b_nbjet;   //!
   //   TBranch        *b_nbjettche;   //!
   //   TBranch        *b_nextra;   //!
   //   TBranch        *b_njetid;   //!
   //   TBranch        *b_pchmet;   //!
   //   TBranch        *b_pdfid1;   //!
   //   TBranch        *b_pdfid2;   //!
   //   TBranch        *b_pdfscalePDF;   //!
   //   TBranch        *b_pdfx1;   //!
   //   TBranch        *b_pdfx1PDF;   //!
   //   TBranch        *b_pdfx2;   //!
   //   TBranch        *b_pdfx2PDF;   //!
   //   TBranch        *b_peaking;   //!
   TBranch        *b_pfmetMEtSig;   //!
   TBranch        *b_pfmetSignificance;   //!
   //   TBranch        *b_pho_HoE;   //!
   //   TBranch        *b_pho_chIso;   //!
   //   TBranch        *b_pho_hasPixelSeed;   //!
   //   TBranch        *b_pho_n_id;   //!
   //   TBranch        *b_pho_nhIso;   //!
   //   TBranch        *b_pho_passElecVeto;   //!
   //   TBranch        *b_pho_phIso;   //!
   //   TBranch        *b_pho_sietaieta;   //!
   //   TBranch        *b_predmet;   //!
   //   TBranch        *b_redmet;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   //   TBranch        *b_softbjpb;   //!
   //   TBranch        *b_softcmvav2;   //!
   //   TBranch        *b_softcsvv2ivf;   //!
   //   TBranch        *b_softpfcsv;   //!
   //   TBranch        *b_softssvhb;   //!
   //   TBranch        *b_softssvhe;   //!
   //   TBranch        *b_softtche;   //!
   //   TBranch        *b_std_vector_GEN_weight;   //!
   TBranch        *b_std_vector_LHE_weight;   //!
   //   TBranch        *b_std_vector_LHElepton_eta;   //!
   //   TBranch        *b_std_vector_LHElepton_id;   //!
   //   TBranch        *b_std_vector_LHElepton_phi;   //!
   //   TBranch        *b_std_vector_LHElepton_pt;   //!
   //   TBranch        *b_std_vector_LHEneutrino_eta;   //!
   //   TBranch        *b_std_vector_LHEneutrino_id;   //!
   //   TBranch        *b_std_vector_LHEneutrino_phi;   //!
   //   TBranch        *b_std_vector_LHEneutrino_pt;   //!
   //   TBranch        *b_std_vector_LHEparton_eta;   //!
   //   TBranch        *b_std_vector_LHEparton_id;   //!
   //   TBranch        *b_std_vector_LHEparton_phi;   //!
   TBranch        *b_std_vector_LHEparton_pt;   //!
   TBranch        *b_std_vector_VBoson_eta;   //!
   TBranch        *b_std_vector_VBoson_mass;   //!
   TBranch        *b_std_vector_VBoson_phi;   //!
   TBranch        *b_std_vector_VBoson_pid;   //!
   TBranch        *b_std_vector_VBoson_pt;   //!
   //   TBranch        *b_std_vector_dressedLeptonGen_eta;   //!
   //   TBranch        *b_std_vector_dressedLeptonGen_phi;   //!
   //   TBranch        *b_std_vector_dressedLeptonGen_pid;   //!
   //   TBranch        *b_std_vector_dressedLeptonGen_pt;   //!
   TBranch        *b_std_vector_jetGen_eta;   //!
   TBranch        *b_std_vector_jetGen_phi;   //!
   TBranch        *b_std_vector_jetGen_pt;   //!
   TBranch        *b_std_vector_leptonGen_eta;   //!
   //   TBranch        *b_std_vector_leptonGen_fromHardProcess;   //!
   //   TBranch        *b_std_vector_leptonGen_isDirectHadronDecayProduct;   //!
   TBranch        *b_std_vector_leptonGen_isDirectPromptTauDecayProduct;   //!
   TBranch        *b_std_vector_leptonGen_isPrompt;   //!
   //   TBranch        *b_std_vector_leptonGen_isTauDecayProduct;   //!
   TBranch        *b_std_vector_leptonGen_phi;   //!
   TBranch        *b_std_vector_leptonGen_pid;   //!
   TBranch        *b_std_vector_leptonGen_pt;   //!
   TBranch        *b_std_vector_leptonGen_status;   //!
   TBranch        *b_std_vector_leptonGen_MotherPID;   //!
   TBranch        *b_std_vector_DarkMatterGen_pt;   //!
   TBranch        *b_std_vector_neutrinoGen_eta;   //!
   TBranch        *b_std_vector_neutrinoGen_isDirectPromptTauDecayProduct;   //!
   TBranch        *b_std_vector_neutrinoGen_isPrompt;   //!
   TBranch        *b_std_vector_neutrinoGen_phi;   //!
   TBranch        *b_std_vector_neutrinoGen_pid;   //!
   TBranch        *b_std_vector_neutrinoGen_MotherPID;   //!
   TBranch        *b_std_vector_neutrinoGen_pt;   //!
   TBranch        *b_std_vector_partonGen_eta;   //!
   TBranch        *b_std_vector_partonGen_isHardProcess;   //!
   //   TBranch        *b_std_vector_partonGen_isPrompt;   //!
   TBranch        *b_std_vector_partonGen_phi;   //!
   TBranch        *b_std_vector_partonGen_pid;   //!
   TBranch        *b_std_vector_partonGen_pt;   //!
   //   TBranch        *b_std_vector_photon_eta;   //!
   //   TBranch        *b_std_vector_photon_phi;   //!
   //   TBranch        *b_std_vector_photon_pt;   //!
   //   TBranch        *b_std_vector_photonid_eta;   //!
   //   TBranch        *b_std_vector_photonid_phi;   //!
   //   TBranch        *b_std_vector_photonid_pt;   //!
   TBranch        *b_std_vector_softMuD0;   //!
   TBranch        *b_std_vector_softMuDz;   //!
   //   TBranch        *b_std_vector_softMuEta;   //!
   TBranch        *b_std_vector_softMuIsTrackerMuon;   //!
   TBranch        *b_std_vector_softMuIso;   //!
   //   TBranch        *b_std_vector_softMuPhi;   //!
   TBranch        *b_std_vector_softMuPt;   //!
   TBranch        *b_std_vector_softMuTMLastStationAngTight;   //!
   //   TBranch        *b_std_vector_tau_eta;   //!
   //   TBranch        *b_std_vector_tau_phi;   //!
   //   TBranch        *b_std_vector_tau_pt;   //!
   //   TBranch        *b_std_vector_trackjet_eta;   //!
   //   TBranch        *b_std_vector_trackjet_phi;   //!
   //   TBranch        *b_std_vector_trackjet_probabilityB;   //!
   //   TBranch        *b_std_vector_trackjet_pt;   //!
   TBranch        *b_std_vector_trigger_special;   //!
   TBranch        *b_std_vector_trigger;   //!
   //   TBranch        *b_tightmu;   //!
   TBranch        *b_triggW;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_trig_DbleEle;
   TBranch        *b_trig_SnglEle;
   TBranch        *b_trig_DbleMu;
   TBranch        *b_trig_SnglMu;
   TBranch        *b_trig_EleMu;
   TBranch        *b_veto_EMTFBug;   //!
   //   TBranch        *b_triggerFakeRate;   //!
   //   TBranch        *b_itpu;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_nGoodVtx;   //!
   //   TBranch        *b_ootpum1;   //!
   //   TBranch        *b_ootpum2;   //!
   //   TBranch        *b_ootpum3;   //!
   //   TBranch        *b_ootpum4;   //!
   //   TBranch        *b_ootpum5;   //!
   //   TBranch        *b_ootpup1;   //!
   //   TBranch        *b_ootpup2;   //!
   //   TBranch        *b_ootpup3;   //!
   //   TBranch        *b_trpu;   //!
   //   TBranch        *b_bveto_ip;   //!
   //   TBranch        *b_std_vector_electron_ooEmooP;   //!
   //   TBranch        *b_std_vector_jet_puid;   //!
   //   TBranch        *b_std_vector_muon_NTkLayers;   //!
   TBranch        *b_std_vector_jet_cmvav2;   //!
   //   TBranch        *b_std_vector_muon_Chi2LocalPos;   //!
   TBranch        *b_std_vector_jet_phi;   //!
   TBranch        *b_std_vector_jet_mass;   //!
   TBranch        *b_std_vector_jet_chargedHadronFraction;   //!
   //   TBranch        *b_std_vector_muon_NValidPixelHitsInTrk;   //!
   //   TBranch        *b_std_vector_electron_passConversionVeto;   //!
   //   TBranch        *b_std_vector_puppijet_eta;   //!
   //   TBranch        *b_std_vector_muon_NValidHitsSATrk;   //!
   //   TBranch        *b_std_vector_electron_ecalPFClusterIso;   //!
   //   TBranch        *b_std_vector_jet_tche;   //!
   //   TBranch        *b_std_vector_muon_NValidHitsInTrk;   //!
   TBranch        *b_std_vector_jet_csvv2ivf;   //!
   //   TBranch        *b_std_vector_jet_tchp;   //!
   //   TBranch        *b_std_vector_electron_dEtaIn;   //!
   //   TBranch        *b_std_vector_muon_TrkKink;   //!
   //   TBranch        *b_std_vector_muon_NormChi2GTrk;   //!
   //   TBranch        *b_std_vector_electron_full5x5_sigmaIetaIeta;   //!
   //   TBranch        *b_std_vector_jet_ssvhe;   //!
   //   TBranch        *b_std_vector_puppijet_phi;   //!
   TBranch        *b_std_vector_jet_eta;   //!
   TBranch        *b_std_vector_jet_pt;   //!
   TBranch        *b_std_vector_jet_isFromISR;   //!
   //   TBranch        *b_std_vector_muon_NValidFractInTrk;   //!
   //   TBranch        *b_std_vector_jet_bjp;   //!
   //   TBranch        *b_std_vector_puppijet_pt;   //!
   //   TBranch        *b_std_vector_muon_SegCompatibilty;   //!
   TBranch        *b_std_vector_electron_effectiveArea;   //!
   TBranch        *b_std_vector_jet_PartonFlavour;   //!
   //   TBranch        *b_std_vector_jet_softMuDz;   //!
   //   TBranch        *b_std_vector_jet_softMuIso;   //!
   //   TBranch        *b_std_vector_jet_softMuEta;   //!
   //   TBranch        *b_std_vector_jet_softMuPt;   //!
   //   TBranch        *b_std_vector_jet_ssvhb;   //!
   //   TBranch        *b_std_vector_jet_softMuD0;   //!
   //   TBranch        *b_std_vector_jet_QGlikelihood;   //!
   //   TBranch        *b_std_vector_muon_NumOfMatchedStations;   //!
   //   TBranch        *b_std_vector_jet_softMuPhi;   //!
   //   TBranch        *b_std_vector_electron_hcalPFClusterIso;   //!
   //   TBranch        *b_std_vector_jet_bjpb;   //!
   //   TBranch        *b_std_vector_jet_pfcsv;   //!
   //   TBranch        *b_std_vector_electron_dPhiIn;   //!
   //   TBranch        *b_std_vector_electron_hOverE;   //!
   //   TBranch        *b_std_vector_jet_NumberSoftMu;   //!
   TBranch        *b_std_vector_jet_HadronFlavour;   //!
   //   TBranch        *b_std_vector_electron_scEta;   //!
   //   TBranch        *b_std_vector_electron_expectedMissingInnerHits;   //!
   TBranch        *b_metFilter;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_jetphi1;   //!
   TBranch        *b_jetphi2;   //!
   TBranch        *b_jetmass1;   //!
   TBranch        *b_jetmass2;   //!
   //   TBranch        *b_jettche1;   //!
   //   TBranch        *b_jettche2;   //!
   TBranch        *b_puW;   //!
   //   TBranch        *b_puW6p3;   //!
   //   TBranch        *b_Xsec;   //!
   TBranch        *b_baseW;   //!
   //   TBranch        *b_nllnnloW;   //!
   TBranch        *b_nllW;   //!
   //   TBranch        *b_nllW_Rup;   //!
   //   TBranch        *b_nllW_Qup;   //!
   //   TBranch        *b_nllW_Rdown;   //!
   //   TBranch        *b_nllW_Qdown;   //!
   //   TBranch        *b_gen_mww;   //!
   //   TBranch        *b_gen_ptww;   //!
   TBranch        *b_gen_ptll;   //!
   TBranch        *b_std_vector_lepton_chargedHadronIso;   //!
   TBranch        *b_std_vector_lepton_photonIso;   //!
   //   TBranch        *b_std_vector_lepton_closejet_PartonFlavour;   //!
   //   TBranch        *b_std_vector_lepton_closejet_pt;   //!
   TBranch        *b_std_vector_lepton_neutralHadronIso;   //!
   TBranch        *b_std_vector_lepton_eleIdMedium;   //!
   //   TBranch        *b_std_vector_lepton_chargedParticleIso;   //!
   TBranch        *b_std_vector_lepton_phi;   //!
   //   TBranch        *b_std_vector_lepton_eleIdLoose;   //!
   //   TBranch        *b_std_vector_lepton_dz;   //!
   //   TBranch        *b_std_vector_lepton_eleIdVeto;   //!
   TBranch        *b_std_vector_lepton_isLooseLepton;   //!
   TBranch        *b_std_vector_lepton_pt;   //!
   TBranch        *b_std_vector_lepton_sumPUPt;   //!
   TBranch        *b_std_vector_lepton_eta;   //!
   TBranch        *b_std_vector_lepton_isMediumMuon;   //!
   //   TBranch        *b_std_vector_lepton_PfIsoDeltaBeta;   //!
   //   TBranch        *b_std_vector_lepton_closejet_phi;   //!
   TBranch        *b_std_vector_lepton_trackIso;   //!
   //   TBranch        *b_std_vector_lepton_isWgsLepton;   //!
   //   TBranch        *b_std_vector_lepton_closejet_eta;   //!
   //   TBranch        *b_std_vector_lepton_SIP3D;   //!
   TBranch        *b_std_vector_lepton_isTightLepton;   //!
   TBranch        *b_std_vector_lepton_ch;   //!
   TBranch        *b_std_vector_lepton_flavour;   //!
   //   TBranch        *b_std_vector_lepton_closejet_drlj;   //!
   TBranch        *b_std_vector_lepton_eleIdTight;   //!
   TBranch        *b_std_vector_lepton_d0;   //!
   TBranch        *b_std_vector_lepton_genmatched;   //!
   TBranch        *b_std_vector_lepton_promptgenmatched;   //!
   TBranch        *b_std_vector_lepton_isTightMuon;   //!
   TBranch        *b_susyMstop;   //!
   TBranch        *b_susyMLSP;   //!
   TBranch        *b_metPfType1Phi;   //!
   TBranch        *b_metPfType1;   //!
   TBranch        *b_bPogSF;   //!
   TBranch        *b_bPogSFUp;   //!
   TBranch        *b_bPogSFDown;   //!
   TBranch        *b_bPogSF_CMVAL;   //!
   TBranch        *b_bPogSF_CMVAL_up;   //!
   TBranch        *b_bPogSF_CMVAL_down;   //!
   TBranch        *b_bPogSF_CMVAM;   //!
   TBranch        *b_bPogSF_CMVAM_up;   //!
   TBranch        *b_bPogSF_CMVAM_down;   //!
   TBranch        *b_bPogSF_CMVAT;   //!
   TBranch        *b_bPogSF_CMVAT_up;   //!
   TBranch        *b_bPogSF_CMVAT_down;   //!
   TBranch        *b_bPogSF_CSVL;   //!
   TBranch        *b_bPogSF_CSVL_up;   //!
   TBranch        *b_bPogSF_CSVL_down;   //!
   TBranch        *b_bPogSF_CSVM;   //!
   TBranch        *b_bPogSF_CSVM_up;   //!
   TBranch        *b_bPogSF_CSVM_down;   //!
   TBranch        *b_bPogSF_CSVT;   //!
   TBranch        *b_bPogSF_CSVT_up;   //!
   TBranch        *b_bPogSF_CSVT_down;   //!
   //   TBranch        *b_bTPSF;   //!
   //   TBranch        *b_bTPSFUp;   //!
   //   TBranch        *b_bTPSFDown;   //!
   //   TBranch        *b_bPogSF1Jet;   //!
   //   TBranch        *b_bPogSF1JetUp;   //!
   //   TBranch        *b_bPogSF1JetDown;   //!
   //   TBranch        *b_bTPSF1Jet;   //!
   //   TBranch        *b_bTPSF1JetUp;   //!
   //   TBranch        *b_bTPSF1JetDown;   //!
   //   TBranch        *b_bPogSF2Jet;   //!
   //   TBranch        *b_bPogSF2JetUp;   //!
   //   TBranch        *b_bPogSF2JetDown;   //!
   //   TBranch        *b_bTPSF2Jet;   //!
   //   TBranch        *b_bTPSF2JetUp;   //!
   //   TBranch        *b_bTPSF2JetDown;   //!
   TBranch        *b_effTrigW_Down;   //!
   TBranch        *b_effTrigW_Up;   //!
   TBranch        *b_effTrigW;   //!
   TBranch        *b_std_vector_lepton_idisoWcut_WP_Tight80X;   //!
   TBranch        *b_std_vector_lepton_idisoWcut_WP_Tight80X_Up;   //!
   TBranch        *b_std_vector_lepton_idisoWcut_WP_Tight80X_Down;   //!
   TBranch        *b_std_vector_lepton_idisoW;   //!
   TBranch        *b_std_vector_lepton_idisoW_Up;   //!
   TBranch        *b_std_vector_lepton_idisoW_Down;   //!
   //   TBranch        *b_std_vector_lepton_idisoW_Syst;   //!
   //   TBranch        *b_std_vector_lepton_idisoLooseW;   //!
   //   TBranch        *b_std_vector_lepton_idisoLooseW_Up;   //!
   //   TBranch        *b_std_vector_lepton_idisoLooseW_Down;   //!
   //   TBranch        *b_std_vector_lepton_idisoLooseW_Syst;   //!
   TBranch        *b_std_vector_lepton_recoW;   //!
   TBranch        *b_std_vector_lepton_recoW_Up;   //!
   TBranch        *b_std_vector_lepton_recoW_Down;   //!
   TBranch        *b_std_vector_lepton_fastsimW;   //!
   TBranch        *b_std_vector_lepton_fastsimW_Up;   //!
   TBranch        *b_std_vector_lepton_fastsimW_Down;   //!
   //   TBranch        *b_mcoll;   //!
   //   TBranch        *b_mcollWW;   //!
   //   TBranch        *b_vht_pt;   //!
   //   TBranch        *b_mjj;   //!
   //   TBranch        *b_mllThird;   //!
   TBranch        *b_dphilljetjet;   //!
   //   TBranch        *b_vht_phi;   //!
   TBranch        *b_drll;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_dphilljet;   //!
   TBranch        *b_dphilmet1;   //!
   TBranch        *b_dphilmet2;   //!
   //   TBranch        *b_projpfmet;   //!
   //   TBranch        *b_mTi;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_dphill;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_detajj;   //!
   TBranch        *b_ptll;   //!
   TBranch        *b_mll;   //!
   TBranch        *b_dphilmet;   //!
   TBranch        *b_mth;   //!
   TBranch        *b_yll;   //!
   TBranch        *b_mtw2;   //!
   TBranch        *b_mtw1;   //!
   //   TBranch        *b_isJsonOk;   //!


   // 3lepton variables
   TBranch        *b_njet_3l;   //!
   TBranch        *b_flagOSSF;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_eta3;   //!
   TBranch        *b_zveto_3l;   //!
   TBranch        *b_pfmet;   //!
   TBranch        *b_chlll;   //!
   TBranch        *b_drllmin3l;   //!
   TBranch        *b_mlll;   //!
   TBranch        *b_mllmin3l;   //!
   TBranch        *b_phi3;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_pt3;   //!
   TBranch        *b_nbjet_3l;   //!


   // Full2016_Apr17 electron working points
   TBranch        *b_LepCut2l__ele_cut_WP_Tight80X__mu_cut_Tight80x;   //!
   TBranch        *b_LepSF2l__ele_cut_WP_Tight80X__mu_cut_Tight80x;   //
   TBranch        *b_LepSF2l__mu_cut_Tight80x__Do;   //
   TBranch        *b_LepSF2l__mu_cut_Tight80x__Up;   //
   TBranch        *b_LepSF2l__ele_cut_WP_Tight80X__Do;   //
   TBranch        *b_LepSF2l__ele_cut_WP_Tight80X__Up;   //
   TBranch        *b_METFilter_DATA;   //!
   TBranch        *b_METFilter_MC;   //!

   TBranch        *b_std_vector_muon_isTightLepton_cut_Tight80x;   //!
   TBranch        *b_std_vector_muon_idisoW_cut_Tight80x;   //!
   TBranch        *b_std_vector_muon_idisoW_cut_Tight80x_Down;   //!
   TBranch        *b_std_vector_muon_idisoW_cut_Tight80x_Up;   //!

   TBranch        *b_std_vector_electron_isTightLepton_cut_WP_Tight80X;   //!
   TBranch        *b_std_vector_electron_isTightLepton_cut_WP_Tight80X_SS;   //!
   TBranch        *b_std_vector_electron_isTightLepton_mva_80p_Iso2015;   //!
   TBranch        *b_std_vector_electron_isTightLepton_mva_80p_Iso2016;   //!
   TBranch        *b_std_vector_electron_isTightLepton_mva_90p_Iso2015;   //!
   TBranch        *b_std_vector_electron_isTightLepton_mva_90p_Iso2016;   //!

   TBranch        *b_std_vector_electron_idisoW_cut_WP_Tight80X;   //!
   TBranch        *b_std_vector_electron_idisoW_cut_WP_Tight80X_Down;   //!
   TBranch        *b_std_vector_electron_idisoW_cut_WP_Tight80X_Up;   //!

   TBranch        *b_std_vector_electron_idisoW_cut_WP_Tight80X_SS;   //!
   TBranch        *b_std_vector_electron_idisoW_mva_80p_Iso2015;   //!
   TBranch        *b_std_vector_electron_idisoW_mva_80p_Iso2016;   //!
   TBranch        *b_std_vector_electron_idisoW_mva_90p_Iso2015;   //!
   TBranch        *b_std_vector_electron_idisoW_mva_90p_Iso2016;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox_76x/22Jan_25ns_mAODv2_MC__l2loose__hadd__bSFL2pTEff__l2tight/latino_WWTo2L2Nu.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox_76x/22Jan_25ns_mAODv2_MC__l2loose__hadd__bSFL2pTEff__l2tight/latino_WWTo2L2Nu.root");
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
  //   std_vector_GEN_weight = 0;
   std_vector_LHE_weight = 0;
   //   std_vector_LHElepton_eta = 0;
   //   std_vector_LHElepton_id = 0;
   //   std_vector_LHElepton_phi = 0;
   //   std_vector_LHElepton_pt = 0;
   //   std_vector_LHEneutrino_eta = 0;
   //   std_vector_LHEneutrino_id = 0;
   //   std_vector_LHEneutrino_phi = 0;
   //   std_vector_LHEneutrino_pt = 0;
   //   std_vector_LHEparton_eta = 0;
   //   std_vector_LHEparton_id = 0;
   //   std_vector_LHEparton_phi = 0;
   std_vector_LHEparton_pt = 0;
   std_vector_VBoson_eta = 0;
   std_vector_VBoson_mass = 0;
   std_vector_VBoson_phi = 0;
   std_vector_VBoson_pid = 0;
   std_vector_VBoson_pt = 0;
   //   std_vector_dressedLeptonGen_eta = 0;
   //   std_vector_dressedLeptonGen_phi = 0;
   //   std_vector_dressedLeptonGen_pid = 0;
   //   std_vector_dressedLeptonGen_pt = 0;
   std_vector_jetGen_eta = 0;
   std_vector_jetGen_phi = 0;
   std_vector_jetGen_pt = 0;
   std_vector_leptonGen_eta = 0;
   //   std_vector_leptonGen_fromHardProcess = 0;
   //   std_vector_leptonGen_isDirectHadronDecayProduct = 0;
   std_vector_leptonGen_isDirectPromptTauDecayProduct = 0;
   std_vector_leptonGen_isPrompt = 0;
   //   std_vector_leptonGen_isTauDecayProduct = 0;
   std_vector_leptonGen_phi = 0;
   std_vector_leptonGen_pid = 0;
   std_vector_leptonGen_pt = 0;
   std_vector_leptonGen_status = 0;
   std_vector_leptonGen_MotherPID = 0; 
   std_vector_DarkMatterGen_pt = 0;
   std_vector_neutrinoGen_eta = 0;
   std_vector_neutrinoGen_isDirectPromptTauDecayProduct = 0;
   std_vector_neutrinoGen_isPrompt = 0;
   std_vector_neutrinoGen_phi = 0;
   std_vector_neutrinoGen_pid = 0;
   std_vector_neutrinoGen_MotherPID = 0;
   std_vector_neutrinoGen_pt = 0;
   std_vector_partonGen_eta = 0;
   std_vector_partonGen_isHardProcess = 0;
   //   std_vector_partonGen_isPrompt = 0;
   std_vector_partonGen_phi = 0;
   std_vector_partonGen_pid = 0;
   std_vector_partonGen_pt = 0;
   //   std_vector_photon_eta = 0;
   //   std_vector_photon_phi = 0;
   //   std_vector_photon_pt = 0;
   //   std_vector_photonid_eta = 0;
   //   std_vector_photonid_phi = 0;
   //   std_vector_photonid_pt = 0;
   std_vector_softMuD0 = 0;
   std_vector_softMuDz = 0;
   //   std_vector_softMuEta = 0;
   std_vector_softMuIsTrackerMuon = 0;
   std_vector_softMuIso = 0;
   //   std_vector_softMuPhi = 0;
   std_vector_softMuPt = 0;
   std_vector_softMuTMLastStationAngTight = 0;
   //   std_vector_tau_eta = 0;
   //   std_vector_tau_phi = 0;
   //   std_vector_tau_pt = 0;
   //   std_vector_trackjet_eta = 0;
   //   std_vector_trackjet_phi = 0;
   //   std_vector_trackjet_probabilityB = 0;
   //   std_vector_trackjet_pt = 0;
   std_vector_trigger_special = 0;
   std_vector_trigger = 0;
   //   std_vector_electron_ooEmooP = 0;
   //   std_vector_jet_puid = 0;
   //   std_vector_muon_NTkLayers = 0;
   std_vector_jet_cmvav2 = 0;
   //   std_vector_muon_Chi2LocalPos = 0;
   std_vector_jet_phi = 0;
   std_vector_jet_mass = 0;
   std_vector_jet_chargedHadronFraction = 0;
   //   std_vector_muon_NValidPixelHitsInTrk = 0;
   //   std_vector_electron_passConversionVeto = 0;
   //   std_vector_puppijet_eta = 0;
   //   std_vector_muon_NValidHitsSATrk = 0;
   //   std_vector_electron_ecalPFClusterIso = 0;
   //   std_vector_jet_tche = 0;
   //   std_vector_muon_NValidHitsInTrk = 0;
   std_vector_jet_csvv2ivf = 0;
   //   std_vector_jet_tchp = 0;
   //   std_vector_electron_dEtaIn = 0;
   //   std_vector_muon_TrkKink = 0;
   //   std_vector_muon_NormChi2GTrk = 0;
   //   std_vector_electron_full5x5_sigmaIetaIeta = 0;
   //   std_vector_jet_ssvhe = 0;
   //   std_vector_puppijet_phi = 0;
   std_vector_jet_eta = 0;
   std_vector_jet_pt = 0;
   std_vector_jet_isFromISR = 0;
   //   std_vector_muon_NValidFractInTrk = 0;
   //   std_vector_jet_bjp = 0;
   //   std_vector_puppijet_pt = 0;
   //   std_vector_muon_SegCompatibilty = 0;
   std_vector_electron_effectiveArea = 0;
   std_vector_jet_PartonFlavour = 0;
   //   std_vector_jet_softMuDz = 0;
   //   std_vector_jet_softMuIso = 0;
   //   std_vector_jet_softMuEta = 0;
   //   std_vector_jet_softMuPt = 0;
   //   std_vector_jet_ssvhb = 0;
   //   std_vector_jet_softMuD0 = 0;
   //   std_vector_jet_QGlikelihood = 0;
   //   std_vector_muon_NumOfMatchedStations = 0;
   //   std_vector_jet_softMuPhi = 0;
   //   std_vector_electron_hcalPFClusterIso = 0;
   //   std_vector_jet_bjpb = 0;
   //   std_vector_jet_pfcsv = 0;
   //   std_vector_electron_dPhiIn = 0;
   //   std_vector_electron_hOverE = 0;
   //   std_vector_jet_NumberSoftMu = 0;
   std_vector_jet_HadronFlavour = 0;
   //   std_vector_electron_scEta = 0;
   //   std_vector_electron_expectedMissingInnerHits = 0;
   std_vector_lepton_chargedHadronIso = 0;
   std_vector_lepton_photonIso = 0;
   //   std_vector_lepton_closejet_PartonFlavour = 0;
   //   std_vector_lepton_closejet_pt = 0;
   std_vector_lepton_neutralHadronIso = 0;
   std_vector_lepton_eleIdMedium = 0;
   //   std_vector_lepton_chargedParticleIso = 0;
   std_vector_lepton_phi = 0;
   //   std_vector_lepton_eleIdLoose = 0;
   //   std_vector_lepton_dz = 0;
   //   std_vector_lepton_eleIdVeto = 0;
   std_vector_lepton_isLooseLepton = 0;
   std_vector_lepton_pt = 0;
   std_vector_lepton_sumPUPt = 0;
   std_vector_lepton_eta = 0;
   std_vector_lepton_isMediumMuon = 0;
   //   std_vector_lepton_PfIsoDeltaBeta = 0;
   //   std_vector_lepton_closejet_phi = 0;
   std_vector_lepton_trackIso = 0;
   //   std_vector_lepton_isWgsLepton = 0;
   //   std_vector_lepton_closejet_eta = 0;
   //   std_vector_lepton_SIP3D = 0;
   std_vector_lepton_isTightLepton = 0;
   std_vector_lepton_ch = 0;
   std_vector_lepton_flavour = 0;
   //   std_vector_lepton_closejet_drlj = 0;
   std_vector_lepton_eleIdTight = 0;
   std_vector_lepton_d0 = 0;
   std_vector_lepton_genmatched = 0;
   std_vector_lepton_promptgenmatched = 0;
   std_vector_lepton_isTightMuon = 0;
   std_vector_lepton_idisoWcut_WP_Tight80X = 0;
   std_vector_lepton_idisoWcut_WP_Tight80X_Up = 0;
   std_vector_lepton_idisoWcut_WP_Tight80X_Down = 0;
   std_vector_lepton_idisoW = 0;
   std_vector_lepton_idisoW_Up = 0;
   std_vector_lepton_idisoW_Down = 0;
   //   std_vector_lepton_idisoW_Syst = 0;
   //   std_vector_lepton_idisoLooseW = 0;
   //   std_vector_lepton_idisoLooseW_Up = 0;
   //   std_vector_lepton_idisoLooseW_Down = 0;
   //   std_vector_lepton_idisoLooseW_Syst = 0;
   std_vector_lepton_recoW = 0;
   std_vector_lepton_recoW_Up = 0;
   std_vector_lepton_recoW_Down = 0;
   std_vector_lepton_fastsimW = 0;
   std_vector_lepton_fastsimW_Up = 0;
   std_vector_lepton_fastsimW_Down = 0;


   // Full2016_Apr17 electron working points
   std_vector_muon_isTightLepton_cut_Tight80x = 0;
   std_vector_muon_idisoW_cut_Tight80x = 0;
   std_vector_muon_idisoW_cut_Tight80x_Down = 0;
   std_vector_muon_idisoW_cut_Tight80x_Up = 0;

   std_vector_electron_isTightLepton_cut_WP_Tight80X = 0;
   std_vector_electron_isTightLepton_cut_WP_Tight80X_SS = 0;
   std_vector_electron_isTightLepton_mva_80p_Iso2015 = 0;
   std_vector_electron_isTightLepton_mva_80p_Iso2016 = 0;
   std_vector_electron_isTightLepton_mva_90p_Iso2015 = 0;
   std_vector_electron_isTightLepton_mva_90p_Iso2016 = 0;

   std_vector_electron_idisoW_cut_WP_Tight80X = 0;
   std_vector_electron_idisoW_cut_WP_Tight80X_Down = 0;
   std_vector_electron_idisoW_cut_WP_Tight80X_Up = 0;

   std_vector_electron_idisoW_cut_WP_Tight80X_SS = 0;
   std_vector_electron_idisoW_mva_80p_Iso2015 = 0;
   std_vector_electron_idisoW_mva_80p_Iso2016 = 0;
   std_vector_electron_idisoW_mva_90p_Iso2015 = 0;
   std_vector_electron_idisoW_mva_90p_Iso2016 = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("GEN_weight_SM", &GEN_weight_SM, &b_GEN_weight_SM);
   //   fChain->SetBranchAddress("Gen_ZGstar_deltaR", &Gen_ZGstar_deltaR, &b_Gen_ZGstar_deltaR);
   fChain->SetBranchAddress("Gen_ZGstar_mass", &Gen_ZGstar_mass, &b_Gen_ZGstar_mass);
   fChain->SetBranchAddress("Gen_ZGstar_MomId", &Gen_ZGstar_MomId, &b_Gen_ZGstar_MomId);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu1_eta", &Gen_ZGstar_mu1_eta, &b_Gen_ZGstar_mu1_eta);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu1_phi", &Gen_ZGstar_mu1_phi, &b_Gen_ZGstar_mu1_phi);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu1_pt", &Gen_ZGstar_mu1_pt, &b_Gen_ZGstar_mu1_pt);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu2_eta", &Gen_ZGstar_mu2_eta, &b_Gen_ZGstar_mu2_eta);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu2_phi", &Gen_ZGstar_mu2_phi, &b_Gen_ZGstar_mu2_phi);
   //   fChain->SetBranchAddress("Gen_ZGstar_mu2_pt", &Gen_ZGstar_mu2_pt, &b_Gen_ZGstar_mu2_pt);
   fChain->SetBranchAddress("topLHEpt",     &topLHEpt,     &b_topLHEpt);
   fChain->SetBranchAddress("antitopLHEpt", &antitopLHEpt, &b_antitopLHEpt);
   //   fChain->SetBranchAddress("LHE_weight_SM", &LHE_weight_SM, &b_LHE_weight_SM);
   //   fChain->SetBranchAddress("chSumEt", &chSumEt, &b_chSumEt);
   //   fChain->SetBranchAddress("chmet", &chmet, &b_chmet);
   //   fChain->SetBranchAddress("chmetphi", &chmetphi, &b_chmetphi);
   //   fChain->SetBranchAddress("dataset", &dataset, &b_dataset);
   //   fChain->SetBranchAddress("effW", &effW, &b_effW);
   //   fChain->SetBranchAddress("fakeW", &fakeW, &b_fakeW);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleUp", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleUp, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleUp);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleDown", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleDown, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_EleDown);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleUp", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleUp, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleUp);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleDown", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleDown, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statEleDown);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuUp", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuUp, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuUp);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuDown", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuDown, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_MuDown);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuUp", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuUp, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuUp);
   fChain->SetBranchAddress("fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuDown", &fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuDown, &b_fakeW2l_ele_cut_WP_Tight80X_mu_cut_Tight80x_statMuDown);
   fChain->SetBranchAddress("fakeW2l0j", &fakeW2l0j, &b_fakeW2l0j);
   fChain->SetBranchAddress("fakeW2l0jElUp", &fakeW2l0jElUp, &b_fakeW2l0jElUp);
   fChain->SetBranchAddress("fakeW2l0jElDown", &fakeW2l0jElDown, &b_fakeW2l0jElDown);
   fChain->SetBranchAddress("fakeW2l0jstatElUp", &fakeW2l0jstatElUp, &b_fakeW2l0jstatElUp);
   fChain->SetBranchAddress("fakeW2l0jstatElDown", &fakeW2l0jstatElDown, &b_fakeW2l0jstatElDown);
   fChain->SetBranchAddress("fakeW2l0jMuUp", &fakeW2l0jMuUp, &b_fakeW2l0jMuUp);
   fChain->SetBranchAddress("fakeW2l0jMuDown", &fakeW2l0jMuDown, &b_fakeW2l0jMuDown);
   fChain->SetBranchAddress("fakeW2l0jstatMuUp", &fakeW2l0jstatMuUp, &b_fakeW2l0jstatMuUp);
   fChain->SetBranchAddress("fakeW2l0jstatMuDown", &fakeW2l0jstatMuDown, &b_fakeW2l0jstatMuDown);
   fChain->SetBranchAddress("fakeW2l1j", &fakeW2l1j, &b_fakeW2l1j);
   fChain->SetBranchAddress("fakeW2l1jElUp", &fakeW2l1jElUp, &b_fakeW2l1jElUp);
   fChain->SetBranchAddress("fakeW2l1jElDown", &fakeW2l1jElDown, &b_fakeW2l1jElDown);
   fChain->SetBranchAddress("fakeW2l1jstatElUp", &fakeW2l1jstatElUp, &b_fakeW2l1jstatElUp);
   fChain->SetBranchAddress("fakeW2l1jstatElDown", &fakeW2l1jstatElDown, &b_fakeW2l1jstatElDown);
   fChain->SetBranchAddress("fakeW2l1jMuUp", &fakeW2l1jMuUp, &b_fakeW2l1jMuUp);
   fChain->SetBranchAddress("fakeW2l1jMuDown", &fakeW2l1jMuDown, &b_fakeW2l1jMuDown);
   fChain->SetBranchAddress("fakeW2l1jstatMuUp", &fakeW2l1jstatMuUp, &b_fakeW2l1jstatMuUp);
   fChain->SetBranchAddress("fakeW2l1jstatMuDown", &fakeW2l1jstatMuDown, &b_fakeW2l1jstatMuDown);
   fChain->SetBranchAddress("fakeW2l2j", &fakeW2l2j, &b_fakeW2l2j);
   fChain->SetBranchAddress("fakeW2l2jElUp", &fakeW2l2jElUp, &b_fakeW2l2jElUp);
   fChain->SetBranchAddress("fakeW2l2jElDown", &fakeW2l2jElDown, &b_fakeW2l2jElDown);
   fChain->SetBranchAddress("fakeW2l2jstatElUp", &fakeW2l2jstatElUp, &b_fakeW2l2jstatElUp);
   fChain->SetBranchAddress("fakeW2l2jstatElDown", &fakeW2l2jstatElDown, &b_fakeW2l2jstatElDown);
   fChain->SetBranchAddress("fakeW2l2jMuUp", &fakeW2l2jMuUp, &b_fakeW2l2jMuUp);
   fChain->SetBranchAddress("fakeW2l2jMuDown", &fakeW2l2jMuDown, &b_fakeW2l2jMuDown);
   fChain->SetBranchAddress("fakeW2l2jstatMuUp", &fakeW2l2jstatMuUp, &b_fakeW2l2jstatMuUp);
   fChain->SetBranchAddress("fakeW2l2jstatMuDown", &fakeW2l2jstatMuDown, &b_fakeW2l2jstatMuDown);
   fChain->SetBranchAddress("fakeW3l", &fakeW3l, &b_fakeW3l);
   fChain->SetBranchAddress("fakeW3lElUp", &fakeW3lElUp, &b_fakeW3lElUp);
   fChain->SetBranchAddress("fakeW3lElDown", &fakeW3lElDown, &b_fakeW3lElDown);
   fChain->SetBranchAddress("fakeW3lstatElUp", &fakeW3lstatElUp, &b_fakeW3lstatElUp);
   fChain->SetBranchAddress("fakeW3lstatElDown", &fakeW3lstatElDown, &b_fakeW3lstatElDown);
   fChain->SetBranchAddress("fakeW3lMuUp", &fakeW3lMuUp, &b_fakeW3lMuUp);
   fChain->SetBranchAddress("fakeW3lMuDown", &fakeW3lMuDown, &b_fakeW3lMuDown);
   fChain->SetBranchAddress("fakeW3lstatMuUp", &fakeW3lstatMuUp, &b_fakeW3lstatMuUp);
   fChain->SetBranchAddress("fakeW3lstatMuDown", &fakeW3lstatMuDown, &b_fakeW3lstatMuDown);
   //   fChain->SetBranchAddress("hardbjpb", &hardbjpb, &b_hardbjpb);
   //   fChain->SetBranchAddress("hardcmvav2", &hardcmvav2, &b_hardcmvav2);
   //   fChain->SetBranchAddress("hardcsvv2ivf", &hardcsvv2ivf, &b_hardcsvv2ivf);
   //   fChain->SetBranchAddress("hardpfcsv", &hardpfcsv, &b_hardpfcsv);
   //   fChain->SetBranchAddress("hardssvhb", &hardssvhb, &b_hardssvhb);
   //   fChain->SetBranchAddress("hardssvhe", &hardssvhe, &b_hardssvhe);
   //   fChain->SetBranchAddress("hardtche", &hardtche, &b_hardtche);
   //   fChain->SetBranchAddress("higgsGeneta", &higgsGeneta, &b_higgsGeneta);
   //   fChain->SetBranchAddress("higgsGenmass", &higgsGenmass, &b_higgsGenmass);
   //   fChain->SetBranchAddress("higgsGenphi", &higgsGenphi, &b_higgsGenphi);
   //   fChain->SetBranchAddress("higgsGenpt", &higgsGenpt, &b_higgsGenpt);
   //   fChain->SetBranchAddress("higgsLHEeta", &higgsLHEeta, &b_higgsLHEeta);
   //   fChain->SetBranchAddress("higgsLHEmass", &higgsLHEmass, &b_higgsLHEmass);
   //   fChain->SetBranchAddress("higgsLHEphi", &higgsLHEphi, &b_higgsLHEphi);
   //   fChain->SetBranchAddress("higgsLHEpt", &higgsLHEpt, &b_higgsLHEpt);
   //   fChain->SetBranchAddress("imet", &imet, &b_imet);
   fChain->SetBranchAddress("jetRho", &jetRho, &b_jetRho);
   //   fChain->SetBranchAddress("jetcmvav21", &jetcmvav21, &b_jetcmvav21);
   //   fChain->SetBranchAddress("jetcmvav22", &jetcmvav22, &b_jetcmvav22);
   //   fChain->SetBranchAddress("jetcsvv2ivf1", &jetcsvv2ivf1, &b_jetcsvv2ivf1);
   //   fChain->SetBranchAddress("jetcsvv2ivf2", &jetcsvv2ivf2, &b_jetcsvv2ivf2);
   //   fChain->SetBranchAddress("jetpfcsv1", &jetpfcsv1, &b_jetpfcsv1);
   //   fChain->SetBranchAddress("jetpfcsv2", &jetpfcsv2, &b_jetpfcsv2);
   //   fChain->SetBranchAddress("jetssvhb1", &jetssvhb1, &b_jetssvhb1);
   //   fChain->SetBranchAddress("jetssvhb2", &jetssvhb2, &b_jetssvhb2);
   //   fChain->SetBranchAddress("jetssvhe1", &jetssvhe1, &b_jetssvhe1);
   //   fChain->SetBranchAddress("jetssvhe2", &jetssvhe2, &b_jetssvhe2);
   //   fChain->SetBranchAddress("kfW", &kfW, &b_kfW);
   //   fChain->SetBranchAddress("mctruth", &mctruth, &b_mctruth);
   fChain->SetBranchAddress("metGeneta", &metGeneta, &b_metGeneta);
   fChain->SetBranchAddress("metGenphi", &metGenphi, &b_metGenphi);
   fChain->SetBranchAddress("metGenpt", &metGenpt, &b_metGenpt);
   //   fChain->SetBranchAddress("metLHEeta", &metLHEeta, &b_metLHEeta);
   //   fChain->SetBranchAddress("metLHEphi", &metLHEphi, &b_metLHEphi);
   //   fChain->SetBranchAddress("metLHEpt", &metLHEpt, &b_metLHEpt);
   //   fChain->SetBranchAddress("metPfNoHf", &metPfNoHf, &b_metPfNoHf);
   //   fChain->SetBranchAddress("metPfProj", &metPfProj, &b_metPfProj);
   //   fChain->SetBranchAddress("metPfProjElecEnDn", &metPfProjElecEnDn, &b_metPfProjElecEnDn);
   //   fChain->SetBranchAddress("metPfProjElecEnUp", &metPfProjElecEnUp, &b_metPfProjElecEnUp);
   //   fChain->SetBranchAddress("metPfProjJetEnDn", &metPfProjJetEnDn, &b_metPfProjJetEnDn);
   //   fChain->SetBranchAddress("metPfProjJetEnUp", &metPfProjJetEnUp, &b_metPfProjJetEnUp);
   //   fChain->SetBranchAddress("metPfProjJetResDn", &metPfProjJetResDn, &b_metPfProjJetResDn);
   //   fChain->SetBranchAddress("metPfProjJetResUp", &metPfProjJetResUp, &b_metPfProjJetResUp);
   //   fChain->SetBranchAddress("metPfProjMuonEnDn", &metPfProjMuonEnDn, &b_metPfProjMuonEnDn);
   //   fChain->SetBranchAddress("metPfProjMuonEnUp", &metPfProjMuonEnUp, &b_metPfProjMuonEnUp);
   //   fChain->SetBranchAddress("metPfProjUnclEnDn", &metPfProjUnclEnDn, &b_metPfProjUnclEnDn);
   //   fChain->SetBranchAddress("metPfProjUnclEnUp", &metPfProjUnclEnUp, &b_metPfProjUnclEnUp);
   //   fChain->SetBranchAddress("metPfRaw", &metPfRaw, &b_metPfRaw);
   //   fChain->SetBranchAddress("metPfRawElecEnDn", &metPfRawElecEnDn, &b_metPfRawElecEnDn);
   //   fChain->SetBranchAddress("metPfRawElecEnUp", &metPfRawElecEnUp, &b_metPfRawElecEnUp);
   //   fChain->SetBranchAddress("metPfRawJetEnDn", &metPfRawJetEnDn, &b_metPfRawJetEnDn);
   //   fChain->SetBranchAddress("metPfRawJetEnUp", &metPfRawJetEnUp, &b_metPfRawJetEnUp);
   //   fChain->SetBranchAddress("metPfRawJetResDn", &metPfRawJetResDn, &b_metPfRawJetResDn);
   //   fChain->SetBranchAddress("metPfRawJetResUp", &metPfRawJetResUp, &b_metPfRawJetResUp);
   //   fChain->SetBranchAddress("metPfRawMuonEnDn", &metPfRawMuonEnDn, &b_metPfRawMuonEnDn);
   //   fChain->SetBranchAddress("metPfRawMuonEnUp", &metPfRawMuonEnUp, &b_metPfRawMuonEnUp);
   //   fChain->SetBranchAddress("metPfRawPhi", &metPfRawPhi, &b_metPfRawPhi);
   //   fChain->SetBranchAddress("metPfRawPhiElecEnDn", &metPfRawPhiElecEnDn, &b_metPfRawPhiElecEnDn);
   //   fChain->SetBranchAddress("metPfRawPhiElecEnUp", &metPfRawPhiElecEnUp, &b_metPfRawPhiElecEnUp);
   //   fChain->SetBranchAddress("metPfRawPhiJetEnDn", &metPfRawPhiJetEnDn, &b_metPfRawPhiJetEnDn);
   //   fChain->SetBranchAddress("metPfRawPhiJetEnUp", &metPfRawPhiJetEnUp, &b_metPfRawPhiJetEnUp);
   //   fChain->SetBranchAddress("metPfRawPhiJetResDn", &metPfRawPhiJetResDn, &b_metPfRawPhiJetResDn);
   //   fChain->SetBranchAddress("metPfRawPhiJetResUp", &metPfRawPhiJetResUp, &b_metPfRawPhiJetResUp);
   //   fChain->SetBranchAddress("metPfRawPhiMuonEnDn", &metPfRawPhiMuonEnDn, &b_metPfRawPhiMuonEnDn);
   //   fChain->SetBranchAddress("metPfRawPhiMuonEnUp", &metPfRawPhiMuonEnUp, &b_metPfRawPhiMuonEnUp);
   //   fChain->SetBranchAddress("metPfRawPhiUnclEnDn", &metPfRawPhiUnclEnDn, &b_metPfRawPhiUnclEnDn);
   //   fChain->SetBranchAddress("metPfRawPhiUnclEnUp", &metPfRawPhiUnclEnUp, &b_metPfRawPhiUnclEnUp);
   //   fChain->SetBranchAddress("metPfRawSumEt", &metPfRawSumEt, &b_metPfRawSumEt);
   //   fChain->SetBranchAddress("metPfRawUnclEnDn", &metPfRawUnclEnDn, &b_metPfRawUnclEnDn);
   //   fChain->SetBranchAddress("metPfRawUnclEnUp", &metPfRawUnclEnUp, &b_metPfRawUnclEnUp);
   //   fChain->SetBranchAddress("metPfType1ElecEnDn", &metPfType1ElecEnDn, &b_metPfType1ElecEnDn);
   //   fChain->SetBranchAddress("metPfType1ElecEnUp", &metPfType1ElecEnUp, &b_metPfType1ElecEnUp);
   //   fChain->SetBranchAddress("metPfType1JetEnDn", &metPfType1JetEnDn, &b_metPfType1JetEnDn);
   //   fChain->SetBranchAddress("metPfType1JetEnUp", &metPfType1JetEnUp, &b_metPfType1JetEnUp);
   //   fChain->SetBranchAddress("metPfType1JetResDn", &metPfType1JetResDn, &b_metPfType1JetResDn);
   //   fChain->SetBranchAddress("metPfType1JetResUp", &metPfType1JetResUp, &b_metPfType1JetResUp);
   //   fChain->SetBranchAddress("metPfType1MuonEnDn", &metPfType1MuonEnDn, &b_metPfType1MuonEnDn);
   //   fChain->SetBranchAddress("metPfType1MuonEnUp", &metPfType1MuonEnUp, &b_metPfType1MuonEnUp);
   //   fChain->SetBranchAddress("metPfType1SumEt", &metPfType1SumEt, &b_metPfType1SumEt);
   //   fChain->SetBranchAddress("metPfType1UnclEnDn", &metPfType1UnclEnDn, &b_metPfType1UnclEnDn);
   //   fChain->SetBranchAddress("metPfType1UnclEnUp", &metPfType1UnclEnUp, &b_metPfType1UnclEnUp);
   fChain->SetBranchAddress("metPuppi", &metPuppi, &b_metPuppi);
   fChain->SetBranchAddress("metTtrk", &metTtrk, &b_metTtrk);
   fChain->SetBranchAddress("metTtrkPhi", &metTtrkPhi, &b_metTtrkPhi);
   //   fChain->SetBranchAddress("mllg", &mllg, &b_mllg);
   //   fChain->SetBranchAddress("mllgid", &mllgid, &b_mllgid);
   fChain->SetBranchAddress("mpmet", &mpmet, &b_mpmet);
   //   fChain->SetBranchAddress("nPhos", &nPhos, &b_nPhos);
   //   fChain->SetBranchAddress("nbjet", &nbjet, &b_nbjet);
   //   fChain->SetBranchAddress("nbjettche", &nbjettche, &b_nbjettche);
   //   fChain->SetBranchAddress("nextra", &nextra, &b_nextra);
   //   fChain->SetBranchAddress("njetid", &njetid, &b_njetid);
   //   fChain->SetBranchAddress("pchmet", &pchmet, &b_pchmet);
   //   fChain->SetBranchAddress("pdfid1", &pdfid1, &b_pdfid1);
   //   fChain->SetBranchAddress("pdfid2", &pdfid2, &b_pdfid2);
   //   fChain->SetBranchAddress("pdfscalePDF", &pdfscalePDF, &b_pdfscalePDF);
   //   fChain->SetBranchAddress("pdfx1", &pdfx1, &b_pdfx1);
   //   fChain->SetBranchAddress("pdfx1PDF", &pdfx1PDF, &b_pdfx1PDF);
   //   fChain->SetBranchAddress("pdfx2", &pdfx2, &b_pdfx2);
   //   fChain->SetBranchAddress("pdfx2PDF", &pdfx2PDF, &b_pdfx2PDF);
   //   fChain->SetBranchAddress("peaking", &peaking, &b_peaking);
   fChain->SetBranchAddress("pfmetMEtSig", &pfmetMEtSig, &b_pfmetMEtSig);
   fChain->SetBranchAddress("pfmetSignificance", &pfmetSignificance, &b_pfmetSignificance);
   //   fChain->SetBranchAddress("pho_HoE", &pho_HoE, &b_pho_HoE);
   //   fChain->SetBranchAddress("pho_chIso", &pho_chIso, &b_pho_chIso);
   //   fChain->SetBranchAddress("pho_hasPixelSeed", &pho_hasPixelSeed, &b_pho_hasPixelSeed);
   //   fChain->SetBranchAddress("pho_n_id", &pho_n_id, &b_pho_n_id);
   //   fChain->SetBranchAddress("pho_nhIso", &pho_nhIso, &b_pho_nhIso);
   //   fChain->SetBranchAddress("pho_passElecVeto", &pho_passElecVeto, &b_pho_passElecVeto);
   //   fChain->SetBranchAddress("pho_phIso", &pho_phIso, &b_pho_phIso);
   //   fChain->SetBranchAddress("pho_sietaieta", &pho_sietaieta, &b_pho_sietaieta);
   //   fChain->SetBranchAddress("predmet", &predmet, &b_predmet);
   //   fChain->SetBranchAddress("redmet", &redmet, &b_redmet);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   //   fChain->SetBranchAddress("softbjpb", &softbjpb, &b_softbjpb);
   //   fChain->SetBranchAddress("softcmvav2", &softcmvav2, &b_softcmvav2);
   //   fChain->SetBranchAddress("softcsvv2ivf", &softcsvv2ivf, &b_softcsvv2ivf);
   //   fChain->SetBranchAddress("softpfcsv", &softpfcsv, &b_softpfcsv);
   //   fChain->SetBranchAddress("softssvhb", &softssvhb, &b_softssvhb);
   //   fChain->SetBranchAddress("softssvhe", &softssvhe, &b_softssvhe);
   //   fChain->SetBranchAddress("softtche", &softtche, &b_softtche);
   //   fChain->SetBranchAddress("std_vector_GEN_weight", &std_vector_GEN_weight, &b_std_vector_GEN_weight);
   fChain->SetBranchAddress("std_vector_LHE_weight", &std_vector_LHE_weight, &b_std_vector_LHE_weight);
   //   fChain->SetBranchAddress("std_vector_LHElepton_eta", &std_vector_LHElepton_eta, &b_std_vector_LHElepton_eta);
   //   fChain->SetBranchAddress("std_vector_LHElepton_id", &std_vector_LHElepton_id, &b_std_vector_LHElepton_id);
   //   fChain->SetBranchAddress("std_vector_LHElepton_phi", &std_vector_LHElepton_phi, &b_std_vector_LHElepton_phi);
   //   fChain->SetBranchAddress("std_vector_LHElepton_pt", &std_vector_LHElepton_pt, &b_std_vector_LHElepton_pt);
   //   fChain->SetBranchAddress("std_vector_LHEneutrino_eta", &std_vector_LHEneutrino_eta, &b_std_vector_LHEneutrino_eta);
   //   fChain->SetBranchAddress("std_vector_LHEneutrino_id", &std_vector_LHEneutrino_id, &b_std_vector_LHEneutrino_id);
   //   fChain->SetBranchAddress("std_vector_LHEneutrino_phi", &std_vector_LHEneutrino_phi, &b_std_vector_LHEneutrino_phi);
   //   fChain->SetBranchAddress("std_vector_LHEneutrino_pt", &std_vector_LHEneutrino_pt, &b_std_vector_LHEneutrino_pt);
   //   fChain->SetBranchAddress("std_vector_LHEparton_eta", &std_vector_LHEparton_eta, &b_std_vector_LHEparton_eta);
   //   fChain->SetBranchAddress("std_vector_LHEparton_id", &std_vector_LHEparton_id, &b_std_vector_LHEparton_id);
   //   fChain->SetBranchAddress("std_vector_LHEparton_phi", &std_vector_LHEparton_phi, &b_std_vector_LHEparton_phi);
   fChain->SetBranchAddress("std_vector_LHEparton_pt", &std_vector_LHEparton_pt, &b_std_vector_LHEparton_pt);
   fChain->SetBranchAddress("std_vector_VBoson_eta", &std_vector_VBoson_eta, &b_std_vector_VBoson_eta);
   fChain->SetBranchAddress("std_vector_VBoson_mass", &std_vector_VBoson_mass, &b_std_vector_VBoson_mass);
   fChain->SetBranchAddress("std_vector_VBoson_phi", &std_vector_VBoson_phi, &b_std_vector_VBoson_phi);
   fChain->SetBranchAddress("std_vector_VBoson_pid", &std_vector_VBoson_pid, &b_std_vector_VBoson_pid);
   fChain->SetBranchAddress("std_vector_VBoson_pt", &std_vector_VBoson_pt, &b_std_vector_VBoson_pt);
   //   fChain->SetBranchAddress("std_vector_dressedLeptonGen_eta", &std_vector_dressedLeptonGen_eta, &b_std_vector_dressedLeptonGen_eta);
   //   fChain->SetBranchAddress("std_vector_dressedLeptonGen_phi", &std_vector_dressedLeptonGen_phi, &b_std_vector_dressedLeptonGen_phi);
   //   fChain->SetBranchAddress("std_vector_dressedLeptonGen_pid", &std_vector_dressedLeptonGen_pid, &b_std_vector_dressedLeptonGen_pid);
   //   fChain->SetBranchAddress("std_vector_dressedLeptonGen_pt", &std_vector_dressedLeptonGen_pt, &b_std_vector_dressedLeptonGen_pt);
   fChain->SetBranchAddress("std_vector_jetGen_eta", &std_vector_jetGen_eta, &b_std_vector_jetGen_eta);
   fChain->SetBranchAddress("std_vector_jetGen_phi", &std_vector_jetGen_phi, &b_std_vector_jetGen_phi);
   fChain->SetBranchAddress("std_vector_jetGen_pt", &std_vector_jetGen_pt, &b_std_vector_jetGen_pt);
   fChain->SetBranchAddress("std_vector_leptonGen_eta", &std_vector_leptonGen_eta, &b_std_vector_leptonGen_eta);
   //   fChain->SetBranchAddress("std_vector_leptonGen_fromHardProcess", &std_vector_leptonGen_fromHardProcess, &b_std_vector_leptonGen_fromHardProcess);
   //   fChain->SetBranchAddress("std_vector_leptonGen_isDirectHadronDecayProduct", &std_vector_leptonGen_isDirectHadronDecayProduct, &b_std_vector_leptonGen_isDirectHadronDecayProduct);
   fChain->SetBranchAddress("std_vector_leptonGen_isDirectPromptTauDecayProduct", &std_vector_leptonGen_isDirectPromptTauDecayProduct, &b_std_vector_leptonGen_isDirectPromptTauDecayProduct);
   fChain->SetBranchAddress("std_vector_leptonGen_isPrompt", &std_vector_leptonGen_isPrompt, &b_std_vector_leptonGen_isPrompt);
   //   fChain->SetBranchAddress("std_vector_leptonGen_isTauDecayProduct", &std_vector_leptonGen_isTauDecayProduct, &b_std_vector_leptonGen_isTauDecayProduct);
   fChain->SetBranchAddress("std_vector_leptonGen_phi", &std_vector_leptonGen_phi, &b_std_vector_leptonGen_phi);
   fChain->SetBranchAddress("std_vector_leptonGen_pid", &std_vector_leptonGen_pid, &b_std_vector_leptonGen_pid);
   fChain->SetBranchAddress("std_vector_leptonGen_pt", &std_vector_leptonGen_pt, &b_std_vector_leptonGen_pt);
   fChain->SetBranchAddress("std_vector_leptonGen_status", &std_vector_leptonGen_status, &b_std_vector_leptonGen_status);
   fChain->SetBranchAddress("std_vector_leptonGen_MotherPID", &std_vector_leptonGen_MotherPID, &b_std_vector_leptonGen_MotherPID);
   fChain->SetBranchAddress("std_vector_DarkMatterGen_pt", &std_vector_DarkMatterGen_pt, &b_std_vector_DarkMatterGen_pt);
   fChain->SetBranchAddress("std_vector_neutrinoGen_eta", &std_vector_neutrinoGen_eta, &b_std_vector_neutrinoGen_eta);
   fChain->SetBranchAddress("std_vector_neutrinoGen_isDirectPromptTauDecayProduct", &std_vector_neutrinoGen_isDirectPromptTauDecayProduct, &b_std_vector_neutrinoGen_isDirectPromptTauDecayProduct);
   fChain->SetBranchAddress("std_vector_neutrinoGen_isPrompt", &std_vector_neutrinoGen_isPrompt, &b_std_vector_neutrinoGen_isPrompt);
   fChain->SetBranchAddress("std_vector_neutrinoGen_phi", &std_vector_neutrinoGen_phi, &b_std_vector_neutrinoGen_phi);
   fChain->SetBranchAddress("std_vector_neutrinoGen_pid", &std_vector_neutrinoGen_pid, &b_std_vector_neutrinoGen_pid);
   fChain->SetBranchAddress("std_vector_neutrinoGen_MotherPID", &std_vector_neutrinoGen_MotherPID, &b_std_vector_neutrinoGen_MotherPID);
   fChain->SetBranchAddress("std_vector_neutrinoGen_pt", &std_vector_neutrinoGen_pt, &b_std_vector_neutrinoGen_pt);
   fChain->SetBranchAddress("std_vector_partonGen_eta", &std_vector_partonGen_eta, &b_std_vector_partonGen_eta);
   fChain->SetBranchAddress("std_vector_partonGen_isHardProcess", &std_vector_partonGen_isHardProcess, &b_std_vector_partonGen_isHardProcess);
   //   fChain->SetBranchAddress("std_vector_partonGen_isPrompt", &std_vector_partonGen_isPrompt, &b_std_vector_partonGen_isPrompt);
   fChain->SetBranchAddress("std_vector_partonGen_phi", &std_vector_partonGen_phi, &b_std_vector_partonGen_phi);
   fChain->SetBranchAddress("std_vector_partonGen_pid", &std_vector_partonGen_pid, &b_std_vector_partonGen_pid);
   fChain->SetBranchAddress("std_vector_partonGen_pt", &std_vector_partonGen_pt, &b_std_vector_partonGen_pt);
   //   fChain->SetBranchAddress("std_vector_photon_eta", &std_vector_photon_eta, &b_std_vector_photon_eta);
   //   fChain->SetBranchAddress("std_vector_photon_phi", &std_vector_photon_phi, &b_std_vector_photon_phi);
   //   fChain->SetBranchAddress("std_vector_photon_pt", &std_vector_photon_pt, &b_std_vector_photon_pt);
   //   fChain->SetBranchAddress("std_vector_photonid_eta", &std_vector_photonid_eta, &b_std_vector_photonid_eta);
   //   fChain->SetBranchAddress("std_vector_photonid_phi", &std_vector_photonid_phi, &b_std_vector_photonid_phi);
   //   fChain->SetBranchAddress("std_vector_photonid_pt", &std_vector_photonid_pt, &b_std_vector_photonid_pt);
   fChain->SetBranchAddress("std_vector_softMuD0", &std_vector_softMuD0, &b_std_vector_softMuD0);
   fChain->SetBranchAddress("std_vector_softMuDz", &std_vector_softMuDz, &b_std_vector_softMuDz);
   //   fChain->SetBranchAddress("std_vector_softMuEta", &std_vector_softMuEta, &b_std_vector_softMuEta);
   fChain->SetBranchAddress("std_vector_softMuIsTrackerMuon", &std_vector_softMuIsTrackerMuon, &b_std_vector_softMuIsTrackerMuon);
   fChain->SetBranchAddress("std_vector_softMuIso", &std_vector_softMuIso, &b_std_vector_softMuIso);
   //   fChain->SetBranchAddress("std_vector_softMuPhi", &std_vector_softMuPhi, &b_std_vector_softMuPhi);
   fChain->SetBranchAddress("std_vector_softMuPt", &std_vector_softMuPt, &b_std_vector_softMuPt);
   fChain->SetBranchAddress("std_vector_softMuTMLastStationAngTight", &std_vector_softMuTMLastStationAngTight, &b_std_vector_softMuTMLastStationAngTight);
   //   fChain->SetBranchAddress("std_vector_tau_eta", &std_vector_tau_eta, &b_std_vector_tau_eta);
   //   fChain->SetBranchAddress("std_vector_tau_phi", &std_vector_tau_phi, &b_std_vector_tau_phi);
   //   fChain->SetBranchAddress("std_vector_tau_pt", &std_vector_tau_pt, &b_std_vector_tau_pt);
   //   fChain->SetBranchAddress("std_vector_trackjet_eta", &std_vector_trackjet_eta, &b_std_vector_trackjet_eta);
   //   fChain->SetBranchAddress("std_vector_trackjet_phi", &std_vector_trackjet_phi, &b_std_vector_trackjet_phi);
   //   fChain->SetBranchAddress("std_vector_trackjet_probabilityB", &std_vector_trackjet_probabilityB, &b_std_vector_trackjet_probabilityB);
   //   fChain->SetBranchAddress("std_vector_trackjet_pt", &std_vector_trackjet_pt, &b_std_vector_trackjet_pt);
   fChain->SetBranchAddress("std_vector_trigger_special", &std_vector_trigger_special, &b_std_vector_trigger_special);
   fChain->SetBranchAddress("std_vector_trigger", &std_vector_trigger, &b_std_vector_trigger);
   //   fChain->SetBranchAddress("tightmu", &tightmu, &b_tightmu);
   fChain->SetBranchAddress("triggW", &triggW, &b_triggW);
   fChain->SetBranchAddress("trigger", &trigger, &b_trigger);
   fChain->SetBranchAddress("trig_DbleEle", &trig_DbleEle, &b_trig_DbleEle);
   fChain->SetBranchAddress("trig_SnglEle", &trig_SnglEle, &b_trig_SnglEle);
   fChain->SetBranchAddress("trig_DbleMu",  &trig_DbleMu,  &b_trig_DbleMu);
   fChain->SetBranchAddress("trig_SnglMu",  &trig_SnglMu,  &b_trig_SnglMu);
   fChain->SetBranchAddress("trig_EleMu",   &trig_EleMu,   &b_trig_EleMu);
   fChain->SetBranchAddress("veto_EMTFBug", &veto_EMTFBug, &b_veto_EMTFBug);
   //   fChain->SetBranchAddress("triggerFakeRate", &triggerFakeRate, &b_triggerFakeRate);
   //   fChain->SetBranchAddress("itpu", &itpu, &b_itpu);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("nGoodVtx", &nGoodVtx, &b_nGoodVtx);
   //   fChain->SetBranchAddress("ootpum1", &ootpum1, &b_ootpum1);
   //   fChain->SetBranchAddress("ootpum2", &ootpum2, &b_ootpum2);
   //   fChain->SetBranchAddress("ootpum3", &ootpum3, &b_ootpum3);
   //   fChain->SetBranchAddress("ootpum4", &ootpum4, &b_ootpum4);
   //   fChain->SetBranchAddress("ootpum5", &ootpum5, &b_ootpum5);
   //   fChain->SetBranchAddress("ootpup1", &ootpup1, &b_ootpup1);
   //   fChain->SetBranchAddress("ootpup2", &ootpup2, &b_ootpup2);
   //   fChain->SetBranchAddress("ootpup3", &ootpup3, &b_ootpup3);
   //   fChain->SetBranchAddress("trpu", &trpu, &b_trpu);
   //   fChain->SetBranchAddress("bveto_ip", &bveto_ip, &b_bveto_ip);
   //   fChain->SetBranchAddress("std_vector_electron_ooEmooP", &std_vector_electron_ooEmooP, &b_std_vector_electron_ooEmooP);
   //   fChain->SetBranchAddress("std_vector_jet_puid", &std_vector_jet_puid, &b_std_vector_jet_puid);
   //   fChain->SetBranchAddress("std_vector_muon_NTkLayers", &std_vector_muon_NTkLayers, &b_std_vector_muon_NTkLayers);
   fChain->SetBranchAddress("std_vector_jet_cmvav2", &std_vector_jet_cmvav2, &b_std_vector_jet_cmvav2);
   //   fChain->SetBranchAddress("std_vector_muon_Chi2LocalPos", &std_vector_muon_Chi2LocalPos, &b_std_vector_muon_Chi2LocalPos);
   fChain->SetBranchAddress("std_vector_jet_phi", &std_vector_jet_phi, &b_std_vector_jet_phi);
   fChain->SetBranchAddress("std_vector_jet_mass", &std_vector_jet_mass, &b_std_vector_jet_mass);
   fChain->SetBranchAddress("std_vector_jet_chargedHadronFraction", &std_vector_jet_chargedHadronFraction, &b_std_vector_jet_chargedHadronFraction);
   //   fChain->SetBranchAddress("std_vector_muon_NValidPixelHitsInTrk", &std_vector_muon_NValidPixelHitsInTrk, &b_std_vector_muon_NValidPixelHitsInTrk);
   //   fChain->SetBranchAddress("std_vector_electron_passConversionVeto", &std_vector_electron_passConversionVeto, &b_std_vector_electron_passConversionVeto);
   //   fChain->SetBranchAddress("std_vector_puppijet_eta", &std_vector_puppijet_eta, &b_std_vector_puppijet_eta);
   //   fChain->SetBranchAddress("std_vector_muon_NValidHitsSATrk", &std_vector_muon_NValidHitsSATrk, &b_std_vector_muon_NValidHitsSATrk);
   //   fChain->SetBranchAddress("std_vector_electron_ecalPFClusterIso", &std_vector_electron_ecalPFClusterIso, &b_std_vector_electron_ecalPFClusterIso);
   //   fChain->SetBranchAddress("std_vector_jet_tche", &std_vector_jet_tche, &b_std_vector_jet_tche);
   //   fChain->SetBranchAddress("std_vector_muon_NValidHitsInTrk", &std_vector_muon_NValidHitsInTrk, &b_std_vector_muon_NValidHitsInTrk);
   fChain->SetBranchAddress("std_vector_jet_csvv2ivf", &std_vector_jet_csvv2ivf, &b_std_vector_jet_csvv2ivf);
   //   fChain->SetBranchAddress("std_vector_jet_tchp", &std_vector_jet_tchp, &b_std_vector_jet_tchp);
   //   fChain->SetBranchAddress("std_vector_electron_dEtaIn", &std_vector_electron_dEtaIn, &b_std_vector_electron_dEtaIn);
   //   fChain->SetBranchAddress("std_vector_muon_TrkKink", &std_vector_muon_TrkKink, &b_std_vector_muon_TrkKink);
   //   fChain->SetBranchAddress("std_vector_muon_NormChi2GTrk", &std_vector_muon_NormChi2GTrk, &b_std_vector_muon_NormChi2GTrk);
   //   fChain->SetBranchAddress("std_vector_electron_full5x5_sigmaIetaIeta", &std_vector_electron_full5x5_sigmaIetaIeta, &b_std_vector_electron_full5x5_sigmaIetaIeta);
   //   fChain->SetBranchAddress("std_vector_jet_ssvhe", &std_vector_jet_ssvhe, &b_std_vector_jet_ssvhe);
   //   fChain->SetBranchAddress("std_vector_puppijet_phi", &std_vector_puppijet_phi, &b_std_vector_puppijet_phi);
   fChain->SetBranchAddress("std_vector_jet_eta", &std_vector_jet_eta, &b_std_vector_jet_eta);
   fChain->SetBranchAddress("std_vector_jet_pt", &std_vector_jet_pt, &b_std_vector_jet_pt);
   fChain->SetBranchAddress("std_vector_jet_isFromISR", &std_vector_jet_isFromISR, &b_std_vector_jet_isFromISR);
   //   fChain->SetBranchAddress("std_vector_muon_NValidFractInTrk", &std_vector_muon_NValidFractInTrk, &b_std_vector_muon_NValidFractInTrk);
   //   fChain->SetBranchAddress("std_vector_jet_bjp", &std_vector_jet_bjp, &b_std_vector_jet_bjp);
   //   fChain->SetBranchAddress("std_vector_puppijet_pt", &std_vector_puppijet_pt, &b_std_vector_puppijet_pt);
   //   fChain->SetBranchAddress("std_vector_muon_SegCompatibilty", &std_vector_muon_SegCompatibilty, &b_std_vector_muon_SegCompatibilty);
   fChain->SetBranchAddress("std_vector_electron_effectiveArea", &std_vector_electron_effectiveArea, &b_std_vector_electron_effectiveArea);
   fChain->SetBranchAddress("std_vector_jet_PartonFlavour", &std_vector_jet_PartonFlavour, &b_std_vector_jet_PartonFlavour);
   //   fChain->SetBranchAddress("std_vector_jet_softMuDz", &std_vector_jet_softMuDz, &b_std_vector_jet_softMuDz);
   //   fChain->SetBranchAddress("std_vector_jet_softMuIso", &std_vector_jet_softMuIso, &b_std_vector_jet_softMuIso);
   //   fChain->SetBranchAddress("std_vector_jet_softMuEta", &std_vector_jet_softMuEta, &b_std_vector_jet_softMuEta);
   //   fChain->SetBranchAddress("std_vector_jet_softMuPt", &std_vector_jet_softMuPt, &b_std_vector_jet_softMuPt);
   //   fChain->SetBranchAddress("std_vector_jet_ssvhb", &std_vector_jet_ssvhb, &b_std_vector_jet_ssvhb);
   //   fChain->SetBranchAddress("std_vector_jet_softMuD0", &std_vector_jet_softMuD0, &b_std_vector_jet_softMuD0);
   //   fChain->SetBranchAddress("std_vector_jet_QGlikelihood", &std_vector_jet_QGlikelihood, &b_std_vector_jet_QGlikelihood);
   //   fChain->SetBranchAddress("std_vector_muon_NumOfMatchedStations", &std_vector_muon_NumOfMatchedStations, &b_std_vector_muon_NumOfMatchedStations);
   //   fChain->SetBranchAddress("std_vector_jet_softMuPhi", &std_vector_jet_softMuPhi, &b_std_vector_jet_softMuPhi);
   //   fChain->SetBranchAddress("std_vector_electron_hcalPFClusterIso", &std_vector_electron_hcalPFClusterIso, &b_std_vector_electron_hcalPFClusterIso);
   //   fChain->SetBranchAddress("std_vector_jet_bjpb", &std_vector_jet_bjpb, &b_std_vector_jet_bjpb);
   //   fChain->SetBranchAddress("std_vector_jet_pfcsv", &std_vector_jet_pfcsv, &b_std_vector_jet_pfcsv);
   //   fChain->SetBranchAddress("std_vector_electron_dPhiIn", &std_vector_electron_dPhiIn, &b_std_vector_electron_dPhiIn);
   //   fChain->SetBranchAddress("std_vector_electron_hOverE", &std_vector_electron_hOverE, &b_std_vector_electron_hOverE);
   //   fChain->SetBranchAddress("std_vector_jet_NumberSoftMu", &std_vector_jet_NumberSoftMu, &b_std_vector_jet_NumberSoftMu);
   fChain->SetBranchAddress("std_vector_jet_HadronFlavour", &std_vector_jet_HadronFlavour, &b_std_vector_jet_HadronFlavour);
   //   fChain->SetBranchAddress("std_vector_electron_scEta", &std_vector_electron_scEta, &b_std_vector_electron_scEta);
   //   fChain->SetBranchAddress("std_vector_electron_expectedMissingInnerHits", &std_vector_electron_expectedMissingInnerHits, &b_std_vector_electron_expectedMissingInnerHits);
   fChain->SetBranchAddress("metFilter", &metFilter, &b_metFilter);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("jetphi1", &jetphi1, &b_jetphi1);
   fChain->SetBranchAddress("jetphi2", &jetphi2, &b_jetphi2);
   fChain->SetBranchAddress("jetmass1", &jetmass1, &b_jetmass1);
   fChain->SetBranchAddress("jetmass2", &jetmass2, &b_jetmass2);
   //   fChain->SetBranchAddress("jettche1", &jettche1, &b_jettche1);
   //   fChain->SetBranchAddress("jettche2", &jettche2, &b_jettche2);
   fChain->SetBranchAddress("puW", &puW, &b_puW);
   //   fChain->SetBranchAddress("puW6p3", &puW6p3, &b_puW6p3);
   //   fChain->SetBranchAddress("Xsec", &Xsec, &b_Xsec);
   fChain->SetBranchAddress("baseW", &baseW, &b_baseW);
   //   fChain->SetBranchAddress("nllnnloW", &nllnnloW, &b_nllnnloW);
   fChain->SetBranchAddress("nllW", &nllW, &b_nllW);
   //   fChain->SetBranchAddress("nllW_Rup", &nllW_Rup, &b_nllW_Rup);
   //   fChain->SetBranchAddress("nllW_Qup", &nllW_Qup, &b_nllW_Qup);
   //   fChain->SetBranchAddress("nllW_Rdown", &nllW_Rdown, &b_nllW_Rdown);
   //   fChain->SetBranchAddress("nllW_Qdown", &nllW_Qdown, &b_nllW_Qdown);
   //   fChain->SetBranchAddress("gen_mww", &gen_mww, &b_gen_mww);
   //   fChain->SetBranchAddress("gen_ptww", &gen_ptww, &b_gen_ptww);
   fChain->SetBranchAddress("gen_ptll", &gen_ptll, &b_gen_ptll);
   fChain->SetBranchAddress("std_vector_lepton_chargedHadronIso", &std_vector_lepton_chargedHadronIso, &b_std_vector_lepton_chargedHadronIso);
   fChain->SetBranchAddress("std_vector_lepton_photonIso", &std_vector_lepton_photonIso, &b_std_vector_lepton_photonIso);
   //   fChain->SetBranchAddress("std_vector_lepton_closejet_PartonFlavour", &std_vector_lepton_closejet_PartonFlavour, &b_std_vector_lepton_closejet_PartonFlavour);
   //   fChain->SetBranchAddress("std_vector_lepton_closejet_pt", &std_vector_lepton_closejet_pt, &b_std_vector_lepton_closejet_pt);
   fChain->SetBranchAddress("std_vector_lepton_neutralHadronIso", &std_vector_lepton_neutralHadronIso, &b_std_vector_lepton_neutralHadronIso);
   fChain->SetBranchAddress("std_vector_lepton_eleIdMedium", &std_vector_lepton_eleIdMedium, &b_std_vector_lepton_eleIdMedium);
   //   fChain->SetBranchAddress("std_vector_lepton_chargedParticleIso", &std_vector_lepton_chargedParticleIso, &b_std_vector_lepton_chargedParticleIso);
   fChain->SetBranchAddress("std_vector_lepton_phi", &std_vector_lepton_phi, &b_std_vector_lepton_phi);
   //   fChain->SetBranchAddress("std_vector_lepton_eleIdLoose", &std_vector_lepton_eleIdLoose, &b_std_vector_lepton_eleIdLoose);
   //   fChain->SetBranchAddress("std_vector_lepton_dz", &std_vector_lepton_dz, &b_std_vector_lepton_dz);
   //   fChain->SetBranchAddress("std_vector_lepton_eleIdVeto", &std_vector_lepton_eleIdVeto, &b_std_vector_lepton_eleIdVeto);
   fChain->SetBranchAddress("std_vector_lepton_isLooseLepton", &std_vector_lepton_isLooseLepton, &b_std_vector_lepton_isLooseLepton);
   fChain->SetBranchAddress("std_vector_lepton_pt", &std_vector_lepton_pt, &b_std_vector_lepton_pt);
   fChain->SetBranchAddress("std_vector_lepton_sumPUPt", &std_vector_lepton_sumPUPt, &b_std_vector_lepton_sumPUPt);
   fChain->SetBranchAddress("std_vector_lepton_eta", &std_vector_lepton_eta, &b_std_vector_lepton_eta);
   fChain->SetBranchAddress("std_vector_lepton_isMediumMuon", &std_vector_lepton_isMediumMuon, &b_std_vector_lepton_isMediumMuon);
   //   fChain->SetBranchAddress("std_vector_lepton_PfIsoDeltaBeta", &std_vector_lepton_PfIsoDeltaBeta, &b_std_vector_lepton_PfIsoDeltaBeta);
   //   fChain->SetBranchAddress("std_vector_lepton_closejet_phi", &std_vector_lepton_closejet_phi, &b_std_vector_lepton_closejet_phi);
   fChain->SetBranchAddress("std_vector_lepton_trackIso", &std_vector_lepton_trackIso, &b_std_vector_lepton_trackIso);
   //   fChain->SetBranchAddress("std_vector_lepton_isWgsLepton", &std_vector_lepton_isWgsLepton, &b_std_vector_lepton_isWgsLepton);
   //   fChain->SetBranchAddress("std_vector_lepton_closejet_eta", &std_vector_lepton_closejet_eta, &b_std_vector_lepton_closejet_eta);
   //   fChain->SetBranchAddress("std_vector_lepton_SIP3D", &std_vector_lepton_SIP3D, &b_std_vector_lepton_SIP3D);
   fChain->SetBranchAddress("std_vector_lepton_isTightLepton", &std_vector_lepton_isTightLepton, &b_std_vector_lepton_isTightLepton);
   fChain->SetBranchAddress("std_vector_lepton_ch", &std_vector_lepton_ch, &b_std_vector_lepton_ch);
   fChain->SetBranchAddress("std_vector_lepton_flavour", &std_vector_lepton_flavour, &b_std_vector_lepton_flavour);
   //   fChain->SetBranchAddress("std_vector_lepton_closejet_drlj", &std_vector_lepton_closejet_drlj, &b_std_vector_lepton_closejet_drlj);
   fChain->SetBranchAddress("std_vector_lepton_eleIdTight", &std_vector_lepton_eleIdTight, &b_std_vector_lepton_eleIdTight);
   fChain->SetBranchAddress("std_vector_lepton_d0", &std_vector_lepton_d0, &b_std_vector_lepton_d0);
   fChain->SetBranchAddress("std_vector_lepton_genmatched", &std_vector_lepton_genmatched, &b_std_vector_lepton_genmatched);
   fChain->SetBranchAddress("std_vector_lepton_promptgenmatched", &std_vector_lepton_promptgenmatched, &b_std_vector_lepton_promptgenmatched);
   fChain->SetBranchAddress("std_vector_lepton_isTightMuon", &std_vector_lepton_isTightMuon, &b_std_vector_lepton_isTightMuon);
   fChain->SetBranchAddress("susyMstop", &susyMstop, &b_susyMstop);
   fChain->SetBranchAddress("susyMLSP", &susyMLSP, &b_susyMLSP);
   fChain->SetBranchAddress("metPfType1Phi", &metPfType1Phi, &b_metPfType1Phi);
   fChain->SetBranchAddress("metPfType1", &metPfType1, &b_metPfType1);
   fChain->SetBranchAddress("bPogSF", &bPogSF, &b_bPogSF);
   fChain->SetBranchAddress("bPogSFUp", &bPogSFUp, &b_bPogSFUp);
   fChain->SetBranchAddress("bPogSFDown", &bPogSFDown, &b_bPogSFDown);
   fChain->SetBranchAddress("bPogSF_CMVAL", &bPogSF_CMVAL, &b_bPogSF_CMVAL);
   fChain->SetBranchAddress("bPogSF_CMVAL_up", &bPogSF_CMVAL_up, &b_bPogSF_CMVAL_up);
   fChain->SetBranchAddress("bPogSF_CMVAL_down", &bPogSF_CMVAL_down, &b_bPogSF_CMVAL_down);
   fChain->SetBranchAddress("bPogSF_CMVAM", &bPogSF_CMVAM, &b_bPogSF_CMVAM);
   fChain->SetBranchAddress("bPogSF_CMVAM_up", &bPogSF_CMVAM_up, &b_bPogSF_CMVAM_up);
   fChain->SetBranchAddress("bPogSF_CMVAM_down", &bPogSF_CMVAM_down, &b_bPogSF_CMVAM_down);
   fChain->SetBranchAddress("bPogSF_CMVAT", &bPogSF_CMVAT, &b_bPogSF_CMVAT);
   fChain->SetBranchAddress("bPogSF_CMVAT_up", &bPogSF_CMVAT_up, &b_bPogSF_CMVAT_up);
   fChain->SetBranchAddress("bPogSF_CMVAT_down", &bPogSF_CMVAT_down, &b_bPogSF_CMVAT_down);
   fChain->SetBranchAddress("bPogSF_CSVL", &bPogSF_CSVL, &b_bPogSF_CSVL);
   fChain->SetBranchAddress("bPogSF_CSVL_up", &bPogSF_CSVL_up, &b_bPogSF_CSVL_up);
   fChain->SetBranchAddress("bPogSF_CSVL_down", &bPogSF_CSVL_down, &b_bPogSF_CSVL_down);
   fChain->SetBranchAddress("bPogSF_CSVM", &bPogSF_CSVM, &b_bPogSF_CSVM);
   fChain->SetBranchAddress("bPogSF_CSVM_up", &bPogSF_CSVM_up, &b_bPogSF_CSVM_up);
   fChain->SetBranchAddress("bPogSF_CSVM_down", &bPogSF_CSVM_down, &b_bPogSF_CSVM_down);
   fChain->SetBranchAddress("bPogSF_CSVT", &bPogSF_CSVT, &b_bPogSF_CSVT);
   fChain->SetBranchAddress("bPogSF_CSVT_up", &bPogSF_CSVT_up, &b_bPogSF_CSVT_up);
   fChain->SetBranchAddress("bPogSF_CSVT_down", &bPogSF_CSVT_down, &b_bPogSF_CSVT_down);
   //   fChain->SetBranchAddress("bTPSF", &bTPSF, &b_bTPSF);
   //   fChain->SetBranchAddress("bTPSFUp", &bTPSFUp, &b_bTPSFUp);
   //   fChain->SetBranchAddress("bTPSFDown", &bTPSFDown, &b_bTPSFDown);
   //   fChain->SetBranchAddress("bPogSF1Jet", &bPogSF1Jet, &b_bPogSF1Jet);
   //   fChain->SetBranchAddress("bPogSF1JetUp", &bPogSF1JetUp, &b_bPogSF1JetUp);
   //   fChain->SetBranchAddress("bPogSF1JetDown", &bPogSF1JetDown, &b_bPogSF1JetDown);
   //   fChain->SetBranchAddress("bTPSF1Jet", &bTPSF1Jet, &b_bTPSF1Jet);
   //   fChain->SetBranchAddress("bTPSF1JetUp", &bTPSF1JetUp, &b_bTPSF1JetUp);
   //   fChain->SetBranchAddress("bTPSF1JetDown", &bTPSF1JetDown, &b_bTPSF1JetDown);
   //   fChain->SetBranchAddress("bPogSF2Jet", &bPogSF2Jet, &b_bPogSF2Jet);
   //   fChain->SetBranchAddress("bPogSF2JetUp", &bPogSF2JetUp, &b_bPogSF2JetUp);
   //   fChain->SetBranchAddress("bPogSF2JetDown", &bPogSF2JetDown, &b_bPogSF2JetDown);
   //   fChain->SetBranchAddress("bTPSF2Jet", &bTPSF2Jet, &b_bTPSF2Jet);
   //   fChain->SetBranchAddress("bTPSF2JetUp", &bTPSF2JetUp, &b_bTPSF2JetUp);
   //   fChain->SetBranchAddress("bTPSF2JetDown", &bTPSF2JetDown, &b_bTPSF2JetDown);
   fChain->SetBranchAddress("effTrigW_Down", &effTrigW_Down, &b_effTrigW_Down);
   fChain->SetBranchAddress("effTrigW_Up", &effTrigW_Up, &b_effTrigW_Up);
   fChain->SetBranchAddress("effTrigW", &effTrigW, &b_effTrigW);
   fChain->SetBranchAddress("std_vector_lepton_idisoWcut_WP_Tight80X", &std_vector_lepton_idisoWcut_WP_Tight80X, &b_std_vector_lepton_idisoWcut_WP_Tight80X);
   fChain->SetBranchAddress("std_vector_lepton_idisoWcut_WP_Tight80X_Up", &std_vector_lepton_idisoWcut_WP_Tight80X_Up, &b_std_vector_lepton_idisoWcut_WP_Tight80X_Up);
   fChain->SetBranchAddress("std_vector_lepton_idisoWcut_WP_Tight80X_Down", &std_vector_lepton_idisoWcut_WP_Tight80X_Down, &b_std_vector_lepton_idisoWcut_WP_Tight80X_Down);
   fChain->SetBranchAddress("std_vector_lepton_idisoW", &std_vector_lepton_idisoW, &b_std_vector_lepton_idisoW);
   fChain->SetBranchAddress("std_vector_lepton_idisoW_Up", &std_vector_lepton_idisoW_Up, &b_std_vector_lepton_idisoW_Up);
   fChain->SetBranchAddress("std_vector_lepton_idisoW_Down", &std_vector_lepton_idisoW_Down, &b_std_vector_lepton_idisoW_Down);
   //   fChain->SetBranchAddress("std_vector_lepton_idisoW_Syst", &std_vector_lepton_idisoW_Syst, &b_std_vector_lepton_idisoW_Syst);
   //   fChain->SetBranchAddress("std_vector_lepton_idisoLooseW", &std_vector_lepton_idisoLooseW, &b_std_vector_lepton_idisoLooseW);
   //   fChain->SetBranchAddress("std_vector_lepton_idisoLooseW_Up", &std_vector_lepton_idisoLooseW_Up, &b_std_vector_lepton_idisoLooseW_Up);
   //   fChain->SetBranchAddress("std_vector_lepton_idisoLooseW_Down", &std_vector_lepton_idisoLooseW_Down, &b_std_vector_lepton_idisoLooseW_Down);
   //   fChain->SetBranchAddress("std_vector_lepton_idisoLooseW_Syst", &std_vector_lepton_idisoLooseW_Syst, &b_std_vector_lepton_idisoLooseW_Syst);
   fChain->SetBranchAddress("std_vector_lepton_recoW", &std_vector_lepton_recoW, &b_std_vector_lepton_recoW);
   fChain->SetBranchAddress("std_vector_lepton_recoW_Up", &std_vector_lepton_recoW_Up, &b_std_vector_lepton_recoW_Up);
   fChain->SetBranchAddress("std_vector_lepton_recoW_Down", &std_vector_lepton_recoW_Down, &b_std_vector_lepton_recoW_Down);
   fChain->SetBranchAddress("std_vector_lepton_fastsimW", &std_vector_lepton_fastsimW, &b_std_vector_lepton_fastsimW);
   fChain->SetBranchAddress("std_vector_lepton_fastsimW_Up", &std_vector_lepton_fastsimW_Up, &b_std_vector_lepton_fastsimW_Up);
   fChain->SetBranchAddress("std_vector_lepton_fastsimW_Down", &std_vector_lepton_fastsimW_Down, &b_std_vector_lepton_fastsimW_Down);
   //   fChain->SetBranchAddress("mcoll", &mcoll, &b_mcoll);
   //   fChain->SetBranchAddress("mcollWW", &mcollWW, &b_mcollWW);
   //   fChain->SetBranchAddress("vht_pt", &vht_pt, &b_vht_pt);
   //   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   //   fChain->SetBranchAddress("mllThird", &mllThird, &b_mllThird);
   fChain->SetBranchAddress("dphilljetjet", &dphilljetjet, &b_dphilljetjet);
   //   fChain->SetBranchAddress("vht_phi", &vht_phi, &b_vht_phi);
   fChain->SetBranchAddress("drll", &drll, &b_drll);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("dphilljet", &dphilljet, &b_dphilljet);
   fChain->SetBranchAddress("dphilmet1", &dphilmet1, &b_dphilmet1);
   fChain->SetBranchAddress("dphilmet2", &dphilmet2, &b_dphilmet2);
   //   fChain->SetBranchAddress("projpfmet", &projpfmet, &b_projpfmet);
   //   fChain->SetBranchAddress("mTi", &mTi, &b_mTi);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("dphill", &dphill, &b_dphill);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("detajj", &detajj, &b_detajj);
   fChain->SetBranchAddress("ptll", &ptll, &b_ptll);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("dphilmet", &dphilmet, &b_dphilmet);
   fChain->SetBranchAddress("mth", &mth, &b_mth);
   fChain->SetBranchAddress("yll", &yll, &b_yll);
   fChain->SetBranchAddress("mtw2", &mtw2, &b_mtw2);
   fChain->SetBranchAddress("mtw1", &mtw1, &b_mtw1);
   //   fChain->SetBranchAddress("isJsonOk", &isJsonOk, &b_isJsonOk);


   // 3lepton variables
   fChain->SetBranchAddress("njet_3l", &njet_3l, &b_njet_3l);
   fChain->SetBranchAddress("flagOSSF", &flagOSSF, &b_flagOSSF);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("eta3", &eta3, &b_eta3);
   fChain->SetBranchAddress("zveto_3l", &zveto_3l, &b_zveto_3l);
   fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
   fChain->SetBranchAddress("chlll", &chlll, &b_chlll);
   fChain->SetBranchAddress("drllmin3l", &drllmin3l, &b_drllmin3l);
   fChain->SetBranchAddress("mlll", &mlll, &b_mlll);
   fChain->SetBranchAddress("mllmin3l", &mllmin3l, &b_mllmin3l);
   fChain->SetBranchAddress("phi3", &phi3, &b_phi3);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("pt3", &pt3, &b_pt3);
   fChain->SetBranchAddress("nbjet_3l", &nbjet_3l, &b_nbjet_3l);


   // Full2016_Apr17 electron working points
   fChain->SetBranchAddress("LepCut2l__ele_cut_WP_Tight80X__mu_cut_Tight80x", &LepCut2l__ele_cut_WP_Tight80X__mu_cut_Tight80x, &b_LepCut2l__ele_cut_WP_Tight80X__mu_cut_Tight80x);
   fChain->SetBranchAddress("LepSF2l__ele_cut_WP_Tight80X__mu_cut_Tight80x", &LepSF2l__ele_cut_WP_Tight80X__mu_cut_Tight80x, &b_LepSF2l__ele_cut_WP_Tight80X__mu_cut_Tight80x);
   fChain->SetBranchAddress("LepSF2l__mu_cut_Tight80x__Do", &LepSF2l__mu_cut_Tight80x__Do, &b_LepSF2l__mu_cut_Tight80x__Do);
   fChain->SetBranchAddress("LepSF2l__mu_cut_Tight80x__Up", &LepSF2l__mu_cut_Tight80x__Up, &b_LepSF2l__mu_cut_Tight80x__Up);
   fChain->SetBranchAddress("LepSF2l__ele_cut_WP_Tight80X__Do", &LepSF2l__ele_cut_WP_Tight80X__Do, &b_LepSF2l__ele_cut_WP_Tight80X__Do);
   fChain->SetBranchAddress("LepSF2l__ele_cut_WP_Tight80X__Up", &LepSF2l__ele_cut_WP_Tight80X__Up, &b_LepSF2l__ele_cut_WP_Tight80X__Up);
   fChain->SetBranchAddress("METFilter_DATA", &METFilter_DATA, &b_METFilter_DATA);
   fChain->SetBranchAddress("METFilter_MC", &METFilter_MC, &b_METFilter_MC);

   fChain->SetBranchAddress("std_vector_muon_isTightLepton_cut_Tight80x", &std_vector_muon_isTightLepton_cut_Tight80x, &b_std_vector_muon_isTightLepton_cut_Tight80x);
   fChain->SetBranchAddress("std_vector_muon_idisoW_cut_Tight80x", &std_vector_muon_idisoW_cut_Tight80x, &b_std_vector_muon_idisoW_cut_Tight80x);
   fChain->SetBranchAddress("std_vector_muon_idisoW_cut_Tight80x_Down", &std_vector_muon_idisoW_cut_Tight80x_Down, &b_std_vector_muon_idisoW_cut_Tight80x_Down);
   fChain->SetBranchAddress("std_vector_muon_idisoW_cut_Tight80x_Up", &std_vector_muon_idisoW_cut_Tight80x_Up, &b_std_vector_muon_idisoW_cut_Tight80x_Up);

   fChain->SetBranchAddress("std_vector_electron_isTightLepton_cut_WP_Tight80X", &std_vector_electron_isTightLepton_cut_WP_Tight80X, &b_std_vector_electron_isTightLepton_cut_WP_Tight80X);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_cut_WP_Tight80X_SS", &std_vector_electron_isTightLepton_cut_WP_Tight80X_SS, &b_std_vector_electron_isTightLepton_cut_WP_Tight80X_SS);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_mva_80p_Iso2015", &std_vector_electron_isTightLepton_mva_80p_Iso2015, &b_std_vector_electron_isTightLepton_mva_80p_Iso2015);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_mva_80p_Iso2016", &std_vector_electron_isTightLepton_mva_80p_Iso2016, &b_std_vector_electron_isTightLepton_mva_80p_Iso2016);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_mva_90p_Iso2015", &std_vector_electron_isTightLepton_mva_90p_Iso2015, &b_std_vector_electron_isTightLepton_mva_90p_Iso2015);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_mva_90p_Iso2016", &std_vector_electron_isTightLepton_mva_90p_Iso2016, &b_std_vector_electron_isTightLepton_mva_90p_Iso2016);

   fChain->SetBranchAddress("std_vector_electron_idisoW_cut_WP_Tight80X", &std_vector_electron_idisoW_cut_WP_Tight80X, &b_std_vector_electron_idisoW_cut_WP_Tight80X);
   fChain->SetBranchAddress("std_vector_electron_idisoW_cut_WP_Tight80X_Down", &std_vector_electron_idisoW_cut_WP_Tight80X_Down, &b_std_vector_electron_idisoW_cut_WP_Tight80X_Down);
   fChain->SetBranchAddress("std_vector_electron_idisoW_cut_WP_Tight80X_Up", &std_vector_electron_idisoW_cut_WP_Tight80X_Up, &b_std_vector_electron_idisoW_cut_WP_Tight80X_Up);

   fChain->SetBranchAddress("std_vector_electron_idisoW_cut_WP_Tight80X_SS", &std_vector_electron_idisoW_cut_WP_Tight80X_SS, &b_std_vector_electron_idisoW_cut_WP_Tight80X_SS);
   fChain->SetBranchAddress("std_vector_electron_idisoW_mva_80p_Iso2015", &std_vector_electron_idisoW_mva_80p_Iso2015, &b_std_vector_electron_idisoW_mva_80p_Iso2015);
   fChain->SetBranchAddress("std_vector_electron_idisoW_mva_80p_Iso2016", &std_vector_electron_idisoW_mva_80p_Iso2016, &b_std_vector_electron_idisoW_mva_80p_Iso2016);
   fChain->SetBranchAddress("std_vector_electron_idisoW_mva_90p_Iso2015", &std_vector_electron_idisoW_mva_90p_Iso2015, &b_std_vector_electron_idisoW_mva_90p_Iso2015);
   fChain->SetBranchAddress("std_vector_electron_idisoW_mva_90p_Iso2016", &std_vector_electron_idisoW_mva_90p_Iso2016, &b_std_vector_electron_idisoW_mva_90p_Iso2016);

   fChain->SetBranchAddress("std_vector_muon_isTightLepton_cut_Tight80x", &std_vector_muon_isTightLepton_cut_Tight80x, &b_std_vector_muon_isTightLepton_cut_Tight80x);
   fChain->SetBranchAddress("std_vector_electron_isTightLepton_cut_WP_Tight80X", &std_vector_electron_isTightLepton_cut_WP_Tight80X, &b_std_vector_electron_isTightLepton_cut_WP_Tight80X);


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
   return 1;
}
#endif // #ifdef AnalysisBase_cxx
