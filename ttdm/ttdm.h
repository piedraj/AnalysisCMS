#include "TCut.h"
#include "TH1F.h"
//const TString  inputdir = "sanIsidoro";  // where the minitrees are stored
//const TString  inputdir = "Helsinki_LepElepTCutup";  // where the minitrees are stored

const TString storageSite   = "/eos/user/j/jgarciaf/minitrees/"; 

const TString theproduction = "Helsinki"; 

const float thelumi = 35.9/15; 

const float xs2l = 1.0/9; 

const float PUrw = 1.030108192;

const float    ttSF = 1.0;  const float ettSF = 0.15;
const float    DYSF = 1.0;  const float eDYSF = 0.07;
			    const float efakes= 0.30;

//const float    ttSF = 0.97;  const float ettSF = 0.15;
//const float    DYSF = 1.07;  const float eDYSF = 0.07;
//			       const float efakes= 0.30;

const bool doshape = false; 

const TCut selection = "metPfType1>0.&&mt2ll>0."; 


const TCut soft_cut = "metPfType1>80."; 
const TCut hard_cut = soft_cut&&"mt2ll>80.&&darkpt>0."; 
TCut MVA_cut;

const float inicio = 0.00;
const float paso   = 0.02;


enum{ data,
      ttDM,
      fakes,
      TT,
      ST,
      DY,
      TTV,
      WW,
      WZ, 
      VZ,
      VVV,
      Wg,
      Zg,
      //HWW,
      //HZ,
      //ttDMlight,
      //ttDMheavy,
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

enum{ soft, hard, NN, nlevel };

enum{ nrmlz, shape, nsysttype };

enum{ 	nominal, 
	Btagup, 
	Btagdo, 
	Idisoup, 
	Idisodo, 
	Triggerup, 
	Triggerdo,
        METup,
        METdo, 
        JESup,
        JESdo, 
        EleESup,
	EleESdo,
        MuESup,
        MuESdo,
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
      m2l, mt2ll, mt2lblb, mtw1, mtw2,
      ht, htjets, htnojets,
      njet, nbjet30csvv2l, nbjet30csvv2m, nbjet30csvv2t,  
      dphijet1met, dphijet2met, dphijj, dphijjmet, dphill, dphilep1jet1, dphilep1jet2, dphilep2jet1, dphilep2jet2, dphilmet1, dphilmet2, dphillmet,	
      top1eta_gen, top1phi_gen, top1pt_gen, top2eta_gen, top2phi_gen, top2pt_gen, detatt_gen,  
      nvtx, //ntrueint,
      //scale, uPara, uPerp,
      //sphericity, alignment, planarity,
      darkpt,
      MVAtanh, MVAsigm,	
      nhisto };

TCut eventW[nsystematic];  

TString processID  [nprocess];
TString minitreeDir[nsystematic];
TString scalarID   [nscalar];
float  scalarMVAcut[nscalar]; 
TString  pseudoID  [npseudo ];
TString         systematicID[nsystematic];
TString systematicIDdatacard[nsystematic];
TString systtypeID[nsysttype];

TString b_name[nhisto];
TString g_name[nhisto];
TH1F* myhisto [nhisto];



void Assign(){

	//----------

	processID[ttDM ] = "ttDM0001scalar00010"     ;   //     tune !
	processID[data ] = "01_Data_1outof15"        ;
	processID[fakes] = "00_Fakes_1outof15"       ; 
	processID[TT   ] = "04_TTTo2L2Nu"            ; 
	processID[ST   ] = "05_ST"                   ; 
	processID[DY   ] = "07_ZJets"                ; 
	processID[TTV  ] = "09_TTV"                  ; 
	processID[WW   ] = "06_WW"                   ; 
	processID[WZ   ] = "02_WZTo3LNu"             ; 
	processID[VZ   ] = "03_VZ"                   ; 
	processID[VVV  ] = "13_VVV"                  ; 
	processID[Wg   ] = "11_Wg"                   ; 
	processID[Zg   ] = "12_Zg"                   ; 
	//processID[HWW  ] = "10_HWW"                  ; 
	//processID[HZ   ] = "14_HZ"                   ;
	//processID[ttDMlight] = "ttDM0001scalar00010" ; 
	//processID[ttDMheavy] = "ttDM0001scalar00500" ; 


	scalarMVAcut[ttDM0001scalar00010] = 0.74; 
	///scalarMVAcut[ttDM0001scalar00020] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00050] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00100] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00200] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00300] = 0.50; 
	scalarMVAcut[ttDM0001scalar00500] = 0.98; 

	scalarID[ttDM0001scalar00010] = "ttDM0001scalar00010"; 
	scalarID[ttDM0001scalar00020] = "ttDM0001scalar00020"; 
	scalarID[ttDM0001scalar00050] = "ttDM0001scalar00050"; 
	scalarID[ttDM0001scalar00100] = "ttDM0001scalar00100"; 
	scalarID[ttDM0001scalar00200] = "ttDM0001scalar00200"; 
	scalarID[ttDM0001scalar00300] = "ttDM0001scalar00300"; 
	scalarID[ttDM0001scalar00500] = "ttDM0001scalar00500";

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
	systematicID[METup    ] = "METup"    ;
	systematicID[METdo    ] = "METdo"    ;
	systematicID[JESup    ] = "JESup"    ;
	systematicID[JESdo    ] = "JESdo"    ;
	systematicID[EleESup  ] = "LepElepTCutup";
	systematicID[EleESdo  ] = "LepElepTCutdo";
	systematicID[MuESup   ] = "LepMupTCutup" ;
	systematicID[MuESdo   ] = "LepMupTCutdo" ;
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
	systematicIDdatacard[METup    ] = "MET";
	systematicIDdatacard[METdo    ] = "";
	systematicIDdatacard[JESup    ] = "JES";
	systematicIDdatacard[JESdo    ] = "";
	systematicIDdatacard[EleESup  ] = "EleES";
	systematicIDdatacard[EleESdo  ] = "";
	systematicIDdatacard[MuESup   ] = "MuES";
	systematicIDdatacard[MuESdo   ] = "";
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

	eventW[nominal  ] = "eventW"          *selection;
	eventW[Btagup   ] = "eventW_Btagup"   *selection;
	eventW[Btagdo   ] = "eventW_Btagdo"   *selection;
	eventW[Idisoup  ] = "eventW_Idisoup"  *selection;
	eventW[Idisodo  ] = "eventW_Idisodo"  *selection;
	eventW[Triggerup] = "eventW_Triggerup"*selection;
	eventW[Triggerdo] = "eventW_Triggerdo"*selection;
	eventW[METup    ] = "eventW"          *selection;
	eventW[METdo    ] = "eventW"          *selection;
	eventW[JESup    ] = "eventW"          *selection;
	eventW[JESdo    ] = "eventW"          *selection;
	eventW[EleESup  ] = "eventW"          *selection;
	eventW[EleESdo  ] = "eventW"          *selection;
	eventW[MuESup   ] = "eventW"          *selection;
	eventW[MuESdo   ] = "eventW"          *selection;
	eventW[QCDup    ] = "eventW"          *selection;
	eventW[QCDdo    ] = "eventW"          *selection;
	eventW[PDFup    ] = "eventW"          *selection;
	eventW[PDFdo    ] = "eventW"          *selection;
	eventW[toppTrw  ] = "eventW"          *selection;



	for( int i = 0; i < nsystematic; i++ ){

		minitreeDir[i] = theproduction;

		if( i >= METup && i <= MuESdo ) minitreeDir[i] = minitreeDir[i] + "_" + systematicID[i];

	} 


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
	b_name[mtw1   ] = "mtw1"   ;
	b_name[mtw2   ] = "mtw2"   ;

	b_name[ht      ] = "ht"      ;
	b_name[htjets  ] = "htjets"  ;
	b_name[htnojets] = "htnojets";

	b_name[njet         ] = "njet"         ;
	b_name[nbjet30csvv2l] = "nbjet30csvv2l";
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
	//b_name[ntrueint    ] = "ntrueint"    ;

	//b_name[scale       ] = "scale"        ;
	//b_name[uPara       ] = "uPara"        ;
	//b_name[uPerp       ] = "uPerp"        ;
	//b_name[sphericity] = "sphericity";
	//b_name[alignment ] = "alignment" ;
	//b_name[planarity ] = "planarity" ;

	b_name[darkpt    ] = "darkpt";
	b_name[MVAtanh] = "ANN_tanh_mt2ll80_regina_ttDM0001scalar00010";
	b_name[MVAsigm] = "ANN_sigm_mt2ll80_regina_ttDM0001scalar00010";


	for( int i = 0; i < nhisto; i++ ){

 		g_name[i] = b_name[i];

	}

	//----------

}
