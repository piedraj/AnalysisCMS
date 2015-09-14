0. Documentation
====

    https://twiki.cern.ch/twiki/bin/view/CMS/WZ13TeV


1. Everything starts here
====

Log in to gridui

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh

Log in to lxplus

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    bash -l


2. Set the CMSSW release
====

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_7
    cd CMSSW_7_4_7/src
    cmsenv


3. Get the material
====

    git clone https://github.com/piedraj/WZ13TeV
    cd WZ13TeV

`AnalysisBase` should be recreated when the latino trees have been updated. To do so, first read any new latino tree,

    root -l latino_WZTo3LNu.root

Create the `AnalysisBase` skeleton,

    latino->MakeClass("AnalysisBase")

ROOT 6 is more exigent than ROOT 5. Please add `using namespace std;` before the `class AnalysisBase {` line.


4. Compile and run
====

    `root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`
    ./runWZ 

