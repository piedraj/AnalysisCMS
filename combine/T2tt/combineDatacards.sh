#!/bin/bash

cat MassPointList.txt | while read line
do
   if [[ -n $line && "$line" != *"#"* ]]
   then
      ./combinePointDatacards.sh Datacards/MassPoint$line
   fi
done
