#ifndef AnalysisWZ_h
#define AnalysisWZ_h

#include "AnalysisBase.h"

#include <fstream>
#include <iostream>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>


class AnalysisWZ : public AnalysisBase
{
 public :

 AnalysisWZ(TTree *tree=0) : AnalysisBase(tree) {}

  float   MuonIsolation    (int     k);

  float   ElectronIsolation(int     k);

  bool    IsFiducialLepton (int     k);

  bool    IsTightLepton    (int     k);

  bool    IsIsolatedLepton (int     k);

  void    FillHistograms   (int     ichannel,
			    int     icut);

  void    Loop             (TString sample,
			    TString era,
			    float   luminosity);

  void    Summary          (TString precision,
			    TString title);

  void    GetSampleName    (TString filename);

  void    ApplyWeights     (TString sample,
			    TString era);
};

#endif
