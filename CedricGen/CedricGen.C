#define CedricGen_cxx
#include "CedricGen.h"
#include "../include/Constants.h"
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>
#include <TTree.h>
#include <TStyle.h>
#include <TString.h>
#include <TLorentzVector.h>



struct Lepton
{
  int                index;
  int                flavour;
  TLorentzVector     v;
};


std::vector<Lepton>  AnalysisLeptons;
std::vector<Lepton>  AnalysisLeptonsGen;

Lepton               WLeptonGen;
Lepton               ZLeptonGen1;
Lepton               ZLeptonGen2;
Lepton               WLepton;
Lepton               ZLepton1; 
Lepton               ZLepton2;

int                  _nlepton; 
int                  _nleptonGen;
int                  vector_leptonGen_size;
int                  vector_lepton_size;

float                inv_massGen;
float                _m2lGen;
float                inv_mass;
float                _m2l;

TH1D*               h_GEN_m2l;
TH1D*               h_GEN_m2l_DeltaR;
TH1D*               h_RECO_m2l;
TH1D*               h_GEN_wlzl1deltar;
TH1D*               h_GEN_wlzl2deltar;
TH1D*               h_GEN_zl1zl2deltar;
TH1D*               h_GEN_zl1pt;
TH1D*               h_GEN_zl2pt;
TH1D*               h_GEN_wlpt;
TH1D*               h_GEN_met;
TH1D*               h_RECO_met;
TH1D*               h_met_comparison;
TH1D*               h_GEN_m3l;
TH1D*               h_RECO_m3l;
TH1D*               h_GEN_m3l_DeltaR;

TFile*              root_output;



void CedricGen::Loop()
{
 root_output            = new TFile("output.root", "recreate");

 h_GEN_m2l              = new TH1D("h_GEN_m2l",         "m2l of the Gen Z leptons",                                             100, 0, 150);
 h_GEN_m2l_DeltaR       = new TH1D("h_GEN_m2l_DeltaR",  "m2l of the Gen Z leptons with #Delta R matching",                      100, 0, 150);
 h_RECO_m2l             = new TH1D("h_RECO_m2l",        "m2l of the Reco Z leptons",                                            100, 0, 150);

 h_GEN_zl1pt            = new TH1D("h_GEN_zl1pt","Pt of the leading Z Gen lepton",                                               75, 0, 100);
 h_GEN_zl2pt            = new TH1D("h_GEN_zl2pt","Pt of the trailing Z Gen lepton",                                              75, 0, 100);
 h_GEN_wlpt             = new TH1D("h_GEN_wlpt", "Pt of the W Gen lepton",                                                       75, 0, 100);

 h_GEN_met              = new TH1D("h_GEN_met",         "Met of the gen leptons",                                                50, 0,  80);
 h_RECO_met             = new TH1D("h_RECO_met",        "Met of the reco leptons",                                               50, 0,  80);
 h_met_comparison       = new TH1D("h_met_comparison",  "Met of the reco leptons - met of the gen leptons",                      50, 0,  80);
  
 h_GEN_wlzl1deltar      = new TH1D("h_GEN_wlzl1deltar",   "#Delta R between the W Gen and the trailing lepton of the Z Gen",    100, 0,   5);
 h_GEN_wlzl2deltar      = new TH1D("h_GEN_wlzl2deltar",   "#Delta R between the W Gen and the leading lepton of the Z Gen",     100, 0,   5);
 h_GEN_zl1zl2deltar     = new TH1D("h_GEN_zl1zl2deltar",  "#Delta R between both Z Gen leptons",                                100, 0,   5);
 

//   In A ROOT session, you can do:
//      root> .L CedricGen.C
//      root> CedricGen t
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


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

   GetLeptonsGen();
   GetLeptons();

   // To print the value of the progress of the compilation 
   double progress = 1e2 * (jentry+1)/nentries;
   double fractpart, intpart;
   fractpart = modf(progress, &intpart);
   
   if (fractpart < 1e-2)
     {
       std::cout << "progress : " << int(ceil(progress)) << "%\r";
       std::cout.flush();
     }

   WZSelection();

   //Fill the m2l histograms

   h_GEN_m2l                      ->    Fill(_m2lGen);
   h_RECO_m2l                     ->    Fill(_m2l);

   if (ZLepton1.v.DeltaR(ZLeptonGen1.v) < 0.1 && ZLepton2.v.DeltaR(ZLeptonGen2.v) < 0.1){
     h_GEN_m2l_DeltaR             ->    Fill(_m2lGen);
   }

   //Fill the pt histograms

   h_GEN_zl1pt                    ->    Fill(ZLeptonGen1.v.Pt());
   h_GEN_zl2pt                    ->    Fill(ZLeptonGen2.v.Pt());
   h_GEN_wlpt                     ->    Fill(WLeptonGen.v.Pt());


   //Fill the met histograms

   h_GEN_met                      ->    Fill(metGenpt);
   h_RECO_met                     ->    Fill(pfType1Met);
   h_met_comparison               ->    Fill(metGenpt-pfType1Met);

}
   
   
   root_output->cd();
   root_output->Write("",TObject::kOverwrite);
   root_output->Close();
}




// ============================================================
// Getleptonsgen
// ============================================================

void CedricGen::GetLeptonsGen()
{
  
  AnalysisLeptonsGen.clear();
  
    int vector_leptonGen_size = std_vector_leptonGen_pid->size(); 

   for (int i=0;i<vector_leptonGen_size;i++) {

   float ptGen  = std_vector_leptonGen_pt  -> at(i);
   float etaGen = std_vector_leptonGen_eta -> at(i);
   float phiGen = std_vector_leptonGen_phi -> at(i);

   Lepton lepGen;
        
	lepGen.index   = i;
	lepGen.flavour = std_vector_leptonGen_pid  -> at(i);

        float massGen = -999;

	if(abs(lepGen.flavour) == ELECTRON_FLAVOUR)
	{
	  massGen=ELECTRON_MASS;
	}  
	else if(abs(lepGen.flavour) == MUON_FLAVOUR)
	{
	  massGen=MUON_MASS;
	}

	TLorentzVector tlvGen;  

	tlvGen.SetPtEtaPhiM(ptGen, etaGen, phiGen, massGen);
	lepGen.v = tlvGen;
   	  
  AnalysisLeptonsGen.push_back(lepGen);
   }
   _nleptonGen = AnalysisLeptonsGen.size();

}	  




// ============================================================
// GetLeptons
// ============================================================


void CedricGen::GetLeptons()
{

  
  AnalysisLeptons.clear();

  int vector_lepton_size = std_vector_lepton_pt->size();
  
   for (int i=0;i<vector_lepton_size;i++) {

   float pt  = std_vector_lepton_pt  -> at(i);
   float eta = std_vector_lepton_eta -> at(i);
   float phi = std_vector_lepton_phi -> at(i);

   Lepton lep;
        
	lep.index   = i;
	lep.flavour = std_vector_lepton_flavour -> at(i);
	
   float mass = -999;

   if(abs(lep.flavour) == ELECTRON_FLAVOUR)
	  {
	    mass=ELECTRON_MASS;
	  }  
	else if(abs(lep.flavour) == MUON_FLAVOUR)
          {
            mass=MUON_MASS;
	  }

   TLorentzVector tlv;  

	  tlv.SetPtEtaPhiM(pt, eta, phi, mass);
	  lep.v = tlv;
	  
  AnalysisLeptons.push_back(lep);
  }	  

   _nlepton = AnalysisLeptons.size();   
}



// ===========================================================
// WZ Selection
// ===========================================================

void CedricGen::WZSelection(){

  _m2lGen=-999;
  _m2l=-999;

     for (int i=0;i<_nleptonGen;i++){

       for (int j=i+1;j<_nleptonGen;j++){

	 float Lepton1_mpid = std_vector_leptonGen_mpid -> at(i);
	 float Lepton2_mpid = std_vector_leptonGen_mpid -> at(j);

	 if(fabs(Lepton1_mpid) == 24){
	   WLeptonGen=AnalysisLeptonsGen[i];
	 }

	 else if(fabs(Lepton2_mpid) == 24){
	   WLeptonGen=AnalysisLeptonsGen[j];
	 }

	 if (Lepton1_mpid == 23 && Lepton2_mpid  == 23){
	   inv_massGen = (AnalysisLeptonsGen[i].v + AnalysisLeptonsGen[j].v).M();

	  if (_m2lGen<0 || fabs(inv_massGen - Z_MASS) < fabs(_m2lGen - Z_MASS)) {
	    _m2lGen=inv_massGen;	   
	  }

	    ZLeptonGen1=AnalysisLeptonsGen[i];
	    ZLeptonGen2=AnalysisLeptonsGen[j];

	    for (int k=0;k<_nlepton;k++){

	       for (int l=k+1; l<_nlepton;l++){

		 if (AnalysisLeptons[k].flavour + AnalysisLeptons[l].flavour !=0) continue;

		 inv_mass= (AnalysisLeptons[k].v + AnalysisLeptons[l].v).M();

		 if (_m2l<0 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

		   _m2l=inv_mass;
		   ZLepton1=AnalysisLeptons[k];
		   ZLepton2=AnalysisLeptons[l];

		   for (int z=0; z<3; z++){
		     if (z==k) continue;
		     if (z==l) continue;
		     WLepton=AnalysisLeptons[z];
		   }

		 }
	       }
	    }
	 }
       }
     }
}





