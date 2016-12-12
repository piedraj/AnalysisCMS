Non-prompt systematic uncertainty
====

    root -l -b -q getFakes.C


PDF and QCD systematic uncertainty
====

To compute both the PDF and QCD uncertainties we read the **list_vectors_weights** histogram, which is saved in those MC latino trees that contain LHE information. Before running getPdfQcd.C the analyzer should modify the selection, for it to match his/her analysis.

    eosmount eos

    root -l -b -q getPdfQcd.C+

