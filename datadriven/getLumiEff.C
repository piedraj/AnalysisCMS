const float zmin =  76;  // [GeV]
const float zmax = 106;  // [GeV]

bool highpt = true;

int bin_zmin;
int bin_zmax;

int bin_ele_ptmin;
int bin_ele_ptmax;

int bin_muon_ptmin;
int bin_muon_ptmax;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// root -l getLumiEff.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void getLumiEff()
{

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  //TFile*  data  = new TFile ("../rootfiles/nominal/FR/01_Data.root","read");
  //TFile*  tt = new TFile ("../rootfilesFR/nominal/FR/06_TT.root","read");
  //TFile*  zjets = new TFile ("../rootfiles/nominal/FR/07_ZJets.root","read");
  //TFile*  wjets  = new TFile ("../rootfiles/nominal/FR/08_WJets.root","read");

  TFile*  data  = new TFile ("../rootfiles/FirstWP/FR/01_Data.root","read");
  //TFile*  tt = new TFile ("../rootfilesFR/nominal/FR/06_TT.root","read");
  TFile*  zjets = new TFile ("../rootfiles/FirstWP/FR/07_ZJets.root","read");
  TFile*  wjets  = new TFile ("../rootfiles/FirstWP/FR/08_WJets.root","read");

  float muonjetet = 25.;
  float elejetet = 35.;

  TString muonsuffix = Form("_%.0fGeV", muonjetet);
  TString elesuffix = Form("_%.0fGeV", elejetet);

  // DATA

  TH2D* h2_Ele_loose_pt_m2l_ZRegion  = (TH2D*) data -> Get("FR/01_Zpeak/h_Ele_loose_pt_m2l"  + elesuffix);
  TH2D* h2_Ele_tight_pt_m2l_ZRegion  = (TH2D*) data -> Get("FR/01_Zpeak/h_Ele_tight_pt_m2l"  + elesuffix);
  TH2D* h2_Muon_loose_pt_m2l_ZRegion = (TH2D*) data -> Get("FR/01_Zpeak/h_Muon_loose_pt_m2l" + muonsuffix);
  TH2D* h2_Muon_tight_pt_m2l_ZRegion = (TH2D*) data -> Get("FR/01_Zpeak/h_Muon_tight_pt_m2l" + muonsuffix);

  if (!highpt) {
    bin_ele_ptmin = -1;
    bin_ele_ptmax = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.) - 1;
  } else {    
    bin_ele_ptmin = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.);
    bin_ele_ptmax = -1;
  }  

  if (!highpt) {
    bin_muon_ptmin = -1;  
    bin_muon_ptmax = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.) - 1;
  } else {
    bin_muon_ptmin = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.);
    bin_muon_ptmax = -1;
  }

  TH1D* h_Ele_loose_m2l_ZRegion  = (TH1D*) h2_Ele_loose_pt_m2l_ZRegion  -> ProjectionX("Ele loose m2l data",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Ele_tight_m2l_ZRegion  = (TH1D*) h2_Ele_tight_pt_m2l_ZRegion  -> ProjectionX("Ele tight m2l data",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Muon_loose_m2l_ZRegion = (TH1D*) h2_Muon_loose_pt_m2l_ZRegion -> ProjectionX("Muon loose m2l data", bin_muon_ptmin, bin_muon_ptmax);
  TH1D* h_Muon_tight_m2l_ZRegion = (TH1D*) h2_Muon_tight_pt_m2l_ZRegion -> ProjectionX("Muon tight m2l data", bin_muon_ptmin, bin_muon_ptmax);
 
  // ZJETS

  TH2D* h2_Ele_loose_pt_m2l_ZRegion_zjets  = (TH2D*) zjets -> Get("FR/01_Zpeak/h_Ele_loose_pt_m2l"  + elesuffix);
  TH2D* h2_Ele_tight_pt_m2l_ZRegion_zjets  = (TH2D*) zjets -> Get("FR/01_Zpeak/h_Ele_tight_pt_m2l"  + elesuffix);
  TH2D* h2_Muon_loose_pt_m2l_ZRegion_zjets = (TH2D*) zjets -> Get("FR/01_Zpeak/h_Muon_loose_pt_m2l" + muonsuffix);
  TH2D* h2_Muon_tight_pt_m2l_ZRegion_zjets = (TH2D*) zjets -> Get("FR/01_Zpeak/h_Muon_tight_pt_m2l" + muonsuffix);
  
  if (!highpt) {
    bin_ele_ptmin = -1;
    bin_ele_ptmax = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.) - 1;
  } else {
    bin_ele_ptmin = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.);
    bin_ele_ptmax = -1;
  }
  
  if (!highpt) {
    bin_muon_ptmin = -1;
    bin_muon_ptmax = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.) - 1;
  } else {
    bin_muon_ptmin = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.);
    bin_muon_ptmax = -1;
  }

  TH1D* h_Ele_loose_m2l_ZRegion_zjets  = (TH1D*) h2_Ele_loose_pt_m2l_ZRegion_zjets  -> ProjectionX("Ele loose m2l zjets",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Ele_tight_m2l_ZRegion_zjets  = (TH1D*) h2_Ele_tight_pt_m2l_ZRegion_zjets  -> ProjectionX("Ele tight m2l zjets",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Muon_loose_m2l_ZRegion_zjets = (TH1D*) h2_Muon_loose_pt_m2l_ZRegion_zjets -> ProjectionX("Muon loose m2l zjets", bin_muon_ptmin, bin_muon_ptmax);
  TH1D* h_Muon_tight_m2l_ZRegion_zjets = (TH1D*) h2_Muon_tight_pt_m2l_ZRegion_zjets -> ProjectionX("Muon tight m2l zjets", bin_muon_ptmin, bin_muon_ptmax);

  // WJETS

  TH2D* h2_Ele_loose_pt_m2l_ZRegion_wjets  = (TH2D*) wjets -> Get("FR/01_Zpeak/h_Ele_loose_pt_m2l"  + elesuffix);
  TH2D* h2_Ele_tight_pt_m2l_ZRegion_wjets  = (TH2D*) wjets -> Get("FR/01_Zpeak/h_Ele_tight_pt_m2l"  + elesuffix);
  TH2D* h2_Muon_loose_pt_m2l_ZRegion_wjets = (TH2D*) wjets -> Get("FR/01_Zpeak/h_Muon_loose_pt_m2l" + muonsuffix);
  TH2D* h2_Muon_tight_pt_m2l_ZRegion_wjets = (TH2D*) wjets -> Get("FR/01_Zpeak/h_Muon_tight_pt_m2l" + muonsuffix);

  if (!highpt) {
    bin_ele_ptmin = -1;
    bin_ele_ptmax = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.) - 1;
  } else {
    bin_ele_ptmin = h2_Ele_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(25.);
    bin_ele_ptmax = -1;
  }

  if (!highpt) {   
    bin_muon_ptmin = -1; 
    bin_muon_ptmax = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.) - 1;
  } else {
    bin_muon_ptmin = h2_Muon_tight_pt_m2l_ZRegion -> ProjectionY() -> FindBin(20.);
    bin_muon_ptmax = -1;
  }  

  TH1D* h_Ele_loose_m2l_ZRegion_wjets  = (TH1D*) h2_Ele_loose_pt_m2l_ZRegion_wjets  -> ProjectionX("Ele loose m2l wjets",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Ele_tight_m2l_ZRegion_wjets  = (TH1D*) h2_Ele_tight_pt_m2l_ZRegion_wjets  -> ProjectionX("Ele tight m2l wjets",  bin_ele_ptmin,  bin_ele_ptmax);
  TH1D* h_Muon_loose_m2l_ZRegion_wjets = (TH1D*) h2_Muon_loose_pt_m2l_ZRegion_wjets -> ProjectionX("Muon loose m2l wjets", bin_muon_ptmin, bin_muon_ptmax);
  TH1D* h_Muon_tight_m2l_ZRegion_wjets = (TH1D*) h2_Muon_tight_pt_m2l_ZRegion_wjets -> ProjectionX("Muon tight m2l wjets", bin_muon_ptmin, bin_muon_ptmax);

  bin_zmin = h_Ele_loose_m2l_ZRegion->FindBin(zmin);
  bin_zmax = h_Ele_loose_m2l_ZRegion->FindBin(zmax);

  // Effective luminosity calculation

  printf("============ Datos ============ \n \n");

  float ele_loose_data =  h_Ele_loose_m2l_ZRegion -> Integral(bin_zmin, bin_zmax);
  float ele_tight_data = h_Ele_tight_m2l_ZRegion -> Integral(bin_zmin, bin_zmax);
  float muon_loose_data = h_Muon_loose_m2l_ZRegion -> Integral(bin_zmin, bin_zmax);
  float muon_tight_data = h_Muon_tight_m2l_ZRegion -> Integral(bin_zmin, bin_zmax);

  printf("Loose electrons in the Z window = %.0f \n", ele_loose_data);
  printf("Tight electrons in the Z window = %.0f \n", ele_tight_data);
  printf("Loose muons in the Z window = %.0f \n", muon_loose_data);
  printf("Tight muons in the Z window = %.0f \n \n", muon_tight_data);
  
  float ele_loose_tt = 0.;
  float ele_tight_tt = 0.;
  float muon_loose_tt = 0.;
  float muon_tight_tt = 0.;

  /*
  printf("============ TT ============ \n \n");

  float ele_loose_tt = h_Ele_loose_m2l_ZRegion_tt -> Integral(bin_zmin, bin_zmax);
  float ele_tight_tt = h_Ele_tight_m2l_ZRegion_tt -> Integral(bin_zmin, bin_zmax);
  float muon_loose_tt = h_Muon_loose_m2l_ZRegion_tt -> Integral(bin_zmin, bin_zmax);
  float muon_tight_tt = h_Muon_tight_m2l_ZRegion_tt -> Integral(bin_zmin, bin_zmax);

  printf("Loose electrons in the Z window = %.0f \n", ele_loose_tt);
  printf("Tight electrons in the Z window = %.0f \n", ele_tight_tt);
  printf("Loose muons in the Z window = %.0f \n", muon_loose_tt);
  printf("Tight muons in the Z window = %.0f \n \n", muon_tight_tt);
  */

  printf("============ ZJets ============ \n \n");

  float ele_loose_zjets =  h_Ele_loose_m2l_ZRegion_zjets -> Integral(bin_zmin, bin_zmax);
  float ele_tight_zjets = h_Ele_tight_m2l_ZRegion_zjets -> Integral(bin_zmin, bin_zmax);
  float muon_loose_zjets = h_Muon_loose_m2l_ZRegion_zjets -> Integral(bin_zmin, bin_zmax);
  float muon_tight_zjets = h_Muon_tight_m2l_ZRegion_zjets -> Integral(bin_zmin, bin_zmax);

  printf("Loose electrons in the Z window = %.0f \n", ele_loose_zjets);
  printf("Tight electrons in the Z window = %.0f \n", ele_tight_zjets);
  printf("Loose muons in the Z window = %.0f \n", muon_loose_zjets);
  printf("Tight muons in the Z window = %.0f \n \n", muon_tight_zjets);
  
  printf("============ WJets ============ \n \n");

  float ele_loose_wjets  = max(0.0, h_Ele_loose_m2l_ZRegion_wjets  -> Integral(bin_zmin, bin_zmax));
  float ele_tight_wjets  = max(0.0, h_Ele_tight_m2l_ZRegion_wjets  -> Integral(bin_zmin, bin_zmax));
  float muon_loose_wjets = max(0.0, h_Muon_loose_m2l_ZRegion_wjets -> Integral(bin_zmin, bin_zmax));
  float muon_tight_wjets = max(0.0, h_Muon_tight_m2l_ZRegion_wjets -> Integral(bin_zmin, bin_zmax));

  printf("Loose electrons in the Z window = %.0f \n", ele_loose_wjets);
  printf("Tight electrons in the Z window = %.0f \n", ele_tight_wjets);
  printf("Loose muons in the Z window = %.0f \n", muon_loose_wjets);
  printf("Tight muons in the Z window = %.0f \n \n", muon_tight_wjets);

  float lum_ele_loose = 0.;
  float lum_ele_tight = 0.;
  float lum_muon_loose = 0.;
  float lum_muon_tight = 0.;

  lum_ele_loose  = ele_loose_data / (ele_loose_tt + ele_loose_zjets + ele_loose_wjets);
  lum_ele_tight  = ele_tight_data / (ele_tight_tt + ele_tight_zjets + ele_tight_wjets);
  lum_muon_loose = muon_loose_data / (muon_loose_tt + muon_loose_zjets + muon_loose_wjets);
  lum_muon_tight = muon_tight_data / (muon_tight_tt + muon_tight_zjets + muon_tight_wjets);

  // Statistic uncertainty

  float stat_ele_loose = 0.0;
  float stat_ele_tight = 0.0;
  float stat_muon_loose = 0.0;
  float stat_muon_tight = 0.0;

  float first_term = pow(1/sqrt(ele_loose_data),2);
  float second_term = pow((sqrt(ele_loose_tt) + sqrt(ele_loose_zjets) + sqrt(ele_loose_wjets)) / (ele_loose_tt + ele_loose_zjets + ele_loose_wjets),2);

  stat_ele_loose = lum_ele_loose * sqrt(first_term + second_term);

  first_term = pow(1/sqrt(ele_tight_data),2);
  second_term = pow((sqrt(ele_tight_tt) + sqrt(ele_tight_zjets) + sqrt(ele_tight_wjets)) / (ele_tight_tt + ele_tight_zjets + ele_tight_wjets),2);

  stat_ele_tight = lum_ele_tight * sqrt(first_term + second_term);

  first_term = pow(1/sqrt(muon_loose_data),2);
  second_term = pow((sqrt(muon_loose_tt) + sqrt(muon_loose_zjets) + sqrt(muon_loose_wjets)) / (muon_loose_tt + muon_loose_zjets + muon_loose_wjets),2);

  stat_muon_loose = lum_muon_loose * sqrt(first_term + second_term);

  first_term = pow(1/sqrt(muon_tight_data),2);
  second_term = pow((sqrt(muon_tight_tt) + sqrt(muon_tight_zjets) + sqrt(muon_tight_wjets)) / (muon_tight_tt + muon_tight_zjets + muon_tight_wjets),2);

  stat_muon_tight = lum_muon_tight * sqrt(first_term + second_term);

  printf("===================== Effective luminosity ===================== \n \n");
	
  printf("Effective luminosity in electrons loose = (%.2f pm %.2f) pb\n", 1e3 * lum_ele_loose, 1e3 * stat_ele_loose);
  printf("Effective luminosity in electrons tight = (%.2f pm %.2f) pb\n", 1e3 * lum_ele_tight, 1e3 * stat_ele_tight);
  printf("Effective luminosity in muons     loose = (%.2f pm %.2f) pb\n", 1e3 * lum_muon_loose, 1e3 * stat_muon_loose);
  printf("Effective luminosity in muons     tight = (%.2f pm %.2f) pb\n", 1e3 * lum_muon_tight, 1e3 * stat_muon_tight);
  printf("\n");

  printf(" Z-peak data/MC in electrons loose = %.3f\n", lum_ele_loose);
  printf(" Z-peak data/MC in electrons tight = %.3f\n", lum_ele_tight);
  printf(" Z-peak data/MC in muons     loose = %.3f\n", lum_muon_loose);
  printf(" Z-peak data/MC in muons     tight = %.3f\n", lum_muon_tight);
  printf("\n");
}

