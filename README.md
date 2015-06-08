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
     Exactly3Leptons          1796          2106          2391          2757
                HasZ          1622          1823          2025          2429
                HasW          1119          1222          1378          1605

