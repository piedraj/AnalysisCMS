TString path_l1loose = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017/";


void runNanoFakesDataRunC()
{
  TChain* mychain = new TChain("Events", "Events");

  //  mychain->Add(path_mc + "nanoLatino_ZZTo4L__part2.root");

  mychain->Add(path_l1loose + "nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part0.root");

  mychain->Process("nanoFakesDataRunC.C+");
}
