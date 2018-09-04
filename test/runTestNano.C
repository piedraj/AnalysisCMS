//------------------------------------------------------------------------------
//
//      Target: Check nanoLatino trees
//     Authors: Jonatan Piedra, Cedric Prieels
// Last update: August 2nd, 2018
//
//------------------------------------------------------------------------------
TString path_mc = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Study2017/MCl1loose2017__baseW__hadd/";

TString path_data = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017__hadd/";


void runTestNano(TString filename = "NONE")
{
  if (filename.EqualTo("NONE"))
    {
      printf("\n Examples\n\n");
      printf(" root -l -b -q \'runTestNano.C(\"SingleMuon_Run2017E-31Mar2018-v1__part2\")\'\n");
      printf(" root -l -b -q \'runTestNano.C(\"SingleElectron_Run2017B-31Mar2018-v1__part1\")\' *** Break *** segmentation violation\n");
      printf(" root -l -b -q \'runTestNano.C(\"SingleElectron_Run2017E-31Mar2018-v1__part2\")\'\n");
      printf(" root -l -b -q \'runTestNano.C(\"WWW\")\'\n");
      printf(" root -l -b -q \'runTestNano.C(\"GluGluHToWWTo2L2NuPowheg_M125_private\")\'\n");
      printf("\n");
      
      return;
    }
  
  TString path = (filename.Contains("Run2017")) ? path_data : path_mc;

  TChain* mychain = new TChain("Events", "Events");

  mychain->Add(path + "nanoLatino_" + filename + ".root");

  printf("\nExecuting mychain->MakeSelector(\"testNano\")...\n\n");

  mychain->MakeSelector("testNano");

  gSystem->Exec("cp testNano_template.C testNano.C");

  printf("\nExecuting mychain->Process(\"testNano.C+\")...\n\n");

  mychain->Process("testNano.C+");

  return;
}
