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
    cd WZ13TeV


Run
====

    root -l AnalysisWZ.C+
    root[1] AnalysisWZ awz
    root[2] awz.Loop()


Results
====

                                     raw yields

                               eee                            eem                            emm                            mmm                            all
     Exactly3Leptons       4130.00 +- 64.27               5065.00 +- 71.17               6164.00 +- 78.51               7290.00 +- 85.38              22649.00 +- 150.50       
                HasZ       3823.00 +- 61.83               4493.00 +- 67.03               5377.00 +- 73.33               6653.00 +- 81.57              20346.00 +- 142.64       
                HasW       2668.00 +- 51.65               3030.00 +- 55.05               3661.00 +- 60.51               4382.00 +- 66.20              13741.00 +- 117.22       


                                     1 fb-1 yields

                               eee                            eem                            emm                            mmm                            all
     Exactly3Leptons         38.26 +- 0.60                  46.92 +- 0.66                  57.10 +- 0.73                  67.53 +- 0.79                 209.78 +- 1.39         
                HasZ         35.42 +- 0.57                  41.62 +- 0.62                  49.81 +- 0.68                  61.63 +- 0.76                 188.45 +- 1.32         
                HasW         24.72 +- 0.48                  28.07 +- 0.51                  33.91 +- 0.56                  40.59 +- 0.61                 127.28 +- 1.09         


Synchronization
====

    cd txt
    sort -k 2 --field-separator=":" WZ_mmm_ifca.txt > WZ_mmm_ifca_sorted.txt
    sort -k 2 --field-separator=":" WZ_mmm_uw.txt   > WZ_mmm_uw_sorted.txt

    diff WZ_mmm_ifca_sorted.txt WZ_mmm_uw_sorted.txt | grep -E "<|>"
    > 1:2101:10040
    > 1:404:40309
    < 1:4984:98324

