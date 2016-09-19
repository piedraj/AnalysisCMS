#ifndef MassVariations_class_h
#define MassVariations_class_h
#include "TLorentzVector.h"
#include "TVector2.h"
#include "../src/MassSolver.cc"
#include "TRandom3.h"

#include<iostream>
#include <vector>

using namespace std;

class MassVariations 
{
    public:
  
        MassVariations();
        ~MassVariations();

        void performAllVariations(Int_t, Int_t, Int_t, TLorentzVector &, TLorentzVector &, std::vector<TLorentzVector> &, std::vector<Float_t> &, TVector2 &, 
                             std::vector<TLorentzVector>&, std::vector<TLorentzVector> &nu2);
  
    private: 

        MassSolver *massSolver;
        TRandom3   *random;
  
        int variations(Int_t, Int_t, Int_t, TLorentzVector &, TLorentzVector &, TLorentzVector &, TLorentzVector &, Float_t unc1, Float_t unc2, TVector2 &,
                             std::vector<TLorentzVector>&, std::vector<TLorentzVector> &nu2);


}; //end class MassVariations  

#endif

#ifndef PI
#define PI fabs(acos(-1.))
#endif
