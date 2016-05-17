#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./submit-jobs-interactive.sh samples/samples_data.txt       nominal"
    echo "  ./submit-jobs-interactive.sh samples/samples_datadriven.txt nominal"
    echo "  ./submit-jobs-interactive.sh samples/samples_mc.txt         nominal"
    echo "  "
    echo "  ./submit-jobs-interactive.sh samples/samples_monoh.txt      nominal"
    echo "  ./submit-jobs-interactive.sh samples/samples_ttdm.txt       nominal"
    echo "  "
    echo "  ./submit-jobs-interactive.sh samples/samples_fakes_data.txt nominal"
    echo "  ./submit-jobs-interactive.sh samples/samples_fakes_mc.txt   nominal"
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


# Submit jobs interactively
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo " Submit the jobs interactively..."
echo "  "
eval `cat $SAMPLES | grep latino | grep -v "#" | awk -v syst=$SYSTEMATIC '{ print "./runAnalysis "$1,syst,"2> /dev/null &" }'`
