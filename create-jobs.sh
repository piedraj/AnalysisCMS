#!/bin/bash

shopt -s expand_aliases

 alias grepv='grep latino'
#alias grepv='grep -v -e QCD -e GluGluH -e HToWW -e WJetsToLNu_HT -e WZJets -e ttH'
#alias grepv='grep -e WWTo2L2Nu_NLL -e TTJets'

if [ $# -lt 2 ]; then
    echo "  "
    echo "  (Default) With 'grep latino' nothing is filtered"
    echo "            With 'grep -v -e pattern1 ... -e patternN' these patterns are filtered out"
    echo "            With 'grep -e pattern1 ... -e patternN' only these patterns are accepted"
    echo "  "
    echo "  You have chosen "`alias | grep grepv | awk -F= '{print $2}'`
    echo "  "
    echo "  Please choose one of these two aliases"
    echo "  "
    echo "  alias latino2hist='./create-jobs.sh'"
    echo "  alias latino2hist='./submit-jobs.sh'"
    echo "  "
    echo "  Now you are ready to create or submit the jobs"
    echo "  "
    echo "  latino2hist /gpfs/csic_projects/tier3data/LatinosSkims/RunII/Data13TeV/05Aug/50ns_Skim2l      50ns"
    echo "  latino2hist /gpfs/csic_projects/tier3data/LatinosSkims/RunII/MC_Spring15/05Aug/50ns_PU_Skim2l 50ns"
    echo "  "
### echo "  latino2hist /gpfs/csic_projects/tier3data/LatinosSkims/RunII/Data13TeV/17Sep/25ns   25ns"
### echo "  latino2hist /gpfs/csic_projects/tier3data/LatinosSkims/RunII/MC_Spring15/17Sep/25ns 25ns"
### echo "  "
    echo "  Synchronization with Oviedo"
    echo "  "
    echo "  ./runWZ /gpfs/csic_projects/cms/piedra/latino/latino_WZ_synchro.root 25ns"
    echo "  "
    exit -1
fi

export FILEPATH=$1
export ERA=$2

ls -1 $FILEPATH | grepv | awk '{ print "./runWZ '$FILEPATH'/"$1,"'$ERA'" }'
