#define nanoFakesDataRunC_cxx
// The class definition in nanoFakesDataRunC.h has been generated automatically
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
// root> T->Process("nanoFakesDataRunC.C")
// root> T->Process("nanoFakesDataRunC.C","some options")
// root> T->Process("nanoFakesDataRunC.C+")
//


#include "nanoFakesDataRunC.h"
#include <TStyle.h>

int nentries = 0;
int maxentries = -1;

TFile* root_output;
float _channel;
float inputJetEt;
float leptonPtMin;
float leptonEtaMax;
float event_weight;
float deltaR;

void nanoFakesDataRunC::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

   for (int i=0; i<njetet; i++) {

     TString muonsuffix = Form("_%.0fGeV", muonjetet[i]);
     TString elesuffix = Form("_%.0fGeV", elejetet[i]);
     
     //Histograms definition for all the variables considered
     h_Muon_loose_pt_eta_bin[i] = new TH2D("h_Muon_loose_pt_eta_bin" + muonsuffix, "", nptbin, ptbins, netabin, etabins);
     h_Muon_tight_pt_eta_bin[i] = new TH2D("h_Muon_tight_pt_eta_bin" + muonsuffix, "", nptbin, ptbins, netabin, etabins);
     h_Ele_loose_pt_eta_bin [i] = new TH2D("h_Ele_loose_pt_eta_bin"  + elesuffix,  "", nptbin, ptbins, netabin, etabins);
     h_Ele_tight_pt_eta_bin [i] = new TH2D("h_Ele_tight_pt_eta_bin"  + elesuffix,  "", nptbin, ptbins, netabin, etabins);
     
     h_Muon_loose_pt_bin[i] = new TH1D("h_Muon_loose_pt_bin" + muonsuffix, "", nptbin, ptbins);
     h_Muon_tight_pt_bin[i] = new TH1D("h_Muon_tight_pt_bin" + muonsuffix, "", nptbin, ptbins);
     h_Ele_loose_pt_bin [i] = new TH1D("h_Ele_loose_pt_bin"  + elesuffix,  "", nptbin, ptbins);
     h_Ele_tight_pt_bin [i] = new TH1D("h_Ele_tight_pt_bin"  + elesuffix,  "", nptbin, ptbins);
     
     h_Muon_loose_eta_bin[i] = new TH1D("h_Muon_loose_eta_bin" + muonsuffix, "", netabin, etabins);
     h_Muon_tight_eta_bin[i] = new TH1D("h_Muon_tight_eta_bin" + muonsuffix, "", netabin, etabins);
     h_Ele_loose_eta_bin [i] = new TH1D("h_Ele_loose_eta_bin"  + elesuffix,  "", netabin, etabins);
     h_Ele_tight_eta_bin [i] = new TH1D("h_Ele_tight_eta_bin"  + elesuffix,  "", netabin, etabins);

   }

  root_output = new TFile("results.root", "recreate");
  root_output->cd();

}

void nanoFakesDataRunC::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t nanoFakesDataRunC::Process(Long64_t entry)
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

  if(entry > maxentries && maxentries > -1) return 0;
  if(entry%10000 == 0) printf("Entry number %lld \n", entry); 	 
  nentries++;

  _channel = (abs(Lepton_pdgId[0]) == 11) ? e : m;

  leptonPtMin  = (_channel == e) ?  13 :  10;
  leptonEtaMax = (_channel == m) ? 2.5 : 2.4;

  if (Lepton_pt[0] < leptonPtMin) return 0;
  if (fabs(Lepton_eta[0]) > leptonEtaMax) return 0;
  
  event_weight = 1.0;

  bool passTrigger = false;
  
  // Muons
  //------------------------------------------------------------------------
  if (_channel == m) {
    
    if (Lepton_pt[0] <= 20. && *HLT_Mu8_TrkIsoVVL > 0.5) {
      
      passTrigger = true;

    } else if (Lepton_pt[0] > 20. && *HLT_Mu17_TrkIsoVVL > 0.5) {

      passTrigger = true;
    }
  }
  
  
  // Electrons
  //------------------------------------------------------------------------
  if (_channel == e) {
     
    /*if (Lepton_pt[0] <= 25. && *HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v > 0.5) {
      
       passTrigger = true;
       
       } else if (Lepton_pt[0] > 25. && *HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v > 0.5) {
       
       passTrigger = true;
       }*/
   }

  for (int i=0; i<njetet; i++) {
    
    inputJetEt = (_channel == e) ? elejetet[i] : muonjetet[i];

    bool passCuts = passTrigger;

    passCuts &= (*nLepton == 1);
    passCuts &= (*mtw1 < 20.);

    //Equivalent to getAwayJets function
    bool passJets = passCuts;
    
    deltaR = sqrt((*dphilep1jet1**dphilep1jet1)+(CleanJet_eta[0]*CleanJet_eta[0]));

    passJets &= (*nCleanJet > 1); 
    passJets &= (deltaR > 1 && CleanJet_pt[0] >= inputJetEt);

    FillLevelHistograms(i, passJets);

  }

  return kTRUE;
}

void nanoFakesDataRunC::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void nanoFakesDataRunC::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  printf("\n\n Writing histograms. This can take a while...");

  h_Muon_loose_pt_bin[0] -> Draw();

  root_output->Write("", TObject::kOverwrite);
  root_output->Close();

}

//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void nanoFakesDataRunC::FillLevelHistograms(int i, bool pass)
{
  if (!pass) return;

  FillAnalysisHistograms(i);
}


//------------------------------------------------------------------------------   
// FillanalysisHistograms
//------------------------------------------------------------------------------                 
void nanoFakesDataRunC::FillAnalysisHistograms(int i)
{


  if (_channel == m) {

    h_Muon_loose_pt_eta_bin[i]->Fill(Lepton_pt[0], Lepton_eta[0], event_weight);
    h_Muon_loose_pt_bin    [i]->Fill(Lepton_pt[0],  event_weight);
    h_Muon_loose_eta_bin   [i]->Fill(Lepton_eta[0], event_weight);

  } else if (_channel == e) {

    h_Ele_loose_pt_eta_bin[i]->Fill(Lepton_pt[0], Lepton_eta[0], event_weight);
    h_Ele_loose_pt_bin    [i]->Fill(Lepton_pt[0],  event_weight);
    h_Ele_loose_eta_bin   [i]->Fill(Lepton_eta[0], event_weight);
    
  }

  if (Lepton_isTightElectron_mvaFall17Iso[0] > 0.5 || Lepton_isTightMuon_cut_Tight80x[0] > 0.5) {
      
    if (_channel == m) {

      h_Muon_tight_pt_eta_bin[i]->Fill(Lepton_pt[0], Lepton_eta[0], event_weight);
      h_Muon_tight_pt_bin [i]->Fill(Lepton_pt[0],  event_weight);
      h_Muon_tight_eta_bin[i]->Fill(Lepton_eta[0], event_weight);
      
    } else if (_channel == e) {

      h_Ele_tight_pt_eta_bin[i]->Fill(Lepton_pt[0], Lepton_eta[0], event_weight);
      h_Ele_tight_pt_bin [i]->Fill(Lepton_pt[0],  event_weight);
      h_Ele_tight_eta_bin[i]->Fill(Lepton_eta[0], event_weight);
    }
  }

}

/* 

[2018/07/16] Total luminosity = 41.530/fb
export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH

brilcalc lumi -b "STABLE BEAMS"               --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json               -u /pb               -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt  --hltpath "HLT_Mu8_TrkIsoVVL_v*"

#Summary: 
+-----------------------+-------+------+--------+-------------------+------------------+
| hltpath               | nfill | nrun | ncms   | totdelivered(/pb) | totrecorded(/pb) |
+-----------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu8_TrkIsoVVL_v10 | 91    | 218  | 124448 | 2.092             | 1.831            |
| HLT_Mu8_TrkIsoVVL_v11 | 2     | 13   | 4469   | 0.065             | 0.062            |
| HLT_Mu8_TrkIsoVVL_v6  | 13    | 46   | 15947  | 0.148             | 0.143            |
| HLT_Mu8_TrkIsoVVL_v7  | 13    | 36   | 8349   | 0.080             | 0.074            |
| HLT_Mu8_TrkIsoVVL_v8  | 9     | 21   | 5909   | 0.119             | 0.109            |
| HLT_Mu8_TrkIsoVVL_v9  | 47    | 140  | 46316  | 0.733             | 0.684            |
+-----------------------+-------+------+--------+-------------------+------------------+
#Sum delivered : 3.237
#Sum recorded : 2.903


brilcalc lumi -b "STABLE BEAMS"               --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json               -u /pb               -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt  --hltpath "HLT_Mu17_TrkIsoVVL_v*" 

#Summary: 
+------------------------+-------+------+--------+-------------------+------------------+
| hltpath                | nfill | nrun | ncms   | totdelivered(/pb) | totrecorded(/pb) |
+------------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu17_TrkIsoVVL_v10 | 91    | 218  | 124448 | 46.654            | 44.670           |
| HLT_Mu17_TrkIsoVVL_v11 | 2     | 13   | 4469   | 1.588             | 1.535            |
| HLT_Mu17_TrkIsoVVL_v5  | 2     | 3    | 1775   | 0.137             | 0.132            |
| HLT_Mu17_TrkIsoVVL_v6  | 11    | 43   | 14172  | 1.552             | 1.491            |
| HLT_Mu17_TrkIsoVVL_v7  | 13    | 36   | 8349   | 1.114             | 1.038            |
| HLT_Mu17_TrkIsoVVL_v8  | 9     | 21   | 5909   | 1.896             | 1.741            |
| HLT_Mu17_TrkIsoVVL_v9  | 47    | 140  | 46316  | 16.322            | 15.335           |
+------------------------+-------+------+--------+-------------------+------------------+
#Sum delivered : 69.263
#Sum recorded : 65.944


brilcalc lumi -b "STABLE BEAMS"               --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json               -u /pb               -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt  --hltpath "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v*"

#Summary: 
+----------------------------------------------+-------+------+-------+-------------------+------------------+
| hltpath                                      | nfill | nrun | ncms  | totdelivered(/pb) | totrecorded(/pb) |
+----------------------------------------------+-------+------+-------+-------------------+------------------+
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v10 | 9     | 21   | 5909  | 0.602             | 0.555            |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v11 | 20    | 84   | 23011 | 2.288             | 2.091            |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v12 | 27    | 56   | 23305 | 1.173             | 1.131            |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v13 | 18    | 47   | 19043 | 4.148             | 3.858            |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v14 | 67    | 144  | 90816 | 17.347            | 16.644           |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v15 | 6     | 27   | 14589 | 2.745             | 2.637            |
| HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v16 | 2     | 13   | 4469  | 0.810             | 0.783            |
+----------------------------------------------+-------+------+-------+-------------------+------------------+
#Sum delivered : 29.114
#Sum recorded : 27.699


brilcalc lumi -b "STABLE BEAMS"               --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json               -u /pb               -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt  --hltpath "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v*"

#Summary: 
+----------------------------------------------+-------+------+-------+-------------------+------------------+
| hltpath                                      | nfill | nrun | ncms  | totdelivered(/pb) | totrecorded(/pb) |
+----------------------------------------------+-------+------+-------+-------------------+------------------+
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v10 | 9     | 21   | 5909  | 1.070             | 0.994            |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v11 | 20    | 84   | 23011 | 4.410             | 4.056            |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v12 | 27    | 56   | 23305 | 4.573             | 4.407            |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v13 | 18    | 47   | 19043 | 4.148             | 3.858            |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v14 | 67    | 144  | 90816 | 21.239            | 20.408           |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v15 | 6     | 27   | 14589 | 7.811             | 7.534            |
| HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v16 | 2     | 13   | 4469  | 2.279             | 2.211            |
+----------------------------------------------+-------+------+-------+-------------------+------------------+
#Sum delivered : 45.531
#Sum recorded : 43.469

*/
