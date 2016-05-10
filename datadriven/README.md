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

To be filled.
