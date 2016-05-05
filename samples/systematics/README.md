Copy files from srm to gridui
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240


**l2tight** Standard Model files for systematic uncertainties.

    voms-proxy-init --voms=cms --valid 168:00

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup/"$11}' > lcg-cp-LepMupTup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo/"$11}' > lcg-cp-LepMupTdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup/"$11}' > lcg-cp-LepElepTup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo/"$11}' > lcg-cp-LepElepTdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup/"$11}' > lcg-cp-METup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo/"$11}' > lcg-cp-METdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup/"$11}' > lcg-cp-JESMaxup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/22Jan_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo/"$11}' > lcg-cp-JESMaxdo.sh

    source lcg-cp.sh


**l2tight** signal (ttDM) files for systematic uncertainties.

    voms-proxy-init --voms=cms --valid 168:00

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTup/"$11}' > lcg-cp-LepMupTup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepMupTdo/"$11}' > lcg-cp-LepMupTdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTup/"$11}' > lcg-cp-LepElepTup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__LepElepTdo/"$11}' > lcg-cp-LepElepTdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METup/"$11}' > lcg-cp-METup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__METdo/"$11}' > lcg-cp-METdo.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxup/"$11}' > lcg-cp-JESMaxup.sh

    mkdir -p /gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo
    lcg-ls srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo | awk -F'/' '{print "lcg-cp -v srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/xjanssen/HWW2015/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo/"$11,"/gpfs/csic_projects/tier3data/LatinosSkims/RunII/cernbox/03Mar_25ns_mAODv2_MC/MCl2loose__hadd__bSFL2pTEff__l2tight__JESMaxdo/"$11}' > lcg-cp-JESMaxdo.sh

    source lcg-cp.sh
