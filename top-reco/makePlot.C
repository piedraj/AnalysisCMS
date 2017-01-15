#include "/afs/cern.ch/user/p/pablom/tdrstyle.C"


void makePlot() {

    setTDRStyle();

    TFile f("output.root");
    TH1F *dm300 = (TH1F *)f.Get("DM300");
    TH1F *ttbar = (TH1F *)f.Get("ttbar");
    dm300->GetXaxis()->SetTitle("weight");
    ttbar->GetXaxis()->SetTitle("weight");
    dm300->GetXaxis()->CenterTitle();
    ttbar->GetXaxis()->CenterTitle();
    dm300->SetLineColor(kBlue);
    ttbar->SetLineColor(kRed);
    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.9);
    leg->AddEntry(dm300, "DM(300,1)", "L");
    leg->AddEntry(ttbar, "ttbar", "L");
    leg->SetFillColor(kWhite);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->SetLineWidth(0);
    leg->SetBorderSize(0);

    TCanvas *m = new TCanvas("m");
    m->cd();
    m->SetLogy(1);
    dm300->Draw();
    ttbar->Draw("SAME");
    leg->Draw();
    m->SaveAs("weights.png"); 




}
