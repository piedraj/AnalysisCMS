#!/bin/bash

if [ $# -lt 3 ]; then
    echo "  "
    echo "  ./create-jobs.sh FILEPATH                                                              ERA  LUMINOSITY"
    echo "  "
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeVRun2015B          50ns 0.07152"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/50ns_August_PU 50ns 0.07152"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU 25ns 0.01547"
    echo "  "
    exit -1
fi

export FILEPATH=$1
export ERA=$2
export LUMINOSITY=$3

ls -1 $FILEPATH | awk '{ print "./runWZ '$FILEPATH'/"$1,"'$ERA' '$LUMINOSITY'" }'

