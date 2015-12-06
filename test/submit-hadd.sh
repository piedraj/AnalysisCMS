#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./submit-hadd.sh ../rootfiles"
    echo "  "
    exit -1
fi

export FOLDER=$1
export NJOBS=`cat merge.sh | grep hadd | wc -l`

echo "  "
echo "  Submitting" $NJOBS "jobs"
echo "  "
qsub -t 1-$NJOBS -v FOLDER settings.sge
echo "  "
