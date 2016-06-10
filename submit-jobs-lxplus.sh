#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs-lxplus.sh samples/samples_data_2015D_74X_eos.txt nominal"
    echo "  ./submit-jobs-lxplus.sh samples/samples_mc_74X_eos.txt         nominal"
    echo "  ./submit-jobs-lxplus.sh samples/samples_stop_74X_eos.txt       nominal"
    echo "  "
    exit -1
fi


export WORKDIRECTORY=$PWD
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
cd jobs/
bsub -q 1nh -J "settings[1-$NJOBS]" -o $WORKDIRECTORY/jobs $WORKDIRECTORY/settings.lsf WORKDIRECTORY SAMPLES SYSTEMATIC
