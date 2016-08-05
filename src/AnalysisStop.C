#define AnalysisStop_cxx
#include "../include/AnalysisStop.h"
#include "../../BTagSFUtil/BTagSFUtil.C"


//------------------------------------------------------------------------------
// AnalysisStop
//------------------------------------------------------------------------------
AnalysisStop::AnalysisStop(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisStop::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;
 
  Setup(analysis, filename, luminosity);
 

  // Define histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (int j=0; j<ncut; j++) {
  
    for (int k=0; k<=njetbin; k++) {

      TString sbin = (k < njetbin) ? Form("/%djet", k) : "";

      TString directory = scut[j] + sbin;

      root_output->cd();

      if (k < njetbin) gDirectory->mkdir(directory);

      root_output->cd(directory);

      for (int i=ee; i<=ll; i++) {

	TString suffix = "_" + schannel[i];

	DefineHistograms(i, j, k, suffix);

	h_mt2lblbcomb       [i][j][k] = new TH1D("h_mt2lblbcomb"      + suffix, "", 3000,    0, 3000);
	h_mt2bbtrue         [i][j][k] = new TH1D("h_mt2bbtrue"        + suffix, "", 3000,    0, 3000);
	h_mt2lblbtrue       [i][j][k] = new TH1D("h_mt2lblbtrue"      + suffix, "", 3000,    0, 3000);
	h_mt2lblbmatch      [i][j][k] = new TH1D("h_mt2lblbmatch"     + suffix, "", 3000,    0, 3000);
	h_mlb1comb          [i][j][k] = new TH1D("h_mlb1comb"         + suffix, "", 3000,    0, 3000);
	h_mlb2comb          [i][j][k] = new TH1D("h_mlb2comb"         + suffix, "", 3000,    0, 3000);
	h_mlb1true          [i][j][k] = new TH1D("h_mlb1true"         + suffix, "", 3000,    0, 3000);
	h_mlb2true          [i][j][k] = new TH1D("h_mlb2true"         + suffix, "", 3000,    0, 3000);
	h_mt2lblbvsmlbtrue  [i][j][k] = new TH2D("h_mt2lblbvsmlbtrue" + suffix, "",  100,    0, 1000,  100,    0, 1000);
	
      }
    }
  }

  root_output->cd();

  TString FastSimDataset = (filename.Contains("T2tt") || filename.Contains("T2tb") || filename.Contains("T2bW")) ? "_T2" : "";
  BTagSFUtil *BTagSF = new BTagSFUtil("mujets", "CSVv2", "Medium", 0, FastSimDataset);


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup(2.4);

    if (_ismc) {

      float EventBTagSF = 1.;

      for (int ijet = 0; ijet<_njet; ijet++) {
      
	int ThisIndex = AnalysisJets[ijet].index;
	int ThisFlavour = std_vector_jet_HadronFlavour->at(ThisIndex);
	
	float MonteCarloEfficiency = BTagSF->JetTagEfficiency(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
	float DataEfficiency = MonteCarloEfficiency*BTagSF->GetJetSF(ThisFlavour, AnalysisJets[ijet].v.Pt(), AnalysisJets[ijet].v.Eta());
	
	if (AnalysisJets[ijet].csvv2ivf>CSVv2M) 
	  EventBTagSF *= DataEfficiency/MonteCarloEfficiency;
	else 
	  EventBTagSF *= (1. - DataEfficiency)/(1. - MonteCarloEfficiency);
	
      }
      
      _event_weight *= EventBTagSF;
      
      if (FastSimDataset!="") { 
	
	GetStopCrossSection(susyMstop);
	_event_weight *= StopCrossSection;
	
      }

    }

    // Analysis
    //--------------------------------------------------------------------------
    //if (!_ismc && run > 274240) continue;  // Luminosity for any blinded analysis  
  
    if (Lepton1.flavour * Lepton2.flavour > 0) continue;

    if (Lepton1.v.Pt() < 20.) continue;
    if (Lepton2.v.Pt() < 20.) continue;
    
    if (_nlepton > 2) continue;

    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    
    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    _m2l  = mll;
    _pt2l = ptll;

    

    // Fill histograms
    //--------------------------------------------------------------------------
    bool pass = true;

    FillLevelHistograms(Stop_00_Has2Leptons, pass);    

    pass &= _mt2ll < 100; // GeV    

    FillLevelHistograms(Stop_00_2LMt2upper100, pass);

   
    // Basics Stop
    //-------------------------------------------------------------------------
    pass &= mll>20.;
   
    FillLevelHistograms(Stop_00_mll20, pass);

    pass &= fabs(mll - Z_MASS) > 15.;

    FillLevelHistograms(Stop_00_Zveto, pass);

    if (pass && _saveminitree) minitree->Fill();

    pass &= (MET.Et() > 40.);
    
    FillLevelHistograms(Stop_00_Met40, pass); 
/*   
    //-------------------------------------------------------------------------
    // Basics + _ht > 260 + Has2Leptons    

//    bool pass1 = _ht > 260.;
//    bool pass1_jet = _htjets > 100.;   
    bool pass1_jet = _htjets > 100.; 
//    bool pass1_1 = _ht > 300.;   
//    bool pass1_1_jet = _htjets > 125.;
    bool pass1_1_jet = _htjets > 125.; 
//    FillLevelHistograms(Stop_00_ht260, pass && pass1);
//    FillLevelHistograms(Stop_00_ht300, pass && pass1_1);
    FillLevelHistograms( Stop_00_htjets100, pass && pass1_jet);
    FillLevelHistograms( Stop_00_htjets125, pass && pass1_1_jet);

    //    bool pass1_2_jet = _htjets > 150.;
    bool pass1_2_jet = _htjets > 150.;
    FillLevelHistograms( Stop_00_htjets150, pass && pass1_2_jet);




    //--------------------------------------------------------------------------
    // Basics + _ht > 260 + met > 50 + Has2Leptons
     
//    pass1 &= MET.Et() > 50.;    
//    pass1_1 &= MET.Et() > 50.;    	
      pass1_jet &= MET.Et() > 50.;
      pass1_1_jet &= MET.Et() > 50.;
  
//    FillLevelHistograms(Stop_00_ht260Met50, pass && pass1);
//    FillLevelHistograms(Stop_00_ht300Met50, pass && pass1_1);
      FillLevelHistograms( Stop_00_htjets100Met50, pass && pass1_jet);
      FillLevelHistograms( Stop_00_htjets125Met50, pass && pass1_1_jet);
	 
      pass1_2_jet &= MET.Et() > 50.;
      FillLevelHistograms( Stop_00_htjets150Met50, pass && pass1_2_jet);
*/
    //-------------------------------------------------------------------------
    // Basics + Has1BJet
   
    bool pass2 = pass && (_njet > 1);

    //pass &= (njet > 1);
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! pass &= (_nbjet20loose > 0);

    FillLevelHistograms(Stop_01_Has2Jets, pass2);

    pass2 &= (_nbjet30csvv2m > 0);

    FillLevelHistograms(Stop_02_Has1BJet, pass2);

/*
    FillLevelHistograms(Stop_02_Has2BJet, pass2 && (_nbjet30csvv2m>=2));

    pass2 &= _htjets > 150.;    

    FillLevelHistograms(Stop_02_Has1BJetHtJets150, pass2);
    
    //--------------------------------------------------------------------------
    // Basics + met > 50 + Has2Leptons

    bool pass_met50 = MET.Et() > 50.;

    bool pass3 = pass && pass_met50;

    FillLevelHistograms(Stop_00_Met50, pass3);
 */   //--------------------------------------------------------------------------
  }

  EndJob();
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
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisStop::FillLevelHistograms(int  icut,
				       bool pass)
{
  if (!pass) return;

  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);

  FillAnalysisHistograms(_channel, icut, _jetbin);
  FillAnalysisHistograms(_channel, icut, njetbin);
}

void AnalysisStop::GetStopCrossSection(float StopMass)
{

  int iStopMass = StopMass;

  if (StopMass<100. || StopMass>2000. || iStopMass%5!=0) {
    StopCrossSection = -1.; return;
  }

  if (iStopMass==100) { StopCrossSection = 	1521.11; StopCrossSectionUncertainty = StopCrossSection* 15.4038/100.; }
  if (iStopMass==105) { StopCrossSection = 	1233.18; StopCrossSectionUncertainty = StopCrossSection* 15.4059/100.; }
  if (iStopMass==110) { StopCrossSection = 	1013.76; StopCrossSectionUncertainty = StopCrossSection* 15.4088/100.; }
  if (iStopMass==115) { StopCrossSection = 	832.656; StopCrossSectionUncertainty = StopCrossSection* 15.1503/100.; }
  if (iStopMass==120) { StopCrossSection = 	689.799; StopCrossSectionUncertainty = StopCrossSection* 15.044/100.; }
  if (iStopMass==125) { StopCrossSection = 	574.981; StopCrossSectionUncertainty = StopCrossSection* 14.9895/100.; }
  if (iStopMass==130) { StopCrossSection = 	481.397; StopCrossSectionUncertainty = StopCrossSection* 14.8906/100.; }
  if (iStopMass==135) { StopCrossSection = 	405.159; StopCrossSectionUncertainty = StopCrossSection* 14.8952/100.; }
  if (iStopMass==140) { StopCrossSection = 	342.865; StopCrossSectionUncertainty = StopCrossSection* 14.9119/100.; }
  if (iStopMass==145) { StopCrossSection = 	291.752; StopCrossSectionUncertainty = StopCrossSection* 14.8022/100.; }
  if (iStopMass==150) { StopCrossSection = 	249.409; StopCrossSectionUncertainty = StopCrossSection* 14.7477/100.; }
  if (iStopMass==155) { StopCrossSection = 	214.221; StopCrossSectionUncertainty = StopCrossSection* 14.5928/100.; }
  if (iStopMass==160) { StopCrossSection = 	184.623; StopCrossSectionUncertainty = StopCrossSection* 14.5821/100.; }
  if (iStopMass==165) { StopCrossSection = 	159.614; StopCrossSectionUncertainty = StopCrossSection* 14.7859/100.; }
  if (iStopMass==170) { StopCrossSection = 	139.252; StopCrossSectionUncertainty = StopCrossSection* 14.547/100.; }
  if (iStopMass==175) { StopCrossSection = 	121.416; StopCrossSectionUncertainty = StopCrossSection* 14.6341/100.; }
  if (iStopMass==180) { StopCrossSection = 	106.194; StopCrossSectionUncertainty = StopCrossSection* 14.2033/100.; }
  if (iStopMass==185) { StopCrossSection = 	93.3347; StopCrossSectionUncertainty = StopCrossSection* 14.4893/100.; }
  if (iStopMass==190) { StopCrossSection = 	82.2541; StopCrossSectionUncertainty = StopCrossSection* 14.4677/100.; }
  if (iStopMass==195) { StopCrossSection = 	72.7397; StopCrossSectionUncertainty = StopCrossSection* 14.4452/100.; }
  if (iStopMass==200) { StopCrossSection = 	64.5085; StopCrossSectionUncertainty = StopCrossSection* 14.4098/100.; }
  if (iStopMass==205) { StopCrossSection = 	57.2279; StopCrossSectionUncertainty = StopCrossSection* 14.4191/100.; }
  if (iStopMass==210) { StopCrossSection = 	50.9226; StopCrossSectionUncertainty = StopCrossSection* 14.2457/100.; }
  if (iStopMass==215) { StopCrossSection = 	45.3761; StopCrossSectionUncertainty = StopCrossSection* 14.344/100.; }
  if (iStopMass==220) { StopCrossSection = 	40.5941; StopCrossSectionUncertainty = StopCrossSection* 14.2634/100.; }
  if (iStopMass==225) { StopCrossSection = 	36.3818; StopCrossSectionUncertainty = StopCrossSection* 14.2189/100.; }
  if (iStopMass==230) { StopCrossSection = 	32.6679; StopCrossSectionUncertainty = StopCrossSection* 14.1592/100.; }
  if (iStopMass==235) { StopCrossSection = 	29.3155; StopCrossSectionUncertainty = StopCrossSection* 14.2233/100.; }
  if (iStopMass==240) { StopCrossSection = 	26.4761; StopCrossSectionUncertainty = StopCrossSection* 14.1723/100.; }
  if (iStopMass==245) { StopCrossSection = 	23.8853; StopCrossSectionUncertainty = StopCrossSection* 13.9482/100.; }
  if (iStopMass==250) { StopCrossSection = 	21.5949; StopCrossSectionUncertainty = StopCrossSection* 14.0595/100.; }
  if (iStopMass==255) { StopCrossSection = 	19.5614; StopCrossSectionUncertainty = StopCrossSection* 13.8755/100.; }
  if (iStopMass==260) { StopCrossSection = 	17.6836; StopCrossSectionUncertainty = StopCrossSection* 13.9505/100.; }
  if (iStopMass==265) { StopCrossSection = 	16.112; StopCrossSectionUncertainty = StopCrossSection* 13.9531/100.; }
  if (iStopMass==270) { StopCrossSection = 	14.6459; StopCrossSectionUncertainty = StopCrossSection* 13.9278/100.; }
  if (iStopMass==275) { StopCrossSection = 	13.3231; StopCrossSectionUncertainty = StopCrossSection* 14.2549/100.; }
  if (iStopMass==280) { StopCrossSection = 	12.1575; StopCrossSectionUncertainty = StopCrossSection* 14.1584/100.; }
  if (iStopMass==285) { StopCrossSection = 	11.0925; StopCrossSectionUncertainty = StopCrossSection* 14.0904/100.; }
  if (iStopMass==290) { StopCrossSection = 	10.1363; StopCrossSectionUncertainty = StopCrossSection* 13.8967/100.; }
  if (iStopMass==295) { StopCrossSection = 	9.29002; StopCrossSectionUncertainty = StopCrossSection* 13.9107/100.; }
  if (iStopMass==300) { StopCrossSection = 	8.51615; StopCrossSectionUncertainty = StopCrossSection* 13.9223/100.; }
  if (iStopMass==305) { StopCrossSection = 	7.81428; StopCrossSectionUncertainty = StopCrossSection* 13.8996/100.; }
  if (iStopMass==310) { StopCrossSection = 	7.17876; StopCrossSectionUncertainty = StopCrossSection* 13.9357/100.; }
  if (iStopMass==315) { StopCrossSection = 	6.60266; StopCrossSectionUncertainty = StopCrossSection* 13.9256/100.; }
  if (iStopMass==320) { StopCrossSection = 	6.08444; StopCrossSectionUncertainty = StopCrossSection* 13.7957/100.; }
  if (iStopMass==325) { StopCrossSection = 	5.60471; StopCrossSectionUncertainty = StopCrossSection* 13.8144/100.; }
  if (iStopMass==330) { StopCrossSection = 	5.17188; StopCrossSectionUncertainty = StopCrossSection* 13.6954/100.; }
  if (iStopMass==335) { StopCrossSection = 	4.77871; StopCrossSectionUncertainty = StopCrossSection* 13.7554/100.; }
  if (iStopMass==340) { StopCrossSection = 	4.41629; StopCrossSectionUncertainty = StopCrossSection* 13.7945/100.; }
  if (iStopMass==345) { StopCrossSection = 	4.08881; StopCrossSectionUncertainty = StopCrossSection* 13.7075/100.; }
  if (iStopMass==350) { StopCrossSection = 	3.78661; StopCrossSectionUncertainty = StopCrossSection* 13.6877/100.; }
  if (iStopMass==355) { StopCrossSection = 	3.50911; StopCrossSectionUncertainty = StopCrossSection* 13.8089/100.; }
  if (iStopMass==360) { StopCrossSection = 	3.25619; StopCrossSectionUncertainty = StopCrossSection* 13.8002/100.; }
  if (iStopMass==365) { StopCrossSection = 	3.02472; StopCrossSectionUncertainty = StopCrossSection* 13.7093/100.; }
  if (iStopMass==370) { StopCrossSection = 	2.8077; StopCrossSectionUncertainty = StopCrossSection* 13.8064/100.; }
  if (iStopMass==375) { StopCrossSection = 	2.61162; StopCrossSectionUncertainty = StopCrossSection* 13.8477/100.; }
  if (iStopMass==380) { StopCrossSection = 	2.43031; StopCrossSectionUncertainty = StopCrossSection* 13.6999/100.; }
  if (iStopMass==385) { StopCrossSection = 	2.26365; StopCrossSectionUncertainty = StopCrossSection* 13.728/100.; }
  if (iStopMass==390) { StopCrossSection = 	2.10786; StopCrossSectionUncertainty = StopCrossSection* 13.732/100.; }
  if (iStopMass==395) { StopCrossSection = 	1.9665; StopCrossSectionUncertainty = StopCrossSection* 13.4737/100.; }
  if (iStopMass==400) { StopCrossSection = 	1.83537; StopCrossSectionUncertainty = StopCrossSection* 13.6985/100.; }
  if (iStopMass==405) { StopCrossSection = 	1.70927; StopCrossSectionUncertainty = StopCrossSection* 13.7114/100.; }
  if (iStopMass==410) { StopCrossSection = 	1.60378; StopCrossSectionUncertainty = StopCrossSection* 13.5468/100.; }
  if (iStopMass==415) { StopCrossSection = 	1.49798; StopCrossSectionUncertainty = StopCrossSection* 13.4453/100.; }
  if (iStopMass==420) { StopCrossSection = 	1.39688; StopCrossSectionUncertainty = StopCrossSection* 13.6719/100.; }
  if (iStopMass==425) { StopCrossSection = 	1.31169; StopCrossSectionUncertainty = StopCrossSection* 13.5013/100.; }
  if (iStopMass==430) { StopCrossSection = 	1.22589; StopCrossSectionUncertainty = StopCrossSection* 13.3237/100.; }
  if (iStopMass==435) { StopCrossSection = 	1.14553; StopCrossSectionUncertainty = StopCrossSection* 13.5478/100.; }
  if (iStopMass==440) { StopCrossSection = 	1.07484; StopCrossSectionUncertainty = StopCrossSection* 13.7238/100.; }
  if (iStopMass==445) { StopCrossSection = 	1.01019; StopCrossSectionUncertainty = StopCrossSection* 13.4187/100.; }
  if (iStopMass==450) { StopCrossSection = 	0.948333; StopCrossSectionUncertainty = StopCrossSection* 13.4559/100.; }
  if (iStopMass==455) { StopCrossSection = 	0.890847; StopCrossSectionUncertainty = StopCrossSection* 13.4587/100.; }
  if (iStopMass==460) { StopCrossSection = 	0.836762; StopCrossSectionUncertainty = StopCrossSection* 13.4468/100.; }
  if (iStopMass==465) { StopCrossSection = 	0.787221; StopCrossSectionUncertainty = StopCrossSection* 13.4149/100.; }
  if (iStopMass==470) { StopCrossSection = 	0.740549; StopCrossSectionUncertainty = StopCrossSection* 13.4127/100.; }
  if (iStopMass==475) { StopCrossSection = 	0.697075; StopCrossSectionUncertainty = StopCrossSection* 13.3926/100.; }
  if (iStopMass==480) { StopCrossSection = 	0.655954; StopCrossSectionUncertainty = StopCrossSection* 13.4392/100.; }
  if (iStopMass==485) { StopCrossSection = 	0.618562; StopCrossSectionUncertainty = StopCrossSection* 13.3705/100.; }
  if (iStopMass==490) { StopCrossSection = 	0.582467; StopCrossSectionUncertainty = StopCrossSection* 13.3914/100.; }
  if (iStopMass==495) { StopCrossSection = 	0.549524; StopCrossSectionUncertainty = StopCrossSection* 13.3691/100.; }
  if (iStopMass==500) { StopCrossSection = 	0.51848; StopCrossSectionUncertainty = StopCrossSection* 13.3797/100.; }
  if (iStopMass==505) { StopCrossSection = 	0.489324; StopCrossSectionUncertainty = StopCrossSection* 13.3608/100.; }
  if (iStopMass==510) { StopCrossSection = 	0.462439; StopCrossSectionUncertainty = StopCrossSection* 13.3046/100.; }
  if (iStopMass==515) { StopCrossSection = 	0.436832; StopCrossSectionUncertainty = StopCrossSection* 13.3703/100.; }
  if (iStopMass==520) { StopCrossSection = 	0.412828; StopCrossSectionUncertainty = StopCrossSection* 13.272/100.; }
  if (iStopMass==525) { StopCrossSection = 	0.390303; StopCrossSectionUncertainty = StopCrossSection* 13.3443/100.; }
  if (iStopMass==530) { StopCrossSection = 	0.368755; StopCrossSectionUncertainty = StopCrossSection* 13.3769/100.; }
  if (iStopMass==535) { StopCrossSection = 	0.348705; StopCrossSectionUncertainty = StopCrossSection* 13.2706/100.; }
  if (iStopMass==540) { StopCrossSection = 	0.330157; StopCrossSectionUncertainty = StopCrossSection* 13.2981/100.; }
  if (iStopMass==545) { StopCrossSection = 	0.312672; StopCrossSectionUncertainty = StopCrossSection* 13.277/100.; }
  if (iStopMass==550) { StopCrossSection = 	0.296128; StopCrossSectionUncertainty = StopCrossSection* 13.2687/100.; }
  if (iStopMass==555) { StopCrossSection = 	0.280734; StopCrossSectionUncertainty = StopCrossSection* 13.2363/100.; }
  if (iStopMass==560) { StopCrossSection = 	0.266138; StopCrossSectionUncertainty = StopCrossSection* 13.193/100.; }
  if (iStopMass==565) { StopCrossSection = 	0.251557; StopCrossSectionUncertainty = StopCrossSection* 13.1731/100.; }
  if (iStopMass==570) { StopCrossSection = 	0.238537; StopCrossSectionUncertainty = StopCrossSection* 13.3409/100.; }
  if (iStopMass==575) { StopCrossSection = 	0.226118; StopCrossSectionUncertainty = StopCrossSection* 13.2741/100.; }
  if (iStopMass==580) { StopCrossSection = 	0.214557; StopCrossSectionUncertainty = StopCrossSection* 13.1697/100.; }
  if (iStopMass==585) { StopCrossSection = 	0.203566; StopCrossSectionUncertainty = StopCrossSection* 13.3257/100.; }
  if (iStopMass==590) { StopCrossSection = 	0.193079; StopCrossSectionUncertainty = StopCrossSection* 13.2037/100.; }
  if (iStopMass==595) { StopCrossSection = 	0.183604; StopCrossSectionUncertainty = StopCrossSection* 13.0973/100.; }
  if (iStopMass==600) { StopCrossSection = 	0.174599; StopCrossSectionUncertainty = StopCrossSection* 13.2074/100.; }
  if (iStopMass==605) { StopCrossSection = 	0.166131; StopCrossSectionUncertainty = StopCrossSection* 13.0154/100.; }
  if (iStopMass==610) { StopCrossSection = 	0.158242; StopCrossSectionUncertainty = StopCrossSection* 13.142/100.; }
  if (iStopMass==615) { StopCrossSection = 	0.150275; StopCrossSectionUncertainty = StopCrossSection* 13.285/100.; }
  if (iStopMass==620) { StopCrossSection = 	0.142787; StopCrossSectionUncertainty = StopCrossSection* 13.0642/100.; }
  if (iStopMass==625) { StopCrossSection = 	0.136372; StopCrossSectionUncertainty = StopCrossSection* 12.7962/100.; }
  if (iStopMass==630) { StopCrossSection = 	0.129886; StopCrossSectionUncertainty = StopCrossSection* 13.2957/100.; }
  if (iStopMass==635) { StopCrossSection = 	0.123402; StopCrossSectionUncertainty = StopCrossSection* 13.016/100.; }
  if (iStopMass==640) { StopCrossSection = 	0.11795; StopCrossSectionUncertainty = StopCrossSection* 12.7132/100.; }
  if (iStopMass==645) { StopCrossSection = 	0.112008; StopCrossSectionUncertainty = StopCrossSection* 12.808/100.; }
  if (iStopMass==650) { StopCrossSection = 	0.107045; StopCrossSectionUncertainty = StopCrossSection* 12.9232/100.; }
  if (iStopMass==655) { StopCrossSection = 	0.102081; StopCrossSectionUncertainty = StopCrossSection* 13.0012/100.; }
  if (iStopMass==660) { StopCrossSection = 	0.09725; StopCrossSectionUncertainty = StopCrossSection* 12.9038/100.; }
  if (iStopMass==665) { StopCrossSection = 	0.0927515; StopCrossSectionUncertainty = StopCrossSection* 12.9548/100.; }
  if (iStopMass==670) { StopCrossSection = 	0.0885084; StopCrossSectionUncertainty = StopCrossSection* 13.0218/100.; }
  if (iStopMass==675) { StopCrossSection = 	0.0844877; StopCrossSectionUncertainty = StopCrossSection* 13.0703/100.; }
  if (iStopMass==680) { StopCrossSection = 	0.0806192; StopCrossSectionUncertainty = StopCrossSection* 13.1131/100.; }
  if (iStopMass==685) { StopCrossSection = 	0.0769099; StopCrossSectionUncertainty = StopCrossSection* 13.1517/100.; }
  if (iStopMass==690) { StopCrossSection = 	0.0734901; StopCrossSectionUncertainty = StopCrossSection* 13.2344/100.; }
  if (iStopMass==695) { StopCrossSection = 	0.0701805; StopCrossSectionUncertainty = StopCrossSection* 13.2716/100.; }
  if (iStopMass==700) { StopCrossSection = 	0.0670476; StopCrossSectionUncertainty = StopCrossSection* 13.3429/100.; }
  if (iStopMass==705) { StopCrossSection = 	0.0641426; StopCrossSectionUncertainty = StopCrossSection* 13.363/100.; }
  if (iStopMass==710) { StopCrossSection = 	0.0612942; StopCrossSectionUncertainty = StopCrossSection* 13.3941/100.; }
  if (iStopMass==715) { StopCrossSection = 	0.0585678; StopCrossSectionUncertainty = StopCrossSection* 13.4663/100.; }
  if (iStopMass==720) { StopCrossSection = 	0.0560753; StopCrossSectionUncertainty = StopCrossSection* 13.4984/100.; }
  if (iStopMass==725) { StopCrossSection = 	0.0536438; StopCrossSectionUncertainty = StopCrossSection* 13.5804/100.; }
  if (iStopMass==730) { StopCrossSection = 	0.0513219; StopCrossSectionUncertainty = StopCrossSection* 13.5682/100.; }
  if (iStopMass==735) { StopCrossSection = 	0.0491001; StopCrossSectionUncertainty = StopCrossSection* 13.6268/100.; }
  if (iStopMass==740) { StopCrossSection = 	0.0470801; StopCrossSectionUncertainty = StopCrossSection* 13.6895/100.; }
  if (iStopMass==745) { StopCrossSection = 	0.045061; StopCrossSectionUncertainty = StopCrossSection* 13.6816/100.; }
  if (iStopMass==750) { StopCrossSection = 	0.0431418; StopCrossSectionUncertainty = StopCrossSection* 13.7455/100.; }
  if (iStopMass==755) { StopCrossSection = 	0.0413447; StopCrossSectionUncertainty = StopCrossSection* 13.7833/100.; }
  if (iStopMass==760) { StopCrossSection = 	0.0396264; StopCrossSectionUncertainty = StopCrossSection* 13.8518/100.; }
  if (iStopMass==765) { StopCrossSection = 	0.0379036; StopCrossSectionUncertainty = StopCrossSection* 13.8537/100.; }
  if (iStopMass==770) { StopCrossSection = 	0.0363856; StopCrossSectionUncertainty = StopCrossSection* 13.9334/100.; }
  if (iStopMass==775) { StopCrossSection = 	0.0348796; StopCrossSectionUncertainty = StopCrossSection* 13.9597/100.; }
  if (iStopMass==780) { StopCrossSection = 	0.0334669; StopCrossSectionUncertainty = StopCrossSection* 14.0267/100.; }
  if (iStopMass==785) { StopCrossSection = 	0.0320548; StopCrossSectionUncertainty = StopCrossSection* 14.0406/100.; }
  if (iStopMass==790) { StopCrossSection = 	0.0307373; StopCrossSectionUncertainty = StopCrossSection* 14.115/100.; }
  if (iStopMass==795) { StopCrossSection = 	0.0295348; StopCrossSectionUncertainty = StopCrossSection* 14.1397/100.; }
  if (iStopMass==800) { StopCrossSection = 	0.0283338; StopCrossSectionUncertainty = StopCrossSection* 14.171/100.; }
  if (iStopMass==805) { StopCrossSection = 	0.0272206; StopCrossSectionUncertainty = StopCrossSection* 14.241/100.; }
  if (iStopMass==810) { StopCrossSection = 	0.0261233; StopCrossSectionUncertainty = StopCrossSection* 14.2891/100.; }
  if (iStopMass==815) { StopCrossSection = 	0.0251107; StopCrossSectionUncertainty = StopCrossSection* 14.3632/100.; }
  if (iStopMass==820) { StopCrossSection = 	0.0241099; StopCrossSectionUncertainty = StopCrossSection* 14.3805/100.; }
  if (iStopMass==825) { StopCrossSection = 	0.0230866; StopCrossSectionUncertainty = StopCrossSection* 14.4428/100.; }
  if (iStopMass==830) { StopCrossSection = 	0.0221834; StopCrossSectionUncertainty = StopCrossSection* 14.4791/100.; }
  if (iStopMass==835) { StopCrossSection = 	0.0213766; StopCrossSectionUncertainty = StopCrossSection* 14.5511/100.; }
  if (iStopMass==840) { StopCrossSection = 	0.0204715; StopCrossSectionUncertainty = StopCrossSection* 14.6131/100.; }
  if (iStopMass==845) { StopCrossSection = 	0.0197653; StopCrossSectionUncertainty = StopCrossSection* 14.6602/100.; }
  if (iStopMass==850) { StopCrossSection = 	0.0189612; StopCrossSectionUncertainty = StopCrossSection* 14.702/100.; }
  if (iStopMass==855) { StopCrossSection = 	0.0182516; StopCrossSectionUncertainty = StopCrossSection* 14.7648/100.; }
  if (iStopMass==860) { StopCrossSection = 	0.0175509; StopCrossSectionUncertainty = StopCrossSection* 14.7944/100.; }
  if (iStopMass==865) { StopCrossSection = 	0.0168336; StopCrossSectionUncertainty = StopCrossSection* 14.8528/100.; }
  if (iStopMass==870) { StopCrossSection = 	0.0162314; StopCrossSectionUncertainty = StopCrossSection* 14.8772/100.; }
  if (iStopMass==875) { StopCrossSection = 	0.015625; StopCrossSectionUncertainty = StopCrossSection* 14.9567/100.; }
  if (iStopMass==880) { StopCrossSection = 	0.0150143; StopCrossSectionUncertainty = StopCrossSection* 15.0389/100.; }
  if (iStopMass==885) { StopCrossSection = 	0.0144112; StopCrossSectionUncertainty = StopCrossSection* 15.0614/100.; }
  if (iStopMass==890) { StopCrossSection = 	0.0138979; StopCrossSectionUncertainty = StopCrossSection* 15.1/100.; }
  if (iStopMass==895) { StopCrossSection = 	0.0133962; StopCrossSectionUncertainty = StopCrossSection* 15.1325/100.; }
  if (iStopMass==900) { StopCrossSection = 	0.0128895; StopCrossSectionUncertainty = StopCrossSection* 15.2026/100.; }
  if (iStopMass==905) { StopCrossSection = 	0.0123843; StopCrossSectionUncertainty = StopCrossSection* 15.2968/100.; }
  if (iStopMass==910) { StopCrossSection = 	0.0119837; StopCrossSectionUncertainty = StopCrossSection* 15.3089/100.; }
  if (iStopMass==915) { StopCrossSection = 	0.0114713; StopCrossSectionUncertainty = StopCrossSection* 15.3678/100.; }
  if (iStopMass==920) { StopCrossSection = 	0.0110688; StopCrossSectionUncertainty = StopCrossSection* 15.4082/100.; }
  if (iStopMass==925) { StopCrossSection = 	0.0106631; StopCrossSectionUncertainty = StopCrossSection* 15.4806/100.; }
  if (iStopMass==930) { StopCrossSection = 	0.0102629; StopCrossSectionUncertainty = StopCrossSection* 15.5313/100.; }
  if (iStopMass==935) { StopCrossSection = 	0.0098874; StopCrossSectionUncertainty = StopCrossSection* 15.6066/100.; }
  if (iStopMass==940) { StopCrossSection = 	0.00952142; StopCrossSectionUncertainty = StopCrossSection* 15.6055/100.; }
  if (iStopMass==945) { StopCrossSection = 	0.00916636; StopCrossSectionUncertainty = StopCrossSection* 15.6849/100.; }
  if (iStopMass==950) { StopCrossSection = 	0.00883465; StopCrossSectionUncertainty = StopCrossSection* 15.7177/100.; }
  if (iStopMass==955) { StopCrossSection = 	0.00851073; StopCrossSectionUncertainty = StopCrossSection* 15.8094/100.; }
  if (iStopMass==960) { StopCrossSection = 	0.00820884; StopCrossSectionUncertainty = StopCrossSection* 15.844/100.; }
  if (iStopMass==965) { StopCrossSection = 	0.00791403; StopCrossSectionUncertainty = StopCrossSection* 15.9216/100.; }
  if (iStopMass==970) { StopCrossSection = 	0.00763112; StopCrossSectionUncertainty = StopCrossSection* 15.9742/100.; }
  if (iStopMass==975) { StopCrossSection = 	0.00735655; StopCrossSectionUncertainty = StopCrossSection* 16.0548/100.; }
  if (iStopMass==980) { StopCrossSection = 	0.00710317; StopCrossSectionUncertainty = StopCrossSection* 16.0626/100.; }
  if (iStopMass==985) { StopCrossSection = 	0.00684867; StopCrossSectionUncertainty = StopCrossSection* 16.144/100.; }
  if (iStopMass==990) { StopCrossSection = 	0.00660695; StopCrossSectionUncertainty = StopCrossSection* 16.1813/100.; }
  if (iStopMass==995) { StopCrossSection = 	0.00637546; StopCrossSectionUncertainty = StopCrossSection* 16.2158/100.; }
  if (iStopMass==1000) { StopCrossSection = 	0.00615134; StopCrossSectionUncertainty = StopCrossSection* 16.2953/100.; }
  if (iStopMass==1005) { StopCrossSection = 	0.00593765; StopCrossSectionUncertainty = StopCrossSection* 16.3716/100.; }
  if (iStopMass==1010) { StopCrossSection = 	0.00572452; StopCrossSectionUncertainty = StopCrossSection* 16.3857/100.; }
  if (iStopMass==1015) { StopCrossSection = 	0.00553094; StopCrossSectionUncertainty = StopCrossSection* 16.4628/100.; }
  if (iStopMass==1020) { StopCrossSection = 	0.00533968; StopCrossSectionUncertainty = StopCrossSection* 16.4963/100.; }
  if (iStopMass==1025) { StopCrossSection = 	0.00514619; StopCrossSectionUncertainty = StopCrossSection* 16.5762/100.; }
  if (iStopMass==1030) { StopCrossSection = 	0.00497235; StopCrossSectionUncertainty = StopCrossSection* 16.5838/100.; }
  if (iStopMass==1035) { StopCrossSection = 	0.00479906; StopCrossSectionUncertainty = StopCrossSection* 16.6646/100.; }
  if (iStopMass==1040) { StopCrossSection = 	0.00463806; StopCrossSectionUncertainty = StopCrossSection* 16.6947/100.; }
  if (iStopMass==1045) { StopCrossSection = 	0.00447537; StopCrossSectionUncertainty = StopCrossSection* 16.7071/100.; }
  if (iStopMass==1050) { StopCrossSection = 	0.00432261; StopCrossSectionUncertainty = StopCrossSection* 16.7859/100.; }
  if (iStopMass==1055) { StopCrossSection = 	0.00417983; StopCrossSectionUncertainty = StopCrossSection* 16.8637/100.; }
  if (iStopMass==1060) { StopCrossSection = 	0.00403886; StopCrossSectionUncertainty = StopCrossSection* 16.8981/100.; }
  if (iStopMass==1065) { StopCrossSection = 	0.0038962; StopCrossSectionUncertainty = StopCrossSection* 16.9794/100.; }
  if (iStopMass==1070) { StopCrossSection = 	0.00376343; StopCrossSectionUncertainty = StopCrossSection* 16.9764/100.; }
  if (iStopMass==1075) { StopCrossSection = 	0.00364174; StopCrossSectionUncertainty = StopCrossSection* 17.0634/100.; }
  if (iStopMass==1080) { StopCrossSection = 	0.00352093; StopCrossSectionUncertainty = StopCrossSection* 17.0908/100.; }
  if (iStopMass==1085) { StopCrossSection = 	0.00339813; StopCrossSectionUncertainty = StopCrossSection* 17.1929/100.; }
  if (iStopMass==1090) { StopCrossSection = 	0.00328695; StopCrossSectionUncertainty = StopCrossSection* 17.2274/100.; }
  if (iStopMass==1095) { StopCrossSection = 	0.00317628; StopCrossSectionUncertainty = StopCrossSection* 17.2617/100.; }
  if (iStopMass==1100) { StopCrossSection = 	0.00307413; StopCrossSectionUncertainty = StopCrossSection* 17.3377/100.; }
  if (iStopMass==1105) { StopCrossSection = 	0.00297377; StopCrossSectionUncertainty = StopCrossSection* 17.3822/100.; }
  if (iStopMass==1110) { StopCrossSection = 	0.00287148; StopCrossSectionUncertainty = StopCrossSection* 17.4725/100.; }
  if (iStopMass==1115) { StopCrossSection = 	0.00278078; StopCrossSectionUncertainty = StopCrossSection* 17.5091/100.; }
  if (iStopMass==1120) { StopCrossSection = 	0.00268873; StopCrossSectionUncertainty = StopCrossSection* 17.5883/100.; }
  if (iStopMass==1125) { StopCrossSection = 	0.00260821; StopCrossSectionUncertainty = StopCrossSection* 17.6126/100.; }
  if (iStopMass==1130) { StopCrossSection = 	0.00251529; StopCrossSectionUncertainty = StopCrossSection* 17.6836/100.; }
  if (iStopMass==1135) { StopCrossSection = 	0.00243484; StopCrossSectionUncertainty = StopCrossSection* 17.7128/100.; }
  if (iStopMass==1140) { StopCrossSection = 	0.00236295; StopCrossSectionUncertainty = StopCrossSection* 17.7977/100.; }
  if (iStopMass==1145) { StopCrossSection = 	0.00228192; StopCrossSectionUncertainty = StopCrossSection* 17.8507/100.; }
  if (iStopMass==1150) { StopCrossSection = 	0.00221047; StopCrossSectionUncertainty = StopCrossSection* 17.9259/100.; }
  if (iStopMass==1155) { StopCrossSection = 	0.00213907; StopCrossSectionUncertainty = StopCrossSection* 18.0255/100.; }
  if (iStopMass==1160) { StopCrossSection = 	0.00206845; StopCrossSectionUncertainty = StopCrossSection* 18.0518/100.; }
  if (iStopMass==1165) { StopCrossSection = 	0.0020063; StopCrossSectionUncertainty = StopCrossSection* 18.0954/100.; }
  if (iStopMass==1170) { StopCrossSection = 	0.00194569; StopCrossSectionUncertainty = StopCrossSection* 18.1194/100.; }
  if (iStopMass==1175) { StopCrossSection = 	0.0018741; StopCrossSectionUncertainty = StopCrossSection* 18.2145/100.; }
  if (iStopMass==1180) { StopCrossSection = 	0.00182266; StopCrossSectionUncertainty = StopCrossSection* 18.3074/100.; }
  if (iStopMass==1185) { StopCrossSection = 	0.00176211; StopCrossSectionUncertainty = StopCrossSection* 18.3375/100.; }
  if (iStopMass==1190) { StopCrossSection = 	0.00170006; StopCrossSectionUncertainty = StopCrossSection* 18.4075/100.; }
  if (iStopMass==1195) { StopCrossSection = 	0.00164968; StopCrossSectionUncertainty = StopCrossSection* 18.4438/100.; }
  if (iStopMass==1200) { StopCrossSection = 	0.00159844; StopCrossSectionUncertainty = StopCrossSection* 18.5209/100.; }
  if (iStopMass==1205) { StopCrossSection = 	0.0015472; StopCrossSectionUncertainty = StopCrossSection* 18.5977/100.; }
  if (iStopMass==1210) { StopCrossSection = 	0.00149657; StopCrossSectionUncertainty = StopCrossSection* 18.6485/100.; }
  if (iStopMass==1215) { StopCrossSection = 	0.00145544; StopCrossSectionUncertainty = StopCrossSection* 18.7347/100.; }
  if (iStopMass==1220) { StopCrossSection = 	0.00140288; StopCrossSectionUncertainty = StopCrossSection* 18.8774/100.; }
  if (iStopMass==1225) { StopCrossSection = 	0.00136155; StopCrossSectionUncertainty = StopCrossSection* 18.989/100.; }
  if (iStopMass==1230) { StopCrossSection = 	0.00131271; StopCrossSectionUncertainty = StopCrossSection* 18.8763/100.; }
  if (iStopMass==1235) { StopCrossSection = 	0.0012717; StopCrossSectionUncertainty = StopCrossSection* 18.9588/100.; }
  if (iStopMass==1240) { StopCrossSection = 	0.00123066; StopCrossSectionUncertainty = StopCrossSection* 19.049/100.; }
  if (iStopMass==1245) { StopCrossSection = 	0.00119994; StopCrossSectionUncertainty = StopCrossSection* 19.1442/100.; }
  if (iStopMass==1250) { StopCrossSection = 	0.0011583; StopCrossSectionUncertainty = StopCrossSection* 19.3006/100.; }
  if (iStopMass==1255) { StopCrossSection = 	0.00112694; StopCrossSectionUncertainty = StopCrossSection* 19.4441/100.; }
  if (iStopMass==1260) { StopCrossSection = 	0.00108716; StopCrossSectionUncertainty = StopCrossSection* 19.4141/100.; }
  if (iStopMass==1265) { StopCrossSection = 	0.00105517; StopCrossSectionUncertainty = StopCrossSection* 19.6361/100.; }
  if (iStopMass==1270) { StopCrossSection = 	0.00102241; StopCrossSectionUncertainty = StopCrossSection* 19.6297/100.; }
  if (iStopMass==1275) { StopCrossSection = 	0.000991293; StopCrossSectionUncertainty = StopCrossSection* 19.762/100.; }
  if (iStopMass==1280) { StopCrossSection = 	0.000961012; StopCrossSectionUncertainty = StopCrossSection* 19.7926/100.; }
  if (iStopMass==1285) { StopCrossSection = 	0.000932394; StopCrossSectionUncertainty = StopCrossSection* 19.8682/100.; }
  if (iStopMass==1290) { StopCrossSection = 	0.000903404; StopCrossSectionUncertainty = StopCrossSection* 19.9924/100.; }
  if (iStopMass==1295) { StopCrossSection = 	0.000876957; StopCrossSectionUncertainty = StopCrossSection* 20.0777/100.; }
  if (iStopMass==1300) { StopCrossSection = 	0.000850345; StopCrossSectionUncertainty = StopCrossSection* 20.1604/100.; }
  if (iStopMass==1305) { StopCrossSection = 	0.00082443; StopCrossSectionUncertainty = StopCrossSection* 20.2883/100.; }
  if (iStopMass==1310) { StopCrossSection = 	0.00079983; StopCrossSectionUncertainty = StopCrossSection* 20.373/100.; }
  if (iStopMass==1315) { StopCrossSection = 	0.000775222; StopCrossSectionUncertainty = StopCrossSection* 20.4622/100.; }
  if (iStopMass==1320) { StopCrossSection = 	0.000751372; StopCrossSectionUncertainty = StopCrossSection* 20.5919/100.; }
  if (iStopMass==1325) { StopCrossSection = 	0.000728912; StopCrossSectionUncertainty = StopCrossSection* 20.6884/100.; }
  if (iStopMass==1330) { StopCrossSection = 	0.000706867; StopCrossSectionUncertainty = StopCrossSection* 20.7763/100.; }
  if (iStopMass==1335) { StopCrossSection = 	0.000685372; StopCrossSectionUncertainty = StopCrossSection* 20.8587/100.; }
  if (iStopMass==1340) { StopCrossSection = 	0.000664649; StopCrossSectionUncertainty = StopCrossSection* 20.9879/100.; }
  if (iStopMass==1345) { StopCrossSection = 	0.000644804; StopCrossSectionUncertainty = StopCrossSection* 21.1487/100.; }
  if (iStopMass==1350) { StopCrossSection = 	0.000625155; StopCrossSectionUncertainty = StopCrossSection* 21.2761/100.; }
  if (iStopMass==1355) { StopCrossSection = 	0.000606802; StopCrossSectionUncertainty = StopCrossSection* 21.3529/100.; }
  if (iStopMass==1360) { StopCrossSection = 	0.000588512; StopCrossSectionUncertainty = StopCrossSection* 21.4428/100.; }
  if (iStopMass==1365) { StopCrossSection = 	0.000570506; StopCrossSectionUncertainty = StopCrossSection* 21.6584/100.; }
  if (iStopMass==1370) { StopCrossSection = 	0.000553379; StopCrossSectionUncertainty = StopCrossSection* 21.6036/100.; }
  if (iStopMass==1375) { StopCrossSection = 	0.000536646; StopCrossSectionUncertainty = StopCrossSection* 21.775/100.; }
  if (iStopMass==1380) { StopCrossSection = 	0.000521404; StopCrossSectionUncertainty = StopCrossSection* 21.8383/100.; }
  if (iStopMass==1385) { StopCrossSection = 	0.000505008; StopCrossSectionUncertainty = StopCrossSection* 21.9675/100.; }
  if (iStopMass==1390) { StopCrossSection = 	0.000490353; StopCrossSectionUncertainty = StopCrossSection* 22.1444/100.; }
  if (iStopMass==1395) { StopCrossSection = 	0.000476164; StopCrossSectionUncertainty = StopCrossSection* 22.2016/100.; }
  if (iStopMass==1400) { StopCrossSection = 	0.000461944; StopCrossSectionUncertainty = StopCrossSection* 22.2704/100.; }
  if (iStopMass==1405) { StopCrossSection = 	0.000448172; StopCrossSectionUncertainty = StopCrossSection* 22.4911/100.; }
  if (iStopMass==1410) { StopCrossSection = 	0.000435082; StopCrossSectionUncertainty = StopCrossSection* 22.5606/100.; }
  if (iStopMass==1415) { StopCrossSection = 	0.000422967; StopCrossSectionUncertainty = StopCrossSection* 22.6095/100.; }
  if (iStopMass==1420) { StopCrossSection = 	0.000410381; StopCrossSectionUncertainty = StopCrossSection* 22.797/100.; }
  if (iStopMass==1425) { StopCrossSection = 	0.000398106; StopCrossSectionUncertainty = StopCrossSection* 22.8949/100.; }
  if (iStopMass==1430) { StopCrossSection = 	0.000386792; StopCrossSectionUncertainty = StopCrossSection* 23.1319/100.; }
  if (iStopMass==1435) { StopCrossSection = 	0.000375724; StopCrossSectionUncertainty = StopCrossSection* 23.1724/100.; }
  if (iStopMass==1440) { StopCrossSection = 	0.000364616; StopCrossSectionUncertainty = StopCrossSection* 23.2234/100.; }
  if (iStopMass==1445) { StopCrossSection = 	0.000353965; StopCrossSectionUncertainty = StopCrossSection* 23.4637/100.; }
  if (iStopMass==1450) { StopCrossSection = 	0.000343923; StopCrossSectionUncertainty = StopCrossSection* 23.4948/100.; }
  if (iStopMass==1455) { StopCrossSection = 	0.000333885; StopCrossSectionUncertainty = StopCrossSection* 23.5468/100.; }
  if (iStopMass==1460) { StopCrossSection = 	0.000324344; StopCrossSectionUncertainty = StopCrossSection* 23.771/100.; }
  if (iStopMass==1465) { StopCrossSection = 	0.0003153; StopCrossSectionUncertainty = StopCrossSection* 23.8004/100.; }
  if (iStopMass==1470) { StopCrossSection = 	0.00030583; StopCrossSectionUncertainty = StopCrossSection* 24.0064/100.; }
  if (iStopMass==1475) { StopCrossSection = 	0.000296811; StopCrossSectionUncertainty = StopCrossSection* 24.0314/100.; }
  if (iStopMass==1480) { StopCrossSection = 	0.000288149; StopCrossSectionUncertainty = StopCrossSection* 23.9248/100.; }
  if (iStopMass==1485) { StopCrossSection = 	0.000279711; StopCrossSectionUncertainty = StopCrossSection* 24.1257/100.; }
  if (iStopMass==1490) { StopCrossSection = 	0.000271724; StopCrossSectionUncertainty = StopCrossSection* 24.1274/100.; }
  if (iStopMass==1495) { StopCrossSection = 	0.000264275; StopCrossSectionUncertainty = StopCrossSection* 24.3545/100.; }
  if (iStopMass==1500) { StopCrossSection = 	0.000256248; StopCrossSectionUncertainty = StopCrossSection* 24.372/100.; }
  if (iStopMass==1505) { StopCrossSection = 	0.000248853; StopCrossSectionUncertainty = StopCrossSection* 24.5827/100.; }
  if (iStopMass==1510) { StopCrossSection = 	0.000241844; StopCrossSectionUncertainty = StopCrossSection* 24.6187/100.; }
  if (iStopMass==1515) { StopCrossSection = 	0.000234438; StopCrossSectionUncertainty = StopCrossSection* 24.8442/100.; }
  if (iStopMass==1520) { StopCrossSection = 	0.000227374; StopCrossSectionUncertainty = StopCrossSection* 24.8909/100.; }
  if (iStopMass==1525) { StopCrossSection = 	0.000221045; StopCrossSectionUncertainty = StopCrossSection* 25.0895/100.; }
  if (iStopMass==1530) { StopCrossSection = 	0.000214431; StopCrossSectionUncertainty = StopCrossSection* 24.8728/100.; }
  if (iStopMass==1535) { StopCrossSection = 	0.000208092; StopCrossSectionUncertainty = StopCrossSection* 25.1043/100.; }
  if (iStopMass==1540) { StopCrossSection = 	0.000201748; StopCrossSectionUncertainty = StopCrossSection* 25.3207/100.; }
  if (iStopMass==1545) { StopCrossSection = 	0.000196399; StopCrossSectionUncertainty = StopCrossSection* 25.5641/100.; }
  if (iStopMass==1550) { StopCrossSection = 	0.000190474; StopCrossSectionUncertainty = StopCrossSection* 25.5213/100.; }
  if (iStopMass==1555) { StopCrossSection = 	0.000185188; StopCrossSectionUncertainty = StopCrossSection* 25.7329/100.; }
  if (iStopMass==1560) { StopCrossSection = 	0.000179263; StopCrossSectionUncertainty = StopCrossSection* 25.6931/100.; }
  if (iStopMass==1565) { StopCrossSection = 	0.000174021; StopCrossSectionUncertainty = StopCrossSection* 25.9111/100.; }
  if (iStopMass==1570) { StopCrossSection = 	0.000169176; StopCrossSectionUncertainty = StopCrossSection* 25.8106/100.; }
  if (iStopMass==1575) { StopCrossSection = 	0.000163861; StopCrossSectionUncertainty = StopCrossSection* 26.0597/100.; }
  if (iStopMass==1580) { StopCrossSection = 	0.000159583; StopCrossSectionUncertainty = StopCrossSection* 26.2958/100.; }
  if (iStopMass==1585) { StopCrossSection = 	0.000154719; StopCrossSectionUncertainty = StopCrossSection* 26.195/100.; }
  if (iStopMass==1590) { StopCrossSection = 	0.000150506; StopCrossSectionUncertainty = StopCrossSection* 26.4111/100.; }
  if (iStopMass==1595) { StopCrossSection = 	0.000145626; StopCrossSectionUncertainty = StopCrossSection* 26.3077/100.; }
  if (iStopMass==1600) { StopCrossSection = 	0.000141382; StopCrossSectionUncertainty = StopCrossSection* 26.5291/100.; }
  if (iStopMass==1605) { StopCrossSection = 	0.000137131; StopCrossSectionUncertainty = StopCrossSection* 26.7424/100.; }
  if (iStopMass==1610) { StopCrossSection = 	0.000132187; StopCrossSectionUncertainty = StopCrossSection* 26.668/100.; }
  if (iStopMass==1615) { StopCrossSection = 	0.000127929; StopCrossSectionUncertainty = StopCrossSection* 26.9117/100.; }
  if (iStopMass==1620) { StopCrossSection = 	0.000124086; StopCrossSectionUncertainty = StopCrossSection* 26.7738/100.; }
  if (iStopMass==1625) { StopCrossSection = 	0.00011982; StopCrossSectionUncertainty = StopCrossSection* 27.0483/100.; }
  if (iStopMass==1630) { StopCrossSection = 	0.000116042; StopCrossSectionUncertainty = StopCrossSection* 26.8071/100.; }
  if (iStopMass==1635) { StopCrossSection = 	0.000112767; StopCrossSectionUncertainty = StopCrossSection* 27.127/100.; }
  if (iStopMass==1640) { StopCrossSection = 	0.000108936; StopCrossSectionUncertainty = StopCrossSection* 26.9351/100.; }
  if (iStopMass==1645) { StopCrossSection = 	0.000105746; StopCrossSectionUncertainty = StopCrossSection* 27.1783/100.; }
  if (iStopMass==1650) { StopCrossSection = 	0.000102693; StopCrossSectionUncertainty = StopCrossSection* 27.292/100.; }
  if (iStopMass==1655) { StopCrossSection = 	0.000100112; StopCrossSectionUncertainty = StopCrossSection* 27.4445/100.; }
  if (iStopMass==1660) { StopCrossSection = 	9.75763e-05; StopCrossSectionUncertainty = StopCrossSection* 27.5431/100.; }
  if (iStopMass==1665) { StopCrossSection = 	9.52062e-05; StopCrossSectionUncertainty = StopCrossSection* 27.6946/100.; }
  if (iStopMass==1670) { StopCrossSection = 	9.29857e-05; StopCrossSectionUncertainty = StopCrossSection* 27.7869/100.; }
  if (iStopMass==1675) { StopCrossSection = 	9.08285e-05; StopCrossSectionUncertainty = StopCrossSection* 27.9347/100.; }
  if (iStopMass==1680) { StopCrossSection = 	8.87433e-05; StopCrossSectionUncertainty = StopCrossSection* 28.1539/100.; }
  if (iStopMass==1685) { StopCrossSection = 	8.66618e-05; StopCrossSectionUncertainty = StopCrossSection* 28.3509/100.; }
  if (iStopMass==1690) { StopCrossSection = 	8.46535e-05; StopCrossSectionUncertainty = StopCrossSection* 28.4432/100.; }
  if (iStopMass==1695) { StopCrossSection = 	8.27102e-05; StopCrossSectionUncertainty = StopCrossSection* 28.591/100.; }
  if (iStopMass==1700) { StopCrossSection = 	8.07774e-05; StopCrossSectionUncertainty = StopCrossSection* 28.7497/100.; }
  if (iStopMass==1705) { StopCrossSection = 	7.8666e-05; StopCrossSectionUncertainty = StopCrossSection* 28.8194/100.; }
  if (iStopMass==1710) { StopCrossSection = 	7.6572e-05; StopCrossSectionUncertainty = StopCrossSection* 29.0265/100.; }
  if (iStopMass==1715) { StopCrossSection = 	7.45994e-05; StopCrossSectionUncertainty = StopCrossSection* 29.1193/100.; }
  if (iStopMass==1720) { StopCrossSection = 	7.25199e-05; StopCrossSectionUncertainty = StopCrossSection* 29.3013/100.; }
  if (iStopMass==1725) { StopCrossSection = 	7.05189e-05; StopCrossSectionUncertainty = StopCrossSection* 29.3697/100.; }
  if (iStopMass==1730) { StopCrossSection = 	6.85712e-05; StopCrossSectionUncertainty = StopCrossSection* 29.4972/100.; }
  if (iStopMass==1735) { StopCrossSection = 	6.67296e-05; StopCrossSectionUncertainty = StopCrossSection* 29.6167/100.; }
  if (iStopMass==1740) { StopCrossSection = 	6.49184e-05; StopCrossSectionUncertainty = StopCrossSection* 29.7686/100.; }
  if (iStopMass==1745) { StopCrossSection = 	6.30949e-05; StopCrossSectionUncertainty = StopCrossSection* 29.8524/100.; }
  if (iStopMass==1750) { StopCrossSection = 	6.13637e-05; StopCrossSectionUncertainty = StopCrossSection* 29.9789/100.; }
  if (iStopMass==1755) { StopCrossSection = 	5.97301e-05; StopCrossSectionUncertainty = StopCrossSection* 30.0928/100.; }
  if (iStopMass==1760) { StopCrossSection = 	5.80751e-05; StopCrossSectionUncertainty = StopCrossSection* 30.2585/100.; }
  if (iStopMass==1765) { StopCrossSection = 	5.65479e-05; StopCrossSectionUncertainty = StopCrossSection* 30.366/100.; }
  if (iStopMass==1770) { StopCrossSection = 	5.49998e-05; StopCrossSectionUncertainty = StopCrossSection* 30.5241/100.; }
  if (iStopMass==1775) { StopCrossSection = 	5.35686e-05; StopCrossSectionUncertainty = StopCrossSection* 30.6718/100.; }
  if (iStopMass==1780) { StopCrossSection = 	5.20828e-05; StopCrossSectionUncertainty = StopCrossSection* 30.6799/100.; }
  if (iStopMass==1785) { StopCrossSection = 	5.07079e-05; StopCrossSectionUncertainty = StopCrossSection* 30.9201/100.; }
  if (iStopMass==1790) { StopCrossSection = 	4.93948e-05; StopCrossSectionUncertainty = StopCrossSection* 31.0043/100.; }
  if (iStopMass==1795) { StopCrossSection = 	4.80635e-05; StopCrossSectionUncertainty = StopCrossSection* 31.138/100.; }
  if (iStopMass==1800) { StopCrossSection = 	4.67492e-05; StopCrossSectionUncertainty = StopCrossSection* 31.2291/100.; }
  if (iStopMass==1805) { StopCrossSection = 	4.55055e-05; StopCrossSectionUncertainty = StopCrossSection* 31.4321/100.; }
  if (iStopMass==1810) { StopCrossSection = 	4.42835e-05; StopCrossSectionUncertainty = StopCrossSection* 31.5499/100.; }
  if (iStopMass==1815) { StopCrossSection = 	4.30744e-05; StopCrossSectionUncertainty = StopCrossSection* 31.6302/100.; }
  if (iStopMass==1820) { StopCrossSection = 	4.19954e-05; StopCrossSectionUncertainty = StopCrossSection* 31.7151/100.; }
  if (iStopMass==1825) { StopCrossSection = 	4.08527e-05; StopCrossSectionUncertainty = StopCrossSection* 31.9048/100.; }
  if (iStopMass==1830) { StopCrossSection = 	3.97561e-05; StopCrossSectionUncertainty = StopCrossSection* 31.9718/100.; }
  if (iStopMass==1835) { StopCrossSection = 	3.87041e-05; StopCrossSectionUncertainty = StopCrossSection* 32.2028/100.; }
  if (iStopMass==1840) { StopCrossSection = 	3.76008e-05; StopCrossSectionUncertainty = StopCrossSection* 32.268/100.; }
  if (iStopMass==1845) { StopCrossSection = 	3.66914e-05; StopCrossSectionUncertainty = StopCrossSection* 32.4529/100.; }
  if (iStopMass==1850) { StopCrossSection = 	3.56995e-05; StopCrossSectionUncertainty = StopCrossSection* 32.5039/100.; }
  if (iStopMass==1855) { StopCrossSection = 	3.47689e-05; StopCrossSectionUncertainty = StopCrossSection* 32.6767/100.; }
  if (iStopMass==1860) { StopCrossSection = 	3.38528e-05; StopCrossSectionUncertainty = StopCrossSection* 32.8878/100.; }
  if (iStopMass==1865) { StopCrossSection = 	3.29644e-05; StopCrossSectionUncertainty = StopCrossSection* 32.8975/100.; }
  if (iStopMass==1870) { StopCrossSection = 	3.20679e-05; StopCrossSectionUncertainty = StopCrossSection* 32.9608/100.; }
  if (iStopMass==1875) { StopCrossSection = 	3.12583e-05; StopCrossSectionUncertainty = StopCrossSection* 33.1541/100.; }
  if (iStopMass==1880) { StopCrossSection = 	3.04342e-05; StopCrossSectionUncertainty = StopCrossSection* 33.3117/100.; }
  if (iStopMass==1885) { StopCrossSection = 	2.96516e-05; StopCrossSectionUncertainty = StopCrossSection* 33.2866/100.; }
  if (iStopMass==1890) { StopCrossSection = 	2.88952e-05; StopCrossSectionUncertainty = StopCrossSection* 33.6279/100.; }
  if (iStopMass==1895) { StopCrossSection = 	2.81145e-05; StopCrossSectionUncertainty = StopCrossSection* 33.6845/100.; }
  if (iStopMass==1900) { StopCrossSection = 	2.73974e-05; StopCrossSectionUncertainty = StopCrossSection* 33.8247/100.; }
  if (iStopMass==1905) { StopCrossSection = 	2.66796e-05; StopCrossSectionUncertainty = StopCrossSection* 33.9708/100.; }
  if (iStopMass==1910) { StopCrossSection = 	2.59941e-05; StopCrossSectionUncertainty = StopCrossSection* 33.9526/100.; }
  if (iStopMass==1915) { StopCrossSection = 	2.52784e-05; StopCrossSectionUncertainty = StopCrossSection* 34.1137/100.; }
  if (iStopMass==1920) { StopCrossSection = 	2.46598e-05; StopCrossSectionUncertainty = StopCrossSection* 34.2714/100.; }
  if (iStopMass==1925) { StopCrossSection = 	2.39932e-05; StopCrossSectionUncertainty = StopCrossSection* 34.2328/100.; }
  if (iStopMass==1930) { StopCrossSection = 	2.33737e-05; StopCrossSectionUncertainty = StopCrossSection* 34.394/100.; }
  if (iStopMass==1935) { StopCrossSection = 	2.27623e-05; StopCrossSectionUncertainty = StopCrossSection* 34.5138/100.; }
  if (iStopMass==1940) { StopCrossSection = 	2.21454e-05; StopCrossSectionUncertainty = StopCrossSection* 34.6933/100.; }
  if (iStopMass==1945) { StopCrossSection = 	2.15924e-05; StopCrossSectionUncertainty = StopCrossSection* 35.0815/100.; }
  if (iStopMass==1950) { StopCrossSection = 	2.10232e-05; StopCrossSectionUncertainty = StopCrossSection* 34.9444/100.; }
  if (iStopMass==1955) { StopCrossSection = 	2.05211e-05; StopCrossSectionUncertainty = StopCrossSection* 35.0155/100.; }
  if (iStopMass==1960) { StopCrossSection = 	1.98996e-05; StopCrossSectionUncertainty = StopCrossSection* 35.2135/100.; }
  if (iStopMass==1965) { StopCrossSection = 	1.9408e-05; StopCrossSectionUncertainty = StopCrossSection* 35.3328/100.; }
  if (iStopMass==1970) { StopCrossSection = 	1.88974e-05; StopCrossSectionUncertainty = StopCrossSection* 35.4643/100.; }
  if (iStopMass==1975) { StopCrossSection = 	1.84612e-05; StopCrossSectionUncertainty = StopCrossSection* 35.7904/100.; }
  if (iStopMass==1980) { StopCrossSection = 	1.79562e-05; StopCrossSectionUncertainty = StopCrossSection* 35.8898/100.; }
  if (iStopMass==1985) { StopCrossSection = 	1.75673e-05; StopCrossSectionUncertainty = StopCrossSection* 35.989/100.; }
  if (iStopMass==1990) { StopCrossSection = 	1.70612e-05; StopCrossSectionUncertainty = StopCrossSection* 36.0953/100.; }
  if (iStopMass==1995) { StopCrossSection = 	1.66228e-05; StopCrossSectionUncertainty = StopCrossSection* 36.4709/100.; }
  if (iStopMass==2000) { StopCrossSection = 	1.62355e-05; StopCrossSectionUncertainty = StopCrossSection* 36.5277/100.; }


} 
