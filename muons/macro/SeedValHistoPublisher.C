#include <vector>
#include <algorithm>
#include "TMath.h"


//Uncomment the following line to get some more output
//#define DEBUG 1

void SeedValHistoPublisher(char* newFile=(char*)"NEW_FILE",char* refFile=(char*)"REF_FILE") {
  cout << ">> Starting SeedValHistoPublisher(" << newFile << "," << refFile << ")..." << endl;

  //====  To be replaced from python ====================
  
  const char* dataType = "DATATYPE";
  const char* refLabel("REF_LABEL, REF_RELEASE REFSELECTION");
  const char* newLabel("NEW_LABEL, NEW_RELEASE NEWSELECTION");

  string stringreffile="REF_FILE";
  string stringnewfile="NEW_FILE";
  std::string base_reffile = stringreffile.substr(stringreffile.find_last_of("/\\") + 1);
  cout << base_reffile.c_str() << endl;
  std::string base_newfile = stringnewfile.substr(stringnewfile.find_last_of("/\\") + 1);
  cout << base_newfile.c_str() << endl;
  const char* refFileLabel=base_reffile.c_str();
  const char* newFileLabel=base_newfile.c_str();

  // ==== Initial settings and loads
  //gROOT->ProcessLine(".x HistoCompare_Tracks.C");
  //gROOT ->Reset();
  gROOT ->SetBatch();

  R__LOAD_LIBRARY(macro/MuonStyle.C);
  R__LOAD_LIBRARY(macro/PlotHelpers.C);

  gErrorIgnoreLevel = kWarning; // Get rid of the info messages

  PlotHelpers testt;
  testt.SetGlobalStyle();


  // ==== Some cleaning... is this needed?  
  delete gROOT->GetListOfFiles()->FindObject(refFile);
  delete gROOT->GetListOfFiles()->FindObject(newFile); 
  


  // ==== Opening files, moving to the right branch and getting the list of sub-branches
  cout << ">> Opening file, moving to the right branch and getting sub-branches" << endl;

  TFile* sfile = new TFile(newFile);
  TList* sl = testt.getListOfBranches(dataType, sfile, "Seeds");
  if (!sl) {
    cout << "ERROR: Could not find keys!!!" << endl;
    cerr << "ERROR: Could not find keys!!!" << endl;
    return;
  }
  TDirectory*  sdir  = gDirectory;
  for (unsigned int i = 0; i < sl->GetEntries(); i++)
    cout << "   + " << sl->At(i)->GetName() << endl;
    
  cout << ">> Finding references..." << endl;
  TFile* rfile = new TFile(refFile);
  TList* rl = testt.getListOfBranches(dataType, rfile, "Seeds");
  if (!rl) {
    cout << "ERROR: Could not find keys!!!" << endl;
    cerr << "ERROR: Could not find keys!!!" << endl;
    return;
  }
  TDirectory* rdir  = gDirectory;
  for (unsigned int i = 0; i < sl->GetEntries(); i++)
    cout << "   + " << sl->At(i)->GetName() << endl;



 
 

  //==== Get the number of events for the normalization:
  cout << ">> Find out number of events for normalization..." << endl;
  TH1F *sevt, *revt;
  sdir->GetObject("RecoMuonV/RecoMuon_TrackAssoc/Muons/NMuon",sevt);
  rdir->GetObject("RecoMuonV/RecoMuon_TrackAssoc/Muons/NMuon",revt);

  /*  if (sevt && revt) {
    if (revt->GetEntries()>0) 
      norm = sevt->GetEntries()/revt->GetEntries();
  }
  else {  
    cerr << "WARNING: Missing seed normalization histos" << endl; 
    cout << "WARNING: Missing seed normalization histos" << endl; 
  }
  cout << "   + NORM = " << norm << endl;
  */

  //==== Iterate now over histograms and collections
  cout << ">> Iterating over histograms and collections..." << endl;
  TIter iter_r( rl );
  TIter iter_s( sl );
  TKey* rKey = 0;
  TKey* sKey = 0;
  TString rcollname;
  TString scollname;

  rKey = (TKey*)iter_r();

  while (rKey) {
    TString myName = rKey->GetName();
#ifdef DEBUG
    cout << "DEBUG: Checking key " << myName << endl;
#endif
    rcollname = myName;
    sKey = (TKey*)iter_s();
    if (!sKey) continue;
    scollname = sKey->GetName();
    if ( (rcollname != scollname) && (rcollname+"FS" != scollname) && (rcollname != scollname+"FS") ) {
      cerr << "ERROR: Different collection names, please check: " << rcollname << " : " << scollname << endl;
      cout << "ERROR: Different collection names, please check: " << rcollname << " : " << scollname << endl;
      continue;
    }
  
    // ==== Now let's go for the plotting...
    cout << ">> Comparing plots in " << myName << "..." << endl;    
    cerr << ">> Comparing plots in " << myName << "..." << endl;    
    TString newDir("NEW_RELEASE/NEWSELECTION/NEW_LABEL/");
    newDir+=myName;
    gSystem->mkdir(newDir,kTRUE);

 
    bool    logy    [] = {false,   true,   false,      true    };
    bool    doKolmo [] = {true,    true,   true,       true    };


    //===== muon seeds plots, first page:
    const char* plots1 [] = {"seedEta_", "seedEtaErr_", "seedPhi_", "seedPhiErr_"};
    const char* plotsl1[] = {"seed #eta", "seed #eta error", "seed #phi", "seed #phi error"};
    Double_t norm_1   [] = {0.,0.,0.,0.}; 
    testt.Plot4Histograms(newDir + "/muonSeed1.pdf",
		    rdir, sdir, 
		    rcollname, scollname,
		    "Seeds1", "Seeds eta and phi",
		    refLabel, newLabel,
		    refFileLabel, newFileLabel,
		    plots1, plotsl1,
		    logy, doKolmo, norm_1);
 
    bool    logy2   [] = {false, true, false, true, false, true};
    bool    doKolmo2[] = {true,  true, true,  true, true,  true};
    const char* plots2  [] = {"seedPt_", "seedPtErrOverPt_", "seedPz_", "seedPzErrOverPz_",
			      "NumberOfRecHitsPerSeed_", "seedPErrOverP_"};
    const char* plotsl2 [] = {"seed P_{T}", "seed P_{T} Err/P_{T}", "seed P_{Z}", "seed P_{Z} Err/P_{Z}",
			      "Nr RecHits per seed", "seed P Err/P"};
    Double_t norm_2   [] = {0.,0.,0.,0.,0.,0.}; 
    testt.Plot6Histograms(newDir + "/muonSeed2.pdf",
		    rdir, sdir, 
		    rcollname, scollname,
		    "Seeds2", "Seeds momenta and hits",
		    refLabel, newLabel,
		    refFileLabel, newFileLabel,
		    plots2, plotsl2,
		    logy2, doKolmo2, norm_2);
   
 
    //// Merge pdf histograms together into larger files, and name them based on the collection names
    TString mergefile = "merged_seed.pdf"; // File name where partial pdfs will be merged
    TString destfile  = newDir + "/../" + myName + ".pdf"; // Destination file name
    TString gscommand = "gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=" + mergefile + " "
      + newDir + "/muonSeed1.pdf "
      + newDir + "/muonSeed2.pdf ";
      //      + newDir + "/muonSeed3.pdf ";
    cout << ">> Merging partial pdfs to " << mergefile << "..." << endl;
#ifdef DEBUG
    cout << "DEBUG: ...with command \"" << gscommand << "\"" << endl;
#endif
    gSystem->Exec(gscommand);
    cout << ">> Moving " << mergefile << " to " << destfile << "..." << endl;
    gSystem->Rename(mergefile, destfile);
    
    cout << ">> Deleting partial pdf files" << endl;
    gSystem->Exec("rm -r "+newDir);
    cout << "   ... Done" << endl;

    rKey = (TKey*)iter_r();    
  }  // end of "while loop"
  
  cout << ">> Removing the relval files from ROOT before closing..." << endl;
  gROOT->GetListOfFiles()->Remove(sfile);
  gROOT->GetListOfFiles()->Remove(rfile);

#ifdef DEBUG
  cout << "DEBUG: Exiting!" << endl;
  cerr << "DEBUG: Exiting!" << endl;
#endif
}
