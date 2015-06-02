#ifndef AnalysisWZ_h
#define AnalysisWZ_h

#include "AnalysisBase.h"

#include <TH1.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>


class AnalysisWZ : public AnalysisBase
{
 public :

  void Loop             ();
  bool IsFiducialLepton (int k);
  bool IsTightLepton    (int k);
  bool IsIsolatedLepton (int k);
  void FillHistograms   (int ichannel, int icut);
  void Summary          ();

};

#endif
