//root -l -b -q "MVA.C(80,80,\"ttDM0001scalar00010\")"

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

#include "ttdm.h"

// Constants
//------------------------------------------------------------------------------
const TString trainingdir    = "output/training/";
const TString weightsdir     = "output/weights/";
const TString applicationdir = "output/application/";

//const float metPfType1_cut = 50.; 
//const float mt2ll_cut      = 80.;


// Functions
//------------------------------------------------------------------------------
void MVATrain  (float metPfType1_cut, float mt2ll_cut, TString signal);

void MVARead   (TString MVA_id,
		TString signal,
		TString filename,
                int     systematic);

void AddProcess(TString kind,
		TString filename);


// Data members
//------------------------------------------------------------------------------
TTree*              _signaltree;
std::vector<TTree*> _mctree;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MVA
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MVA(float metPfType1_cut = 50.,
         float mt2ll_cut      = 0.,
	 TString signal     = "ttDM0001scalar00010", 
	 bool    doMVATrain = 0,
	 bool    doMVARead  = 1)
{
  if (!doMVATrain && !doMVARead) return;

  Assign();

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  gSystem->mkdir(trainingdir,    kTRUE);
  gSystem->mkdir(applicationdir, kTRUE);

  (TMVA::gConfig().GetIONames()).fWeightFileDir = weightsdir;


  // Training
  //----------------------------------------------------------------------------
  if (doMVATrain) MVATrain(metPfType1_cut,mt2ll_cut,signal);


  // Reading
  //----------------------------------------------------------------------------
  if (doMVARead)
    {

      //TString MVA_id = Form("met%.0f_mt2ll%.0f", metPfType1_cut, mt2ll_cut); 
      TString MVA_id = Form("mt2ll%.0f", mt2ll_cut); 

      for( int k = 0; k < nsystematic; k++ ){

	      if (  /*k != nominal &&*/  ( k < METdo || k > METdo ) ) continue;
	      //if( k!= EleESdo ) continue;

	      //MVARead(MVA_id, signal, "00_Fakes_1outof15", k);

	      ////MVARead(MVA_id, signal, "01_Data_1outof15", k);
	      //MVARead(MVA_id, signal, "01_Data_Full2016", k);
	      ///MVARead(MVA_id, signal, "09_TTV", k);

	      /*MVARead(MVA_id, signal, "02_WZTo3LNu", k);
	      MVARead(MVA_id, signal, "03_VZ", k);

	      MVARead(MVA_id, signal, "05_ST", k);
	      MVARead(MVA_id, signal, "06_WW", k);
	      MVARead(MVA_id, signal, "07_ZJets", k);
	      ///MVARead(MVA_id, signal, "11_Wg", k);
	      ///MVARead(MVA_id, signal, "12_Zg", k);
	      MVARead(MVA_id, signal, "13_VVV", k);

	      ///MVARead(MVA_id, signal, "00_Fakes_Full2016", k);
	      MVARead(MVA_id, signal, "04_TTTo2L2Nu", k);
	      //MVARead(MVA_id, signal, "04_TTTo2L2Nu_2", k);

	      ///MVARead(MVA_id, signal, signal, k);

	      MVARead(MVA_id, signal, "ttDM0001scalar00010", k);
	      MVARead(MVA_id, signal, "ttDM0001scalar00020", k);*/
	      MVARead(MVA_id, signal, "ttDM0001scalar00050", k);
	      MVARead(MVA_id, signal, "ttDM0001scalar00100", k);
	      MVARead(MVA_id, signal, "ttDM0001scalar00200", k);
	      MVARead(MVA_id, signal, "ttDM0001scalar00300", k);
	      MVARead(MVA_id, signal, "ttDM0001scalar00500", k);

	      MVARead(MVA_id, signal, "ttDM0001pseudo00010", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00020", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00050", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00100", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00200", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00300", k);
	      MVARead(MVA_id, signal, "ttDM0001pseudo00500", k);

      } // k 

    }  // if doMVARead

}  // MVA


//------------------------------------------------------------------------------
// MVATrain
//------------------------------------------------------------------------------
void MVATrain(float metPfType1_cut, float mt2ll_cut, TString signal)
{
  TFile* outputfile = TFile::Open(trainingdir + signal + ".root", "recreate");


  // Factory
  //----------------------------------------------------------------------------
  TMVA::Factory* factory = new TMVA::Factory(signal, outputfile,    
					    // "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
						"!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification");


  // Get the trees
  //----------------------------------------------------------------------------
  _mctree.clear();

  AddProcess("signal"    , signal);//"01_Data_reduced_1outof6"); //signal
  AddProcess("background", "skimmed_04_TTTo2L2Nu2");
  
  /*AddProcess("background", "14_HZ");
  AddProcess("background", "10_HWW");
  AddProcess("background", "06_WW");
  AddProcess("background", "02_WZTo3LNu");
  AddProcess("background", "03_VZ");
  AddProcess("background", "11_Wg");
  AddProcess("background", "07_ZJets");
  AddProcess("background", "09_TTV");
  AddProcess("background", "05_ST");
  AddProcess("background", "00_Fakes_reduced_1outof6");*/


  Double_t weight = 1.0;

  factory->AddSignalTree(_signaltree, weight);

  for (UInt_t i=0; i<_mctree.size(); i++) factory->AddBackgroundTree(_mctree[i], weight);

  factory->SetWeightExpression("eventW");


  // Add variables
  //----------------------------------------------------------------------------
  // Be careful with the order: it must be respected at the reading step
  // factory->AddVariable("<var1>+<var2>", "pretty title", "unit", 'F');

	factory->AddVariable("darkpt"       , "", "", 'F');
	//factory->AddVariable("topRecoW"     , "", "", 'F');
	//factory->AddVariable("lep1pt"       , "", "", 'F');
	//factory->AddVariable("lep1eta"      , "", "", 'F');
	//factory->AddVariable("lep1phi"      , "", "", 'F'); 
	//factory->AddVariable("lep1mass"     , "", "", 'F');
	//factory->AddVariable("lep2pt"       , "", "", 'F'); 
	//factory->AddVariable("lep2eta"      , "", "", 'F');
	//factory->AddVariable("lep2phi"      , "", "", 'F'); 
	//factory->AddVariable("lep2mass"     , "", "", 'F');
	//factory->AddVariable("jet1pt "      , "", "", 'F');
	//factory->AddVariable("jet1eta"      , "", "", 'F');
	//factory->AddVariable("jet1phi"      , "", "", 'F');
	//factory->AddVariable("jet1mass"     , "", "", 'F');
	//factory->AddVariable("jet2pt"       , "", "", 'F');
	//factory->AddVariable("jet2eta"      , "", "", 'F');
	//factory->AddVariable("jet2phi"      , "", "", 'F');
	//factory->AddVariable("jet2mass"     , "", "", 'F');
	factory->AddVariable("metPfType1"   , "", "", 'F');
	//factory->AddVariable("metPfType1Phi", "", "", 'F');
	//factory->AddVariable("m2l"          , "", "", 'F');
	factory->AddVariable("mt2ll"        , "", "", 'F');
	//factory->AddVariable("mt2lblb"      , "", "", 'F');
	//factory->AddVariable("mtw1"         , "", "", 'F');
	//factory->AddVariable("mtw2"         , "", "", 'F');
	//factory->AddVariable("ht"           , "", "", 'F');
	//factory->AddVariable("htjets"       , "", "", 'F');
	//factory->AddVariable("htnojets"     , "", "", 'F');
	//factory->AddVariable("njet"         , "", "", 'F');
	//factory->AddVariable("nbjet30csvv2l", "", "", 'F');
	//factory->AddVariable("nbjet30csvv2m", "", "", 'F');
	//factory->AddVariable("nbjet30csvv2t", "", "", 'F');
	//factory->AddVariable("dphijet1met"  , "", "", 'F');
	//factory->AddVariable("dphijet2met"  , "", "", 'F');
	//factory->AddVariable("dphijj"       , "", "", 'F');
	//factory->AddVariable("dphijjmet"    , "", "", 'F');
	//factory->AddVariable("dphill"       , "", "", 'F');
	//factory->AddVariable("dphilep1jet1" , "", "", 'F');
	//factory->AddVariable("dphilep1jet2" , "", "", 'F');
	//factory->AddVariable("dphilep2jet1" , "", "", 'F');
	//factory->AddVariable("dphilep2jet2" , "", "", 'F');
	//factory->AddVariable("dphilmet1"    , "", "", 'F');
	//factory->AddVariable("dphilmet2"    , "", "", 'F');
	factory->AddVariable("dphillmet"    , "", "", 'F');	
	//factory->AddVariable("sphericity"   , "", "", 'F');
	//factory->AddVariable("alignment"    , "", "", 'F');
	//factory->AddVariable("planarity"    , "", "", 'F');



  // Preselection cuts and preparation
  //----------------------------------------------------------------------------
  //factory->PrepareTrainingAndTestTree(Form("(((channel==3||channel==4)&&metPfType1>%5.2f)||channel==5)&&mt2ll>%5.2f&&darkpt>-3500.", metPfType1_cut, mt2ll_cut), "NormMode=EqualNumEvents:nTrain_Signal=0:nTest_Signal=0:nTrain_Background=0:nTest_Background=0:!V");

  factory->PrepareTrainingAndTestTree(Form("(((channel==3||channel==4)&&metPfType1>%5.2f)||channel==5)&&mt2ll>%5.2f&&darkpt>-5000", metPfType1_cut, mt2ll_cut), "NormMode=EqualNumEvents:nTrain_Signal=0:nTest_Signal=0:nTrain_Background=0:nTest_Background=0:!V");


  // Book MVA
  //----------------------------------------------------------------------------
  
    factory->BookMethod(TMVA::Types::kMLP, "justMET50SF",
    	      	      "H:!V:NeuronType=tanh:NCycles=500:VarTransform=Norm:HiddenLayers=6,3:TestRate=5:LearningRate=0.01:EstimatorType=MSE");

    //factory->BookMethod(TMVA::Types::kMLP, "sigmoid",
    //	      	      "H:!V:NeuronType=sigmoid:NCycles=500:VarTransform=Norm:HiddenLayers=6,3:TestRate=5:LearningRate=0.01:EstimatorType=MSE");

    //factory->BookMethod(TMVA::Types::kMLP, "sigmoid2",
    //	      	      "H:!V:NeuronType=sigmoid:NCycles=500:VarTransform=Norm:HiddenLayers=6,3:TestRate=5:LearningRate=0.05:EstimatorType=MSE");

    //factory->BookMethod(TMVA::Types::kMLP, "sigmoid3",
    //	      	      "H:!V:NeuronType=sigmoid:NCycles=500:VarTransform=Norm:HiddenLayers=6,3:TestRate=5:LearningRate=0.005:EstimatorType=MSE");

    //factory->BookMethod(TMVA::Types::kMLP, "sigmoid4",
    //	      	      "H:!V:NeuronType=sigmoid:NCycles=500:VarTransform=Norm:HiddenLayers=6,3:TestRate=5:LearningRate=0.0005:EstimatorType=MSE");

  //factory->BookMethod(TMVA::Types::kBDT, "BDT04", "NTrees=50:MaxDepth=2" );
  //factory->BookMethod(TMVA::Types::kBDT, "BDT05", "NTrees=50:MaxDepth=3" );



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
void MVARead(TString MVA_id, TString signal, TString filename, int systematic)
{

  cout << "\n\n\n" << filename << "\n\n\n" << endl; 

  
  //----- TMVA::Reader   

  TMVA::Reader* reader   = new TMVA::Reader("!Color:!Silent");   

	float darkpt       ;
	//float topRecoW     ;
	//float lep1eta      ; 
	//float lep1phi      ;  
	//float lep1mass     ;
	//float lep2pt       ; 
	//float lep2eta      ;
	//float lep2phi      ; 
	//float lep2mass     ;
	//float jet1pt       ;
	//float jet1eta      ;
	//float jet1phi      ;
	//float jet1mass     ;
	//float jet2pt       ;
	//float jet2eta      ;
	//float jet2phi      ;
	//float jet2mass     ;
	float metPfType1   ;
	//float metPfType1Phi;
	//float m2l          ;
	float mt2ll        ;
	//float mt2lblb      ;
	//float mtw1         ;
	//float mtw2         ;
	//float ht           ;
	//float htjets       ;
	//float htnojets     ;
	//float njet         ;
	//float nbjet30csvv2l;
	//float nbjet30csvv2m;
	//float nbjet30csvv2t;  
	//float dphijet1met  ;
	//float dphijet2met  ;
	//float dphijj       ;
	//float dphijjmet    ;
	//float dphill       ;
	//float dphilep1jet1 ;
	//float dphilep1jet2 ;
	//float dphilep2jet1 ;
	//float dphilep2jet2 ;
	//float dphilmet1    ;
	//float dphilmet2    ;
	float dphillmet    ;
	//float sphericity   ;
	//float alignment    ;
	//float planarity    ;

reader->AddVariable( "darkpt"       , &darkpt        );
	//reader->AddVariable( "topRecoW"     , &topRecoW      );
	//reader->AddVariable( "lep1eta"      , &lep1eta       );
	//reader->AddVariable( "lep1phi "     , &lep1phi       );
	//reader->AddVariable( "lep1mass"     , &lep1mass      );
	//reader->AddVariable( "lep2pt "      , &lep2pt        );
	//reader->AddVariable( "lep2eta"      , &lep2eta       );
	//reader->AddVariable( "lep2phi "     , &lep2phi       );
	//reader->AddVariable( "lep2mass"     , &lep2mass      );
	//reader->AddVariable( "jet1pt "      , &jet1pt        );
	//reader->AddVariable( "jet1eta"      , &jet1eta       );
	//reader->AddVariable( "jet1phi"      , &jet1phi       );
	//reader->AddVariable( "jet1mass"     , &jet1mass      );
	//reader->AddVariable( "jet2pt"       , &jet2pt        );
	//reader->AddVariable( "jet2eta"      , &jet2eta       );
	//reader->AddVariable( "jet2phi"      , &jet2phi       );
	//reader->AddVariable( "jet2mass"     , &jet2mass      );
reader->AddVariable( "metPfType1"   , &metPfType1    );
	//reader->AddVariable( "metPfType1Phi", &metPfType1Phi );
	//reader->AddVariable( "m2l"          , &m2l           );
reader->AddVariable( "mt2ll"        , &mt2ll         );
	//reader->AddVariable( "mt2lblb"      , &mt2lblb       );
	//reader->AddVariable( "mtw1"         , &mtw1          );
	//reader->AddVariable( "mtw2"         , &mtw2          );
	//reader->AddVariable( "ht"           , &ht            );
	//reader->AddVariable( "htjets"       , &htjets        );
	//reader->AddVariable( "htnojets"     , &htnojets      );
	//reader->AddVariable( "njet"         , &njet          );
	//reader->AddVariable( "nbjet30csvv2l", &nbjet30csvv2l );
	//reader->AddVariable( "nbjet30csvv2m", &nbjet30csvv2m );
	//reader->AddVariable( "nbjet30csvv2t", &nbjet30csvv2t );
	//reader->AddVariable( "dphijet1met"  , &dphijet1met   );
	//reader->AddVariable( "dphijet2met"  , &dphijet2met   );
	//reader->AddVariable( "dphijj"       , &dphijj        );
	//reader->AddVariable( "dphijjmet"    , &dphijjmet     );
	//reader->AddVariable( "dphill"       , &dphill        );
	//reader->AddVariable( "dphilep1jet1" , &dphilep1jet1  );
	//reader->AddVariable( "dphilep1jet2" , &dphilep1jet2  );
	//reader->AddVariable( "dphilep2jet1" , &dphilep2jet1  );
	//reader->AddVariable( "dphilep2jet2" , &dphilep2jet2  );
	//reader->AddVariable( "dphilmet1"    , &dphilmet1     );
	//reader->AddVariable( "dphilmet2"    , &dphilmet2     );
reader->AddVariable( "dphillmet"    , &dphillmet     );
	//reader->AddVariable( "sphericity"   , &sphericity    );
	//reader->AddVariable( "alignment"    , &alignment     );
	//reader->AddVariable( "planarity"    , &planarity     );



  // Get MVA response
  //----------------------------------------------------------------------------

  TFile* input = TFile::Open( storageSite + minitreeDir[systematic] + "/TTDM/" + filename + ".root", "update");
  //TFile* input = TFile::Open( "/eos/user/j/jgarciaf/minitrees/fucking-mom/TTDM/" + filename + ".root", "update");

  TTree* theTree = (TTree*)input->Get("latino");

	
  //----- read 

theTree->SetBranchAddress( "darkpt"       , &darkpt        );
	//theTree->SetBranchAddress( "topRecoW"     , &topRecoW      );
	//theTree->SetBranchAddress( "lep1eta"      , &lep1eta       );
	//theTree->SetBranchAddress( "lep1phi "     , &lep1phi       );
	//theTree->SetBranchAddress( "lep1mass"     , &lep1mass      );
	//theTree->SetBranchAddress( "lep2pt "      , &lep2pt        );
	//theTree->SetBranchAddress( "lep2eta"      , &lep2eta       );
	//theTree->SetBranchAddress( "lep2phi "     , &lep2phi       );
	//theTree->SetBranchAddress( "lep2mass"     , &lep2mass      );
	//theTree->SetBranchAddress( "jet1pt "      , &jet1pt        );
	//theTree->SetBranchAddress( "jet1eta"      , &jet1eta       );
	//theTree->SetBranchAddress( "jet1phi"      , &jet1phi       );
	//theTree->SetBranchAddress( "jet1mass"     , &jet1mass      );
	//theTree->SetBranchAddress( "jet2pt"       , &jet2pt        );
	//theTree->SetBranchAddress( "jet2eta"      , &jet2eta       );
	//theTree->SetBranchAddress( "jet2phi"      , &jet2phi       );
	//theTree->SetBranchAddress( "jet2mass"     , &jet2mass      );
theTree->SetBranchAddress( "metPfType1"   , &metPfType1    );
	//theTree->SetBranchAddress( "metPfType1Phi", &metPfType1Phi );
	//theTree->SetBranchAddress( "m2l"          , &m2l           );
theTree->SetBranchAddress( "mt2ll"        , &mt2ll         );
	//theTree->SetBranchAddress( "mt2lblb"      , &mt2lblb       );
	//theTree->SetBranchAddress( "mtw1"         , &mtw1          );
	//theTree->SetBranchAddress( "mtw2"         , &mtw2          );
	//theTree->SetBranchAddress( "ht"           , &ht            );
	//theTree->SetBranchAddress( "htjets"       , &htjets        );
	//theTree->SetBranchAddress( "htnojets"     , &htnojets      );
	//theTree->SetBranchAddress( "njet"         , &njet          );
	//theTree->SetBranchAddress( "nbjet30csvv2l", &nbjet30csvv2l );
	//theTree->SetBranchAddress( "nbjet30csvv2m", &nbjet30csvv2m );
	//theTree->SetBranchAddress( "nbjet30csvv2t", &nbjet30csvv2t );
	//theTree->SetBranchAddress( "dphijet1met"  , &dphijet1met   );
	//theTree->SetBranchAddress( "dphijet2met"  , &dphijet2met   );
	//theTree->SetBranchAddress( "dphijj"       , &dphijj        );
	//theTree->SetBranchAddress( "dphijjmet"    , &dphijjmet     );
	//theTree->SetBranchAddress( "dphill"       , &dphill        );
	//theTree->SetBranchAddress( "dphilep1jet1" , &dphilep1jet1  );
	//theTree->SetBranchAddress( "dphilep1jet2" , &dphilep1jet2  );
	//theTree->SetBranchAddress( "dphilep2jet1" , &dphilep2jet1  );
	//theTree->SetBranchAddress( "dphilep2jet2" , &dphilep2jet2  );
	//theTree->SetBranchAddress( "dphilmet1"    , &dphilmet1     );
	//theTree->SetBranchAddress( "dphilmet2"    , &dphilmet2     );
theTree->SetBranchAddress( "dphillmet"    , &dphillmet     );
	//theTree->SetBranchAddress( "sphericity"   , &sphericity    );
	//theTree->SetBranchAddress( "alignment"    , &alignment     );
	//theTree->SetBranchAddress( "planarity"    , &planarity     );


  //----- write      DO NOT USE '-' WHITHIN THE NAME OF A BRANCH !!!

  	////float mva01; TBranch* b_mva01 = theTree->Branch("ANN_check_" + signal, &mva01, "mva/F" );

	float s010;
	float s020;
	float s050;
	float s100;
	float s200;
	float s300;
	float s500;

	float p010;
	float p020;
	float p050;
	float p100;
	float p200;
	float p300;
	float p500;

	/*float p010ii;
	float p020ii;
	float p050ii;
	float p100ii;
	float p200ii;
	float p300ii;
	float p500ii;*/

 	TBranch* b_s010 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00010", &s010, "s010/F" );
 	TBranch* b_s020 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00020", &s020, "s020/F" );
 	TBranch* b_s050 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00050", &s050, "s050/F" );
 	TBranch* b_s100 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00100", &s100, "s100/F" );
 	TBranch* b_s200 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00200", &s200, "s200/F" );
 	TBranch* b_s300 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00300", &s300, "s300/F" );
 	TBranch* b_s500 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001scalar00500", &s500, "s500/F" );

 	TBranch* b_p010 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00010", &p010, "p010/F" );
 	TBranch* b_p020 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00020", &p020, "p020/F" );
 	TBranch* b_p050 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00050", &p050, "p050/F" );
 	TBranch* b_p100 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00100", &p100, "p100/F" );
 	TBranch* b_p200 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00200", &p200, "p200/F" );
 	TBranch* b_p300 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00300", &p300, "p300/F" );
 	TBranch* b_p500 = theTree->Branch("ANN_170904_justMET50SF_ttDM0001pseudo00500", &p500, "p500/F" );

 	/*TBranch* b_p010ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00010", &p010ii, "p010/F" );
 	TBranch* b_p020ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00020", &p020ii, "p020/F" );
 	TBranch* b_p050ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00050", &p050ii, "p050/F" );
 	TBranch* b_p100ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00100", &p100ii, "p100/F" );
 	TBranch* b_p200ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00200", &p200ii, "p200/F" );
 	TBranch* b_p300ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00300", &p300ii, "p300/F" );
 	TBranch* b_p500ii = theTree->Branch("ANN_170905_justMET50SF_redoing_ttDM0001pseudo00500", &p500ii, "p500/F" );*/


  // Book MVA methods
  //----------------------------------------------------------------------------
  //reader->BookMVA("01", weightsdir + signal + "_check.weights.xml");

  reader->BookMVA("s010", weightsdir + "ttDM0001scalar00010_justMET50SF.weights.xml");
  reader->BookMVA("s020", weightsdir + "ttDM0001scalar00020_justMET50SF.weights.xml");
  reader->BookMVA("s050", weightsdir + "ttDM0001scalar00050_justMET50SF.weights.xml");
  reader->BookMVA("s100", weightsdir + "ttDM0001scalar00100_justMET50SF.weights.xml");
  reader->BookMVA("s200", weightsdir + "ttDM0001scalar00200_justMET50SF.weights.xml");
  reader->BookMVA("s300", weightsdir + "ttDM0001scalar00300_justMET50SF.weights.xml");
  reader->BookMVA("s500", weightsdir + "ttDM0001scalar00500_justMET50SF.weights.xml");

  reader->BookMVA("p010", weightsdir + "ttDM0001pseudo00010_justMET50SF.weights.xml");
  reader->BookMVA("p020", weightsdir + "ttDM0001pseudo00020_justMET50SF.weights.xml");
  reader->BookMVA("p050", weightsdir + "ttDM0001pseudo00050_justMET50SF.weights.xml");
  reader->BookMVA("p100", weightsdir + "ttDM0001pseudo00100_justMET50SF.weights.xml");
  reader->BookMVA("p200", weightsdir + "ttDM0001pseudo00200_justMET50SF.weights.xml");
  reader->BookMVA("p300", weightsdir + "ttDM0001pseudo00300_justMET50SF.weights.xml");
  reader->BookMVA("p500", weightsdir + "ttDM0001pseudo00500_justMET50SF.weights.xml");

  /*reader->BookMVA("p010ii", weightsdir + "ttDM0001pseudo00010_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p020ii", weightsdir + "ttDM0001pseudo00020_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p050ii", weightsdir + "ttDM0001pseudo00050_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p100ii", weightsdir + "ttDM0001pseudo00100_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p200ii", weightsdir + "ttDM0001pseudo00200_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p300ii", weightsdir + "ttDM0001pseudo00300_justMET50SF_redoing.weights.xml");
  reader->BookMVA("p500ii", weightsdir + "ttDM0001pseudo00500_justMET50SF_redoing.weights.xml");*/

  Long64_t nentries = theTree->GetEntries();

  cout << "\n the total number of entries is... "<< nentries <<  endl; 

  for (Long64_t ievt=0; ievt<nentries; ievt++){

    theTree->GetEntry(ievt);

    if( ievt%10000==0) cout << "\t ievt = " << ievt << endl; 	

    //mva01 = reader->EvaluateMVA("01"); b_mva01->Fill();

    s010 = reader->EvaluateMVA("s010");
    s020 = reader->EvaluateMVA("s020");
    s050 = reader->EvaluateMVA("s050");
    s100 = reader->EvaluateMVA("s100");
    s200 = reader->EvaluateMVA("s200");
    s300 = reader->EvaluateMVA("s300");
    s500 = reader->EvaluateMVA("s500");

    p010 = reader->EvaluateMVA("p010");
    p020 = reader->EvaluateMVA("p020");
    p050 = reader->EvaluateMVA("p050");
    p100 = reader->EvaluateMVA("p100");
    p200 = reader->EvaluateMVA("p200");
    p300 = reader->EvaluateMVA("p300");
    p500 = reader->EvaluateMVA("p500");

    /*p010ii = reader->EvaluateMVA("p010ii");
    p020ii = reader->EvaluateMVA("p020ii");
    p050ii = reader->EvaluateMVA("p050ii");
    p100ii = reader->EvaluateMVA("p100ii");
    p200ii = reader->EvaluateMVA("p200ii");
    p300ii = reader->EvaluateMVA("p300ii");
    p500ii = reader->EvaluateMVA("p500ii");*/

    b_s010->Fill();
    b_s020->Fill();
    b_s050->Fill();
    b_s100->Fill();
    b_s200->Fill();
    b_s300->Fill();
    b_s500->Fill();

    b_p010->Fill();
    b_p020->Fill();
    b_p050->Fill();
    b_p100->Fill();
    b_p200->Fill();
    b_p300->Fill();
    b_p500->Fill();

    /*b_p010ii->Fill();
    b_p020ii->Fill();
    b_p050ii->Fill();
    b_p100ii->Fill();
    b_p200ii->Fill();
    b_p300ii->Fill();
    b_p500ii->Fill();*/

  } 

  // Save
  //----------------------------------------------------------------------------
  theTree->Write("", TObject::kOverwrite);

  input->Close();

  delete reader;

}


//------------------------------------------------------------------------------
// AddProcess
//------------------------------------------------------------------------------
void AddProcess(TString kind, TString filename)
{

  TString fullname = storageSite + minitreeDir[nominal] + "/TTDM/" + filename + ".root";
  //TString fullname = "/eos/user/j/jgarciaf/minitrees/fucking-mom/TTDM/" + filename + ".root";

  if (gSystem->AccessPathName(fullname))
    {
      printf(" [MVA::AddProcess] Cannot access %s\n", fullname.Data());
      return;
    }

  TFile* file = new TFile(fullname, "read");

  TTree* tree = (TTree*)file->Get("latino");

  if (kind.EqualTo("signal"))
    _signaltree = tree;
  else
    _mctree.push_back(tree);
}
