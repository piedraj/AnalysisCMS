0. Analysis documentation
====

These twiki pages contain the CMS synchronization status of the WW and WZ cross section analyses at 13 TeV. There we can find the datasets used in each analysis, trigger bits, lepton selections and analysis cuts.

    https://twiki.cern.ch/twiki/bin/view/CMS/WZ13TeV
    https://twiki.cern.ch/twiki/bin/view/CMS/WW2015Variables


1. First time only
====

Log in to gridui.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh

Set a CMSSW release.

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_5_3
    cd CMSSW_7_5_3/src
    cmsenv

Go to the master repository (https://github.com/piedraj/AnalysisCMS) and click **Fork** in the top-right corner of the page. Then get the code in your working area.

    git clone https://github.com/YOUR_USERNAME/AnalysisCMS

You should also add a remote upstream, to be able to modify both your and the master repository.

    git remote add upstream https://github.com/piedraj/AnalysisCMS

Then, do a git remote in order to check if the upstream appears. 

    git remote -v

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
    cd CMSSW_7_5_3/src
    cmsenv
    cd AnalysisCMS


3. Compile and run
====

    ./make

It is recommended to test the code. The following example reads a latino tree and produces the corresponding histograms.

    ./runAnalysis /full/path/latino_WZTo3LNu.root 25ns

Submit jobs to the gridui batch system.

    rm -rf rootfiles
    rm -rf txt

    ./submit-jobs.sh

Show the status of the submitted jobs.

    qstat -u $USER
    qstat -j <job-ID.ja-task-ID>

Alternatively one can login to a node and run interactively.

    qlogin -P l.gaes
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cd CMSSW_7_5_3/src
    cmsenv
    cd AnalysisCMS
    ./make
    ./runAnalysis /full/path/latino_WZTo3LNu.root 25ns

<!---
Notice that input files can be accessed directly from eos when working from lxplus.

    ./runAnalysis root://eoscms.cern.ch//eos/cms/store/user/kbutanov/HWWwidthRun2/7September/25ns/latino_WZTo3LNu.root 25ns
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
    cp /afs/cern.ch/user/p/piedra/www/.htaccess www/.

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

    https://amanjong.web.cern.ch/amanjong/figures/
    https://cprieels.web.cern.ch/cprieels/figures/
    https://ntrevisa.web.cern.ch/ntrevisa/
    https://piedra.web.cern.ch/piedra/figures/

A parallel WZ study is being performed at Oviedo, reading heppy trees. The corresponding plots can be found here,

    http://www.hep.uniovi.es/nachos/WZ/ValidationPlots_v2_2090pb/


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
    eval `notes/tdr runtime -csh`
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

