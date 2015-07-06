Everything starts here
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_6
    cd CMSSW_7_4_6/src
    cmsenv


Get the material
====

    git clone https://github.com/piedraj/WZ13TeV
    cd WZ13TeV


Run
====

    `root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`
    ./runWZ 

