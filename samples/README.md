Non-prompt background
====

We need the **l1loose** and **l2loose** lepton skims for data, and the **l1loose** lepton skim for the following MC samples.

    DYJetsToLL_M-10to50
    DYJetsToLL_M-10to50ext3
    DYJetsToLL_M-50
    DYJetsToLL_M-50-LO

    WJetsToLNu
    WJetsToLNu_HT100_200
    WJetsToLNu_HT200_400
    WJetsToLNu_HT400_600
    WJetsToLNu_HT600_800
    WJetsToLNu_HT800_1200
    WJetsToLNu_HT1200_2500
    WJetsToLNu_HT2500_inf

    QCD_Pt-15to20_EMEnriched
    QCD_Pt-20to30_EMEnriched
    QCD_Pt-30to50_EMEnriched
    QCD_Pt-50to80_EMEnriched
    QCD_Pt-20toInf_MuEnrichedPt15

    TT
    TTJets


Copy files from srm to gridui
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240


**l1loose** data files for non-prompt background estimation.

    voms-proxy-init --voms=cms

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_Run2015C_16Dec2015/l1loose__EpTCorr/
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_Run2015C_16Dec2015/l1loose__EpTCorr | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_Run2015C_16Dec2015/l1loose__EpTCorr/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_Run2015C_16Dec2015/l1loose__EpTCorr/"$11}' > lcg-cp-2015C.sh

    source lcg-cp-2015C.sh


    voms-proxy-init --voms=cms

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_Run2015D_16Dec2015/l1loose__EpTCorr/
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_Run2015D_16Dec2015/l1loose__EpTCorr | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_Run2015D_16Dec2015/l1loose__EpTCorr/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_Run2015D_16Dec2015/l1loose__EpTCorr/"$11}' > lcg-cp-2015D.sh

    source lcg-cp-2015D.sh


**l1loose** MC files for non-prompt background estimation.

    voms-proxy-init --voms=cms

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr/"$11}' > lcg-cp.sh

    wc lcg-cp.sh
    split --lines=51 -d lcg-cp.sh lcg-cp-

    voms-proxy-init --voms=cms; source lcg-cp-00
    voms-proxy-init --voms=cms; source lcg-cp-01
    voms-proxy-init --voms=cms; source lcg-cp-02
    voms-proxy-init --voms=cms; source lcg-cp-03
