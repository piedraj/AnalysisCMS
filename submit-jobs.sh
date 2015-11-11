#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/files_25ns.txt 25ns"
    echo "  "
    exit -1
fi

export SAMPLES=$1
export ERA=$2
export NJOBS=`cat $SAMPLES | grep latino | wc -l`

echo "  "
echo "  Let's play it safe. Compiling runAnalysis..."
./make

echo "  "
echo "  Removing rootfiles/"$ERA"..."
rm -rf rootfiles/$ERA

echo "  "
echo "  And... submitting jobs"
echo "  "
qsub -t 1-$NJOBS -v SAMPLES -v ERA settings.sge
echo "  "
