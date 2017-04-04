#!/bin/bash

echo "Combining datacards for the validation regions"
#
cd  Datacards/ValidationRegions/
#
combineCards.py em=VR1_Tag_em/MT2ll/datacard.txt   ee=VR1_Tag_ee/MT2ll/datacard.txt   mm=VR1_Tag_mm/MT2ll/datacard.txt   > datacardVR1Tag.txt
combineCards.py em=VR1_NoTag_em/MT2ll/datacard.txt ee=VR1_NoTag_ee/MT2ll/datacard.txt mm=VR1_NoTag_mm/MT2ll/datacard.txt > datacardVR1NoTag.txt
#
#combineCards.py em=$VR/VR1_Tag_em/MT2ll/datacard.txt   ee=$VR/VR1_Tag_ee/MT2ll/datacard.txt   mm=$VR/VR1_Tag_mm/MT2ll/datacard.txt   > datacardVR1Tag.txt
#combineCards.py em=$VR/VR1_NoTag_em/MT2ll/datacard.txt ee=$VR/VR1_NoTag_ee/MT2ll/datacard.txt mm=$VR/VR1_NoTag_mm/MT2ll/datacard.txt > datacardVR1NoTag.txt
#
combineCards.py Tag=datacardVR1Tag.txt NoTag=datacardVR1NoTag.txt > datacardVR1.txt

cd .. 
#
#mv datacard*.txt $VR/
#
