#include "TCut.h"
#include "TH1F.h"

const TString storageSite     = "/eos/user/j/jgarciaf/minitrees/"; 
//const TString storageSite     = "/eos/user/c/cprieels/minitrees/"; 
const TString datacard_dir    = "/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/";



const TString theproduction   = "carlota"; 
const TString histoSite       = "histos/loyola_AN-updating_fig-6-7/"; 


//const TString datacard_folder = "mierda/";   // "work/";
const TString datacard_folder = "carlota_blinded/";   // "work/";

const TString histoSite_datacards = datacard_dir + datacard_folder; //"histos/loyola_sync/"; 


///////////////////////////////////
      const bool doshape = 0; 
///////////////////////////////////


const float inicio = 0.00;
const float paso   = 1.00;

const float thelumi = 35.867/15.; 

const float xs2l = 1.0/9; 

const float    ttSF = 1.00;  const float ettSF = 0.073;
const float    DYSF = 1.04;  const float eDYSF = 0.26;   // 7% + 25%
			     const float efakes= 0.30;
                             const float ettV  = 0.20;

const float epileup = 0.046; 
const float elumi   = 0.025;


//---- Omaha --- 
//const float    ttSF = 0.91;  const float ettSF = 0.05;
//const float    DYSF = 0.827;  const float eDYSF = 0.04;
//			      const float efakes= 0.30;

//const float    ttSF = 0.97;  const float ettSF = 0.15;
//const float    DYSF = 1.07;  const float eDYSF = 0.07;
//			       const float efakes= 0.30;



//const TCut selection = "metPfType1>80.&&mt2ll>100.&&channel==5";  // sync
const TCut selection = "(channel==3&&metPfType1>50.)||(channel==4&&metPfType1>50.)||(channel==5&&metPfType1>0.)";//&&darkpt>0."; // hard
//const TCut selection = "(channel==3&&metPfType1>80.)||(channel==4&&metPfType1>80.)||(channel==5&&metPfType1>0.)&&mt2ll>80.&&darkpt>-5000";//&&darkpt>0."; // hard
//const TCut selection = "lep1id*lep2id<0&&metPfType1>50.&&mt2ll>50.";//&&darkpt>0."; // hard
//const TCut selection = "((channel==3||channel==4)&&metPfType1>50.)||channel==5";   // DESY
//const TCut selection = "metPfType1>50.&&mt2ll<110.";//&&channel==5";   // NU
//const TCut selection = "( (channel==3||channel==4) && metPfType1>50. ) || ( channel==5 && metPfType1>0. )";
//const TCut selection = "( metPfType1>0. )";


// loyola
const TCut RemovingFakes = "eventW_genmatched";

// jefferson
//const TCut RemovingFakes = "eventW_truegenmatched&&eventW_genmatched";

// fucking-mum
//const TCut RemovingFakes = "eventW_genMatched && ( abs(lep1mid)==24 || abs(lep1mid)==15 ) && ( abs(lep2mid)==24 || abs(lep2mid)==15 )"; 


const TString region = "SR";



enum{ data,
      ttDM,
      fakes,
      ST,
      TTV,
      DY,
      //DYTT,
      //TT1,
      //TT2,
      //TTSemi,
      TT,
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

enum{ lnN, shape, nsysttype };

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
	toppTrwup,
	toppTrwdo,
	DDtt,
	DDDY,
	DDfakes,
	DDttV,
	pileup,
	luminosity,
	nsystematic }; 

enum{ nlepton,
      lep1pt, lep1eta, lep1phi, lep1mass,
      lep2pt, lep2eta, lep2phi, lep2mass, 
      jet1pt, jet1eta, jet1phi, jet1mass,
      jet2pt, jet2eta, jet2phi, jet2mass,
      metPfType1, metPfType1Phi,
      m2l, mt2ll, mt2lblb, mtw1, mtw2,
      htjets, htnojets,
      njet, nbjet30csvv2l, nbjet30csvv2m, nbjet30csvv2t,  
      dphijet1met, dphijet2met, dphijj, dphijjmet, dphill, dphilep1jet1, dphilep1jet2, dphilep2jet1, dphilep2jet2, dphilmet1, dphilmet2,dphillmet,	
      nvtx, 
      darkpt,
      ANN,
      nhisto };

TCut eventW[nsystematic];  

TString processID  [nprocess];
TString processID2 [nprocess];
TString minitreeDir[nsystematic];
TString scalarID   [nscalar];
float  scalarMVAcut[nscalar]; 
TString  pseudoID  [npseudo ];
TString         systematicID[nsystematic];
TString systematicIDdatacard[nsystematic];
TString systematicIDdatacard0[nsystematic];
TString systtypeID[nsysttype];

TString b_name[nhisto];
TH1F* myhisto [nhisto];
int    nbinraw[nhisto];
float   lowraw[nhisto];
float    upraw[nhisto];

void Assign(){

	//----------

	processID[ttDM ] = "ttDM0001scalar00500"     ;   //     tune !
	processID[data ] = "01_Data_1outof15"        ;
	//processID[data ] = "01_Data_Full2016"        ;
	//processID[fakes] = "00_Fakes_1outof15"       ; 
	processID[fakes] = "00_Fakes_Full2016"       ; 
	processID[TT   ] = "04_TTTo2L2Nu"        ; 
	//processID[TT1  ] = "04_TTTo2L2Nu_1"          ;
	//processID[TT2  ] = "04_TTTo2L2Nu_2"          ;
 	//processID[TTSemi]= "04_TTToSemiLepton"       ;
	processID[ST   ] = "05_ST"                   ; 
	processID[DY   ] = "07_ZJets"                ; 
	//processID[DYTT ] = "07_ZJetsTT"              ; 
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

	processID2[ttDM ] = "signal"  ;  
	processID2[data ] = "data_obs";
	processID2[fakes] = "fakes"   ; 
	processID2[TT   ] = "TT"      ; 
	processID2[ST   ] = "ST"      ; 
	processID2[DY   ] = "DY"      ; 
	processID2[TTV  ] = "TTV"     ; 
	processID2[WW   ] = "WW"      ; 
	processID2[WZ   ] = "WZ"      ;
	processID2[VZ   ] = "VZ"      ; 
	processID2[VVV  ] = "VVV"     ; 

	///scalarMVAcut[ttDM0001scalar00010] = 0.74; 
	///scalarMVAcut[ttDM0001scalar00020] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00050] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00100] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00200] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00300] = 0.50; 
	///scalarMVAcut[ttDM0001scalar00500] = 0.98; 

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
	systematicID[toppTrwup] = "toppTrwup";
	systematicID[toppTrwdo] = "toppTrwdo";
	systematicID[DDtt     ] = "DDtt"     ;
	systematicID[DDDY     ] = "DDDY"     ;
	systematicID[DDfakes  ] = "DDfakes"  ;
	systematicID[DDttV    ] = "DDttV"    ;
	systematicID[pileup   ] = "pileup"   ;
	systematicID[luminosity]= "lumi"     ;

	systematicIDdatacard[nominal  ] = "";
	systematicIDdatacard[Btagup   ] = "BtagUp";
	systematicIDdatacard[Btagdo   ] = "BtagDown";
	systematicIDdatacard[Idisoup  ] = "IdisoUp";
	systematicIDdatacard[Idisodo  ] = "IdisoDown";
	systematicIDdatacard[Triggerup] = "TriggerUp";
	systematicIDdatacard[Triggerdo] = "TriggerDown";
	systematicIDdatacard[METup    ] = "METUp";
	systematicIDdatacard[METdo    ] = "METDown";
	systematicIDdatacard[JESup    ] = "JESUp";
	systematicIDdatacard[JESdo    ] = "JESDown";
	systematicIDdatacard[EleESup  ] = "EleESUp";
	systematicIDdatacard[EleESdo  ] = "EleESDown";
	systematicIDdatacard[MuESup   ] = "MuESUp";
	systematicIDdatacard[MuESdo   ] = "MuESDown";
	systematicIDdatacard[QCDup    ] = "QCDUp";
	systematicIDdatacard[QCDdo    ] = "QCDDown";
	systematicIDdatacard[PDFup    ] = "PDFUp";
	systematicIDdatacard[PDFdo    ] = "PDFDown";
	systematicIDdatacard[toppTrwup] = "toppTrwUp";
	systematicIDdatacard[toppTrwdo] = "toppTrwDown";
	systematicIDdatacard[DDtt     ] = "DDtt"   ;
	systematicIDdatacard[DDDY     ] = "DDDY"   ;
	systematicIDdatacard[DDfakes  ] = "DDfakes";
	systematicIDdatacard[DDttV    ] = "DDttV"  ;
	systematicIDdatacard[pileup   ] = "pileup" ;
	systematicIDdatacard[luminosity]= "lumi"   ;

	systematicIDdatacard0[nominal  ] = "";
	systematicIDdatacard0[Btagup   ] = "Btag"   ;
	systematicIDdatacard0[Btagdo   ] = ""       ;
	systematicIDdatacard0[Idisoup  ] = "Idiso"  ;
	systematicIDdatacard0[Idisodo  ] = ""       ;
	systematicIDdatacard0[Triggerup] = "Trigger";
	systematicIDdatacard0[Triggerdo] = ""       ;
	systematicIDdatacard0[METup    ] = "MET";
	systematicIDdatacard0[METdo    ] = "";
	systematicIDdatacard0[JESup    ] = "JES";
	systematicIDdatacard0[JESdo    ] = "";
	systematicIDdatacard0[EleESup  ] = "EleES";
	systematicIDdatacard0[EleESdo  ] = "";
	systematicIDdatacard0[MuESup   ] = "MuES";
	systematicIDdatacard0[MuESdo   ] = "";
	systematicIDdatacard0[QCDup    ] = "QCD"    ;
	systematicIDdatacard0[QCDdo    ] = ""       ;
	systematicIDdatacard0[PDFup    ] = "PDF"    ;
	systematicIDdatacard0[PDFdo    ] = ""       ;
	systematicIDdatacard0[toppTrwup] = "toppTrw";
	systematicIDdatacard0[toppTrwdo] = "";
	systematicIDdatacard0[DDtt     ] = "DDtt"   ;
	systematicIDdatacard0[DDDY     ] = "DDDY"   ;
	systematicIDdatacard0[DDfakes  ] = "DDfakes";
	systematicIDdatacard0[DDttV    ] = "DDttV"  ;
	systematicIDdatacard0[pileup   ] = "pileup" ;
	systematicIDdatacard0[luminosity]= "lumi"   ;

	systtypeID[lnN  ] = "lnN";
	systtypeID[shape] = "shape";

	//----------

	eventW[nominal  ] = "eventW"          ;
	eventW[Btagup   ] = "eventW_Btagup"   ;
	eventW[Btagdo   ] = "eventW_Btagdo"   ;
	eventW[Idisoup  ] = "eventW_Idisoup"  ;
	eventW[Idisodo  ] = "eventW_Idisodo"  ;
	eventW[Triggerup] = "eventW_Triggerup";
	eventW[Triggerdo] = "eventW_Triggerdo";
	eventW[METup    ] = "eventW"          ;
	eventW[METdo    ] = "eventW"          ;
	eventW[JESup    ] = "eventW_Btagup*1.01";
	eventW[JESdo    ] = "eventW_Btagdo*0.99";
	eventW[EleESup  ] = "eventW"          ;
	eventW[EleESdo  ] = "eventW"          ;
	eventW[MuESup   ] = "eventW"          ;
	eventW[MuESdo   ] = "eventW"          ;
	eventW[QCDup    ] = "eventW"          ;
	eventW[QCDdo    ] = "eventW"          ;
	eventW[PDFup    ] = "eventW"          ;
	eventW[PDFdo    ] = "eventW"          ;
	eventW[toppTrwup] = "eventW"          ;
	eventW[toppTrwdo] = "eventW"          ;


	for( int i = 0; i < nsystematic; i++ ){

		minitreeDir[i] = theproduction;

		if( i >= METup && i <= METdo ) minitreeDir[i] = minitreeDir[i] + "_" + systematicID[i];

		//if( i >= METup && i <= MuESdo ) minitreeDir[i] = minitreeDir[i] + "_" + systematicID[i];

	}      


	//----------     

	b_name[nlepton]  = "nlepton" ;		         nbinraw[nlepton]      =  10;	lowraw[nlepton]      =   0. ;	upraw[nlepton]      =  10. ;

 	b_name[lep1pt  ] = "lep1pt"  ;		         nbinraw[lep1pt  ]     =3000;	lowraw[lep1pt  ]     =   0. ;	upraw[lep1pt  ]     =3000. ;
	b_name[lep1eta ] = "lep1eta" ;		         nbinraw[lep1eta ]     =  60;	lowraw[lep1eta ]     =  -3. ;	upraw[lep1eta ]     =   3. ;
	b_name[lep1phi ] = "lep1phi" ;		         nbinraw[lep1phi ]     = 200;	lowraw[lep1phi ]     =  -3.2;	upraw[lep1phi ]     =   3.2;
	b_name[lep1mass] = "lep1mass";		         nbinraw[lep1mass]     = 100;	lowraw[lep1mass]     =   0. ;	upraw[lep1mass]     = 100. ;

 	b_name[lep2pt  ] = "lep2pt"  ;		         nbinraw[lep2pt  ]     =3000;	lowraw[lep2pt  ]     =   0. ;	upraw[lep2pt  ]     =3000. ;	
	b_name[lep2eta ] = "lep2eta" ;		         nbinraw[lep2eta ]     =  60;	lowraw[lep2eta ]     =  -3. ;	upraw[lep2eta ]     =   3. ;
	b_name[lep2phi ] = "lep2phi" ;		         nbinraw[lep2phi ]     = 200;	lowraw[lep2phi ]     =  -3.2;	upraw[lep2phi ]     =   3.2;
	b_name[lep2mass] = "lep2mass";		         nbinraw[lep2mass]     = 100;	lowraw[lep2mass]     =   0. ;	upraw[lep2mass]     = 100. ;

	b_name[jet1pt  ] = "jet1pt"  ;		         nbinraw[jet1pt  ]     =3000;	lowraw[jet1pt  ]     =   0. ;	upraw[jet1pt  ]     =3000. ;
	b_name[jet1eta ] = "jet1eta" ;		         nbinraw[jet1eta ]     =  80;	lowraw[jet1eta ]     =  -4. ;	upraw[jet1eta ]     =   4. ;
	b_name[jet1phi ] = "jet1phi" ;		         nbinraw[jet1phi ]     = 200;	lowraw[jet1phi ]     =  -3.2;	upraw[jet1phi ]     =   3.2;
	b_name[jet1mass] = "jet1mass";		         nbinraw[jet1mass]     = 100;	lowraw[jet1mass]     =   0. ;	upraw[jet1mass]     = 100. ;

	b_name[jet2pt  ] = "jet2pt"  ;		         nbinraw[jet2pt  ]     =3000;	lowraw[jet2pt  ]     =   0. ;	upraw[jet2pt  ]     =3000. ;
	b_name[jet2eta ] = "jet2eta" ;		         nbinraw[jet2eta ]     =  80;	lowraw[jet2eta ]     =  -4. ;	upraw[jet2eta ]     =   4. ;
	b_name[jet2phi ] = "jet2phi" ;		         nbinraw[jet2phi ]     = 200;	lowraw[jet2phi ]     =  -3.2;	upraw[jet2phi ]     =   3.2;
	b_name[jet2mass] = "jet2mass";		         nbinraw[jet2mass]     = 100;	lowraw[jet2mass]     =   0. ;	upraw[jet2mass]     = 100. ;

	b_name[metPfType1   ] = "metPfType1"   ;         nbinraw[metPfType1   ]=3000;	lowraw[metPfType1   ]=   0. ;	upraw[metPfType1   ]=3000. ;
	b_name[metPfType1Phi] = "metPfType1Phi";         nbinraw[metPfType1Phi]= 200;	lowraw[metPfType1Phi]=  -3.2;	upraw[metPfType1Phi]=   3.2;

	b_name[m2l    ] = "m2l"    ;                     nbinraw[m2l    ]      =3000;	lowraw[m2l    ]      =   0. ;	upraw[m2l    ]      =3000. ;
	b_name[mt2ll  ] = "mt2ll"  ;                     nbinraw[mt2ll  ]      =3000;	lowraw[mt2ll  ]      =   0. ;	upraw[mt2ll  ]      =3000. ;        
	b_name[mt2lblb] = "mt2lblb";                     nbinraw[mt2lblb]      =3000;	lowraw[mt2lblb]      =   0. ;	upraw[mt2lblb]      =3000. ;
	b_name[mtw1   ] = "mtw1"   ;                     nbinraw[mtw1   ]      =3000;	lowraw[mtw1   ]      =   0. ;	upraw[mtw1   ]      =3000. ;
	b_name[mtw2   ] = "mtw2"   ;                     nbinraw[mtw2   ]      =3000;	lowraw[mtw2   ]      =   0. ;	upraw[mtw2   ]      =3000. ;

	b_name[htjets  ] = "htjets"  ;                   nbinraw[htjets  ]     =3000;	lowraw[htjets  ]     =   0. ;	upraw[htjets  ]     =3000. ;
	b_name[htnojets] = "htnojets";                   nbinraw[htnojets]     =3000;	lowraw[htnojets]     =   0. ;	upraw[htnojets]     =3000. ;

	b_name[njet         ] = "njet"         ;         nbinraw[njet         ]=  10;	lowraw[njet         ]=   0. ;	upraw[njet         ]=  10. ;
	b_name[nbjet30csvv2l] = "nbjet30csvv2l";         nbinraw[nbjet30csvv2l]=  10;	lowraw[nbjet30csvv2l]=   0. ;	upraw[nbjet30csvv2l]=  10. ;
	b_name[nbjet30csvv2m] = "nbjet30csvv2m";         nbinraw[nbjet30csvv2m]=  10;	lowraw[nbjet30csvv2m]=   0. ;	upraw[nbjet30csvv2m]=  10. ;
	b_name[nbjet30csvv2t] = "nbjet30csvv2t";         nbinraw[nbjet30csvv2t]=  10;	lowraw[nbjet30csvv2t]=   0. ;	upraw[nbjet30csvv2t]=  10. ;

	b_name[dphijet1met ] = "dphijet1met" ;           nbinraw[dphijet1met ] = 100;	lowraw[dphijet1met ] =   0. ;	upraw[dphijet1met ] =   3.2;   
	b_name[dphijet2met ] = "dphijet2met" ;           nbinraw[dphijet2met ] = 100;	lowraw[dphijet2met ] =   0. ;	upraw[dphijet2met ] =   3.2;   
	b_name[dphijj      ] = "dphijj"      ;           nbinraw[dphijj      ] = 100;	lowraw[dphijj      ] =   0. ;	upraw[dphijj      ] =   3.2;     
	b_name[dphijjmet   ] = "dphijjmet"   ;           nbinraw[dphijjmet   ] = 100;	lowraw[dphijjmet   ] =   0. ;	upraw[dphijjmet   ] =   3.2;    
	b_name[dphill      ] = "dphill"      ;           nbinraw[dphill      ] = 100;	lowraw[dphill      ] =   0. ;	upraw[dphill      ] =   3.2;    
	b_name[dphilep1jet1] = "dphilep1jet1";           nbinraw[dphilep1jet1] = 100;	lowraw[dphilep1jet1] =   0. ;	upraw[dphilep1jet1] =   3.2; 
	b_name[dphilep1jet2] = "dphilep1jet2";           nbinraw[dphilep1jet2] = 100;	lowraw[dphilep1jet2] =   0. ;	upraw[dphilep1jet2] =   3.2; 
	b_name[dphilep2jet1] = "dphilep2jet1";           nbinraw[dphilep2jet1] = 100;	lowraw[dphilep2jet1] =   0. ;	upraw[dphilep2jet1] =   3.2;
	b_name[dphilep2jet2] = "dphilep2jet2";           nbinraw[dphilep2jet2] = 100;	lowraw[dphilep2jet2] =   0. ;	upraw[dphilep2jet2] =   3.2; 
	b_name[dphilmet1   ] = "dphilmet1"   ;           nbinraw[dphilmet1   ] = 100;	lowraw[dphilmet1   ] =   0. ;	upraw[dphilmet1   ] =   3.2; 
	b_name[dphilmet2   ] = "dphilmet2"   ;           nbinraw[dphilmet2   ] = 100;	lowraw[dphilmet2   ] =   0. ;	upraw[dphilmet2   ] =   3.2; 
	b_name[dphillmet   ] = "dphillmet"   ;           nbinraw[dphillmet   ] = 100;	lowraw[dphillmet   ] =   0. ;	upraw[dphillmet   ] =   3.2; 

	b_name[nvtx] = "nvtx";                           nbinraw[nvtx]         =  60;	lowraw[nvtx]         =   0. ;	upraw[nvtx]         =  60. ;

	b_name[darkpt] = "darkpt";                       nbinraw[darkpt]       = 1300;	lowraw[darkpt]       =-10000.;	upraw[darkpt]       =3000. ;

	b_name[ANN] = "ANN_tanh_mt2ll80_camille_" + processID[ttDM];   
                                                         nbinraw[ANN]          = 120;   lowraw[ANN]          =  -0.1;   upraw[ANN]          =   1.1;

	//----------

}
