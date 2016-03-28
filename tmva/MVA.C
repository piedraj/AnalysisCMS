#include <iostream>

#include "TFile.h"
#include "TMVA/Config.h"
#include "TMVA/Factory.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"


void MVATrain(TString signal);
void MVARead (TString signal,
	      TString sample);


const TString analysis       = "TTDM";
const TString inputdir       = "../minitrees/";
const TString trainingdir    = "output/training/";
const TString weightsdir     = "output/weights/";
const TString applicationdir = "output/application/";


void MVA(TString signal = "06_WW")
{
  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  gSystem->mkdir(trainingdir,    kTRUE);
  gSystem->mkdir(applicationdir, kTRUE);

  (TMVA::gConfig().GetIONames()).fWeightFileDir = weightsdir;

  MVATrain(signal);

  MVARead(signal, signal);
  MVARead(signal, "01_Data");
  MVARead(signal, "09_TTV");
  MVARead(signal, "07_ZJets");
  MVARead(signal, "02_WZTo3LNu");
  MVARead(signal, "05_ST");
  MVARead(signal, "03_ZZ");
}


//------------------------------------------------------------------------------
// MVATrain
//------------------------------------------------------------------------------
void MVATrain(TString signal)
{
  TFile* outputfile = TFile::Open(trainingdir + signal + ".root", "recreate");


  // Factory
  //----------------------------------------------------------------------------
  TMVA::Factory* factory = new TMVA::Factory(signal, outputfile,    
					     "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");


  // Get the trees
  //----------------------------------------------------------------------------
  TFile *inputSFile  = TFile::Open(inputdir + analysis + "/" + signal + ".root", "read");
  TFile *inputB1File = TFile::Open(inputdir + analysis + "/09_TTV.root",         "read");
  TFile *inputB2File = TFile::Open(inputdir + analysis + "/07_ZJets.root",       "read");
  TFile *inputB3File = TFile::Open(inputdir + analysis + "/02_WZTo3LNu.root",    "read");
  TFile *inputB4File = TFile::Open(inputdir + analysis + "/05_ST.root",          "read");
  TFile *inputB5File = TFile::Open(inputdir + analysis + "/03_ZZ.root",          "read");

  TTree *signalTree  = (TTree*)inputSFile ->Get("latino");
  TTree *background1 = (TTree*)inputB1File->Get("latino");
  TTree *background2 = (TTree*)inputB2File->Get("latino");
  TTree *background3 = (TTree*)inputB3File->Get("latino");
  TTree *background4 = (TTree*)inputB4File->Get("latino");
  TTree *background5 = (TTree*)inputB5File->Get("latino");

  Double_t weight = 1.0;

  factory->AddSignalTree    (signalTree , weight);
  factory->AddBackgroundTree(background1, weight);
  factory->AddBackgroundTree(background2, weight);
  factory->AddBackgroundTree(background3, weight);
  factory->AddBackgroundTree(background4, weight);
  factory->AddBackgroundTree(background5, weight);
  
  factory->SetWeightExpression("eventW");


  // Add variables
  //----------------------------------------------------------------------------
  // Be careful with the order: it must be respected at the reading step
  // factory->AddVariable("<var1>+<var2>", "pretty title", "unit", 'F');

//factory->AddVariable("channel",      "", "", 'F');
//factory->AddVariable("metPfType1",   "", "", 'F');  // Empty range too large
//factory->AddVariable("mll",          "", "", 'F');  // Empty range too large
  factory->AddVariable("njet",         "", "", 'F');
//factory->AddVariable("nbjet20loose", "", "", 'F');
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

//factory->PrepareTrainingAndTestTree(mycut, ":nTrain_Signal=0:nTest_Signal=0:nTrain_Background=2000:nTest_Background=2000:SplitMode=Alternate:!V");
  factory->PrepareTrainingAndTestTree(mycut, ":nTrain_Signal=1000:nTest_Signal=1000:nTrain_Background=1500:nTest_Background=1500:SplitMode=Alternate:!V");  // Faster


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

  delete factory;
}


//------------------------------------------------------------------------------
// MVARead
//------------------------------------------------------------------------------
void MVARead(TString signal, TString sample)
{
  TMVA::Reader* reader = new TMVA::Reader("!Color:!Silent");   

  float channel;
  float metPfType1;
  float mll;
  float njet;
  float nbjet20loose;
  float lep1pt;
  float lep2pt;
  float jet1pt;
  float jet2pt;
  float dphill;
  float dphilep1jet1;
  float dphilep1jet2;
  float dphilmet1;
  float dphilep2jet1;
  float dphilep2jet2;
  float dphilmet2;
  float dphijj;
  float dphijet1met;
  float dphijet2met;
  float dphillmet;
  float eventW;
  float mva; 

//reader->AddVariable("channel",      &channel);
//reader->AddVariable("metPfType1",   &metPfType1);
//reader->AddVariable("mll",          &mll);
  reader->AddVariable("njet",         &njet);
//reader->AddVariable("nbjet20loose", &nbjet20loose);
  reader->AddVariable("lep1pt",       &lep1pt);
  reader->AddVariable("lep2pt",       &lep2pt);
  reader->AddVariable("jet1pt",       &jet1pt);
//reader->AddVariable("jet2pt",       &jet2pt);
  reader->AddVariable("dphill",       &dphill);
  reader->AddVariable("dphilep1jet1", &dphilep1jet1);
//reader->AddVariable("dphilep1jet2", &dphilep1jet2);
  reader->AddVariable("dphilmet1",    &dphilmet1);
  reader->AddVariable("dphilep2jet1", &dphilep2jet1);
//reader->AddVariable("dphilep2jet2", &dphilep2jet2);
  reader->AddVariable("dphilmet2",    &dphilmet2);
//reader->AddVariable("dphijj",       &dphijj);
  reader->AddVariable("dphijet1met",  &dphijet1met);
//reader->AddVariable("dphijet2met",  &dphijet2met);
  reader->AddVariable("dphillmet",    &dphillmet);


  // Book MVA methods
  //----------------------------------------------------------------------------
  reader->BookMVA("MLP", weightsdir + signal + "_MLP.weights.xml");


  // Get MVA response
  //----------------------------------------------------------------------------
  TH1F* h_mva = new TH1F("h_mva", "", 100, -0.5, 1.5);

  TFile* input = TFile::Open(inputdir + analysis + "/" + sample + ".root", "update");

  TTree* theTree = (TTree*)input->Get("latino");

  TBranch* b_mva = theTree->Branch("mva" + signal, &mva, "mva/F" );

  theTree->SetBranchAddress("channel",      &channel);
  theTree->SetBranchAddress("metPfType1",   &metPfType1);
  theTree->SetBranchAddress("mll",          &mll);
  theTree->SetBranchAddress("njet",         &njet);
  theTree->SetBranchAddress("nbjet20loose", &nbjet20loose);
  theTree->SetBranchAddress("lep1pt",       &lep1pt);
  theTree->SetBranchAddress("lep2pt",       &lep2pt);
  theTree->SetBranchAddress("jet1pt",       &jet1pt);
  theTree->SetBranchAddress("jet2pt",       &jet2pt);
  theTree->SetBranchAddress("dphill",       &dphill);
  theTree->SetBranchAddress("dphilep1jet1", &dphilep1jet1);
  theTree->SetBranchAddress("dphilep1jet2", &dphilep1jet2);
  theTree->SetBranchAddress("dphilmet1",    &dphilmet1);
  theTree->SetBranchAddress("dphilep2jet1", &dphilep2jet1);
  theTree->SetBranchAddress("dphilep2jet2", &dphilep2jet2);
  theTree->SetBranchAddress("dphilmet2",    &dphilmet2);
  theTree->SetBranchAddress("dphijj",       &dphijj);
  theTree->SetBranchAddress("dphijet1met",  &dphijet1met);
  theTree->SetBranchAddress("dphijet2met",  &dphijet2met);
  theTree->SetBranchAddress("dphillmet",    &dphillmet);
  theTree->SetBranchAddress("eventW",       &eventW);

  for (Long64_t ievt=0; ievt<theTree->GetEntries(); ievt++) {

    theTree->GetEntry(ievt);

    mva = reader->EvaluateMVA("MLP");

    h_mva->Fill(mva, eventW);
    b_mva->Fill();
  }

  // Save
  //----------------------------------------------------------------------------
  theTree->Write("", TObject::kOverwrite);

  input->Close();

  TFile* target = TFile::Open(applicationdir + signal + "__" + sample + ".root", "recreate");

  h_mva->Write();
  
  target->Close();

  h_mva->Delete();

  delete reader;
}
