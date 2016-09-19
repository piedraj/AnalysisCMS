#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  rm -rf jobs"
    echo "  rm -rf rootfiles/"
    echo "  "
    echo "  >>> 80x files"
    echo "  "
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_data_l1loose.txt       nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_mc_l1loose.txt         nominal"
    echo "  "		 
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_data_l2tight.txt       nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_mc_l2tight.txt         nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_datadriven_l2tight.txt nominal"
    echo "  ./submit-jobs-lxplus.sh samples/80x/samples_ttdm_l2tight.txt       nominal"
    echo "  "		 
    echo "  "		 
    echo "  "		 
    echo "  >>> 76x files-lxplus"
    echo "  "		 
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_data.txt               nominal"
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_datadriven.txt         nominal"
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_mc.txt                 nominal"
    echo "  "		 
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_monoh.txt              nominal"
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_ttdm.txt               nominal"
    echo "  "		 
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_data_fakes.txt         nominal"
    echo "  ./submit-jobs-lxplus.sh samples/76x/samples_mc_fakes.txt           nominal"
    echo "  "		 
    echo "  "		 
    echo "  "		 
    echo "  >>> 74x files-lxplus"
    echo "  "		 
    echo "  ./submit-jobs-lxplus.sh samples/74x/samples_data_2015D.txt         nominal"
    echo "  ./submit-jobs-lxplus.sh samples/74x/samples_mc.txt                 nominal"
    echo "  ./submit-jobs-lxplus.sh samples/74x/samples_stop.txt               nominal"
    echo "  "
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
