#include "TFile.h"
#include "TTree.h"
#include "TList.h"
#include <fstream>
#include <iostream>
/* =============================================================
   
               	***Instructions***

  TTree *tree1, *tree2, *tree3; //pointers to your 3 Trees
  TList *list - new TList;
  list->Add(tree1);
  list->Add(tree2);
  list->Add(tree3);
  TTree *newtree = TTree::MergeTrees(list);
  newtree->SetName("newtree");
  newtree->Write();

 ================================================================*/

void merging_TTrees() {
cout <<"start"<<endl;


/*
//  T2tt_mStop up to 150  Sample


   TFile *Stop_part0 = new TFile( "../../minitrees/nominal/Stop/T2tt_mStop-150to250.root", "read");
   TFile *Stop_part1 = new TFile( "../../minitrees/nominal/Stop/T2tt_mStop-250to350.root", "read");
   TFile *Stop_part2 = new TFile( "../../minitrees/nominal/Stop/T2tt_mStop-350to400.root", "read");
   TFile *Stop_part3 = new TFile( "../../minitrees/nominal/Stop/T2tt_mStop-400to1200.root","read");

   TTree  *Stop_tt0 = (TTree*)Stop_part0->Get("latino");
   TTree  *Stop_tt1 = (TTree*)Stop_part1->Get("latino");
   TTree  *Stop_tt2 = (TTree*)Stop_part2->Get("latino");
   TTree  *Stop_tt3 = (TTree*)Stop_part3->Get("latino");
   cout <<"the list"<<endl;

   TList *Stop_myList = new TList();
   Stop_myList->Add(Stop_tt0);
   Stop_myList->Add(Stop_tt1);
   Stop_myList->Add(Stop_tt2);
   Stop_myList->Add(Stop_tt3);

   cout <<"The Tfile recreate"<<endl;  
   TFile *Stop_YY = new TFile("../../minitrees/nominal/Stop/T2tt_mStop-150to1200.root", "recreate");
   TTree *Stop_newtree = TTree::MergeTrees(Stop_myList);
   Stop_newtree->SetName("latino");
   Stop_newtree->Write();
   cout <<"Close"<<endl;  
   Stop_part0->Close(); 
   Stop_part1->Close(); 
   Stop_part2->Close(); 
   Stop_part3->Close(); 

*/
//  TTTo2L2Nu_ext1  Sample
   
   TFile *ST_part0 = new TFile("minitrees/nominal/Stop/ST_tW_antitop.root", "read");
   TFile *ST_part1 = new TFile("minitrees/nominal/Stop/ST_tW_top.root", "read");

   TTree  *ST_tt0 = (TTree*)ST_part0->Get("latino");
   TTree  *ST_tt1 = (TTree*)ST_part1->Get("latino");

   cout <<"the list"<<endl;  
  
   TList *ST_myList = new TList();
   ST_myList->Add(ST_tt0);
   ST_myList->Add(ST_tt1);
  
   cout <<"The Tfile recreate"<<endl;  
   TFile *ST_YY = new TFile("minitrees/nominal/Stop/ST_tW.root", "recreate");
   TTree *ST_newtree = TTree::MergeTrees(ST_myList);
   ST_newtree->SetName("latino");
   ST_newtree->Write();
   cout <<"Close"<<endl;  
   ST_part0->Close(); 
   ST_part1->Close(); 
 



//  TTTo2L2Nu_ext1  Sample


   TFile *part0 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part0.root", "read");
   TFile *part1 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part1.root", "read");
   TFile *part2 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part2.root", "read");
   TFile *part3 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part3.root", "read");
   TFile *part4 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part4.root", "read");
   TFile *part5 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part5.root", "read");
   TFile *part6 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part6.root", "read");
   TFile *part7 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part7.root", "read");
   TFile *part8 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part8.root", "read");
   TFile *part9 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part9.root", "read");
   TFile *part10 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part10.root", "read");
   TFile *part11 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part11.root", "read");
   TFile *part12 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part12.root", "read");
   TFile *part13= new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part13.root", "read");
   TFile *part14 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part14.root", "read");
   TFile *part15 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part15.root", "read");
   TFile *part16 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part16.root", "read");
   TFile *part17 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part17.root", "read");
   TFile *part18 = new TFile("minitrees/nominal/Stop/TTTo2L2Nu__part18.root", "read");

  TTree  *tt0 = (TTree*)part0->Get("latino");
  TTree  *tt1 = (TTree*)part1->Get("latino");
  TTree  *tt2 = (TTree*)part2->Get("latino");
  TTree  *tt3 = (TTree*)part3->Get("latino");
  TTree  *tt4 = (TTree*)part4->Get("latino");
  TTree  *tt5 = (TTree*)part5->Get("latino");
  TTree  *tt6 = (TTree*)part6->Get("latino");
  TTree  *tt7 = (TTree*)part7->Get("latino");
  TTree  *tt8 = (TTree*)part8->Get("latino");
  TTree  *tt9 = (TTree*)part9->Get("latino");
  TTree  *tt10 = (TTree*)part10->Get("latino");
  TTree  *tt11 = (TTree*)part11->Get("latino");
  TTree  *tt12 = (TTree*)part12->Get("latino");
  TTree  *tt13 = (TTree*)part13->Get("latino");
  TTree  *tt14 = (TTree*)part14->Get("latino");
  TTree  *tt15 = (TTree*)part15->Get("latino");
  TTree  *tt16 = (TTree*)part16->Get("latino");
  TTree  *tt17 = (TTree*)part17->Get("latino");
  TTree  *tt18 = (TTree*)part18->Get("latino");
  cout <<"the list"<<endl;  
  
  TList *myList = new TList();
  myList->Add(tt0);
  myList->Add(tt1);
  myList->Add(tt2);
  myList->Add(tt3);
  myList->Add(tt4);
  myList->Add(tt5);
  myList->Add(tt6);
  myList->Add(tt7);
  myList->Add(tt8);
  myList->Add(tt9);
  myList->Add(tt10);
  myList->Add(tt11);
  myList->Add(tt12);
  myList->Add(tt13);
  myList->Add(tt14);
  myList->Add(tt15);
  myList->Add(tt16);
  myList->Add(tt17);
  myList->Add(tt18);
  
  //myList->Write("MiLista")
  cout <<"The Tfile recreate"<<endl;  
  TFile *YY = new TFile("minitrees/nominal/Stop/TTTo2L2Nu_ext1.root", "recreate");
  TTree *newtree = TTree::MergeTrees(myList);
  newtree->SetName("latino");
  newtree->Write();
  cout <<"Close"<<endl;  
  part0->Close(); 
  part1->Close(); 
  part2->Close(); 
  part3->Close(); 
  part4->Close(); 
  part5->Close(); 
  part6->Close(); 
  part7->Close(); 
  part8->Close(); 
  part9->Close();
  part10->Close();
  part11->Close();
  part12->Close();
  part13->Close();
  part14->Close();
  part15->Close();
  part16->Close();
  part17->Close();
  part18->Close();

/*
  
 // DYJetsToLL Sample
 
  TFile *DY_part0  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0000__part0.root", "read");
  TFile *DY_part1  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0000__part1.root", "read");
  TFile *DY_part2  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0000__part2.root", "read");
  TFile *DY_part3  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0000__part3.root", "read");
  TFile *DY_part4  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0000__part4.root", "read");
  TFile *DY_part5  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0001__part0.root", "read");
  TFile *DY_part6  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0001__part1.root", "read");
  TFile *DY_part7  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0001__part2.root", "read");
  TFile *DY_part8  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0001__part3.root", "read");
  TFile *DY_part9  = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0001__part4.root", "read");
  TFile *DY_part10 = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-50_0002__part0.root", "read");
  TFile *DY_part11 = new TFile("../../minitrees/nominal/Stop/DYJetsToLL_M-10to50.root"        , "read");
  TFile *DY_part12 = new TFile("../../minitrees/nominal/Stop/DYJetsToTT_MuEle_M-50.root"      , "read");

  TTree  *DY_tt0  = (TTree*)DY_part0->Get("latino");
  TTree  *DY_tt1  = (TTree*)DY_part1->Get("latino");
  TTree  *DY_tt2  = (TTree*)DY_part2->Get("latino");
  TTree  *DY_tt3  = (TTree*)DY_part3->Get("latino");
  TTree  *DY_tt4  = (TTree*)DY_part4->Get("latino");
  TTree  *DY_tt5  = (TTree*)DY_part5->Get("latino");
  TTree  *DY_tt6  = (TTree*)DY_part6->Get("latino");
  TTree  *DY_tt7  = (TTree*)DY_part7->Get("latino");
  TTree  *DY_tt8  = (TTree*)DY_part8->Get("latino");
  TTree  *DY_tt9  = (TTree*)DY_part9->Get("latino");
  TTree  *DY_tt10 = (TTree*)DY_part10->Get("latino");
  TTree  *DY_tt11 = (TTree*)DY_part11->Get("latino");
  TTree  *DY_tt12 = (TTree*)DY_part12->Get("latino");
  cout <<"the list"<<endl;

  TList *DY_myList = new TList();
  DY_myList->Add(DY_tt0);
  DY_myList->Add(DY_tt1);
  DY_myList->Add(DY_tt2);
  DY_myList->Add(DY_tt3);
  DY_myList->Add(DY_tt4);
  DY_myList->Add(DY_tt5);
  DY_myList->Add(DY_tt6);
  DY_myList->Add(DY_tt7);
  DY_myList->Add(DY_tt8);
  DY_myList->Add(DY_tt9);
  DY_myList->Add(DY_tt10);
  DY_myList->Add(DY_tt11);
  DY_myList->Add(DY_tt12);
  
  //myList->Write("MiLista")

  cout <<"The Tfile recreate"<<endl;
  TFile *DY_YY = new TFile("../../minitrees/nominal/Stop/DYJetsToLL.root", "recreate");
  TTree *DY_newtree = TTree::MergeTrees(DY_myList);
  DY_newtree->SetName("latino");
  DY_newtree->Write();
  cout <<"Close"<<endl;
  DY_part0->Close();
  DY_part1->Close();
  DY_part2->Close();
  DY_part3->Close();
  DY_part4->Close();
  DY_part5->Close();
  DY_part6->Close();
  DY_part7->Close();
  DY_part8->Close();
  DY_part9->Close();
  DY_part10->Close();
  DY_part11->Close();
  DY_part12->Close();
 
*/
}
