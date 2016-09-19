#include "src/MassVariations.cc"
#include "TLorentzVector.h"
#include "TVector2.h"

#include <vector>



int main(int argc, char **argv) {

  MassVariations theMass;
 
  TLorentzVector l1(1, 1.01, 0, 0);
  TLorentzVector l2(1, -1.01, 0, 0);
  TLorentzVector j1(3, 3.01, 0, 0);
  TLorentzVector j2(4, 4.01, 0, 0);
  TLorentzVector j3(5, 5.01, 0, 0);
  TVector2 MET(50, 50);

  std::vector<TLorentzVector> nu1, nu2;
  std::vector<TLorentzVector> jets;
  jets.push_back(j1);
  jets.push_back(j2);
  jets.push_back(j3);
  std::vector<Float_t> unc;
  unc.push_back(1);
  unc.push_back(2);
  unc.push_back(3);
  theMass.performAllVariations(1, 1, 1, l1, l2, jets, unc, MET, nu1, nu2);


  return 1;

}
