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

    n_event[EEE][Exactly3Leptons]: 2305
    n_event[EEM][Exactly3Leptons]: 3551
    n_event[MME][Exactly3Leptons]: 4788
    n_event[MMM][Exactly3Leptons]: 7295

