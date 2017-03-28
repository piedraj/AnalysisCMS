#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  No skim"
    echo "  "
#   echo "  ./submit-jobs.sh  samples/80x/l2tight/samples_data_l2loose_fakeW.txt  nominal"
#   echo "  ./submit-jobs.sh  samples/80x/l2tight/samples_data_l2tight.txt        nominal"
    echo "  ./submit-jobs.sh  samples/80x/l2tight/samples_data_l2tight_RemAOD.txt nominal"
#   echo "  ./submit-jobs.sh  samples/80x/l2tight/samples_mc_l2tight.txt          nominal"
    echo "  ./submit-jobs.sh  samples/80x/l2tight/samples_mc_l2tight_summer16.txt nominal"
    echo "  "
    echo "  "
    echo "  "
    echo "  stopSel skim"
    echo "  "
    echo "  ./submit-jobs.sh  samples/80x/stopSel/samples_data_miniIso.txt  nominal"
    echo "  ./submit-jobs.sh  samples/80x/stopSel/samples_mc_miniIso.txt    nominal"
    echo "  ./submit-jobs.sh  samples/80x/stopSel/samples_stop_miniIso.txt  nominal"
    echo "  "
    echo "  "
    echo "  "
    echo "  wwSel skim"
    echo "  "
    echo "  ./submit-jobs.sh  samples/80x/wwSel/samples_data_l2loose_fakeW.txt  nominal"
    echo "  ./submit-jobs.sh  samples/80x/wwSel/samples_data_l2tight.txt        nominal"
    echo "  ./submit-jobs.sh  samples/80x/wwSel/samples_mc_l2tight.txt          nominal"
    echo "  ./submit-jobs.sh  samples/80x/wwSel/samples_monoh_l2tight.txt       nominal"
    echo "  "
    echo "  "
    echo "  "
    echo "  topSel skim"
    echo "  "
    echo "  ./submit-jobs.sh  samples/80x/topSel/samples_data_l2loose_fakeW.txt  nominal"
    echo "  ./submit-jobs.sh  samples/80x/topSel/samples_data_l2tight.txt        nominal"
    echo "  ./submit-jobs.sh  samples/80x/topSel/samples_mc_l2tight.txt          nominal"
    echo "  ./submit-jobs.sh  samples/80x/topSel/samples_ttdm_l2tight.txt        nominal"
    echo "  ./submit-jobs.sh  samples/80x/topSel/samples_stop_l2tight.txt        nominal"
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
