#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  >>> 80x files"
    echo "  "
    echo "  ./submit-jobs.sh samples/80x/samples_data_l1loose.txt nominal"
    echo "  ./submit-jobs.sh samples/80x/samples_mc_l1loose.txt   nominal"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> 76x files"
    echo "  "
    echo "  ./submit-jobs.sh samples/76x/samples_data.txt       nominal"
    echo "  ./submit-jobs.sh samples/76x/samples_datadriven.txt nominal"
    echo "  ./submit-jobs.sh samples/76x/samples_mc.txt         nominal"
    echo "  "
    echo "  ./submit-jobs.sh samples/76x/samples_monoh.txt      nominal"
    echo "  ./submit-jobs.sh samples/76x/samples_ttdm.txt       nominal"
    echo "  "
    echo "  ./submit-jobs.sh samples/76x/samples_data_fakes.txt nominal"
    echo "  ./submit-jobs.sh samples/76x/samples_mc_fakes.txt   nominal"
    echo "  "
    echo "  "
    echo "  "
    echo "  >>> 74x files"
    echo "  "
    echo "  ./submit-jobs.sh samples/74x/samples_data_2015D.txt nominal"
    echo "  ./submit-jobs.sh samples/74x/samples_mc.txt         nominal"
    echo "  ./submit-jobs.sh samples/74x/samples_stop.txt       nominal"
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
