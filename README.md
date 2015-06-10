
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

 number of tight electrons: 120160
 number of tight muons:     149109

                               eee           eem           mme           mmm
     Exactly3Leptons          4420          5306          6311          7292
                HasZ          3984          4581          5365          6423
                HasW          2795          3112          3654          4227

