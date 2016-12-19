#include "TLorentzVector.h"
#include "TVector2.h"
#include "math.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <vector>


//-----------------Data----------------------------
struct Event {
  
  float phiMED, etaMED, thetaMED, ptMED, pMED, mMED;
  float phiTOP1, etaTOP1, thetaTOP1, ptTOP1, pTOP1, mTOP1;
  float phiTOP2, etaTOP2, thetaTOP2, ptTOP2, pTOP2, mTOP2;
  
  float phiW1, etaW1, thetaW1, ptW1, pW1, mW1;
  float phiW2, etaW2, thetaW2, ptW2, pW2, mW2;
  
  float philep1, etalep1, thetalep1, ptlep1, plep1, mlep1;
  float philep2, etalep2, thetalep2, ptlep2, plep2, mlep2;
  
  float phinu1, etanu1, thetanu1, ptnu1, pnu1, mnu1;
  float phinu2, etanu2, thetanu2, ptnu2, pnu2, mnu2;
  
  float phib1, etab1, thetab1, ptb1, pb1, mb1;
  float phib2, etab2, thetab2, ptb2, pb2, mb2;
  
  float phiMET, etaMET, thetaMET, ptMET, pMET, mMET;
  float phiMETNu, etaMETNu, thetaMETNu, ptMETNu, pMETNu, mMETNu;
  float phiJets, etaJets, thetaJets, ptJets, pJets, mJets;
 
  float massTOP, massMED;

  float jzb;

  int nEvent;

} ev;


void setSetBranchAddresses(TTree *);




int main(int argc, char **argv) {


  TFile *f = new TFile(argv[1]);
  f->cd();
  TTree *tree = (TTree *)f->Get("events");
  setSetBranchAddresses(tree);

  Long64_t nentries = tree->GetEntriesFast();
  Long64_t nb = 0;

  TH1F *mlb = new TH1F("mlb", "", 200, 0, 200);

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

      nb = tree->GetEntry(jentry);

      //Loading into TLorentzVector    
      TLorentzVector l1; l1.SetPtEtaPhiM(ev.ptlep1, ev.etalep1, ev.philep1, ev.mlep1);
      TLorentzVector l2; l2.SetPtEtaPhiM(ev.ptlep2, ev.etalep2, ev.philep2, ev.mlep2);
      TLorentzVector j1; j1.SetPtEtaPhiM(ev.ptb1, ev.etab1, ev.phib1, ev.mb1);
      TLorentzVector j2; j2.SetPtEtaPhiM(ev.ptb2, ev.etab2, ev.phib2, ev.mb2);
      TLorentzVector n1; n1.SetPtEtaPhiM(ev.ptnu1, ev.etanu1, ev.phinu1, ev.mnu1);
      TLorentzVector n2; n2.SetPtEtaPhiM(ev.ptnu2, ev.etanu2, ev.phinu2, ev.mnu2);
      mlb->Fill((l1+j1).M());
      mlb->Fill((l2+j2).M());
  }  

  mlb->Scale(1.0/mlb->Integral());
  TFile *foutput = new TFile("mlb.root", "UPDATE");
  foutput->cd();
  mlb->Write();
  foutput->Close();
  f->Close();

  return 1;

}








void setSetBranchAddresses(TTree *s) {
  
  s->SetBranchAddress("massTOP", &ev.massTOP);
  s->SetBranchAddress("massMED", &ev.massMED);
  s->SetBranchAddress("nEvent", &ev.nEvent);

  s->SetBranchAddress("phiMED", &ev.phiMED);
  s->SetBranchAddress("etaMED", &ev.etaMED);
  s->SetBranchAddress("thetaMED", &ev.thetaMED);
  s->SetBranchAddress("ptMED", &ev.ptMED);
  s->SetBranchAddress("pMED", &ev.pMED);
  s->SetBranchAddress("mMED", &ev.mMED);

  s->SetBranchAddress("phiTOP1", &ev.phiTOP1);
  s->SetBranchAddress("etaTOP1", &ev.etaTOP1);
  s->SetBranchAddress("thetaTOP1", &ev.thetaTOP1);
  s->SetBranchAddress("ptTOP1", &ev.ptTOP1);
  s->SetBranchAddress("pTOP1", &ev.pTOP1);
  s->SetBranchAddress("mTOP1", &ev.mTOP1);

  s->SetBranchAddress("phiTOP2", &ev.phiTOP2);
  s->SetBranchAddress("etaTOP2", &ev.etaTOP2);
  s->SetBranchAddress("thetaTOP2", &ev.thetaTOP2);
  s->SetBranchAddress("ptTOP2", &ev.ptTOP2);
  s->SetBranchAddress("pTOP2", &ev.pTOP2);
  s->SetBranchAddress("mTOP2", &ev.mTOP2);

  s->SetBranchAddress("phiW1", &ev.phiW1);
  s->SetBranchAddress("etaW1", &ev.etaW1);
  s->SetBranchAddress("thetaW1", &ev.thetaW1);
  s->SetBranchAddress("ptW1", &ev.ptW1);
  s->SetBranchAddress("pW1", &ev.pW1);
  s->SetBranchAddress("mW1", &ev.mW1);
  
  s->SetBranchAddress("phiW2", &ev.phiW2);
  s->SetBranchAddress("etaW2", &ev.etaW2);
  s->SetBranchAddress("thetaW2", &ev.thetaW2);
  s->SetBranchAddress("ptW2", &ev.ptW2);
  s->SetBranchAddress("pW2", &ev.pW2);
  s->SetBranchAddress("mW2", &ev.mW2);


  s->SetBranchAddress("phib1", &ev.phib1);
  s->SetBranchAddress("etab1", &ev.etab1);
  s->SetBranchAddress("thetab1", &ev.thetab1);
  s->SetBranchAddress("ptb1", &ev.ptb1);
  s->SetBranchAddress("pb1", &ev.pb1);
  s->SetBranchAddress("mb1", &ev.mb1);
  
  s->SetBranchAddress("phib2", &ev.phib2);
  s->SetBranchAddress("etab2", &ev.etab2);
  s->SetBranchAddress("thetab2", &ev.thetab2);
  s->SetBranchAddress("ptb2", &ev.ptb2);
  s->SetBranchAddress("pb2", &ev.pb2);
  s->SetBranchAddress("mb2", &ev.mb2);

  s->SetBranchAddress("philep1", &ev.philep1);
  s->SetBranchAddress("etalep1", &ev.etalep1);
  s->SetBranchAddress("thetalep1", &ev.thetalep1);
  s->SetBranchAddress("ptlep1", &ev.ptlep1);
  s->SetBranchAddress("plep1", &ev.plep1);
  s->SetBranchAddress("mlep1", &ev.mlep1);

  s->SetBranchAddress("philep2", &ev.philep2);
  s->SetBranchAddress("etalep2", &ev.etalep2);
  s->SetBranchAddress("thetalep2", &ev.thetalep2);
  s->SetBranchAddress("ptlep2", &ev.ptlep2);
  s->SetBranchAddress("plep2", &ev.plep2);
  s->SetBranchAddress("mlep2", &ev.mlep2);


  s->SetBranchAddress("phinu1", &ev.phinu1);
  s->SetBranchAddress("etanu1", &ev.etanu1);
  s->SetBranchAddress("thetanu1", &ev.thetanu1);
  s->SetBranchAddress("ptnu1", &ev.ptnu1);
  s->SetBranchAddress("pnu1", &ev.pnu1);
  s->SetBranchAddress("mnu1", &ev.mnu1);

  s->SetBranchAddress("phinu2", &ev.phinu2);
  s->SetBranchAddress("etanu2", &ev.etanu2);
  s->SetBranchAddress("thetanu2", &ev.thetanu2);
  s->SetBranchAddress("ptnu2", &ev.ptnu2);
  s->SetBranchAddress("pnu2", &ev.pnu2);
  s->SetBranchAddress("mnu2", &ev.mnu2);

  s->SetBranchAddress("phiMET", &ev.phiMET);
  s->SetBranchAddress("etaMET", &ev.etaMET);
  s->SetBranchAddress("thetaMET", &ev.thetaMET);
  s->SetBranchAddress("ptMET", &ev.ptMET);
  s->SetBranchAddress("pMET", &ev.pMET);
  s->SetBranchAddress("mMET", &ev.mMET);
  
  s->SetBranchAddress("phiMETNu", &ev.phiMETNu);
  s->SetBranchAddress("etaMETNu", &ev.etaMETNu);
  s->SetBranchAddress("thetaMETNu", &ev.thetaMETNu);
  s->SetBranchAddress("ptMETNu", &ev.ptMETNu);
  s->SetBranchAddress("pMETNu", &ev.pMETNu);
  s->SetBranchAddress("mMETNu", &ev.mMETNu);
  
  s->SetBranchAddress("phiJets", &ev.phiJets);
  s->SetBranchAddress("etaJets", &ev.etaJets);
  s->SetBranchAddress("thetaJets", &ev.thetaJets);
  s->SetBranchAddress("ptJets", &ev.ptJets);
  s->SetBranchAddress("pJets", &ev.pJets);
  s->SetBranchAddress("mJets", &ev.mJets);

  s->SetBranchAddress("jzb", &ev.jzb);


}


