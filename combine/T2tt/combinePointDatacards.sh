#!/bin/bash

echo "Combining datacards for" $1
combineCards.py em=$1/SR1_Tag_em/MT2ll/datacard.txt     ee=$1/SR1_Tag_ee/MT2ll/datacard.txt     mm=$1/SR1_Tag_mm/MT2ll/datacard.txt   > datacardSR1T.txt
combineCards.py em=$1/SR1_NoTag_em/MT2ll/datacard.txt   ee=$1/SR1_NoTag_ee/MT2ll/datacard.txt   mm=$1/SR1_NoTag_mm/MT2ll/datacard.txt > datacardSR1V.txt
#
combineCards.py em=$1/SR2_Tag_em/MT2ll/datacard.txt     ee=$1/SR2_Tag_ee/MT2ll/datacard.txt     mm=$1/SR2_Tag_mm/MT2ll/datacard.txt   > datacardSR2T.txt
combineCards.py em=$1/SR2_NoTag_em/MT2ll/datacard.txt   ee=$1/SR2_NoTag_ee/MT2ll/datacard.txt   mm=$1/SR2_NoTag_mm/MT2ll/datacard.txt > datacardSR2V.txt
#
combineCards.py em=$1/SR3_Tag_em/MT2ll/datacard.txt     ee=$1/SR3_Tag_ee/MT2ll/datacard.txt     mm=$1/SR3_Tag_mm/MT2ll/datacard.txt   > datacardSR3T.txt
combineCards.py em=$1/SR3_NoTag_em/MT2ll/datacard.txt   ee=$1/SR3_NoTag_ee/MT2ll/datacard.txt   mm=$1/SR3_NoTag_mm/MT2ll/datacard.txt > datacardSR3V.txt
#
combineCards.py SR1=datacardSR1T.txt SR2=datacardSR2T.txt SR3=datacardSR3T.txt > datacardTag.txt 
combineCards.py SR1=datacardSR1V.txt SR2=datacardSR2V.txt SR3=datacardSR3V.txt > datacardNoTag.txt 
#
combineCards.py Tag=datacardTag.txt NoTag=datacardNoTag.txt > datacardFinal.txt 
#
mv datacard*.txt $1/

