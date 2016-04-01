0. Get the input files
====

    cd AnalysisCMS/test
    ./submit-hadd.sh ../minitrees/TTDM


1. Train the MVA
====

    cd AnalysisCMS/tmva
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0010\")"


2. Draw the MVA response
====

    cd AnalysisCMS/tmva
    ./make
    ./runPlotter