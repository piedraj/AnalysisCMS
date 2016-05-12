# getDYScale.C

This macro computes the DY scale factor for analyses that veto the Z-peak. It
needs as input a set of two-dimensional histograms, with MET in the x-axis and
the dilepton invariant mass in the y-axis. These histograms are normally filled
once all analysis cuts have been applied, but removing the Z-peak veto and any
MET requirement. The scale factor for the ee channel (the procedure is identical
for the mm channel) is computed with these two equations.

    k_ee  = 0.5 * sqrt(n_ee / n_mm);
    scale = (n_in_ee - n_in_wz - n_in_zz - k_ee * n_in_em) / n_in_dy;


# getFakeRate.C

This macro allows the user to calculate the Fake Rate for a given definition of
loose and tight leptons. The output of the macro is two 2D histograms representing
in the x-axis the lepton pt and in the y-axis the lepton eta values. The first
histogram represents the Fake Rate without correction while the second one
represents the Fake Rate with the electroweak correction given by the Z+Jets and
W+jets processes. 
