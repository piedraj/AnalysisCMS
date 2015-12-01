//root -l -b -q DY.C

#include "TCanvas.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TLatex.h"
#include "TH1D.h"
#include "TH2D.h"

const UInt_t numberMetCuts = 5;

Int_t MetCuts[numberMetCuts] = {20, 25, 30, 45, 1000};
Int_t MetDraw[numberMetCuts] = {20, 25, 30, 45,   75};

TCanvas* canvas = NULL;

// Member functions
//------------------------------------------------------------------------------
Double_t errkFunction              (Double_t a,
				    Double_t b);

Double_t errkSF                    (Double_t a,
				    Double_t b);

Double_t errRFunction              (Double_t Nout,
				    Double_t Nin);

Double_t RDataFunction             (Double_t NoutSF,
				    Double_t NoutOF,
				    Double_t NinSF,
				    Double_t NinOF,
				    Double_t k);

Double_t errRDataFunction          (Double_t NoutSF,
				    Double_t NoutOF,
				    Double_t NinSF,
				    Double_t NinOF,
				    Double_t k,
				    Double_t errk);

Double_t errNinEstFunction         (Double_t NinDataSF,
				    Double_t NinDataOF,
				    Double_t k,
				    Double_t errk);

Double_t errNestFunction           (Double_t Nest,
				    Double_t R,
				    Double_t errR,
				    Double_t Ninest,
				    Double_t errNinest);

Double_t errNinEstNoDibosonFunction(Double_t NinDataSF,
				    Double_t k,
				    Double_t errk,
				    Double_t NinDataOF,
				    Double_t NinVV);

void DrawTLatex                    (Double_t x, 
				    Double_t y, 
				    Double_t tsize, 
				    const char* text);

void doDY                          (Int_t   njet,
				    Bool_t  useDataDriven,
				    Int_t   printLevel,
				    Bool_t  drawR);

void DY(){
  
    for (Int_t qq = 0; qq < 4; ++qq){
      doDY(qq, true, 100, true);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// DY
//
// channel = ee,mm,em,ll
//
// jetbin = "0jet","1jet","2jet","" (let's start with inclusive jetbin only)
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void doDY(Int_t   njet,
	  Bool_t  useDataDriven = true,
	  Int_t   printLevel = 100,
	  Bool_t  drawR = true){

  Double_t yield;                                                                                                                      
  Double_t statError;
  Double_t systError;                                                                                                                    
  Double_t scaleFactor;

  //jetbin
  const int nJetbin = 4;

  TString jetbin[nJetbin];
  jetbin[0] = "0jet";
  jetbin[1] = "1jet";
  jetbin[2] = "2jet";
  jetbin[3] = "";

  //lepton channels
  const int nChannel = 4;
  enum {iee,imm,iem,ill};

  TString channel[nChannel];
  channel[iee] = "ee";
  channel[imm] = "mm";
  channel[iem] = "em";
  channel[ill] = "ll";

  // Input files and maps
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TString path   = "../rootfiles/25ns/";

  TFile* inputDY   = new TFile(path + "07_ZJets.root");
  TFile* inputWZ   = new TFile(path + "02_WZ.root");
  TFile* inputZZ   = new TFile(path + "03_ZZ.root");
  TFile* inputData = new TFile(path + "01_Data.root");

  TH2D* mDY  [nChannel];
  TH2D* mWZ  [nChannel];
  TH2D* mZZ  [nChannel];
  TH2D* mData[nChannel];
  TH1F* hExpectedDY[nChannel];

  for (UInt_t nC = 0; nC < nChannel; ++nC){
    mDY[nC]   = (TH2D*)inputDY  ->Get("WW/11_DY/h_metvar_m2l_" + channel[nC]);
    mWZ[nC]   = (TH2D*)inputWZ  ->Get("WW/11_DY/h_metvar_m2l_" + channel[nC]);
    mZZ[nC]   = (TH2D*)inputZZ  ->Get("WW/11_DY/h_metvar_m2l_" + channel[nC]);
    mData[nC] = (TH2D*)inputData->Get("WW/11_DY/h_metvar_m2l_" + channel[nC]);
    hExpectedDY[nC] = (TH1F*)inputDY -> Get("WW/10_Ht/h_pt1_"  + channel[nC]);
  }


  // k estimation
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t NinDYk  [nChannel]; 
  Double_t NinVVk  [nChannel]; 
  Double_t NinDatak[nChannel]; 

  for (UInt_t nC = 0; nC < nChannel; ++nC){
    NinDYk[nC]   = mDY[nC]   -> Integral(0,4,84,98);
    NinVVk[nC]   = mWZ[nC]   -> Integral(0,4,84,98) + mZZ[nC] -> Integral(0,4,84,98);
    NinDatak[nC] = mData[nC] -> Integral(0,4,84,98);
  }

  Double_t k[nChannel];
  Double_t errk[nChannel];

  k[iee]    = 0.5 * sqrt (NinDYk[iee] / NinDYk[imm]);
  errk[iee] = errkFunction(NinDYk[iee], NinDYk[imm]);

  k[imm]    = 0.5 * sqrt (NinDYk[imm] / NinDYk[iee]);
  errk[imm] = errkFunction(NinDYk[imm], NinDYk[iee]);

  k[iem]    = 0;
  errk[iem] = 0;

  k[ill]    = k[iee] + k[imm];
  errk[ill] = errkSF(NinDYk[imm], NinDYk[iee]);

  // Counters
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t NinDY   [numberMetCuts][nChannel]; //ee,mm,em,ll
  Double_t NinVV   [numberMetCuts][nChannel];
  Double_t NinData [numberMetCuts][nChannel];

  Double_t NoutDY   [numberMetCuts][nChannel];
  Double_t NoutVV   [numberMetCuts][nChannel]; 
  Double_t NoutData [numberMetCuts][nChannel];

  Double_t NpeakDY   [numberMetCuts][nChannel];
  Double_t NpeakVV   [numberMetCuts][nChannel]; 
  Double_t NpeakData [numberMetCuts][nChannel];

  for (UInt_t nM=0; nM<numberMetCuts-1; nM++) {
    for (UInt_t nC=0; nC<nChannel; nC++) {
      NinDY    [nM][nC] = mDY[nC]   -> Integral(nM,nM+1,84,98);
      NinVV    [nM][nC] = mWZ[nC]   -> Integral(nM,nM+1,84,98) + mZZ[nC] -> Integral(nM,nM+1,84,98);
      NinData  [nM][nC] = mData[nC] -> Integral(nM,nM+1,84,98);

      NpeakDY    [nM][nC] = mDY[nC]   -> Integral(nM,nM+1,76,106);
      NpeakVV    [nM][nC] = mWZ[nC]   -> Integral(nM,nM+1,76,106) + mZZ[nC] -> Integral(nM,nM+1,76,106);
      NpeakData  [nM][nC] = mData[nC] -> Integral(nM,nM+1,76,106);

      NoutDY   [nM][nC] = mDY[nC]   -> Integral(nM,nM+1,0,200) - NpeakDY[nM][nC];
      NoutVV   [nM][nC] = mWZ[nC]   -> Integral(nM,nM+1,0,200) + mZZ[nC] -> Integral(nM,nM+1,0,200) - NpeakVV[nM][nC];
      NoutData [nM][nC] = mData[nC] -> Integral(nM,nM+1,0,200) - NpeakData[nM][nC];
    }
  }

  // R estimation
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t R       [numberMetCuts];
  Double_t RData   [numberMetCuts];
  Double_t errR    [numberMetCuts];
  Double_t errRData[numberMetCuts];
  
  // Loop over the SF channels - ee, mm, ll (for now just look 2 rows lower: nC = ill)
  //----------------------------------------------------------------------------
  //  UInt_t nC = ill{
  for (UInt_t nC = 0; nC < nChannel; ++nC){
    // Loop over the met cuts
    //----------------------------------------------------------------------------
    for (UInt_t nM=0; nM<numberMetCuts-1; nM++) {
      
      if (nC == iem) continue;
      
      R   [nM] = NoutDY[nM][nC] / NinDY[nM][nC]; 
      errR[nM] = errRFunction(NoutDY[nM][nC], NinDY[nM][nC]);
      
      RData   [nM] = RDataFunction   (NoutData[nM][nC], NoutData[nM][iem], NinData[nM][nC], NinData[nM][iem], k[nC]);
      errRData[nM] = errRDataFunction(NoutData[nM][nC], NoutData[nM][iem], NinData[nM][nC], NinData[nM][iem], k[nC], errk[nC]);
      
      if (printLevel > 2) {
	printf("\n %.0d < mpmet < %.0d GeV\n", MetCuts[nM], MetCuts[nM+1]);
	printf(" -------------------------------------------------\n");
	printf("         N^{MC}_{out,"+channel[nC]+"}   = %6.1f\n", NoutDY[nM][nC]);
	printf("         N^{MC}_{in, "+channel[nC]+"}   = %6.1f\n", NinDY[nM][nC]);
	printf("         N^{data}_{out,"+channel[nC]+"} = %4.0f\n", NoutData[nM][nC]);
	printf("         N^{data}_{out,em} = %4.0f\n", NoutData[nM][iem]);
	printf("         N^{data}_{in, "+channel[nC]+"} = %4.0f\n", NinData[nM][nC]);
	printf("         N^{data}_{in, em} = %4.0f\n", NinData[nM][iem]);
	printf("         k                 = % 5.3f +- %5.3f\n", k[nC],         errk[nC]);
	printf("         R^{MC}            = % 5.3f +- %5.3f\n", R[nM],     errR[nM]);
	printf("         R^{data}          = % 5.3f +- %5.3f\n", RData[nM], errRData[nM]);
      }
    }
    
    // Estimate the R systematic as the difference between R[2] and R[3]
    //----------------------------------------------------------------------------
    Int_t iMaxR = 0; 
    Int_t iMinR = 0; 
    
    for (UInt_t nM=0; nM<numberMetCuts-1; nM++) {
      
      cout<<"hola"<<nM<<"!!"<<endl;
      if (R[nM] > 0 && R[nM] > R[iMaxR]) iMaxR = nM;
      if (R[nM] > 0 && R[nM] < R[iMinR]) iMinR = nM;
    }
    
    Int_t theR = 2;
    Int_t sysR = 3;
    
    Double_t RelDiffR = (R[theR] > 0) ? fabs(R[theR] - R[sysR]) / R[theR] : -999;
    
    cout<<"aaaaaaaaaaaaaaaaaa"<<endl;
    
    if (printLevel > 0) {
      printf("\n [%s] R systematic uncertainty\n", channel[nC].Data());
      printf(" -------------------------------------------------\n");
      printf("         min R               = %5.3f\n",     R[iMinR]);
      printf("         max R               = %5.3f\n",     R[iMaxR]);
      printf("         R[%d]               = %5.3f\n",    theR, R[theR]);
      printf("         R[%d]               = %5.3f\n",    sysR, R[sysR]);
      printf("         |R[%d]-R[%d]| / R[%d] = %.1f%s\n", theR, sysR, theR, 1e2*RelDiffR, "%");
      printf("\n");
    }
    
    // Estimate Nout
    //----------------------------------------------------------------------------
    Double_t NinCountedSFVV   = NinVV  [sysR][nC];
    Double_t NinCountedSFData = NinData[sysR][nC];
    Double_t NinCountedOFData = NinData[sysR][iem];
    
    Double_t NinEstSFFinal    = NinCountedSFData - k[nC]*NinCountedOFData;
    Double_t errNinEstSFFinal = errNinEstFunction(NinCountedSFData, NinCountedOFData, k[nC], errk[nC]);
    
    Double_t NestSFFinal    = R[theR] * NinEstSFFinal;
    Double_t errNestSFFinal = errNestFunction(NestSFFinal, R[theR], errR[theR], NinEstSFFinal, errNinEstSFFinal);
    
    Double_t NinEstSFNoDibosonFinal    = NinEstSFFinal - NinCountedSFVV;
    Double_t errNinEstSFNoDibosonFinal = errNinEstNoDibosonFunction(NinCountedSFData, k[nC], errk[nC], NinCountedOFData, NinCountedSFVV);
    
    Double_t NestSFNoDibosonFinal    = R[theR] * NinEstSFNoDibosonFinal;
    Double_t errNestSFNoDibosonFinal = errNestFunction(NestSFNoDibosonFinal, R[theR], errR[theR], NinEstSFNoDibosonFinal, errNinEstSFNoDibosonFinal);
    Double_t totalError              = sqrt(errNestSFNoDibosonFinal*errNestSFNoDibosonFinal + (RelDiffR*NestSFNoDibosonFinal)*(RelDiffR*NestSFNoDibosonFinal));
    
    
    Double_t SFsf = NestSFNoDibosonFinal / hExpectedDY[nC]->Integral();
    
    if (printLevel > 1) {
      printf("\n Analysis results\n");
      printf(" -------------------------------------------------\n");
      printf("         N^{data}_{in,"+channel[nC]+"} = %4.0f\n", NinCountedSFData);
      printf("         N^{data}_{in,em} = %4.0f\n", NinCountedOFData);
      printf("         k                = %5.3f +- %5.3f\n", k[nC], errk[nC]);
      printf("         R[%d]             = %5.3f +- %5.3f\n", theR, R[theR], errR[theR]);
      printf("         N^{VV}_{in,"+channel[nC]+"}   = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	     NinCountedSFVV, sqrt(NinCountedSFVV), 0.1*NinCountedSFVV);
      printf("         N^{est}_{in,"+channel[nC]+"} = %7.2f +- %6.2f\n", NinEstSFFinal, errNinEstSFFinal);
      printf("         N^{est}_{out,"+channel[nC]+"} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	     NestSFFinal, errNestSFFinal, RelDiffR*NestSFFinal);
      printf(" -------------------------------------------------\n");
      printf(" [no VZ] N^{est}_{out,SF} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.) = %7.2f +- %6.2f (stat. + syst.)\n",
	     NestSFNoDibosonFinal, errNestSFNoDibosonFinal, RelDiffR*NestSFNoDibosonFinal,
	     NestSFNoDibosonFinal, totalError);
      printf("         N^{MC}_{out,"+channel[nC]+"}  = %7.2f +- %6.2f\n",
	     hExpectedDY[nC]->Integral(), sqrt(hExpectedDY[nC]->Integral()));
      printf("     *** scale factor     = %.3f\n\n", SFsf);
    }
    
    // Save the result
    //----------------------------------------------------------------------------
    yield       = (useDataDriven) ? NestSFNoDibosonFinal : hExpectedDY[nC]->Integral();
    statError   = errNestSFNoDibosonFinal;
    systError   = RelDiffR*NestSFNoDibosonFinal;
    scaleFactor = yield / hExpectedDY[nC]->Integral();
    
    // For the note
    //----------------------------------------------------------------------------
    if (printLevel > 0) {
      printf("\n [%s] DY values for the note\n", channel[nC].Data());
      printf(" -------------------------------------------------\n");
      printf(" final state   &             R_{MC}  &  N^{control,data}  &     N_{DY}^{data}  &       N_{DY}^{MC}  &  data/MC\n");
      printf(" same flavour  &  %5.3f $\\pm$ %5.3f  &              %4.0f  &  %5.1f $\\pm$ %4.1f  &  %5.1f $\\pm$ %4.1f  &     %4.1f\n\n",
	     R[theR],
	     errR[theR],
	     NinCountedSFData,
	     yield,
	     statError,
	     hExpectedDY[nC]->Integral(),
	     sqrt(hExpectedDY[nC]->Integral()),
	     scaleFactor);
      printf("\n [%s] DY relative systematic uncertainties\n", channel[nC].Data());
      printf(" -------------------------------------------------\n");
      printf(" DY normalisation = %.0f (stat.) $\\bigoplus$ %.0f (syst.)\n\n",
	     1e2*statError/yield, 1e2*systError/yield);
    }
    
    // Check
    //----------------------------------------------------------------------------
    Double_t check = hExpectedDY[nC]->Integral() - NoutDY[sysR][nC];
    if (check != 0) printf(" WARNING: DY yields do not much by %f\n\n", check);
    
    // Draw histograms
    //----------------------------------------------------------------------------
    if (drawR) {
      
      Double_t absoluteMin = 999;
      
      TGraphErrors* gR     = new TGraphErrors(numberMetCuts-1);
      TGraphErrors* gRdata = new TGraphErrors(numberMetCuts-1);
      
      for (UInt_t i=0; i<numberMetCuts-1; i++) {
	
	gR->SetPoint     (i, 0.5 * (MetDraw[i+1] + MetDraw[i]),    R[i]);
	gR->SetPointError(i, 0.5 * (MetDraw[i+1] - MetDraw[i]), errR[i]);
	
	gRdata->SetPoint     (i, 0.5 * (MetDraw[i+1] + MetDraw[i]),    RData[i]);
	gRdata->SetPointError(i, 0.5 * (MetDraw[i+1] - MetDraw[i]), errRData[i]);
	
	if (absoluteMin > (R[i]     - errR[i]))     absoluteMin = R[i]     - errR[i];
	if (absoluteMin > (RData[i] - errRData[i])) absoluteMin = RData[i] - errRData[i];
      }
      
      if (absoluteMin > 0) absoluteMin = 0;
      
      // Cosmetics
      //--------------------------------------------------------------------------
      gR->SetMarkerSize (0.9);
      gR->SetMarkerStyle(kFullCircle);
      gR->GetYaxis()->SetRangeUser(0,1);
      
      gRdata->SetLineColor  (kRed+1);
      gRdata->SetMarkerColor(kRed+1);
      gRdata->SetMarkerSize (0.9);
      gRdata->SetMarkerStyle(kFullCircle);
      gRdata->GetYaxis()->SetRangeUser(0,1);
      
      // Draw
      //--------------------------------------------------------------------------
      canvas = new TCanvas();
      
      TMultiGraph *mgR = new TMultiGraph();
      mgR->Add(gRdata);
      mgR->Add(gR);
      
      mgR->Draw("ap");
      
      mgR->GetYaxis()->SetTitle("R^{out/in}");
      mgR->GetXaxis()->SetTitle("mpmet (GeV)");
      
      mgR->SetMinimum(0.2);//absoluteMin - 0.1);
      mgR->SetMaximum(1.0);
      
      // Legend
      //--------------------------------------------------------------------------
      TLegend* lmgR = new TLegend(0.62, 0.68, 0.86, 0.88);
      
      lmgR->AddEntry(gR,    " DY MC", "lp");
      lmgR->AddEntry(gRdata," data",  "lp");
      
      lmgR->SetFillColor(0);
      lmgR->SetLineColor(kWhite);
      lmgR->SetTextAlign(12);
      lmgR->SetTextFont (42);
      lmgR->SetTextSize (0.04);
      
      lmgR->SetHeader(channel[nC] + " channel");
      
      lmgR->Draw();
      
      // Line at zero
      //--------------------------------------------------------------------------
      TLine* zeroLine = new TLine(canvas->GetUxmin(), 0.0, canvas->GetUxmax(), 0.0);
      zeroLine->SetLineStyle(3);
      zeroLine->SetLineWidth(2);
      zeroLine->Draw("same");
      mgR->Draw("p,same");
      
      // Save
      //--------------------------------------------------------------------------
      lmgR->Draw("same");
      
      TString writeJet[nJetbin];
      writeJet[0] = "0Jet";
      writeJet[1] = "1Jet";
      writeJet[2] = "2+Jet";
      writeJet[3] = "Inclusive";
      
      DrawTLatex(0.725, 0.65, 0.04,  writeJet[njet]);
      canvas->SaveAs("R_" + channel[nC] + "_" + writeJet[njet] + ".png");
      canvas->SaveAs("R_" + channel[nC] + "_" + writeJet[njet] + ".pdf");
    }
  }
}

//------------------------------------------------------------------------------
// errkFunction
//------------------------------------------------------------------------------
Double_t errkFunction(Double_t a, Double_t b)
{
  Double_t errk = sqrt((1 + a/b) / b) / 4.0;

  return errk;
}

//------------------------------------------------------------------------------
// errkSF
//------------------------------------------------------------------------------
Double_t errkSF(Double_t a, Double_t b)
{
  Double_t errk = sqrt(b/(a*a) + a/(b*b) - 1/b - 1/a) / 4.0;

  return errk;
}


//------------------------------------------------------------------------------
// errRFunction
//------------------------------------------------------------------------------
Double_t errRFunction(Double_t Nout, Double_t Nin)
{
  Double_t R = Nout / Nin;

  Double_t errR = R * sqrt(1.0/Nout + 1.0/Nin);

  return errR;
}


//------------------------------------------------------------------------------
// RDataFunction
//------------------------------------------------------------------------------
Double_t RDataFunction(Double_t NoutSF,
		       Double_t NoutOF,
		       Double_t NinSF,
		       Double_t NinOF,
		       Double_t k)
{
  Double_t R = (NoutSF - k*NoutOF) / (NinSF - k*NinOF);

  return R;
}


//------------------------------------------------------------------------------
// errRDataFunction
//------------------------------------------------------------------------------
Double_t errRDataFunction(Double_t NoutSF,
			  Double_t NoutOF,
			  Double_t NinSF,
			  Double_t NinOF,
			  Double_t k,
			  Double_t errk)
{
  Double_t num = NoutSF - k*NoutOF;
  Double_t den = NinSF  - k*NinOF;
  
  Double_t Term1 = sqrt(NoutSF + sqrt((errk*NoutOF)*(errk*NoutOF) + k*k*NoutOF));
  Double_t Term2 = sqrt(NinSF  + sqrt((errk*NinOF) *(errk*NinOF)  + k*k*NinOF));

  Term1 = Term1 / den;
  Term2 = num * Term2 / (den*den);

  Double_t errRData = sqrt(Term1*Term1 + Term2*Term2);

  return errRData;
}

//------------------------------------------------------------------------------
// errNinEstFunction
//------------------------------------------------------------------------------
Double_t errNinEstFunction(Double_t NinDataSF,
			   Double_t NinDataOF,
			   Double_t k,
			   Double_t errk)
{
  Double_t errNinEst = NinDataSF;

  errNinEst += sqrt(k*k*NinDataOF + (NinDataOF*errk)*(NinDataOF*errk));

  errNinEst = sqrt(errNinEst);

  return errNinEst;
}


//------------------------------------------------------------------------------
// errNestFunction
//------------------------------------------------------------------------------
Double_t errNestFunction(Double_t Nest,
			 Double_t R,
			 Double_t errR,
			 Double_t Ninest,
			 Double_t errNinest)
{
  Double_t errNest = (errR*errR)/(R*R) + (errNinest*errNinest)/(Ninest*Ninest);

  errNest *= (Nest*Nest);

  errNest = sqrt(errNest);

  return errNest;
}


//------------------------------------------------------------------------------
// errNinEstNoDibosonFunction
//------------------------------------------------------------------------------
Double_t errNinEstNoDibosonFunction(Double_t NinDataSF,
				    Double_t k,
				    Double_t errk,
				    Double_t NinDataOF,
				    Double_t NinVV)
{
  Double_t errNVV = sqrt(NinVV + (0.1*NinVV)*(0.1*NinVV));

  Double_t errNinEst = NinDataSF + errNVV*errNVV;

  errNinEst += sqrt(k*k*NinDataOF + (NinDataOF*errk)*(NinDataOF*errk));

  errNinEst = sqrt(errNinEst);

  return errNinEst;
}

//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t x, Double_t y, Double_t tsize, const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(   32);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);

  tl->Draw("same");
}
