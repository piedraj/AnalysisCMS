#include "TLorentzVector.h"
#include "TVector2.h"
#include "TRandom3.h"
#include "../interface/MassVariations.h"

#include<iostream>
#include <vector>

#define debug 0


MassVariations::MassVariations() {

    massSolver = new MassSolver();
    random = new TRandom3();

}


MassVariations::~MassVariations() {

    delete massSolver;
    delete random;

}


void MassVariations::performAllVariations(Int_t NtoysMET, Int_t NtoysJec, Int_t NtoysMassTop, Int_t NtoysMassW, TLorentzVector & lep1, TLorentzVector & lep2, 
                                     std::vector<TLorentzVector> & jets, std::vector<Float_t> & unc, TVector2 & MET, std::vector<TLorentzVector> &nu1, std::vector<TLorentzVector> &nu2) {

    std::vector<Float_t>::iterator unc1 = unc.begin();
    for(std::vector<TLorentzVector>::iterator jet1 = jets.begin(); jet1 != jets.end(); jet1++, unc1++) {    

        std::vector<Float_t>::iterator unc2 = unc.begin();
        for(std::vector<TLorentzVector>::iterator jet2 = jets.begin(); jet2 != jets.end(); jet2++, unc2++) {

            if(jet1 == jet2) continue;     
            
            if( variations(NtoysMET, NtoysJec, NtoysMassTop, NtoysMassW, lep1, lep2, *jet1, *jet2, *unc1, *unc2, MET, nu1, nu2) ) return;

  
        }

    }  

}


int MassVariations::variations(Int_t NtoysMET, Int_t NtoysJec, Int_t NtoysMassTop, Int_t NtoysMassW, TLorentzVector & lep1, TLorentzVector & lep2, TLorentzVector & jet1, TLorentzVector & jet2, 
                                Float_t unc1, Float_t unc2, TVector2 &MET, std::vector<TLorentzVector> &nu1, std::vector<TLorentzVector> &nu2) {



    if(debug == 1) {
        std::cout << "[MassVariation] Starting combination:" << std::endl;;
        std::cout << "[MassVariation] Lepton 1 "; lep1.Print();
        std::cout << "[MassVariation] Jet 1 "; jet1.Print();
        std::cout << "[MassVariation] Lepton 2 "; lep2.Print();
        std::cout << "[MassVariation] Jet 2 "; jet2.Print();
        std::cout << "[MassVariation] MET "; MET.Print();
    }

    for(Int_t masswcounter = 0; masswcounter < NtoysMassW; masswcounter++) {
    
        for(Int_t masstopcounter = 0; masstopcounter < NtoysMassTop; masstopcounter++) {

            for(Int_t massjetcounter = 0; massjetcounter < NtoysJec; massjetcounter++) {

		for(Int_t METcounter = 0; METcounter < NtoysMET; METcounter++) {

		        Float_t theMassW1   = random->BreitWigner(80.385, 0.015);
		        Float_t theMassTop1 = random->BreitWigner(173.34, 1);
		        Float_t theMassW2   = random->BreitWigner(80.385, 0.015);
		        Float_t theMassTop2 = random->BreitWigner(173.34, 1);
		        Float_t theUncJet1  = random->Gaus(0, unc1);
		        Float_t theUncJet2  = random->Gaus(0, unc2);

			Float_t theMETpT    = random->Gaus(MET.Mod(), 1.);
			Float_t theMETphi   = random->Gaus(MET.Phi(), 1.);

		        if(debug == 1) {
		            std::cout << "[MassVariation] Trying: M(W1): " << theMassW1 << " M(W2): " << theMassW2 << " M(top1): " << theMassTop1 << " M(top2): " << theMassTop2;
		            std::cout << " Unc(Jet 1): " << theUncJet1 << " Unc(Jet 2): " << theUncJet2 << std::endl;
		        }
		        
		        TLorentzVector newJet1(0,0,0,0);
		        TLorentzVector newJet2(0,0,0,0);
		        Float_t pCorrection1 = sqrt((jet1.E() + theUncJet1)*(jet1.E() + theUncJet1) - jet1.M()*jet1.M())/jet1.P();
		        Float_t pCorrection2 = sqrt((jet2.E() + theUncJet2)*(jet2.E() + theUncJet2) - jet2.M()*jet2.M())/jet2.P();
		        newJet1.SetPtEtaPhiM(jet1.Pt()*pCorrection1, jet1.Eta(), jet1.Phi(), jet1.M());
		        newJet2.SetPtEtaPhiM(jet2.Pt()*pCorrection2, jet2.Eta(), jet2.Phi(), jet2.M());

			TVector2 freeMET; 
			freeMET.SetMagPhi(theMETpT, theMETphi); 
		        
		        TVector2 DeltaP1(newJet1.Px() - jet1.Px(), newJet1.Py() - jet1.Py());
		        TVector2 DeltaP2(newJet2.Px() - jet2.Px(), newJet2.Py() - jet2.Py());
		        //TVector2 newMET = MET + DeltaP1 + DeltaP2;
			TVector2 newMET = freeMET + DeltaP1 + DeltaP2;

			nu1.clear(); 
			nu2.clear();		

		        massSolver->solve(newMET, newJet1, newJet2, lep1, lep2, theMassW1, theMassW2, theMassTop1, theMassTop2, nu1, nu2);

			//newMET.Print(); 

			//std::cout << nu1.size() << " -- " << nu2.size() << endl;  

		        if(nu1.size() != 0) return 1;

		}
           
            }

        }

    }

return 0;  

}
