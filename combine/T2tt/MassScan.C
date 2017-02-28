#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TTree.h>
#include <fstream>
#include <iostream>

void MassScan() {

  TH2F *MassLimit = new TH2F("MassLimit", "", 9, 287.5, 512.5, 13, 112.5, 437.5);

  ifstream InFile; InFile.open("./MassPointList.txt");

  while (InFile) {

    TString MassPoint;
    InFile >> MassPoint;
   
    if (!MassPoint.Contains("#")) {
      
      TString RootFileName = "./Datacards/MassPoint" + MassPoint + "/datacardFinal.root";
      TFile *RootFile = TFile::Open(RootFileName, "read");

      if (!RootFile) continue;

      TTree *fChain = (TTree*) RootFile->Get("limit");
      
      Float_t _quantileExpected; Double_t _limit;
      fChain->SetBranchAddress("quantileExpected", &_quantileExpected);
      fChain->SetBranchAddress("limit",            &_limit);

      int _nentries     = fChain->GetEntries();

      for (Long64_t jentry=0; jentry<_nentries;jentry++) {

	fChain->GetEntry(jentry);

	if (_quantileExpected==0.5) {

	  TString StopMass = MassPoint;
	  StopMass.Replace(0, StopMass.First("_")+1, "");
	  StopMass.Replace(0, StopMass.First("_")+1, "");
	  TString XMass = StopMass;
	  StopMass.Replace(StopMass.First("_"), 10000,"");
	  StopMass.ReplaceAll("Sm", "");
	  XMass.Replace(0, XMass.First("_")+1, "");
	  XMass.ReplaceAll("Xm", "");

	  MassLimit->Fill(StopMass.Atoi(), XMass.Atoi(), _limit);

	}

      }

    }

  }

  TCanvas *CC = new TCanvas("MassScan", "", 1200,1000);
  gStyle->SetOptStat(0);
  MassLimit->SetXTitle("M_{#tilde t} [GeV]");
  MassLimit->SetYTitle("M_{#tilde #Chi^{0}_{1}} [GeV]");
  MassLimit->Draw("TEXTCOLZ");
  CC -> Print("MassScan.png");
  
}
