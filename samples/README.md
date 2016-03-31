Non-prompt background
====

We need the 2loose lepton skim for data and the 1loose lepton skim for the following MC samples.

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


How to copy files from srm
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    voms-proxy-init --voms=cms

    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl1loose__EpTCorr/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl1loose_EpTCorr/"$11}' > lcg-cp.sh

    source lcg-cp.sh


To be copied.

    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup
    /pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo
