#include "TH1F.h"

using namespace std;

void dy_stimation() {

 // ZJets 
 //-----------
 
  double DyI ;
  double DyO ;
  double DyO1;
  double DyO2;
  double R   ;

  double DyI_error ;
  double DyO_error ;
  double DyO1_error;
  double DyO2_error;
  double R_error   ;

  TFile *dy_Met = new TFile("rootfiles/nominal/Top/07_ZJets.root","read");
  TH2D*  met_m2l = (TH2D*) dy_Met -> Get( "Top/02_Restability/0jet/h_metvar_m2l_mm");
  TH1D *pxI  = met_m2l -> ProjectionX("m2lProyectionOnMet_pxI", 76, 106);
                // where firstXbin = 76 and lastXbin = 106
  TH1D *pxO1 = met_m2l -> ProjectionX("m2lProyectionOnMet_pxO1", 20, 75);
  TH1D *pxO2 = met_m2l -> ProjectionX("m2lProyectionOnMet_pxO2", 107, 3000);
 
  DyI  = pxI  -> Integral (4,4);
  DyO1 = pxO1 -> Integral (4,4);
  DyO2 = pxO2 -> Integral (4,4);
  DyO  = DyO1 + DyO2;
  R    = DyO / DyI;

  DyI_error  = sqrt(pxI -> GetSumw2() -> At(4));
  DyO1_error = sqrt(pxO1-> GetSumw2() -> At(4));
  DyO2_error = sqrt(pxO2-> GetSumw2() -> At(4));
  DyO_error  = sqrt(pow(DyO1_error,2) + pow(DyO2_error,2));
  R_error    = R*sqrt(pow(DyO_error,2)/pow(DyO,2) + pow(DyI_error,2)/pow(DyI,2));

  std::cout << "ZJets " << " \tmm " << endl; 

  std::cout<< " 0 jets" << " & " << R << " $\\pm$ " << R_error <<  "(" << DyO << ", " << DyI << ")" << endl;
  
 


 //Peaking Backgrounds
 //-------------------
 
 //ZZ -> Included in ZJets => DY/ZZ instead of only DY 
 //WZ -> Further considerations


 // Non Peaking Backgrounds: 
 //------------------------------------------------------------------------

 // tw, WW, WJets, ttbar full leptonic (my signal) 

	TFile* f_data = new TFile("rootfiles/nominal/Top/01_Data.root", "read");

        TH2D*  data_ee    = (TH2D*) f_data    -> Get( "Top/02_Restability/0jet/h_metvar_m2l_ee");
        TH2D*  data_mm    = (TH2D*) f_data    -> Get( "Top/02_Restability/0jet/h_metvar_m2l_mm");
        TH2D*  data_em    = (TH2D*) f_data    -> Get( "Top/02_Restability/0jet/h_metvar_m2l_mm");

        TH1D *pxI_data_ee = data_ee -> ProjectionX("m2lProyectionOnMet_pxI_data_ee", 76, 106);
        TH1D *pxI_data_mm = data_mm -> ProjectionX("m2lProyectionOnMet_pxI_data_mm", 76, 106);
        TH1D *pxI_data_em = data_em -> ProjectionX("m2lProyectionOnMet_pxI_data_em", 76, 106);

	double n_ee = pxI_data_ee -> Integral (-1,5);
	double n_mm = pxI_data_ee -> Integral (-1,5);

	double n_ee_err       = sqrt( pxI_data_ee -> GetSumw2() -> GetSum());
	double n_mm_err       = sqrt( pxI_data_mm -> GetSumw2() -> GetSum());

	double kee = 0.5*sqrt(n_ee /  n_mm);
        double kmm  = 0.5*sqrt(n_mm /  n_ee);

        double err_kmm = 0.5*sqrt(1/(4*n_ee) + 4*n_mm/pow(n_ee,2));
        double err_kee = 0.5*sqrt(1/(4*n_mm) + 4*n_ee/pow(n_mm,2));

	double In_data_ee = pxI_data_ee -> Integral (4,4);
        double In_data_mm = pxI_data_mm -> Integral (4,4);
        double In_data_em = pxI_data_em -> Integral (4,4);

	double In_data_ee_err = sqrt(In_data_ee + pow(kee,2)*In_data_em + pow(In_data_em,2)*pow(err_kee,2));                        
	double In_data_mm_err = sqrt(In_data_mm + pow(kmm,2)*In_data_em + pow(In_data_em,2)*pow(err_kmm,2));                        

	double Out_data_ee = (In_data_ee - kee*In_data_em)*R;	
 	double Out_data_mm = (In_data_mm - kmm*In_data_em)*R;

	double Out_data_ee_err = Out_data_ee*sqrt(pow(R_error,2)/pow(R,2) + pow(In_data_ee_err,2)/pow(In_data_ee,2));
	double Out_data_mm_err = Out_data_mm*sqrt(pow(R_error,2)/pow(R,2) + pow(In_data_mm_err,2)/pow(In_data_mm,2));
   
}
