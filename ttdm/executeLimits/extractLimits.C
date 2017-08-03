#include "../ttdm.h"

void extractLimits2( int j, TString signalID );


void extractLimits(){

	Assign();

	for( int s = 0; s < nscalar; s++){
 
		extractLimits2( s, scalarID[s] ); 

	}
	
	for( int p = 0; p < npseudo; p++){
 
		extractLimits2( p, pseudoID[p] ); 

	}

}

void extractLimits2( int j, TString signalID ){

double limit; 
double mh; 
float  quantileExpected;

TFile* f_limit = new TFile( datacard_dir + datacard_folder + signalID + "_camille_combined_optimisation.root", "read");

TTree* t_limit = (TTree*) f_limit -> Get( "limit" );

t_limit -> SetBranchAddress( "limit"           , &limit           );
t_limit -> SetBranchAddress( "mh"              , &mh              );
t_limit -> SetBranchAddress( "quantileExpected", &quantileExpected);

int nentries = t_limit-> GetEntries(); 

double cutOpt = 100; 
double limitOpt = 1e3; 

cout << "\n\t" << signalID << endl;
cout << "\t--------------" << endl; 
 
for( int i = 0; i < nentries; i++ ){

	t_limit -> GetEntry(i);

	float cut = mh - 100*(j+1); 

	if ( quantileExpected != 0.5 ) continue;

	//cout << "\t\t" << cut << "\t" << limit << endl; 

	if( limit < limitOpt ){ cutOpt = cut; limitOpt = limit; } 
}

cout << "\n\t the best:  cut = " << cutOpt << "\t exp. limit = " << limitOpt << "\n" << endl; 

f_limit -> Close(); 

}
