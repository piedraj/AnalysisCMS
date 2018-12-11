//------------------------------------------------------------------------------
//
// root -l 'chain.C+("Study2017")'
// root -l 'chain.C+("Full2017v2")'
//
//------------------------------------------------------------------------------

#include <TChain.h>
#include <TTree.h>


void chain(TString name = "Study2017")
{
  TChain* tree = new TChain("Events", "Events");

  // Study2017
  if (name.EqualTo("Study2017"))
    {
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017__hadd/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part0.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017__hadd/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part1.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Study2017/DATAl1loose2017__hadd/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part2.root");
    }

  // Full2017v2
  if (name.EqualTo("Full2017v2"))
    {
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part0.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part1.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part10.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part11.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part12.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part13.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part14.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part15.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part16.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part17.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part18.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part19.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part2.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part20.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part21.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part22.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part23.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part24.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part3.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part4.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part5.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part6.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part7.root");
      tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2/DATAl1loose2017v2__DATACorr2017__fakeSel/nanoLatino_DoubleMuon_Run2017C-31Mar2018-v1__part8.root");
    }

  // Run
  tree->Draw("MET_pt", "MET_pt < 20 && mtw1 < 20");
}
