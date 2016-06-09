#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_data.txt       nominal"
    echo "  ./submit-jobs.sh samples/samples_datadriven.txt nominal"
    echo "  ./submit-jobs.sh samples/samples_mc.txt         nominal"
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_monoh.txt      nominal"
    echo "  ./submit-jobs.sh samples/samples_ttdm.txt       nominal"
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_fakes_data.txt nominal"
    echo "  ./submit-jobs.sh samples/samples_fakes_mc.txt   nominal"
    echo "  "
    exit -1
fi

export SAMPLES=$1
export SYSTEMATIC=$2


# Compile
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo "  "
echo "  Let's play it safe. Compiling runAnalysis..."
./make
echo "  "


# Submit jobs to the queues
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
export NJOBS=`cat $SAMPLES | grep latino | grep -v "#" | wc -l`
echo "  And... submitting" $NJOBS "jobs"
echo "  "
mkdir -p jobs
qsub -t 1-$NJOBS -v SAMPLES -v SYSTEMATIC settings.sge
