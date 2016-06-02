// Constants
//------------------------------------------------------------------------------
const TCut all   = "metPfType1 > -999";
const TCut lep1  = "std_vector_lepton_pt[0] > 30";
const TCut lep2  = "std_vector_lepton_pt[1] > 10";
const TCut lep3  = "std_vector_lepton_pt[2] < 10";
const TCut njet  = "njet > 1";
const TCut met   = "metPfType1 > 50";
const TCut mm    = "std_vector_lepton_flavour[0]*std_vector_lepton_flavour[1] == -13*13";
const TCut ee    = "std_vector_lepton_flavour[0]*std_vector_lepton_flavour[1] == -11*11";
const TCut em    = "std_vector_lepton_flavour[0]*std_vector_lepton_flavour[1] == -11*13";
const TCut zveto = em || "(mll < 76.188 && mll > 106.188)";
const TCut m2l   = "mll > 20";

const TString path = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight/";


// Data members
//------------------------------------------------------------------------------
TGraphErrors* old_graph;
TGraphErrors* new_graph;

TEfficiency* old_efficiency;
TEfficiency* new_efficiency;

int          old_sum_denominator;
int          old_sum_numerator;
int          new_sum_denominator;
int          new_sum_numerator;


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

  old_efficiency = new TEfficiency("old_efficiency", ";ttDM sample;efficiency", 18, -0.5, 18);
  new_efficiency = new TEfficiency("new_efficiency", ";ttDM sample;efficiency", 18, -0.5, 18);

  old_sum_denominator = 0;
  old_sum_numerator   = 0;
  new_sum_denominator = 0;
  new_sum_numerator   = 0;

  int graph_element = 0;

  printf("\n");

  GetEfficiency("old", graph_element,   "ttDM0001pseudo0010");
  GetEfficiency("new", graph_element++, "ttDM0001pseudo00010");

  GetEfficiency("old", graph_element,   "ttDM0001pseudo0100");
  GetEfficiency("new", graph_element++, "ttDM0001pseudo00100");

  GetEfficiency("old", graph_element,   "ttDM0010pseudo0100");
  GetEfficiency("new", graph_element++, "ttDM0010pseudo00100");

  GetEfficiency("old", graph_element,   "ttDM0050pseudo0200");
  GetEfficiency("new", graph_element++, "ttDM0050pseudo00200");

  GetEfficiency("old", graph_element,   "ttDM0150pseudo0200");
  GetEfficiency("new", graph_element++, "ttDM0150pseudo00200");

  GetEfficiency("old", graph_element,   "ttDM0150pseudo0500");
  GetEfficiency("new", graph_element++, "ttDM0150pseudo00500");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0010");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00010");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0050");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00050");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0100");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00100");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0200");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00200");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0300");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00300");

  GetEfficiency("old", graph_element,   "ttDM0001scalar0500");
  GetEfficiency("new", graph_element++, "ttDM0001scalar00500");

  GetEfficiency("old", graph_element,   "ttDM0010scalar0050");
  GetEfficiency("new", graph_element++, "ttDM0010scalar00050");

  GetEfficiency("old", graph_element,   "ttDM0010scalar0100");
  GetEfficiency("new", graph_element++, "ttDM0010scalar00100");

  GetEfficiency("old", graph_element,   "ttDM0050scalar0050");
  GetEfficiency("new", graph_element++, "ttDM0050scalar00050");

  GetEfficiency("old", graph_element,   "ttDM0050scalar0200");
  GetEfficiency("new", graph_element++, "ttDM0050scalar00200");

  GetEfficiency("old", graph_element,   "ttDM0050scalar0300");
  GetEfficiency("new", graph_element++, "ttDM0050scalar00300");

  GetEfficiency("old", graph_element,   "ttDM0500scalar0500");
  GetEfficiency("new", graph_element++, "ttDM0500scalar00500");


  // Draw TGraph
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1");

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

  c1->SaveAs("graphs.png");


  // Draw TEfficiency
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("c2", "c2");

  old_efficiency->SetLineWidth  (2);
  old_efficiency->SetLineColor  (kRed+1);
  old_efficiency->SetMarkerColor(kRed+1);
  old_efficiency->SetMarkerStyle(kOpenCircle);

  new_efficiency->SetLineWidth  (2);
  new_efficiency->SetLineColor  (kBlack);
  new_efficiency->SetMarkerColor(kBlack);
  new_efficiency->SetMarkerStyle(kFullCircle);

  old_efficiency->Draw("ap");
  new_efficiency->Draw("p,same");

  c2->Update();

  old_efficiency->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.6);
  old_efficiency->GetPaintedGraph()->GetYaxis()->SetTitleOffset(1.8);

  c2->SaveAs("efficiencies.png");


  // Draw TGraph differences
  //----------------------------------------------------------------------------
  TCanvas* c3 = new TCanvas("c3", "c3");

  TH1F* hdiff = new TH1F("hdiff", ";(old - new) / error; entries / bin", 10, -3, 3);

  hdiff->SetFillStyle(1001);
  hdiff->SetFillColor(kGray+1);
  hdiff->SetLineColor(kGray+1);

  for (int i=0; i<old_graph->GetN(); i++)
    {
      double x;

      double old_value;
      double new_value;

      old_graph->GetPoint(i, x, old_value);
      new_graph->GetPoint(i, x, new_value);

      float old_error = old_graph->GetErrorY(i);
      float new_error = new_graph->GetErrorY(i);

      float dif_value = old_value - new_value;
      float dif_error = sqrt(old_error*old_error + new_error*new_error);

      hdiff->Fill(dif_value/dif_error);
    }

  hdiff->Draw("hist");


  // Fit
  //----------------------------------------------------------------------------
  TF1* g1 = new TF1("g1", "gaus", -3, 3);

  g1->SetParameters(18, 0, 1);

  g1->SetLineColor(kRed+1);

  hdiff->Fit(g1);

  g1->Draw("same");

  hdiff->GetXaxis()->SetTitleOffset(1.6);
  hdiff->GetYaxis()->SetTitleOffset(1.6);

  c3->GetFrame()->DrawClone();

  c3->SaveAs("diff.png");


  // Grand total
  //----------------------------------------------------------------------------
  float old_eff_value = float(old_sum_numerator) / old_sum_denominator;
  float old_eff_error = 1e2 * sqrt(old_eff_value*(1. - old_eff_value) / old_sum_denominator);

  float new_eff_value = float(new_sum_numerator) / new_sum_denominator;
  float new_eff_error = 1e2 * sqrt(new_eff_value*(1. - new_eff_value) / new_sum_denominator);

  old_eff_value *= 1e2;
  new_eff_value *= 1e2;

  float rel_dif_value = 2e2 * (old_eff_value - new_eff_value) / (old_eff_value + new_eff_value);

  printf("\n");
  printf(" old efficiency      = (%5.2f +- %4.2f)%% (%6d / %6d)\n", old_eff_value, old_eff_error, old_sum_numerator, old_sum_denominator);
  printf(" new efficiency      = (%5.2f +- %4.2f)%% (%6d / %6d)\n", new_eff_value, new_eff_error, new_sum_numerator, new_sum_denominator);
  printf(" relative difference = %.2f%%\n", rel_dif_value);
  printf("\n");
}


//------------------------------------------------------------------------------
// GetEfficiency
//------------------------------------------------------------------------------
void GetEfficiency(TString era,
		   int     element,
		   TString sample)
{
  TFile* file = new TFile(path + "latino_" + sample + ".root");

  TCanvas* canvas = new TCanvas("canvas " + sample, "canvas " + sample);
  
  TCut pass_den = all;
  TCut pass_num = pass_den && lep1 && lep2 && lep3 && njet && met && (ee || mm || em) && zveto && m2l;

  TTree* latino = (TTree*)file->Get("latino");

  int numerator   = latino->Draw("metPfType1", pass_num);
  int denominator = latino->Draw("metPfType1", pass_den);

  float eff_value = float(numerator) / denominator;
  float eff_error = 1e2 * sqrt(eff_value*(1. - eff_value) / denominator);
  
  eff_value *= 1e2;

  printf(" %-20s efficiency = (%5.2f +- %4.2f)%% (%4d / %4d)\n",
	 sample.Data(),
	 eff_value,
	 eff_error,
	 numerator,
	 denominator);

  if (era.EqualTo("old"))
    {
      old_graph->SetPoint     (element, element, eff_value);
      old_graph->SetPointError(element,    1e-9, eff_error);

      old_efficiency->SetTotalEvents (element+1, denominator);
      old_efficiency->SetPassedEvents(element+1, numerator);

      old_sum_denominator += denominator;
      old_sum_numerator   += numerator;
    }
  else
    {
      printf("\n");

      new_graph->SetPoint     (element, element, eff_value);
      new_graph->SetPointError(element,    1e-9, eff_error);

      new_efficiency->SetTotalEvents (element+1, denominator);
      new_efficiency->SetPassedEvents(element+1, numerator);

      new_sum_denominator += denominator;
      new_sum_numerator   += numerator;
    }


  // Close
  //----------------------------------------------------------------------------
  delete latino;

  file->Close();
}
