#!/bin/bash

if [ $# -lt 3 ]; then
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  '/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__LepTrgFix__formulasMC/' \\"
    echo "  mc-names.txt \\"
    echo "  samples_mc_l2tightOR.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  '/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016*_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/' \\"
    echo "  data-names.txt \\"
    echo "  samples_data_l2tightOR.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  '/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016?_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__multiFakeW__formulasFAKE__hadd/' \\"
    echo "  data-names.txt \\"
    echo "  samples_data_multiFakeW.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  '/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016*_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__fakeSel__hadd/' \\"
    echo "  data-names.txt \\"
    echo "  samples_data_fakeSel.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  '/eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__fakeSelMC__hadd/' \\"
    echo "  mc-fakeSel-names.txt \\"
    echo "  samples_mc_fakeSel.txt"
    echo "  "

    exit -1
fi

export SAMPLES_PATH=$1
export SAMPLES_NAMES=$2
export SAMPLES_FILES=$3

rm -rf $SAMPLES_FILES

for MYPATH in "${SAMPLES_PATH[@]}"; do
    awk -v dir=$MYPATH '{ print "ls",dir"latino_"$1 }' $SAMPLES_NAMES | bash > $SAMPLES_FILES
done
