//------------------------------------------------------------------------------
//
// root -l -b -q synchronization.C
//
//------------------------------------------------------------------------------
void synchronization(TString sample = "GluGluWWTo2L2Nu_MCFM")
{
  TChain* crab_tree = new TChain("latino", "latino");

  crab_tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Feb2017/MC/LatinoTrees/latino_GluGluWWTo2L2Nu_MCFM__part0.root");

  TChain* l2loose_tree = new TChain("latino", "latino");

  l2loose_tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut/latino_" + sample + ".root");

  TChain* l2tight_tree = new TChain("latino", "latino");

  l2tight_tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut__l2tight/latino_" + sample + ".root");


  // Common selection
  //----------------------------------------------------------------------------
  TCut pt      = "(std_vector_lepton_pt[0] > 25) && (std_vector_lepton_pt[1] > 20)"; 
  TCut l2tight = "(std_vector_lepton_isTightLepton[0] > 0.5) && (std_vector_lepton_isTightLepton[1] > 0.5)";


  // Electron selection
  //----------------------------------------------------------------------------
  TCut ee                       = "(std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -121)";
  TCut eleEta                   = "(abs(std_vector_lepton_eta[0])) < 2.5 && (abs(std_vector_lepton_eta[1]) < 2.5)"; 
  TCut eleIdHLT                 = "(std_vector_lepton_eleIdHLT[0] == 1) && (std_vector_lepton_eleIdHLT[1] == 1)";
  TCut eleIdTight               = "(std_vector_lepton_eleIdTight[0] == 1) && (std_vector_lepton_eleIdTight[1] == 1)";
  TCut tripleChargeAgreement    = "(std_vector_electron_tripleChargeAgreement[0] == 1) && (std_vector_electron_tripleChargeAgreement[1] == 1)";
  TCut expectedMissingInnerHits = "(std_vector_electron_expectedMissingInnerHits[0] < 1) && (std_vector_electron_expectedMissingInnerHits[1] < 1)";
  TCut ele1_d0                  = "(abs(std_vector_lepton_d0[0]) < (0.05 * (1 + (abs(std_vector_lepton_eta[0]) > 1.479))))";
  TCut ele2_d0                  = "(abs(std_vector_lepton_d0[1]) < (0.05 * (1 + (abs(std_vector_lepton_eta[1]) > 1.479))))";
  TCut ele1_dz                  = "(abs(std_vector_lepton_dz[0]) < (0.1  * (1 + (abs(std_vector_lepton_eta[0]) > 1.479))))";
  TCut ele2_dz                  = "(abs(std_vector_lepton_dz[1]) < (0.1  * (1 + (abs(std_vector_lepton_eta[1]) > 1.479))))";
  TCut eleIP                    = ele1_d0 && ele2_d0 && ele1_dz && ele2_dz;


  // Muon selection
  //----------------------------------------------------------------------------
  TCut    mm             = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -169";
  TCut    muonEta        = "abs(std_vector_lepton_eta[0]) < 2.4 && abs(std_vector_lepton_eta[1]) < 2.4"; 
  TCut    isTightMuon    = "std_vector_lepton_isTightMuon[0] && std_vector_lepton_isTightMuon[1]";
  TCut    muon1_trackIso = "std_vector_lepton_trackIso[0] / std_vector_lepton_pt[0] < 0.4";
  TCut    muon2_trackIso = "std_vector_lepton_trackIso[1] / std_vector_lepton_pt[1] < 0.4";
  TCut    trackIso       = muon1_trackIso && muon2_trackIso;
  TCut    muon1_d0       = "abs(std_vector_lepton_d0[0]) < (0.01 * (1 + (std_vector_lepton_pt[0] > 20)))";
  TCut    muon2_d0       = "abs(std_vector_lepton_d0[1]) < (0.01 * (1 + (std_vector_lepton_pt[1] > 20)))";
  TCut    muon1_dz       = "abs(std_vector_lepton_dz[0]) < 0.1";
  TCut    muon2_dz       = "abs(std_vector_lepton_dz[1]) < 0.1";
  TCut    muonIP         = muon1_d0 && muon2_d0 && muon1_dz && muon2_dz;
  TString iso1           = "(std_vector_lepton_photonIso[0] + std_vector_lepton_neutralHadronIso[0] - 0.5 * std_vector_lepton_sumPUPt[0])";
  TString iso2           = "(std_vector_lepton_photonIso[1] + std_vector_lepton_neutralHadronIso[1] - 0.5 * std_vector_lepton_sumPUPt[1])";
  TCut    muon1_iso      = Form("(std_vector_lepton_chargedHadronIso[0] + %s * (%s > 0)) / std_vector_lepton_pt[0] < 0.15", iso1.Data(), iso1.Data());
  TCut    muon2_iso      = Form("(std_vector_lepton_chargedHadronIso[1] + %s * (%s > 0)) / std_vector_lepton_pt[1] < 0.15", iso2.Data(), iso2.Data());
  TCut    muonIso        = muon1_iso && muon2_iso;


  // Draw
  //----------------------------------------------------------------------------
  TH1D* l2loose_ee_cuts    = new TH1D("l2loose_ee_cuts",    "", 10, 0, 1000);
  TH1D* l2loose_ee_l2tight = new TH1D("l2loose_ee_l2tight", "", 10, 0, 1000);
  TH1D* l2tight_ee         = new TH1D("l2tight_ee",         "", 10, 0, 1000);

  TH1D* l2loose_mm_cuts    = new TH1D("l2loose_mm_cuts",    "", 10, 0, 1000);
  TH1D* l2loose_mm_l2tight = new TH1D("l2loose_mm_l2tight", "", 10, 0, 1000);
  TH1D* l2tight_mm         = new TH1D("l2tight_mm",         "", 10, 0, 1000);

  TCanvas* c1 = new TCanvas("c1", "c1");

  l2loose_tree->Draw("mll>>l2loose_ee_cuts",    pt && ee && eleEta && eleIdHLT && tripleChargeAgreement && expectedMissingInnerHits && eleIP);
  l2loose_tree->Draw("mll>>l2loose_ee_l2tight", pt && ee && l2tight);
  l2tight_tree->Draw("mll>>l2tight_ee",         pt && ee);

  l2loose_tree->Draw("mll>>l2loose_mm_cuts",    pt && mm && muonEta && isTightMuon && trackIso && muonIP && muonIso);
  l2loose_tree->Draw("mll>>l2loose_mm_l2tight", pt && mm && l2tight);
  l2tight_tree->Draw("mll>>l2tight_mm",         pt && mm);


  // Print
  //----------------------------------------------------------------------------
  printf("\n ee yields for %s\n\n", sample.Data());
  printf(" l2loose (cuts)   : %.0f events\n", l2loose_ee_cuts   ->Integral(-1, -1));
  printf(" l2loose (l2tight): %.0f events\n", l2loose_ee_l2tight->Integral(-1, -1));
  printf(" l2tight          : %.0f events\n", l2tight_ee        ->Integral(-1, -1));
  printf("\n");
  printf("\n mm yields for %s\n\n", sample.Data());
  printf(" l2loose (cuts)   : %.0f events\n", l2loose_mm_cuts   ->Integral(-1, -1));
  printf(" l2loose (l2tight): %.0f events\n", l2loose_mm_l2tight->Integral(-1, -1));
  printf(" l2tight          : %.0f events\n", l2tight_mm        ->Integral(-1, -1));
  printf("\n");
}
