#include <vector>
#include <algorithm>
#include "TMath.h"

//Uncomment the following line to get some more output
//#define DEBUG 1


void TrackValHistoPublisher(const char* newFile="NEW_FILE",const char* refFile="REF_FILE") {

  cout << ">> Starting TrackValHistoPublisher(" 
       << newFile << "," << refFile << ")..." << endl;

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

  Float_t maxPT;
  TString File = newFile;
  if (File.Contains("SingleMuPt1000") ||File.Contains("WpM")||File.Contains("ZpMM")   ) maxPT=1400.;
  else if (File.Contains("SingleMuPt100")) {maxPT = 400.;}
  else if(File.Contains("SingleMuPt10")) {maxPT = 70.;}
  else maxPT = 400.;

  bool ctf=1;

  bool resol=0;

  // ==== Some cleaning... is this needed?  
  delete gROOT->GetListOfFiles()->FindObject(refFile);
  delete gROOT->GetListOfFiles()->FindObject(newFile); 
  


  // ==== Opening files, moving to the right branch and getting the list of sub-branches
  cout << ">> Opening file, moving to the right branch and getting sub-branches" << endl;

  TFile * sfile = new TFile(newFile);
  TList* sl = testt.GetListOfBranches(dataType, sfile);
  if (!sl) {
    cout << "ERROR: Could not find keys!!!" << endl;
    cerr << "ERROR: Could not find keys!!!" << endl;
    return;
  }
  TDirectory*  sdir  = gDirectory;
  for (unsigned int i = 0; i < sl->GetEntries(); i++)
    cout << "   + " << sl->At(i)->GetName() << endl;

  cout << ">> Finding references..." << endl;
  TFile * rfile = new TFile(refFile);
  TList* rl = testt.GetListOfBranches(dataType, rfile);
  if (!rl) {
    cout << "ERROR: Could not find keys!!!" << endl;
    cerr << "ERROR: Could not find keys!!!" << endl;
    return;
  }
  TDirectory* rdir  = gDirectory;
  for (unsigned int i = 0; i < sl->GetEntries(); i++)
    cout << "   + " << sl->At(i)->GetName() << endl;
  



  

  //==== Find if the collection has muon associator or track associator
  cout << ">> Find if the collection has muon associator or track associator..." << endl;
  bool hasOnlyMuonAssociatorInRef = true;
  bool hasOnlyMuonAssociatorInSig = true;
  bool hasOnlyTrackAssociatorInRef = true;
  bool hasOnlyTrackAssociatorInSig = true;
  TIter iter_r0( rl );
  TIter iter_s0( sl );
  TKey* rKey = 0;
  TKey* sKey = 0;
 
  bool considerOnlyMuonAssociator = hasOnlyMuonAssociatorInRef || hasOnlyMuonAssociatorInSig;

  cout << "   + Has Only Muon Associator (reference):  " << hasOnlyMuonAssociatorInRef << endl;
  cout << "   + Has Only Muon Associator (signal):     " << hasOnlyMuonAssociatorInSig << endl;
  cout << "   + Has Only Track Associator (reference): " << hasOnlyTrackAssociatorInRef << endl;
  cout << "   + Has Only Track Associator (signal):    " << hasOnlyTrackAssociatorInSig << endl;
  cout << "   + Consider only Muon Associator:         " << considerOnlyMuonAssociator << endl;
 


  //==== Iterate now over histograms and collections
  cout << ">> Iterating over histograms and collections..." << endl;

  TIter iter_r( rl );
  TIter iter_s( sl );
  TString scollname;
  TString rcollname;
  
  TKey *myNext2=0;

  rKey = (TKey*)iter_r(); 
  while ( rKey ) { //while loop

    TString myName = rKey->GetName();
#ifdef DEBUG
    cout << "DEBUG: Checking key " << myName << endl;
#endif
    if (myName.Contains("SET")) continue;
    rcollname = myName;
    // ==> extractedGlobalMuons are in a different position wrt globalMuons:
    if (myNext2) {
      sKey = myNext2;
      myNext2 = 0;
    }
    else {
      sKey = (TKey*)iter_s();
    }
    if (!sKey) continue;
    TString myName2 = sKey->GetName();
   
    if (myName2 != myName) {
      myNext2 =  (TKey*)iter_s();
      TKey* myTemp = sKey; 
      sKey = myNext2;
      myName2 = sKey->GetName();
      myNext2 = myTemp;
      if(myName2 != myName)
	{myNext2 =  (TKey*)iter_s();
	  TKey* myTemp = sKey; 
	  sKey = myNext2;
	  myName2 = sKey->GetName();
	  myNext2 = myTemp;
	}
    }
    
    scollname=myName2;
#ifdef DEBUG
    cout << "DEBUG: Comparing " << rcollname << " and " << scollname << endl;
#endif
    if ( 
	(myName == myName2) || (myName+"FS" == myName2) || (myName == myName2+"FS" )
	|| (myName.Contains("extractedGlobalMuons") && myName2.Contains("globalMuons") )
	|| (myName.Contains("globalMuons") && myName2.Contains("extractedGlobalMuons") )
	 ) {
      rcollname = rKey->GetName();
      scollname = sKey->GetName();
    }
    else if ( (rcollname != scollname) && (rcollname+"FS" != scollname) && (rcollname != scollname+"FS") ) 
      {
	bool goodAsWell = false;
	if (rcollname.BeginsWith("StandAloneMuons_UpdAtVtx") && 
	    scollname.BeginsWith("StandAloneMuons_UpdAtVtx")) {
	  if (rcollname.Contains("MuonAssociation")==scollname.Contains("MuonAssociation"));
	  goodAsWell = true;
	}
	if (rcollname.BeginsWith("hltL2Muons_UpdAtVtx") && 
	    scollname.BeginsWith("hltL2Muons_UpdAtVtx")) {
	  if (rcollname.Contains("MuonAssociation")==scollname.Contains("MuonAssociation"));
	  goodAsWell = true;
	}
	if (rcollname.BeginsWith("hltL3Tk") && scollname.BeginsWith("hltL3Tk")) {
	  if (rcollname.Contains("MuonAssociation")==scollname.Contains("MuonAssociation"));
	  goodAsWell = true;
	}
	//     TString isGood = (goodAsWell? "good": "NOT good");
	//     cout << " -- The two collections: " << rcollname << " : " << scollname << " -> " << isGood << endl;
	if (! goodAsWell) {
	  
	  if (rcollname.Contains("SET") && !scollname.Contains("SET")) {
	    while (rcollname.Contains("SET")) {
	      rKey = (TKey*)iter_r();
	      if (rKey)  rcollname = rKey->GetName();
	    }
	  }
	  else if (scollname.Contains("SET") && !rcollname.Contains("SET")) {
	    while (scollname.Contains("SET")) {
	      sKey = (TKey*)iter_s();
	      if (sKey )  scollname = sKey->GetName();
	    }
	  }
	
	  if (rcollname.Contains("dyt") && !scollname.Contains("dyt")) {
	    while (rcollname.Contains("dyt")) {
	      rKey = (TKey*)iter_r();
	      if (rKey)  rcollname = rKey->GetName();
	    }
	  }
	  else if (scollname.Contains("dyt") && !rcollname.Contains("dyt")) {
	    while (scollname.Contains("dyt")) {
	      sKey = (TKey*)iter_s();
	      if (sKey)  scollname = sKey->GetName();
	    }
	  }
	  
	  if (rcollname.Contains("refitted") && !scollname.Contains("refitted")) {
	    while (rcollname.Contains("refitted")) {
	      rKey = (TKey*)iter_r();
	      if (rKey)  rcollname = rKey->GetName();
	    }
	  }
	  else if (scollname.Contains("refitted") && !rcollname.Contains("refitted")) {
	    while (scollname.Contains("refitted")) {
	      sKey = (TKey*)iter_s();
	      if (sKey )  scollname = sKey->GetName();
	    }
	  }
	  
	  if ( (rcollname != scollname) && 
	       (rcollname+"FS" != scollname) && 
	       (rcollname != scollname+"FS") ) {
	    cout << " Different collection names, please check: " << rcollname 
		 << " : " << scollname << endl;
	    continue;
	  }
	  else {
	    //	 cout << "    The NEW collections: " << rcollname << " : " << scollname << endl;
	    myName = rKey->GetName();
	  }
	}
      }

    // ==== Now let's go for the plotting...
    //    cout << ">> Comparing plots in " << myName << "..." << endl;    
    //cerr << ">> Comparing plots in " << myName << "..." << endl;    
    TString newDir("NEW_RELEASE/NEWSELECTION/NEW_LABEL/");
    newDir+=myName;
    gSystem->mkdir(newDir,kTRUE);
    
    bool     logyfalse  [] = {false,  false,  false,  false,   false,  false  };
    bool     doKolmo    [] = {true,   true,   true,  true,   true,   true   };
    //Double_t norm   [] = {-999.,-999.,-999.,-999.,-999.,-999.};
    Double_t minx   [] = {-1E100, -1E100, 5.,     5.,    -1E100, -1E100 };
    Double_t maxx   [] = {-1E100, -1E100, maxPT, maxPT,  -1E100, -1E100 };
    Double_t miny   [] = {0.5,    -1E100, 0,     -1E100, 0.5,    -1E100 };
    Double_t maxy   [] = {1.0125, -1E100, 1.025, -1E100, 1.0125, -1E100 };

    //////////////////////////////////////
    /////////// CTF //////////////////////
    //////////////////////////////////////


    if (ctf) {
      //===== building
      
      const char* plots1[] = {"effic", "fakerate", "efficPt", "fakeratePt"};
      const char* plotsl1[] = {"efficiency vs #eta", "fakerate vs #eta", "efficiency vs Pt", "fakerate vs Pt"};
      bool    logy [] = {false,  false, false,  false  };
      Double_t norm_1   [] = {-999.,-999.,-999.,-999.};
      testt.Plot4Histograms(newDir + "/building.pdf",
                      rdir, sdir, 
                      rcollname, scollname,
                      "Seeds", "Efficiency Vs Pt and Vs #eta",
                      refLabel, newLabel,
		      refFileLabel, newFileLabel,
                      plots1, plotsl1,
                      logy, doKolmo, norm_1,0,minx,maxx,miny,maxy);     
      
      const char* plots12[] = { "effic_vs_hit", "fakerate_vs_hit","effic_vs_phi","fakerate_vs_phi"};
      const char* plotsl12[] = { "efficiency vs hits", "fakerate vs hits","efficiency vs #phi","fakerate vs #phi"};

      //      bool    logy [] = {false,  false, false,  false  };
      Double_t norm_2   [] = {-999.,-999.,-999.,-999.};
      testt.Plot4Histograms(newDir + "/building2.pdf",
                      rdir, sdir, 
                      rcollname, scollname,
                      "Seeds2", "Efficiency vs hits and #phi",
                      refLabel, newLabel,
		      refFileLabel, newFileLabel,
                      plots12, plotsl12,
                      logy, doKolmo, norm_2, 0,minx,maxx,miny,maxy);     

      // ====== hits and pt
      const char* plots2  [] = { "hits_eta", "hits", "num_simul_pT","num_reco_pT"};
      const char* plotsl2  [] = { "nhits vs eta", "number of hits per track", "Number of Pt Simulated","Pt of Reco tracks"};
            Double_t norm2_1 [] = {-999.,-999.,0.,0.};
      
      testt.Plot4Histograms(newDir + "/hitsAndPt.pdf",
                      rdir, sdir, 
                      rcollname, scollname,
                      "hits", "Pt",
                      refLabel, newLabel,
		      refFileLabel, newFileLabel,
                      plots2, plotsl2,
                      logyfalse, doKolmo, norm2_1,0,minx,maxx);
      
    
      //===== tuning
      bool    logy3    [] = {false,   true,   false,      true    };
      bool    doKolmo3 [] = {true,    true,   true,       true    };
      const char* plots3 [] = {"chi2","chi2_prob","chi2mean", "ptres_vs_eta_Mean"};
      Double_t norm2_2 [] = {-1.,-1.,-999.,-999.};
      testt.Plot4Histograms(newDir + "/tuning.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
		      "IsoHistos2", "HCAL, HO Deposits",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots3, 0,
		      logy3, doKolmo3,norm2_2,0);
      
      
      
      
      
      //===== pulls
      const char* plots4  [] = {"pullPt", "pullQoverp", "pullPhi", "pullTheta", "pullDxy","pullDz"};
      const char* plotsl4  [] = {"Pt Pull", "Q/P Pull", "#Phi Pull", "#theta Pull", "Dxy Pull","Dz Pull"};
      Double_t norm2_3 [] = {-1.,-1.,-1.,-1.,-1.,-1.};
      testt.Plot6Histograms(newDir + "/Pulls.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
		      "Pullsdis", "Pull Distributions",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots4, plotsl4,
		      logyfalse, doKolmo, norm2_3,0);     
      
      
      
      
      
      //===== residuals
      const char* plots5  [] = {"ptres_vs_eta", "etares_vs_eta", "phires_vs_eta", "cotThetares_vs_eta", "dxyres_vs_eta","dzres_vs_eta"};
      const char* plotsl5  [] = {"p_{t} resolution", "#eta resolution", "#Phi resolution", "cot(#theta) resolution", "Dxy resolution","Dz resolution"};

      resol= true;
      Double_t norm2_4 [] = {-1.,-1.,-1.,-1.,-1.,-1.};
      testt.Plot6Histograms(newDir + "/residuals.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
		      "residualdis", "residuals vs Pt",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots5, plotsl5,
		      logyfalse, doKolmo, norm2_4,resol);    
      
      
      
      
      //===== resolutions vs eta
      const char* plots6  [] = {"phires_vs_eta_Sigma", "cotThetares_vs_eta_Sigma", "dxyres_vs_eta_Sigma", "dzres_vs_eta_Sigma", "ptres_vs_eta_Sigma"};
      const char* plotsl6  [] = {"#sigma(#delta #phi) vs #eta", "#sigma(#delta cot(#theta)) vs #eta", "#sigma(#delta d_{0}) vs #eta", "#sigma(#delta z_{0}) vs #eta", "#sigma(#delta p_{t}/p_{t}) vs #eta"};
      Double_t norm_3   [] = {-999.,-999.,-999.,-999.,-999.,-999.};
      testt.Plot5Histograms(newDir + "/residualsEta.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
	 	      "residualsdisEta", "residuals vs Eta",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots6, plotsl6,
		      logyfalse, doKolmo, norm_3,0);     
      
      
      //
      //===== mean values vs eta
      const char* plots7   [] = {"phires_vs_eta_Mean", "cotThetares_vs_eta_Mean", "dxyres_vs_eta_Mean", "dzres_vs_eta_Mean", "ptres_vs_eta_Mean"};
      const char* plotsl7  [] = {"#delta #phi vs #eta", "#delta cot(#theta) vs #eta", "#delta d_{0} vs #eta", "#delta z_{0} ve #eta", "#delta p_{t}/p_{t} vs #eta"};
      Double_t norm_4   [] = {-999.,-999.,-999.,-999.,-999.,-999.};
      testt.Plot5Histograms(newDir + "/meanvaluesEta.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
		      "meanvaluesEtadis", "mean values vs eta",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots7, plotsl7,
		      logyfalse, doKolmo,norm_4,0);     
      
      
      
      
      //
      //===== resolutions vs pt
      //
      const char* plots8  [] = {"phires_vs_pt_Sigma", "cotThetares_vs_pt_Sigma", "dxyres_vs_pt_Sigma", "dzres_vs_pt_Sigma", "ptres_vs_pt_Sigma","ptres_vs_pt_Sigma"};
      const char* plotsl8 [] = {"#sigma(#delta #phi) [rad]", "#sigma(#delta cot(#theta))", "#sigma(#delta d_{0}) [cm]", "#sigma(#delta z_{0}) [cm]", "#sigma(#delta p_{t}/p_{t})"};
      Double_t minx8  [] = {5., 5., 5., 5., 5., 5.};
      Double_t maxx8  [] = {maxPT,maxPT,maxPT,maxPT,maxPT,maxPT};
      // Double_t miny8  [] = {0.,    -1E100, 0., -1E100, 0,  -1E100 };
      // Double_t maxy8  [] = {-1E100, -1E100, -1E100, -1E100, -1E100, -1E100 };
      Double_t norm_5   [] = {-999.,-999.,-999.,-999.,-999.};
      testt.Plot5Histograms(newDir + "/resolutionsPt.pdf",
		      rdir, sdir, 
		      rcollname, scollname,
		      "resolutionsPtdis", "resolution vs pt",
		      refLabel, newLabel,
		      refFileLabel, newFileLabel,
		      plots8, plotsl8,
		      logyfalse, doKolmo, norm_5,0,minx8,maxx8);     
      
      
      
    }  // end of "if CTF"

    //// Merge pdf histograms together into larger files, and name them based on the collection names
    TString mergefile = "merged_tracks.pdf"; // File name where partial pdfs will be merged
    TString destfile  = newDir + "/../" + myName + ".pdf"; // Destination file name
    TString gscommand = "gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="  + mergefile + " "
      + newDir + "/building.pdf "
      + newDir + "/building2.pdf "
      + newDir + "/hitsAndPt.pdf "
      + newDir + "/tuning.pdf "
      + newDir + "/Pulls.pdf "
      + newDir + "/residuals.pdf "
      + newDir + "/residualsEta.pdf "
      + newDir + "/meanvaluesEta.pdf "
      + newDir + "/resolutionsPt.pdf ";
    
    cout << ">> Merging partial pdfs to " << mergefile << "..." << endl;
#ifdef DEBUG
    cout << "DEBUG: ...with command \"" << gscommand << "\"" << endl;
#endif
    gSystem->Exec(gscommand);
    cout << ">> Moving " << mergefile << " to " << destfile << "..." << endl;
    gSystem->Rename(mergefile, destfile);

    cout << ">> Deleting partial pdf files" << endl;
    gSystem->Exec("rm -rf "+newDir); 
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

