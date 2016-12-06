

rootfile  = new TFile ("<Path_to_rootfile>",  "read");

const int nsample = 12;
const int nsystematics = 2;

TString samples[nsample] = {"02_WZTo3LNu.root", "03_VZ.root", "04_TTTo2L2Nu.root", "05_ST.root", "06_WW.root", "07_ZJets.root", "10_HWW.root", "11_Wg.root", "12_Zg.root", "13_VVV.root", "14_HZ.root", "15_WgStar.root"};
TString systematics[nsystematics] = {"nominal", "Idisoup"};

float yields[nsample][nsystematics]; 

void getYields() {
  
  for(int i = 0; i < nsample; i++) {
    for(int j = 0; j < nsystematics; j++) {

      TString histo_name = Form(%s_%s, samples[i], systematics[i]);
      TH1D* histo = (TH1D*)rootfile -> Get(histo_name);
      
      yields[samples[i]][systematics[j]] = histo -> GetBinContent(-1, -1);
    
    }
  }

}

ofstream datacard;
datacard.open("datacards/test.txt");
  
datacard << "imax 1 number of channels \n" ;
datacard << Form("jmax %d number of backgrounds \n", nsample-2);
datacard << Form("kmax %d number of nuisance parameters \n", nsystematics);
datacard << "------------ \n");

datacard << "\n");

datacard << Form("observation %5.0f \n", yields[data][nominal]);
datacard << "------------ \n");

datacard << "bin        \t     \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1   \t   1 \n" ;
datacard << "process    \t     \t  DM   \t  TT   \t  ST   \t  WW   \t  DY   \t fakes \t  WZ   \t  ZZ   \t  TTV  \t  Wg   \t  HZ \n" ;
datacard << "process    \t     \t   0   \t   1   \t   2   \t   3   \t   4   \t   5   \t   6   \t   7   \t   8   \t   9   \t  10 \n" ;

datacard.close();
