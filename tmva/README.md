1. Run MVA
====

    root -l -b -q "MVA.C+(\"<signal>\")"
    root -l -b -q "MVA.C+(\"ttDM0001pseudo0010\")"


2. Draw MVA
====

    ./make
    ./runPlotter
    ./runPlotter ttDM0001pseudo0010
