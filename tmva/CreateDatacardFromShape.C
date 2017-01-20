void CreateDatacardFromShape()
{

  TFile* datacard_file = new TFile ("/afs/cern.ch/work/j/jgarciaf/public/rootfile-for-datacard/datacard.root",  "read");

  enum {
    WZ, VZ, TT, ST, WW, ZJets, HWW, Wg, Zg, VVV, HZ, WgStar, nprocess
  };

  enum {
    nominal, Idisoup, Triggerup, nsystematic
  };

  TString processes[nprocess] = {"02_WZTo3LNu", "03_VZ", "04_TTTo2L2Nu", "05_ST", "06_WW", "07_ZJets", "10_HWW", "11_Wg", "12_Zg", "13_VVV", "14_HZ", "15_WgStar"};
  TString systematics[nsystematic] = {"nominal", "Idisoup", "Triggerup"};
 
  TH1D* h;
  float yield[nprocess][nsystematic];
 
  TString histo_name = "";

  for(int i = 0; i < nprocess; i++) {
    for(int j = 0; j < nsystematic; j++) {

      if(systematics[j] == "nominal") {
	histo_name = Form("%s", processes[i].Data());
      } else {
	histo_name = Form("%s_%s", processes[i].Data(), systematics[j].Data());
      }

      h = (TH1D*) datacard_file -> Get(histo_name);
      yield[i][j] = h -> Integral();

    }
  }

  ofstream datacard;
  datacard.open("datacards/datacard.txt");
  
  datacard << "imax 1 number of channels \n" ;
  datacard << Form("jmax %d number of backgrounds \n", nprocess-2);
  datacard << Form("kmax %d number of nuisance parameters \n", nsystematic);
  datacard << "------------ \n";
  datacard << "\n";

  //datacard << Form("observation %5.0f \n", yield[data][nominal]);
  datacard << "------------ \n";

  datacard << "bin        \t     \t  1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1  \t   1  \t   1   \t   1  \t  1       \n" ;
  datacard << "process    \t     \t  WZ  \t  VZ   \t  TT   \t  ST   \t  WW   \t ZJets \t  HWW  \t  Wg  \t  Zg  \t  VVV  \t  HZ  \t  WgStar  \n" ;
  datacard << "process    \t     \t  2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   10  \t  12  \t  12  \t   13  \t  14  \t  15      \n" ;

  datacard << Form("rate  \t  \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \t%7.3f \n", yield[WZ][nominal], yield[VZ][nominal], yield[TT][nominal], yield[ST][nominal], yield[WW][nominal], yield[ZJets][nominal], yield[HWW][nominal], yield[Wg][nominal], yield[Zg][nominal], yield[VVV][nominal], yield[HZ][nominal], yield[WgStar][nominal]);

  datacard << "------------ \n";

  for(int i = 0; i < nsystematic; i++){
    datacard << Form( "%s \t lnN \t", systematics[i].Data()); 
    for( int j = 1; j < nprocess; j++ ){
      datacard << Form( "%7.3f \t", yield[i][j]);
    }
    datacard << "\n"; 
  }

  datacard.close();
}
