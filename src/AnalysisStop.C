#define AnalysisStop_cxx
#include "../include/AnalysisStop.h"
#include <fstream>
#include <iostream>

//------------------------------------------------------------------------------
// AnalysisStop
//------------------------------------------------------------------------------
AnalysisStop::AnalysisStop(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{

  SetStopNeutralinoMap();
  if (systematic=="nominal") {
    SetSaveMinitree(true);
    _SaveHistograms = 0;
  } else {
    SetSaveMinitree(false);
    _SaveHistograms = 1;
  }
}

AnalysisStop::AnalysisStop(TFile* MiniTreeFile, TString systematic, int SaveHistograms) 
{
  SetSaveMinitree(false);
  SetStopNeutralinoMap();
  GetMiniTree(MiniTreeFile, systematic);
  _systematic = systematic;
  _SaveHistograms = SaveHistograms;
}

//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisStop::Loop(TString analysis, TString filename, float luminosity, float StopRefMass, float NeutralinoRefMass)
{
  if (fChain == 0) return;

  TString MassPointFlag = "";

  if (filename.Contains("T2tt")) {

    if (StopRefMass==-1.) {
  
      if (filename.Contains("150to250"))       { StopRefMass = 150.; NeutralinoRefMass =  25.; }
      else if (filename.Contains("250to350"))  { StopRefMass = 275.; NeutralinoRefMass = 150.; }
      else if (filename.Contains("350to400"))  { StopRefMass = 350.; NeutralinoRefMass = 225.; }
      else if (filename.Contains("400to1200")) { StopRefMass = 450.; NeutralinoRefMass = 325.; }
      else                                     { StopRefMass = 350.; NeutralinoRefMass = 225.; }

    }

    int iStopRefMass = StopRefMass, iNeutralinoRefMass = NeutralinoRefMass;
    MassPointFlag = "_Sm"; MassPointFlag += iStopRefMass; MassPointFlag += "_Xm"; MassPointFlag += iNeutralinoRefMass;

  }

  _applytopptreweighting = true;
 
  Setup(analysis, filename, luminosity, MassPointFlag);
 

  // Define histograms
  //----------------------------------------------------------------------------
  BookAnalysisHistograms();
  if (_SaveHistograms==2) BookSystematicHistograms();
  
  root_output->cd();

  FastSimDataset = _isfastsim ? "_T2" : "";
  BTagSF       = new BTagSFUtil("mujets", "CSVv2", "Medium",   0, FastSimDataset);
  BTagSF_Upb   = new BTagSFUtil("mujets", "CSVv2", "Medium",  +1, FastSimDataset);
  BTagSF_Dob   = new BTagSFUtil("mujets", "CSVv2", "Medium",  -1, FastSimDataset);
  BTagSF_UpFSb = new BTagSFUtil("mujets", "CSVv2", "Medium", +11, FastSimDataset);
  BTagSF_DoFSb = new BTagSFUtil("mujets", "CSVv2", "Medium", -11, FastSimDataset);

  // Loop over events
  //----------------------------------------------------------------------------
  
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {
    
    if (!_isminitree) {

      Long64_t ientry = LoadTree(jentry);
      
      if (ientry < 0) break;

    }

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    bool pass_masspoint = true;
    if (filename.Contains("T2tt")) 
      pass_masspoint = (susyMstop==StopRefMass && susyMLSP==NeutralinoRefMass) ? true : false;
    if (!pass_masspoint && !_saveminitree) continue;

    if (!_isminitree) {

      // I found an event with metPfType1 = nan, this cause MT2 calculation to stuck
      if (metPfType1!=metPfType1) continue;
      EventSetup(2.4, 20.);
   
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSRecommendationsMoriond17#Cleaning_up_of_fastsim_jets_from
      if (!PassFastsimJetsCleanup()) continue;

    }
    
    
    // Get analysis variables
    //--------------------------------------------------------------------------
    
    GetAnalysisVariables();
    
    // Analysis
    //--------------------------------------------------------------------------

    if (!_isminitree) {

      // To merge DY_LO and DY_HTbins( always ht >70 GeV) samples it is needed require ht > 70 in DY_LO sample; 
      // The analysis default sample is DY_NNLO  
      if (filename.Contains("DY") && filename.Contains("LO") && !filename.Contains("HT")) 
	if (_htgen>70.) continue;

      if (!_systematic.Contains("fake") && _nlepton != 2) continue; // 2 and only 2 leptons
      if (_systematic.Contains("fake") && (_nlepton <= 2 || _ntightlepton>2)) continue;
      
      if (!_systematic.Contains("SS") && Lepton1.flavour * Lepton2.flavour > 0) continue;
      if (_systematic.Contains("SS") && Lepton1.flavour * Lepton2.flavour < 0) continue;

      if (_ismc) CorrectEventWeight(); 

      if (Lepton1.v.Pt() < 25.) continue;
      if (Lepton2.v.Pt() < 20.) continue;
   
      _nelectron = 0;

      if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
      if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    
      if      (_nelectron == 2) _channel = ee;
      else if (_nelectron == 1) _channel = em;
      else if (_nelectron == 0) _channel = mm; 
    
      _m2l  = mll;
      _pt2l = ptll;

    }

    // ISR reweighting
    if (_applyisrreweighting && _isfastsim && _isminitree) ApplyISRReweighting();

    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;
    bool pass_blind = true; 

//    // Blinding policy: Just CR with 10  /fb.  For us blinded () = Met < 140; 
    if (filename.Contains("Data") || filename.Contains("PromptReco") || filename.Contains("23Sep2016") || _filename.Contains("03Feb2017")) 
    {

      pass_blind = false;
      //if (_mt2ll<40.) pass_blind = true;
      //if (MET.Et()<140.) pass_blind = true;
      if (run < 276502) pass_blind = true;
     }

    if (!_isminitree) {

      // Fill histograms
      // -----------------------------------------------------------------------------
           
      // Basics Stop
      //-------------------------------------------------------------------------
      FillLevelHistograms(Stop_00_Has2Leptons, pass && pass_blind && pass_masspoint);    

      pass &= mll>20.;
  
   /*   FillLevelHistograms(Stop_00_mll20, pass && pass_blind && pass_masspoint);
      
              // Look in the Z-peak
     */         // ---------------------------------------------------------------

               bool Zpeak = pass && ( _channel == em || fabs(_m2l - Z_MASS) < 15. );  

               FillLevelHistograms(Stop_05_Zpeak,      Zpeak && pass_blind && pass_masspoint); // 2 OS Leptons, mll > 20, Z peak
              
    /*           FillLevelHistograms(Stop_05_NoTagZpeak, Zpeak && (_leadingPtCSVv2M <  20.) && pass_masspoint); // 2 OS Leptons, mll > 20, Z peak + 0 b Tag (VET0) 

               FillLevelHistograms(Stop_05_TagZpeak,   Zpeak && (_leadingPtCSVv2M >= 20.) && pass_masspoint); // 2 OS Leptons, mll > 20, Z peak + 1 b Tag         

	      // DY estimation -> Routin Level
	      // ---------------------------------------------------------------

               FillLevelHistograms(Stop_04_Routin,      pass && pass_masspoint);  // 2 OS Leptons, mll > 20 
               
               FillLevelHistograms(Stop_04_Jet2Routin,  pass &&  jetpt2 >= 30. && pass_masspoint);  // 2 OS Leptons, mll > 20,  2 Jets
     
               FillLevelHistograms(Stop_04_TagRoutin,   pass && (_leadingPtCSVv2M >= 20.) && pass_masspoint);  // 2 OS Leptons, mll > 20, 1 b Tag
               
               FillLevelHistograms(Stop_04_NoTagRoutin, pass && (_leadingPtCSVv2M <  20.) && pass_masspoint);  // 2 OS Leptons, mll > 20, 0 b Tag (VET0)

   */           // ---------------------------------------------------------------



      pass &= ( _channel == em || fabs(_m2l - Z_MASS) > 15. );
      

       //~~~~~~~~~~~~~~~~~~~~~~ save minitree ~~~~~~~~~~~~~~~~~~

      // Leave this line at the end of this if or the results on latino trees and minitrees will be inconsistent
      if (pass && _saveminitree) minitree->Fill();      
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } 

    FillLevelHistograms(Stop_00_Zveto, pass && pass_blind && pass_masspoint);

    //~~~~~~~~~~~~~~~~~~~~~~ MC normalization study ~~~~~~~~~

    bool WW = pass && _njet == 0;
    FillLevelHistograms(Stop_00_WWsel, WW && pass_blind && pass_masspoint);
    FillLevelHistograms(Stop_00_WWselMET, WW && MET.Et()>= 50 && pass_blind && pass_masspoint);    
    bool TTbar = pass && _njet >1 && _leadingPtCSVv2M >= 20.; 
    FillLevelHistograms(Stop_00_TTsel, TTbar && pass_blind && pass_masspoint);
    FillLevelHistograms(Stop_00_TTselMET, TTbar && MET.Et()>= 50 && pass_blind && pass_masspoint);
      
   //~~~~~~~~~~~~~~~~~~~~~~ Analysis Cuts ~~~~~~~~~~~~~~~~~~ 

   // Tag SELECTION -> Bin0Tag & Bin1Tag;  used in minitrees and latino trees

    FillLevelHistograms(Stop_01_Tag,       pass && (_leadingPtCSVv2M >= 20.) && pass_blind && pass_masspoint);
    FillLevelHistograms(Stop_01_NoTag,     pass && (_leadingPtCSVv2M <  20.) && pass_blind && pass_masspoint);
    
    if (_leadingPtCSVv2M >= 20.) {
      FillLevelHistograms(Stop_02_VR1_Tag,   pass && (MET.Et()>=100. && MET.Et()<140.) && pass_blind && pass_masspoint);
      if (_leadingPtCSVv2T >= 30. && njet>1)
	FillLevelHistograms(Stop_02_VR1_Tag2Jet,   pass && (MET.Et()>=100. && MET.Et()<140.) && pass_masspoint);
      FillLevelHistograms(Stop_02_SR1_Tag,   pass && (MET.Et()>=140. && MET.Et()<200.) && pass_blind && pass_masspoint);
      FillLevelHistograms(Stop_02_SR2_Tag,   pass && (MET.Et()>=200. && MET.Et()<300.) && pass_blind && pass_masspoint);
      FillLevelHistograms(Stop_02_SR3_Tag,   pass && (MET.Et()>=300.) && pass_blind && pass_masspoint);
    }
    
    if (_leadingPtCSVv2M <  20.) {
      FillLevelHistograms(Stop_02_VR1_NoTag,   pass && (MET.Et()>=100. && MET.Et()<140.) && pass_blind && pass_masspoint);
      if (njet<1)
	FillLevelHistograms(Stop_02_VR1_NoJet,   pass && (MET.Et()>=100. && MET.Et()<140.) && pass_masspoint);
      FillLevelHistograms(Stop_02_SR1_NoTag,   pass && (MET.Et()>=140. && MET.Et()<200.) && pass_blind && pass_masspoint);
      FillLevelHistograms(Stop_02_SR2_NoTag,   pass && (MET.Et()>=200. && MET.Et()<300.) && pass_blind && pass_masspoint);
      FillLevelHistograms(Stop_02_SR3_NoTag,   pass && (MET.Et()>=300.) && pass_blind && pass_masspoint);
    }

  }

  if (_SaveHistograms==2) SaveSystematicHistograms();

  EndJob();
}

//------------------------------------------------------------------------------
// BookAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisStop::BookAnalysisHistograms()
{

  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {
  
    if (_isminitree && j<Stop_00_Zveto) continue; 
      
    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=ee; i<=ll; i++) {

	TString suffix = "_" + schannel[i];

	if (_SaveHistograms==0) DefineHistograms(i, j, k, suffix);

	h_mt2lblbcomb       [i][j][k] = new TH1D("h_mt2lblbcomb"        + suffix, "", 3000,    0, 3000);
	h_mt2bbtrue         [i][j][k] = new TH1D("h_mt2bbtrue"          + suffix, "", 3000,    0, 3000);
	h_mt2lblbtrue       [i][j][k] = new TH1D("h_mt2lblbtrue"        + suffix, "", 3000,    0, 3000);
	h_mt2lblbmatch      [i][j][k] = new TH1D("h_mt2lblbmatch"       + suffix, "", 3000,    0, 3000);
	h_mlb1comb          [i][j][k] = new TH1D("h_mlb1comb"           + suffix, "", 3000,    0, 3000);
	h_mlb2comb          [i][j][k] = new TH1D("h_mlb2comb"           + suffix, "", 3000,    0, 3000);
	h_mlb1true          [i][j][k] = new TH1D("h_mlb1true"           + suffix, "", 3000,    0, 3000);
	h_mlb2true          [i][j][k] = new TH1D("h_mlb2true"           + suffix, "", 3000,    0, 3000);
	h_mt2lblbvsmlbtrue  [i][j][k] = new TH2D("h_mt2lblbvsmlbtrue"   + suffix, "",  100,    0, 1000,  100,    0, 1000);
	h_nisrjet           [i][j][k] = new TH1D("h_nisrjet"            + suffix, "",   10,    0,   10);

	h_metmeff           [i][j][k] = new TH1D("h_metmeff"            + suffix, "",  500,    0,    5);
	h_MT2ll             [i][j][k] = new TH1F("h_MT2ll"              + suffix, "",    7,    0,  140);
	h_MT2llgen          [i][j][k] = new TH1F("h_MT2llgen"           + suffix, "",    7,    0,  140);
	h_MT2llisr          [i][j][k] = new TH1F("h_MT2llisr"           + suffix, "",    7,    0,  140);
	h_MT2llisrgen       [i][j][k] = new TH1F("h_MT2llisrgen"        + suffix, "",    7,    0,  140);
	h_MT2ll_nvtxup      [i][j][k] = new TH1F("h_MT2ll_nvtxup"       + suffix, "",    7,    0,  140);
	h_MT2ll_nvtxdo      [i][j][k] = new TH1F("h_MT2ll_nvtxdo"       + suffix, "",    7,    0,  140);
	h_MT2llgen_nvtxup   [i][j][k] = new TH1F("h_MT2llgen_nvtxup"    + suffix, "",    7,    0,  140);
	h_MT2llgen_nvtxdo   [i][j][k] = new TH1F("h_MT2llgen_nvtxdo"    + suffix, "",    7,    0,  140);
	h_MT2llisr_nvtxup   [i][j][k] = new TH1F("h_MT2llisr_nvtxup"    + suffix, "",    7,    0,  140);
	h_MT2llisr_nvtxdo   [i][j][k] = new TH1F("h_MT2llisr_nvtxdo"    + suffix, "",    7,    0,  140);
	h_MT2llisrgen_nvtxup[i][j][k] = new TH1F("h_MT2llisrgen_nvtxup" + suffix, "",    7,    0,  140);
	h_MT2llisrgen_nvtxdo[i][j][k] = new TH1F("h_MT2llisrgen_nvtxdo" + suffix, "",    7,    0,  140);
	h_MT2ll_fake        [i][j][k] = new TH1F("h_MT2ll_fake"         + suffix, "",    7,    0,  140);
	h_MT2ll_truth       [i][j][k] = new TH1F("h_MT2ll_truth"        + suffix, "",    7,    0,  140);
	h_MET_fake          [i][j][k] = new TH1F("h_MET_fake"           + suffix, "",  100,    0, 1000);
	h_MET_truth         [i][j][k] = new TH1F("h_MET_truth"          + suffix, "",  100,    0, 1000);
	
	h_MT2_Met           [i][j][k] = new TH1D("h_MT2_Met" + suffix, "", NbinsMT2*NbinsMet, vMinMT2, vMinMT2 + NbinsMet*(vMaxMT2-vMinMT2));
	h_HTvisible_Met     [i][j][k] = new TH1D("h_HTvisible_Met" + suffix, "", NbinsHTvisible*NbinsMet, vMinHTvisible, vMinHTvisible + NbinsMet*(vMaxHTvisible-vMinHTvisible));
	h_MetMeff_Met       [i][j][k] = new TH1D("h_MetMeff_Met" + suffix, "", NbinsMetMeff*NbinsMet, vMinMetMeff, vMinMetMeff + NbinsMet*(vMaxMetMeff-vMinMetMeff));
	h_R2_Met            [i][j][k] = new TH1D("h_R2_Met" + suffix, "", NbinsR2*NbinsMet, vMinR2, vMinR2 + NbinsMet*(vMaxR2-vMinR2));

      }
    }
  }

}

//------------------------------------------------------------------------------
// BookSystematicHistograms
//------------------------------------------------------------------------------
void AnalysisStop::BookSystematicHistograms()
{

  TH1::SetDefaultSumw2();

  for (int is = 0; is<nsystematic; is++) {
 
    if (ssystematic[is]=="nominal" || !systematicfromweight[is]) continue;
    if (ssystematic[is].Contains("isrnjet") && !_isfastsim) continue;
    
    TString prefix = (_isminitree) ? "minitrees/" : "";
    gSystem->mkdir(prefix + "rootfiles/" + ssystematic[is] + "/" + _analysis, kTRUE);
    TString _systematiclongname = _longname;
    _systematiclongname.ReplaceAll("nominal/", ssystematic[is] + "/");
    root_output_systematic[is] = new TFile(prefix + "rootfiles/" + _systematiclongname + ".root", "recreate");

    root_output_systematic[is]->cd();

    for (int j=0; j<ncut; j++) {
  
      if (_isminitree && j<Stop_00_Zveto) continue; 
      
      TString directory = scut[j];

      root_output_systematic[is]->cd();

      gDirectory->mkdir(directory);

      root_output_systematic[is]->cd(directory);
      
      for (int i=ee; i<=ll; i++) {

	TString suffix = "_" + schannel[i];

	h_MT2ll_systematic        [i][j][is] = new TH1F("h_MT2ll"            + suffix, "",    7,    0,  140);
	h_MT2llgen_systematic     [i][j][is] = new TH1F("h_MT2llgen"         + suffix, "",    7,    0,  140);
	h_MT2llisr_systematic     [i][j][is] = new TH1F("h_MT2llisr"         + suffix, "",    7,    0,  140);
	h_MT2llisrgen_systematic  [i][j][is] = new TH1F("h_MT2llisrgen"      + suffix, "",    7,    0,  140);
	
      }

    }

  }

}

//------------------------------------------------------------------------------
// GetAnalysisVariables
//------------------------------------------------------------------------------
void AnalysisStop::GetAnalysisVariables()
{

  // Met
  if (_isminitree) MET.SetPtEtaPhiM(metPfType1, 0.0, metPfType1Phi, 0.0); 

  _metmeff = MET.Et()/_meff;
  _MT2ll = (_mt2ll<140.) ? _mt2ll : 139.;
  _MT2llgen = (_mt2llgen<140.) ? _mt2llgen : 139.;

  // NbinsMet;
  //float tempMet = MET.Et(); if (tempMet<vMinMet) tempMet = vMinMet; if (tempMet>=vMaxMet) tempMet = vMaxMet - 0.1;
  //float widthMetBin = (vMaxMet - vMinMet)/NbinsMet;
  int binMet;// = (tempMet - vMinMet)/widthMetBin;
  if (MET.Et()<100.) binMet = 0;
  else if (MET.Et()<140.) binMet = 1;
  else if (MET.Et()<200.) binMet = 2;
  else if (MET.Et()<300.) binMet = 3;
  else binMet = 4;

  // _MT2_Met
  float tempMT2 = _mt2ll; if (tempMT2<vMinMT2) tempMT2 = vMinMT2; if (tempMT2>=vMaxMT2) tempMT2 = vMaxMT2 - 0.1;
  _MT2_Met = tempMT2 + (vMaxMT2 - vMinMT2)*binMet;

  // _HTvisible_Met
  float tempHTvisible = _htvisible; if (tempHTvisible<vMinHTvisible) tempHTvisible = vMinHTvisible; if (tempHTvisible>=vMaxHTvisible) tempHTvisible = vMaxHTvisible - 0.1;
  _HTvisible_Met = tempHTvisible + (vMaxHTvisible - vMinHTvisible)*binMet;

  // _MetMeff_Met
  float tempMetMeff = _metmeff; if (tempMetMeff<vMinMetMeff) tempMetMeff = vMinMetMeff; if (tempMetMeff>=vMaxMetMeff) tempMetMeff = vMaxMetMeff - 0.01;
  _MetMeff_Met = tempMetMeff + (vMaxMetMeff - vMinMetMeff)*binMet;

  // _R2_Met
  float tempR2 = _metmeff; if (tempR2<vMinR2) tempR2 = vMinR2; if (tempR2>=vMaxR2) tempR2 = vMaxR2 - 0.01;
  _R2_Met = tempR2 + (vMaxR2 - vMinR2)*binMet;

  
  // Fake
  _nLeptonsMatched = 0;
  if (fabs(_lep1isfake)<0.1) _nLeptonsMatched++; 
  if (fabs(_lep2isfake)<0.1) _nLeptonsMatched++; 
  
  // ISR jet
  _hasisrjet = (jetpt1>150. && jetpt1!=_leadingPtCSVv2M && acos(cos(metPfType1Phi-jetphi1))>2.5) ? true : false;

}

//------------------------------------------------------------------------------
// FillAnalysisHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillAnalysisHistograms(int ichannel,
					  int icut,
					  int ijet)
{
  if (ichannel != ll) FillAnalysisHistograms(ll, icut, ijet);


  h_mt2lblbcomb      [ichannel][icut][ijet]->Fill(_mt2lblbcomb,    _event_weight);
  h_mt2bbtrue        [ichannel][icut][ijet]->Fill(_mt2bbtrue,      _event_weight);
  h_mt2lblbtrue      [ichannel][icut][ijet]->Fill(_mt2lblbtrue,    _event_weight);
  h_mt2lblbmatch     [ichannel][icut][ijet]->Fill(_mt2lblbmatch,   _event_weight);
  h_mlb1comb         [ichannel][icut][ijet]->Fill(_mlb1comb,       _event_weight);
  h_mlb2comb         [ichannel][icut][ijet]->Fill(_mlb2comb,       _event_weight);
  h_mlb1true         [ichannel][icut][ijet]->Fill(_mlb1true,       _event_weight);
  h_mlb2true         [ichannel][icut][ijet]->Fill(_mlb2true,       _event_weight);
  h_mt2lblbvsmlbtrue [ichannel][icut][ijet]->Fill(_mlb1true, _mt2lblbtrue,       _event_weight);
  h_mt2lblbvsmlbtrue [ichannel][icut][ijet]->Fill(_mlb2true, _mt2lblbtrue,       _event_weight);
  h_nisrjet          [ichannel][icut][ijet]->Fill(_nisrjet,        _event_weight);
  h_metmeff          [ichannel][icut][ijet]->Fill(_metmeff,        _event_weight);
  h_MT2ll            [ichannel][icut][ijet]->Fill(_MT2ll,          _event_weight);
  h_MT2llgen         [ichannel][icut][ijet]->Fill(_MT2llgen,       _event_weight);

  if (nvtx>=20) {
    h_MT2ll_nvtxup   [ichannel][icut][ijet]->Fill(_MT2ll,       _event_weight);
    h_MT2llgen_nvtxup[ichannel][icut][ijet]->Fill(_MT2llgen,    _event_weight);
  } else {
    h_MT2ll_nvtxdo   [ichannel][icut][ijet]->Fill(_MT2ll,       _event_weight);
    h_MT2llgen_nvtxdo[ichannel][icut][ijet]->Fill(_MT2llgen,    _event_weight);
  }

  if (_hasisrjet) {
    h_MT2llisr         [ichannel][icut][ijet]->Fill(_MT2ll,          _event_weight);
    h_MT2llisrgen      [ichannel][icut][ijet]->Fill(_MT2llgen,       _event_weight);
    if (nvtx>=20) {
      h_MT2llisr_nvtxup   [ichannel][icut][ijet]->Fill(_MT2ll,       _event_weight);
      h_MT2llisrgen_nvtxup[ichannel][icut][ijet]->Fill(_MT2llgen,    _event_weight);
    } else {
      h_MT2llisr_nvtxdo   [ichannel][icut][ijet]->Fill(_MT2ll,       _event_weight);
      h_MT2llisrgen_nvtxdo[ichannel][icut][ijet]->Fill(_MT2llgen,    _event_weight);
    }
  }

  if (_nLeptonsMatched==2) {
    h_MT2ll_truth    [ichannel][icut][ijet]->Fill(_MT2ll,          _event_weight);
    h_MET_truth      [ichannel][icut][ijet]->Fill(MET.Et(),        _event_weight);
  } else { 
    h_MT2ll_fake     [ichannel][icut][ijet]->Fill(_MT2ll,          _event_weight);
    h_MET_fake       [ichannel][icut][ijet]->Fill(MET.Et(),        _event_weight);
  }

  h_MT2_Met          [ichannel][icut][ijet]->Fill(_MT2_Met,        _event_weight);
  h_HTvisible_Met    [ichannel][icut][ijet]->Fill(_HTvisible_Met,  _event_weight);
  h_MetMeff_Met      [ichannel][icut][ijet]->Fill(_MetMeff_Met,    _event_weight);
  h_R2_Met           [ichannel][icut][ijet]->Fill(_R2_Met,         _event_weight);
  

}


//------------------------------------------------------------------------------
// FillSystematicHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillSystematicHistograms(int ichannel,
					    int icut)
{
  if (ichannel != ll) FillSystematicHistograms(ll, icut);

  for (int is = 0; is<nsystematic; is++) {
  
    if (ssystematic[is]=="nominal" || !systematicfromweight[is]) continue;
    if (ssystematic[is].Contains("isrnjet") && !_isfastsim) continue;
    
    root_output_systematic[is]->cd();

    float _event_weight_systematic = -999.;
    if (ssystematic[is]=="Triggerup") _event_weight_systematic = _event_weight_Triggerup;
    if (ssystematic[is]=="Triggerdo") _event_weight_systematic = _event_weight_Triggerdo;
    if (ssystematic[is]=="Recoup")    _event_weight_systematic = _event_weight_Recoup;
    if (ssystematic[is]=="Recodo")    _event_weight_systematic = _event_weight_Recodo;
    if (ssystematic[is]=="Idisoup")   _event_weight_systematic = _event_weight_Idisoup;
    if (ssystematic[is]=="Idisodo")   _event_weight_systematic = _event_weight_Idisodo;
    if (ssystematic[is]=="Fastsimup") _event_weight_systematic = _event_weight_Fastsimup;
    if (ssystematic[is]=="Fastsimdo") _event_weight_systematic = _event_weight_Fastsimdo;
    if (ssystematic[is]=="Btagup")    _event_weight_systematic = _event_weight_Btagup;
    if (ssystematic[is]=="Btagdo")    _event_weight_systematic = _event_weight_Btagdo;
    if (ssystematic[is]=="BtagFSup")  _event_weight_systematic = _event_weight_BtagFSup;
    if (ssystematic[is]=="BtagFSdo")  _event_weight_systematic = _event_weight_BtagFSdo;
    if (ssystematic[is]=="Topptup")   _event_weight_systematic = _event_weight_Toppt;
    if (ssystematic[is]=="Topptdo")   _event_weight_systematic = _event_weight;
    if (ssystematic[is]=="Isrnjetup") _event_weight_systematic = _event_weight_Isrnjetup;
    if (ssystematic[is]=="Isrnjetdo") _event_weight_systematic = _event_weight_Isrnjetdo;

    if (_event_weight_systematic==-999.) 
      printf("\n\n Bad name for systematics, please check!\n");

    h_MT2ll_systematic      [ichannel][icut][is]->Fill(_MT2ll,    _event_weight_systematic);
    h_MT2llgen_systematic   [ichannel][icut][is]->Fill(_MT2llgen, _event_weight_systematic);
    if (_hasisrjet) {
      h_MT2llisr_systematic      [ichannel][icut][is]->Fill(_MT2ll,    _event_weight_systematic);
      h_MT2llisrgen_systematic   [ichannel][icut][is]->Fill(_MT2llgen, _event_weight_systematic);
    }

  }

}



//------------------------------------------------------------------------------
// SaveSystematicHistograms
//------------------------------------------------------------------------------
void AnalysisStop::SaveSystematicHistograms()
{

  printf("\n\n Writing histograms for systematics.\n");

  for (int is = 0; is<nsystematic; is++) {
  
    if (ssystematic[is]=="nominal" || !systematicfromweight[is]) continue;
    if (ssystematic[is].Contains("isrnjet") && !_isfastsim) continue;

    root_output_systematic[is]->cd();
    root_output_systematic[is]->Write("", TObject::kOverwrite);
    root_output_systematic[is]->Close();

  }

}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillLevelHistograms(int  icut,
				       bool pass)
{
  if (!pass) return;
  
  if (_SaveHistograms==0) {

    FillHistograms(_channel, icut, _jetbin);
    FillHistograms(_channel, icut, njetbin);
  }


  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);

  if (_SaveHistograms==2) FillSystematicHistograms(_channel, icut);
  
}

void AnalysisStop::SetStopNeutralinoMap() {

  MassPoint mp0 (150, 1);
  StopCrossSection cs0 (249.409, 36.7821);
  MassPointParameters mpp0 (cs0, 812165);
  StopNeutralinoMap.insert(std::make_pair(mp0, mpp0));

  MassPoint mp1 (150, 25);
  StopCrossSection cs1 (249.409, 36.7821);
  MassPointParameters mpp1 (cs1, 809375);
  StopNeutralinoMap.insert(std::make_pair(mp1, mpp1));

  MassPoint mp2 (150, 50);
  StopCrossSection cs2 (249.409, 36.7821);
  MassPointParameters mpp2 (cs2, 783429);
  StopNeutralinoMap.insert(std::make_pair(mp2, mpp2));

  MassPoint mp3 (150, 63);
  StopCrossSection cs3 (249.409, 36.7821);
  MassPointParameters mpp3 (cs3, 825930);
  StopNeutralinoMap.insert(std::make_pair(mp3, mpp3));

  MassPoint mp4 (167, 1);
  StopCrossSection cs4 (151.469, 22.263);
  MassPointParameters mpp4 (cs4, 785649);
  StopNeutralinoMap.insert(std::make_pair(mp4, mpp4));

  MassPoint mp5 (175, 1);
  StopCrossSection cs5 (121.416, 17.7681);
  MassPointParameters mpp5 (cs5, 758602);
  StopNeutralinoMap.insert(std::make_pair(mp5, mpp5));

  MassPoint mp6 (175, 25);
  StopCrossSection cs6 (121.416, 17.7681);
  MassPointParameters mpp6 (cs6, 735081);
  StopNeutralinoMap.insert(std::make_pair(mp6, mpp6));

  MassPoint mp7 (175, 50);
  StopCrossSection cs7 (121.416, 17.7681);
  MassPointParameters mpp7 (cs7, 740603);
  StopNeutralinoMap.insert(std::make_pair(mp7, mpp7));

  MassPoint mp8 (175, 75);
  StopCrossSection cs8 (121.416, 17.7681);
  MassPointParameters mpp8 (cs8, 746712);
  StopNeutralinoMap.insert(std::make_pair(mp8, mpp8));

  MassPoint mp9 (175, 88);
  StopCrossSection cs9 (121.416, 17.7681);
  MassPointParameters mpp9 (cs9, 747927);
  StopNeutralinoMap.insert(std::make_pair(mp9, mpp9));

  MassPoint mp10 (183, 1);
  StopCrossSection cs10 (98.4784, 14.1473);
  MassPointParameters mpp10 (cs10, 732848);
  StopNeutralinoMap.insert(std::make_pair(mp10, mpp10));

  MassPoint mp11 (192, 25);
  StopCrossSection cs11 (78.4483, 11.3431);
  MassPointParameters mpp11 (cs11, 716416);
  StopNeutralinoMap.insert(std::make_pair(mp11, mpp11));

  MassPoint mp12 (200, 1);
  StopCrossSection cs12 (64.5085, 9.29555);
  MassPointParameters mpp12 (cs12, 957598);
  StopNeutralinoMap.insert(std::make_pair(mp12, mpp12));

  MassPoint mp13 (200, 25);
  StopCrossSection cs13 (64.5085, 9.29555);
  MassPointParameters mpp13 (cs13, 982117);
  StopNeutralinoMap.insert(std::make_pair(mp13, mpp13));

  MassPoint mp14 (200, 50);
  StopCrossSection cs14 (64.5085, 9.29555);
  MassPointParameters mpp14 (cs14, 962087);
  StopNeutralinoMap.insert(std::make_pair(mp14, mpp14));

  MassPoint mp15 (200, 75);
  StopCrossSection cs15 (64.5085, 9.29555);
  MassPointParameters mpp15 (cs15, 967672);
  StopNeutralinoMap.insert(std::make_pair(mp15, mpp15));

  MassPoint mp16 (200, 100);
  StopCrossSection cs16 (64.5085, 9.29555);
  MassPointParameters mpp16 (cs16, 981898);
  StopNeutralinoMap.insert(std::make_pair(mp16, mpp16));

  MassPoint mp17 (200, 113);
  StopCrossSection cs17 (64.5085, 9.29555);
  MassPointParameters mpp17 (cs17, 995617);
  StopNeutralinoMap.insert(std::make_pair(mp17, mpp17));

  MassPoint mp18 (208, 25);
  StopCrossSection cs18 (53.4447, 7.65327);
  MassPointParameters mpp18 (cs18, 966699);
  StopNeutralinoMap.insert(std::make_pair(mp18, mpp18));

  MassPoint mp19 (217, 50);
  StopCrossSection cs19 (43.4633, 6.22129);
  MassPointParameters mpp19 (cs19, 938206);
  StopNeutralinoMap.insert(std::make_pair(mp19, mpp19));

  MassPoint mp20 (225, 1);
  StopCrossSection cs20 (36.3818, 5.17309);
  MassPointParameters mpp20 (cs20, 914858);
  StopNeutralinoMap.insert(std::make_pair(mp20, mpp20));

  MassPoint mp21 (225, 25);
  StopCrossSection cs21 (36.3818, 5.17309);
  MassPointParameters mpp21 (cs21, 945708);
  StopNeutralinoMap.insert(std::make_pair(mp21, mpp21));

  MassPoint mp22 (225, 50);
  StopCrossSection cs22 (36.3818, 5.17309);
  MassPointParameters mpp22 (cs22, 920154);
  StopNeutralinoMap.insert(std::make_pair(mp22, mpp22));

  MassPoint mp23 (225, 75);
  StopCrossSection cs23 (36.3818, 5.17309);
  MassPointParameters mpp23 (cs23, 957949);
  StopNeutralinoMap.insert(std::make_pair(mp23, mpp23));

  MassPoint mp24 (225, 100);
  StopCrossSection cs24 (36.3818, 5.17309);
  MassPointParameters mpp24 (cs24, 972699);
  StopNeutralinoMap.insert(std::make_pair(mp24, mpp24));

  MassPoint mp25 (225, 125);
  StopCrossSection cs25 (36.3818, 5.17309);
  MassPointParameters mpp25 (cs25, 929549);
  StopNeutralinoMap.insert(std::make_pair(mp25, mpp25));

  MassPoint mp26 (225, 138);
  StopCrossSection cs26 (36.3818, 5.17309);
  MassPointParameters mpp26 (cs26, 935628);
  StopNeutralinoMap.insert(std::make_pair(mp26, mpp26));

  MassPoint mp27 (233, 50);
  StopCrossSection cs27 (30.6565, 4.35198);
  MassPointParameters mpp27 (cs27, 906357);
  StopNeutralinoMap.insert(std::make_pair(mp27, mpp27));

  MassPoint mp28 (242, 75);
  StopCrossSection cs28 (25.4398, 3.58399);
  MassPointParameters mpp28 (cs28, 857664);
  StopNeutralinoMap.insert(std::make_pair(mp28, mpp28));

  MassPoint mp29 (250, 1);
  StopCrossSection cs29 (21.5949, 3.03613);
  MassPointParameters mpp29 (cs29, 878903);
  StopNeutralinoMap.insert(std::make_pair(mp29, mpp29));

  MassPoint mp30 (250, 25);
  StopCrossSection cs30 (21.5949, 3.03613);
  MassPointParameters mpp30 (cs30, 887984);
  StopNeutralinoMap.insert(std::make_pair(mp30, mpp30));

  MassPoint mp31 (250, 50);
  StopCrossSection cs31 (21.5949, 3.03613);
  MassPointParameters mpp31 (cs31, 878134);
  StopNeutralinoMap.insert(std::make_pair(mp31, mpp31));

  MassPoint mp32 (250, 75);
  StopCrossSection cs32 (21.5949, 3.03613);
  MassPointParameters mpp32 (cs32, 849429);
  StopNeutralinoMap.insert(std::make_pair(mp32, mpp32));

  MassPoint mp33 (250, 100);
  StopCrossSection cs33 (21.5949, 3.03613);
  MassPointParameters mpp33 (cs33, 877614);
  StopNeutralinoMap.insert(std::make_pair(mp33, mpp33));

  MassPoint mp34 (250, 125);
  StopCrossSection cs34 (21.5949, 3.03613);
  MassPointParameters mpp34 (cs34, 872114);
  StopNeutralinoMap.insert(std::make_pair(mp34, mpp34));

  MassPoint mp35 (250, 150);
  StopCrossSection cs35 (21.5949, 3.03613);
  MassPointParameters mpp35 (cs35, 885660);
  StopNeutralinoMap.insert(std::make_pair(mp35, mpp35));

  MassPoint mp36 (250, 163);
  StopCrossSection cs36 (21.5949, 3.03613);
  MassPointParameters mpp36 (cs36, 866692);
  StopNeutralinoMap.insert(std::make_pair(mp36, mpp36));

  MassPoint mp37 (258, 75);
  StopCrossSection cs37 (18.4347, 2.56587);
  MassPointParameters mpp37 (cs37, 853986);
  StopNeutralinoMap.insert(std::make_pair(mp37, mpp37));

  MassPoint mp38 (267, 100);
  StopCrossSection cs38 (15.5256, 2.16481);
  MassPointParameters mpp38 (cs38, 845095);
  StopNeutralinoMap.insert(std::make_pair(mp38, mpp38));

  MassPoint mp39 (275, 1);
  StopCrossSection cs39 (13.3231, 1.89919);
  MassPointParameters mpp39 (cs39, 861956);
  StopNeutralinoMap.insert(std::make_pair(mp39, mpp39));

  MassPoint mp40 (275, 25);
  StopCrossSection cs40 (13.3231, 1.89919);
  MassPointParameters mpp40 (cs40, 824537);
  StopNeutralinoMap.insert(std::make_pair(mp40, mpp40));

  MassPoint mp41 (275, 50);
  StopCrossSection cs41 (13.3231, 1.89919);
  MassPointParameters mpp41 (cs41, 851997);
  StopNeutralinoMap.insert(std::make_pair(mp41, mpp41));

  MassPoint mp42 (275, 75);
  StopCrossSection cs42 (13.3231, 1.89919);
  MassPointParameters mpp42 (cs42, 844693);
  StopNeutralinoMap.insert(std::make_pair(mp42, mpp42));

  MassPoint mp43 (275, 100);
  StopCrossSection cs43 (13.3231, 1.89919);
  MassPointParameters mpp43 (cs43, 838466);
  StopNeutralinoMap.insert(std::make_pair(mp43, mpp43));

  MassPoint mp44 (275, 125);
  StopCrossSection cs44 (13.3231, 1.89919);
  MassPointParameters mpp44 (cs44, 829187);
  StopNeutralinoMap.insert(std::make_pair(mp44, mpp44));

  MassPoint mp45 (275, 150);
  StopCrossSection cs45 (13.3231, 1.89919);
  MassPointParameters mpp45 (cs45, 840658);
  StopNeutralinoMap.insert(std::make_pair(mp45, mpp45));

  MassPoint mp46 (275, 175);
  StopCrossSection cs46 (13.3231, 1.89919);
  MassPointParameters mpp46 (cs46, 846634);
  StopNeutralinoMap.insert(std::make_pair(mp46, mpp46));

  MassPoint mp47 (275, 188);
  StopCrossSection cs47 (13.3231, 1.89919);
  MassPointParameters mpp47 (cs47, 845385);
  StopNeutralinoMap.insert(std::make_pair(mp47, mpp47));

  MassPoint mp48 (283, 100);
  StopCrossSection cs48 (11.5185, 1.62631);
  MassPointParameters mpp48 (cs48, 851914);
  StopNeutralinoMap.insert(std::make_pair(mp48, mpp48));

  MassPoint mp49 (292, 125);
  StopCrossSection cs49 (9.79779, 1.36209);
  MassPointParameters mpp49 (cs49, 783430);
  StopNeutralinoMap.insert(std::make_pair(mp49, mpp49));

  MassPoint mp50 (300, 1);
  StopCrossSection cs50 (8.51615, 1.18564);
  MassPointParameters mpp50 (cs50, 975907);
  StopNeutralinoMap.insert(std::make_pair(mp50, mpp50));

  MassPoint mp51 (300, 25);
  StopCrossSection cs51 (8.51615, 1.18564);
  MassPointParameters mpp51 (cs51, 934485);
  StopNeutralinoMap.insert(std::make_pair(mp51, mpp51));

  MassPoint mp52 (300, 50);
  StopCrossSection cs52 (8.51615, 1.18564);
  MassPointParameters mpp52 (cs52, 983303);
  StopNeutralinoMap.insert(std::make_pair(mp52, mpp52));

  MassPoint mp53 (300, 75);
  StopCrossSection cs53 (8.51615, 1.18564);
  MassPointParameters mpp53 (cs53, 978530);
  StopNeutralinoMap.insert(std::make_pair(mp53, mpp53));

  MassPoint mp54 (300, 100);
  StopCrossSection cs54 (8.51615, 1.18564);
  MassPointParameters mpp54 (cs54, 975157);
  StopNeutralinoMap.insert(std::make_pair(mp54, mpp54));

  MassPoint mp55 (300, 125);
  StopCrossSection cs55 (8.51615, 1.18564);
  MassPointParameters mpp55 (cs55, 966457);
  StopNeutralinoMap.insert(std::make_pair(mp55, mpp55));

  MassPoint mp56 (300, 150);
  StopCrossSection cs56 (8.51615, 1.18564);
  MassPointParameters mpp56 (cs56, 970794);
  StopNeutralinoMap.insert(std::make_pair(mp56, mpp56));

  MassPoint mp57 (300, 175);
  StopCrossSection cs57 (8.51615, 1.18564);
  MassPointParameters mpp57 (cs57, 980640);
  StopNeutralinoMap.insert(std::make_pair(mp57, mpp57));

  MassPoint mp58 (300, 200);
  StopCrossSection cs58 (8.51615, 1.18564);
  MassPointParameters mpp58 (cs58, 962116);
  StopNeutralinoMap.insert(std::make_pair(mp58, mpp58));

  MassPoint mp59 (300, 213);
  StopCrossSection cs59 (8.51615, 1.18564);
  MassPointParameters mpp59 (cs59, 960632);
  StopNeutralinoMap.insert(std::make_pair(mp59, mpp59));

  MassPoint mp60 (308, 125);
  StopCrossSection cs60 (7.43297, 1.03471);
  MassPointParameters mpp60 (cs60, 954600);
  StopNeutralinoMap.insert(std::make_pair(mp60, mpp60));

  MassPoint mp61 (317, 150);
  StopCrossSection cs61 (6.39537, 0.887432);
  MassPointParameters mpp61 (cs61, 962749);
  StopNeutralinoMap.insert(std::make_pair(mp61, mpp61));

  MassPoint mp62 (325, 25);
  StopCrossSection cs62 (5.60471, 0.774257);
  MassPointParameters mpp62 (cs62, 952721);
  StopNeutralinoMap.insert(std::make_pair(mp62, mpp62));

  MassPoint mp63 (325, 50);
  StopCrossSection cs63 (5.60471, 0.774257);
  MassPointParameters mpp63 (cs63, 962633);
  StopNeutralinoMap.insert(std::make_pair(mp63, mpp63));

  MassPoint mp64 (325, 75);
  StopCrossSection cs64 (5.60471, 0.774257);
  MassPointParameters mpp64 (cs64, 948917);
  StopNeutralinoMap.insert(std::make_pair(mp64, mpp64));

  MassPoint mp65 (325, 100);
  StopCrossSection cs65 (5.60471, 0.774257);
  MassPointParameters mpp65 (cs65, 960304);
  StopNeutralinoMap.insert(std::make_pair(mp65, mpp65));

  MassPoint mp66 (325, 125);
  StopCrossSection cs66 (5.60471, 0.774257);
  MassPointParameters mpp66 (cs66, 930616);
  StopNeutralinoMap.insert(std::make_pair(mp66, mpp66));

  MassPoint mp67 (325, 150);
  StopCrossSection cs67 (5.60471, 0.774257);
  MassPointParameters mpp67 (cs67, 956900);
  StopNeutralinoMap.insert(std::make_pair(mp67, mpp67));

  MassPoint mp68 (325, 175);
  StopCrossSection cs68 (5.60471, 0.774257);
  MassPointParameters mpp68 (cs68, 950666);
  StopNeutralinoMap.insert(std::make_pair(mp68, mpp68));

  MassPoint mp69 (325, 200);
  StopCrossSection cs69 (5.60471, 0.774257);
  MassPointParameters mpp69 (cs69, 934111);
  StopNeutralinoMap.insert(std::make_pair(mp69, mpp69));

  MassPoint mp70 (325, 225);
  StopCrossSection cs70 (5.60471, 0.774257);
  MassPointParameters mpp70 (cs70, 958895);
  StopNeutralinoMap.insert(std::make_pair(mp70, mpp70));

  MassPoint mp71 (325, 238);
  StopCrossSection cs71 (5.60471, 0.774257);
  MassPointParameters mpp71 (cs71, 957518);
  StopNeutralinoMap.insert(std::make_pair(mp71, mpp71));

  MassPoint mp72 (333, 150);
  StopCrossSection cs72 (4.93598, 0.677722);
  MassPointParameters mpp72 (cs72, 908690);
  StopNeutralinoMap.insert(std::make_pair(mp72, mpp72));

  MassPoint mp73 (342, 175);
  StopCrossSection cs73 (4.2853, 0.589713);
  MassPointParameters mpp73 (cs73, 919345);
  StopNeutralinoMap.insert(std::make_pair(mp73, mpp73));

  MassPoint mp74 (350, 1);
  StopCrossSection cs74 (3.78661, 0.5183);
  MassPointParameters mpp74 (cs74, 907345);
  StopNeutralinoMap.insert(std::make_pair(mp74, mpp74));

  MassPoint mp75 (350, 50);
  StopCrossSection cs75 (3.78661, 0.5183);
  MassPointParameters mpp75 (cs75, 898779);
  StopNeutralinoMap.insert(std::make_pair(mp75, mpp75));

  MassPoint mp76 (350, 75);
  StopCrossSection cs76 (3.78661, 0.5183);
  MassPointParameters mpp76 (cs76, 910911);
  StopNeutralinoMap.insert(std::make_pair(mp76, mpp76));

  MassPoint mp77 (350, 100);
  StopCrossSection cs77 (3.78661, 0.5183);
  MassPointParameters mpp77 (cs77, 900543);
  StopNeutralinoMap.insert(std::make_pair(mp77, mpp77));

  MassPoint mp78 (350, 125);
  StopCrossSection cs78 (3.78661, 0.5183);
  MassPointParameters mpp78 (cs78, 901150);
  StopNeutralinoMap.insert(std::make_pair(mp78, mpp78));

  MassPoint mp79 (350, 150);
  StopCrossSection cs79 (3.78661, 0.5183);
  MassPointParameters mpp79 (cs79, 905247);
  StopNeutralinoMap.insert(std::make_pair(mp79, mpp79));

  MassPoint mp80 (350, 175);
  StopCrossSection cs80 (3.78661, 0.5183);
  MassPointParameters mpp80 (cs80, 883578);
  StopNeutralinoMap.insert(std::make_pair(mp80, mpp80));

  MassPoint mp81 (350, 200);
  StopCrossSection cs81 (3.78661, 0.5183);
  MassPointParameters mpp81 (cs81, 894275);
  StopNeutralinoMap.insert(std::make_pair(mp81, mpp81));

  MassPoint mp82 (350, 225);
  StopCrossSection cs82 (3.78661, 0.5183);
  MassPointParameters mpp82 (cs82, 894541);
  StopNeutralinoMap.insert(std::make_pair(mp82, mpp82));

  MassPoint mp83 (350, 250);
  StopCrossSection cs83 (3.78661, 0.5183);
  MassPointParameters mpp83 (cs83, 896650);
  StopNeutralinoMap.insert(std::make_pair(mp83, mpp83));

  MassPoint mp84 (350, 263);
  StopCrossSection cs84 (3.78661, 0.5183);
  MassPointParameters mpp84 (cs84, 907384);
  StopNeutralinoMap.insert(std::make_pair(mp84, mpp84));

  MassPoint mp85 (358, 175);
  StopCrossSection cs85 (3.35736, 0.463444);
  MassPointParameters mpp85 (cs85, 882289);
  StopNeutralinoMap.insert(std::make_pair(mp85, mpp85));

  MassPoint mp86 (367, 200);
  StopCrossSection cs86 (2.93791, 0.403858);
  MassPointParameters mpp86 (cs86, 885117);
  StopNeutralinoMap.insert(std::make_pair(mp86, mpp86));

  MassPoint mp87 (375, 75);
  StopCrossSection cs87 (2.61162, 0.361649);
  MassPointParameters mpp87 (cs87, 856669);
  StopNeutralinoMap.insert(std::make_pair(mp87, mpp87));

  MassPoint mp88 (375, 100);
  StopCrossSection cs88 (2.61162, 0.361649);
  MassPointParameters mpp88 (cs88, 882061);
  StopNeutralinoMap.insert(std::make_pair(mp88, mpp88));

  MassPoint mp89 (375, 125);
  StopCrossSection cs89 (2.61162, 0.361649);
  MassPointParameters mpp89 (cs89, 890334);
  StopNeutralinoMap.insert(std::make_pair(mp89, mpp89));

  MassPoint mp90 (375, 150);
  StopCrossSection cs90 (2.61162, 0.361649);
  MassPointParameters mpp90 (cs90, 891883);
  StopNeutralinoMap.insert(std::make_pair(mp90, mpp90));

  MassPoint mp91 (375, 175);
  StopCrossSection cs91 (2.61162, 0.361649);
  MassPointParameters mpp91 (cs91, 865630);
  StopNeutralinoMap.insert(std::make_pair(mp91, mpp91));

  MassPoint mp92 (375, 200);
  StopCrossSection cs92 (2.61162, 0.361649);
  MassPointParameters mpp92 (cs92, 913123);
  StopNeutralinoMap.insert(std::make_pair(mp92, mpp92));

  MassPoint mp93 (375, 225);
  StopCrossSection cs93 (2.61162, 0.361649);
  MassPointParameters mpp93 (cs93, 890175);
  StopNeutralinoMap.insert(std::make_pair(mp93, mpp93));

  MassPoint mp94 (375, 250);
  StopCrossSection cs94 (2.61162, 0.361649);
  MassPointParameters mpp94 (cs94, 883829);
  StopNeutralinoMap.insert(std::make_pair(mp94, mpp94));

  MassPoint mp95 (375, 275);
  StopCrossSection cs95 (2.61162, 0.361649);
  MassPointParameters mpp95 (cs95, 889172);
  StopNeutralinoMap.insert(std::make_pair(mp95, mpp95));

  MassPoint mp96 (375, 288);
  StopCrossSection cs96 (2.61162, 0.361649);
  MassPointParameters mpp96 (cs96, 877750);
  StopNeutralinoMap.insert(std::make_pair(mp96, mpp96));

  MassPoint mp97 (383, 200);
  StopCrossSection cs97 (2.33031, 0.319632);
  MassPointParameters mpp97 (cs97, 816009);
  StopNeutralinoMap.insert(std::make_pair(mp97, mpp97));

  MassPoint mp98 (392, 225);
  StopCrossSection cs98 (2.05132, 0.279655);
  MassPointParameters mpp98 (cs98, 712720);
  StopNeutralinoMap.insert(std::make_pair(mp98, mpp98));

  MassPoint mp99 (400, 1);
  StopCrossSection cs99 (1.83537, 0.251418);
  MassPointParameters mpp99 (cs99, 698496);
  StopNeutralinoMap.insert(std::make_pair(mp99, mpp99));

  MassPoint mp100 (400, 50);
  StopCrossSection cs100 (1.83537, 0.251418);
  MassPointParameters mpp100 (cs100, 701993);
  StopNeutralinoMap.insert(std::make_pair(mp100, mpp100));

  MassPoint mp101 (400, 100);
  StopCrossSection cs101 (1.83537, 0.251418);
  MassPointParameters mpp101 (cs101, 685608);
  StopNeutralinoMap.insert(std::make_pair(mp101, mpp101));

  MassPoint mp102 (400, 125);
  StopCrossSection cs102 (1.83537, 0.251418);
  MassPointParameters mpp102 (cs102, 696052);
  StopNeutralinoMap.insert(std::make_pair(mp102, mpp102));

  MassPoint mp103 (400, 150);
  StopCrossSection cs103 (1.83537, 0.251418);
  MassPointParameters mpp103 (cs103, 703616);
  StopNeutralinoMap.insert(std::make_pair(mp103, mpp103));

  MassPoint mp104 (400, 175);
  StopCrossSection cs104 (1.83537, 0.251418);
  MassPointParameters mpp104 (cs104, 697935);
  StopNeutralinoMap.insert(std::make_pair(mp104, mpp104));

  MassPoint mp105 (400, 200);
  StopCrossSection cs105 (1.83537, 0.251418);
  MassPointParameters mpp105 (cs105, 701844);
  StopNeutralinoMap.insert(std::make_pair(mp105, mpp105));

  MassPoint mp106 (400, 225);
  StopCrossSection cs106 (1.83537, 0.251418);
  MassPointParameters mpp106 (cs106, 701488);
  StopNeutralinoMap.insert(std::make_pair(mp106, mpp106));

  MassPoint mp107 (400, 250);
  StopCrossSection cs107 (1.83537, 0.251418);
  MassPointParameters mpp107 (cs107, 697102);
  StopNeutralinoMap.insert(std::make_pair(mp107, mpp107));

  MassPoint mp108 (400, 275);
  StopCrossSection cs108 (1.83537, 0.251418);
  MassPointParameters mpp108 (cs108, 686788);
  StopNeutralinoMap.insert(std::make_pair(mp108, mpp108));

  MassPoint mp109 (400, 300);
  StopCrossSection cs109 (1.83537, 0.251418);
  MassPointParameters mpp109 (cs109, 704446);
  StopNeutralinoMap.insert(std::make_pair(mp109, mpp109));

  MassPoint mp110 (400, 313);
  StopCrossSection cs110 (1.83537, 0.251418);
  MassPointParameters mpp110 (cs110, 699713);
  StopNeutralinoMap.insert(std::make_pair(mp110, mpp110));

  MassPoint mp111 (408, 225);
  StopCrossSection cs111 (1.64598, 0.224102);
  MassPointParameters mpp111 (cs111, 629945);
  StopNeutralinoMap.insert(std::make_pair(mp111, mpp111));

  MassPoint mp112 (417, 250);
  StopCrossSection cs112 (1.45754, 0.197237);
  MassPointParameters mpp112 (cs112, 554977);
  StopNeutralinoMap.insert(std::make_pair(mp112, mpp112));

  MassPoint mp113 (425, 125);
  StopCrossSection cs113 (1.31169, 0.177095);
  MassPointParameters mpp113 (cs113, 480381);
  StopNeutralinoMap.insert(std::make_pair(mp113, mpp113));

  MassPoint mp114 (425, 150);
  StopCrossSection cs114 (1.31169, 0.177095);
  MassPointParameters mpp114 (cs114, 482995);
  StopNeutralinoMap.insert(std::make_pair(mp114, mpp114));

  MassPoint mp115 (425, 175);
  StopCrossSection cs115 (1.31169, 0.177095);
  MassPointParameters mpp115 (cs115, 481395);
  StopNeutralinoMap.insert(std::make_pair(mp115, mpp115));

  MassPoint mp116 (425, 200);
  StopCrossSection cs116 (1.31169, 0.177095);
  MassPointParameters mpp116 (cs116, 503771);
  StopNeutralinoMap.insert(std::make_pair(mp116, mpp116));

  MassPoint mp117 (425, 225);
  StopCrossSection cs117 (1.31169, 0.177095);
  MassPointParameters mpp117 (cs117, 486428);
  StopNeutralinoMap.insert(std::make_pair(mp117, mpp117));

  MassPoint mp118 (425, 250);
  StopCrossSection cs118 (1.31169, 0.177095);
  MassPointParameters mpp118 (cs118, 494006);
  StopNeutralinoMap.insert(std::make_pair(mp118, mpp118));

  MassPoint mp119 (425, 275);
  StopCrossSection cs119 (1.31169, 0.177095);
  MassPointParameters mpp119 (cs119, 481194);
  StopNeutralinoMap.insert(std::make_pair(mp119, mpp119));

  MassPoint mp120 (425, 300);
  StopCrossSection cs120 (1.31169, 0.177095);
  MassPointParameters mpp120 (cs120, 487595);
  StopNeutralinoMap.insert(std::make_pair(mp120, mpp120));

  MassPoint mp121 (425, 325);
  StopCrossSection cs121 (1.31169, 0.177095);
  MassPointParameters mpp121 (cs121, 482190);
  StopNeutralinoMap.insert(std::make_pair(mp121, mpp121));

  MassPoint mp122 (425, 338);
  StopCrossSection cs122 (1.31169, 0.177095);
  MassPointParameters mpp122 (cs122, 471920);
  StopNeutralinoMap.insert(std::make_pair(mp122, mpp122));

  MassPoint mp123 (433, 250);
  StopCrossSection cs123 (1.17767, 0.15845);
  MassPointParameters mpp123 (cs123, 441590);
  StopNeutralinoMap.insert(std::make_pair(mp123, mpp123));

  MassPoint mp124 (442, 275);
  StopCrossSection cs124 (1.04898, 0.142727);
  MassPointParameters mpp124 (cs124, 372555);
  StopNeutralinoMap.insert(std::make_pair(mp124, mpp124));

  MassPoint mp125 (450, 1);
  StopCrossSection cs125 (0.948333, 0.127607);
  MassPointParameters mpp125 (cs125, 345199);
  StopNeutralinoMap.insert(std::make_pair(mp125, mpp125));

  MassPoint mp126 (450, 50);
  StopCrossSection cs126 (0.948333, 0.127607);
  MassPointParameters mpp126 (cs126, 360986);
  StopNeutralinoMap.insert(std::make_pair(mp126, mpp126));

  MassPoint mp127 (450, 100);
  StopCrossSection cs127 (0.948333, 0.127607);
  MassPointParameters mpp127 (cs127, 340979);
  StopNeutralinoMap.insert(std::make_pair(mp127, mpp127));

  MassPoint mp128 (450, 150);
  StopCrossSection cs128 (0.948333, 0.127607);
  MassPointParameters mpp128 (cs128, 348945);
  StopNeutralinoMap.insert(std::make_pair(mp128, mpp128));

  MassPoint mp129 (450, 175);
  StopCrossSection cs129 (0.948333, 0.127607);
  MassPointParameters mpp129 (cs129, 355531);
  StopNeutralinoMap.insert(std::make_pair(mp129, mpp129));

  MassPoint mp130 (450, 200);
  StopCrossSection cs130 (0.948333, 0.127607);
  MassPointParameters mpp130 (cs130, 338247);
  StopNeutralinoMap.insert(std::make_pair(mp130, mpp130));

  MassPoint mp131 (450, 225);
  StopCrossSection cs131 (0.948333, 0.127607);
  MassPointParameters mpp131 (cs131, 353388);
  StopNeutralinoMap.insert(std::make_pair(mp131, mpp131));

  MassPoint mp132 (450, 250);
  StopCrossSection cs132 (0.948333, 0.127607);
  MassPointParameters mpp132 (cs132, 342538);
  StopNeutralinoMap.insert(std::make_pair(mp132, mpp132));

  MassPoint mp133 (450, 275);
  StopCrossSection cs133 (0.948333, 0.127607);
  MassPointParameters mpp133 (cs133, 341142);
  StopNeutralinoMap.insert(std::make_pair(mp133, mpp133));

  MassPoint mp134 (450, 300);
  StopCrossSection cs134 (0.948333, 0.127607);
  MassPointParameters mpp134 (cs134, 347312);
  StopNeutralinoMap.insert(std::make_pair(mp134, mpp134));

  MassPoint mp135 (450, 325);
  StopCrossSection cs135 (0.948333, 0.127607);
  MassPointParameters mpp135 (cs135, 331272);
  StopNeutralinoMap.insert(std::make_pair(mp135, mpp135));

  MassPoint mp136 (450, 350);
  StopCrossSection cs136 (0.948333, 0.127607);
  MassPointParameters mpp136 (cs136, 345538);
  StopNeutralinoMap.insert(std::make_pair(mp136, mpp136));

  MassPoint mp137 (450, 363);
  StopCrossSection cs137 (0.948333, 0.127607);
  MassPointParameters mpp137 (cs137, 341472);
  StopNeutralinoMap.insert(std::make_pair(mp137, mpp137));

  MassPoint mp138 (458, 275);
  StopCrossSection cs138 (0.858396, 0.115469);
  MassPointParameters mpp138 (cs138, 320588);
  StopNeutralinoMap.insert(std::make_pair(mp138, mpp138));

  MassPoint mp139 (467, 300);
  StopCrossSection cs139 (0.768552, 0.103094);
  MassPointParameters mpp139 (cs139, 277504);
  StopNeutralinoMap.insert(std::make_pair(mp139, mpp139));

  MassPoint mp140 (475, 175);
  StopCrossSection cs140 (0.697075, 0.0933565);
  MassPointParameters mpp140 (cs140, 249037);
  StopNeutralinoMap.insert(std::make_pair(mp140, mpp140));

  MassPoint mp141 (475, 200);
  StopCrossSection cs141 (0.697075, 0.0933565);
  MassPointParameters mpp141 (cs141, 245396);
  StopNeutralinoMap.insert(std::make_pair(mp141, mpp141));

  MassPoint mp142 (475, 225);
  StopCrossSection cs142 (0.697075, 0.0933565);
  MassPointParameters mpp142 (cs142, 252206);
  StopNeutralinoMap.insert(std::make_pair(mp142, mpp142));

  MassPoint mp143 (475, 250);
  StopCrossSection cs143 (0.697075, 0.0933565);
  MassPointParameters mpp143 (cs143, 250149);
  StopNeutralinoMap.insert(std::make_pair(mp143, mpp143));

  MassPoint mp144 (475, 275);
  StopCrossSection cs144 (0.697075, 0.0933565);
  MassPointParameters mpp144 (cs144, 259063);
  StopNeutralinoMap.insert(std::make_pair(mp144, mpp144));

  MassPoint mp145 (475, 300);
  StopCrossSection cs145 (0.697075, 0.0933565);
  MassPointParameters mpp145 (cs145, 238918);
  StopNeutralinoMap.insert(std::make_pair(mp145, mpp145));

  MassPoint mp146 (475, 325);
  StopCrossSection cs146 (0.697075, 0.0933565);
  MassPointParameters mpp146 (cs146, 255027);
  StopNeutralinoMap.insert(std::make_pair(mp146, mpp146));

  MassPoint mp147 (475, 350);
  StopCrossSection cs147 (0.697075, 0.0933565);
  MassPointParameters mpp147 (cs147, 258151);
  StopNeutralinoMap.insert(std::make_pair(mp147, mpp147));

  MassPoint mp148 (475, 375);
  StopCrossSection cs148 (0.697075, 0.0933565);
  MassPointParameters mpp148 (cs148, 246829);
  StopNeutralinoMap.insert(std::make_pair(mp148, mpp148));

  MassPoint mp149 (475, 388);
  StopCrossSection cs149 (0.697075, 0.0933565);
  MassPointParameters mpp149 (cs149, 243991);
  StopNeutralinoMap.insert(std::make_pair(mp149, mpp149));

  MassPoint mp150 (483, 300);
  StopCrossSection cs150 (0.633519, 0.0848849);
  MassPointParameters mpp150 (cs150, 236700);
  StopNeutralinoMap.insert(std::make_pair(mp150, mpp150));

  MassPoint mp151 (492, 325);
  StopCrossSection cs151 (0.56929, 0.0761869);
  MassPointParameters mpp151 (cs151, 200396);
  StopNeutralinoMap.insert(std::make_pair(mp151, mpp151));

  MassPoint mp152 (500, 1);
  StopCrossSection cs152 (0.51848, 0.0693711);
  MassPointParameters mpp152 (cs152, 201790);
  StopNeutralinoMap.insert(std::make_pair(mp152, mpp152));

  MassPoint mp153 (500, 50);
  StopCrossSection cs153 (0.51848, 0.0693711);
  MassPointParameters mpp153 (cs153, 204320);
  StopNeutralinoMap.insert(std::make_pair(mp153, mpp153));

  MassPoint mp154 (500, 100);
  StopCrossSection cs154 (0.51848, 0.0693711);
  MassPointParameters mpp154 (cs154, 193841);
  StopNeutralinoMap.insert(std::make_pair(mp154, mpp154));

  MassPoint mp155 (500, 150);
  StopCrossSection cs155 (0.51848, 0.0693711);
  MassPointParameters mpp155 (cs155, 196526);
  StopNeutralinoMap.insert(std::make_pair(mp155, mpp155));

  MassPoint mp156 (500, 200);
  StopCrossSection cs156 (0.51848, 0.0693711);
  MassPointParameters mpp156 (cs156, 198566);
  StopNeutralinoMap.insert(std::make_pair(mp156, mpp156));

  MassPoint mp157 (500, 225);
  StopCrossSection cs157 (0.51848, 0.0693711);
  MassPointParameters mpp157 (cs157, 202707);
  StopNeutralinoMap.insert(std::make_pair(mp157, mpp157));

  MassPoint mp158 (500, 250);
  StopCrossSection cs158 (0.51848, 0.0693711);
  MassPointParameters mpp158 (cs158, 193604);
  StopNeutralinoMap.insert(std::make_pair(mp158, mpp158));

  MassPoint mp159 (500, 275);
  StopCrossSection cs159 (0.51848, 0.0693711);
  MassPointParameters mpp159 (cs159, 201321);
  StopNeutralinoMap.insert(std::make_pair(mp159, mpp159));

  MassPoint mp160 (500, 300);
  StopCrossSection cs160 (0.51848, 0.0693711);
  MassPointParameters mpp160 (cs160, 191044);
  StopNeutralinoMap.insert(std::make_pair(mp160, mpp160));

  MassPoint mp161 (500, 325);
  StopCrossSection cs161 (0.51848, 0.0693711);
  MassPointParameters mpp161 (cs161, 190896);
  StopNeutralinoMap.insert(std::make_pair(mp161, mpp161));

  MassPoint mp162 (500, 350);
  StopCrossSection cs162 (0.51848, 0.0693711);
  MassPointParameters mpp162 (cs162, 190200);
  StopNeutralinoMap.insert(std::make_pair(mp162, mpp162));

  MassPoint mp163 (500, 375);
  StopCrossSection cs163 (0.51848, 0.0693711);
  MassPointParameters mpp163 (cs163, 188442);
  StopNeutralinoMap.insert(std::make_pair(mp163, mpp163));

  MassPoint mp164 (500, 400);
  StopCrossSection cs164 (0.51848, 0.0693711);
  MassPointParameters mpp164 (cs164, 196993);
  StopNeutralinoMap.insert(std::make_pair(mp164, mpp164));

  MassPoint mp165 (500, 413);
  StopCrossSection cs165 (0.51848, 0.0693711);
  MassPointParameters mpp165 (cs165, 189000);
  StopNeutralinoMap.insert(std::make_pair(mp165, mpp165));

  MassPoint mp166 (508, 325);
  StopCrossSection cs166 (0.473193, 0.0630664);
  MassPointParameters mpp166 (cs166, 184242);
  StopNeutralinoMap.insert(std::make_pair(mp166, mpp166));

  MassPoint mp167 (517, 350);
  StopCrossSection cs167 (0.42723, 0.0569597);
  MassPointParameters mpp167 (cs167, 163267);
  StopNeutralinoMap.insert(std::make_pair(mp167, mpp167));

  MassPoint mp168 (525, 225);
  StopCrossSection cs168 (0.390303, 0.0520832);
  MassPointParameters mpp168 (cs168, 149626);
  StopNeutralinoMap.insert(std::make_pair(mp168, mpp168));

  MassPoint mp169 (525, 250);
  StopCrossSection cs169 (0.390303, 0.0520832);
  MassPointParameters mpp169 (cs169, 146799);
  StopNeutralinoMap.insert(std::make_pair(mp169, mpp169));

  MassPoint mp170 (525, 275);
  StopCrossSection cs170 (0.390303, 0.0520832);
  MassPointParameters mpp170 (cs170, 153427);
  StopNeutralinoMap.insert(std::make_pair(mp170, mpp170));

  MassPoint mp171 (525, 300);
  StopCrossSection cs171 (0.390303, 0.0520832);
  MassPointParameters mpp171 (cs171, 154624);
  StopNeutralinoMap.insert(std::make_pair(mp171, mpp171));

  MassPoint mp172 (525, 325);
  StopCrossSection cs172 (0.390303, 0.0520832);
  MassPointParameters mpp172 (cs172, 138642);
  StopNeutralinoMap.insert(std::make_pair(mp172, mpp172));

  MassPoint mp173 (525, 350);
  StopCrossSection cs173 (0.390303, 0.0520832);
  MassPointParameters mpp173 (cs173, 149976);
  StopNeutralinoMap.insert(std::make_pair(mp173, mpp173));

  MassPoint mp174 (525, 375);
  StopCrossSection cs174 (0.390303, 0.0520832);
  MassPointParameters mpp174 (cs174, 148360);
  StopNeutralinoMap.insert(std::make_pair(mp174, mpp174));

  MassPoint mp175 (525, 400);
  StopCrossSection cs175 (0.390303, 0.0520832);
  MassPointParameters mpp175 (cs175, 142655);
  StopNeutralinoMap.insert(std::make_pair(mp175, mpp175));

  MassPoint mp176 (525, 425);
  StopCrossSection cs176 (0.390303, 0.0520832);
  MassPointParameters mpp176 (cs176, 138180);
  StopNeutralinoMap.insert(std::make_pair(mp176, mpp176));

  MassPoint mp177 (525, 438);
  StopCrossSection cs177 (0.390303, 0.0520832);
  MassPointParameters mpp177 (cs177, 134106);
  StopNeutralinoMap.insert(std::make_pair(mp177, mpp177));

  MassPoint mp178 (533, 350);
  StopCrossSection cs178 (0.356725, 0.0474963);
  MassPointParameters mpp178 (cs178, 148390);
  StopNeutralinoMap.insert(std::make_pair(mp178, mpp178));

  MassPoint mp179 (542, 375);
  StopCrossSection cs179 (0.323163, 0.0429481);
  MassPointParameters mpp179 (cs179, 120506);
  StopNeutralinoMap.insert(std::make_pair(mp179, mpp179));

  MassPoint mp180 (550, 1);
  StopCrossSection cs180 (0.296128, 0.0392923);
  MassPointParameters mpp180 (cs180, 114315);
  StopNeutralinoMap.insert(std::make_pair(mp180, mpp180));

  MassPoint mp181 (550, 50);
  StopCrossSection cs181 (0.296128, 0.0392923);
  MassPointParameters mpp181 (cs181, 109953);
  StopNeutralinoMap.insert(std::make_pair(mp181, mpp181));

  MassPoint mp182 (550, 100);
  StopCrossSection cs182 (0.296128, 0.0392923);
  MassPointParameters mpp182 (cs182, 106444);
  StopNeutralinoMap.insert(std::make_pair(mp182, mpp182));

  MassPoint mp183 (550, 150);
  StopCrossSection cs183 (0.296128, 0.0392923);
  MassPointParameters mpp183 (cs183, 114667);
  StopNeutralinoMap.insert(std::make_pair(mp183, mpp183));

  MassPoint mp184 (550, 200);
  StopCrossSection cs184 (0.296128, 0.0392923);
  MassPointParameters mpp184 (cs184, 107117);
  StopNeutralinoMap.insert(std::make_pair(mp184, mpp184));

  MassPoint mp185 (550, 250);
  StopCrossSection cs185 (0.296128, 0.0392923);
  MassPointParameters mpp185 (cs185, 106871);
  StopNeutralinoMap.insert(std::make_pair(mp185, mpp185));

  MassPoint mp186 (550, 275);
  StopCrossSection cs186 (0.296128, 0.0392923);
  MassPointParameters mpp186 (cs186, 114239);
  StopNeutralinoMap.insert(std::make_pair(mp186, mpp186));

  MassPoint mp187 (550, 300);
  StopCrossSection cs187 (0.296128, 0.0392923);
  MassPointParameters mpp187 (cs187, 105867);
  StopNeutralinoMap.insert(std::make_pair(mp187, mpp187));

  MassPoint mp188 (550, 325);
  StopCrossSection cs188 (0.296128, 0.0392923);
  MassPointParameters mpp188 (cs188, 109650);
  StopNeutralinoMap.insert(std::make_pair(mp188, mpp188));

  MassPoint mp189 (550, 350);
  StopCrossSection cs189 (0.296128, 0.0392923);
  MassPointParameters mpp189 (cs189, 121720);
  StopNeutralinoMap.insert(std::make_pair(mp189, mpp189));

  MassPoint mp190 (550, 375);
  StopCrossSection cs190 (0.296128, 0.0392923);
  MassPointParameters mpp190 (cs190, 111063);
  StopNeutralinoMap.insert(std::make_pair(mp190, mpp190));

  MassPoint mp191 (550, 400);
  StopCrossSection cs191 (0.296128, 0.0392923);
  MassPointParameters mpp191 (cs191, 120961);
  StopNeutralinoMap.insert(std::make_pair(mp191, mpp191));

  MassPoint mp192 (550, 425);
  StopCrossSection cs192 (0.296128, 0.0392923);
  MassPointParameters mpp192 (cs192, 110705);
  StopNeutralinoMap.insert(std::make_pair(mp192, mpp192));

  MassPoint mp193 (550, 450);
  StopCrossSection cs193 (0.296128, 0.0392923);
  MassPointParameters mpp193 (cs193, 106443);
  StopNeutralinoMap.insert(std::make_pair(mp193, mpp193));

  MassPoint mp194 (550, 463);
  StopCrossSection cs194 (0.296128, 0.0392923);
  MassPointParameters mpp194 (cs194, 114809);
  StopNeutralinoMap.insert(std::make_pair(mp194, mpp194));

  MassPoint mp195 (558, 375);
  StopCrossSection cs195 (0.271976, 0.0359305);
  MassPointParameters mpp195 (cs195, 100950);
  StopNeutralinoMap.insert(std::make_pair(mp195, mpp195));

  MassPoint mp196 (567, 400);
  StopCrossSection cs196 (0.246349, 0.0326119);
  MassPointParameters mpp196 (cs196, 89804);
  StopNeutralinoMap.insert(std::make_pair(mp196, mpp196));

  MassPoint mp197 (575, 275);
  StopCrossSection cs197 (0.226118, 0.0300151);
  MassPointParameters mpp197 (cs197, 85820);
  StopNeutralinoMap.insert(std::make_pair(mp197, mpp197));

  MassPoint mp198 (575, 300);
  StopCrossSection cs198 (0.226118, 0.0300151);
  MassPointParameters mpp198 (cs198, 91177);
  StopNeutralinoMap.insert(std::make_pair(mp198, mpp198));

  MassPoint mp199 (575, 325);
  StopCrossSection cs199 (0.226118, 0.0300151);
  MassPointParameters mpp199 (cs199, 84924);
  StopNeutralinoMap.insert(std::make_pair(mp199, mpp199));

  MassPoint mp200 (575, 350);
  StopCrossSection cs200 (0.226118, 0.0300151);
  MassPointParameters mpp200 (cs200, 87200);
  StopNeutralinoMap.insert(std::make_pair(mp200, mpp200));

  MassPoint mp201 (575, 375);
  StopCrossSection cs201 (0.226118, 0.0300151);
  MassPointParameters mpp201 (cs201, 80105);
  StopNeutralinoMap.insert(std::make_pair(mp201, mpp201));

  MassPoint mp202 (575, 400);
  StopCrossSection cs202 (0.226118, 0.0300151);
  MassPointParameters mpp202 (cs202, 90596);
  StopNeutralinoMap.insert(std::make_pair(mp202, mpp202));

  MassPoint mp203 (575, 425);
  StopCrossSection cs203 (0.226118, 0.0300151);
  MassPointParameters mpp203 (cs203, 79896);
  StopNeutralinoMap.insert(std::make_pair(mp203, mpp203));

  MassPoint mp204 (575, 450);
  StopCrossSection cs204 (0.226118, 0.0300151);
  MassPointParameters mpp204 (cs204, 84527);
  StopNeutralinoMap.insert(std::make_pair(mp204, mpp204));

  MassPoint mp205 (575, 475);
  StopCrossSection cs205 (0.226118, 0.0300151);
  MassPointParameters mpp205 (cs205, 80622);
  StopNeutralinoMap.insert(std::make_pair(mp205, mpp205));

  MassPoint mp206 (575, 488);
  StopCrossSection cs206 (0.226118, 0.0300151);
  MassPointParameters mpp206 (cs206, 76205);
  StopNeutralinoMap.insert(std::make_pair(mp206, mpp206));

  MassPoint mp207 (583, 400);
  StopCrossSection cs207 (0.207962, 0.0275786);
  MassPointParameters mpp207 (cs207, 83316);
  StopNeutralinoMap.insert(std::make_pair(mp207, mpp207));

  MassPoint mp208 (592, 425);
  StopCrossSection cs208 (0.189289, 0.024915);
  MassPointParameters mpp208 (cs208, 64819);
  StopNeutralinoMap.insert(std::make_pair(mp208, mpp208));

  MassPoint mp209 (600, 1);
  StopCrossSection cs209 (0.174599, 0.02306);
  MassPointParameters mpp209 (cs209, 65444);
  StopNeutralinoMap.insert(std::make_pair(mp209, mpp209));

  MassPoint mp210 (600, 50);
  StopCrossSection cs210 (0.174599, 0.02306);
  MassPointParameters mpp210 (cs210, 67689);
  StopNeutralinoMap.insert(std::make_pair(mp210, mpp210));

  MassPoint mp211 (600, 100);
  StopCrossSection cs211 (0.174599, 0.02306);
  MassPointParameters mpp211 (cs211, 77545);
  StopNeutralinoMap.insert(std::make_pair(mp211, mpp211));

  MassPoint mp212 (600, 150);
  StopCrossSection cs212 (0.174599, 0.02306);
  MassPointParameters mpp212 (cs212, 70701);
  StopNeutralinoMap.insert(std::make_pair(mp212, mpp212));

  MassPoint mp213 (600, 200);
  StopCrossSection cs213 (0.174599, 0.02306);
  MassPointParameters mpp213 (cs213, 61618);
  StopNeutralinoMap.insert(std::make_pair(mp213, mpp213));

  MassPoint mp214 (600, 250);
  StopCrossSection cs214 (0.174599, 0.02306);
  MassPointParameters mpp214 (cs214, 72780);
  StopNeutralinoMap.insert(std::make_pair(mp214, mpp214));

  MassPoint mp215 (600, 300);
  StopCrossSection cs215 (0.174599, 0.02306);
  MassPointParameters mpp215 (cs215, 72083);
  StopNeutralinoMap.insert(std::make_pair(mp215, mpp215));

  MassPoint mp216 (600, 325);
  StopCrossSection cs216 (0.174599, 0.02306);
  MassPointParameters mpp216 (cs216, 67829);
  StopNeutralinoMap.insert(std::make_pair(mp216, mpp216));

  MassPoint mp217 (600, 350);
  StopCrossSection cs217 (0.174599, 0.02306);
  MassPointParameters mpp217 (cs217, 70760);
  StopNeutralinoMap.insert(std::make_pair(mp217, mpp217));

  MassPoint mp218 (600, 375);
  StopCrossSection cs218 (0.174599, 0.02306);
  MassPointParameters mpp218 (cs218, 64281);
  StopNeutralinoMap.insert(std::make_pair(mp218, mpp218));

  MassPoint mp219 (600, 400);
  StopCrossSection cs219 (0.174599, 0.02306);
  MassPointParameters mpp219 (cs219, 68997);
  StopNeutralinoMap.insert(std::make_pair(mp219, mpp219));

  MassPoint mp220 (600, 425);
  StopCrossSection cs220 (0.174599, 0.02306);
  MassPointParameters mpp220 (cs220, 74713);
  StopNeutralinoMap.insert(std::make_pair(mp220, mpp220));

  MassPoint mp221 (600, 450);
  StopCrossSection cs221 (0.174599, 0.02306);
  MassPointParameters mpp221 (cs221, 72238);
  StopNeutralinoMap.insert(std::make_pair(mp221, mpp221));

  MassPoint mp222 (600, 475);
  StopCrossSection cs222 (0.174599, 0.02306);
  MassPointParameters mpp222 (cs222, 73545);
  StopNeutralinoMap.insert(std::make_pair(mp222, mpp222));

  MassPoint mp223 (600, 500);
  StopCrossSection cs223 (0.174599, 0.02306);
  MassPointParameters mpp223 (cs223, 62982);
  StopNeutralinoMap.insert(std::make_pair(mp223, mpp223));

  MassPoint mp224 (600, 513);
  StopCrossSection cs224 (0.174599, 0.02306);
  MassPointParameters mpp224 (cs224, 70385);
  StopNeutralinoMap.insert(std::make_pair(mp224, mpp224));

  MassPoint mp225 (608, 425);
  StopCrossSection cs225 (0.161398, 0.0211267);
  MassPointParameters mpp225 (cs225, 57022);
  StopNeutralinoMap.insert(std::make_pair(mp225, mpp225));

  MassPoint mp226 (617, 450);
  StopCrossSection cs226 (0.14728, 0.01944);
  MassPointParameters mpp226 (cs226, 61131);
  StopNeutralinoMap.insert(std::make_pair(mp226, mpp226));

  MassPoint mp227 (625, 325);
  StopCrossSection cs227 (0.136372, 0.0174504);
  MassPointParameters mpp227 (cs227, 56053);
  StopNeutralinoMap.insert(std::make_pair(mp227, mpp227));

  MassPoint mp228 (625, 350);
  StopCrossSection cs228 (0.136372, 0.0174504);
  MassPointParameters mpp228 (cs228, 55670);
  StopNeutralinoMap.insert(std::make_pair(mp228, mpp228));

  MassPoint mp229 (625, 375);
  StopCrossSection cs229 (0.136372, 0.0174504);
  MassPointParameters mpp229 (cs229, 51913);
  StopNeutralinoMap.insert(std::make_pair(mp229, mpp229));

  MassPoint mp230 (625, 400);
  StopCrossSection cs230 (0.136372, 0.0174504);
  MassPointParameters mpp230 (cs230, 52024);
  StopNeutralinoMap.insert(std::make_pair(mp230, mpp230));

  MassPoint mp231 (625, 425);
  StopCrossSection cs231 (0.136372, 0.0174504);
  MassPointParameters mpp231 (cs231, 58821);
  StopNeutralinoMap.insert(std::make_pair(mp231, mpp231));

  MassPoint mp232 (625, 450);
  StopCrossSection cs232 (0.136372, 0.0174504);
  MassPointParameters mpp232 (cs232, 57771);
  StopNeutralinoMap.insert(std::make_pair(mp232, mpp232));

  MassPoint mp233 (625, 475);
  StopCrossSection cs233 (0.136372, 0.0174504);
  MassPointParameters mpp233 (cs233, 53659);
  StopNeutralinoMap.insert(std::make_pair(mp233, mpp233));

  MassPoint mp234 (625, 500);
  StopCrossSection cs234 (0.136372, 0.0174504);
  MassPointParameters mpp234 (cs234, 51350);
  StopNeutralinoMap.insert(std::make_pair(mp234, mpp234));

  MassPoint mp235 (625, 525);
  StopCrossSection cs235 (0.136372, 0.0174504);
  MassPointParameters mpp235 (cs235, 55581);
  StopNeutralinoMap.insert(std::make_pair(mp235, mpp235));

  MassPoint mp236 (625, 538);
  StopCrossSection cs236 (0.136372, 0.0174504);
  MassPointParameters mpp236 (cs236, 54603);
  StopNeutralinoMap.insert(std::make_pair(mp236, mpp236));

  MassPoint mp237 (633, 450);
  StopCrossSection cs237 (0.125996, 0.0165449);
  MassPointParameters mpp237 (cs237, 48772);
  StopNeutralinoMap.insert(std::make_pair(mp237, mpp237));

  MassPoint mp238 (642, 475);
  StopCrossSection cs238 (0.115573, 0.0147355);
  MassPointParameters mpp238 (cs238, 52247);
  StopNeutralinoMap.insert(std::make_pair(mp238, mpp238));

  MassPoint mp239 (650, 1);
  StopCrossSection cs239 (0.107045, 0.0138336);
  MassPointParameters mpp239 (cs239, 39561);
  StopNeutralinoMap.insert(std::make_pair(mp239, mpp239));

  MassPoint mp240 (650, 50);
  StopCrossSection cs240 (0.107045, 0.0138336);
  MassPointParameters mpp240 (cs240, 41932);
  StopNeutralinoMap.insert(std::make_pair(mp240, mpp240));

  MassPoint mp241 (650, 100);
  StopCrossSection cs241 (0.107045, 0.0138336);
  MassPointParameters mpp241 (cs241, 39789);
  StopNeutralinoMap.insert(std::make_pair(mp241, mpp241));

  MassPoint mp242 (650, 150);
  StopCrossSection cs242 (0.107045, 0.0138336);
  MassPointParameters mpp242 (cs242, 45693);
  StopNeutralinoMap.insert(std::make_pair(mp242, mpp242));

  MassPoint mp243 (650, 200);
  StopCrossSection cs243 (0.107045, 0.0138336);
  MassPointParameters mpp243 (cs243, 40678);
  StopNeutralinoMap.insert(std::make_pair(mp243, mpp243));

  MassPoint mp244 (650, 250);
  StopCrossSection cs244 (0.107045, 0.0138336);
  MassPointParameters mpp244 (cs244, 45334);
  StopNeutralinoMap.insert(std::make_pair(mp244, mpp244));

  MassPoint mp245 (650, 300);
  StopCrossSection cs245 (0.107045, 0.0138336);
  MassPointParameters mpp245 (cs245, 39559);
  StopNeutralinoMap.insert(std::make_pair(mp245, mpp245));

  MassPoint mp246 (650, 350);
  StopCrossSection cs246 (0.107045, 0.0138336);
  MassPointParameters mpp246 (cs246, 42018);
  StopNeutralinoMap.insert(std::make_pair(mp246, mpp246));

  MassPoint mp247 (650, 375);
  StopCrossSection cs247 (0.107045, 0.0138336);
  MassPointParameters mpp247 (cs247, 39344);
  StopNeutralinoMap.insert(std::make_pair(mp247, mpp247));

  MassPoint mp248 (650, 400);
  StopCrossSection cs248 (0.107045, 0.0138336);
  MassPointParameters mpp248 (cs248, 42883);
  StopNeutralinoMap.insert(std::make_pair(mp248, mpp248));

  MassPoint mp249 (650, 425);
  StopCrossSection cs249 (0.107045, 0.0138336);
  MassPointParameters mpp249 (cs249, 40883);
  StopNeutralinoMap.insert(std::make_pair(mp249, mpp249));

  MassPoint mp250 (650, 450);
  StopCrossSection cs250 (0.107045, 0.0138336);
  MassPointParameters mpp250 (cs250, 41270);
  StopNeutralinoMap.insert(std::make_pair(mp250, mpp250));

  MassPoint mp251 (650, 475);
  StopCrossSection cs251 (0.107045, 0.0138336);
  MassPointParameters mpp251 (cs251, 42292);
  StopNeutralinoMap.insert(std::make_pair(mp251, mpp251));

  MassPoint mp252 (650, 500);
  StopCrossSection cs252 (0.107045, 0.0138336);
  MassPointParameters mpp252 (cs252, 39517);
  StopNeutralinoMap.insert(std::make_pair(mp252, mpp252));

  MassPoint mp253 (650, 525);
  StopCrossSection cs253 (0.107045, 0.0138336);
  MassPointParameters mpp253 (cs253, 42652);
  StopNeutralinoMap.insert(std::make_pair(mp253, mpp253));

  MassPoint mp254 (650, 550);
  StopCrossSection cs254 (0.107045, 0.0138336);
  MassPointParameters mpp254 (cs254, 41964);
  StopNeutralinoMap.insert(std::make_pair(mp254, mpp254));

  MassPoint mp255 (650, 563);
  StopCrossSection cs255 (0.107045, 0.0138336);
  MassPointParameters mpp255 (cs255, 40391);
  StopNeutralinoMap.insert(std::make_pair(mp255, mpp255));

  MassPoint mp256 (658, 475);
  StopCrossSection cs256 (0.0991824, 0.0128381);
  MassPointParameters mpp256 (cs256, 39792);
  StopNeutralinoMap.insert(std::make_pair(mp256, mpp256));

  MassPoint mp257 (667, 500);
  StopCrossSection cs257 (0.0910543, 0.0118196);
  MassPointParameters mpp257 (cs257, 35953);
  StopNeutralinoMap.insert(std::make_pair(mp257, mpp257));

  MassPoint mp258 (675, 375);
  StopCrossSection cs258 (0.0844877, 0.0110428);
  MassPointParameters mpp258 (cs258, 32183);
  StopNeutralinoMap.insert(std::make_pair(mp258, mpp258));

  MassPoint mp259 (675, 400);
  StopCrossSection cs259 (0.0844877, 0.0110428);
  MassPointParameters mpp259 (cs259, 32162);
  StopNeutralinoMap.insert(std::make_pair(mp259, mpp259));

  MassPoint mp260 (675, 425);
  StopCrossSection cs260 (0.0844877, 0.0110428);
  MassPointParameters mpp260 (cs260, 33487);
  StopNeutralinoMap.insert(std::make_pair(mp260, mpp260));

  MassPoint mp261 (675, 450);
  StopCrossSection cs261 (0.0844877, 0.0110428);
  MassPointParameters mpp261 (cs261, 33464);
  StopNeutralinoMap.insert(std::make_pair(mp261, mpp261));

  MassPoint mp262 (675, 475);
  StopCrossSection cs262 (0.0844877, 0.0110428);
  MassPointParameters mpp262 (cs262, 33683);
  StopNeutralinoMap.insert(std::make_pair(mp262, mpp262));

  MassPoint mp263 (675, 500);
  StopCrossSection cs263 (0.0844877, 0.0110428);
  MassPointParameters mpp263 (cs263, 34330);
  StopNeutralinoMap.insert(std::make_pair(mp263, mpp263));

  MassPoint mp264 (675, 525);
  StopCrossSection cs264 (0.0844877, 0.0110428);
  MassPointParameters mpp264 (cs264, 37133);
  StopNeutralinoMap.insert(std::make_pair(mp264, mpp264));

  MassPoint mp265 (675, 550);
  StopCrossSection cs265 (0.0844877, 0.0110428);
  MassPointParameters mpp265 (cs265, 30405);
  StopNeutralinoMap.insert(std::make_pair(mp265, mpp265));

  MassPoint mp266 (675, 575);
  StopCrossSection cs266 (0.0844877, 0.0110428);
  MassPointParameters mpp266 (cs266, 32896);
  StopNeutralinoMap.insert(std::make_pair(mp266, mpp266));

  MassPoint mp267 (675, 588);
  StopCrossSection cs267 (0.0844877, 0.0110428);
  MassPointParameters mpp267 (cs267, 33017);
  StopNeutralinoMap.insert(std::make_pair(mp267, mpp267));

  MassPoint mp268 (683, 500);
  StopCrossSection cs268 (0.0783936, 0.0102976);
  MassPointParameters mpp268 (cs268, 25776);
  StopNeutralinoMap.insert(std::make_pair(mp268, mpp268));

  MassPoint mp269 (692, 525);
  StopCrossSection cs269 (0.0721663, 0.00956121);
  MassPointParameters mpp269 (cs269, 25630);
  StopNeutralinoMap.insert(std::make_pair(mp269, mpp269));

  MassPoint mp270 (700, 1);
  StopCrossSection cs270 (0.0670476, 0.00894609);
  MassPointParameters mpp270 (cs270, 26111);
  StopNeutralinoMap.insert(std::make_pair(mp270, mpp270));

  MassPoint mp271 (700, 50);
  StopCrossSection cs271 (0.0670476, 0.00894609);
  MassPointParameters mpp271 (cs271, 27465);
  StopNeutralinoMap.insert(std::make_pair(mp271, mpp271));

  MassPoint mp272 (700, 100);
  StopCrossSection cs272 (0.0670476, 0.00894609);
  MassPointParameters mpp272 (cs272, 25649);
  StopNeutralinoMap.insert(std::make_pair(mp272, mpp272));

  MassPoint mp273 (700, 150);
  StopCrossSection cs273 (0.0670476, 0.00894609);
  MassPointParameters mpp273 (cs273, 20171);
  StopNeutralinoMap.insert(std::make_pair(mp273, mpp273));

  MassPoint mp274 (700, 200);
  StopCrossSection cs274 (0.0670476, 0.00894609);
  MassPointParameters mpp274 (cs274, 22547);
  StopNeutralinoMap.insert(std::make_pair(mp274, mpp274));

  MassPoint mp275 (700, 250);
  StopCrossSection cs275 (0.0670476, 0.00894609);
  MassPointParameters mpp275 (cs275, 26741);
  StopNeutralinoMap.insert(std::make_pair(mp275, mpp275));

  MassPoint mp276 (700, 300);
  StopCrossSection cs276 (0.0670476, 0.00894609);
  MassPointParameters mpp276 (cs276, 28242);
  StopNeutralinoMap.insert(std::make_pair(mp276, mpp276));

  MassPoint mp277 (700, 350);
  StopCrossSection cs277 (0.0670476, 0.00894609);
  MassPointParameters mpp277 (cs277, 23129);
  StopNeutralinoMap.insert(std::make_pair(mp277, mpp277));

  MassPoint mp278 (700, 400);
  StopCrossSection cs278 (0.0670476, 0.00894609);
  MassPointParameters mpp278 (cs278, 23085);
  StopNeutralinoMap.insert(std::make_pair(mp278, mpp278));

  MassPoint mp279 (700, 425);
  StopCrossSection cs279 (0.0670476, 0.00894609);
  MassPointParameters mpp279 (cs279, 25370);
  StopNeutralinoMap.insert(std::make_pair(mp279, mpp279));

  MassPoint mp280 (700, 450);
  StopCrossSection cs280 (0.0670476, 0.00894609);
  MassPointParameters mpp280 (cs280, 24735);
  StopNeutralinoMap.insert(std::make_pair(mp280, mpp280));

  MassPoint mp281 (700, 475);
  StopCrossSection cs281 (0.0670476, 0.00894609);
  MassPointParameters mpp281 (cs281, 26015);
  StopNeutralinoMap.insert(std::make_pair(mp281, mpp281));

  MassPoint mp282 (700, 500);
  StopCrossSection cs282 (0.0670476, 0.00894609);
  MassPointParameters mpp282 (cs282, 26271);
  StopNeutralinoMap.insert(std::make_pair(mp282, mpp282));

  MassPoint mp283 (700, 525);
  StopCrossSection cs283 (0.0670476, 0.00894609);
  MassPointParameters mpp283 (cs283, 26332);
  StopNeutralinoMap.insert(std::make_pair(mp283, mpp283));

  MassPoint mp284 (700, 550);
  StopCrossSection cs284 (0.0670476, 0.00894609);
  MassPointParameters mpp284 (cs284, 20934);
  StopNeutralinoMap.insert(std::make_pair(mp284, mpp284));

  MassPoint mp285 (700, 575);
  StopCrossSection cs285 (0.0670476, 0.00894609);
  MassPointParameters mpp285 (cs285, 27086);
  StopNeutralinoMap.insert(std::make_pair(mp285, mpp285));

  MassPoint mp286 (700, 600);
  StopCrossSection cs286 (0.0670476, 0.00894609);
  MassPointParameters mpp286 (cs286, 24494);
  StopNeutralinoMap.insert(std::make_pair(mp286, mpp286));

  MassPoint mp287 (700, 613);
  StopCrossSection cs287 (0.0670476, 0.00894609);
  MassPointParameters mpp287 (cs287, 26506);
  StopNeutralinoMap.insert(std::make_pair(mp287, mpp287));

  MassPoint mp288 (708, 525);
  StopCrossSection cs288 (0.0624336, 0.00835443);
  MassPointParameters mpp288 (cs288, 27069);
  StopNeutralinoMap.insert(std::make_pair(mp288, mpp288));

  MassPoint mp289 (717, 550);
  StopCrossSection cs289 (0.0575708, 0.00775986);
  MassPointParameters mpp289 (cs289, 23062);
  StopNeutralinoMap.insert(std::make_pair(mp289, mpp289));

  MassPoint mp290 (725, 425);
  StopCrossSection cs290 (0.0536438, 0.00728504);
  MassPointParameters mpp290 (cs290, 18160);
  StopNeutralinoMap.insert(std::make_pair(mp290, mpp290));

  MassPoint mp291 (725, 450);
  StopCrossSection cs291 (0.0536438, 0.00728504);
  MassPointParameters mpp291 (cs291, 22343);
  StopNeutralinoMap.insert(std::make_pair(mp291, mpp291));

  MassPoint mp292 (725, 475);
  StopCrossSection cs292 (0.0536438, 0.00728504);
  MassPointParameters mpp292 (cs292, 20603);
  StopNeutralinoMap.insert(std::make_pair(mp292, mpp292));

  MassPoint mp293 (725, 500);
  StopCrossSection cs293 (0.0536438, 0.00728504);
  MassPointParameters mpp293 (cs293, 22427);
  StopNeutralinoMap.insert(std::make_pair(mp293, mpp293));

  MassPoint mp294 (725, 525);
  StopCrossSection cs294 (0.0536438, 0.00728504);
  MassPointParameters mpp294 (cs294, 24821);
  StopNeutralinoMap.insert(std::make_pair(mp294, mpp294));

  MassPoint mp295 (725, 550);
  StopCrossSection cs295 (0.0536438, 0.00728504);
  MassPointParameters mpp295 (cs295, 22408);
  StopNeutralinoMap.insert(std::make_pair(mp295, mpp295));

  MassPoint mp296 (725, 575);
  StopCrossSection cs296 (0.0536438, 0.00728504);
  MassPointParameters mpp296 (cs296, 19038);
  StopNeutralinoMap.insert(std::make_pair(mp296, mpp296));

  MassPoint mp297 (725, 600);
  StopCrossSection cs297 (0.0536438, 0.00728504);
  MassPointParameters mpp297 (cs297, 22627);
  StopNeutralinoMap.insert(std::make_pair(mp297, mpp297));

  MassPoint mp298 (725, 625);
  StopCrossSection cs298 (0.0536438, 0.00728504);
  MassPointParameters mpp298 (cs298, 22703);
  StopNeutralinoMap.insert(std::make_pair(mp298, mpp298));

  MassPoint mp299 (725, 638);
  StopCrossSection cs299 (0.0536438, 0.00728504);
  MassPointParameters mpp299 (cs299, 22341);
  StopNeutralinoMap.insert(std::make_pair(mp299, mpp299));

  MassPoint mp300 (733, 550);
  StopCrossSection cs300 (0.0499888, 0.00679985);
  MassPointParameters mpp300 (cs300, 17080);
  StopNeutralinoMap.insert(std::make_pair(mp300, mpp300));

  MassPoint mp301 (742, 575);
  StopCrossSection cs301 (0.0462725, 0.00633304);
  MassPointParameters mpp301 (cs301, 21237);
  StopNeutralinoMap.insert(std::make_pair(mp301, mpp301));

  MassPoint mp302 (750, 1);
  StopCrossSection cs302 (0.0431418, 0.00593006);
  MassPointParameters mpp302 (cs302, 18315);
  StopNeutralinoMap.insert(std::make_pair(mp302, mpp302));

  MassPoint mp303 (750, 50);
  StopCrossSection cs303 (0.0431418, 0.00593006);
  MassPointParameters mpp303 (cs303, 18103);
  StopNeutralinoMap.insert(std::make_pair(mp303, mpp303));

  MassPoint mp304 (750, 100);
  StopCrossSection cs304 (0.0431418, 0.00593006);
  MassPointParameters mpp304 (cs304, 22648);
  StopNeutralinoMap.insert(std::make_pair(mp304, mpp304));

  MassPoint mp305 (750, 150);
  StopCrossSection cs305 (0.0431418, 0.00593006);
  MassPointParameters mpp305 (cs305, 18839);
  StopNeutralinoMap.insert(std::make_pair(mp305, mpp305));

  MassPoint mp306 (750, 200);
  StopCrossSection cs306 (0.0431418, 0.00593006);
  MassPointParameters mpp306 (cs306, 18964);
  StopNeutralinoMap.insert(std::make_pair(mp306, mpp306));

  MassPoint mp307 (750, 250);
  StopCrossSection cs307 (0.0431418, 0.00593006);
  MassPointParameters mpp307 (cs307, 17522);
  StopNeutralinoMap.insert(std::make_pair(mp307, mpp307));

  MassPoint mp308 (750, 300);
  StopCrossSection cs308 (0.0431418, 0.00593006);
  MassPointParameters mpp308 (cs308, 21378);
  StopNeutralinoMap.insert(std::make_pair(mp308, mpp308));

  MassPoint mp309 (750, 350);
  StopCrossSection cs309 (0.0431418, 0.00593006);
  MassPointParameters mpp309 (cs309, 17361);
  StopNeutralinoMap.insert(std::make_pair(mp309, mpp309));

  MassPoint mp310 (750, 400);
  StopCrossSection cs310 (0.0431418, 0.00593006);
  MassPointParameters mpp310 (cs310, 18457);
  StopNeutralinoMap.insert(std::make_pair(mp310, mpp310));

  MassPoint mp311 (750, 450);
  StopCrossSection cs311 (0.0431418, 0.00593006);
  MassPointParameters mpp311 (cs311, 19891);
  StopNeutralinoMap.insert(std::make_pair(mp311, mpp311));

  MassPoint mp312 (750, 475);
  StopCrossSection cs312 (0.0431418, 0.00593006);
  MassPointParameters mpp312 (cs312, 18658);
  StopNeutralinoMap.insert(std::make_pair(mp312, mpp312));

  MassPoint mp313 (750, 500);
  StopCrossSection cs313 (0.0431418, 0.00593006);
  MassPointParameters mpp313 (cs313, 19258);
  StopNeutralinoMap.insert(std::make_pair(mp313, mpp313));

  MassPoint mp314 (750, 525);
  StopCrossSection cs314 (0.0431418, 0.00593006);
  MassPointParameters mpp314 (cs314, 15547);
  StopNeutralinoMap.insert(std::make_pair(mp314, mpp314));

  MassPoint mp315 (750, 550);
  StopCrossSection cs315 (0.0431418, 0.00593006);
  MassPointParameters mpp315 (cs315, 20206);
  StopNeutralinoMap.insert(std::make_pair(mp315, mpp315));

  MassPoint mp316 (750, 575);
  StopCrossSection cs316 (0.0431418, 0.00593006);
  MassPointParameters mpp316 (cs316, 20945);
  StopNeutralinoMap.insert(std::make_pair(mp316, mpp316));

  MassPoint mp317 (750, 600);
  StopCrossSection cs317 (0.0431418, 0.00593006);
  MassPointParameters mpp317 (cs317, 20420);
  StopNeutralinoMap.insert(std::make_pair(mp317, mpp317));

  MassPoint mp318 (750, 625);
  StopCrossSection cs318 (0.0431418, 0.00593006);
  MassPointParameters mpp318 (cs318, 23907);
  StopNeutralinoMap.insert(std::make_pair(mp318, mpp318));

  MassPoint mp319 (750, 650);
  StopCrossSection cs319 (0.0431418, 0.00593006);
  MassPointParameters mpp319 (cs319, 16680);
  StopNeutralinoMap.insert(std::make_pair(mp319, mpp319));

  MassPoint mp320 (758, 575);
  StopCrossSection cs320 (0.0403137, 0.00557285);
  MassPointParameters mpp320 (cs320, 22200);
  StopNeutralinoMap.insert(std::make_pair(mp320, mpp320));

  MassPoint mp321 (767, 600);
  StopCrossSection cs321 (0.0372964, 0.00517853);
  MassPointParameters mpp321 (cs321, 19945);
  StopNeutralinoMap.insert(std::make_pair(mp321, mpp321));

  MassPoint mp322 (775, 475);
  StopCrossSection cs322 (0.0348796, 0.00486909);
  MassPointParameters mpp322 (cs322, 18999);
  StopNeutralinoMap.insert(std::make_pair(mp322, mpp322));

  MassPoint mp323 (775, 500);
  StopCrossSection cs323 (0.0348796, 0.00486909);
  MassPointParameters mpp323 (cs323, 17163);
  StopNeutralinoMap.insert(std::make_pair(mp323, mpp323));

  MassPoint mp324 (775, 525);
  StopCrossSection cs324 (0.0348796, 0.00486909);
  MassPointParameters mpp324 (cs324, 17133);
  StopNeutralinoMap.insert(std::make_pair(mp324, mpp324));

  MassPoint mp325 (775, 550);
  StopCrossSection cs325 (0.0348796, 0.00486909);
  MassPointParameters mpp325 (cs325, 15054);
  StopNeutralinoMap.insert(std::make_pair(mp325, mpp325));

  MassPoint mp326 (775, 575);
  StopCrossSection cs326 (0.0348796, 0.00486909);
  MassPointParameters mpp326 (cs326, 19633);
  StopNeutralinoMap.insert(std::make_pair(mp326, mpp326));

  MassPoint mp327 (775, 600);
  StopCrossSection cs327 (0.0348796, 0.00486909);
  MassPointParameters mpp327 (cs327, 17283);
  StopNeutralinoMap.insert(std::make_pair(mp327, mpp327));

  MassPoint mp328 (775, 625);
  StopCrossSection cs328 (0.0348796, 0.00486909);
  MassPointParameters mpp328 (cs328, 20492);
  StopNeutralinoMap.insert(std::make_pair(mp328, mpp328));

  MassPoint mp329 (775, 650);
  StopCrossSection cs329 (0.0348796, 0.00486909);
  MassPointParameters mpp329 (cs329, 18582);
  StopNeutralinoMap.insert(std::make_pair(mp329, mpp329));

  MassPoint mp330 (783, 600);
  StopCrossSection cs330 (0.0326196, 0.00457813);
  MassPointParameters mpp330 (cs330, 18739);
  StopNeutralinoMap.insert(std::make_pair(mp330, mpp330));

  MassPoint mp331 (792, 625);
  StopCrossSection cs331 (0.0302563, 0.00427359);
  MassPointParameters mpp331 (cs331, 16790);
  StopNeutralinoMap.insert(std::make_pair(mp331, mpp331));

  MassPoint mp332 (800, 1);
  StopCrossSection cs332 (0.0283338, 0.00401518);
  MassPointParameters mpp332 (cs332, 18533);
  StopNeutralinoMap.insert(std::make_pair(mp332, mpp332));

  MassPoint mp333 (800, 50);
  StopCrossSection cs333 (0.0283338, 0.00401518);
  MassPointParameters mpp333 (cs333, 18635);
  StopNeutralinoMap.insert(std::make_pair(mp333, mpp333));

  MassPoint mp334 (800, 100);
  StopCrossSection cs334 (0.0283338, 0.00401518);
  MassPointParameters mpp334 (cs334, 23126);
  StopNeutralinoMap.insert(std::make_pair(mp334, mpp334));

  MassPoint mp335 (800, 150);
  StopCrossSection cs335 (0.0283338, 0.00401518);
  MassPointParameters mpp335 (cs335, 19048);
  StopNeutralinoMap.insert(std::make_pair(mp335, mpp335));

  MassPoint mp336 (800, 200);
  StopCrossSection cs336 (0.0283338, 0.00401518);
  MassPointParameters mpp336 (cs336, 18703);
  StopNeutralinoMap.insert(std::make_pair(mp336, mpp336));

  MassPoint mp337 (800, 250);
  StopCrossSection cs337 (0.0283338, 0.00401518);
  MassPointParameters mpp337 (cs337, 22584);
  StopNeutralinoMap.insert(std::make_pair(mp337, mpp337));

  MassPoint mp338 (800, 300);
  StopCrossSection cs338 (0.0283338, 0.00401518);
  MassPointParameters mpp338 (cs338, 16831);
  StopNeutralinoMap.insert(std::make_pair(mp338, mpp338));

  MassPoint mp339 (800, 350);
  StopCrossSection cs339 (0.0283338, 0.00401518);
  MassPointParameters mpp339 (cs339, 18809);
  StopNeutralinoMap.insert(std::make_pair(mp339, mpp339));

  MassPoint mp340 (800, 400);
  StopCrossSection cs340 (0.0283338, 0.00401518);
  MassPointParameters mpp340 (cs340, 19428);
  StopNeutralinoMap.insert(std::make_pair(mp340, mpp340));

  MassPoint mp341 (800, 450);
  StopCrossSection cs341 (0.0283338, 0.00401518);
  MassPointParameters mpp341 (cs341, 19534);
  StopNeutralinoMap.insert(std::make_pair(mp341, mpp341));

  MassPoint mp342 (800, 500);
  StopCrossSection cs342 (0.0283338, 0.00401518);
  MassPointParameters mpp342 (cs342, 21312);
  StopNeutralinoMap.insert(std::make_pair(mp342, mpp342));

  MassPoint mp343 (800, 525);
  StopCrossSection cs343 (0.0283338, 0.00401518);
  MassPointParameters mpp343 (cs343, 17702);
  StopNeutralinoMap.insert(std::make_pair(mp343, mpp343));

  MassPoint mp344 (800, 550);
  StopCrossSection cs344 (0.0283338, 0.00401518);
  MassPointParameters mpp344 (cs344, 18987);
  StopNeutralinoMap.insert(std::make_pair(mp344, mpp344));

  MassPoint mp345 (800, 575);
  StopCrossSection cs345 (0.0283338, 0.00401518);
  MassPointParameters mpp345 (cs345, 15937);
  StopNeutralinoMap.insert(std::make_pair(mp345, mpp345));

  MassPoint mp346 (800, 600);
  StopCrossSection cs346 (0.0283338, 0.00401518);
  MassPointParameters mpp346 (cs346, 20128);
  StopNeutralinoMap.insert(std::make_pair(mp346, mpp346));

  MassPoint mp347 (800, 625);
  StopCrossSection cs347 (0.0283338, 0.00401518);
  MassPointParameters mpp347 (cs347, 14755);
  StopNeutralinoMap.insert(std::make_pair(mp347, mpp347));

  MassPoint mp348 (800, 650);
  StopCrossSection cs348 (0.0283338, 0.00401518);
  MassPointParameters mpp348 (cs348, 19720);
  StopNeutralinoMap.insert(std::make_pair(mp348, mpp348));

  MassPoint mp349 (808, 625);
  StopCrossSection cs349 (0.0265622, 0.00379026);
  MassPointParameters mpp349 (cs349, 21277);
  StopNeutralinoMap.insert(std::make_pair(mp349, mpp349));

  MassPoint mp350 (817, 650);
  StopCrossSection cs350 (0.0247104, 0.00355087);
  MassPointParameters mpp350 (cs350, 19143);
  StopNeutralinoMap.insert(std::make_pair(mp350, mpp350));

  MassPoint mp351 (825, 525);
  StopCrossSection cs351 (0.0230866, 0.00333435);
  MassPointParameters mpp351 (cs351, 21129);
  StopNeutralinoMap.insert(std::make_pair(mp351, mpp351));

  MassPoint mp352 (825, 550);
  StopCrossSection cs352 (0.0230866, 0.00333435);
  MassPointParameters mpp352 (cs352, 19159);
  StopNeutralinoMap.insert(std::make_pair(mp352, mpp352));

  MassPoint mp353 (825, 575);
  StopCrossSection cs353 (0.0230866, 0.00333435);
  MassPointParameters mpp353 (cs353, 18458);
  StopNeutralinoMap.insert(std::make_pair(mp353, mpp353));

  MassPoint mp354 (825, 600);
  StopCrossSection cs354 (0.0230866, 0.00333435);
  MassPointParameters mpp354 (cs354, 20323);
  StopNeutralinoMap.insert(std::make_pair(mp354, mpp354));

  MassPoint mp355 (825, 625);
  StopCrossSection cs355 (0.0230866, 0.00333435);
  MassPointParameters mpp355 (cs355, 19352);
  StopNeutralinoMap.insert(std::make_pair(mp355, mpp355));

  MassPoint mp356 (825, 650);
  StopCrossSection cs356 (0.0230866, 0.00333435);
  MassPointParameters mpp356 (cs356, 19509);
  StopNeutralinoMap.insert(std::make_pair(mp356, mpp356));

  MassPoint mp357 (833, 650);
  StopCrossSection cs357 (0.0216993, 0.0031511);
  MassPointParameters mpp357 (cs357, 21277);
  StopNeutralinoMap.insert(std::make_pair(mp357, mpp357));

  MassPoint mp358 (850, 1);
  StopCrossSection cs358 (0.0189612, 0.00278768);
  MassPointParameters mpp358 (cs358, 18872);
  StopNeutralinoMap.insert(std::make_pair(mp358, mpp358));

  MassPoint mp359 (850, 50);
  StopCrossSection cs359 (0.0189612, 0.00278768);
  MassPointParameters mpp359 (cs359, 21196);
  StopNeutralinoMap.insert(std::make_pair(mp359, mpp359));

  MassPoint mp360 (850, 100);
  StopCrossSection cs360 (0.0189612, 0.00278768);
  MassPointParameters mpp360 (cs360, 18684);
  StopNeutralinoMap.insert(std::make_pair(mp360, mpp360));

  MassPoint mp361 (850, 150);
  StopCrossSection cs361 (0.0189612, 0.00278768);
  MassPointParameters mpp361 (cs361, 19904);
  StopNeutralinoMap.insert(std::make_pair(mp361, mpp361));

  MassPoint mp362 (850, 200);
  StopCrossSection cs362 (0.0189612, 0.00278768);
  MassPointParameters mpp362 (cs362, 17242);
  StopNeutralinoMap.insert(std::make_pair(mp362, mpp362));

  MassPoint mp363 (850, 250);
  StopCrossSection cs363 (0.0189612, 0.00278768);
  MassPointParameters mpp363 (cs363, 17776);
  StopNeutralinoMap.insert(std::make_pair(mp363, mpp363));

  MassPoint mp364 (850, 300);
  StopCrossSection cs364 (0.0189612, 0.00278768);
  MassPointParameters mpp364 (cs364, 18397);
  StopNeutralinoMap.insert(std::make_pair(mp364, mpp364));

  MassPoint mp365 (850, 350);
  StopCrossSection cs365 (0.0189612, 0.00278768);
  MassPointParameters mpp365 (cs365, 19063);
  StopNeutralinoMap.insert(std::make_pair(mp365, mpp365));

  MassPoint mp366 (850, 400);
  StopCrossSection cs366 (0.0189612, 0.00278768);
  MassPointParameters mpp366 (cs366, 16887);
  StopNeutralinoMap.insert(std::make_pair(mp366, mpp366));

  MassPoint mp367 (850, 450);
  StopCrossSection cs367 (0.0189612, 0.00278768);
  MassPointParameters mpp367 (cs367, 21780);
  StopNeutralinoMap.insert(std::make_pair(mp367, mpp367));

  MassPoint mp368 (850, 500);
  StopCrossSection cs368 (0.0189612, 0.00278768);
  MassPointParameters mpp368 (cs368, 21169);
  StopNeutralinoMap.insert(std::make_pair(mp368, mpp368));

  MassPoint mp369 (850, 550);
  StopCrossSection cs369 (0.0189612, 0.00278768);
  MassPointParameters mpp369 (cs369, 18999);
  StopNeutralinoMap.insert(std::make_pair(mp369, mpp369));

  MassPoint mp370 (850, 575);
  StopCrossSection cs370 (0.0189612, 0.00278768);
  MassPointParameters mpp370 (cs370, 16400);
  StopNeutralinoMap.insert(std::make_pair(mp370, mpp370));

  MassPoint mp371 (850, 600);
  StopCrossSection cs371 (0.0189612, 0.00278768);
  MassPointParameters mpp371 (cs371, 14517);
  StopNeutralinoMap.insert(std::make_pair(mp371, mpp371));

  MassPoint mp372 (850, 625);
  StopCrossSection cs372 (0.0189612, 0.00278768);
  MassPointParameters mpp372 (cs372, 18129);
  StopNeutralinoMap.insert(std::make_pair(mp372, mpp372));

  MassPoint mp373 (850, 650);
  StopCrossSection cs373 (0.0189612, 0.00278768);
  MassPointParameters mpp373 (cs373, 19614);
  StopNeutralinoMap.insert(std::make_pair(mp373, mpp373));

  MassPoint mp374 (875, 575);
  StopCrossSection cs374 (0.015625, 0.00233698);
  MassPointParameters mpp374 (cs374, 17851);
  StopNeutralinoMap.insert(std::make_pair(mp374, mpp374));

  MassPoint mp375 (875, 600);
  StopCrossSection cs375 (0.015625, 0.00233698);
  MassPointParameters mpp375 (cs375, 20189);
  StopNeutralinoMap.insert(std::make_pair(mp375, mpp375));

  MassPoint mp376 (875, 625);
  StopCrossSection cs376 (0.015625, 0.00233698);
  MassPointParameters mpp376 (cs376, 18632);
  StopNeutralinoMap.insert(std::make_pair(mp376, mpp376));

  MassPoint mp377 (875, 650);
  StopCrossSection cs377 (0.015625, 0.00233698);
  MassPointParameters mpp377 (cs377, 18532);
  StopNeutralinoMap.insert(std::make_pair(mp377, mpp377));

  MassPoint mp378 (900, 1);
  StopCrossSection cs378 (0.0128895, 0.00195954);
  MassPointParameters mpp378 (cs378, 19667);
  StopNeutralinoMap.insert(std::make_pair(mp378, mpp378));

  MassPoint mp379 (900, 50);
  StopCrossSection cs379 (0.0128895, 0.00195954);
  MassPointParameters mpp379 (cs379, 21651);
  StopNeutralinoMap.insert(std::make_pair(mp379, mpp379));

  MassPoint mp380 (900, 100);
  StopCrossSection cs380 (0.0128895, 0.00195954);
  MassPointParameters mpp380 (cs380, 18211);
  StopNeutralinoMap.insert(std::make_pair(mp380, mpp380));

  MassPoint mp381 (900, 150);
  StopCrossSection cs381 (0.0128895, 0.00195954);
  MassPointParameters mpp381 (cs381, 17432);
  StopNeutralinoMap.insert(std::make_pair(mp381, mpp381));

  MassPoint mp382 (900, 200);
  StopCrossSection cs382 (0.0128895, 0.00195954);
  MassPointParameters mpp382 (cs382, 19494);
  StopNeutralinoMap.insert(std::make_pair(mp382, mpp382));

  MassPoint mp383 (900, 250);
  StopCrossSection cs383 (0.0128895, 0.00195954);
  MassPointParameters mpp383 (cs383, 16297);
  StopNeutralinoMap.insert(std::make_pair(mp383, mpp383));

  MassPoint mp384 (900, 300);
  StopCrossSection cs384 (0.0128895, 0.00195954);
  MassPointParameters mpp384 (cs384, 19115);
  StopNeutralinoMap.insert(std::make_pair(mp384, mpp384));

  MassPoint mp385 (900, 350);
  StopCrossSection cs385 (0.0128895, 0.00195954);
  MassPointParameters mpp385 (cs385, 19783);
  StopNeutralinoMap.insert(std::make_pair(mp385, mpp385));

  MassPoint mp386 (900, 400);
  StopCrossSection cs386 (0.0128895, 0.00195954);
  MassPointParameters mpp386 (cs386, 19405);
  StopNeutralinoMap.insert(std::make_pair(mp386, mpp386));

  MassPoint mp387 (900, 450);
  StopCrossSection cs387 (0.0128895, 0.00195954);
  MassPointParameters mpp387 (cs387, 19605);
  StopNeutralinoMap.insert(std::make_pair(mp387, mpp387));

  MassPoint mp388 (900, 500);
  StopCrossSection cs388 (0.0128895, 0.00195954);
  MassPointParameters mpp388 (cs388, 17254);
  StopNeutralinoMap.insert(std::make_pair(mp388, mpp388));

  MassPoint mp389 (900, 550);
  StopCrossSection cs389 (0.0128895, 0.00195954);
  MassPointParameters mpp389 (cs389, 16796);
  StopNeutralinoMap.insert(std::make_pair(mp389, mpp389));

  MassPoint mp390 (900, 600);
  StopCrossSection cs390 (0.0128895, 0.00195954);
  MassPointParameters mpp390 (cs390, 18222);
  StopNeutralinoMap.insert(std::make_pair(mp390, mpp390));

  MassPoint mp391 (900, 625);
  StopCrossSection cs391 (0.0128895, 0.00195954);
  MassPointParameters mpp391 (cs391, 18390);
  StopNeutralinoMap.insert(std::make_pair(mp391, mpp391));

  MassPoint mp392 (900, 650);
  StopCrossSection cs392 (0.0128895, 0.00195954);
  MassPointParameters mpp392 (cs392, 17897);
  StopNeutralinoMap.insert(std::make_pair(mp392, mpp392));

  MassPoint mp393 (925, 625);
  StopCrossSection cs393 (0.0106631, 0.00165071);
  MassPointParameters mpp393 (cs393, 21218);
  StopNeutralinoMap.insert(std::make_pair(mp393, mpp393));

  MassPoint mp394 (925, 650);
  StopCrossSection cs394 (0.0106631, 0.00165071);
  MassPointParameters mpp394 (cs394, 20376);
  StopNeutralinoMap.insert(std::make_pair(mp394, mpp394));

  MassPoint mp395 (950, 1);
  StopCrossSection cs395 (0.00883465, 0.0013886);
  MassPointParameters mpp395 (cs395, 23521);
  StopNeutralinoMap.insert(std::make_pair(mp395, mpp395));

  MassPoint mp396 (950, 50);
  StopCrossSection cs396 (0.00883465, 0.0013886);
  MassPointParameters mpp396 (cs396, 20772);
  StopNeutralinoMap.insert(std::make_pair(mp396, mpp396));

  MassPoint mp397 (950, 100);
  StopCrossSection cs397 (0.00883465, 0.0013886);
  MassPointParameters mpp397 (cs397, 20889);
  StopNeutralinoMap.insert(std::make_pair(mp397, mpp397));

  MassPoint mp398 (950, 150);
  StopCrossSection cs398 (0.00883465, 0.0013886);
  MassPointParameters mpp398 (cs398, 17523);
  StopNeutralinoMap.insert(std::make_pair(mp398, mpp398));

  MassPoint mp399 (950, 200);
  StopCrossSection cs399 (0.00883465, 0.0013886);
  MassPointParameters mpp399 (cs399, 20573);
  StopNeutralinoMap.insert(std::make_pair(mp399, mpp399));

  MassPoint mp400 (950, 250);
  StopCrossSection cs400 (0.00883465, 0.0013886);
  MassPointParameters mpp400 (cs400, 21709);
  StopNeutralinoMap.insert(std::make_pair(mp400, mpp400));

  MassPoint mp401 (950, 300);
  StopCrossSection cs401 (0.00883465, 0.0013886);
  MassPointParameters mpp401 (cs401, 17749);
  StopNeutralinoMap.insert(std::make_pair(mp401, mpp401));

  MassPoint mp402 (950, 350);
  StopCrossSection cs402 (0.00883465, 0.0013886);
  MassPointParameters mpp402 (cs402, 17035);
  StopNeutralinoMap.insert(std::make_pair(mp402, mpp402));

  MassPoint mp403 (950, 400);
  StopCrossSection cs403 (0.00883465, 0.0013886);
  MassPointParameters mpp403 (cs403, 15832);
  StopNeutralinoMap.insert(std::make_pair(mp403, mpp403));

  MassPoint mp404 (950, 450);
  StopCrossSection cs404 (0.00883465, 0.0013886);
  MassPointParameters mpp404 (cs404, 15418);
  StopNeutralinoMap.insert(std::make_pair(mp404, mpp404));

  MassPoint mp405 (950, 500);
  StopCrossSection cs405 (0.00883465, 0.0013886);
  MassPointParameters mpp405 (cs405, 18750);
  StopNeutralinoMap.insert(std::make_pair(mp405, mpp405));

  MassPoint mp406 (950, 550);
  StopCrossSection cs406 (0.00883465, 0.0013886);
  MassPointParameters mpp406 (cs406, 18086);
  StopNeutralinoMap.insert(std::make_pair(mp406, mpp406));

  MassPoint mp407 (950, 600);
  StopCrossSection cs407 (0.00883465, 0.0013886);
  MassPointParameters mpp407 (cs407, 18997);
  StopNeutralinoMap.insert(std::make_pair(mp407, mpp407));

  MassPoint mp408 (950, 650);
  StopCrossSection cs408 (0.00883465, 0.0013886);
  MassPointParameters mpp408 (cs408, 19072);
  StopNeutralinoMap.insert(std::make_pair(mp408, mpp408));

  MassPoint mp409 (1000, 1);
  StopCrossSection cs409 (0.00615134, 0.00100238);
  MassPointParameters mpp409 (cs409, 19536);
  StopNeutralinoMap.insert(std::make_pair(mp409, mpp409));

  MassPoint mp410 (1000, 50);
  StopCrossSection cs410 (0.00615134, 0.00100238);
  MassPointParameters mpp410 (cs410, 17885);
  StopNeutralinoMap.insert(std::make_pair(mp410, mpp410));

  MassPoint mp411 (1000, 100);
  StopCrossSection cs411 (0.00615134, 0.00100238);
  MassPointParameters mpp411 (cs411, 19056);
  StopNeutralinoMap.insert(std::make_pair(mp411, mpp411));

  MassPoint mp412 (1000, 150);
  StopCrossSection cs412 (0.00615134, 0.00100238);
  MassPointParameters mpp412 (cs412, 23375);
  StopNeutralinoMap.insert(std::make_pair(mp412, mpp412));

  MassPoint mp413 (1000, 200);
  StopCrossSection cs413 (0.00615134, 0.00100238);
  MassPointParameters mpp413 (cs413, 15383);
  StopNeutralinoMap.insert(std::make_pair(mp413, mpp413));

  MassPoint mp414 (1000, 250);
  StopCrossSection cs414 (0.00615134, 0.00100238);
  MassPointParameters mpp414 (cs414, 19039);
  StopNeutralinoMap.insert(std::make_pair(mp414, mpp414));

  MassPoint mp415 (1000, 300);
  StopCrossSection cs415 (0.00615134, 0.00100238);
  MassPointParameters mpp415 (cs415, 14698);
  StopNeutralinoMap.insert(std::make_pair(mp415, mpp415));

  MassPoint mp416 (1000, 350);
  StopCrossSection cs416 (0.00615134, 0.00100238);
  MassPointParameters mpp416 (cs416, 17436);
  StopNeutralinoMap.insert(std::make_pair(mp416, mpp416));

  MassPoint mp417 (1000, 400);
  StopCrossSection cs417 (0.00615134, 0.00100238);
  MassPointParameters mpp417 (cs417, 16450);
  StopNeutralinoMap.insert(std::make_pair(mp417, mpp417));

  MassPoint mp418 (1000, 450);
  StopCrossSection cs418 (0.00615134, 0.00100238);
  MassPointParameters mpp418 (cs418, 15813);
  StopNeutralinoMap.insert(std::make_pair(mp418, mpp418));

  MassPoint mp419 (1000, 500);
  StopCrossSection cs419 (0.00615134, 0.00100238);
  MassPointParameters mpp419 (cs419, 17173);
  StopNeutralinoMap.insert(std::make_pair(mp419, mpp419));

  MassPoint mp420 (1000, 550);
  StopCrossSection cs420 (0.00615134, 0.00100238);
  MassPointParameters mpp420 (cs420, 22344);
  StopNeutralinoMap.insert(std::make_pair(mp420, mpp420));

  MassPoint mp421 (1000, 600);
  StopCrossSection cs421 (0.00615134, 0.00100238);
  MassPointParameters mpp421 (cs421, 20898);
  StopNeutralinoMap.insert(std::make_pair(mp421, mpp421));

  MassPoint mp422 (1000, 650);
  StopCrossSection cs422 (0.00615134, 0.00100238);
  MassPointParameters mpp422 (cs422, 25491);
  StopNeutralinoMap.insert(std::make_pair(mp422, mpp422));

  MassPoint mp423 (1050, 1);
  StopCrossSection cs423 (0.00432261, 0.000725589);
  MassPointParameters mpp423 (cs423, 18683);
  StopNeutralinoMap.insert(std::make_pair(mp423, mpp423));

  MassPoint mp424 (1050, 50);
  StopCrossSection cs424 (0.00432261, 0.000725589);
  MassPointParameters mpp424 (cs424, 15579);
  StopNeutralinoMap.insert(std::make_pair(mp424, mpp424));

  MassPoint mp425 (1050, 100);
  StopCrossSection cs425 (0.00432261, 0.000725589);
  MassPointParameters mpp425 (cs425, 14860);
  StopNeutralinoMap.insert(std::make_pair(mp425, mpp425));

  MassPoint mp426 (1050, 150);
  StopCrossSection cs426 (0.00432261, 0.000725589);
  MassPointParameters mpp426 (cs426, 20257);
  StopNeutralinoMap.insert(std::make_pair(mp426, mpp426));

  MassPoint mp427 (1050, 200);
  StopCrossSection cs427 (0.00432261, 0.000725589);
  MassPointParameters mpp427 (cs427, 17442);
  StopNeutralinoMap.insert(std::make_pair(mp427, mpp427));

  MassPoint mp428 (1050, 250);
  StopCrossSection cs428 (0.00432261, 0.000725589);
  MassPointParameters mpp428 (cs428, 19038);
  StopNeutralinoMap.insert(std::make_pair(mp428, mpp428));

  MassPoint mp429 (1050, 300);
  StopCrossSection cs429 (0.00432261, 0.000725589);
  MassPointParameters mpp429 (cs429, 19647);
  StopNeutralinoMap.insert(std::make_pair(mp429, mpp429));

  MassPoint mp430 (1050, 350);
  StopCrossSection cs430 (0.00432261, 0.000725589);
  MassPointParameters mpp430 (cs430, 18029);
  StopNeutralinoMap.insert(std::make_pair(mp430, mpp430));

  MassPoint mp431 (1050, 400);
  StopCrossSection cs431 (0.00432261, 0.000725589);
  MassPointParameters mpp431 (cs431, 18420);
  StopNeutralinoMap.insert(std::make_pair(mp431, mpp431));

  MassPoint mp432 (1050, 450);
  StopCrossSection cs432 (0.00432261, 0.000725589);
  MassPointParameters mpp432 (cs432, 18297);
  StopNeutralinoMap.insert(std::make_pair(mp432, mpp432));

  MassPoint mp433 (1050, 500);
  StopCrossSection cs433 (0.00432261, 0.000725589);
  MassPointParameters mpp433 (cs433, 16860);
  StopNeutralinoMap.insert(std::make_pair(mp433, mpp433));

  MassPoint mp434 (1050, 550);
  StopCrossSection cs434 (0.00432261, 0.000725589);
  MassPointParameters mpp434 (cs434, 18895);
  StopNeutralinoMap.insert(std::make_pair(mp434, mpp434));

  MassPoint mp435 (1050, 600);
  StopCrossSection cs435 (0.00432261, 0.000725589);
  MassPointParameters mpp435 (cs435, 16704);
  StopNeutralinoMap.insert(std::make_pair(mp435, mpp435));

  MassPoint mp436 (1050, 650);
  StopCrossSection cs436 (0.00432261, 0.000725589);
  MassPointParameters mpp436 (cs436, 18113);
  StopNeutralinoMap.insert(std::make_pair(mp436, mpp436));

  MassPoint mp437 (1100, 1);
  StopCrossSection cs437 (0.00307413, 0.000532983);
  MassPointParameters mpp437 (cs437, 17516);
  StopNeutralinoMap.insert(std::make_pair(mp437, mpp437));

  MassPoint mp438 (1100, 50);
  StopCrossSection cs438 (0.00307413, 0.000532983);
  MassPointParameters mpp438 (cs438, 19165);
  StopNeutralinoMap.insert(std::make_pair(mp438, mpp438));

  MassPoint mp439 (1100, 100);
  StopCrossSection cs439 (0.00307413, 0.000532983);
  MassPointParameters mpp439 (cs439, 18053);
  StopNeutralinoMap.insert(std::make_pair(mp439, mpp439));

  MassPoint mp440 (1100, 150);
  StopCrossSection cs440 (0.00307413, 0.000532983);
  MassPointParameters mpp440 (cs440, 21117);
  StopNeutralinoMap.insert(std::make_pair(mp440, mpp440));

  MassPoint mp441 (1100, 200);
  StopCrossSection cs441 (0.00307413, 0.000532983);
  MassPointParameters mpp441 (cs441, 20849);
  StopNeutralinoMap.insert(std::make_pair(mp441, mpp441));

  MassPoint mp442 (1100, 250);
  StopCrossSection cs442 (0.00307413, 0.000532983);
  MassPointParameters mpp442 (cs442, 21487);
  StopNeutralinoMap.insert(std::make_pair(mp442, mpp442));

  MassPoint mp443 (1100, 300);
  StopCrossSection cs443 (0.00307413, 0.000532983);
  MassPointParameters mpp443 (cs443, 18002);
  StopNeutralinoMap.insert(std::make_pair(mp443, mpp443));

  MassPoint mp444 (1100, 350);
  StopCrossSection cs444 (0.00307413, 0.000532983);
  MassPointParameters mpp444 (cs444, 17150);
  StopNeutralinoMap.insert(std::make_pair(mp444, mpp444));

  MassPoint mp445 (1100, 400);
  StopCrossSection cs445 (0.00307413, 0.000532983);
  MassPointParameters mpp445 (cs445, 21811);
  StopNeutralinoMap.insert(std::make_pair(mp445, mpp445));

  MassPoint mp446 (1100, 450);
  StopCrossSection cs446 (0.00307413, 0.000532983);
  MassPointParameters mpp446 (cs446, 18963);
  StopNeutralinoMap.insert(std::make_pair(mp446, mpp446));

  MassPoint mp447 (1100, 500);
  StopCrossSection cs447 (0.00307413, 0.000532983);
  MassPointParameters mpp447 (cs447, 20169);
  StopNeutralinoMap.insert(std::make_pair(mp447, mpp447));

  MassPoint mp448 (1100, 550);
  StopCrossSection cs448 (0.00307413, 0.000532983);
  MassPointParameters mpp448 (cs448, 15726);
  StopNeutralinoMap.insert(std::make_pair(mp448, mpp448));

  MassPoint mp449 (1100, 600);
  StopCrossSection cs449 (0.00307413, 0.000532983);
  MassPointParameters mpp449 (cs449, 18043);
  StopNeutralinoMap.insert(std::make_pair(mp449, mpp449));

  MassPoint mp450 (1100, 650);
  StopCrossSection cs450 (0.00307413, 0.000532983);
  MassPointParameters mpp450 (cs450, 17519);
  StopNeutralinoMap.insert(std::make_pair(mp450, mpp450));

  MassPoint mp451 (1150, 1);
  StopCrossSection cs451 (0.00221047, 0.000396247);
  MassPointParameters mpp451 (cs451, 20192);
  StopNeutralinoMap.insert(std::make_pair(mp451, mpp451));

  MassPoint mp452 (1150, 50);
  StopCrossSection cs452 (0.00221047, 0.000396247);
  MassPointParameters mpp452 (cs452, 20634);
  StopNeutralinoMap.insert(std::make_pair(mp452, mpp452));

  MassPoint mp453 (1150, 100);
  StopCrossSection cs453 (0.00221047, 0.000396247);
  MassPointParameters mpp453 (cs453, 18944);
  StopNeutralinoMap.insert(std::make_pair(mp453, mpp453));

  MassPoint mp454 (1150, 150);
  StopCrossSection cs454 (0.00221047, 0.000396247);
  MassPointParameters mpp454 (cs454, 18300);
  StopNeutralinoMap.insert(std::make_pair(mp454, mpp454));

  MassPoint mp455 (1150, 200);
  StopCrossSection cs455 (0.00221047, 0.000396247);
  MassPointParameters mpp455 (cs455, 17542);
  StopNeutralinoMap.insert(std::make_pair(mp455, mpp455));

  MassPoint mp456 (1150, 250);
  StopCrossSection cs456 (0.00221047, 0.000396247);
  MassPointParameters mpp456 (cs456, 17251);
  StopNeutralinoMap.insert(std::make_pair(mp456, mpp456));

  MassPoint mp457 (1150, 300);
  StopCrossSection cs457 (0.00221047, 0.000396247);
  MassPointParameters mpp457 (cs457, 19473);
  StopNeutralinoMap.insert(std::make_pair(mp457, mpp457));

  MassPoint mp458 (1150, 350);
  StopCrossSection cs458 (0.00221047, 0.000396247);
  MassPointParameters mpp458 (cs458, 16007);
  StopNeutralinoMap.insert(std::make_pair(mp458, mpp458));

  MassPoint mp459 (1150, 400);
  StopCrossSection cs459 (0.00221047, 0.000396247);
  MassPointParameters mpp459 (cs459, 20796);
  StopNeutralinoMap.insert(std::make_pair(mp459, mpp459));

  MassPoint mp460 (1150, 450);
  StopCrossSection cs460 (0.00221047, 0.000396247);
  MassPointParameters mpp460 (cs460, 18274);
  StopNeutralinoMap.insert(std::make_pair(mp460, mpp460));

  MassPoint mp461 (1150, 500);
  StopCrossSection cs461 (0.00221047, 0.000396247);
  MassPointParameters mpp461 (cs461, 21031);
  StopNeutralinoMap.insert(std::make_pair(mp461, mpp461));

  MassPoint mp462 (1150, 550);
  StopCrossSection cs462 (0.00221047, 0.000396247);
  MassPointParameters mpp462 (cs462, 18437);
  StopNeutralinoMap.insert(std::make_pair(mp462, mpp462));

  MassPoint mp463 (1150, 600);
  StopCrossSection cs463 (0.00221047, 0.000396247);
  MassPointParameters mpp463 (cs463, 18597);
  StopNeutralinoMap.insert(std::make_pair(mp463, mpp463));

  MassPoint mp464 (1150, 650);
  StopCrossSection cs464 (0.00221047, 0.000396247);
  MassPointParameters mpp464 (cs464, 19505);
  StopNeutralinoMap.insert(std::make_pair(mp464, mpp464));

  MassPoint mp465 (1200, 1);
  StopCrossSection cs465 (0.00159844, 0.000296045);
  MassPointParameters mpp465 (cs465, 16938);
  StopNeutralinoMap.insert(std::make_pair(mp465, mpp465));

  MassPoint mp466 (1200, 50);
  StopCrossSection cs466 (0.00159844, 0.000296045);
  MassPointParameters mpp466 (cs466, 17111);
  StopNeutralinoMap.insert(std::make_pair(mp466, mpp466));

  MassPoint mp467 (1200, 100);
  StopCrossSection cs467 (0.00159844, 0.000296045);
  MassPointParameters mpp467 (cs467, 21532);
  StopNeutralinoMap.insert(std::make_pair(mp467, mpp467));

  MassPoint mp468 (1200, 150);
  StopCrossSection cs468 (0.00159844, 0.000296045);
  MassPointParameters mpp468 (cs468, 20130);
  StopNeutralinoMap.insert(std::make_pair(mp468, mpp468));

  MassPoint mp469 (1200, 200);
  StopCrossSection cs469 (0.00159844, 0.000296045);
  MassPointParameters mpp469 (cs469, 21315);
  StopNeutralinoMap.insert(std::make_pair(mp469, mpp469));

  MassPoint mp470 (1200, 250);
  StopCrossSection cs470 (0.00159844, 0.000296045);
  MassPointParameters mpp470 (cs470, 18727);
  StopNeutralinoMap.insert(std::make_pair(mp470, mpp470));

  MassPoint mp471 (1200, 300);
  StopCrossSection cs471 (0.00159844, 0.000296045);
  MassPointParameters mpp471 (cs471, 18112);
  StopNeutralinoMap.insert(std::make_pair(mp471, mpp471));

  MassPoint mp472 (1200, 350);
  StopCrossSection cs472 (0.00159844, 0.000296045);
  MassPointParameters mpp472 (cs472, 18028);
  StopNeutralinoMap.insert(std::make_pair(mp472, mpp472));

  MassPoint mp473 (1200, 400);
  StopCrossSection cs473 (0.00159844, 0.000296045);
  MassPointParameters mpp473 (cs473, 20219);
  StopNeutralinoMap.insert(std::make_pair(mp473, mpp473));

  MassPoint mp474 (1200, 450);
  StopCrossSection cs474 (0.00159844, 0.000296045);
  MassPointParameters mpp474 (cs474, 23181);
  StopNeutralinoMap.insert(std::make_pair(mp474, mpp474));

  MassPoint mp475 (1200, 500);
  StopCrossSection cs475 (0.00159844, 0.000296045);
  MassPointParameters mpp475 (cs475, 18878);
  StopNeutralinoMap.insert(std::make_pair(mp475, mpp475));

  MassPoint mp476 (1200, 550);
  StopCrossSection cs476 (0.00159844, 0.000296045);
  MassPointParameters mpp476 (cs476, 21553);
  StopNeutralinoMap.insert(std::make_pair(mp476, mpp476));

  MassPoint mp477 (1200, 600);
  StopCrossSection cs477 (0.00159844, 0.000296045);
  MassPointParameters mpp477 (cs477, 19744);
  StopNeutralinoMap.insert(std::make_pair(mp477, mpp477));

  MassPoint mp478 (1200, 650);
  StopCrossSection cs478 (0.00159844, 0.000296045);
  MassPointParameters mpp478 (cs478, 21272);
  StopNeutralinoMap.insert(std::make_pair(mp478, mpp478));
  
  MassPoint mpisr0 (258, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr0, 1.09939));

  MassPoint mpisr1 (267, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr1, 1.09652));

  MassPoint mpisr2 (275, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr2, 1.10285));

  MassPoint mpisr3 (275, 25);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr3, 1.10219));

  MassPoint mpisr4 (275, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr4, 1.10108));

  MassPoint mpisr5 (275, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr5, 1.09992));

  MassPoint mpisr6 (275, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr6, 1.09982));

  MassPoint mpisr7 (275, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr7, 1.09519));

  MassPoint mpisr8 (275, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr8, 1.09265));

  MassPoint mpisr9 (275, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr9, 1.09216));

  MassPoint mpisr10 (275, 188);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr10, 1.09456));

  MassPoint mpisr11 (283, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr11, 1.10093));

  MassPoint mpisr12 (292, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr12, 1.10276));

  MassPoint mpisr13 (300, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr13, 1.10687));

  MassPoint mpisr14 (300, 25);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr14, 1.10644));

  MassPoint mpisr15 (300, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr15, 1.10644));

  MassPoint mpisr16 (300, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr16, 1.10535));

  MassPoint mpisr17 (300, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr17, 1.10603));

  MassPoint mpisr18 (300, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr18, 1.1038));

  MassPoint mpisr19 (300, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr19, 1.09642));

  MassPoint mpisr20 (300, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr20, 1.09563));

  MassPoint mpisr21 (300, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr21, 1.09772));

  MassPoint mpisr22 (300, 213);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr22, 1.0967));

  MassPoint mpisr23 (308, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr23, 1.1043));

  MassPoint mpisr24 (317, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr24, 1.1044));

  MassPoint mpisr25 (325, 25);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr25, 1.1088));

  MassPoint mpisr26 (325, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr26, 1.11033));

  MassPoint mpisr27 (325, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr27, 1.11037));

  MassPoint mpisr28 (325, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr28, 1.10851));

  MassPoint mpisr29 (325, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr29, 1.10818));

  MassPoint mpisr30 (325, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr30, 1.10746));

  MassPoint mpisr31 (325, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr31, 1.10279));

  MassPoint mpisr32 (325, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr32, 1.10115));

  MassPoint mpisr33 (325, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr33, 1.10065));

  MassPoint mpisr34 (325, 238);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr34, 1.10038));

  MassPoint mpisr35 (333, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr35, 1.10925));

  MassPoint mpisr36 (342, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr36, 1.10865));

  MassPoint mpisr37 (350, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr37, 1.1139));

  MassPoint mpisr38 (350, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr38, 1.11552));

  MassPoint mpisr39 (350, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr39, 1.11271));

  MassPoint mpisr40 (350, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr40, 1.11288));

  MassPoint mpisr41 (350, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr41, 1.11276));

  MassPoint mpisr42 (350, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr42, 1.11163));

  MassPoint mpisr43 (350, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr43, 1.11045));

  MassPoint mpisr44 (350, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr44, 1.10544));

  MassPoint mpisr45 (350, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr45, 1.10282));

  MassPoint mpisr46 (350, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr46, 1.10173));

  MassPoint mpisr47 (350, 263);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr47, 1.10243));

  MassPoint mpisr48 (358, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr48, 1.1118));

  MassPoint mpisr49 (367, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr49, 1.11048));

  MassPoint mpisr50 (375, 75);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr50, 1.11803));

  MassPoint mpisr51 (375, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr51, 1.11762));

  MassPoint mpisr52 (375, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr52, 1.11712));

  MassPoint mpisr53 (375, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr53, 1.11556));

  MassPoint mpisr54 (375, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr54, 1.11525));

  MassPoint mpisr55 (375, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr55, 1.11384));

  MassPoint mpisr56 (375, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr56, 1.10938));

  MassPoint mpisr57 (375, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr57, 1.10722));

  MassPoint mpisr58 (375, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr58, 1.10616));

  MassPoint mpisr59 (375, 288);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr59, 1.10725));

  MassPoint mpisr60 (383, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr60, 1.11511));

  MassPoint mpisr61 (392, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr61, 1.11102));

  MassPoint mpisr62 (400, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr62, 1.12242));

  MassPoint mpisr63 (400, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr63, 1.12138));

  MassPoint mpisr64 (400, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr64, 1.12198));

  MassPoint mpisr65 (400, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr65, 1.12093));

  MassPoint mpisr66 (400, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr66, 1.11733));

  MassPoint mpisr67 (400, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr67, 1.11907));

  MassPoint mpisr68 (400, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr68, 1.11708));

  MassPoint mpisr69 (400, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr69, 1.11843));

  MassPoint mpisr70 (400, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr70, 1.11194));

  MassPoint mpisr71 (400, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr71, 1.11043));

  MassPoint mpisr72 (400, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr72, 1.10846));

  MassPoint mpisr73 (400, 313);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr73, 1.10905));

  MassPoint mpisr74 (408, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr74, 1.11916));

  MassPoint mpisr75 (417, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr75, 1.1141));

  MassPoint mpisr76 (425, 125);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr76, 1.12245));

  MassPoint mpisr77 (425, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr77, 1.12223));

  MassPoint mpisr78 (425, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr78, 1.12179));

  MassPoint mpisr79 (425, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr79, 1.1212));

  MassPoint mpisr80 (425, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr80, 1.12004));

  MassPoint mpisr81 (425, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr81, 1.11788));

  MassPoint mpisr82 (425, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr82, 1.11047));

  MassPoint mpisr83 (425, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr83, 1.11136));

  MassPoint mpisr84 (425, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr84, 1.11147));

  MassPoint mpisr85 (425, 338);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr85, 1.10975));

  MassPoint mpisr86 (433, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr86, 1.11947));

  MassPoint mpisr87 (442, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr87, 1.1165));

  MassPoint mpisr88 (450, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr88, 1.12939));

  MassPoint mpisr89 (450, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr89, 1.1298));

  MassPoint mpisr90 (450, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr90, 1.1246));

  MassPoint mpisr91 (450, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr91, 1.12658));

  MassPoint mpisr92 (450, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr92, 1.12292));

  MassPoint mpisr93 (450, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr93, 1.12345));

  MassPoint mpisr94 (450, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr94, 1.12255));

  MassPoint mpisr95 (450, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr95, 1.12208));

  MassPoint mpisr96 (450, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr96, 1.11882));

  MassPoint mpisr97 (450, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr97, 1.11625));

  MassPoint mpisr98 (450, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr98, 1.11436));

  MassPoint mpisr99 (450, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr99, 1.11378));

  MassPoint mpisr100 (450, 363);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr100, 1.113));

  MassPoint mpisr101 (458, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr101, 1.12095));

  MassPoint mpisr102 (467, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr102, 1.11719));

  MassPoint mpisr103 (475, 175);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr103, 1.12955));

  MassPoint mpisr104 (475, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr104, 1.12545));

  MassPoint mpisr105 (475, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr105, 1.13005));

  MassPoint mpisr106 (475, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr106, 1.12246));

  MassPoint mpisr107 (475, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr107, 1.12453));

  MassPoint mpisr108 (475, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr108, 1.12444));

  MassPoint mpisr109 (475, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr109, 1.11947));

  MassPoint mpisr110 (475, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr110, 1.11748));

  MassPoint mpisr111 (475, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr111, 1.11656));

  MassPoint mpisr112 (475, 388);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr112, 1.11581));

  MassPoint mpisr113 (483, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr113, 1.12339));

  MassPoint mpisr114 (492, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr114, 1.12258));

  MassPoint mpisr115 (500, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr115, 1.13682));

  MassPoint mpisr116 (500, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr116, 1.13149));

  MassPoint mpisr117 (500, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr117, 1.13353));

  MassPoint mpisr118 (500, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr118, 1.13513));

  MassPoint mpisr119 (500, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr119, 1.13079));

  MassPoint mpisr120 (500, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr120, 1.12813));

  MassPoint mpisr121 (500, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr121, 1.12317));

  MassPoint mpisr122 (500, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr122, 1.12434));

  MassPoint mpisr123 (500, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr123, 1.12236));

  MassPoint mpisr124 (500, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr124, 1.12149));

  MassPoint mpisr125 (500, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr125, 1.11871));

  MassPoint mpisr126 (500, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr126, 1.12142));

  MassPoint mpisr127 (500, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr127, 1.11723));

  MassPoint mpisr128 (500, 413);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr128, 1.11538));

  MassPoint mpisr129 (508, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr129, 1.12803));

  MassPoint mpisr130 (517, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr130, 1.11866));

  MassPoint mpisr131 (525, 225);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr131, 1.13105));

  MassPoint mpisr132 (525, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr132, 1.12959));

  MassPoint mpisr133 (525, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr133, 1.1259));

  MassPoint mpisr134 (525, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr134, 1.1258));

  MassPoint mpisr135 (525, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr135, 1.12817));

  MassPoint mpisr136 (525, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr136, 1.12323));

  MassPoint mpisr137 (525, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr137, 1.12303));

  MassPoint mpisr138 (525, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr138, 1.12073));

  MassPoint mpisr139 (525, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr139, 1.11615));

  MassPoint mpisr140 (525, 438);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr140, 1.12139));

  MassPoint mpisr141 (533, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr141, 1.12904));

  MassPoint mpisr142 (542, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr142, 1.12538));

  MassPoint mpisr143 (550, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr143, 1.14376));

  MassPoint mpisr144 (550, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr144, 1.14437));

  MassPoint mpisr145 (550, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr145, 1.14353));

  MassPoint mpisr146 (550, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr146, 1.13843));

  MassPoint mpisr147 (550, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr147, 1.13628));

  MassPoint mpisr148 (550, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr148, 1.14014));

  MassPoint mpisr149 (550, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr149, 1.13282));

  MassPoint mpisr150 (550, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr150, 1.13635));

  MassPoint mpisr151 (550, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr151, 1.12987));

  MassPoint mpisr152 (550, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr152, 1.13007));

  MassPoint mpisr153 (550, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr153, 1.12971));

  MassPoint mpisr154 (550, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr154, 1.12552));

  MassPoint mpisr155 (550, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr155, 1.12439));

  MassPoint mpisr156 (550, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr156, 1.12283));

  MassPoint mpisr157 (550, 463);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr157, 1.12003));

  MassPoint mpisr158 (558, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr158, 1.12618));

  MassPoint mpisr159 (567, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr159, 1.1325));

  MassPoint mpisr160 (575, 275);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr160, 1.13582));

  MassPoint mpisr161 (575, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr161, 1.1326));

  MassPoint mpisr162 (575, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr162, 1.13672));

  MassPoint mpisr163 (575, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr163, 1.13294));

  MassPoint mpisr164 (575, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr164, 1.13459));

  MassPoint mpisr165 (575, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr165, 1.13107));

  MassPoint mpisr166 (575, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr166, 1.12545));

  MassPoint mpisr167 (575, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr167, 1.12595));

  MassPoint mpisr168 (575, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr168, 1.12067));

  MassPoint mpisr169 (575, 488);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr169, 1.12097));

  MassPoint mpisr170 (583, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr170, 1.13288));

  MassPoint mpisr171 (592, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr171, 1.1246));

  MassPoint mpisr172 (600, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr172, 1.14754));

  MassPoint mpisr173 (600, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr173, 1.15362));

  MassPoint mpisr174 (600, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr174, 1.15038));

  MassPoint mpisr175 (600, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr175, 1.14482));

  MassPoint mpisr176 (600, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr176, 1.14897));

  MassPoint mpisr177 (600, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr177, 1.13337));

  MassPoint mpisr178 (600, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr178, 1.14057));

  MassPoint mpisr179 (600, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr179, 1.13166));

  MassPoint mpisr180 (600, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr180, 1.1415));

  MassPoint mpisr181 (600, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr181, 1.1313));

  MassPoint mpisr182 (600, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr182, 1.13188));

  MassPoint mpisr183 (600, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr183, 1.13149));

  MassPoint mpisr184 (600, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr184, 1.12344));

  MassPoint mpisr185 (600, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr185, 1.12318));

  MassPoint mpisr186 (600, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr186, 1.114));

  MassPoint mpisr187 (600, 513);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr187, 1.12465));

  MassPoint mpisr188 (608, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr188, 1.13318));

  MassPoint mpisr189 (617, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr189, 1.13585));

  MassPoint mpisr190 (625, 325);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr190, 1.13639));

  MassPoint mpisr191 (625, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr191, 1.12981));

  MassPoint mpisr192 (625, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr192, 1.13574));

  MassPoint mpisr193 (625, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr193, 1.13988));

  MassPoint mpisr194 (625, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr194, 1.12688));

  MassPoint mpisr195 (625, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr195, 1.13626));

  MassPoint mpisr196 (625, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr196, 1.12071));

  MassPoint mpisr197 (625, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr197, 1.12731));

  MassPoint mpisr198 (625, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr198, 1.12857));

  MassPoint mpisr199 (625, 538);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr199, 1.12557));

  MassPoint mpisr200 (633, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr200, 1.13177));

  MassPoint mpisr201 (642, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr201, 1.13209));

  MassPoint mpisr202 (650, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr202, 1.1482));

  MassPoint mpisr203 (650, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr203, 1.15763));

  MassPoint mpisr204 (650, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr204, 1.15855));

  MassPoint mpisr205 (650, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr205, 1.14548));

  MassPoint mpisr206 (650, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr206, 1.15617));

  MassPoint mpisr207 (650, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr207, 1.14977));

  MassPoint mpisr208 (650, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr208, 1.13456));

  MassPoint mpisr209 (650, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr209, 1.1338));

  MassPoint mpisr210 (650, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr210, 1.12472));

  MassPoint mpisr211 (650, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr211, 1.13228));

  MassPoint mpisr212 (650, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr212, 1.14667));

  MassPoint mpisr213 (650, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr213, 1.13262));

  MassPoint mpisr214 (650, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr214, 1.14043));

  MassPoint mpisr215 (650, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr215, 1.12905));

  MassPoint mpisr216 (650, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr216, 1.13324));

  MassPoint mpisr217 (650, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr217, 1.11932));

  MassPoint mpisr218 (650, 563);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr218, 1.12924));

  MassPoint mpisr219 (658, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr219, 1.12514));

  MassPoint mpisr220 (667, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr220, 1.12435));

  MassPoint mpisr221 (675, 375);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr221, 1.14882));

  MassPoint mpisr222 (675, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr222, 1.14118));

  MassPoint mpisr223 (675, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr223, 1.13847));

  MassPoint mpisr224 (675, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr224, 1.13794));

  MassPoint mpisr225 (675, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr225, 1.14758));

  MassPoint mpisr226 (675, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr226, 1.14628));

  MassPoint mpisr227 (675, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr227, 1.13982));

  MassPoint mpisr228 (675, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr228, 1.13435));

  MassPoint mpisr229 (675, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr229, 1.12391));

  MassPoint mpisr230 (675, 588);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr230, 1.12392));

  MassPoint mpisr231 (683, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr231, 1.13949));

  MassPoint mpisr232 (692, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr232, 1.13009));

  MassPoint mpisr233 (700, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr233, 1.15994));

  MassPoint mpisr234 (700, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr234, 1.16807));

  MassPoint mpisr235 (700, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr235, 1.15958));

  MassPoint mpisr236 (700, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr236, 1.16041));

  MassPoint mpisr237 (700, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr237, 1.15311));

  MassPoint mpisr238 (700, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr238, 1.155));

  MassPoint mpisr239 (700, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr239, 1.1535));

  MassPoint mpisr240 (700, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr240, 1.15872));

  MassPoint mpisr241 (700, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr241, 1.14083));

  MassPoint mpisr242 (700, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr242, 1.14703));

  MassPoint mpisr243 (700, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr243, 1.14662));

  MassPoint mpisr244 (700, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr244, 1.14751));

  MassPoint mpisr245 (700, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr245, 1.14056));

  MassPoint mpisr246 (700, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr246, 1.13689));

  MassPoint mpisr247 (700, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr247, 1.14499));

  MassPoint mpisr248 (700, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr248, 1.12886));

  MassPoint mpisr249 (700, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr249, 1.12144));

  MassPoint mpisr250 (700, 613);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr250, 1.12191));

  MassPoint mpisr251 (708, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr251, 1.13039));

  MassPoint mpisr252 (717, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr252, 1.14802));

  MassPoint mpisr253 (725, 425);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr253, 1.14836));

  MassPoint mpisr254 (725, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr254, 1.14807));

  MassPoint mpisr255 (725, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr255, 1.13741));

  MassPoint mpisr256 (725, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr256, 1.13104));

  MassPoint mpisr257 (725, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr257, 1.14591));

  MassPoint mpisr258 (725, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr258, 1.13439));

  MassPoint mpisr259 (725, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr259, 1.11445));

  MassPoint mpisr260 (725, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr260, 1.12962));

  MassPoint mpisr261 (725, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr261, 1.13948));

  MassPoint mpisr262 (725, 638);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr262, 1.12652));

  MassPoint mpisr263 (733, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr263, 1.14281));

  MassPoint mpisr264 (742, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr264, 1.12871));

  MassPoint mpisr265 (750, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr265, 1.16636));

  MassPoint mpisr266 (750, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr266, 1.15857));

  MassPoint mpisr267 (750, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr267, 1.16633));

  MassPoint mpisr268 (750, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr268, 1.16172));

  MassPoint mpisr269 (750, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr269, 1.1725));

  MassPoint mpisr270 (750, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr270, 1.1429));

  MassPoint mpisr271 (750, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr271, 1.14705));

  MassPoint mpisr272 (750, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr272, 1.15204));

  MassPoint mpisr273 (750, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr273, 1.12263));

  MassPoint mpisr274 (750, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr274, 1.15549));

  MassPoint mpisr275 (750, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr275, 1.1495));

  MassPoint mpisr276 (750, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr276, 1.15792));

  MassPoint mpisr277 (750, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr277, 1.14426));

  MassPoint mpisr278 (750, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr278, 1.135));

  MassPoint mpisr279 (750, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr279, 1.11677));

  MassPoint mpisr280 (750, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr280, 1.14233));

  MassPoint mpisr281 (750, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr281, 1.1338));

  MassPoint mpisr282 (750, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr282, 1.12112));

  MassPoint mpisr283 (758, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr283, 1.13855));

  MassPoint mpisr284 (767, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr284, 1.14584));

  MassPoint mpisr285 (775, 475);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr285, 1.15004));

  MassPoint mpisr286 (775, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr286, 1.13012));

  MassPoint mpisr287 (775, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr287, 1.13234));

  MassPoint mpisr288 (775, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr288, 1.14058));

  MassPoint mpisr289 (775, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr289, 1.1484));

  MassPoint mpisr290 (775, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr290, 1.14797));

  MassPoint mpisr291 (775, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr291, 1.12148));

  MassPoint mpisr292 (775, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr292, 1.13118));

  MassPoint mpisr293 (783, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr293, 1.13886));

  MassPoint mpisr294 (792, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr294, 1.12784));

  MassPoint mpisr295 (800, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr295, 1.16574));

  MassPoint mpisr296 (800, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr296, 1.16605));

  MassPoint mpisr297 (800, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr297, 1.16356));

  MassPoint mpisr298 (800, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr298, 1.15644));

  MassPoint mpisr299 (800, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr299, 1.16301));

  MassPoint mpisr300 (800, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr300, 1.16884));

  MassPoint mpisr301 (800, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr301, 1.16093));

  MassPoint mpisr302 (800, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr302, 1.16127));

  MassPoint mpisr303 (800, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr303, 1.15514));

  MassPoint mpisr304 (800, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr304, 1.15149));

  MassPoint mpisr305 (800, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr305, 1.14955));

  MassPoint mpisr306 (800, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr306, 1.1405));

  MassPoint mpisr307 (800, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr307, 1.14245));

  MassPoint mpisr308 (800, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr308, 1.14363));

  MassPoint mpisr309 (800, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr309, 1.13627));

  MassPoint mpisr310 (800, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr310, 1.13621));

  MassPoint mpisr311 (800, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr311, 1.1292));

  MassPoint mpisr312 (808, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr312, 1.14083));

  MassPoint mpisr313 (817, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr313, 1.13463));

  MassPoint mpisr314 (825, 525);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr314, 1.15137));

  MassPoint mpisr315 (825, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr315, 1.14363));

  MassPoint mpisr316 (825, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr316, 1.13486));

  MassPoint mpisr317 (825, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr317, 1.13348));

  MassPoint mpisr318 (825, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr318, 1.14001));

  MassPoint mpisr319 (825, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr319, 1.1343));

  MassPoint mpisr320 (833, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr320, 1.1546));

  MassPoint mpisr321 (850, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr321, 1.18319));

  MassPoint mpisr322 (850, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr322, 1.19036));

  MassPoint mpisr323 (850, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr323, 1.16609));

  MassPoint mpisr324 (850, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr324, 1.17082));

  MassPoint mpisr325 (850, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr325, 1.1754));

  MassPoint mpisr326 (850, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr326, 1.15958));

  MassPoint mpisr327 (850, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr327, 1.15632));

  MassPoint mpisr328 (850, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr328, 1.15144));

  MassPoint mpisr329 (850, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr329, 1.15964));

  MassPoint mpisr330 (850, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr330, 1.15231));

  MassPoint mpisr331 (850, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr331, 1.1398));

  MassPoint mpisr332 (850, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr332, 1.12633));

  MassPoint mpisr333 (850, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr333, 1.13893));

  MassPoint mpisr334 (850, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr334, 1.13216));

  MassPoint mpisr335 (850, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr335, 1.14319));

  MassPoint mpisr336 (850, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr336, 1.13978));

  MassPoint mpisr337 (875, 575);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr337, 1.1396));

  MassPoint mpisr338 (875, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr338, 1.13458));

  MassPoint mpisr339 (875, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr339, 1.1422));

  MassPoint mpisr340 (875, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr340, 1.14109));

  MassPoint mpisr341 (900, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr341, 1.17442));

  MassPoint mpisr342 (900, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr342, 1.17724));

  MassPoint mpisr343 (900, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr343, 1.17204));

  MassPoint mpisr344 (900, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr344, 1.17348));

  MassPoint mpisr345 (900, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr345, 1.1739));

  MassPoint mpisr346 (900, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr346, 1.15122));

  MassPoint mpisr347 (900, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr347, 1.16623));

  MassPoint mpisr348 (900, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr348, 1.16449));

  MassPoint mpisr349 (900, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr349, 1.1783));

  MassPoint mpisr350 (900, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr350, 1.1727));

  MassPoint mpisr351 (900, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr351, 1.15646));

  MassPoint mpisr352 (900, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr352, 1.16534));

  MassPoint mpisr353 (900, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr353, 1.14733));

  MassPoint mpisr354 (900, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr354, 1.15658));

  MassPoint mpisr355 (900, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr355, 1.15216));

  MassPoint mpisr356 (925, 625);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr356, 1.14785));

  MassPoint mpisr357 (925, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr357, 1.13313));

  MassPoint mpisr358 (950, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr358, 1.16598));

  MassPoint mpisr359 (950, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr359, 1.17097));

  MassPoint mpisr360 (950, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr360, 1.17542));

  MassPoint mpisr361 (950, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr361, 1.17886));

  MassPoint mpisr362 (950, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr362, 1.16387));

  MassPoint mpisr363 (950, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr363, 1.17795));

  MassPoint mpisr364 (950, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr364, 1.16374));

  MassPoint mpisr365 (950, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr365, 1.17061));

  MassPoint mpisr366 (950, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr366, 1.17313));

  MassPoint mpisr367 (950, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr367, 1.15202));

  MassPoint mpisr368 (950, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr368, 1.16556));

  MassPoint mpisr369 (950, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr369, 1.15359));

  MassPoint mpisr370 (950, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr370, 1.15367));

  MassPoint mpisr371 (950, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr371, 1.15128));

  MassPoint mpisr372 (1000, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr372, 1.16677));

  MassPoint mpisr373 (1000, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr373, 1.17969));

  MassPoint mpisr374 (1000, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr374, 1.19146));

  MassPoint mpisr375 (1000, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr375, 1.18331));

  MassPoint mpisr376 (1000, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr376, 1.19863));

  MassPoint mpisr377 (1000, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr377, 1.15597));

  MassPoint mpisr378 (1000, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr378, 1.15939));

  MassPoint mpisr379 (1000, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr379, 1.18102));

  MassPoint mpisr380 (1000, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr380, 1.17657));

  MassPoint mpisr381 (1000, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr381, 1.17589));

  MassPoint mpisr382 (1000, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr382, 1.15259));

  MassPoint mpisr383 (1000, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr383, 1.17282));

  MassPoint mpisr384 (1000, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr384, 1.15516));

  MassPoint mpisr385 (1000, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr385, 1.15701));

  MassPoint mpisr386 (1050, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr386, 1.19495));

  MassPoint mpisr387 (1050, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr387, 1.19304));

  MassPoint mpisr388 (1050, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr388, 1.18343));

  MassPoint mpisr389 (1050, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr389, 1.18851));

  MassPoint mpisr390 (1050, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr390, 1.19881));

  MassPoint mpisr391 (1050, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr391, 1.18418));

  MassPoint mpisr392 (1050, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr392, 1.19096));

  MassPoint mpisr393 (1050, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr393, 1.20406));

  MassPoint mpisr394 (1050, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr394, 1.17712));

  MassPoint mpisr395 (1050, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr395, 1.1965));

  MassPoint mpisr396 (1050, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr396, 1.16857));

  MassPoint mpisr397 (1050, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr397, 1.16531));

  MassPoint mpisr398 (1050, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr398, 1.16034));

  MassPoint mpisr399 (1050, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr399, 1.15379));

  MassPoint mpisr400 (1100, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr400, 1.1881));

  MassPoint mpisr401 (1100, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr401, 1.2007));

  MassPoint mpisr402 (1100, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr402, 1.17227));

  MassPoint mpisr403 (1100, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr403, 1.18213));

  MassPoint mpisr404 (1100, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr404, 1.18482));

  MassPoint mpisr405 (1100, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr405, 1.20425));

  MassPoint mpisr406 (1100, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr406, 1.20723));

  MassPoint mpisr407 (1100, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr407, 1.18643));

  MassPoint mpisr408 (1100, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr408, 1.20526));

  MassPoint mpisr409 (1100, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr409, 1.19195));

  MassPoint mpisr410 (1100, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr410, 1.16528));

  MassPoint mpisr411 (1100, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr411, 1.17079));

  MassPoint mpisr412 (1100, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr412, 1.16766));

  MassPoint mpisr413 (1100, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr413, 1.15882));

  MassPoint mpisr414 (1150, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr414, 1.20455));

  MassPoint mpisr415 (1150, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr415, 1.17532));

  MassPoint mpisr416 (1150, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr416, 1.20228));

  MassPoint mpisr417 (1150, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr417, 1.19624));

  MassPoint mpisr418 (1150, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr418, 1.1929));

  MassPoint mpisr419 (1150, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr419, 1.20319));

  MassPoint mpisr420 (1150, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr420, 1.1762));

  MassPoint mpisr421 (1150, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr421, 1.18633));

  MassPoint mpisr422 (1150, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr422, 1.18039));

  MassPoint mpisr423 (1150, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr423, 1.19445));

  MassPoint mpisr424 (1150, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr424, 1.17712));

  MassPoint mpisr425 (1150, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr425, 1.17515));

  MassPoint mpisr426 (1150, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr426, 1.17276));

  MassPoint mpisr427 (1150, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr427, 1.15744));

  MassPoint mpisr428 (1200, 1);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr428, 1.19212));

  MassPoint mpisr429 (1200, 50);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr429, 1.1862));

  MassPoint mpisr430 (1200, 100);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr430, 1.20723));

  MassPoint mpisr431 (1200, 150);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr431, 1.18632));

  MassPoint mpisr432 (1200, 200);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr432, 1.19046));

  MassPoint mpisr433 (1200, 250);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr433, 1.20552));

  MassPoint mpisr434 (1200, 300);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr434, 1.19503));

  MassPoint mpisr435 (1200, 350);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr435, 1.20144));

  MassPoint mpisr436 (1200, 400);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr436, 1.16597));

  MassPoint mpisr437 (1200, 450);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr437, 1.17837));

  MassPoint mpisr438 (1200, 500);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr438, 1.17703));

  MassPoint mpisr439 (1200, 550);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr439, 1.19067));

  MassPoint mpisr440 (1200, 600);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr440, 1.18409));

  MassPoint mpisr441 (1200, 650);
  StopNeutralinoISRMap.insert(std::make_pair(mpisr441, 1.16849));


}

void AnalysisStop::CorrectEventWeight() {

  float EventBTagSF       = 1.;
  float EventBTagSF_Upb   = 1.;
  float EventBTagSF_Dob   = 1.;
  float EventBTagSF_UpFSb = 1.;
  float EventBTagSF_DoFSb = 1.;
  
  for (int ijet = 0; ijet<_njet; ijet++) {
    
    if (AnalysisJets[ijet].v.Pt() <= 20.) continue; 
    
    int ThisIndex = AnalysisJets[ijet].index;
    int ThisFlavour = std_vector_jet_HadronFlavour->at(ThisIndex);
    float MonteCarloEfficiency = BTagSF->JetTagEfficiency(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
    float DataEfficiency       = MonteCarloEfficiency*BTagSF->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
    float DataEfficiency_Upb   = MonteCarloEfficiency*BTagSF_Upb->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
    float DataEfficiency_Dob   = MonteCarloEfficiency*BTagSF_Dob->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
    float DataEfficiency_UpFSb = MonteCarloEfficiency*BTagSF_UpFSb->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
    float DataEfficiency_DoFSb = MonteCarloEfficiency*BTagSF_DoFSb->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());

    if (AnalysisJets[ijet].csvv2ivf>CSVv2M) {
      EventBTagSF       *= DataEfficiency/MonteCarloEfficiency;
      EventBTagSF_Upb   *= DataEfficiency_Upb/MonteCarloEfficiency;
      EventBTagSF_Dob   *= DataEfficiency_Dob/MonteCarloEfficiency;
      EventBTagSF_UpFSb *= DataEfficiency_UpFSb/MonteCarloEfficiency;
      EventBTagSF_DoFSb *= DataEfficiency_DoFSb/MonteCarloEfficiency;
    } else {
      EventBTagSF       *= (1. - DataEfficiency)/(1. - MonteCarloEfficiency);
      EventBTagSF_Upb   *= (1. - DataEfficiency_Upb)/(1. - MonteCarloEfficiency);
      EventBTagSF_Dob   *= (1. - DataEfficiency_Dob)/(1. - MonteCarloEfficiency);
      EventBTagSF_UpFSb *= (1. - DataEfficiency_UpFSb)/(1. - MonteCarloEfficiency);
      EventBTagSF_DoFSb *= (1. - DataEfficiency_DoFSb)/(1. - MonteCarloEfficiency);
    }

  }
  
  _event_weight           *= EventBTagSF/bPogSF_CSVM;
  _event_weight_Btagup     = _event_weight*EventBTagSF_Upb/EventBTagSF; 
  _event_weight_Btagdo     = _event_weight*EventBTagSF_Dob/EventBTagSF; 
  _event_weight_BtagFSup   = _event_weight*EventBTagSF_UpFSb/EventBTagSF; 
  _event_weight_BtagFSdo   = _event_weight*EventBTagSF_DoFSb/EventBTagSF; 
  _event_weight_Idisoup   *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Idisodo   *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Triggerup *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Triggerdo *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Recoup    *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Recodo    *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Fastsimup *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Fastsimdo *= EventBTagSF/bPogSF_CSVM; 
  _event_weight_Toppt     *= EventBTagSF/bPogSF_CSVM; 
  
  if (FastSimDataset!="") { 
    
    int StopMass = susyMstop, NeutralinoMass = susyMLSP;
    MassPoint ThisMassPoint (StopMass, NeutralinoMass);
    MassPointParameters TheseMassPointParameters = StopNeutralinoMap.at(ThisMassPoint);
    StopCrossSection ThisStopCrossSection = TheseMassPointParameters.first;
    int SampleSize = TheseMassPointParameters.second;
    _event_weight           *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW;
    _event_weight_Btagup    *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Btagdo    *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW;
    _event_weight_BtagFSup  *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_BtagFSdo  *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Idisoup   *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Idisodo   *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Triggerup *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Triggerdo *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Recoup    *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Recodo    *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Fastsimup *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW; 
    _event_weight_Fastsimdo *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW;
    _event_weight_Toppt     *= (1000.*ThisStopCrossSection.first/SampleSize)/baseW;
    
  }
  
}

bool AnalysisStop::PassFastsimJetsCleanup() {

  // Cleaning up of fastsim jets 
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSRecommendationsMoriond17#Cleaning_up_of_fastsim_jets_from

  if (!_isfastsim) return true;
  
  for (int ijet = 0; ijet<_njet; ijet++) 
    if (AnalysisJets[ijet].v.Pt()>20 && fabs(AnalysisJets[ijet].v.Eta())<2.5) 
      if (std_vector_jet_chargedHadronFraction->at(AnalysisJets[ijet].index)<0.1) {

	bool isgenmatched = false;
	
	for (int gjet = 0; gjet<std_vector_jetGen_pt->size(); gjet++) {
	  
	  if (std_vector_jetGen_pt->at(gjet)<0.) continue;

	  TLorentzVector ThisGenJet; 
	  ThisGenJet.SetPtEtaPhiM(std_vector_jetGen_pt->at(gjet), 
				  std_vector_jetGen_eta->at(gjet), 
				  std_vector_jetGen_phi->at(gjet), 0.0);

	  if (fabs(AnalysisJets[ijet].v.DeltaR(ThisGenJet))<0.3) isgenmatched = true;

	}
	
	if (!isgenmatched) return false;
      
      }

  return true;
  
}

void AnalysisStop::ApplyISRReweighting() {

  int StopMass = susyMstop, NeutralinoMass = susyMLSP;
  MassPoint ThisMassPoint (StopMass, NeutralinoMass);
  float ISRGlobalNormalization = StopNeutralinoISRMap.at(ThisMassPoint);

  int _isrbin = (_nisrjet<nISRMultiplicityBins) ? _nisrjet : (nISRMultiplicityBins-1);

  float _isr_weight = ISRGlobalNormalization*ISRBinWeight[_isrbin];
  float _isr_weight_up = (1. + _isr_weight)/2.;
  float _isr_weight_do = _isr_weight - (_isr_weight_up - _isr_weight);

  _event_weight           *= _isr_weight;
  _event_weight_Btagup    *= _isr_weight; 
  _event_weight_Btagdo    *= _isr_weight;
  _event_weight_BtagFSup  *= _isr_weight; 
  _event_weight_BtagFSdo  *= _isr_weight; 
  _event_weight_Idisoup   *= _isr_weight; 
  _event_weight_Idisodo   *= _isr_weight; 
  _event_weight_Triggerup *= _isr_weight; 
  _event_weight_Triggerdo *= _isr_weight; 
  _event_weight_Recoup    *= _isr_weight; 
  _event_weight_Recodo    *= _isr_weight; 
  _event_weight_Fastsimup *= _isr_weight; 
  _event_weight_Fastsimdo *= _isr_weight;
  _event_weight_Toppt     *= _isr_weight;
  _event_weight_Isrnjetup  = _event_weight*_isr_weight_up/_isr_weight;
  _event_weight_Isrnjetdo  = _event_weight*_isr_weight_do/_isr_weight;

}

void AnalysisStop::GetMiniTree(TFile *MiniTreeFile, TString systematic) {

  fChain = (TTree*) MiniTreeFile->Get("latino");

  fChain->SetBranchAddress("dyll",            &_dyll);
  fChain->SetBranchAddress("ptbll",           &_ptbll);
  fChain->SetBranchAddress("m2l",             &_m2l);
  fChain->SetBranchAddress("mllbb",           &_mllbb);
  fChain->SetBranchAddress("meff",            &_meff);
  fChain->SetBranchAddress("nvtx",            &nvtx);
  
  fChain->SetBranchAddress("dphill",          &dphill);
  
  fChain->SetBranchAddress("metPfType1",      &metPfType1);
  fChain->SetBranchAddress("metPfType1Phi",   &metPfType1Phi);
  fChain->SetBranchAddress("dphillmet",       &_dphillmet);
  fChain->SetBranchAddress("dphilmet1",       &dphilmet1);
  fChain->SetBranchAddress("dphimetjet",      &_dphimetjet);
  fChain->SetBranchAddress("dphijet1met",     &_dphijet1met);
  fChain->SetBranchAddress("dphimetptbll",    &_dphimetptbll); 

  fChain->SetBranchAddress("jet1pt",          &jetpt1);
  fChain->SetBranchAddress("jet1phi",         &jetphi1);
  fChain->SetBranchAddress("jet1eta",         &jeteta1);
  fChain->SetBranchAddress("jet2pt",          &jetpt2);
  fChain->SetBranchAddress("jet2phi",         &jetphi2);
  fChain->SetBranchAddress("jet2eta",         &jeteta2);
  
  fChain->SetBranchAddress("lep1eta",         &_lep1eta); 
  fChain->SetBranchAddress("lep1phi",         &_lep1phi);  
  fChain->SetBranchAddress("lep1pt",          &_lep1pt); 
  fChain->SetBranchAddress("lep1isfake",      &_lep1isfake);
  fChain->SetBranchAddress("lep2eta",         &_lep2eta);
  fChain->SetBranchAddress("lep2phi",         &_lep2phi);
  fChain->SetBranchAddress("lep2pt",          &_lep2pt);
  fChain->SetBranchAddress("lep2isfake",      &_lep2isfake);

  fChain->SetBranchAddress("ht",              &_ht);
  fChain->SetBranchAddress("htjets",          &_htjets);
  fChain->SetBranchAddress("htnojets",        &_htnojets);
  fChain->SetBranchAddress("htvisible",       &_htvisible);
  fChain->SetBranchAddress("nisrjet",         &_nisrjet);
  fChain->SetBranchAddress("njet",            &_njet);
  
  fChain->SetBranchAddress("susyMLSP",        &susyMLSP);
  fChain->SetBranchAddress("susyMstop",       &susyMstop);

  if (systematic=="nominal") 
    fChain->SetBranchAddress("eventW",           &_event_weight);
  else if (systematic=="Btagup")
    fChain->SetBranchAddress("eventW_Btagup",    &_event_weight);
  else if (systematic=="Btagdo")
    fChain->SetBranchAddress("eventW_Btagdo",    &_event_weight);
  else if (systematic=="BtagFSup")
    fChain->SetBranchAddress("eventW_BtagFSup",  &_event_weight);
  else if (systematic=="BtagFSdo")
    fChain->SetBranchAddress("eventW_BtagFSdo",  &_event_weight);
  else if (systematic=="Idisoup")
    fChain->SetBranchAddress("eventW_Idisoup",   &_event_weight);
  else if (systematic=="Idisodo") 
    fChain->SetBranchAddress("eventW_Idisodo",   &_event_weight);
  else if (systematic=="Triggerup")
    fChain->SetBranchAddress("eventW_Triggerup", &_event_weight);
  else if (systematic=="Triggerdo")
    fChain->SetBranchAddress("eventW_Triggerdo", &_event_weight);
  else if (systematic=="Recoup")
    fChain->SetBranchAddress("eventW_Recoup",    &_event_weight);
  else if (systematic=="Recodo")
    fChain->SetBranchAddress("eventW_Recodo",    &_event_weight);
  else if (systematic=="Fastsimup")
    fChain->SetBranchAddress("eventW_Fastsimup", &_event_weight);
  else if (systematic=="Fastsimdo")
    fChain->SetBranchAddress("eventW_Fastsimdo", &_event_weight);
  else if (systematic=="Toppt")
    fChain->SetBranchAddress("eventW_Toppt",     &_event_weight);
  else 
    fChain->SetBranchAddress("eventW",           &_event_weight);

  fChain->SetBranchAddress("eventW_Btagup",    &_event_weight_Btagup);
  fChain->SetBranchAddress("eventW_Btagdo",    &_event_weight_Btagdo);
  fChain->SetBranchAddress("eventW_BtagFSup",  &_event_weight_BtagFSup);
  fChain->SetBranchAddress("eventW_BtagFSdo",  &_event_weight_BtagFSdo);
  fChain->SetBranchAddress("eventW_Idisoup",   &_event_weight_Idisoup);
  fChain->SetBranchAddress("eventW_Idisodo",   &_event_weight_Idisodo);
  fChain->SetBranchAddress("eventW_Triggerup", &_event_weight_Triggerup);
  fChain->SetBranchAddress("eventW_Triggerdo", &_event_weight_Triggerdo);
  fChain->SetBranchAddress("eventW_Recoup",    &_event_weight_Recoup);
  fChain->SetBranchAddress("eventW_Recodo",    &_event_weight_Recodo);
  fChain->SetBranchAddress("eventW_Fastsimup", &_event_weight_Fastsimup);
  fChain->SetBranchAddress("eventW_Fastsimdo", &_event_weight_Fastsimdo);
  fChain->SetBranchAddress("eventW_Toppt",     &_event_weight_Toppt);

  _event_weight_Isrnjetup = _event_weight;
  _event_weight_Isrnjetdo = _event_weight;

  fChain->SetBranchAddress("channel",         &_channel);
  fChain->SetBranchAddress("njet",            &_njet); 

  fChain->SetBranchAddress("nbjet30csvv2l",   &_nbjet30csvv2l);
  fChain->SetBranchAddress("nbjet30csvv2m",   &_nbjet30csvv2m);
  fChain->SetBranchAddress("nbjet30csvv2t",   &_nbjet30csvv2t);
  //fChain->SetBranchAddress("LeadingPtCSVv2M", &_leadingPtCSVv2M);
  //fChain->SetBranchAddress("LeadingPtCSVv2L", &_leadingPtCSVv2L);
  //fChain->SetBranchAddress("LeadingPtCSVv2T", &_leadingPtCSVv2T);
  fChain->SetBranchAddress("leadingPtCSVv2M", &_leadingPtCSVv2M);
  fChain->SetBranchAddress("leadingPtCSVv2L", &_leadingPtCSVv2L);
  fChain->SetBranchAddress("leadingPtCSVv2T", &_leadingPtCSVv2T);

  fChain->SetBranchAddress("mt2ll",           &_mt2ll);
  fChain->SetBranchAddress("mt2llgen",        &_mt2llgen);
  fChain->SetBranchAddress("mt2bb",           &_mt2bb);
  fChain->SetBranchAddress("mt2lblb",         &_mt2lblb);
  fChain->SetBranchAddress("mt2bbtrue",       &_mt2bbtrue);
  fChain->SetBranchAddress("mt2lblbcomb",     &_mt2lblbcomb);
  fChain->SetBranchAddress("mt2lblbtrue",     &_mt2lblbtrue);

  fChain->SetBranchAddress("mlb1",            &_mlb1);
  fChain->SetBranchAddress("mlb2",            &_mlb2);
  fChain->SetBranchAddress("mlb1comb",        &_mlb1comb);
  fChain->SetBranchAddress("mlb2comb",        &_mlb2comb);
  fChain->SetBranchAddress("mlb1true",        &_mlb1true);
  fChain->SetBranchAddress("mlb2true",        &_mlb2true);

  fChain->SetBranchAddress("bjet1pt",         &_bjet1pt);
  fChain->SetBranchAddress("bjet1eta",        &_bjet1eta);
  fChain->SetBranchAddress("bjet1phi",        &_bjet1phi);
  fChain->SetBranchAddress("bjet1mass",       &_bjet1mass);
  fChain->SetBranchAddress("bjet1csvv2ivf",   &_bjet1csvv2ivf);
  fChain->SetBranchAddress("bjet2pt",         &_bjet2pt);
  fChain->SetBranchAddress("bjet2eta",        &_bjet2eta);
  fChain->SetBranchAddress("bjet2phi",        &_bjet2phi);
  fChain->SetBranchAddress("bjet2mass",       &_bjet2mass);
  fChain->SetBranchAddress("bjet2csvv2ivf",   &_bjet2csvv2ivf);
  fChain->SetBranchAddress("tjet1pt",         &_tjet1pt);
  fChain->SetBranchAddress("tjet1eta",        &_tjet1eta);
  fChain->SetBranchAddress("tjet1phi",        &_tjet1phi);
  fChain->SetBranchAddress("tjet1mass",       &_tjet1mass);
  fChain->SetBranchAddress("tjet1csvv2ivf",   &_tjet1csvv2ivf);
  fChain->SetBranchAddress("tjet1assignment", &_tjet1assignment);
  fChain->SetBranchAddress("tjet2pt",         &_tjet2pt);
  fChain->SetBranchAddress("tjet2eta",        &_tjet2eta);
  fChain->SetBranchAddress("tjet2phi",        &_tjet2phi);
  fChain->SetBranchAddress("tjet2mass",       &_tjet2mass);
  fChain->SetBranchAddress("tjet2csvv2ivf",   &_tjet2csvv2ivf);
  fChain->SetBranchAddress("tjet2assignment", &_tjet2assignment);

  fChain->SetBranchAddress("MR",              &_MR);
  fChain->SetBranchAddress("R2",              &_R2);
  fChain->SetBranchAddress("Rpt",             &_Rpt);
  fChain->SetBranchAddress("invGamma",        &_invGamma);
  fChain->SetBranchAddress("Mdr",             &_Mdr);
  fChain->SetBranchAddress("DeltaPhiRll",     &_DeltaPhiRll);
  
  fChain->SetBranchAddress("dphijet1met", &_dphijet1met);
  fChain->SetBranchAddress("dphijet2met", &_dphijet2met);//, &b_dphijet2met);
  fChain->SetBranchAddress("dphijj", &_dphijj);//, &b_dphijj);
  fChain->SetBranchAddress("dphijjmet", &_dphijjmet);//, &b_dphijjmet);
  fChain->SetBranchAddress("dphilep1jet1", &_dphilep1jet1);//, &b_dphilep1jet1);
  fChain->SetBranchAddress("dphilep1jet2", &_dphilep1jet2);//, &b_dphilep1jet2);
  fChain->SetBranchAddress("dphilep2jet1", &_dphilep2jet1);//, &b_dphilep2jet1);
  fChain->SetBranchAddress("dphilep2jet2", &_dphilep2jet2);//, &b_dphilep2jet2);
  fChain->SetBranchAddress("dphill", &dphill);//, &b_dphill);
  fChain->SetBranchAddress("dphillstar", &_dphillstar);//, &b_dphillstar);
  fChain->SetBranchAddress("dphilmet1", &dphilmet1);//, &b_dphilmet1);
  fChain->SetBranchAddress("dphilmet2", &dphilmet2);//, &b_dphilmet2);
  fChain->SetBranchAddress("drll", &drll);//, &b_drll);
  fChain->SetBranchAddress("dphimetptbll", &_dphimetptbll);//, &b_dphimetptbll);
  fChain->SetBranchAddress("dphimetbbll", &_dphimetbbll);//, &b_dphimetbbll);
  fChain->SetBranchAddress("dphimetjet", &_dphimetjet);//, &b_dphimetjet);

}
    
