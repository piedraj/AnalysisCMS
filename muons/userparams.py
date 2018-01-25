# /Usr/bin/env python

import os
import shutil
import sys
import fileinput
import string

##############################################################
# Input parameters
#

#
# Should we execute the code?
#
Submit=True
#
# Should we publish the results?
#
Publish=False

# + Location of the AFS place where to put the PDFs
WebRepository = '/afs/cern.ch/cms/Physics/muon/CMSSW/Performance/RecoMuon/Validation/val'

# User enabled to write in the afs area
User='piedra'

#
# Information about the new release
#
NewParams = dict(
    # Type of parameters
    Type='New',
    # CMSSW_10_0_0_pre3_G4VecGeom2-100X_upgrade2018_realistic_v4-v2
    # Releases to compare
    Release='CMSSW_10_0_0_pre3_G4VecGeom2',
    Release_c='CMSSW_10_0_0_pre3_G4VecGeom2',
    Condition='100X_upgrade2018',

    # 'no' if no pileup, otherwise set spacing
#    PileUp ='25ns',
    PileUp='no',

    # If True use Fastsim, else use Fullsim
    FastSim=False,

    # Where to get the root file from. Possible values
    # * WEB: Take root files from the MuonPOG Validation repo on the web
    # * CASTOR: Copy root files from castor
    # * GUI: Copy root files from the DQM GUI server
    # By default, if the root files are already in the local area,
    # they won't be overwritten

    GetFilesFrom='GUI',

    # Base URL of the DQM GUI repository
    DqmGuiBaseRepo='https://cmsweb.cern.ch/dqm/relval/data/browse/ROOT/RelVal/',
    #     DqmGuiRepository='https://cmsweb.cern.ch/dqm/dev/data/browse/Development/RelVal/',
    #     DqmGuiRepository='https://cmsweb.cern.ch/dqm/offline/data/browse/ROOT/RelVal/CMSSW_4_3_x/',

    # Location of Castor repository
    CastorRepository = '/castor/cern.ch/user/a/aperrott/ValidationRecoMuon',

    # These are only needed if you copy any root file from the DQM GUI.
    # See GetLabel function for more details
#     Label='70_V6_corrHARV',
#     Label='HCALdev_v2_BpixFpixHcalGeom',
    Label='realistic_v4',
    Format='DQMIO',
    #Format='GEN-SIM-RECO',

    # Minor Version
    Version='v2',
    DQM='DQM_V0001_R000000001'

)


#
# Information about the reference release.
# We only need to set the variables than are different from the new release
#
RefParams = dict(
#    Condition='100X_mcRun2',
    Condition='100X_upgrade2018',
#    Condition='100X_mc2017',
    Type='Ref',
    # CMSSW_10_0_0_pre3_GEANT4-100X_upgrade2018_realistic_v4_mahiON-v1
    Release='CMSSW_10_0_0_pre3_GEANT4',
    Release_c='CMSSW_10_0_0_pre3_GEANT4',
    Label='realistic_v4_mahiON',
#    Label='asymptotic_v1',
#    Version='v1',
#    PileUp='25ns',
    PileUp='no',
    FastSim=False,
    DQM='DQM_V0001_R000000001',
    Version='v1'
)


ValidateHLT  = True
ValidateRECO = True
ValidateISO  = True
ValidateDQM  = True
#ValidateHLT  = False
#ValidateRECO = False
#ValidateISO  = False 
#ValidateDQM  = False 


# Samples for Validation

# For No PU
samples= ['RelValSingleMuPt100','RelValZMM','RelValZpMM','RelValWpM','RelValUpsilon1SToMuMu','RelValTTbar','RelValSingleMuPt10','RelValSingleMuPt1000']

#samples= ['RelValUpsilon1SToMuMu','RelValTTbar','RelValSingleMuPt10']#samples= ['RelValWpM','RelValUpsilon1SToMuMu']
#samples= ['RelValSingleMuPt10']
#samples= ['RelValZpMM','RelValUpsilon1SToMuMu']
#samples= ['RelValZMM']

# For PU 25 ns and 50 ns
#samples= ['RelValTTbar','RelValZMM']
#samples= ['RelValTTbar','RelValZpMM','RelValWpM','RelValUpsilon1SToMuMu']
# For No PU FastSim

#samples= ['RelValJpsiMuMu_Pt-8']
#samples=['RelValSingleMuPt10','RelValSingleMuPt100','RelValTTbar','RelValZMM']
#samples=['RelValZMM']
#samples= ['RelValTTbar','RelValZMM']
#############################################################

