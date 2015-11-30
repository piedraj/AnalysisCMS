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

const char jet[4] = "0123";
TString Channels[3] = {"EE","MuMu","SF"};

//launch the code for all channels, all jetbins
void DY(){

  for (int pp = 0; pp < 3; ++pp)
    for (int qq = 0; qq < 4; ++qq){
      doDY(jet[qq], Channels[pp]);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// DY
//
// channel = SF, MuMu, EE
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void doDY(TString jetbin, //={"0jet/","1jet/","2jet/",""}
	  TString channel,
	  TString ID = "MediumIDTighterIP",
	  TString bunch = "50ns",
	  Bool_t  useDataDriven = true,
	  Int_t   printLevel = 100,
	  Bool_t  drawR = true)
{
  cout<<"nJet = "<<jetbin<<endl;
  cout<<"Channel = "<<channel<<endl;

  Double_t yield;                                                                                                                      
  Double_t statError;
  Double_t systError;                                                                                                                    
  Double_t scaleFactor;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Input files
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TString path   = "../rootFiles/25ns/";
  //TString pathOF = "../rootFiles/OF/" + ID + "/" + bunch + "/"; 

  TFile* inputDY   = new TFile(path + "07_ZJets.root");
  TFile* inputWZ   = new TFile(path + "02_WZ.root");
  TFile* inputZZ   = new TFile(path + "03_ZZ.root");
  TFile* inputData = new TFile(path + "01_Data.root");
  //TFile* inputDataOF = new TFile(pathOF + "Data2015.root");

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Input histograms
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /*
  TH1F* hNinDYSee  [numberMetCuts];
  TH1F* hNinVVSmm  [numberMetCuts];
  TH1F* hNinVVSem  [numberMetCuts];
  TH1F* hNinDataee[numberMetCuts];
  TH1F* hNinDatamm[numberMetCuts];
  TH1F* hNinDataem[numberMetCuts];

  TH1F* hNoutDYee  [numberMetCuts];
  TH1F* hNoutVVmm  [numberMetCuts];
  TH1F* hNoutVVem  [numberMetCuts];
  TH1F* hNoutDataee[numberMetCuts];
  TH1F* hNoutDatamm[numberMetCuts];
  TH1F* hNoutDataem[numberMetCuts];
  */
  /*
  for (UInt_t nC=0; nC<numberMetCuts; nC++) {
    hNinDYSF  [nC] = (TH1F*)inputDYSF  ->Get(Form("hNinZevents%.i%.c", MetCuts[nC],jetbin));
    cout<<Form("hNinZevents%.i%.c", MetCuts[nC],jetbin)<<endl;
    hNinVVSF  [nC] = (TH1F*)inputVVSF  ->Get(Form("hNinZevents%.i%.c", MetCuts[nC],jetbin));
    hNinDataSF[nC] = (TH1F*)inputDataSF->Get(Form("hNinZevents%.i%.c", MetCuts[nC],jetbin));
    hNinDataOF[nC] = (TH1F*)inputDataOF->Get(Form("hNinZevents%.i%.c", MetCuts[nC],jetbin));

    hNoutDYSF  [nC] = (TH1F*)inputDYSF  ->Get(Form("hNoutZevents%.i%.c", MetCuts[nC],jetbin));     
    hNoutVVSF  [nC] = (TH1F*)inputVVSF  ->Get(Form("hNoutZevents%.i%.c", MetCuts[nC],jetbin));     
    hNoutDataSF[nC] = (TH1F*)inputDataSF->Get(Form("hNoutZevents%.i%.c", MetCuts[nC],jetbin));
    hNoutDataOF[nC] = (TH1F*)inputDataOF->Get(Form("hNoutZevents%.i%.c", MetCuts[nC],jetbin));
  }
  */

  // Histogram at analysis level
  //----------------------------------------
  //TH1F* hExpectedDYSF = (TH1F*)inputDYSF->Get(Form("hPtLepton1WWLevel%.c", jetbin));//("hWTopTagging");

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // k estimation
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //TFile* inputDYmumu = new TFile("../rootFiles/MuMu/" + ID + "/" + bunch + "/DY.root");
  //TFile* inputDYee   = new TFile("../rootFiles/EE/"   + ID + "/" + bunch + "/DY.root");

  //TH1F* hNinDYmumu = (TH1F*)inputDYmumu->Get(Form("hNinLooseZevents20%.c", jetbin));
  //TH1F* hNinDYee   = (TH1F*)inputDYee  ->Get(Form("hNinLooseZevents20%.c", jetbin));
  TH2D* DYee = (TH2D*)inputDY->Get("WW/11_DY/h_metvar_m2l_ee");
  TH2D* DYmm = (TH2D*)inputDY->Get("WW/11_DY/h_metvar_m2l_mm");
  TH2D* DYem = (TH2D*)inputDY->Get("WW/11_DY/h_metvar_m2l_ee");

  //Double_t NinDYmumu = hNinDYmumu -> GetBinContent(2);
  //Double_t NinDYee   = hNinDYee   -> GetBinContent(2);
  Double_t NinDYee = DYee->Integral(0,4,84,98);
  Double_t NinDYmm = DYee->Integral(0,4,84,98);
  Double_t NinDYem = DYmm->Integral(0,4,84,98);

  Double_t k    = 0.5 * (sqrt(NinDYmumu / NinDYee) + sqrt(NinDYee / NinDYmumu));
  Double_t errk = errkSF(NinDYmumu, NinDYee);

  if (channel == "MuMu") {
    k    = 0.5 * sqrt(NinDYmumu / NinDYee);
    errk = errkFunction(NinDYmumu, NinDYee);
  }
  else if (channel == "EE") {
    k    = 0.5 * sqrt(NinDYee / NinDYmumu);
    errk = errkFunction(NinDYee, NinDYmumu);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Counters
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t NinDYee  [numberMetCuts];
  Double_t NinVVmm  [numberMetCuts];
  Double_t NinVVem  [numberMetCuts];

  Double_t NinDataee[numberMetCuts];
  Double_t NinDatamm[numberMetCuts];
  Double_t NinDataem[numberMetCuts];

  Double_t NoutDYee  [numberMetCuts];
  Double_t NoutVVmm  [numberMetCuts]; 
  Double_t NoutVVem  [numberMetCuts];  // Not used for now
  Double_t NoutDataee[numberMetCuts];
  Double_t NoutDatamm[numberMetCuts];
  Double_t NoutDataem[numberMetCuts];

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {
    NinDYSF   [nC] = hNinDYSF   [nC]->GetBinContent(2);
    NinVVSF   [nC] = hNinVVSF   [nC]->GetBinContent(2);
    NinDataSF [nC] = hNinDataSF [nC]->GetBinContent(2);    
    NinDataOF [nC] = hNinDataOF [nC]->GetBinContent(2);

    NoutDYSF  [nC] = hNoutDYSF  [nC]->GetBinContent(2);
    NoutVVSF  [nC] = hNoutVVSF  [nC]->GetBinContent(2);
    NoutDataSF[nC] = hNoutDataSF[nC]->GetBinContent(2);    
    NoutDataOF[nC] = hNoutDataOF[nC]->GetBinContent(2);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // R estimation
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t R       [numberMetCuts];
  Double_t RData   [numberMetCuts];
  Double_t errR    [numberMetCuts];
  Double_t errRData[numberMetCuts];


  // Loop over the met cuts
  //----------------------------------------------------------------------------
  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    R   [nC] = NoutDYSF[nC] / NinDYSF[nC]; 
    errR[nC] = errRFunction(NoutDYSF[nC], NinDYSF[nC]);

    RData   [nC] = RDataFunction   (NoutDataSF[nC], NoutDataOF[nC], NinDataSF[nC], NinDataOF[nC], k);
    errRData[nC] = errRDataFunction(NoutDataSF[nC], NoutDataOF[nC], NinDataSF[nC], NinDataOF[nC], k, errk);

    if (printLevel > 2) {
      printf("\n %.0f < mpmet < %.0f GeV\n", MetCuts[nC], MetCuts[nC+1]);
      printf(" -------------------------------------------------\n");
      printf("         N^{MC}_{out,SF}   = %6.1f\n", NoutDYSF[nC]);
      printf("         N^{MC}_{in, SF}   = %6.1f\n", NinDYSF[nC]);
      printf("         N^{data}_{out,SF} = %4.0f\n", NoutDataSF[nC]);
      printf("         N^{data}_{out,OF} = %4.0f\n", NoutDataOF[nC]);
      printf("         N^{data}_{in, SF} = %4.0f\n", NinDataSF[nC]);
      printf("         N^{data}_{in, OF} = %4.0f\n", NinDataOF[nC]);
      printf("         k                 = % 5.3f +- %5.3f\n", k,         errk);
      printf("         R^{MC}            = % 5.3f +- %5.3f\n", R[nC],     errR[nC]);
      printf("         R^{data}          = % 5.3f +- %5.3f\n", RData[nC], errRData[nC]);
    }
  }

  // Estimate the R systematic as the difference between R[2] and R[3]
  //----------------------------------------------------------------------------
  Int_t iMaxR = 0; 
  Int_t iMinR = 0; 

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    if (R[nC] > 0 && R[nC] > R[iMaxR]) iMaxR = nC;
    if (R[nC] > 0 && R[nC] < R[iMinR]) iMinR = nC;
  }

  Int_t theR = 2;
  Int_t sysR = 3;

  Double_t RelDiffR = (R[theR] > 0) ? fabs(R[theR] - R[sysR]) / R[theR] : -999;


  if (printLevel > 0) {
    printf("\n [%s] R systematic uncertainty\n", channel.Data());
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
  Double_t NinCountedSFVV   = NinVVSF  [sysR];
  Double_t NinCountedSFData = NinDataSF[sysR];
  Double_t NinCountedOFData = NinDataOF[sysR];

  Double_t NinEstSFFinal    = NinCountedSFData - k*NinCountedOFData;
  Double_t errNinEstSFFinal = errNinEstFunction(NinCountedSFData, NinCountedOFData, k, errk);

  Double_t NestSFFinal    = R[theR] * NinEstSFFinal;
  Double_t errNestSFFinal = errNestFunction(NestSFFinal, R[theR], errR[theR], NinEstSFFinal, errNinEstSFFinal);

  Double_t NinEstSFNoDibosonFinal    = NinEstSFFinal - NinCountedSFVV;
  Double_t errNinEstSFNoDibosonFinal = errNinEstNoDibosonFunction(NinCountedSFData, k, errk, NinCountedOFData, NinCountedSFVV);

  Double_t NestSFNoDibosonFinal    = R[theR] * NinEstSFNoDibosonFinal;
  Double_t errNestSFNoDibosonFinal = errNestFunction(NestSFNoDibosonFinal, R[theR], errR[theR], NinEstSFNoDibosonFinal, errNinEstSFNoDibosonFinal);
  Double_t totalError              = sqrt(errNestSFNoDibosonFinal*errNestSFNoDibosonFinal + (RelDiffR*NestSFNoDibosonFinal)*(RelDiffR*NestSFNoDibosonFinal));


  Double_t SFsf = NestSFNoDibosonFinal / hExpectedDYSF->Integral();


  if (printLevel > 1) {
    printf("\n Analysis results\n");
    printf(" -------------------------------------------------\n");
    printf("         N^{data}_{in,SF} = %4.0f\n", NinCountedSFData);
    printf("         N^{data}_{in,OF} = %4.0f\n", NinCountedOFData);
    printf("         k                = %5.3f +- %5.3f\n", k, errk);
    printf("         R[%d]             = %5.3f +- %5.3f\n", theR, R[theR], errR[theR]);
    printf("         N^{VV}_{in,SF}   = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NinCountedSFVV, sqrt(NinCountedSFVV), 0.1*NinCountedSFVV);
    printf("         N^{est}_{in, SF} = %7.2f +- %6.2f\n", NinEstSFFinal, errNinEstSFFinal);
    printf("         N^{est}_{out,SF} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NestSFFinal, errNestSFFinal, RelDiffR*NestSFFinal);
    printf(" -------------------------------------------------\n");
    printf(" [no VZ] N^{est}_{out,SF} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.) = %7.2f +- %6.2f (stat. + syst.)\n",
	   NestSFNoDibosonFinal, errNestSFNoDibosonFinal, RelDiffR*NestSFNoDibosonFinal,
	   NestSFNoDibosonFinal, totalError);
    printf("         N^{MC}_{out,SF}  = %7.2f +- %6.2f\n",
	   hExpectedDYSF->Integral(), sqrt(hExpectedDYSF->Integral()));
    printf("     *** scale factor     = %.3f\n\n", SFsf);
  }


  // Save the result
  //----------------------------------------------------------------------------
  yield       = (useDataDriven) ? NestSFNoDibosonFinal : hExpectedDYSF->Integral();
  statError   = errNestSFNoDibosonFinal;
  systError   = RelDiffR*NestSFNoDibosonFinal;
  scaleFactor = yield / hExpectedDYSF->Integral();


  // For the note
  //----------------------------------------------------------------------------
  if (printLevel > 0) {
    printf("\n [%s] DY values for the note\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf(" final state   &             R_{MC}  &  N^{control,data}  &     N_{DY}^{data}  &       N_{DY}^{MC}  &  data/MC\n");
    printf(" same flavour  &  %5.3f $\\pm$ %5.3f  &              %4.0f  &  %5.1f $\\pm$ %4.1f  &  %5.1f $\\pm$ %4.1f  &     %4.1f\n\n",
	   R[theR],
	   errR[theR],
	   NinCountedSFData,
	   yield,
	   statError,
	   hExpectedDYSF->Integral(),
	   sqrt(hExpectedDYSF->Integral()),
	   scaleFactor);
    printf("\n [%s] DY relative systematic uncertainties\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf(" DY normalisation = %.0f (stat.) $\\bigoplus$ %.0f (syst.)\n\n",
	   1e2*statError/yield, 1e2*systError/yield);
  }


  // Check
  //----------------------------------------------------------------------------
  Double_t check = hExpectedDYSF->Integral() - NoutDYSF[sysR];
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

    gRdata->SetLineColor  (kRed+1);
    gRdata->SetMarkerColor(kRed+1);
    gRdata->SetMarkerSize (0.9);
    gRdata->SetMarkerStyle(kFullCircle);


    // Draw
    //--------------------------------------------------------------------------
    canvas = new TCanvas();

    TMultiGraph *mgR = new TMultiGraph();
    mgR->Add(gRdata);
    mgR->Add(gR);

    mgR->Draw("ap");

    mgR->GetYaxis()->SetTitle("R^{out/in}");
    mgR->GetXaxis()->SetTitle("mpmet (GeV)");

    mgR->SetMinimum(absoluteMin - 0.1);
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

    if      (channel == "SF")   lmgR->SetHeader("ee + #mu#mu");
    else if (channel == "EE")   lmgR->SetHeader("ee");
    else if (channel == "MuMu") lmgR->SetHeader("#mu#mu");

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

    if (jetbin == '3'){
      DrawTLatex(0.725, 0.65, 0.04,  "Inclusive");
      canvas->SaveAs("R_" + channel + "_Inclusive.png");
      canvas->SaveAs("R_" + channel + "_Inclusive.pdf");
    }
    else{                                                   //Inclusive 
      if      (jetbin == '0') DrawTLatex(0.725, 0.65, 0.04,  "0 Jet    ");
      else if (jetbin == '1') DrawTLatex(0.725, 0.65, 0.04,  "1 Jet    ");
      else if (jetbin == '2') DrawTLatex(0.725, 0.65, 0.04,  "2+ Jets  ");
      canvas->SaveAs("R_" + channel + "_" + jetbin + "jet.png");
      canvas->SaveAs("R_" + channel + "_" + jetbin + "jet.pdf");
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
