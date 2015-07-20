#ifndef AnalysisWZ_h
#define AnalysisWZ_h

#include "AnalysisBase.h"

#include <TH1.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <fstream>
#include <iostream>


class AnalysisWZ : public AnalysisBase
{
 public :

 AnalysisWZ(TTree *tree=0) : AnalysisBase(tree) {}

  float MuonIsolation    (int     k);
  float ElectronIsolation(int     k);
  bool  IsFiducialLepton (int     k);
  bool  IsTightLepton    (int     k);
  bool  IsIsolatedLepton (int     k);
  void  FillHistograms   (int     ichannel,
			  int     icut);
  void  Loop             (TString sample);
  void  Summary          (TString precision,
			  TString title);
};

#endif
