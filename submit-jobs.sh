#!/bin/bash

if [ $# -lt 2 ]; then
    ./create-jobs.sh
    exit -1
fi

echo "  "
echo "  Let's play it safe. Compiling runWZ..."
`root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`

echo "  "
echo "  Submitting jobs..."
export FILEPATH=$1
export ERA=$2
export NJOBS=`./create-jobs.sh $FILEPATH $ERA | wc -l`
qsub -t 1-$NJOBS -v FILEPATH -v ERA settings.sge
echo "  "
