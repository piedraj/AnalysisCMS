#define AnalysisShape_cxx
#include "../include/AnalysisShape.h"

//------------------------------------------------------------------------------
// AnalysisShape
//------------------------------------------------------------------------------
AnalysisShape::AnalysisShape(TTree* tree, TString systematic) : AnalysisCMS(tree, systematic)
{
  SetSaveMinitree(false);
}


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void AnalysisShape::Loop(TString analysis, TString filename, float luminosity)
{
  if (fChain == 0) return;

  Setup(analysis, filename, luminosity);


  // Define histograms
  //----------------------------------------------------------------------------
  root_output->cd();

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
      }
    }
  }

  root_output->cd();


  // Loop over events
  //----------------------------------------------------------------------------
  for (Long64_t jentry=0; jentry<_nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    PrintProgress(jentry, _nentries);

    EventSetup();

    // Analysis
    //--------------------------------------------------------------------------
    _nelectron = 0;

    if (abs(Lepton1.flavour) == ELECTRON_FLAVOUR) _nelectron++;
    if (abs(Lepton2.flavour) == ELECTRON_FLAVOUR) _nelectron++;

    if      (_nelectron == 2) _channel = ee;
    else if (_nelectron == 1) _channel = em;
    else if (_nelectron == 0) _channel = mm;
    
    if (Lepton1.v.Pt() < 30.) continue;
    if (Lepton2.v.Pt() < 15.) continue;

    _m2l  = mll;   // Needs l2Sel
    _pt2l = ptll;  // Needs l2Sel

    bool pass;

    // Has 2 "tight" leptons
    //--------------------------------------------------------------------------
    //    pass = (std_vector_lepton_isTightLepton -> at(0) == 1 && std_vector_lepton_isTightLepton -> at(1) == 1);
    pass = true;
    
    FillLevelHistograms(Shape_01_Has2Leptons, pass);

    // No additional lepton
    //--------------------------------------------------------------------------
    pass &= (std_vector_lepton_pt -> at(2) < 10.);

    FillLevelHistograms(Shape_02_HasOnly2Leptons, pass);

    // At least two jets
    //--------------------------------------------------------------------------
    pass &= (_njet > 1);
    pass &= (_nbjet30csvv2m > 0);
    pass &= (AnalysisJets[0].v.Pt() > 30. && fabs(AnalysisJets[0].v.Eta()) < 4.);
    pass &= (AnalysisJets[1].v.Pt() > 30. && fabs(AnalysisJets[1].v.Eta()) < 4.);

    FillLevelHistograms(Shape_03_Has2Jets, pass);

    // MET
    //--------------------------------------------------------------------------
    pass &= (MET.Et() >= 50.);

    FillLevelHistograms(Shape_04_MET, pass);

    // Deltaphi
    //--------------------------------------------------------------------------
    pass &= (_dphillmet > 1.2);

    FillLevelHistograms(Shape_05_DeltaPhi, pass);

    // mll
    //--------------------------------------------------------------------------
    pass &= (mll >= 20.);

    FillLevelHistograms(Shape_06_mll, pass);

    // Zveto
    //--------------------------------------------------------------------------
    pass &= (_channel == em || fabs(_m2l - Z_MASS) > 15.);

    FillLevelHistograms(Shape_07_Zveto, pass);
    /*
    if(pass) {

      printf("================================== \n");
      for(int x=0;x<3;x++) 
	{
	  for(int y=0;y<3;y++)
	    {
	      printf("(%d,%d) : %f ", x, y, GetMomentumTensor()[x][y]);
	    }
	  cout<<endl; 
	}

      _miMatrix = GetMomentumTensor();
      _sphericity = GetSphericity(_miMatrix);
      printf("Sphericity: %f ", _sphericity); 

      printf("  Before adding to the histogram     ");
      std::cout << "Channel :" << _channel << "        Jetbin : " << _jetbin << std::endl;
      h_sphericity[(int)_channel][Shape_07_Zveto][_jetbin]->Fill(_sphericity, _event_weight);
      h_sphericity[ll][Shape_07_Zveto][_jetbin]->Fill(_sphericity, _event_weight);
      // h_sphericity[4][Shape_07_Zveto][_jetbin]->Fill(_sphericity, _event_weight);
      printf("After adding to the histogram \n");

      printf("================================== \n");

    }    
    */
  }

  EndJob();
}


//------------------------------------------------------------------------------
// FillLevelHistograms
//------------------------------------------------------------------------------
void AnalysisShape::FillLevelHistograms(int  icut,
					  bool pass)
{
  if (!pass) return;
  
  FillHistograms(_channel, icut, _jetbin);
  FillHistograms(_channel, icut, njetbin);
}
/*
TMatrixDSym AnalysisShape::GetMomentumTensor() 
{
  TMatrixDSym _smatrix(3); //TMatrixDSym has a funcion implemened to calculate the eigenvalues

  //Leptons
  for(unsigned int i=0; i<2; i++) {
    _smatrix[0][0] = AnalysisLeptons[i].v.Px() * AnalysisLeptons[i].v.Px();
    _smatrix[0][1] = AnalysisLeptons[i].v.Px() * AnalysisLeptons[i].v.Py();
    _smatrix[0][2] = AnalysisLeptons[i].v.Px() * AnalysisLeptons[i].v.Pz();
    
    _smatrix[1][0] = AnalysisLeptons[i].v.Px() * AnalysisLeptons[i].v.Py();
    _smatrix[1][1] = AnalysisLeptons[i].v.Py() * AnalysisLeptons[i].v.Py();
    _smatrix[1][2] = AnalysisLeptons[i].v.Py() * AnalysisLeptons[i].v.Pz();
    
    _smatrix[2][0] = AnalysisLeptons[i].v.Px() * AnalysisLeptons[i].v.Pz();
    _smatrix[2][1] = AnalysisLeptons[i].v.Py() * AnalysisLeptons[i].v.Pz();
    _smatrix[2][2] = AnalysisLeptons[i].v.Pz() * AnalysisLeptons[i].v.Pz();
  }    

  //Jets
  for(unsigned int i=0; i<AnalysisJets.size(); i++) {

    //    if(AnalysisJets[i].v.Pt() > 30.) {
      _smatrix[0][0] += AnalysisJets[i].v.Px() * AnalysisJets[i].v.Px();
      _smatrix[0][1] += AnalysisJets[i].v.Px() * AnalysisJets[i].v.Py();
      _smatrix[0][2] += AnalysisJets[i].v.Px() * AnalysisJets[i].v.Pz();
      
      _smatrix[1][0] += AnalysisJets[i].v.Px() * AnalysisJets[i].v.Py();
      _smatrix[1][1] += AnalysisJets[i].v.Py() * AnalysisJets[i].v.Py();
      _smatrix[1][2] += AnalysisJets[i].v.Py() * AnalysisJets[i].v.Pz();
      
      _smatrix[2][0] += AnalysisJets[i].v.Px() * AnalysisJets[i].v.Pz();
      _smatrix[2][1] += AnalysisJets[i].v.Py() * AnalysisJets[i].v.Pz();
      _smatrix[2][2] += AnalysisJets[i].v.Pz() * AnalysisJets[i].v.Pz();
      //    }
  }      

  return _smatrix;
}

TVectorD AnalysisShape::GetEigenvalues(TMatrixDSym _smatrix) {
  TMatrixDSymEigen eigen(_smatrix);
  TVectorD eigenvalues = eigen.GetEigenValues();
  
  //  sort(eigenvalues[0],eigenvalues[2]);
  
  return eigenvalues; 
}

float AnalysisShape::GetSphericity(TMatrixDSym _smatrix) {
  TVectorD eigenvalues = GetEigenvalues(_smatrix);

  float eigenvalue1 = eigenvalues[0];
  float eigenvalue2 = eigenvalues[1];
  float eigenvalue3 = eigenvalues[2];

  return 3*(eigenvalue2 + eigenvalue3)/(2*(eigenvalue1 + eigenvalue2 + eigenvalue3));
  }
*/


