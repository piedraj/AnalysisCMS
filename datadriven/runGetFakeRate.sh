#!/bin/bash

 root -l -b -q 'getFakeRate.C("../rootfiles/nominal")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/FirstWP-v3",  "36fb_ele_cut_WP_Tight80X")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/SecondWP-v3", "36fb_ele_cut_WP_Tight80X_SS")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/ThirdWP-v3",  "36fb_ele_mva_80p_Iso2015")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/FourthWP-v3", "36fb_ele_mva_80p_Iso2016")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/FifthWP-v3",  "36fb_ele_mva_90p_Iso2015")'
 root -l -b -q 'getFakeRate.C("../rootfiles-cprieels/SixthWP-v3",  "36fb_ele_mva_90p_Iso2016")'

