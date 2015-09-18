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

The base class should be recreated when the latino trees have been updated. To do so, first read any new latino tree,

    root -l latino_WZTo3LNu.root

Then create `AnalysisBase`,

    latino->MakeClass("AnalysisBase")

Finally, open `AnalysisBase.h` and add `using namespace std;`.


4. Compile and run
====

    `root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`

    ./runWZ "WZTo3LNu"            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_WZTo3LNu.root"
    ./runWZ "DYJetsToLL_M-10to50" "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_DYJetsToLL_M-10to50.root"
    ./runWZ "DYJetsToLL_M-50"     "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_DYJetsToLL_M-50.root"
    ./runWZ "ST_tW_top"           "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ST_tW_top.root"
    ./runWZ "TTTo2L2Nu"           "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTTo2L2Nu.root"
    ./runWZ "TTWJetsToLNu"        "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTWJetsToLNu.root"
    ./runWZ "TTZToLLNuNu_M-10"    "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_TTZToLLNuNu_M-10.root"
    ./runWZ "ggZZ2e2m"            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2e2m.root"
    ./runWZ "ggZZ2e2t"            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2e2t.root"
    ./runWZ "ggZZ2m2t"            "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ2m2t.root"
    ./runWZ "ggZZ4e"              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4e.root"
    ./runWZ "ggZZ4m"              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4m.root"
    ./runWZ "ggZZ4t"              "/gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU/latino_ggZZ4t.root"

Input files can be accessed directly from eos when working from lxplus

    ./runWZ "WZ" "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/kbutanov/RunII/15Jul/25ns/latino_WZ.root"


5. Draw
====

    root -l -b -q 'draw.C+(HasW,1)'

