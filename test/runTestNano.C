//------------------------------------------------------------------------------
//
//      Target: Check nanoLatino trees
//     Authors: Jonatan Piedra, Cedric Prieels
// Last update: August 1st, 2018
//
//------------------------------------------------------------------------------

TString path_mc = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Study2017/MCl1loose2017__baseW/";

TString path_data = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017__hadd/";


void runTestNano(TString filename = "NONE")
{
  if (filename.EqualTo("NONE"))
    {
      printf("\n Examples\n\n");
      printf(" root -l -b -q \'runTestNano.C(\"nanoLatino_SingleMuon_Run2017E-31Mar2018-v1__part2.root\")\'\n");
      printf(" root -l -b -q \'runTestNano.C(\"nanoLatino_WWW__part0.root\")\'\n");
      printf("\n");
      
      return;
    }
  
  TString path = (filename.Contains("Run2017")) ? path_data : path_mc;

  TChain* mychain = new TChain("Events", "Events");

  mychain->Add(path + filename);

  mychain->MakeSelector("testNano");

  gSystem->Exec("cp testNano.C.default testNano.C");

  mychain->Process("testNano.C+");

  return;
}
