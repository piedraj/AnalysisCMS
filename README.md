Everything starts here
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240

    export SCRAM_ARCH=slc6_amd64_gcc491
    cmsrel CMSSW_7_4_6
    cd CMSSW_7_4_6/src/
    cmsenv


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
                HasZ          3823          4493          5377          6653
                HasW          2668          3030          3661          4382


Synchronization
====

    cd txt
    sort -k 2 --field-separator=":" WZ13TeV_mmm.txt   > WZ13TeV_mmm_sorted.txt
    sort -k 2 --field-separator=":" events_WZ_mmm.txt > events_WZ_mmm_sorted.txt

    diff WZ13TeV_mmm_sorted.txt events_WZ_mmm_sorted.txt | grep -E "<|>"
    > 1:2101:10040
    > 1:404:40309
    < 1:4984:98324

