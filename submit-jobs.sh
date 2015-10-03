#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeVRun2015B          50ns"
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/50ns_August_PU 50ns"
    echo "  ./submit-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU 25ns"
    echo "  "
    exit -1
fi

export FILEPATH=$1
export ERA=$2
export NJOBS=`ls -1 $FILEPATH | grep -v QCD | wc -l`

qsub -t 1-$NJOBS -v FILEPATH -v ERA settings.sge
