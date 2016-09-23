#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  voms-proxy-init --voms=cms --valid 168:00"
    echo "  "
    echo "  "
    echo "  "
    echo "  ./lcg-cp.sh 07Jun2016_spring16_mAODv2_4p0fbm1/MCl1loose__EpTCorr__hadd"
    echo "  "
    echo "  ./lcg-cp.sh 21Jun2016_Run2016B_PromptReco/l1loose__EpTCorr__hadd"
    echo "  ./lcg-cp.sh 05Jul2016_Run2016B_PromptReco/l1loose__EpTCorr__hadd"
    echo "  ./lcg-cp.sh 08Jul2016_Run2016B_PromptReco/l1loose__EpTCorr__hadd"
    echo "  ./lcg-cp.sh 08Jul2016_Run2016C_PromptReco/l1loose__EpTCorr__hadd"
    echo "  "
    echo "  ./lcg-cp.sh 21Jun2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr"
    echo "  ./lcg-cp.sh 05Jul2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr"
    echo "  ./lcg-cp.sh 08Jul2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr"
    echo "  ./lcg-cp.sh 08Jul2016_Run2016C_PromptReco/l2loose__hadd__EpTCorr"
    echo "  ./lcg-cp.sh 11Jul2016_Run2016C_PromptReco/l2loose__hadd__ICHEPjson__EpTCorr"
    echo "  ./lcg-cp.sh 15Jul2016_Run2016C_PromptReco/l2loose__hadd__ICHEPjson__EpTCorr"
    echo "  ./lcg-cp.sh 15Jul2016_Run2016D_PromptReco/l2loose__hadd__ICHEPjson__EpTCorr"
    echo "  ./lcg-cp.sh 26Jul2016_Run2016D_PromptReco/l2loose__hadd__ICHEPjson__EpTCorr"
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
#export GRIDUI_PATH='/gpfs/csic_projects'
export  LXPLUS_PATH='/afs/cern.ch/work/p/piedra/public/l2loose'

rm -rf lcg-cp-batch.sh

#mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/$FOLDER
mkdir  -p $LXPLUS_PATH/$FOLDER


#
# Copy to $GRIDUI_PATH
#
### eval `lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/$FOLDER | awk -v path=$GRIDUI_PATH -v destination=$FOLDER -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/"destination"/"$11,path"/tier3data/LatinosSkims/RunII/cernbox/"destination"/"$11,"&"}'`


#
# Manually copy to $GRIDUI_PATH
#
### lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/$FOLDER | awk -v path=$GRIDUI_PATH -v destination=$FOLDER -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/"destination"/"$11,path"/tier3data/LatinosSkims/RunII/cernbox/"destination"/"$11,"&"}' > lcg-cp-batch.sh


#
# Manually copy to $LXPLUS_PATH
#
lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/$FOLDER | awk -v path=$LXPLUS_PATH -v destination=$FOLDER -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/"destination"/"$11,path"/"destination"/"$11,"&"}' > lcg-cp-batch.sh
