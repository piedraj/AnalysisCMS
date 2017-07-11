#!/bin/bash

if [ $# -lt 3 ]; then
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_summer16/lepSel__MCWeights__bSFLpTEffMulti__cleanTauMC__l2loose__hadd__l2tightOR__formulasMC__MCFix/ \\"
    echo "  mc-names.txt \\"
    echo "  samples_mc.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016B_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016B.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016C_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016C.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016D_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016D.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016E_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016E.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016F_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016F.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016G_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016G.txt"
    echo "  "
    echo "  ./write-samples.sh \\"
    echo "  /eos/cms/store/group/phys_higgs/cmshww/amassiro/Full2016_Apr17/Apr2017_Run2016H_RemAOD/lepSel__EpTCorr__TrigMakerData__cleanTauData__l2loose__hadd__l2tightOR__formulasDATA/ \\"
    echo "  data-names.txt \\"
    echo "  samples_data_Run2016H.txt"
    echo "  "
    echo "  rm -rf samples_data.txt; cat samples_data_Run2016* > samples_data.txt; rm -rf samples_data_Run2016*"
    echo "  "
    exit -1
fi

export DIRECTORY=$1
export INPUT=$2
export OUTPUT=$3

rm -rf $OUTPUT

awk -v dir=$DIRECTORY '{ print "ls",dir"latino_"$1 }' $INPUT | bash > $OUTPUT
