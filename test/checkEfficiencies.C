// Constants and data members
//------------------------------------------------------------------------------
const TCut all  = "metPfType1 > -999";
const TCut lep1 = "std_vector_lepton_pt[0] > 30";
const TCut lep2 = "std_vector_lepton_pt[1] > 10";
const TCut lep3 = "std_vector_lepton_pt[2] < 10";
const TCut njet = "njet > 1";
const TCut met  = "metPfType1 > 50";

const TString path = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight/";

TGraphErrors* old_graph;
TGraphErrors* new_graph;


// Functions
//------------------------------------------------------------------------------
void GetEfficiency(TString era,
		   int     element,
		   TString sample);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// checkEfficiencies
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void checkEfficiencies()
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  old_graph = new TGraphErrors();
  new_graph = new TGraphErrors();

  int graph_element = 0;

  GetEfficiency("old", graph_element,   "latino_ttDM0001pseudo0010");
  GetEfficiency("new", graph_element++, "latino_ttDM0001pseudo00010");

  GetEfficiency("old", graph_element,   "latino_ttDM0001pseudo0100");
  GetEfficiency("new", graph_element++, "latino_ttDM0001pseudo00100");

  GetEfficiency("old", graph_element,   "latino_ttDM0010pseudo0100");
  GetEfficiency("new", graph_element++, "latino_ttDM0010pseudo00100");

  GetEfficiency("old", graph_element,   "latino_ttDM0050pseudo0200");
  GetEfficiency("new", graph_element++, "latino_ttDM0050pseudo00200");

  GetEfficiency("old", graph_element,   "latino_ttDM0150pseudo0200");
  GetEfficiency("new", graph_element++, "latino_ttDM0150pseudo00200");

  GetEfficiency("old", graph_element,   "latino_ttDM0150pseudo0500");
  GetEfficiency("new", graph_element++, "latino_ttDM0150pseudo00500");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0010");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00010");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0050");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00050");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0100");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00100");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0200");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00200");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0300");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00300");

  GetEfficiency("old", graph_element,   "latino_ttDM0001scalar0500");
  GetEfficiency("new", graph_element++, "latino_ttDM0001scalar00500");

  GetEfficiency("old", graph_element,   "latino_ttDM0010scalar0050");
  GetEfficiency("new", graph_element++, "latino_ttDM0010scalar00050");

  GetEfficiency("old", graph_element,   "latino_ttDM0010scalar0100");
  GetEfficiency("new", graph_element++, "latino_ttDM0010scalar00100");

  GetEfficiency("old", graph_element,   "latino_ttDM0050scalar0050");
  GetEfficiency("new", graph_element++, "latino_ttDM0050scalar00050");

  GetEfficiency("old", graph_element,   "latino_ttDM0050scalar0200");
  GetEfficiency("new", graph_element++, "latino_ttDM0050scalar00200");

  GetEfficiency("old", graph_element,   "latino_ttDM0050scalar0300");
  GetEfficiency("new", graph_element++, "latino_ttDM0050scalar00300");

  GetEfficiency("old", graph_element,   "latino_ttDM0500scalar0500");
  GetEfficiency("new", graph_element++, "latino_ttDM0500scalar00500");


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* output = new TCanvas("output", "output");

  old_graph->SetLineWidth  (2);
  old_graph->SetLineColor  (kRed+1);
  old_graph->SetMarkerColor(kRed+1);
  old_graph->SetMarkerStyle(kOpenCircle);

  new_graph->SetLineWidth  (2);
  new_graph->SetLineColor  (kBlack);
  new_graph->SetMarkerColor(kBlack);
  new_graph->SetMarkerStyle(kFullCircle);

  TMultiGraph* mg = new TMultiGraph();

  mg->Add(old_graph);
  mg->Add(new_graph);

  mg->Draw("ap");

  mg->GetXaxis()->SetTitle("ttDM sample");
  mg->GetYaxis()->SetTitle("efficiency (%)");

  mg->GetXaxis()->SetTitleOffset(1.6);
  mg->GetYaxis()->SetTitleOffset(1.6);

  output->SaveAs("output.png");
}


//------------------------------------------------------------------------------
// GetEfficiency
//------------------------------------------------------------------------------
void GetEfficiency(TString era,
		   int     element,
		   TString sample)
{
  TFile* file = new TFile(path + sample + ".root");

  TCanvas* canvas = new TCanvas("canvas " + sample, "canvas " + sample);
  
  TCut pass_den = all;
  TCut pass_num = pass_den && lep1 && lep2 && lep3 && njet && met;

  TTree* latino = (TTree*)file->Get("latino");

  Long64_t numerator   = latino->Draw("metPfType1", pass_num);
  Long64_t denominator = latino->Draw("metPfType1", pass_den);

  float eff_value = float(numerator) / denominator;
  float eff_error = 1e2 * sqrt(eff_value*(1. - eff_value) / denominator);
  
  eff_value *= 1e2;

  if (era.EqualTo("old"))
    {
      old_graph->SetPoint     (element, element, eff_value);
      old_graph->SetPointError(element,    1e-9, eff_error);
    }
  else
    {
      new_graph->SetPoint     (element, element, eff_value);
      new_graph->SetPointError(element,    1e-9, eff_error);
    }


  // Close
  //----------------------------------------------------------------------------
  delete latino;

  file->Close();
}
