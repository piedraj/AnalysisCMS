[0. Analysis documentation](#analysis-documentation)  
[1. Everything begins here](#everything-begins-here)  
[2. Always do](#always-do)  
[3. Compile and run](#compile-and-run)  
[4. Draw](#draw)  
[5. Create your website](#create-your-website)  
[6. Share on the web](#share-on-the-web)  
[7. It is commit time](#it-is-commit-time)  
[8. Edit a CMS Analysis Note](#edit-a-cms-analysis-note)  
[9. EOS](#eos)  
[10. Copy latino trees](#copy-latino-trees)  
[11. brilcalc](#brilcalc)  
[12. CRAB output destination](#crab-output-destination)  
[13. Polycom connection](#polycom-connection)  


# <a name="analysis-documentation"/>0. Analysis documentation

AnalysisCMS is a framework that produces Physics distributions based on **latino** trees. It should be fully synchronized with the common latinos framework.

    https://twiki.cern.ch/twiki/bin/view/CMS/LatinosFrameworkFor2017
    https://github.com/latinos/LatinoTrees
    https://github.com/latinos/LatinoAnalysis
    https://github.com/latinos/PlotsConfigurations


# <a name="everything-begins-here"/>1. Everything begins here

Log in to gridui or lxplus.

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

Execute this line only in gridui.

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
    git tag -a 20160919_tau -m 'Third AnalysisCMS tag'
    git push origin 20160919_tau
    popd

*Do this only if you want to use a tag.*

    pushd AnalysisCMS
    git checkout tags/20160919_tau
    popd

<!---
The base class should be recreated anytime the latino trees have been updated.
Read a MC latino tree that contains the `GEN_weight_SM` variable,

    root -l latino_DYJetsToLL_M-50.root
    latino->MakeClass("AnalysisBase")
-->


# <a name="always-do"/>2. Always do

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cd CMSSW_8_0_5/src
    cmsenv
    cd AnalysisCMS


# <a name="compile-and-run"/>3. Compile and run

    ./make

It is recommended to first test the code.

    ./runAnalysis

If you submit jobs to the gridui batch system it is encouraged to first read the [Basic Grid Engine Usage](https://grid.ifca.es/wiki/Cluster/Usage/GridEngine) documentation.

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

    ./runAnalysis

    exit


# <a name="draw"/>4. Draw

First `hadd` the files produced in the previous step.

    cd test
    ./submit-hadd.sh

Then compile and draw your favorite distributions.

    ./make
    ./runPlotter


# <a name="create-your-website"/>5. Create your website

Follow the instructions at [Permissions for your AFS folder](https://espace.cern.ch/webservices-help/websitemanagement/ConfiguringAFSSites/Pages/PermissionsforyourAFSfolder.aspx).

    ssh -Y lxplus.cern.ch
    mkdir www
    fs setacl www webserver:afs read
    afind www -t d -e "fs setacl -dir {} -acl webserver:afs read"
    cd www
    wget https://raw.githubusercontent.com/piedraj/AnalysisCMS/master/test/.htaccess

Go to the CERN Web Services and click on [Create a new website](https://webservices.web.cern.ch/webservices/Services/CreateNewSite/Default.aspx).
Choose the "AFS folder" site type.


# <a name="share-on-the-web"/>6. Share on the web

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
    https://fernanpe.web.cern.ch/fernanpe/
    https://jgarciaf.web.cern.ch/jgarciaf/
    https://ntrevisa.web.cern.ch/ntrevisa/
    https://piedra.web.cern.ch/piedra/
    https://rocio.web.cern.ch/rocio/


# <a name="it-is-commit-time"/>7. It is commit time

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


# <a name="edit-a-cms-analysis-note"/>8. Edit a CMS Analysis Note

The following instructions have been extracted from the [CMS TWiki](https://twiki.cern.ch/twiki/bin/view/Main/HowtoNotesInCMS). The note AN-17-208 documents our **Search for scalar top quark pair production with three-body decays in the
dilepton final state at 13 TeV with the CMS detector** analysis.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    svn co -N svn+ssh://svn.cern.ch/reps/tdr2 svn
    cd svn
    svn update utils
    svn update -N notes
    svn update notes/AN-17-208
    eval `notes/tdr runtime -csh`
    cd notes/AN-17-208/trunk
    tdr --style=an b AN-17-208

Get the latest modifications from the note repository. Always do this first.

    svn update

If you want to add new files.

    svn add file1 file2 ... fileN

If you want to know which files you have added and/or modified.

    svn status

Commit your changes.

    svn commit -m 'Modified'


# <a name="eos"/>9. EOS

To access files at eos/cms.

    eosmount  eoscms
    eosumount eoscms
    rmdir eoscms

To access files at eos/user.

    /afs/cern.ch/project/eos/installation/0.3.84-aquamarine.user/bin/eos.select -b fuse mount  eosuser
    /afs/cern.ch/project/eos/installation/0.3.84-aquamarine.user/bin/eos.select -b fuse umount eosuser
    rmdir eosuser

Check the eos quota.

    eos quota /eos/cms/store/group/phys_higgs

    group      logi bytes  aval logib  filled[%]
    zh         171.07 TB   250.00 TB   68.43


# <a name="copy-latino-trees"/>10. Copy latino trees

First of all add your lxplus pub key in [ipa](https://ipa.ifca.es/).

    cat $HOME/.ssh/id_rsa.pub

Then log in to lxplus, mount eos and choose the input folder.

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240
    bash -l
    cd eos/user/j/jlauwers/HWW2015
    rsync --chmod=Du=rwx,Dg=rwx,Fu=rw,Fg=rw -azH 21Jun2016_Run2016B_PromptReco/l2loose__hadd__EpTCorr__l2tight $USER@pool03.ifca.es:

Check that the input folder has be copied at the following gridui path.

    /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/


# <a name="brilcalc"/>11. brilcalc

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

Get the 2016 luminosity. Based on the [PdmV](https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis) TWiki one should use the following.

    brilcalc lumi -b "STABLE BEAMS" \
                  --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json \
                  -u /fb \
                  -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt \
                  --hltpath "HLT_Mu8_TrkIsoVVL_v*"

Get the 2015 luminosity.

    brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/moriond16_normtag.json -u /fb -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt

    +-------+------+-------+-------+-------------------+------------------+
    | nfill | nrun | nls   | ncms  | totdelivered(/fb) | totrecorded(/fb) |
    +-------+------+-------+-------+-------------------+------------------+
    | 47    | 115  | 33208 | 33208 | 2.398             | 2.318            |
    +-------+------+-------+-------+-------------------+------------------+


# <a name="crab-output-destination"/>12. CRAB output destination

    ls -l /gpfs/gaes/cms/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Feb2017/

To change the ACL permissions.

    pushd /gpfs/gaes/cms/store/group/phys_higgs/cmshww/amassiro/RunII/2016/Feb2017/data/25ns/

    mmgetacl LatinoTrees > acl_file_piedra

Modify, if needed, the owner line. In this case, replace **sluca** by **piedra**.

    #owner:sluca

Find the latino files and change their ACL permissions.

    find LatinoTrees/Run2016E -name "latino_*" -exec mmputacl -i acl_file_piedra {} \; -print

    popd


# <a name="polycom-connection"/>13. Polycom connection

1. Open the **Address Book** and call **Vidyogw06.cern.ch**.
2. Once the call fails (because it will fail) enter **Video Call** and connect.
3. It will ask for the number below. Enter it you are all set.

    \* 10 444 191 #
