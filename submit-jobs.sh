#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  Common samples"
    echo "-----------------"
    echo "  ./submit-jobs.sh samples/samples_data.txt"
    echo "  ./submit-jobs.sh samples/samples_datadriven.txt"
    echo "  ./submit-jobs.sh samples/samples_mc.txt"
    echo "  "
#   echo "  Analysis specific samples"
#   echo "----------------------------"
#   echo "  ./submit-jobs.sh samples/samples_metfilters.txt"
#   echo "  ./submit-jobs.sh samples/samples_monoh.txt"
#   echo "  ./submit-jobs.sh samples/samples_ttdm.txt"
#   echo "  "
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
