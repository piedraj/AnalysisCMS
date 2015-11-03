#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/files_05Aug_50ns.txt 50ns"
    echo "  "
    exit -1
fi

echo "  "
echo "  Let's play it safe. Compiling runAnalysis..."
./make

echo "  "
echo "  Submitting jobs..."
export SAMPLES=$1
export ERA=$2
export NJOBS=`cat $SAMPLES | grep latino | wc -l`
qsub -t 1-$NJOBS -v SAMPLES -v ERA settings.sge
echo "  "
