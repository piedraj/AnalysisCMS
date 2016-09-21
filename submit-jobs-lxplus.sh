#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  rm -rf jobs"
    echo "  rm -rf rootfiles"
    echo "  "
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_data_l2tight.txt        nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_data_l2loose_fakeW.txt  nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_mc_l2tight.txt          nominal"
    echo "  "
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_monoh_l2tight.txt       nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_ttdm_l2tight.txt        nominal"
    echo "  "
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_mc_l2tight_topSel.txt   nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_ttdm_l2tight_topSel.txt nominal"
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
