#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./submit-jobs.sh  <samples file>  nominal"
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
export NJOBS=`cat $SAMPLES | grep ".root" | grep -v "#" | wc -l`
echo "  And... submitting" $NJOBS "jobs"
echo "  "
mkdir -p jobs


# lxplus
#
#    -q 8nm (8 minute)
#    -q 1nh (1 hour)
#    -q 8nh (8 hour)
#    -q 1nd (1 day)
#    -q 2nd (2 day)
#    -q 1nw (1 week)
#    -q 2nw (2 week)
#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cd jobs

if [[ "$SAMPLES" != *"minitrees"* ]]; then
    bsub -q 8nh -J "settings[1-$NJOBS]" -o $WORKDIRECTORY/jobs $WORKDIRECTORY/settings.lsf WORKDIRECTORY SAMPLES SYSTEMATIC
else
    bsub -q 1nd -J "settings[1-$NJOBS]" -o $WORKDIRECTORY/jobs $WORKDIRECTORY/settings_minitrees.lsf WORKDIRECTORY SAMPLES SYSTEMATIC
fi


# gridui
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#qsub -t 1-$NJOBS -v SAMPLES -v SYSTEMATIC settings.sge
