#define CedricGen_cxx
#include "CedricGen.h"
#include "include/Constants.h"
#include <TH2.h>
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

Lepton               ZLeptonGen1;
Lepton               ZLeptonGen2;
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

void CedricGen::Loop()
{

  
//   In a ROOT session, you can do:
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

  TFile* root_output           = new TFile("output.root","recreate");
  TH1D* h_GEN_m2l              = new TH1D("h_m2lGen","",       50, 0, 200);
  TH1D* h_GEN_m2l_Deltar = new TH1D("h_m2lGenMatch","",  50, 0, 200);
  TH1D* h_RECO_m2l             = new TH1D("h_m2lReco","",      50, 0, 200);


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

     for (int i=0;i<_nleptonGen;i++){
       
   	for (int j=i+1;j<_nleptonGen;j++){

   	  if (AnalysisLeptonsGen[i].flavour + AnalysisLeptonsGen[j].flavour !=0) continue; 

	  _m2lGen=-999;
   	  inv_massGen = (AnalysisLeptonsGen[i].v + AnalysisLeptonsGen[j].v).M();

   	  if (_m2lGen<0 || fabs(inv_massGen - Z_MASS) < fabs(_m2lGen - Z_MASS)) {

   	    _m2lGen=inv_massGen;
   	    ZLeptonGen1=AnalysisLeptonsGen[i];
   	    ZLeptonGen2=AnalysisLeptonsGen[j];

	    h_GEN_m2l->Fill(_m2lGen);

	    // Now I consider the Reco leptons

	    for (int k=0;k<_nlepton;k++){

	      for (int l=k+1; l<_nlepton;l++){

		if (AnalysisLeptons[k].flavour + AnalysisLeptons[l].flavour !=0) continue;

		_m2l=-999;
		inv_mass= (AnalysisLeptons[k].v + AnalysisLeptons[l].v).M();

		if (_m2l<0 || fabs(inv_mass - Z_MASS) < fabs(_m2l - Z_MASS)) {

		  _m2l=inv_mass;
		  ZLepton1=AnalysisLeptons[k];
		  ZLepton2=AnalysisLeptons[l];

		  h_RECO_m2l->Fill(_m2l);
		  
		  // Draw the diagrams with the desired cuts

		  if (ZLepton1.v.DeltaR(ZLeptonGen1.v) < 0.1 && ZLepton2.v.DeltaR(ZLeptonGen2.v) <0.1)){
		  
		    h_GEN_m2l_Deltar->Fill(_m2lGen);

		  }

		}
	      }
	    }
	  }
	}
     }
   }

     root_output->Write("");
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
	lepGen.flavour = std_vector_leptonGen_pid -> at(i);
	
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








 

 
