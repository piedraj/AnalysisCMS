0. Analysis documentation
====

These twiki pages contain the CMS synchronization status of the WW and WZ cross section analyses at 13 TeV. There we can find the datasets used in each analysis, trigger bits, lepton selections and analysis cuts.

    https://twiki.cern.ch/twiki/bin/view/CMS/WW2015Variables
    https://twiki.cern.ch/twiki/bin/view/CMS/WZ13TeV
    https://twiki.cern.ch/twiki/bin/view/CMS/ReviewSMP16002
    http://www.hep.uniovi.es/nachos/WZ/ValidationPlots_v2_2090pb/


1. First time only
====

Log in to gridui.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh

Set a CMSSW release.

    export SCRAM_ARCH=slc6_amd64_gcc530
    cmsrel CMSSW_8_0_5
    cd CMSSW_8_0_5/src
    cmsenv

Go to the master repository (https://github.com/piedraj/AnalysisCMS) and click **Fork** in the top-right corner of the page. Then get the code in your working area.

    git clone https://github.com/YOUR_USERNAME/AnalysisCMS
    git clone https://github.com/scodella/BTagSFUtil

Create an empty Git repository or reinitialize an existing one.

    git init

You should also add a remote upstream, to be able to modify both your and the master repository.

    git remote add upstream https://github.com/piedraj/AnalysisCMS

Then, do a git remote in order to check if the upstream appears. 

    git remote -v

*Do this only if you want to create a tag.*

    pushd AnalysisCMS
    git tag -a 20160318_electron -m 'Second AnalysisCMS tag'
    git push origin 20160318_electron
    popd

*Do this only if you want to use a tag.*

    pushd AnalysisCMS
    git checkout tags/20160318_electron
    popd

<!---
The base class should be recreated anytime the latino trees have been updated.
Read a MC latino tree that contains the `GEN_weight_SM` variable,

    root -l latino_DYJetsToLL_M-50.root
    latino->MakeClass("AnalysisBase")
-->


2. Always do
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cd CMSSW_8_0_5/src
    cmsenv
    cd AnalysisCMS


3. Compile and run
====

    ./make

It is recommended to test the code. The following example reads a latino tree and produces the corresponding histograms.

    ./runAnalysis /full/path/latino_WZTo3LNu.root nominal

Submit jobs to the gridui batch system. It is encouraged to first read the [Basic Grid Engine Usage](https://grid.ifca.es/wiki/Cluster/Usage/GridEngine) documentation.

    rm -rf minitrees/<systematic>
    rm -rf rootfiles/<systematic>
    rm -rf txt/<systematic>

    ./submit-jobs.sh

Show the status of the submitted jobs.

    qstat -u $USER
    qstat -j <job-ID.ja-task-ID>

Alternatively one can login to a node and run interactively. *Do this only if your jobs will take less than 2 hours.*

    qlogin -P l.gaes
    cd CMSSW_8_0_5/src
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cmsenv
    cd AnalysisCMS

    ./submit-jobs-interactive.sh

    exit

<!---
Notice that input files can be accessed directly from eos when working from lxplus.

    ./runAnalysis root://eoscms.cern.ch//eos/cms/store/user/kbutanov/HWWwidthRun2/7September/25ns/latino_WZTo3LNu.root nominal
-->


4. Draw
====

First `hadd` the files produced in the previous step.

    cd test
    ./submit-hadd.sh

Then compile and draw your favorite distributions.

    ./make
    ./runPlotter


5. Create your website
====

Follow the instructions at [Permissions for your AFS folder](https://espace.cern.ch/webservices-help/websitemanagement/ConfiguringAFSSites/Pages/PermissionsforyourAFSfolder.aspx).

    ssh -Y lxplus.cern.ch
    mkdir www
    fs setacl www webserver:afs read
    afind www -t d -e "fs setacl -dir {} -acl webserver:afs read"
    cd www
    wget https://raw.githubusercontent.com/piedraj/AnalysisCMS/master/test/.htaccess

Go to the CERN Web Services and click on [Create a new website](https://webservices.web.cern.ch/webservices/Services/CreateNewSite/Default.aspx).
Choose the "AFS folder" site type.


6. Share on the web
====

Create a soft link to your `figures` folder.

    cd figures
    export FIGURES_DIR=`pwd`
    cd
    ln -s $FIGURES_DIR

Copy the distributions to lxplus.

    ssh -Y lxplus.cern.ch
    scp -r piedra@gridui.ifca.es:figures www/.

And they should appear here,

    https://amanjong.web.cern.ch/amanjong/
    https://bchazinq.web.cern.ch/bchazinq/
    https://cprieels.web.cern.ch/cprieels/
    https://jgarciaf.web.cern.ch/jgarciaf/
    https://ntrevisa.web.cern.ch/ntrevisa/
    https://piedra.web.cern.ch/piedra/


7. It is commit time
====

This framework is intended to be used and developed by several people. If you want your changes to be shared by others, you should first get the latest modifications from the upstream repository, if any.

    git pull https://github.com/piedraj/AnalysisCMS
    git fetch upstream

Now you can commit any change (both from upstream/master and master) in your repository.

    git status
    git add <filepattern>
    git commit -m 'Modified'
    git push

If there isn't any conflict left you should be able to merge with the upstream/master repository. 

    git merge upstream/master

Finally, go to your repository and click **Pull Request**.

    https://github.com/YOUR_USERNAME/AnalysisCMS


8. Edit a CMS Analysis Note
====

The following instructions have been extracted from the [CMS TWiki](https://twiki.cern.ch/twiki/bin/view/Main/HowtoNotesInCMS). The note number used below corresponds to the IFCA/Oviedo WZ analysis at 13 TeV.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    svn co -N svn+ssh://svn.cern.ch/reps/tdr2 svn
    cd svn
    svn update utils
    svn update -N notes
    svn update notes/AN-16-010
    eval `notes/tdr runtime -sh`
    cd notes/AN-16-010/trunk
    tdr --style=an b AN-16-010

Get the latest modifications from the note repository. Always do this first.

    svn update

If you want to add new files.

    svn add file1 file2 ... fileN

If you want to know which files you have added and/or modified.

    svn status

Commit your changes.

    svn commit -m 'Modified'

A detailed example of a CMS Note written in LaTeX using the *cms-tdr* document class can be found in the [svn-instructions.pdf](https://github.com/piedraj/AnalysisCMS/raw/master/svn-instructions.pdf) file.


9. Copy latino trees from cernbox to gridui
====

First of all add your lxplus pub key in [ipa](https://ipa.ifca.es/).

    cat $HOME/.ssh/id_rsa.pub

Then log in to lxplus, mount eos and choose the input folder.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    bash -l

    /afs/cern.ch/project/eos/installation/0.3.84-aquamarine.user/bin/eos.select -b fuse mount eos

    rsync --chmod=Du=rwx,Dg=rwx,Fu=rw,Fg=rw -azH eos/user/j/jlauwers/HWW2015/07Jun2016_spring16__mAODv2/MCl2loose__hadd__bSFLepEff__l2tight $USER@pool03.ifca.es:

<!---
    rsync --chmod=Du=rwx,Dg=rwx,Fu=rw,Fg=rw -azH eos/user/j/jlauwers/HWW2015/07Jun2016_Run2016B_PromptReco/l2loose16bis__hadd__l2tight $USER@pool03.ifca.es:
    rsync --chmod=Du=rwx,Dg=rwx,Fu=rw,Fg=rw -azH eos/user/j/jlauwers/HWW2015/07Jun2016_spring16__mAODv2/MCl2loose16__hadd__l2tight     $USER@pool03.ifca.es:
-->

Do not forget unmounting eos once everything has been copied.

    /afs/cern.ch/project/eos/installation/0.3.84-aquamarine.user/bin/eos.select -b fuse umount eos
    rmdir eos

Check that the input folder has be copied at the following gridui path.

    /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/


10. brilcalc
====

Log in to lxplus.

    ssh -Y piedra@lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

Go to the **Prerequisite** section of the [BRIL Work Suite](http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html) and export the PATH that corresponds to the _centrally installed virtual environment on lxplus_.

    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH

Do this the first time. Do it also if you want to update the brilcalc version.

    pip uninstall brilws

    pip install --install-option="--prefix=$HOME/.local" brilws

Check your brilcalc version.

    brilcalc --version
    2.0.5

Get the 2016 luminosity.

    brilcalc lumi -b "STABLE BEAMS" -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-274240_13TeV_PromptReco_Collisions16_JSON.txt
    brilcalc lumi -b "STABLE BEAMS" --hltpath "HLT_Mu8_TrkIsoVVL_v*" -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-274240_13TeV_PromptReco_Collisions16_JSON.txt

Get the 2015 luminosity.

    brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/moriond16_normtag.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt

    +-------+------+-------+-------+-------------------+------------------+
    | nfill | nrun | nls   | ncms  | totdelivered(/fb) | totrecorded(/fb) |
    +-------+------+-------+-------+-------------------+------------------+
    | 47    | 115  | 33208 | 33208 | 2.398             | 2.318            |
    +-------+------+-------+-------+-------------------+------------------+
