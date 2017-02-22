#!/bin/bash

cat MassPointList.txt | while read line
do
  if [[ -n $line && "$line" != *"#"* ]]
  then
    ./createPointDatacards.sh $line
  fi
done
