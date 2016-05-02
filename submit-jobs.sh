#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_data.txt       nominal"
    echo "  ./submit-jobs.sh samples/samples_datadriven.txt nominal"
    echo "  ./submit-jobs.sh samples/samples_mc.txt         nominal"
#   echo "  ./submit-jobs.sh samples/samples_monoh.txt      nominal"
    echo "  ./submit-jobs.sh samples/samples_ttdm.txt       nominal"
    echo "  ./submit-jobs.sh samples/samples_fakes_data.txt nominal"
    echo "  ./submit-jobs.sh samples/samples_fakes_mc.txt   nominal"
    echo "  "
    exit -1
fi

export SAMPLES=$1
export SYSTEMATIC=$2
export NJOBS=`cat $SAMPLES | grep latino | grep -v "#" | wc -l`

echo "  "
echo "  Let's play it safe. Compiling runAnalysis..."
./make

echo "  "
echo "  And... submitting" $NJOBS "jobs"
echo "  "
mkdir -p jobs
qsub -t 1-$NJOBS -v SAMPLES -v SYSTEMATIC settings.sge
echo "  "
