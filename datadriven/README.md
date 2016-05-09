# getDYScale.C

This macro computes the DY scale factor for analyses that veto the Z-peak. It
needs as input a set of two-dimensional histograms, with MET in the x-axis and
the dilepton invariant mass in the y-axis. These histograms have been filled
once all analysis cuts have been applied (see below) but removing the Z-peak
veto and any MET requirement. The scale factor for the ee channel (the
procedure is identical for the mm channel) is computed with these two equations.

    k_ee  = 0.5 * sqrt(n_ee / n_mm);
    scale = (n_in_ee - n_in_wz - n_in_zz - k_ee * n_in_em) / n_in_dy;

The selection that has been applied to produce these figures is the following.
The b-tag requirement corresponds to the combined MVA medium working point.

    two ``latino'' tight opposite-charge leptons;
    both leptons have pt > 20 GeV;
    the dilepton invariant mass is > 20 GeV;
    at least two 30 GeV jets in the event;
    at least one 30 GeV b-tagged jet.
