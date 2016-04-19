#include "TH1F.h"

using namespace std;

void dy_stimation() {

 // ZJets
 //-----------
  TFile *dy = new TFile("rootfiles/Top/06_ZJets.root","read");
  TH1F* dy_m2l = (TH1F*)dy->Get("Top/00_Has2Leptons/h_m2l_ee");

  double dy_out_l = dy_m2l -> Integral ( int (dy_m2l -> FindBin(50)), int (dy_m2l -> FindBin(75)));
  double dy_out_r = dy_m2l -> Integral ( int (dy_m2l -> FindBin(107)), int (dy_m2l -> FindBin(3000)));
 
  double dy_Nout = dy_out_l + dy_out_r;
  double dy_Nin = dy_m2l -> Integral ( int (dy_m2l -> FindBin(76)), int (dy_m2l -> FindBin(106)));
  double dy_excluded = dy_m2l -> Integral ( int (dy_m2l -> FindBin(0)), int (dy_m2l -> FindBin(49)) );

  //printf("%f, %f", _mc_Nout,_mc_Nin);

  std::cout << "_dy_Nout:" << dy_Nout << "\t _dy_Nin: " << dy_Nin << "\t dy_excluded:" << dy_excluded << "\n" << std::endl;

 //Data
 //------------------
  TFile *data = new TFile("rootfiles/Top/01_Data.root","read");
  TH1F* dat_m2l = (TH1F*)data->Get("Top/00_Has2Leptons/h_m2l_ee");

  double dat_Nin = dat_m2l -> Integral ( int (dat_m2l -> FindBin(76)), int (dat_m2l -> FindBin(106)));
  double R_outin = (dy_Nout / dy_Nin );
  double dat_Nout = dat_Nin * R_outin;
 
  cout << "data_Nout:"<< dat_Nout << "\t data_Nin:" << dat_Nin << "\n";

 //Peaking Backgrounds
 //-------------------
 
 //ZZ
 //--
  TFile *zz = new TFile("rootfiles/Top/03_ZZ.root","read");
  TH1F* zz_m2l_ee = (TH1F*)zz->Get("Top/00_Has2Leptons/h_m2l_ee");
  TH1F* zz_m2l_mm = (TH1F*)zz->Get("Top/00_Has2Leptons/h_m2l_mm");

  double zz_Nin_ee = zz_m2l_ee -> Integral ( int (zz_m2l_ee -> FindBin(76)), int (zz_m2l_ee -> FindBin(106)));
  double zz_Nin_mm = zz_m2l_mm -> Integral ( int (zz_m2l_mm -> FindBin(76)), int (zz_m2l_mm -> FindBin(106)));

 
  //WZ
  //--
  TFile *wz = new TFile("rootfiles/Top/02_WZTo3LNu.root","read");
  TH1F* wz_m2l_ee = (TH1F*)wz->Get("Top/00_Has2Leptons/h_m2l_ee");
  TH1F* wz_m2l_mm = (TH1F*)wz->Get("Top/00_Has2Leptons/h_m2l_mm");

  double wz_Nin_ee = wz_m2l_ee -> Integral ( int (wz_m2l_ee -> FindBin(76)), int (wz_m2l_ee -> FindBin(106)));
  double wz_Nin_mm = wz_m2l_mm -> Integral ( int (wz_m2l_mm -> FindBin(76)), int (wz_m2l_mm -> FindBin(106)));

  
 // Total N in 

 double PeakBg_Nin_ee = (wz_Nin_ee + zz_Nin_ee);
 double PeakBg_Nin_mm = (wz_Nin_mm + zz_Nin_mm); 
  
 // Non Peaking Backgrounds: tw, WW, WJets,<<< ttbar semileptonic<<< 
 //-----------------------------------------------------------------
 
 // kee & kmm
 
  TFile *data_ = new TFile("rootfiles/Top/01_Data.root","read");
  TH1F* dat_m2l_ee = (TH1F*)data_ ->Get("Top/00_Has2Leptons/h_m2l_ee");
  TH1F* dat_m2l_mm = (TH1F*)data_ ->Get("Top/00_Has2Leptons/h_m2l_mm");
   
  double dat_Nin_ee = dat_m2l_ee -> Integral ( int (dat_m2l_ee -> FindBin(76)), int (dat_m2l_ee -> FindBin(106)));
  double dat_Nin_mm = dat_m2l_mm -> Integral ( int (dat_m2l_mm -> FindBin(76)), int (dat_m2l_mm -> FindBin(106)));

  double kee = 0.5*sqrt( dat_Nin_ee / dat_Nin_mm );
  double kmm = 0.5*sqrt( dat_Nin_mm / dat_Nin_ee );



 // WW
 
 TFile *ww = new TFile("rootfiles/Top/05_WW.root","read");
 TH1F* ww_m2l = (TH1F*)ww->Get("Top/00_Has2Leptons/h_m2l_em");
 
 double ww_Nin = ww_m2l -> Integral ( int (ww_m2l -> FindBin(76)), int (ww_m2l -> FindBin(106))); 
 
 // tw

 TFile *tw = new TFile("rootfiles/Top/04_ST_tw.root","read");
 TH1F* tw_m2l = (TH1F*)tw->Get("Top/00_Has2Leptons/h_m2l_em"); 

 double tw_Nin = tw_m2l -> Integral ( int (tw_m2l -> FindBin(76)), int (tw_m2l -> FindBin(106)));

 // wjets
 
 TFile *wjets = new TFile("rootfiles/Top/07_WJets.root","read");
 TH1F* wjets_m2l = (TH1F*)wjets->Get("Top/00_Has2Leptons/h_m2l_em");

 double wjets_Nin = wjets_m2l -> Integral ( int (wjets_m2l -> FindBin(76)), int (wjets_m2l -> FindBin(106)));

 // Total N in 
 
 double NonPeakBg_Nin_EM_ee = (ww_Nin + tw_Nin + wjets_Nin)*kee;
 double NonPeakBg_Nin_EM_mm = (ww_Nin + tw_Nin + wjets_Nin)*kmm;

 // N out l+ l- obvs
 // ----------------
 
 double obv_Nout_ee = R_outin * ( PeakBg_Nin_ee + NonPeakBg_Nin_EM_ee );
 double obv_Nout_mm = R_outin * ( PeakBg_Nin_mm + NonPeakBg_Nin_EM_mm );

}
