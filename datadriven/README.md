# getDYScale.C

This macro computes the DY scale factor for analyses that veto the Z-peak. It
needs as input a set of two-dimensional histograms, with MET in the x-axis and
the dilepton invariant mass in the y-axis. These histograms are normally filled
once all analysis cuts have been applied, but removing the Z-peak veto and any
MET requirement. The scale factor for the ee channel (the procedure is identical
for the mm channel) is computed with these two equations.

    k_ee  = 0.5 * sqrt(n_ee / n_mm);
    scale = (n_in_ee - n_in_wz - n_in_zz - k_ee * n_in_em) / n_in_dy;

# AnalysisFR

AnalysisFR is the part of the code that creates the histograms needed to add the fake
weights to the latino trees afterwards. It creates 5 Control Region to be able to 
calculate from data the effective luminosity associated to the Z+jets process (the 
luminosity is calculated in the ZRegion and then applied in the ZRegionQCD) and to 
the W+jets process (luminosity calculated in the WRegion and the applied in the 
WRegionQCD). The region defined to calculate the fakes is the QCDRegion.

This macro creates a serie of 1D and 2D histograms that can then be read by getLumiEff.C
and by getFakeRate.C, and uses an array of different input jet energies (different histograms
are created for every given energy of the array).

Two different ways are possible to use this macro. You can either run the code once without
any data weight and then use the getLumiEff.C macro to determine the effective luminosity
from the pic of the Z and then run once again AnalysisFR.C with these weights, or you can 
run the code only once using the luminosity of the triggers given by brilcalc directly. 

# getLumiEff.C

This macro is an easy way to calculate the effective luminosity without needing to
use brilcalc's results. It calculates the number of electrons and muons (loose and tight) in
the Z window for different processes (data, Z+jets, W+jets, tt) and computes the
effective luminosity in each case. This effective luminosity can then be 
used in AnalysisFR.C instead of the brilcalc values, to weight the data or the MC.

For now, this macro is designed to be working for an analysis of two triggers, one at low pt
and another one at high pt. To get the effective luminosity of a given trigger, just change 
the value of the boolean highpt to true, or false. 

# getFakeRate.C

This macro allows the user to calculate the fake rate and the prompt rate for a given
definition of loose and tight leptons. The output of the macro is a set of histograms
representing the fake and prompt rates with respect to the pt or the eta value of the
leptons, and some rootfiles with 2D histograms that can be used directly within the 
latino framework in order to compute the fake weights to include to the data. 

Two main ways to use the code are possible. You can first set the boolean called draw to 
true, chaneg the value of elejetet and muonjetet as required and see the output of the code 
directly on the screen (all the 1D histograms will pop up directly and display the results
with and without electroweak correction). You can also set draw to false and then, the 
result of the execution of the code will be a set of rootfiles corresponding to the FR 
and the PR of electrons and muons, for all the different jet energies used as input.

# Apply the FR weights to the data

It is now possible to submit to gridui the gardener jobs that apply the fake weights
to the data. To do so, first download the latino code to your gridui account and
run the command below.

    ./submit-jobs.sh
