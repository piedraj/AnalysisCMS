// https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSRecommendationsMoriond17#Pileup_lumi
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"

#include "../include/CutsStop.h"

TString RootFilesDirectory = "../minitrees/rootfiles/";
TString MassPointFileName = "../../PlotsConfigurations/Configurations/T2tt/MassPointList.txt";

float FractionNvtxUp = 0.293, FractionNvtxDo = 0.707;
float AverageNvtxUp  = 23.52, AverageNvtxDo  = 13.41;

const int MaxVertex = 60;
float DataVtxFraction[MaxVertex], MonteCarloVtxFraction[MaxVertex];

bool PassRegion(TString TestRegion, TString CutRegion) {

  if (TestRegion==CutRegion) return true;

  if (CutRegion.Contains("_Tag")   && !TestRegion.Contains("_Tag")) return false;
  if (CutRegion.Contains("_NoTag") && !TestRegion.Contains("_NoTag")) return false;
  if (CutRegion.Contains("SR1")    && !TestRegion.Contains("_SR1")) return false;
  if (CutRegion.Contains("SR2")    && !TestRegion.Contains("_SR2")) return false;
  if (CutRegion.Contains("SR3")    && !TestRegion.Contains("_SR3")) return false;
  if (CutRegion.Contains("01")     && !TestRegion.Contains("01")) return false;
  if (CutRegion.Contains("02")     && !TestRegion.Contains("02")) return false;
  if (CutRegion.Contains("SR")     && !TestRegion.Contains("_SR")) return false;

  return true;

}

TH1F *NvtxUpee, *NvtxUpem, *NvtxUpmm, *NvtxUpll;
TH1F *NvtxDoee, *NvtxDoem, *NvtxDomm, *NvtxDoll;

void CompareDistributions(TString CutRegion, TString MassPoint = "_mStop-250to350_Sm275_Xm150", TString Channel = "_ll", TString Variable = "_MT2ll") {

  TString RootFileName = RootFilesDirectory + "/nominal/Stop/T2tt" + MassPoint + ".root";
  TFile *RootFile = TFile::Open(RootFileName);

  TH1F *NvtxCn, *NvtxUp, *NvtxDo;

  bool FirstRegion = true;

  for (int rg = 0; rg<ncut; rg++) {

    if (!PassRegion(scut[rg], CutRegion)) continue;

    cout << "   Merging " << scut[rg] << " region " << endl;

    if (FirstRegion) {

      NvtxCn = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + Channel);
      NvtxUp = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + "_nvtxup" + Channel);
      NvtxDo = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + "_nvtxdo" + Channel);

      FirstRegion = false;

    } else {

      TH1F *DummyCn = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + Channel);
      TH1F *DummyUp = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + "_nvtxup" + Channel);
      TH1F *DummyDo = (TH1F*) RootFile->Get(scut[rg] + "/h" + Variable + "_nvtxdo" + Channel);

      NvtxCn->Add(DummyCn);
      NvtxUp->Add(DummyUp);
      NvtxDo->Add(DummyDo);

    }

  }

  TCanvas *CC = new TCanvas("CC", "" , 600, 400);
  CC->Divide(1, 1); 

  CC->cd(1);

  float ThisIntegral = NvtxCn->Integral(0, NvtxCn->GetNbinsX()+1); NvtxCn->Scale(1./ThisIntegral);
  ThisIntegral = NvtxUp->Integral(0, NvtxUp->GetNbinsX()+1); NvtxUp->Scale(1./ThisIntegral);
  ThisIntegral = NvtxDo->Integral(0, NvtxDo->GetNbinsX()+1); NvtxDo->Scale(1./ThisIntegral);

  NvtxCn->SetXTitle(Variable);
  NvtxUp->SetLineColor(2);
  NvtxDo->SetLineColor(4);
  
  NvtxCn->Draw();
  NvtxUp->Draw("same");
  NvtxDo->Draw("same");
  
}

TFile *InputRootFile;

void GetPUStrategy0(TString TestRegion) {

  NvtxUpee = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxup_ee");
  NvtxUpem = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxup_em");
  NvtxUpmm = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxup_mm");
  NvtxUpll = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxup_ll");

  NvtxUpee->SetName("h_MT2ll_ee");
  NvtxUpem->SetName("h_MT2ll_em");
  NvtxUpmm->SetName("h_MT2ll_mm");
  NvtxUpll->SetName("h_MT2ll_ll");

  NvtxUpee->Scale(1./FractionNvtxUp);
  NvtxUpem->Scale(1./FractionNvtxUp);
  NvtxUpmm->Scale(1./FractionNvtxUp);
  NvtxUpll->Scale(1./FractionNvtxUp);

  NvtxDoee = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxdo_ee");
  NvtxDoem = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxdo_em");
  NvtxDomm = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxdo_mm");
  NvtxDoll = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll_nvtxdo_ll");

  NvtxDoee->SetName("h_MT2ll_ee");
  NvtxDoem->SetName("h_MT2ll_em");
  NvtxDomm->SetName("h_MT2ll_mm");
  NvtxDoll->SetName("h_MT2ll_ll");

  NvtxDoee->Scale(1./FractionNvtxDo);
  NvtxDoem->Scale(1./FractionNvtxDo);
  NvtxDomm->Scale(1./FractionNvtxDo);
  NvtxDoll->Scale(1./FractionNvtxDo);

}

void FillVtxFractions() {

  DataVtxFraction[0]=0; //, x=-0.5
 DataVtxFraction[1]=0; //, x=0.5
 DataVtxFraction[2]=0.000106876; //, x=1.5
 DataVtxFraction[3]=0.000447082; //, x=2.5
 DataVtxFraction[4]=0.00146929; //, x=3.5
 DataVtxFraction[5]=0.00375125; //, x=4.5
 DataVtxFraction[6]=0.00841202; //, x=5.5
 DataVtxFraction[7]=0.0154875; //, x=6.5
 DataVtxFraction[8]=0.0254371; //, x=7.5
 DataVtxFraction[9]=0.0375194; //, x=8.5
 DataVtxFraction[10]=0.0504859; //, x=9.5
 DataVtxFraction[11]=0.0632682; //, x=10.5
 DataVtxFraction[12]=0.0733204; //, x=11.5
 DataVtxFraction[13]=0.0812668; //, x=12.5
 DataVtxFraction[14]=0.085391; //, x=13.5
 DataVtxFraction[15]=0.0843895; //, x=14.5
 DataVtxFraction[16]=0.0807308; //, x=15.5
 DataVtxFraction[17]=0.0740844; //, x=16.5
 DataVtxFraction[18]=0.0653454; //, x=17.5
 DataVtxFraction[19]=0.0552985; //, x=18.5
 DataVtxFraction[20]=0.0458637; //, x=19.5
 DataVtxFraction[21]=0.0374919; //, x=20.5
 DataVtxFraction[22]=0.029127; //, x=21.5
 DataVtxFraction[23]=0.0226842; //, x=22.5
 DataVtxFraction[24]=0.0166156; //, x=23.5
 DataVtxFraction[25]=0.0122955; //, x=24.5
 DataVtxFraction[26]=0.00873899; //, x=25.5
 DataVtxFraction[27]=0.00633692; //, x=26.5
 DataVtxFraction[28]=0.00455865; //, x=27.5
 DataVtxFraction[29]=0.00308777; //, x=28.5
 DataVtxFraction[30]=0.00220843; //, x=29.5
 DataVtxFraction[31]=0.00151108; //, x=30.5
 DataVtxFraction[32]=0.000999982; //, x=31.5
 DataVtxFraction[33]=0.000702633; //, x=32.5
 DataVtxFraction[34]=0.000472478; //, x=33.5
 DataVtxFraction[35]=0.000327507; //, x=34.5
 DataVtxFraction[36]=0.000229096; //, x=35.5
 DataVtxFraction[37]=0.000143384; //, x=36.5
 DataVtxFraction[38]=0.000129627; //, x=37.5
 DataVtxFraction[39]=8.09509e-05; //, x=38.5
 DataVtxFraction[40]=5.66127e-05; //, x=39.5
 DataVtxFraction[41]=4.12691e-05; //, x=40.5
 DataVtxFraction[42]=1.95764e-05; //, x=41.5
 DataVtxFraction[43]=2.328e-05; //, x=42.5
 DataVtxFraction[44]=1.26982e-05; //, x=43.5
 DataVtxFraction[45]=3.70364e-06; //, x=44.5
 DataVtxFraction[46]=5.82e-06; //, x=45.5
 DataVtxFraction[47]=1.00527e-05; //, x=46.5
 DataVtxFraction[48]=3.17455e-06; //, x=47.5
 DataVtxFraction[49]=2.11636e-06; //, x=48.5
 DataVtxFraction[50]=1.05818e-06; //, x=49.5
 DataVtxFraction[51]=2.11636e-06; //, x=50.5
 DataVtxFraction[52]=1.58727e-06; //, x=51.5
 DataVtxFraction[53]=5.29091e-07; //, x=52.5
 DataVtxFraction[54]=0; //, x=53.5
 DataVtxFraction[55]=0; //, x=54.5
 DataVtxFraction[56]=0; //, x=55.5
 DataVtxFraction[57]=0; //, x=56.5
 DataVtxFraction[58]=0; //, x=57.5
 DataVtxFraction[59]=0; //, x=58.5


MonteCarloVtxFraction[0]=0; //, x=-0.5
 MonteCarloVtxFraction[1]=0; //, x=0.5
 MonteCarloVtxFraction[2]=0.00184571; //, x=1.5
 MonteCarloVtxFraction[3]=0.00337959; //, x=2.5
 MonteCarloVtxFraction[4]=0.00550107; //, x=3.5
 MonteCarloVtxFraction[5]=0.00806776; //, x=4.5
 MonteCarloVtxFraction[6]=0.0112365; //, x=5.5
 MonteCarloVtxFraction[7]=0.0155677; //, x=6.5
 MonteCarloVtxFraction[8]=0.020501; //, x=7.5
 MonteCarloVtxFraction[9]=0.0265933; //, x=8.5
 MonteCarloVtxFraction[10]=0.032873; //, x=9.5
 MonteCarloVtxFraction[11]=0.0398053; //, x=10.5
 MonteCarloVtxFraction[12]=0.0462202; //, x=11.5
 MonteCarloVtxFraction[13]=0.0523557; //, x=12.5
 MonteCarloVtxFraction[14]=0.0596394; //, x=13.5
 MonteCarloVtxFraction[15]=0.0628207; //, x=14.5
 MonteCarloVtxFraction[16]=0.0659227; //, x=15.5
 MonteCarloVtxFraction[17]=0.0671051; //, x=16.5
 MonteCarloVtxFraction[18]=0.065665; //, x=17.5
 MonteCarloVtxFraction[19]=0.0632155; //, x=18.5
 MonteCarloVtxFraction[20]=0.0583254; //, x=19.5
 MonteCarloVtxFraction[21]=0.0535165; //, x=20.5
 MonteCarloVtxFraction[22]=0.0469177; //, x=21.5
 MonteCarloVtxFraction[23]=0.0409624; //, x=22.5
 MonteCarloVtxFraction[24]=0.0344935; //, x=23.5
 MonteCarloVtxFraction[25]=0.0283489; //, x=24.5
 MonteCarloVtxFraction[26]=0.0228262; //, x=25.5
 MonteCarloVtxFraction[27]=0.0181939; //, x=26.5
 MonteCarloVtxFraction[28]=0.0135814; //, x=27.5
 MonteCarloVtxFraction[29]=0.0106777; //, x=28.5
 MonteCarloVtxFraction[30]=0.00773611; //, x=29.5
 MonteCarloVtxFraction[31]=0.00542897; //, x=30.5
 MonteCarloVtxFraction[32]=0.00372746; //, x=31.5
 MonteCarloVtxFraction[33]=0.00254506; //, x=32.5
 MonteCarloVtxFraction[34]=0.00166907; //, x=33.5
 MonteCarloVtxFraction[35]=0.00111932; //, x=34.5
 MonteCarloVtxFraction[36]=0.000663301; //, x=35.5
 MonteCarloVtxFraction[37]=0.000389329; //, x=36.5
 MonteCarloVtxFraction[38]=0.00024333; //, x=37.5
 MonteCarloVtxFraction[39]=0.000117159; //, x=38.5
 MonteCarloVtxFraction[40]=8.832e-05; //, x=39.5
 MonteCarloVtxFraction[41]=4.86661e-05; //, x=40.5
 MonteCarloVtxFraction[42]=3.78514e-05; //, x=41.5
 MonteCarloVtxFraction[43]=1.26171e-05; //, x=42.5
 MonteCarloVtxFraction[44]=3.6049e-06; //, x=43.5
 MonteCarloVtxFraction[45]=7.20979e-06; //, x=44.5
 MonteCarloVtxFraction[46]=3.6049e-06; //, x=45.5
 MonteCarloVtxFraction[47]=0; //, x=46.5
 MonteCarloVtxFraction[48]=0; //, x=47.5
 MonteCarloVtxFraction[49]=0; //, x=48.5
 MonteCarloVtxFraction[50]=0; //, x=49.5
 MonteCarloVtxFraction[51]=0; //, x=50.5
 MonteCarloVtxFraction[52]=0; //, x=51.5
 MonteCarloVtxFraction[53]=0; //, x=52.5
 MonteCarloVtxFraction[54]=0; //, x=53.5
 MonteCarloVtxFraction[55]=0; //, x=54.5
 MonteCarloVtxFraction[56]=0; //, x=55.5
 MonteCarloVtxFraction[57]=0; //, x=56.5
 MonteCarloVtxFraction[58]=0; //, x=57.5
 MonteCarloVtxFraction[59]=0; //, x=58.5



}

float NormalizedVtxEfficiency(int nVertices, float EffRatio) {

  return 1. + (EffRatio - 1.)*(nVertices-AverageNvtxDo)/(AverageNvtxUp-AverageNvtxDo);

}

void GetPUStrategy1(TString TestRegion, TString MetType = "") {

  FillVtxFractions();

  TH1F *NvtxUp = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_nvtxup_ll");
  TH1F *NvtxDo = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_nvtxdo_ll");

  TH1F *Nvtxee = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_ee");
  TH1F *Nvtxem = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_em");
  TH1F *Nvtxmm = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_mm");
  TH1F* Nvtxll = (TH1F*) InputRootFile->Get(TestRegion + "/h_MT2ll" + MetType + "_ll");

  int nBins = Nvtxll->GetNbinsX();
  float xMin = Nvtxll->GetBinLowEdge(1);
  float xMax = Nvtxll->GetBinLowEdge(nBins+1);

  NvtxUpee = new TH1F("h_MT2ll" + MetType + "_ee", "", nBins, xMin, xMax);
  NvtxUpem = new TH1F("h_MT2ll" + MetType + "_em", "", nBins, xMin, xMax);
  NvtxUpmm = new TH1F("h_MT2ll" + MetType + "_mm", "", nBins, xMin, xMax);
  NvtxUpll = new TH1F("h_MT2ll" + MetType + "_ll", "", nBins, xMin, xMax);

  NvtxDoee = new TH1F("h_MT2ll" + MetType + "_ee", "", nBins, xMin, xMax);
  NvtxDoem = new TH1F("h_MT2ll" + MetType + "_em", "", nBins, xMin, xMax);
  NvtxDomm = new TH1F("h_MT2ll" + MetType + "_mm", "", nBins, xMin, xMax);
  NvtxDoll = new TH1F("h_MT2ll" + MetType + "_ll", "", nBins, xMin, xMax);

  for (int ib = 1; ib<=NvtxUp->GetNbinsX(); ib++) {

    float EffRatio = 1.;
    if (NvtxUp->GetBinContent(ib)>0. && NvtxDo->GetBinContent(ib)>0.) EffRatio = (NvtxUp->GetBinContent(ib)/FractionNvtxUp)/(NvtxDo->GetBinContent(ib)/FractionNvtxDo);
    
    float DataAverage = 0., MonteCarloAverage = 0.;

    for (int iv = 1; iv<MaxVertex; iv++) {

      DataAverage += DataVtxFraction[iv]*NormalizedVtxEfficiency(iv, EffRatio);
      MonteCarloAverage += MonteCarloVtxFraction[iv]*NormalizedVtxEfficiency(iv, EffRatio);

    }

    float RelativeUncertainty = (DataAverage - MonteCarloAverage)/MonteCarloAverage;

    NvtxDoee->SetBinContent(ib, Nvtxee->GetBinContent(ib)*(1.-RelativeUncertainty));
    NvtxDoem->SetBinContent(ib, Nvtxem->GetBinContent(ib)*(1.-RelativeUncertainty));
    NvtxDomm->SetBinContent(ib, Nvtxmm->GetBinContent(ib)*(1.-RelativeUncertainty));
    NvtxDoll->SetBinContent(ib, Nvtxll->GetBinContent(ib)*(1.-RelativeUncertainty));

    NvtxDoee->SetBinError(ib, Nvtxee->GetBinError(ib));
    NvtxDoem->SetBinError(ib, Nvtxem->GetBinError(ib));
    NvtxDomm->SetBinError(ib, Nvtxmm->GetBinError(ib));
    NvtxDoll->SetBinError(ib, Nvtxll->GetBinError(ib));

    NvtxUpee->SetBinContent(ib, Nvtxee->GetBinContent(ib)*(1.+RelativeUncertainty));
    NvtxUpem->SetBinContent(ib, Nvtxem->GetBinContent(ib)*(1.+RelativeUncertainty));
    NvtxUpmm->SetBinContent(ib, Nvtxmm->GetBinContent(ib)*(1.+RelativeUncertainty));
    NvtxUpll->SetBinContent(ib, Nvtxll->GetBinContent(ib)*(1.+RelativeUncertainty));

    NvtxUpee->SetBinError(ib, Nvtxee->GetBinError(ib));
    NvtxUpem->SetBinError(ib, Nvtxem->GetBinError(ib));
    NvtxUpmm->SetBinError(ib, Nvtxmm->GetBinError(ib));
    NvtxUpll->SetBinError(ib, Nvtxll->GetBinError(ib));

  }
  
  //InputRootFile->Close();

}

void GetPileUpHistograms(int Strategy, TString TestRegion, TString MetType = "") {

  if (Strategy==0) GetPUStrategy0(TestRegion);
  if (Strategy==1) GetPUStrategy1(TestRegion, MetType);

}

void BuildMassPointSystematic(int Strategy, TString MassPoint) {

  TString InputRootFileName = RootFilesDirectory + "/nominal/Stop/T2tt" + MassPoint + ".root";  
  InputRootFile = TFile::Open(InputRootFileName);

  TString OutputDirectoryUp = RootFilesDirectory + "/Pileupup/Stop";
  TString OutputDirectoryDo = RootFilesDirectory + "/Pileupdo/Stop";

  gSystem->mkdir(OutputDirectoryUp, true);
  gSystem->mkdir(OutputDirectoryDo, true);
  
  TFile *Pileupup = new TFile(OutputDirectoryUp + "/T2tt" + MassPoint + ".root", "recreate");
  TFile *Pileupdo = new TFile(OutputDirectoryDo + "/T2tt" + MassPoint + ".root", "recreate");

  TString MetTypeName[4] = {"", "gen", "isr", "isrgen"};

  for (int rg = 0; rg<ncut; rg++) {

    if (!scut[rg].Contains("VR") && !scut[rg].Contains("SR")) continue;
    
    Pileupup->cd();
    
    gDirectory->mkdir(scut[rg]);
      
    Pileupdo->cd();
      
    gDirectory->mkdir(scut[rg]);
    
    for (int mt = 0; mt<4; mt++) {
    
      GetPileUpHistograms(Strategy, scut[rg], MetTypeName[mt]);  
    
      Pileupup->cd(scut[rg]);
      
      NvtxUpee->Write();
      NvtxUpem->Write();
      NvtxUpmm->Write();
      NvtxUpll->Write();
      
      Pileupup->Write();
      
      Pileupdo->cd(scut[rg]);
      
      NvtxDoee->Write();
      NvtxDoem->Write();
      NvtxDomm->Write();
      NvtxDoll->Write();
      
      Pileupdo->Write();
      
    }

  }

  Pileupup->Close();
  Pileupdo->Close();
  
  InputRootFile->Close();

}

void BuildSystematic(int Strategy = 1) {

  ifstream MassPointFile; MassPointFile.open(MassPointFileName);

  while (MassPointFile) {

    TString MassPoint;
    MassPointFile >> MassPoint;

    if (MassPoint!="" && !MassPoint.Contains("#")) 
      BuildMassPointSystematic(Strategy, MassPoint);

  }

}


