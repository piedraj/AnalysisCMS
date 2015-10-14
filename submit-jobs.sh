#!/bin/bash


shopt -s expand_aliases
alias grepv='grep -v -e Run2015C -e QCD -e GluGluH -e HToWW -e WJetsToLNu_HT -e WZJets -e ttH'


if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeV/50ns             50ns"
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/50ns_August_PU 50ns"
    echo "  "
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeV/25ns             25ns"
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU 25ns"
    echo "  "
    echo "  WARNING, will not read any latino tree that contains one of these strings"
    echo "  "`alias | grep grepv | awk -F= '{print $2}' | sed "s/'//g" | sed "s/-e //g" | sed "s/grep -v //g"`
    echo "  "
    exit -1
fi


echo "  "
echo "  Compiling runWZ..."
`root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`


echo "  "
echo "  Submitting jobs..."
export FILEPATH=$1
export ERA=$2
export NJOBS=`ls -1 $FILEPATH | grepv | wc -l`

qsub -t 1-$NJOBS -v FILEPATH -v ERA settings.sge
echo "  "
