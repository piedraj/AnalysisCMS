#include "TCut.h"

//const TString  inputdir = "/gpfs/csic_projects/tier3data/LatinosSkims/RunII/2016/Stop/minitrees/nominal/Stop/";  // where the minitrees are stored
const TString  inputdir = "../minitrees/nominal/Stop/";  // where the minitrees are stored

const float thelumi = 9.983; 
const float    ttSF = 1.;  const float ettSF = 0.0;
const float    DYSF = 1.;  const float eDYSF = 0.0;
			     const float efakes= 0.00;

const bool doshape = false; 


  //const TCut selection= "1>0";
  const TCut selection= "channel == 5";
  //const TCut selection= "channel == 5 && njet < 1";

//const TCut soft_cut = "metPfType1>80. && njet > 1 && leadingPtCSVv2M > 20."; 
/*const TCut hard_cut = soft_cut&&"mt2ll>100.&&darkpt>0."; 
//const TCut  MVA_cut = hard_cut&&"ANN_met80_mt2ll100_ttDM0001scalar00500>0.50";
TCut MVA_cut;
*
*/

enum{ data,
      TT,
      ST,
      DY,
      TTZ,
      TTW,
      WW,
      WZ, 
      VZ,
      VVV,
      nprocess
}; 

enum{ ttDM0001scalar00010, 
      ttDM0001scalar00020, 
      ttDM0001scalar00050, 
      ttDM0001scalar00100, 
      ttDM0001scalar00200, 
      ttDM0001scalar00300, 
      ttDM0001scalar00500, 
      nscalar
};

enum{ ttDM0001pseudo00010, 
      ttDM0001pseudo00020, 
      ttDM0001pseudo00050, 
      ttDM0001pseudo00100, 
      ttDM0001pseudo00200, 
      ttDM0001pseudo00300, 
      ttDM0001pseudo00500, 
      npseudo
};

enum{ ee, mm, em, ll, nchannel }; 

enum{ soft, hard, MVA, nlevel };

enum{ nrmlz, shape, nsysttype };

enum{ 	nominal, 
	Btagup, 
	Btagdo, 
	Idisoup, 
	Idisodo, 
	Triggerup, 
	Triggerdo, 
	QCDup,
	QCDdo,
	PDFup,
	PDFdo,
	toppTrw,
	DDtt,
	DDDY,
	DDfakes,
	nsystematic }; 

enum{ lep1pt, lep1eta, lep1phi, lep1mass,
      lep2pt, lep2eta, lep2phi, lep2mass,
      jet1pt, jet1eta, jet1phi, jet1mass,
      jet2pt, jet2eta, jet2phi, jet2mass,
      metPfType1, metPfType1Phi,
      m2l, mt2ll, mt2lblb, /*mtw1, mtw2,
      ht, htjets, htnojets,*/
      njet, /*nbjet30csvv2l, nbjet30csvv2m, nbjet30csvv2t,  
      dphijet1met, dphijet2met, dphijj, dphijjmet, dphill, dphilep1jet1, dphilep1jet2, dphilep2jet1, dphilep2jet2, dphilmet1, dphilmet2, dphillmet,	
      top1eta_gen, top1phi_gen, top1pt_gen, top2eta_gen, top2phi_gen, top2pt_gen, detatt_gen,  
      nvtx,
      sphericity, alignment, planarity,
      //darkpt,
      //mva01,*/
      nhisto };

TCut mycut[nsystematic];  

TString processID[nprocess];
TString   scalarID[nscalar];	
float scalarMVAcut[nscalar]; 
TString  pseudoID[npseudo ];
TString         systematicID[nsystematic];
TString systematicIDdatacard[nsystematic];
TString systtypeID[nsysttype];

TString b_name[nhisto];
TString g_name[nhisto];
TH1F* myhisto [nhisto];



void Assign(){

	//----------

	processID[data ] = "01_Data"                 ;
	processID[TT   ] = "04_TTTo2L2Nu"            ; 
	processID[ST   ] = "05_ST"                   ; 
	processID[DY   ] = "07_ZJets"                ; 
	processID[TTW  ] = "09_TTW"                  ; 
	processID[WW   ] = "06_WW"                   ; 
	processID[WZ   ] = "02_WZTo3LNu"             ; 
	processID[VZ   ] = "03_VZ"                   ; 
	processID[VVV  ] = "13_VVV"                  ; 
	processID[TTZ  ] = "10_TTZ"                  ; 

	scalarID[ttDM0001scalar00010] = "ttDM0001scalar00010"; 
	scalarID[ttDM0001scalar00020] = "ttDM0001scalar00020"; 
	scalarID[ttDM0001scalar00050] = "ttDM0001scalar00050"; 
	scalarID[ttDM0001scalar00100] = "ttDM0001scalar00100"; 
	scalarID[ttDM0001scalar00200] = "ttDM0001scalar00200"; 
	scalarID[ttDM0001scalar00300] = "ttDM0001scalar00300"; 
	scalarID[ttDM0001scalar00500] = "ttDM0001scalar00500"; 

	scalarMVAcut[ttDM0001scalar00010] = 0.45; 
	scalarMVAcut[ttDM0001scalar00020] = 0.50; 
	scalarMVAcut[ttDM0001scalar00050] = 0.50; 
	scalarMVAcut[ttDM0001scalar00100] = 0.50; 
	scalarMVAcut[ttDM0001scalar00200] = 0.50; 
	scalarMVAcut[ttDM0001scalar00300] = 0.50; 
	scalarMVAcut[ttDM0001scalar00500] = 0.50; 

	pseudoID[ttDM0001pseudo00010] = "ttDM0001pseudo00010"; 
	pseudoID[ttDM0001pseudo00020] = "ttDM0001pseudo00020"; 
	pseudoID[ttDM0001pseudo00050] = "ttDM0001pseudo00050"; 
	pseudoID[ttDM0001pseudo00100] = "ttDM0001pseudo00100"; 
	pseudoID[ttDM0001pseudo00200] = "ttDM0001pseudo00200"; 
	pseudoID[ttDM0001pseudo00300] = "ttDM0001pseudo00300"; 
	pseudoID[ttDM0001pseudo00500] = "ttDM0001pseudo00500"; 

	//----------

	systematicID[nominal  ] = "nominal"  ;
	systematicID[Btagup   ] = "Btagup"   ;
	systematicID[Btagdo   ] = "Btagdo"   ;
	systematicID[Idisoup  ] = "Idisoup"  ;
	systematicID[Idisodo  ] = "Idisodo"  ;
	systematicID[Triggerup] = "Triggerup";
	systematicID[Triggerdo] = "Triggerdo";
	systematicID[QCDup    ] = "QCDup"    ;
	systematicID[QCDdo    ] = "QCDdo"    ;
	systematicID[PDFup    ] = "PDFup"    ;
	systematicID[PDFdo    ] = "PDFdo"    ;
	systematicID[toppTrw  ] = "toppTrw"  ;

	systematicIDdatacard[nominal  ] = "nominal";
	systematicIDdatacard[Btagup   ] = "Btag"   ;
	systematicIDdatacard[Btagdo   ] = ""       ;
	systematicIDdatacard[Idisoup  ] = "Idiso"  ;
	systematicIDdatacard[Idisodo  ] = ""       ;
	systematicIDdatacard[Triggerup] = "Trigger";
	systematicIDdatacard[Triggerdo] = ""       ;
	systematicIDdatacard[QCDup    ] = "QCD"    ;
	systematicIDdatacard[QCDdo    ] = ""       ;
	systematicIDdatacard[PDFup    ] = "PDF"    ;
	systematicIDdatacard[PDFdo    ] = ""       ;
	systematicIDdatacard[toppTrw  ] = "toppTrw";
	systematicIDdatacard[DDtt     ] = "DDtt"   ;
	systematicIDdatacard[DDDY     ] = "DDDY"   ;
	systematicIDdatacard[DDfakes  ] = "DDfakes";

	systtypeID[nrmlz] = "nrmlz";
	systtypeID[shape] = "shape";

	//----------

	mycut[nominal  ] = "eventW"          *selection;
	mycut[Btagup   ] = "eventW_Btagup"   *selection;
	mycut[Btagdo   ] = "eventW_Btagdo"   *selection;
	mycut[Idisoup  ] = "eventW_Idisoup"  *selection;
	mycut[Idisodo  ] = "eventW_Idisodo"  *selection;
	mycut[Triggerup] = "eventW_Triggerup"*selection;
	mycut[Triggerdo] = "eventW_Triggerdo"*selection;
	mycut[QCDup    ] = "eventW"          *selection;
	mycut[QCDdo    ] = "eventW"          *selection;
	mycut[PDFup    ] = "eventW"          *selection;
	mycut[PDFdo    ] = "eventW"          *selection;
	mycut[toppTrw  ] = "eventW"          *selection;

	//----------

 	b_name[lep1pt  ] = "lep1pt"  ;
	b_name[lep1eta ] = "lep1eta" ;
	b_name[lep1phi ] = "lep1phi" ;
	b_name[lep1mass] = "lep1mass";

 	b_name[lep2pt  ] = "lep2pt"  ;
	b_name[lep2eta ] = "lep2eta" ;
	b_name[lep2phi ] = "lep2phi" ;
	b_name[lep2mass] = "lep2mass";

	b_name[jet1pt  ] = "jet1pt"  ;
	b_name[jet1eta ] = "jet1eta" ;
	b_name[jet1phi ] = "jet1phi" ;
	b_name[jet1mass] = "jet1mass";

	b_name[jet2pt  ] = "jet2pt"  ;
	b_name[jet2eta ] = "jet2eta" ;
	b_name[jet2phi ] = "jet2phi" ;
	b_name[jet2mass] = "jet2mass";

	b_name[metPfType1   ] = "metPfType1";
	b_name[metPfType1Phi] = "metPfType1Phi";

	b_name[m2l    ] = "m2l"    ;
	b_name[mt2ll  ] = "mt2ll"  ;
	b_name[mt2lblb] = "mt2lblb";
/*	b_name[mtw1   ] = "mtw1"   ;
	b_name[mtw2   ] = "mtw2"   ;

	b_name[ht      ] = "ht"      ;
	b_name[htjets  ] = "htjets"  ;
	b_name[htnojets] = "htnojets";

*/	b_name[njet         ] = "njet"         ;
/*	b_name[nbjet30csvv2l] = "nbjet30csvv2l";
	b_name[nbjet30csvv2m] = "nbjet30csvv2m";
	b_name[nbjet30csvv2t] = "nbjet30csvv2t";

	b_name[dphijet1met ] = "dphijet1met" ;   
	b_name[dphijet2met ] = "dphijet2met" ;  
	b_name[dphijj      ] = "dphijj"      ;    
	b_name[dphijjmet   ] = "dphijjmet"   ;   
	b_name[dphill      ] = "dphill"      ;   
	b_name[dphilep1jet1] = "dphilep1jet1";
	b_name[dphilep1jet2] = "dphilep1jet2";
	b_name[dphilep2jet1] = "dphilep2jet1";
	b_name[dphilep2jet2] = "dphilep2jet2";
	b_name[dphilmet1   ] = "dphilmet1"   ;
	b_name[dphilmet2   ] = "dphilmet2"   ;
	b_name[dphillmet   ] = "dphillmet"   ;

	b_name[top1eta_gen ] = "top1eta_gen" ;
	b_name[top1phi_gen ] = "top1phi_gen" ;
	b_name[top1pt_gen  ] = "top1pt_gen"  ;
	b_name[top2eta_gen ] = "top2eta_gen" ;
	b_name[top2phi_gen ] = "top2phi_gen" ;
	b_name[top2pt_gen  ] = "top2pt_gen"  ;
	b_name[detatt_gen  ] = "detatt_gen"  ;

	b_name[nvtx        ] = "nvtx"        ;

	b_name[sphericity] = "sphericity";
	b_name[alignment ] = "alignment" ;
	b_name[planarity ] = "planarity" ;
*/
	//b_name[darkpt    ] = "newdarkpt";
	//b_name[mva01     ] = "ANN_mt2ll0_ttDM0001scalar00010";

	for( int i = 0; i < nhisto; i++ ){

 		g_name[i] = b_name[i];

	}

	//----------

}
