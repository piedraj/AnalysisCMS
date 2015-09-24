0. Analysis documentation
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


2. Set a CMSSW release
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

    ./submit-jobs.sh

Notice that input files can be accessed directly from eos when working from lxplus.

    ./runWZ /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns/latino_WZTo3LNu.root 25ns
    ./runWZ root://eoscms.cern.ch//eos/cms/store/user/kbutanov/HWWwidthRun2/7September/25ns/latino_WZTo3LNu.root 25ns


5. Draw
====

    cd test

    ./merge.sh

    root -l -b -q 'draw.C+(Exactly3Leptons)'
    root -l -b -q 'draw.C+(HasZ)'
    root -l -b -q 'draw.C+(HasW)'


6. Create your website
====

Follow the instructions at [Permissions for your AFS folder](https://espace.cern.ch/webservices-help/websitemanagement/ConfiguringAFSSites/Pages/PermissionsforyourAFSfolder.aspx).

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    mkdir www
    fs setacl www webserver:afs read
    afind www -t d -e "fs setacl -dir {} -acl webserver:afs read"

Go to the CERN Web Services and click on [Create a new website](https://webservices.web.cern.ch/webservices/Services/CreateNewSite/Default.aspx).
You should choose the "AFS folder" site type.


7. Share on the web
====

Copy the distributions to lxplus.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    cd www
    scp -r piedra@gridui.ifca.es:/gpfs/csic_projects/cms/piedra/work/CMSSW_7_4_7/src/WZ13TeV/test/png .

