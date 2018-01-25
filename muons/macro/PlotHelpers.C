////////////////////////////////////////////////////////////
//
// A group of helper functions used to compare histograms
// and profiles between two different releases
//
////////////////////////////////////////////////////////////

/////
// Some includes
#include "TH1F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include <vector>
#include "PlotHelpers.h"
#include "MuonStyle.h"

using namespace std;
/////
// Uncomment the following line to get more debuggin output
//#define DEBUG

////////////////////////////////////////////////////////////
//
// Sets the global style for the whole system.
//
// Note: There might be redundancies in other places so
//       things can be further simplified
//


//
// cout<<"hello"<<endl;}
//PlotHelpers::~PlotsHelpers(){}


//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function sets the style for a given histogram
//
// Note: There might be redundancies in other places so
//       things can be further simplified
//


void PlotHelpers::SetHistogramStyle(TH1* h, Style_t mstyle, Color_t color, Size_t msize = 0.7, Width_t lwidth = 2,
		       Float_t tsize = 0.05, Float_t toffset = 1.2) {
  if (!h)
    return;
  h->SetMarkerStyle(mstyle);
  h->SetMarkerColor(color);
  h->SetMarkerSize(msize);
  h->SetLineColor(color);
  h->SetLineWidth(lwidth);
  h->GetYaxis()->SetTitleSize(tsize);
  h->GetYaxis()->SetTitleOffset(toffset);
  return;
}
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function finds the list of TDirectories in a branch
// that match a given string

TList* PlotHelpers::GetListOfDirectories(TDirectory* dir, const char* match = 0) {
 
  TIter  nextkey(dir->GetListOfKeys());
  TList* sl     = new TList();
  TKey*  key    = (TKey*)nextkey();
  //TKey*  oldkey = 0;


    TObject *obj1 = key->ReadObj();



    //  while ( key == (TKey*)nextkey() ) {
  while ( key ) {
    
    TObject *obj = key->ReadObj();

    if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      TString theName = obj->GetName();

      if (!match) {
	cout << " -> " << theName << endl;
	sl->Add(obj);
      }
      else if (theName.Contains(match)) {
	cout << " -> " << theName << endl;
	sl->Add(obj);
      }
    }
    key = (TKey*)nextkey();
  } // End of while

  return sl;
}

void PlotHelpers::SetGlobalStyle(){

  //  R__LOAD_LIBRARY(MuonStyle.C);

  MuonStyle testStyle; 

  TStyle * style = testStyle.getStyle("Muon");
  //style->SetMarkerSize(0.8);
  style->cd();
  style->SetNdivisions(508, "X");
  style->SetMarkerSize(0.8);
  style->SetPadGridX(kTRUE);
  style->SetPadGridY(kTRUE);  
  style->SetGridStyle(3);
  style->SetGridWidth(1);
  style->SetOptTitle(2);
  style->SetTitleFont(42);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.04);
  style->SetPadRightMargin(0.07);
  style->SetPadLeftMargin(0.13);
  style->SetTitleXSize(0.07);
  style->SetTitleXOffset(0.6);
  style->SetTitleYSize(0.3);
  style->SetTitleY(0.99);
  //  return;
}


TList* PlotHelpers::GetListOfBranches(const char* dataType, TFile* file) {
  if (TString(dataType) == "HLT") {
    if(file->cd("DQMData/Run 1/HLT"))
      file->cd("DQMData/Run 1/HLT/Run summary/Muon/MultiTrack");
    else 
      file->cd("DQMData/HLT/Muon/MultiTrack");
  }
  else if (TString(dataType) == "RECO") {
    if(file->cd("DQMData/Run 1/RecoMuonV")) 
      file->cd("DQMData/Run 1/RecoMuonV/Run summary/MultiTrack");
    else if(file->cd("DQMData/Run 1/Muons/Run summary/RecoMuonV")) 
      file->cd("DQMData/Run 1/Muons/Run summary/RecoMuonV/MultiTrack");
    else 
      file->cd("DQMData/RecoMuonV/MultiTrack");
  }
  else {
    cout << "ERROR: Data type " << dataType << " not allowed: only RECO and HLT are considered" << endl;
    return 0;
  }

  TDirectory * dir=gDirectory;
  TList* sl = GetListOfDirectories(dir);
  
  if (sl->GetSize() == 0) {
    cout << "ERROR: No DQM muon reco histos found in NEW file " << endl;
    delete sl;
    return 0;
  }
  
  return sl;
}

////////////////////////////////////////////////////////////
//
// This function goes to the right branch inside the file
// looking for branches having branchContent.
// It returns a list with all those branches,
//
TList* PlotHelpers::getListOfBranches(const char* dataType, TFile* file, const char* branchContent) {
  /*
  if (dataType == "HLT") {
    if(file->cd("DQMData/Run 1/HLT")) {
      file->cd("DQMData/Run 1/HLT/Run summary/Muon/MultiTrack");
    }
    else {
      file->cd("DQMData/HLT/Muon/MultiTrack");
    }
  }
  else if (dataType == "RECO") {
    if(file->cd("DQMData/Run 1/RecoMuonV")) {
      file->cd("DQMData/Run 1/RecoMuonV/Run summary/MultiTrack");
    }
    else if(file->cd("DQMData/Run 1/Muons/Run summary/RecoMuonV")) {
      file->cd("DQMData/Run 1/Muons/Run summary/RecoMuonV/MultiTrack");
    }
    else {
      file->cd("DQMData/RecoMuonV/MultiTrack");
    }
  }
  else {
    cout << "ERROR: Data type " << dataType << " not allowed: only RECO and HLT are considered" << endl;
    cerr << "ERROR: Data type " << dataType << " not allowed: only RECO and HLT are considered" << endl;
    return;
  }
  */
  
  if (TString(dataType) == "RECO") {
    if(! file->cd("DQMData/Run 1/Muons/Run summary")) {
      cout << "ERROR: Muon Histos for " << dataType << " not found" << endl;
      return 0;
    }
  }
  else {
    cout << "ERROR: Data type " << dataType << " not allowed: only RECO is considered" << endl;
    return 0;
  }

  TDirectory * dir=gDirectory;
  TList* sl = GetListOfDirectories(dir, branchContent);


  if (sl->GetSize() == 0) {
    cout << "ERROR: No DQM muon reco histos found in NEW file " << endl;
    delete sl;
    return 0;
  }

  return sl;
}
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function performs a compatibility test between two
// histogram based on the Kolmogorov-Smirnof algorithm. It
// also prints the value in a TPaveLabel at the upper-right
// corner.
// The return value contains the result of the test
//
double PlotHelpers::KolmogorovTest(TH1 *h1, TH1 *h2){

  double mya_array[1300], myb_array[1300];
  vector<double> mya;
  vector<double> myb;
  
  
  for (int i=0; i<h1->GetNbinsX(); i++){
    mya.push_back(h1->GetBinContent(i+1));
    myb.push_back(h2->GetBinContent(i+1));
  }

  sort(mya.begin(),mya.end());
  sort(myb.begin(),myb.end()); 
  copy(mya.begin(),mya.end(),mya_array);
  copy(myb.begin(),myb.end(),myb_array);
  
  const int nbinsa = h1->GetNbinsX();
  const int nbinsb = h2->GetNbinsX();
   
  double kstest = TMath::KolmogorovTest(nbinsa, mya_array,
					nbinsb, myb_array,
					"UOX");
#ifdef DEBUG
  cout << "DEBUG:   + KS value = " << kstest << endl;
#endif

  if (kstest > 0.05) {

    // Create text with the value
    TString legend = Form("KS=%4.2f", kstest);
    
    // Create a pave text to put the value inside
    
    TPaveLabel* pl = new TPaveLabel(0.79,0.91,0.93,0.96, legend.Data(), "NDC");
    
    // Tune style
    //pl->SetTextSize(0.04);
    pl->SetLineColor(41);
    pl->SetLineWidth(1);
    pl->SetLineStyle(1);
    pl->SetFillColor(41);
    pl->SetBorderSize(3);

    if (kstest < 0.7) pl->SetTextColor(kRed);
    
    pl->Draw();
  }
  return kstest;
}
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// This function draws the stat box for the two plots
//
void PlotHelpers::setStats(TH1* s, TH1* r, double startingY, double startingX = .1, bool fit = false){
  if (startingY<0){
    s->SetStats(0);
    r->SetStats(0);
  } 
  else {
    //gStyle->SetOptStat(1001);
    
    if (fit){
      s->Fit("gaus");
      TF1* f1 = (TF1*) s->GetListOfFunctions()->FindObject("gaus");
      if (f1) {
	f1->SetLineColor(2);
	f1->SetLineWidth(1);
      }
    }
    s->Draw();
    gPad->Update(); 
    TPaveStats* st1 = (TPaveStats*) s->GetListOfFunctions()->FindObject("stats");
    if (st1) {
      if (fit) {st1->SetOptFit(0010);    st1->SetOptStat(1001);}
      st1->SetX1NDC(startingX);
      st1->SetX2NDC(startingX+0.30);
      st1->SetY1NDC(startingY+0.20);
      st1->SetY2NDC(startingY+0.35);
      st1->SetTextColor(2);
    }
    else s->SetStats(0);
    if (fit) {
      r->Fit("gaus");
      TF1* f2 = (TF1*) r->GetListOfFunctions()->FindObject("gaus");
      if (f2) {
	f2->SetLineColor(4);
	f2->SetLineWidth(1);
      }
    }
    r->Draw();
    gPad->Update(); 
    TPaveStats* st2 = (TPaveStats*) r->GetListOfFunctions()->FindObject("stats");
    if (st2) {
      if (fit) {st2->SetOptFit(0010);    st2->SetOptStat(1001);}
      st2->SetX1NDC(startingX);
      st2->SetX2NDC(startingX+0.30);
      st2->SetY1NDC(startingY);
      st2->SetY2NDC(startingY+0.15);
      st2->SetTextColor(4);
    }
    else r->SetStats(0);
  }
}
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// Plot a page with several histograms
//
void PlotHelpers::PlotNHistograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     unsigned int nhistos, const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0, bool resol = false,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0                     
		     ) {

#ifdef DEBUG
  cout << "   + Plotting histograms for " << canvasTitle << endl;
  cerr << "   + Plotting histograms for " << canvasTitle << endl;
#endif
  TH1* rh   = 0;
  TH1* sh   = 0;
#ifdef DEBUG
  cout << "DEBUG: Creating canvas..." << endl;
#endif

  TCanvas* canvas =  0;
  if (nhistos >4)
    canvas = new TCanvas(canvasName, canvasTitle, 1000, 1400);
  else
    canvas = new TCanvas(canvasName, canvasTitle, 1000, 1050);
  canvas->Divide(2,(nhistos+1)/2); //This way we print in 2 columns

#ifdef DEBUG
  cout << " [macro/PlotHelpers.C] DEBUG: Looping over histograms" << endl;
#endif

  for (unsigned int i = 0; i < nhistos; i++) {

#ifdef DEBUG
    cout << "DEBUG: [" << i << "] histogram name: " << flush << hnames[i] << endl;
    cout << "DEBUG: rdir:  " << rdir << endl;
    cout << "DEBUG: sdir:  " << sdir << endl;
    cout << "DEBUG: rcollname: " << rcollname << endl;
    cout << "DEBUG: scollname: " << scollname << endl;
    cout << "DEBUG: rname: " << (rcollname + "/" + hnames[i]) << endl;
    cout << "DEBUG: sname: " << (scollname + "/" + hnames[i]) << endl;
#endif

    TString hnamesi = hnames[i];
    
    // Skip histogram if no name is provided
    if (hnamesi == "") continue;
    
    // Get Histograms
    // + Reference

    TString hnames_tmp = hnamesi;
    rdir->cd(rcollname);
    TIter next2(gDirectory->GetListOfKeys());
    TKey *key2= (TKey*)next2();

    while (key2 ) { 
    
      TObject *obj = key2->ReadObj();
      TString temp = obj->GetName();
      //cout << "Obje name= " << temp << "Key: class name= " << key->GetClassName() << endl;
      if ( (temp.Contains("nhits_vs_eta_pfx")) &&
	   hnamesi.Contains("hits_eta")
	 ) {
	hnames_tmp = temp;      
      }

      if ( (temp.Contains("chi2_vs_eta_pfx")) &&
	   hnamesi.Contains("chi2mean") 
	 ) {
	hnames_tmp = temp;      
      }
      key2= (TKey*)next2();
    } 
    

    
#ifdef DEBUG
    cout << "DEBUG: Getting object reference samples " << (rcollname + "/" + hnames_tmp) << " from " << rdir << endl;
#endif

    rdir->GetObject(rcollname + "/" + hnames_tmp, rh);
    if (! rh) {
      cout << " [PlotHelpers.C] WARNING -- Could not find a reference histogram or profile named " << hnames_tmp.Data() 
	   << " in " << rdir->GetName() << endl;
      continue;
    }

    // If it is a 2D project it in Y... is this what we always want?
    if (TString(rh->IsA()->GetName()) == "TH2F") {
#ifdef DEBUG
      cout << "DEBUG: It is a TH2F object... project in Y!" << endl;
#endif
      TH1* proj = ((TH2F*) rh)->ProjectionY();
      rh = proj;
    }


    // + Signal
    
    hnames_tmp = hnamesi;
    
    sdir->cd(scollname);

    TIter next1(gDirectory->GetListOfKeys());
    TKey *key1= (TKey*)next1();;

    while (key1) { 
    
      //      key1 = 
      TObject *obj = key1->ReadObj();
      TString temp1 = obj->GetName();



      if ( (temp1.Contains("nhits_vs_eta_pfx")) &&
	   hnamesi.Contains("hits_eta")
	   ) {
	hnames_tmp = temp1;      
      }


      if ( (temp1.Contains("chi2_vs_eta_pfx")) &&
	   hnamesi.Contains("chi2mean")
	   ) {
	hnames_tmp = temp1;      
      }
      key1= (TKey*)next1();
    } 



#ifdef DEBUG
    cout << "DEBUG: Getting object for selected sample " << (scollname + "/" + hnames_tmp) 
	 << " from " << sdir << endl;
#endif
    sdir->GetObject(scollname + "/" + hnames_tmp, sh);
    if (! sh) {
      cout << " [PlotHelpers.C] WARNING -- Could not find a signal histogram or profile named " << hnames_tmp.Data() 
	   << " in " << rdir->GetName() << endl;
      continue;
    }

    //If it is a 2D project it in Y... is this what we always want?
    if (TString(sh->IsA()->GetName()) == "TH2F") {
#ifdef DEBUG
      cout << "DEBUG: " << hnames_tmp[i] << " is a TH2F object... project in Y!" << endl;
#endif
      TH1* proj = ((TH2F*) sh)->ProjectionY();
      sh = proj;
    }

    if(sh->IsA()->InheritsFrom("TProfile"))norm[i]=-999.;

   


    // Normalize
#ifdef DEBUG
    cout << "Normalizing to = "<< norm[i] << endl; 
#endif
    if (norm[i] == -1.)
      NormalizeHistogramsTo1(rh, sh);
    else if (norm[i] == 0.){ 
      NormalizeHistogramsToFirst(rh,sh);
      }
    else if (norm[i] == -999.){
      //cout << "DEBUG: Normalizing histograms to nothing" << "..." << endl;
    }



    // Set styles
#ifdef DEBUG
    cout << "DEBUG: Setting styles..." << endl;
#endif
    SetHistogramStyle(rh, 21, 4);
    SetHistogramStyle(sh, 20, 2);
    //Change titles
    if (htitles) {
      rh->SetTitle(htitles[i]);
      sh->SetTitle(htitles[i]);
      rh->GetYaxis()->SetTitle(htitles[i]);
      sh->GetYaxis()->SetTitle(htitles[i]);
    }



    //Change x axis range
    if (minx) {

      //      if (minx < -1E99) {

      	rh->GetXaxis()->SetRangeUser(minx[i],maxx[i]);
	sh->GetXaxis()->SetRangeUser(minx[i],maxx[i]);

	//}
    }

    //Change y axis range
    if (miny) {

	rh->GetYaxis()->SetRangeUser(miny[i],maxy[i]);
	sh->GetYaxis()->SetRangeUser(miny[i],maxy[i]);

    }



    // Move to subpad
    canvas->cd(i+1);


    TPad* pad1 = NULL;
    TPad* pad2 = NULL;
    
    pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.);
    pad2 = new TPad("pad2", "pad2", 0, 0.06, 1, 0.27);

    pad1->SetTopMargin   (0.01);
    pad1->SetBottomMargin(0.1);
    pad1->Draw();

    
    pad2->SetTopMargin   (0.0);
    pad2->SetBottomMargin(0.1);
    pad2->Draw();


    pad1->cd();
    // Check  Logy
    if (logy) {
      if (logy[i])	gPad->SetLogy();
    }

    // Set stat boxes
#ifdef DEBUG
    cout << "DEBUG: Setting statistics..." << endl;
#endif
    setStats(sh, rh, -1, 0, false);
    
    // Draw histogram
#ifdef DEBUG
    cout << "DEBUG: Drawing histograms..." << endl;
#endif
    if (sh->GetMaximum() > rh->GetMaximum()) {
      sh->Draw();
      rh->Draw("sames");
    }
    else {
      rh->Draw();
      sh->Draw("sames");
    }
    
    // Perform Kolmogorov test if needed
    if (doKolmo) {
#ifdef DEBUG
      cout << "DEBUG: Performing Kolmogorov test..." << endl;
#endif
      if (doKolmo[i]) {
	//	TPad* c1_1 = canvas->GetPad(i+1);
	double kstest = KolmogorovTest(sh,rh);
	if(kstest>0.05 && kstest<0.7) gPad->SetFillColor(kBlue-10);
      }
    }
    pad2->cd();
    const char* rationame = "ratioPlot";
    TH1F* ratioplot = PlotRatiosHistograms(rh, sh, rationame); //     
      ratioplot->Draw();

  }
  // End loop


   // Draw Legend
#ifdef DEBUG
  cout << "DEBUG: Drawing legend..." << endl;
#endif
  canvas->cd();
  
  TLegend* l = 0;
  if (nhistos > 4)
    l = new TLegend(0.10,0.665,0.90,0.69);
  else
    l = new TLegend(0.10,0.50,0.90,0.535);

  l->SetTextSize(0.011);
  l->SetLineColor(1);
  l->SetLineWidth(1);
  l->SetLineStyle(1);
  l->SetFillColor(0);
  l->SetBorderSize(2);
  l->AddEntry(rh,refFileLabel,"LPF");
  l->AddEntry(sh,newFileLabel,"LPF");
  l->Draw();
  
  // Print Canvas
  canvas->Print(pdfFile);

  // Clean memory
  // delete l;
  delete canvas;
  //  cout << "     ... plotted histograms for " << canvasTitle << endl;
}
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Plot a page with 4 histograms
//
void PlotHelpers::Plot4Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol = 0,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0){

  PlotNHistograms(pdfFile,
		  rdir, sdir,
		  rcollname, scollname,
		  canvasName, canvasTitle,
		  refLabel, newLabel,
		  refFileLabel,newFileLabel,
		  4, hnames, htitles,
		  logy, doKolmo, norm,resol,minx,maxx,miny,maxy);
}
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Plot a page with 6 histograms
//
void PlotHelpers::Plot6Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol=false,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0
		     ) {
  

  PlotNHistograms(pdfFile,
		  rdir, sdir,
		  rcollname, scollname,
		  canvasName, canvasTitle,
		  refLabel, newLabel,
		  refFileLabel, newFileLabel,
		  6, hnames, htitles,
		  logy, doKolmo, norm,resol,
		  minx, maxx,
		  miny, maxy);
}
//
////////////////////////////////////////////////////////////

void PlotHelpers::Plot5Histograms(const TString& pdfFile,
		     TDirectory* rdir, TDirectory* sdir,
		     const TString& rcollname, const TString& scollname,
		     const char* canvasName, const char* canvasTitle,
		     const TString& refLabel, const TString& newLabel,
		     const TString& refFileLabel, const TString& newFileLabel,
		     const char** hnames, const char** htitles = 0,
		     bool* logy = 0, bool* doKolmo = 0, Double_t* norm = 0,bool resol=false,
		     Double_t *minx = 0, Double_t *maxx = 0,
		     Double_t *miny = 0, Double_t *maxy = 0) {
  

  PlotNHistograms(pdfFile,
		  rdir, sdir,
		  rcollname, scollname,
		  canvasName, canvasTitle,
		  refLabel, newLabel,
		  refFileLabel,newFileLabel,
		  5, hnames, htitles,
		  logy, doKolmo, norm,resol,
		  minx, maxx,
		  miny, maxy);
}
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Normalize the two histograms to the entries on the first histogram
//
void PlotHelpers::NormalizeHistogramsToFirst(TH1* h1, TH1* h2) {
  if (h1==0 || h2==0) return;
  //cout << h1->Integral() << " " << h2->Integral() << endl;
  if ( h1->Integral() > 0 && h2->Integral() > 0 ){
    //cout << h1->Integral() << " " << h2->Integral() << endl;
    //cout << h1->Integral(0,-1) << " " << h2->Integral(0,-1) << endl;
    Double_t scale2 = double(h1->Integral(0,-1)/h2->Integral(0,-1));
    h2->Scale(scale2);
  }
}
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//
// Normalize the two histograms to unity
//
/////////////////////////////////////////////////////////
void PlotHelpers::NormalizeHistogramsTo1(TH1* h1, TH1* h2) {
  if (!h1 || !h2) return;
  
  if ( h1->Integral() != 0 && h2->Integral() != 0 ) {
    Double_t scale1 = 1.0/h1->Integral();
    Double_t scale2 = 1.0/h2->Integral();
    h1->Scale(scale1);
    h2->Scale(scale2);
  }
}


////////////////////////////////////////////////////////////
//
// ratio plot from the two histograms
//
/////////////////////////////////////////////////////////

TH1F* PlotHelpers::PlotRatiosHistograms(TH1* signal, TH1* back, const char *collname){ 
 

    
  //  TH1F *rat_h1 = new TH1F(Form("RAT_%s_h1",collname), "", nbins, binmin, binmax);  rat_h1->Sumw2();

  TH1F* ratio = (TH1F*)signal->Clone("ratio");
  TH1F* uncertainty = (TH1F*)back->Clone("uncertainty");
   

  for (Int_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {
    
    Float_t dtValue = signal->GetBinContent(ibin);
    Float_t dtError = signal->GetBinError(ibin);
    
    Float_t mcValue = back->GetBinContent(ibin);
    Float_t mcError = back->GetBinError(ibin);
    
    Float_t ratioVal         = 999;
    Float_t ratioErr         = 999;
    Float_t uncertaintyError = 999;
    
    if (mcValue > 0)
      {
	
	ratioVal         = dtValue / mcValue;
	ratioErr         = dtError / mcValue;
     uncertaintyError = ratioVal * mcError / mcValue;
     
      }
    
    ratio->SetBinContent(ibin, ratioVal);
    ratio->SetBinError  (ibin, ratioErr);
    
    uncertainty->SetBinContent(ibin, 1.);
    uncertainty->SetBinError  (ibin, uncertaintyError);
  }
  
  
  //ratio->SetFillStyle(3001);
  /*  ratio->SetLineWidth(1);
      ratio->SetLineWidth(1);
      ratio->SetFillColor(  kRed+1);
      ratio->SetLineColor(  kRed+1);
      ratio->SetMarkerColor(kRed+1);
  */

  //  ratio->GetXaxis()->SetRangeUser(0, 10000.);
  ratio->GetYaxis()->SetRangeUser(0.7, 1.3);  
  
  
  //  SetAxis(ratio, "", "data / MC", 1.4, 0.75);
  
  return  ratio;
}    
