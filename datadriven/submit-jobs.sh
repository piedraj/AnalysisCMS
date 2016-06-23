#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  >>> 80x files"
    echo "  "
    echo "  ./submit-jobs.sh ../samples/80xx/samples_data_l2loose.txt"
    echo "  "
    exit -1
fi

export SAMPLES=$1


# Compile
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
### echo "  "
### echo "  Let's play it safe. Compiling runAnalysis..."
### pushd $HOME/CMSSW_8_0_5/src
### scram b -j 10
### popd
### echo "  "


# Submit jobs to the queues
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
export NJOBS=`cat $SAMPLES | grep latino | grep -v "#" | wc -l`
echo "  And... submitting" $NJOBS "jobs"
echo "  "
mkdir -p jobs
qsub -t 1-$NJOBS -v SAMPLES settings.sge
