#!/bin/bash


# How to use check-jobs.sh
#-------------------------------------------------------------------------------
if [ $# -lt 1 ]; then
    echo " "
    echo " Check the status of the jobs"
    echo " "
    echo "   ./chek-jobs.sh 0"
    echo " "
    echo " Print the list of failed files and remove the corresponding STDOUT"
    echo " "
    echo "   ./chek-jobs.sh 1"
    echo " "
    exit -1
fi

export OPTION=$1

export NPEND=`bjobs | grep PEND | wc -l`
export NRUN=`bjobs | grep RUN | wc -l`

export NFINISH=0
export NGOOD=0

if [ -d "jobs" ]; then
    NFINISH=`ls jobs/LSFJOB_*/STDOUT | wc -l`
    NGOOD=`cat jobs/LSFJOB_*/STDOUT | grep Done | wc -l`
fi


# Check the status of the jobs
#-------------------------------------------------------------------------------
if [ "$OPTION" -eq "0" ]; then

    printf " \n"
    printf " %3d jobs pending\n"    $NPEND
    printf " %3d jobs running\n"    $NRUN
    printf " %3d jobs finished\n"   $NFINISH
    printf " %3d jobs successful\n" $NGOOD
    printf " \n"

    if [ $NGOOD -ne $NFINISH ]; then

	printf " The following jobs have failed\n\n"

	for fn in `ls jobs/LSFJOB_*/STDOUT`; do

	    export ISDONE=`cat $fn | grep Done | wc -l`

	    if [ $ISDONE -ne 1 ]; then

		printf " %s\n" $fn
	
	    fi
	done

	printf " \n"
    fi
fi


# Print the list of failed files and remove the corresponding STDOUT"
#-------------------------------------------------------------------------------
if [ "$OPTION" -eq "1" ]; then

    if [ $NGOOD -ne $NFINISH ]; then

	rm -rf resubmit.txt

	for fn in `ls jobs/LSFJOB_*/STDOUT`; do

	    export ISDONE=`cat $fn | grep Done | wc -l`

	    if [ $ISDONE -ne 1 ]; then

		export FILENAME=`cat $fn | grep filename | cut -d ':' -f 2`
	    
		echo $FILENAME

		rm -rf $fn
	
	    fi
	done
    else

	printf "\n There are no jobs to resubmit\n"

    fi

    printf " \n"
fi
