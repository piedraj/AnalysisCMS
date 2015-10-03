0. Analysis documentation
====

    https://twiki.cern.ch/twiki/bin/view/CMS/WZ13TeV


1. Everything starts here
====

Log in to gridui.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh


2. Set a CMSSW release
====

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_5_3
    cd CMSSW_7_5_3/src
    cmsenv


3. Get the material
====

    git clone https://github.com/piedraj/WZ13TeV
    cd WZ13TeV

The base class should be recreated anytime the latino trees have been updated.
Read a MC latino tree that contains the `GEN_weight_SM` variable,

    root -l latino_DYJetsToLL_M-50.root
    latino->MakeClass("AnalysisBase")

Open `AnalysisBase.h` and add

    using namespace std;


4. Compile and run
====

    `root-config --cxx --cflags` -o runWZ runWZ.C AnalysisBase.C `root-config --glibs`

With the following script all jobs will be submitted to the gridui batch system.

    ./submit-jobs.sh

In any case one can always run interactively.

    ./create-jobs.sh

Notice that input files can be accessed directly from eos when working from lxplus.

    ./runWZ root://eoscms.cern.ch//eos/cms/store/user/kbutanov/HWWwidthRun2/7September/25ns/latino_WZTo3LNu.root 25ns


5. Draw
====

    cd test

    ./merge.sh

    root -l -b -q 'draw.C+(nlep2_cut0_Exactly2Leptons)'
    root -l -b -q 'draw.C+(nlep2_cut1_ZVeto)'

    root -l -b -q 'draw.C+(nlep3_cut0_Exactly3Leptons)'
    root -l -b -q 'draw.C+(nlep3_cut1_HasZ)'
    root -l -b -q 'draw.C+(nlep3_cut2_HasW)'
    root -l -b -q 'draw.C+(nlep3_cut3_OneBJet)'


6. Create your website
====

Follow the instructions at [Permissions for your AFS folder](https://espace.cern.ch/webservices-help/websitemanagement/ConfiguringAFSSites/Pages/PermissionsforyourAFSfolder.aspx).

    ssh -Y lxplus.cern.ch
    mkdir www
    fs setacl www webserver:afs read
    afind www -t d -e "fs setacl -dir {} -acl webserver:afs read"

Go to the CERN Web Services and click on [Create a new website](https://webservices.web.cern.ch/webservices/Services/CreateNewSite/Default.aspx).
Choose the "AFS folder" site type.


7. Share on the web
====

Create a soft link to your `png` folder.

    ssh -Y gridui.ifca.es
    ln -s /gpfs/csic_projects/cms/piedra/work/CMSSW_7_4_7/src/WZ13TeV/test/png

Copy the distributions to lxplus.

    ssh -Y lxplus.cern.ch
    scp -r piedra@gridui.ifca.es:~/png www/.

And they should appear here,

    https://piedra.web.cern.ch/piedra/png/

