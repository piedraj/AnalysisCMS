
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

 number of tight electrons: 129241
 number of tight muons:     149109

                               eee           eem           mme           mmm
     Exactly3Leptons          5373          6053          6749          7292
                HasZ          4802          5195          5735          6424
                HasW          3341          3515          3845          4229
