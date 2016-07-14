Non-prompt background
====

We need the **l1loose** and **l2loose** lepton skims for data, and the **l1loose** lepton skim for the following MC samples.

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


Copy files from srm to gridui
====

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240

    ./lcg-cp.sh
