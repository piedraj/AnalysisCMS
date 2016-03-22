#include <iostream>

#include "TFile.h"
#include "TMVA/Config.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"


void optim12(TString signal);


const TString analysis  = "TTDM";
const TString inputdir  = "../minitrees/";
const TString outputdir = "training_test_outputs/trainOPT/";


void MVA1()
{
  gSystem->mkdir(outputdir, kTRUE);

  optim12("06_WW");
}


//------------------------------------------------------------------------------
// optim12
//------------------------------------------------------------------------------
void optim12(TString signal)
{
  TFile* outputfile = TFile::Open(outputdir + signal + ".root", "recreate");

  (TMVA::gConfig().GetIONames()).fWeightFileDir = "training_test_outputs/weightsOPT";
   

  // Factory
  //----------------------------------------------------------------------------
  TMVA::Factory* factory = new TMVA::Factory(signal, outputfile,    
					     "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");


  // Get the trees
  //----------------------------------------------------------------------------
  TFile *inputSFile  = TFile::Open(inputdir + analysis + "/" + signal + ".root", "read");
  TFile *inputB1File = TFile::Open(inputdir + analysis + "/08_WJets.root",       "read");
  TFile *inputB2File = TFile::Open(inputdir + analysis + "/09_TTV.root",         "read");
  TFile *inputB3File = TFile::Open(inputdir + analysis + "/01_Data.root",        "read");
  TFile *inputB4File = TFile::Open(inputdir + analysis + "/07_ZJets.root",       "read");
  TFile *inputB5File = TFile::Open(inputdir + analysis + "/00_Fakes.root",       "read");
  TFile *inputB6File = TFile::Open(inputdir + analysis + "/02_WZTo3LNu.root",    "read");
  TFile *inputB7File = TFile::Open(inputdir + analysis + "/05_ST.root",          "read");
  TFile *inputB8File = TFile::Open(inputdir + analysis + "/03_ZZ.root",          "read");
  TFile *inputB9File = TFile::Open(inputdir + analysis + "/04_TTTo2L2Nu.root",   "read");

  TTree *signalTree  = (TTree*)inputSFile ->Get("latino");
  TTree *background1 = (TTree*)inputB1File->Get("latino");
  TTree *background2 = (TTree*)inputB2File->Get("latino");
  TTree *background3 = (TTree*)inputB3File->Get("latino");
  TTree *background4 = (TTree*)inputB4File->Get("latino");
  TTree *background5 = (TTree*)inputB5File->Get("latino");
  TTree *background6 = (TTree*)inputB6File->Get("latino");
  TTree *background7 = (TTree*)inputB7File->Get("latino");
  TTree *background8 = (TTree*)inputB8File->Get("latino");
  TTree *background9 = (TTree*)inputB9File->Get("latino");

  Double_t weight = 1.0;

  factory->AddSignalTree    (signalTree , weight);
  factory->AddBackgroundTree(background1, weight);
  factory->AddBackgroundTree(background2, weight);
  factory->AddBackgroundTree(background3, weight);
  factory->AddBackgroundTree(background4, weight);
  factory->AddBackgroundTree(background5, weight);
  factory->AddBackgroundTree(background6, weight);
  factory->AddBackgroundTree(background7, weight);
  factory->AddBackgroundTree(background8, weight);
  factory->AddBackgroundTree(background9, weight);
  
  factory->SetWeightExpression("eventW");


  // Add variables
  //----------------------------------------------------------------------------
  // Be careful with the order: it must be respected at the application step
  // factory->AddVariable("<var1>+<var2>", "pretty title", "unit", 'F');

//factory->AddVariable("channel",      "", "", 'I');
  factory->AddVariable("metPfType1",   "", "", 'F');
  factory->AddVariable("mll",          "", "", 'F');
  factory->AddVariable("njet",         "", "", 'F');
  factory->AddVariable("nbjet20loose", "", "", 'I');
  factory->AddVariable("lep1pt",       "", "", 'F');
  factory->AddVariable("lep2pt",       "", "", 'F');
  factory->AddVariable("jet1pt",       "", "", 'F');
//factory->AddVariable("jet2pt",       "", "", 'F');
  factory->AddVariable("dphill",       "", "", 'F');
  factory->AddVariable("dphilep1jet1", "", "", 'F');
//factory->AddVariable("dphilep1jet2", "", "", 'F');
  factory->AddVariable("dphilmet1",    "", "", 'F');
  factory->AddVariable("dphilep2jet1", "", "", 'F');
//factory->AddVariable("dphilep2jet2", "", "", 'F');
  factory->AddVariable("dphilmet2",    "", "", 'F');
//factory->AddVariable("dphijj",       "", "", 'F');
  factory->AddVariable("dphijet1met",  "", "", 'F');
//factory->AddVariable("dphijet2met",  "", "", 'F');
  factory->AddVariable("dphillmet",    "", "", 'F');


  // Preselection cuts and preparation
  //----------------------------------------------------------------------------
  TCut mycut = "";

  factory->PrepareTrainingAndTestTree(mycut,     
				      ":nTrain_Signal=0:nTest_Signal=0:nTrain_Background=2000:nTest_Background=2000:SplitMode=Alternate:!V");


  // Book MVA
  //----------------------------------------------------------------------------
  factory->BookMethod(TMVA::Types::kMLP, "MLP",
		      "H:!V:NeuronType=sigmoid:VarTransform=N:NCycles=600:HiddenLayers=25,10:TestRate=5:!UseRegulator");


  // Train, test and evaluate MVA
  //----------------------------------------------------------------------------
  factory->TrainAllMethods();     // Train using the set of training events
  factory->TestAllMethods();      // Evaluate using the set of test events
  factory->EvaluateAllMethods();  // Evaluate and compare performance


  // Save the output
  //----------------------------------------------------------------------------
  outputfile->Close();

  cout << "==> Wrote root file: " << outputfile->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

  delete factory;
}
