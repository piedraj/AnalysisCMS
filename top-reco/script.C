#include "/afs/cern.ch/user/p/pablom/tdrstyle.C"


void run() {

   setTDRStyle();

   ifstream a("values.txt");
   TH1F *h = new TH1F("h", "", 20, -400, 400);
   while(!a.eof()) {
      float b, c, d, e;
      a >> b >> c >> d >> e;
      if(a.eof()) break;
      h->Fill(b-c);
      h->Fill(d-e);
   }
   h->GetXaxis()->SetTitle("MET neutrinos - MET (higher weight) [GeV]");
   h->GetXaxis()->CenterTitle();
   h->Draw();
   a.close();

}
