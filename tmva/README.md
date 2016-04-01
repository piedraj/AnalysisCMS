0. Get the input files
====

    cd AnalysisCMS/test
    ./submit-hadd.sh ../minitrees/TTDM


1. Train the MVA
====

    root -l -b -q "MVA.C+(\"ttDM0001pseudo0010\")"


2. Draw the MVA response
====

    ./make
    ./runPlotter


3. Print final yields
====

    root -l -b -q "yields.C+(\"ttDM0001pseudo0010\",0.56)"

