#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  voms-proxy-init --voms=cms --valid 168:00"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> 80x files"
    echo "  "
    echo "  ./lcg-cp.sh 20May2016_Run2016B_PromptReco/l1loose"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> 76x files"
    echo "  "
    echo "  ./lcg-cp.sh 03Mar_Run2015C_16Dec2015/l1loose__EpTCorr"
    echo "  ./lcg-cp.sh 03Mar_Run2015D_16Dec2015/l1loose__EpTCorr"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup"
    echo "  ./lcg-cp.sh 22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup"
    echo "  ./lcg-cp.sh 03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> 74x files"
    echo "  "
    echo "  ./lcg-cp.sh 21Oct_25ns_MC/MCl2loose__hadd__bSFL2pTEff__l2tight/"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> The previous command should have created the lcg-cp-batch.sh file. Check it, then launch it."
    echo "  "
    echo "  source lcg-cp-batch.sh"
    echo "  "
    exit -1
fi


export  FOLDER=$1
#export GRIDUI_PATH='srm://srm01.ifca.es/cms/store/group'
export  GRIDUI_PATH='/gpfs/csic_projects'

rm -rf lcg-cp-batch.sh

mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/$FOLDER


#
# Copy to $GRIDUI_PATH
#
### eval `lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/$FOLDER | awk -v path=$GRIDUI_PATH -v destination=$FOLDER -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/"destination"/"$11,path"/tier3data/LatinosSkims/RunII/cernbox/"destination"/"$11,"&"}'`


#
# Manually copy to $GRIDUI_PATH
#
lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/$FOLDER | awk -v path=$GRIDUI_PATH -v destination=$FOLDER -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/"destination"/"$11,path"/tier3data/LatinosSkims/RunII/cernbox/"destination"/"$11,"&"}' > lcg-cp-batch.sh
