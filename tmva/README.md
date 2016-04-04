0. Get the input files
====

    cd AnalysisCMS/test
    ./submit-hadd.sh ../minitrees/TTDM


1. Train the MVA
====

    root -l -b -q "MVA.C+(\"ttDM0001pseudo0010\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0020\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0050\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0100\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0200\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0500\")"
    root -l -b -q "MVA.C+(\"ttDM0010pseudo0100\")"
    root -l -b -q "MVA.C+(\"ttDM0050pseudo0200\")"
    root -l -b -q "MVA.C+(\"ttDM0150pseudo0200\")"
    root -l -b -q "MVA.C+(\"ttDM0150pseudo0500\")"

    root -l -b -q "MVA.C+(\"ttDM0001scalar0010\")"
    root -l -b -q "MVA.C+(\"ttDM0001scalar0050\")"
    root -l -b -q "MVA.C+(\"ttDM0001scalar0100\")"
    root -l -b -q "MVA.C+(\"ttDM0001scalar0200\")"
    root -l -b -q "MVA.C+(\"ttDM0001scalar0300\")"
    root -l -b -q "MVA.C+(\"ttDM0001scalar0500\")"
    root -l -b -q "MVA.C+(\"ttDM0010scalar0010\")"
    root -l -b -q "MVA.C+(\"ttDM0010scalar0050\")"
    root -l -b -q "MVA.C+(\"ttDM0010scalar0100\")"
    root -l -b -q "MVA.C+(\"ttDM0050scalar0050\")"
    root -l -b -q "MVA.C+(\"ttDM0050scalar0200\")"
    root -l -b -q "MVA.C+(\"ttDM0050scalar0300\")"
    root -l -b -q "MVA.C+(\"ttDM0150scalar0200\")"
    root -l -b -q "MVA.C+(\"ttDM0500scalar0500\")"


2. Draw the MVA response
====

    ./make
    ./runPlotter


3. Print final yields
====

    root -l -b -q "analysis.C+(\"ttDM0001pseudo0010\",0.56)"

