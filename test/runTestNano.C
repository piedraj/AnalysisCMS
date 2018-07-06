TString path_mc = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Study2017/MCl1loose2017__baseW/";


void runTestNano()
{
  TChain* mychain = new TChain("Events", "Events");

  //  mychain->Add(path_mc + "nanoLatino_ZZTo4L__part2.root");

  mychain->Add(path_mc + "nanoLatino_DYJetsToLL_M-50-LO-ext1__part50.root");

  mychain->Process("testNano.C+");
}
