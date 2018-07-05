void runTestNano()
{
  TChain* mychain = new TChain("Events", "Events");

  mychain->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Study2017/MCl1loose2017__baseW/nanoLatino_ZZTo4L__part2.root");

  mychain->Process("testNano.C+");
}
