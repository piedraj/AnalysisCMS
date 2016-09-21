/**
Original Author Lars Sonnenschein <sonne@lpnhep.in2p3.fr>

Minor modifcations by Dan Boline <ddboline@fnal.gov>

Further minor modifications by Aram Avetisyan <aram.avetisyan@cern.ch>
Debug by Thomas Speer.
 */
#ifndef MassSolver_class_h
#define MassSolver_class_h
#include "TLorentzVector.h"
#include "TVector2.h"

#include<iostream>
#include <vector>

using namespace std;

class MassSolver 
{
    public:
  
        MassSolver();
        ~MassSolver() {};//{cout << "MassSolver_class: destructor executing!" << endl;};

        bool solve( const TVector2 & ETmiss , const TLorentzVector & b , const TLorentzVector & bbar , const TLorentzVector & lep1 , const TLorentzVector & lep2 , double mW1 , double mW2 , double mt1 , double mt2 , vector<TLorentzVector> & nu1 , vector<TLorentzVector> & nu2 );

        void solve(double* ETmiss, double* b, double* bb, double* lp, double* lm, 
                   double mWp, double mWm, double mt, double mtb, 
                   vector<double> *pnux, vector<double> *pnuy, vector<double> *pnuz, 
                   vector<double> *pnubx, vector<double> *pnuby, vector<double> *pnubz,
                   vector<double> *cd_diff, int& cubic_single_root_cmplx);

        void quartic(vector<double> const & poly, vector<double> *pnuy, int& cubic_single_root_cmplx);
        void cubic(vector<double> const & poly, vector<double> *pnuy);
        void quadratic(vector<double> const & poly, vector<double> *pnuy);
        int algebraic_pz(double* b, double* lp, double mWp, double mt, double mb, double mlp, 
                         double pnux, double pnuy, double* pnuz);
        double evalterm1(vector<double> *a1, double pnux, double pnuy);
        double evalterm2(vector<double> *a2, double pnux, double pnuy);
  
    private: 
  
        const double epsilon = 1.e-6; //numerical precision
	bool flagProb;
	bool debug;

	inline double sqr(double const & x) const {return x*x;}

	inline double sign(double const & a) const {return (a < 0) ? -1 : (a > 0) ? 1 : 0;}

	inline double quad(double const & x) const {return x*x*x*x;}

	void Print4Vector(TLorentzVector const &v1){
	  printf("| %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f |", v1.Pt(), v1.Eta(), v1.Phi(),  v1.E() ,v1.M(), v1.Px() , v1.Py() , v1.Pz());
	}
	void PrintVector(const char s[5], TLorentzVector const & v1){
	  printf("%sx= %f;\n", s,v1.Px());
	  printf("%sy= %f;\n", s,v1.Py());
	  printf("%sz= %f;\n", s,v1.Pz());
	  printf("e%s= %f;\n", s,v1.E());
	  printf("m%s= %f;\n", s,v1.M());

	}
	void PrintEtmiss(TVector2 const & v1){
	  printf("Etmisx = %f;\n", v1.Px());
	  printf("Etmisy = %f;\n", v1.Py());
	}

}; //end class MassSolver  

#endif

#ifndef PI
#define PI fabs(acos(-1.))
#endif
