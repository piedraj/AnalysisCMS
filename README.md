
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

                               eee           eem           emm           mmm
     Exactly3Leptons          4130          5065          6164          7290
                HasZ          3723          4384          5246          6420
                HasW          2600          2965          3573          4226


Synchronization
====

    cd txt
    sort -k 2 --field-separator=":" WZ13TeV_mmm.txt   > WZ13TeV_mmm_sorted.txt
    sort -k 2 --field-separator=":" events_WZ_mmm.txt > events_WZ_mmm_sorted.txt

    diff WZ13TeV_mmm_sorted.txt events_WZ_mmm_sorted.txt | grep -E "<|>"
    > 1:2101:10040
    > 1:404:40309
    < 1:4984:98324

