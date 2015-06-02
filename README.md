Get ROOT
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    pushd /gpfs/csic_users/piedra/CMSSW_7_3_0/src
    cmsenv
    popd


Get the material
====

    git clone https://github.com/piedraj/WZ13TeV


Run
====

    root -l -b -q runAnalysisWZ.C


Results
====

                              eee           eem           mme           mmm
    Exactly3Leptons      2305.000      3551.000      4788.000      7295.000
               HasZ      2043.000      3078.000      4050.000      6426.000
               HasW      1431.000      2084.000      2732.000      4229.000

