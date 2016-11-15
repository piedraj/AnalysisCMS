#include <iostream>
using namespace std;


// Constants
//------------------------------------------------------------------------------
enum {
  ee,
  mm,
  em,
  ll,
  nchannel  // This line should be always last
};

const TString schannel[nchannel] = {
  "ee",
  "mm",
  "em",
  "ll"
};



// Data members
//------------------------------------------------------------------------------
TH1D*        h1_data        [ll];  // ee, mm, em
TH1D*        h1_f           [ll];
TH1D*        h1_wz          [ll];  // ee, mm, em
TH1D*        h1_zz          [ll];  // ee, mm, em
TH1D*        h1_tt          [ll];
TH1D*        h1_st          [ll];
TH1D*        h1_ww          [ll];
TH1D*        h1_dy          [ll];
TH1D*        h1_wjets       [ll];
TH1D*        h1_ttv         [ll];
TH1D*        h1_hww         [ll];
TH1D*        h1_wg          [ll];
TH1D*        h1_zg          [ll];
TH1D*        h1_vvv         [ll];
TH1D*        h1_hz          [ll];
TH1D*        h1_wgs         [ll];
TH1D*  h1_tt_mc_signallevel [ll];

double       data_yield     [ll];
double       data_yield_err [ll];
double       mc_yield       [ll];
double       mc_yield_err   [ll];
double       SF             [ll];
double       SF_err         [ll];
double    tt_mc_signallevel[ll];
double    tt_mc_signallevel_err[ll];
double       top_bkg        [ll];
double       top_bkg_err    [ll];



// Functions
//------------------------------------------------------------------------------

void GetTopbkg  (TString analysis = "DY", TString Top_control_level = "09_TopControl/2jet", TString WW_signal_level = "08_ZVeto/2jet", double lumi_fb = 12.9) {

  //Cuidado: los MC estan estalados a lumi_fb, pero los fakes y los datos no   
   
  TFile* file_data = new TFile("../rootfiles/nominal/" + analysis + "/01_Data.root",     "read");
  TFile* file_f    = new TFile("../rootfiles/nominal/" + analysis + "/00_Fakes.root",     "read");
  TFile* file_wz   = new TFile("../rootfiles/nominal/" + analysis + "/02_WZTo3LNu.root", "read");
  TFile* file_zz   = new TFile("../rootfiles/nominal/" + analysis + "/03_VZ.root",       "read");
  TFile* file_tt   = new TFile("../rootfiles/nominal/" + analysis + "/04_TTTo2L2Nu.root","read");
  TFile* file_st   = new TFile("../rootfiles/nominal/" + analysis + "/05_ST.root",       "read");
  TFile* file_ww   = new TFile("../rootfiles/nominal/" + analysis + "/06_WW.root",       "read");
  TFile* file_dy   = new TFile("../rootfiles/nominal/" + analysis + "/07_ZJets.root",    "read");
  TFile* file_wjets= new TFile("../rootfiles/nominal/" + analysis + "/08_WJets.root",    "read");
  TFile* file_ttv  = new TFile("../rootfiles/nominal/" + analysis + "/09_TTV.root",      "read");
  TFile* file_hww  = new TFile("../rootfiles/nominal/" + analysis + "/10_HWW.root",      "read");
  TFile* file_wg   = new TFile("../rootfiles/nominal/" + analysis + "/11_Wg.root",       "read");
  TFile* file_zg   = new TFile("../rootfiles/nominal/" + analysis + "/12_Zg.root",       "read");
  TFile* file_vvv  = new TFile("../rootfiles/nominal/" + analysis + "/13_VVV.root",      "read");
  TFile* file_hz   = new TFile("../rootfiles/nominal/" + analysis + "/14_HZ.root",       "read");
  TFile* file_wgs  = new TFile("../rootfiles/nominal/" + analysis + "/15_WgStar.root",   "read");

  
 for (int i=ee; i<ll; i++)
    {
      h1_data[i] = (TH1D*)file_data->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_f   [i] = (TH1D*)file_f   ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wz  [i] = (TH1D*)file_wz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_zz  [i] = (TH1D*)file_zz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_tt  [i] = (TH1D*)file_tt  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_st  [i] = (TH1D*)file_st  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_ww  [i] = (TH1D*)file_ww  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_dy  [i] = (TH1D*)file_dy  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wjets[i]= (TH1D*)file_wjets->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);  
      h1_ttv [i] = (TH1D*)file_ttv ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_hww [i] = (TH1D*)file_hww ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wg  [i] = (TH1D*)file_wg  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_zg  [i] = (TH1D*)file_zg  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_vvv [i] = (TH1D*)file_vvv ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_hz  [i] = (TH1D*)file_hz  ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);
      h1_wgs [i] = (TH1D*)file_wgs ->Get(analysis + "/" + Top_control_level + "/h_counterLum_" + schannel[i]);


      data_yield [i] = h1_data[i] -> GetBinContent(2);

      data_yield_err [i] = h1_data[i] ->   GetBinError(2);

      mc_yield [i] = h1_f[i] -> GetBinContent(2) + (h1_wz[i] -> GetBinContent(2) + h1_zz[i] -> GetBinContent(2) + h1_tt[i] -> GetBinContent(2)
                  + h1_st[i] -> GetBinContent(2) + h1_ww[i] -> GetBinContent(2) + h1_dy[i] -> GetBinContent(2) + h1_wjets[i] -> GetBinContent(2)
                  + h1_ttv[i] -> GetBinContent(2) + h1_hww[i] -> GetBinContent(2) + h1_wg[i] -> GetBinContent(2) + h1_zg[i] -> GetBinContent(2)
		  + h1_vvv[i] -> GetBinContent(2) + h1_hz[i] -> GetBinContent(2) + h1_wgs[i] -> GetBinContent(2))*lumi_fb;
      
      mc_yield_err [i] = sqrt((h1_f[i] -> GetBinError(2)) * (h1_f[i] -> GetBinError(2)) + ((h1_wz[i] -> GetBinError(2)) * (h1_wz[i] -> GetBinError(2)) 
                              + (h1_zz[i] -> GetBinError(2))*(h1_zz[i] -> GetBinError(2)) + (h1_tt[i] -> GetBinError(2))*(h1_tt[i] -> GetBinError(2))
                              + (h1_st[i] -> GetBinError(2))*(h1_st[i] -> GetBinError(2)) + (h1_ww[i] -> GetBinError(2))*(h1_ww[i] -> GetBinError(2))
                              + (h1_dy[i] -> GetBinError(2))*(h1_dy[i] -> GetBinError(2)) + (h1_wjets[i] -> GetBinError(2))*(h1_wjets[i] -> GetBinError(2))
                              + (h1_ttv[i] -> GetBinError(2))*(h1_ttv[i] -> GetBinError(2)) + (h1_hww[i] -> GetBinError(2))*(h1_hww[i] -> GetBinError(2))
                              + (h1_wg[i] -> GetBinError(2))*(h1_wg[i] -> GetBinError(2)) + (h1_zg[i] -> GetBinError(2))*(h1_zg[i] -> GetBinError(2)) 
                              + (h1_vvv[i] -> GetBinError(2))*(h1_vvv[i] -> GetBinError(2)) + (h1_hz[i] -> GetBinError(2))*(h1_hz[i] -> GetBinError(2))
			      + (h1_wgs[i] -> GetBinError(2))*(h1_wgs[i] -> GetBinError(2)))*lumi_fb*lumi_fb);



       SF [i] = data_yield[i] / mc_yield[i];
       SF_err [i] = sqrt((data_yield_err[i]/mc_yield[i])*data_yield_err[i]/mc_yield[i]) + (data_yield[i]/(mc_yield[i]*mc_yield[i])*mc_yield_err[i])
	          *(data_yield[i]/(mc_yield[i]*mc_yield[i])*mc_yield_err[i]);


       cout << "Canal " << schannel[i] << ": SF = " << SF[i] << "+-" << SF_err[i] << endl;


       // Top bkg stimation

       
       h1_tt_mc_signallevel [i]  = (TH1D*)file_tt  ->Get(analysis + "/" + WW_signal_level + "/h_counterLum_" + schannel[i]);
       tt_mc_signallevel [i]    = (h1_tt_mc_signallevel[i] -> GetBinContent(2))*lumi_fb;
       tt_mc_signallevel_err [i] = (h1_tt_mc_signallevel[i] -> GetBinError(2))*lumi_fb;

       top_bkg [i]     = tt_mc_signallevel[i] * SF[i];
       top_bkg_err [i] = sqrt(SF[i]*SF[i]*tt_mc_signallevel_err[i]*tt_mc_signallevel_err[i] + tt_mc_signallevel[i]*tt_mc_signallevel[i]*SF_err[i]*SF_err[i]);
       
       cout << "Canal " << schannel[i] << ": Top bkg = " << top_bkg[i] << "+-" << top_bkg_err[i] << endl;

}




}






