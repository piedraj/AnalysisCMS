# metFilters.C

    rm -rf pdf
    rm -rf png

    root -l -b -q 'metFilters.C+'

    root -l -b -q 'metFilters.C+("SingleMuon")'
    root -l -b -q 'metFilters.C+("SingleElectron")'
    root -l -b -q 'metFilters.C+("DoubleMuon")'
    root -l -b -q 'metFilters.C+("DoubleEG")'
    root -l -b -q 'metFilters.C+("MuonEG")'

    root -l -b -q 'metFilters.C+("DYJetsToLL_M-50__part0")'
    root -l -b -q 'metFilters.C+("TTTo2L2Nu__part0")'
    root -l -b -q 'metFilters.C+("WWTo2L2Nu")'
    root -l -b -q 'metFilters.C+("GluGluHToWWTo2L2Nu_M125")'

