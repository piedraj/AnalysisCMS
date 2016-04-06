#ifndef AnalysisFR_h
#define AnalysisFR_h

#include "AnalysisCMS.h"

class AnalysisFR: public AnalysisCMS
{
 public :

  AnalysisFR(TTree* tree = 0);

  void FillAnalysisHistograms(int     ichannel,
			      int     icut,
			      int     ijet);

  void FillLevelHistograms   (int     icut,
			      bool    pass);

  void Loop                  (TString analysis,
			      TString sample,
			      float   luminosity);

  // Data members                                                                                                                                          
  //----------------------------------------------------------------------------                                                                            

  Lepton lep;

  // Analysis histograms                                                                                                                                    
  //----------------------------------------------------------------------------                                                                            
 
  TH1D* h_loose [nchannel][ncut][njetbin+1];  
  TH1D* h_tight [nchannel][ncut][njetbin+1];

};

#endif 
