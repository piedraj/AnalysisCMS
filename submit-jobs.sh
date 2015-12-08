#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_21Oct_25ns_MC.txt"
    echo "  ./submit-jobs.sh samples/samples_21Oct_Run2015D_05Oct2015.txt"
    echo "  ./submit-jobs.sh samples/samples_21Oct_Run2015D_PromptReco.txt"
    echo "  "
    exit -1
fi

export SAMPLES=$1
export NJOBS=`cat $SAMPLES | grep latino | grep -v "#" | wc -l`

echo "  "
echo "  Let's play it safe. Compiling runAnalysis..."
./make

echo "  "
echo "  And... submitting" $NJOBS "jobs"
echo "  "
qsub -t 1-$NJOBS -v SAMPLES settings.sge
echo "  "
