Get ROOT
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    pushd /gpfs/csic_users/piedra/CMSSW_7_3_0/src
    cmsenv
    popd


Get the material
====

    git clone https://github.com/piedraj/WZ13TeV


Run
====

    root -l -b -q runAnalysisWZ.C


Results
====

                               eee           eem           mme           mmm
     Exactly3Leptons          4953          5704          6576          7292
                HasZ          4434          4910          5587          6423
                HasW          3095          3332          3778          4227
