0. Documentation
====

    https://twiki.cern.ch/twiki/bin/view/CMS/WZ13TeV


1. Everything starts here
====

Log in to gridui.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh

Log in to lxplus.

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

The base class should be recreated when the latino trees have been updated. To do so, first read any new latino tree,

    root -l latino_WZTo3LNu.root

Then create `AnalysisBase`,

    latino->MakeClass("AnalysisBase")

Finally, open `AnalysisBase.h` and add

    using namespace std;


4. Compile and run
====

    `root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`

    ./create-jobs.sh

Input files can be accessed directly from eos when working from lxplus.

    ./runWZ root://eoscms.cern.ch//eos/cms/store/group/../latino_WZ.root 25ns


5. Draw
====

    cd test

    ./merge.sh

    root -l -b -q 'draw.C+(Exactly3Leptons)'
    root -l -b -q 'draw.C+(HasZ)'
    root -l -b -q 'draw.C+(HasW)'

