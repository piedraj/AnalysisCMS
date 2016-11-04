// https://github.com/susynt/SusyNtuple/blob/master/Root/KinematicTools.cxx 
/////////////////////////////////////////////////////////////////////////////////
// Super-Razor Super Razor Super razor super razor (leptonic) calculation
/////////////////////////////////////////////////////////////////////////////////

void SuperRazor(vector<TLorentzVector> leptons, TLorentzVector met, TVector3& vBETA_z, TVector3& pT_CM,
                TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                double& gamma_R, double& dphi_vBETA_R_vBETA_T,
                double& MDELTAR, double& costhetaRp1)
{
    //
    // Code written by Christopher Rogan <crogan@cern.ch>, 04-23-13
    // Details given in paper (http://arxiv.org/abs/1310.4827) written by
    // Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
    //
    if (leptons.size() < 2) return;

    // necessary variables
    TLorentzVector metlv = met;
    TLorentzVector l0 = leptons[0];
    TLorentzVector l1 = leptons[1];

    //
    // Lab frame 
    //
    //Longitudinal boost
    vBETA_z = (1. / (l0.E() + l1.E()))*(l0 + l1).Vect();
    vBETA_z.SetX(0.0);
    vBETA_z.SetY(0.0);

    l0.Boost(-vBETA_z);
    l1.Boost(-vBETA_z);

    //pT of CM frame
    pT_CM = (l0 + l1).Vect() + metlv.Vect();
    pT_CM.SetZ(0.0);

    TLorentzVector ll = l0 + l1;
    //invariant mass of the total event
    SHATR = sqrt(2.*(ll.E()*ll.E() - ll.Vect().Dot(pT_CM)
        + ll.E()*sqrt(ll.E()*ll.E() + pT_CM.Mag2() - 2.*ll.Vect().Dot(pT_CM))));

    vBETA_T_CMtoR = (1. / sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;

    l0.Boost(-vBETA_T_CMtoR);
    l1.Boost(-vBETA_T_CMtoR);
    ll.Boost(-vBETA_T_CMtoR);

    //
    //R-frame
    //
    dphi_LL_vBETA_T = fabs((ll.Vect()).DeltaPhi(vBETA_T_CMtoR));

    dphi_L1_L2 = fabs(l0.Vect().DeltaPhi(l1.Vect()));

    vBETA_R = (1. / (l0.E() + l1.E()))*(l0.Vect() - l1.Vect());

    gamma_R = 1. / sqrt(1. - vBETA_R.Mag2());

    dphi_vBETA_R_vBETA_T = fabs(vBETA_R.DeltaPhi(vBETA_T_CMtoR));

    l0.Boost(-vBETA_R);
    l1.Boost(vBETA_R);

    //
    //R+1 frame
    //
    MDELTAR = 2.*l0.E();
    costhetaRp1 = l0.Vect().Dot(vBETA_R) / (l0.Vect().Mag()*vBETA_R.Mag());

    return;
}
