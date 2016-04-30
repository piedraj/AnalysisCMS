#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./submit-jobs.sh samples/samples_data.txt       nominal"
    echo "  ./submit-jobs.sh samples/samples_datadriven.txt nominal"
    echo "  ./submit-jobs.sh samples/samples_mc.txt         nominal"
    echo "  ./submit-jobs.sh samples/samples_monoh.txt      nominal"
    echo "  ./submit-jobs.sh samples/samples_ttdm.txt       nominal"
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
if [ -z "$3" ]; then
    export NJOBS=`cat $SAMPLES | grep latino | grep -v "#" | wc -l`
    echo "  And... submitting" $NJOBS "jobs"
    echo "  "
    mkdir -p jobs
    qsub -t 1-$NJOBS -v SAMPLES -v SYSTEMATIC settings.sge
    echo "  "


# Submit jobs interactively
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
else
    echo " Submit the jobs interactively if the cluster is busy..."
    echo "  "
    cat $SAMPLES | grep latino | grep -v "#" | awk -v syst=$SYSTEMATIC '{ print "./runAnalysis "$1,syst,"2> /dev/null &" }'
#   cat $SAMPLES | grep latino | grep -v "#" | awk -v syst=$SYSTEMATIC '{ print "./runAnalysis "$1,syst,"&> /dev/null &" }'
fi
