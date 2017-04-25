void TTVPlot()
{

  gInterpreter->ExecuteMacro("../test/PaperStyle.C");

  TFile* my1file = new TFile( "../minitrees/nominal/TTDM/TTZToLLNuNu_M-10.root", "read" );
  TTree* my1tree = (TTree*) my1file -> Get( "latino" );
  my1tree->Draw("mt2ll>>www(10, 80., 200.)", "2.15*eventW*(mt2ll>80)");
  TH1F* h10 = (TH1F*) gDirectory -> Get( "www" ); cout << h10->Integral() << endl;

  h10->SetDirectory(0);

  TFile* my2file = new TFile( "../minitrees/nominal/TTDM/TTZToQQ.root", "read" );
  TTree* my2tree = (TTree*) my2file -> Get( "latino" );
  my2tree->Draw("mt2ll>>kkk(10, 80., 200.)", "2.15*eventW*(mt2ll>80)");
  TH1F* h20 = (TH1F*) gDirectory -> Get( "kkk" ); cout << h20->Integral() << endl;

  h20->SetDirectory(0);

  TFile* my3file = new TFile( "../minitrees/nominal/TTDM/TTWJetsToLNu.root", "read" );
  TTree* my3tree = (TTree*) my3file -> Get( "latino" );
  my3tree->Draw("mt2ll>>www(10, 80., 200.)", "2.15*eventW*(mt2ll>80)"); 
  TH1F* h30 = (TH1F*) gDirectory -> Get( "www" ); cout << h30->Integral() << endl;
  
  h30->SetDirectory(0);

  TFile* my4file = new TFile( "../minitrees/nominal/TTDM/TTWJetsToQQ.root", "read" );
  TTree* my4tree = (TTree*) my4file -> Get( "latino" );
  my4tree->Draw("mt2ll>>kkk(10, 80., 200.)", "2.15*eventW*(mt2ll>80)"); 
  TH1F* h40 = (TH1F*) gDirectory -> Get( "kkk" ); cout << h40->Integral() << endl;

  h40->SetDirectory(0);

  h30 -> SetTitle("ttV background study");
  h30 -> GetXaxis() -> SetTitle("mt2ll");
  h30 -> SetMinimum(0.);

  h10->SetLineColor(kBlue); h10->SetLineWidth(2.);
  h20->SetLineColor(kGreen+1); h20->SetLineWidth(2.);
  h30->SetLineColor(kRed+2); h30->SetLineWidth(2.);
  h40->SetLineColor(kMagenta+1); h40->SetLineWidth(2.);

  TCanvas* canvas = new TCanvas("ttv", "ttv");

  h30->Draw("hist");
  h10->Draw("hist, same");
  h20->Draw("hist, same");
  h40->Draw("hist,same");
  
  float total = h10->Integral() + h20->Integral() + h30->Integral() + h40->Integral();

  TLegend* theleg = new TLegend( 0.58, 0.70, 0.88, 0.88 );
  theleg->AddEntry( h10, Form("TTZ, Z#rightarrow #nu#nu (%5.0f %%)", 100*(h10->Integral()/total)),"l");
  theleg->AddEntry( h20, Form("TTZ, Z#rightarrow qq (%5.0f %%)", 100*(h20->Integral()/total)),"l");
  theleg->AddEntry( h30, Form("TTW, W#rightarrow l#nu (%5.0f %%)", 100*(h30->Integral()/total)),"l");
  theleg->AddEntry( h40, Form("TTW, W#rightarrow qq (%5.0f %%)", 100*(h40->Integral())/total),"l");
  theleg->Draw("same");
  
  my1file->Close();
  my2file->Close();
  my3file->Close();     
  my4file->Close();    
  
  canvas->SaveAs("ttv-80.png");
}
