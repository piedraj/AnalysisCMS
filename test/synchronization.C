//------------------------------------------------------------------------------
//
// root -l -b -q synchronization.C
//
//------------------------------------------------------------------------------
void synchronization()
{
  TChain* l2loose_tree = new TChain("latino", "latino");

  l2loose_tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut/latino_GluGluWWTo2L2Nu_MCFM.root");

  TChain* l2tight_tree = new TChain("latino", "latino");

  l2tight_tree->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016/Feb2017_summer16/MCl2looseCut__hadd__bSFL2pTEffCut__l2tight/latino_GluGluWWTo2L2Nu_MCFM.root");


  // Cuts
  //----------------------------------------------------------------------------
  TCut pt                       = "std_vector_lepton_pt[0] > 25. && std_vector_lepton_pt[1] > 20."; 
  TCut ee                       = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -121";
  TCut em                       = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -143";
  TCut mm                       = "std_vector_lepton_flavour[0] * std_vector_lepton_flavour[1] == -169";
  TCut eleIdHLT                 = "std_vector_lepton_eleIdHLT[0] * std_vector_lepton_eleIdHLT[1]";
  TCut eleIdTight               = "std_vector_lepton_eleIdTight[0] * std_vector_lepton_eleIdTight[1]";
  TCut tripleChargeAgreement    = "std_vector_electron_tripleChargeAgreement[0] * std_vector_electron_tripleChargeAgreement[1]";
  TCut expectedMissingInnerHits = "std_vector_electron_expectedMissingInnerHits[0] < 1 * std_vector_electron_expectedMissingInnerHits[1] < 1";
  TCut lep1_d0                  = "std_vector_lepton_d0[0] < (0.05 + 0.05*(abs(std_vector_lepton_eta[0]) > 1.479))";
  TCut lep2_d0                  = "std_vector_lepton_d0[1] < (0.05 + 0.05*(abs(std_vector_lepton_eta[1]) > 1.479))";
  TCut lep1_dz                  = "std_vector_lepton_dz[0] < (0.1 + 0.1 *(abs(std_vector_lepton_eta[0]) > 1.479))";
  TCut lep2_dz                  = "std_vector_lepton_dz[1] < (0.1 + 0.1 *(abs(std_vector_lepton_eta[1]) > 1.479))";


  // Draw
  //----------------------------------------------------------------------------
  TH1D* l2loose_mll_ee = new TH1D("l2loose_mll_ee", "", 10, 0, 1000);
  TH1D* l2loose_mll_em = new TH1D("l2loose_mll_em", "", 10, 0, 1000);
  TH1D* l2loose_mll_mm = new TH1D("l2loose_mll_mm", "", 10, 0, 1000);
  TH1D* l2tight_mll_ee = new TH1D("l2tight_mll_ee", "", 10, 0, 1000);
  TH1D* l2tight_mll_em = new TH1D("l2tight_mll_em", "", 10, 0, 1000);
  TH1D* l2tight_mll_mm = new TH1D("l2tight_mll_mm", "", 10, 0, 1000);

  TCanvas* c1 = new TCanvas("c1", "c1");

  l2loose_tree->Draw("mll>>l2loose_mll_ee", pt && ee && eleIdHLT && tripleChargeAgreement && expectedMissingInnerHits && lep1_d0 && lep2_d0 && lep1_dz && lep2_dz);
  l2loose_tree->Draw("mll>>l2loose_mll_em", pt && em && eleIdHLT && tripleChargeAgreement && expectedMissingInnerHits && lep1_d0 && lep2_d0 && lep1_dz && lep2_dz);
  l2loose_tree->Draw("mll>>l2loose_mll_mm", pt && mm && eleIdHLT && tripleChargeAgreement && expectedMissingInnerHits && lep1_d0 && lep2_d0 && lep1_dz && lep2_dz);

  l2tight_tree->Draw("mll>>l2tight_mll_ee", pt && ee);
  l2tight_tree->Draw("mll>>l2tight_mll_em", pt && em);
  l2tight_tree->Draw("mll>>l2tight_mll_mm", pt && mm);

  printf("\n");
  printf(" l2loose ee: %.0f\n", l2loose_mll_ee->Integral(-1, -1));
  printf(" l2tight ee: %.0f\n", l2tight_mll_ee->Integral(-1, -1));
  printf("\n");
  printf(" l2loose em: %.0f\n", l2loose_mll_em->Integral(-1, -1));
  printf(" l2tight em: %.0f\n", l2tight_mll_em->Integral(-1, -1));
  printf("\n");
  printf(" l2loose mm: %.0f\n", l2loose_mll_mm->Integral(-1, -1));
  printf(" l2tight mm: %.0f\n", l2tight_mll_mm->Integral(-1, -1));
  printf("\n");
}
