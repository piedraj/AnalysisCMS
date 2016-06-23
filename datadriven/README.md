# getDYScale.C

This macro computes the DY scale factor for analyses that veto the Z-peak. It
needs as input a set of two-dimensional histograms, with MET in the x-axis and
the dilepton invariant mass in the y-axis. These histograms are normally filled
once all analysis cuts have been applied, but removing the Z-peak veto and any
MET requirement. The scale factor for the ee channel (the procedure is identical
for the mm channel) is computed with these two equations.

    k_ee  = 0.5 * sqrt(n_ee / n_mm);
    scale = (n_in_ee - n_in_wz - n_in_zz - k_ee * n_in_em) / n_in_dy;

# getLumiEff.C

This macro is an easy way to calculate the effective luminosity without needing to
use Brilcalc. It calculates the number of electrons and muons (loose and tight) in
the Z window pic for different processes (data, Z+jets, W+jets, TT) and gives as an
output the effective luminosity in each case. This effective luminosity can then be 
used in AnalysisFR.C instead of Brilcalc results to weight the data or the MC.

# getFakeRate.C

This macro allows the user to calculate the Fake Rate and the Prompt Rate for a given
definition of loose and tight leptons. The output of the macro is a serie of histograms
representing the Fake and Prompt Rate with respect to the pt or the eta value of the
leptons, and some rootfiles with 2D histograms that can be used directly within the 
latino framework in order to compute the fake weights to include to the data. 

The Fake Rate is calculated and represented for different inputjetet, and on the
different histograms created, an electroweak correction (given by the Z+jets, W+jets
and TT processes) is applied and drawed. 

# Apply the FR weights to the data

It is now possible to submit to Gridui the gardener jobs that apply the fake weights
to the data. To do so, first download the latino code to your gridui account and
run the following command

    ./submit-jobs.sh

