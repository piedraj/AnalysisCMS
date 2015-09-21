#!/bin/bash

if [ $# -lt 3 ]; then
    echo "  "
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeVRun2015B          50ns"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/50ns_August_PU 50ns"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU 25ns"
    echo "  "
    exit -1
fi

export FILEPATH=$1
export ERA=$2

ls -1 $FILEPATH | awk '{ print "./runWZ '$FILEPATH'/"$1,"'$ERA'" }'

