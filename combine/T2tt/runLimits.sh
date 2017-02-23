#!/bin/bash

cat MassPointList.txt | while read line
do
   if [[ -n $line && "$line" != *"#"* ]]
   then
       ./runPointLimit.sh $line
   fi
done
