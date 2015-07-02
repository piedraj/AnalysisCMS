Everything starts here
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_6
    cd CMSSW_7_4_6/src
    cmsenv


Get the material
====

    git clone https://github.com/piedraj/WZ13TeV


Run
====

    cd WZ13TeV
    root -l -b -q runAnalysisWZ.C


Results
====

                               eee           eem           emm           mmm
     Exactly3Leptons          4130          5065          6164          7290
                HasZ          3823          4493          5377          6653
                HasW          2668          3030          3661          4382


Synchronization
====

    cd txt
    sort -k 2 --field-separator=":" WZ_mmm_ifca.txt > WZ_mmm_ifca_sorted.txt
    sort -k 2 --field-separator=":" WZ_mmm_uw.txt   > WZ_mmm_uw_sorted.txt

    diff WZ_mmm_ifca_sorted.txt WZ_mmm_uw_sorted.txt | grep -E "<|>"
    > 1:2101:10040
    > 1:404:40309
    < 1:4984:98324

