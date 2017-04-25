# getDYScale.C

This macro computes the DY scale factor for analyses that veto the Z-peak.
It needs as input a set of two-dimensional histograms, with varx (which can
be met, mpmet, mt2ll, etc) in the x-axis, and m2l in the y-axis. These
histograms have been filled once all analysis cuts have been applied, but
removing the Z-peak veto and any requirement on varx.

    root -l getDYScale.C


# AnalysisFR

This macro creates the histograms needed to add the fake weights to the latino
trees afterwards. It also creates several control regions to be able to calculate
from data the effective luminosity associated to the Z+jets process (luminosity
calculated in the ZRegion and then applied in the ZRegionQCD) and to 
the W+jets process (luminosity calculated in the WRegion and then applied in the 
WRegionQCD). The region defined to calculate the fakes is the QCDRegion.

Two different ways are possible to use this macro. You can either run the code
once without any data weight, then use the getLumiEff.C macro to determine the
effective luminosity from the Z-peak, and then run once again AnalysisFR.C
with these weights; or you can run the code only once using the luminosity of
the triggers given by brilcalc directly. 


# getLumiEff.C

This macro is an easy way to calculate the effective luminosity without needing
to use brilcalc's results. It calculates the number of electrons and muons
(loose and tight) in the Z window for different processes (data, Z+jets, W+jets)
and computes the effective luminosity in each case. This effective luminosity
can then be used in AnalysisFR.C instead of the brilcalc values, to weight the
data or the MC.

For now, this macro is designed to be working for an analysis of two triggers,
one at low pt and another one at high pt. To get the effective luminosity of a
given trigger, just change the value of the boolean highpt to true, or false. 


# getFakeRate.C

This macro allows the user to calculate the fake rate and the prompt rate for a given
definition of loose and tight leptons. The output of the macro is a set of histograms
representing the fake and prompt rates with respect to the pt or the eta value of the
leptons, and some rootfiles with 2D histograms that can be used directly within the 
latino framework in order to compute the fake weights to include to the data. 

Two main ways to use the code are possible. You can first set the boolean called draw to 
true, change the value of elejetet and muonjetet as required and see the output of the code 
directly on the screen (all the 1D histograms will pop up directly and display the results
with and without electroweak correction). You can also set draw to false and then, the 
result of the execution of the code will be a set of rootfiles corresponding to the FR 
and the PR of electrons and muons, for all the different jet energies used as input.


# Apply the FR weights to the data

It is now possible to submit to gridui the gardener jobs that apply the fake weights
to the data. To do so, first download the latino code to your gridui account and
then run the command below.

    ./submit-jobs.sh

# Fake production recipe

When new data or MC files are available, it is necessary to produce new rootfiles which will be
used by Xavier to produce the fake files. The production of these rootfiles has to be done following
the next few steps.

1) Update or create a new samples.txt file, within the fakeSel directory, reflecting the latest changes.
2) Check if the triggers used are still the same, run Brilcalc (detailed instructions available in the 
step 11 of https://github.com/piedraj/AnalysisCMS/blob/master/README.md), in order to get the prescaled 
lumi of the different triggers.
3) Run AnalysisFR and AnalysisPR on the data and MC files of the fakeSel samples directory.
4) Once all the jobs are done, run getLumiEff.C to check the data/MC agreement in the peak of the Z.
5) Once this is checked, the fake and prompt rate plots are obtained thanks to getFakeRate.C
6) If everything seems right, the output within the rootfilesFR and rootfilesPR directories can be
commited to the latino repository on Github and Xavier can start the production of the new files.
7) Once the new files ready, a SS control region study is necessary to check if the fakes seems to be
behaving correctly or not.