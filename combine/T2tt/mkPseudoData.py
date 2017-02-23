#!/usr/bin/env python

import json
import sys
import ROOT
import optparse
import hwwtools as hwwtools
#import LatinoAnalysis.Gardener.hwwtools as hwwtools
import logging
import os.path
import math

# ----------------------------------------------------- PseudoDataFactory --------------------------------------

class PseudoDataFactory:
    _logger = logging.getLogger('PseudoDataFactory')

    # _____________________________________________________________________________
    def __init__(self):
      
        variables = {}
        self._variables = variables

        cuts = {}
        self._cuts = cuts

        samples = {}
        self._samples = samples

        ## list of [processes]  
        self.processes = []
        ## list of [signal processes]
        self.signals = []
        ## list of [background processes]
        self.backgrounds = []
        ## data
        self.data = []        
        ## list of [(name of uncert, type of nuisance, list of samples affected, with value, additional value [in case of gmN])]
        self.systs   = []

    # _____________________________________________________________________________
    # 
    # _____________________________________________________________________________
    def makePseudoData( self, inputDir, outputDir, outputFileName, analysisName, lumi, variables, cuts, samples, structureFile, nuisances):
    
        print "=============================="
        print "======= makePseudoData ======="
        print "=============================="
        
        self._variables = variables
        self._samples   = samples
        self._cuts      = cuts

        self._inputDir  = inputDir
        self._outputDir = outputDir

        #self._fileIn = ROOT.TFile(inputFile, "READ")
        
        # divide the list of samples among signal, background and data
        for sampleName, sample in self._samples.iteritems():
          if structureFile[sampleName]['isSignal'] == 1 :
            self.signals.append(sampleName)
          if structureFile[sampleName]['isData'] == 1 :
            self.data.append(sampleName)
          if structureFile[sampleName]['isSignal'] == 0 and structureFile[sampleName]['isData'] == 0:
            self.backgrounds.append(sampleName)
          
        #if not os.path.isdir (self._outputDir + "/") :
        #  os.mkdir (self._outputDir + "/")
          
        self._outFile = ROOT.TFile.Open( self._inputDir + '/nominal/' + analysisName + '/' + outputFileName + ".root", 'recreate')
        ROOT.TH1.SetDefaultSumw2(True)
        
        self._outFile.mkdir (analysisName)

        # loop over cuts to create the output directories
        for cutName in self._cuts :

          cutNameSplit = cutName.split('_')

          if (cutNameSplit[2]=="em"):
              cutDirectory = analysisName + '/02_' + cutNameSplit[0] + '_' + cutNameSplit[1]
              self._outFile.mkdir (cutDirectory)

        # loop over cuts  
        for cutName in self._cuts :

          cutNameSplit = cutName.split('_')

          print "cut = ", cutName, " :: ", cuts[cutName]
          cutDirectory = analysisName + '/02_' + cutNameSplit[0] + '_' + cutNameSplit[1]

          # loop over variables
          for variableName, variable in self._variables.iteritems():
 
            shapeName = cutDirectory + '/h_' + variableName + '_' + cutNameSplit[2]

            print "  variableName = ", variableName
            tagNameToAppearInDatacard = cutName

            # by hand for the time being
            histoOutName = 'h_' + variableName + '_' + cutNameSplit[2]
            histoOut = ROOT.TH1F(histoOutName, '', 7, 0., 140.)
          
            #for sampleName in self.signals:
             # inputFile = inputDir + '/nominal/' + analysisName + '/' + sampleName + '.root'
             # fileIn = ROOT.TFile(inputFile, "READ")
             # print shapeName
             # histo = fileIn.Get(shapeName)
             # histo.SetName('histo_' + sampleName)
             # #histo.Scale(lumi)
             # histo = self._checkBadBins(histo)
             # histoOut.Add(histo)
             # fileIn.Close()
              
            for sampleName in self.backgrounds:
              print "sample = ", sampleName
              inputFile = inputDir + '/nominal/' + analysisName + '/' + sampleName + '.root'
              fileIn = ROOT.TFile(inputFile, "READ")
              histo = fileIn.Get(shapeName)
         #     histo.SetName('histo_' + sampleName)
              #histo.Scale(lumi)
              histo = self._checkBadBins(histo)
              #if sampleName == "06_WW" : # VRtest4
                #  histo.Scale(1.1)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.00)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*0.99)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*0.98)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*0.96)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*0.94)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*0.92)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*0.90)
              #if sampleName == "05_ST" :
                #  histo.Scale(0.85)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*0.98)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*0.96)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*0.93)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*0.89)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*0.85)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*0.80)
              #if sampleName == "04_TTTo2L2Nu" :
                #  histo.Scale(0.80)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*0.98)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*0.96)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*0.93)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*0.89)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*0.85)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*0.80)
              #if sampleName == "06_WW" : # VRtest5
                #  histo.Scale(1.)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.000)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*1.017)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*1.032)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*1.050)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*1.067)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*1.082)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*1.100)
              #if sampleName == "05_ST" :
                #  histo.Scale(1.)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.000)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*1.017)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*1.032)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*1.050)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*1.067)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*1.082)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*1.100)
              #if sampleName == "04_TTTo2L2Nu" :
                #  histo.Scale(1.)
                #  histo.SetBinContent(1, histo.GetBinContent(1)*1.000)
                #  histo.SetBinContent(2, histo.GetBinContent(2)*1.017)
                #  histo.SetBinContent(3, histo.GetBinContent(3)*1.032)
                #  histo.SetBinContent(4, histo.GetBinContent(4)*1.050)
                #  histo.SetBinContent(5, histo.GetBinContent(5)*1.067)
                #  histo.SetBinContent(6, histo.GetBinContent(6)*1.082)
                #  histo.SetBinContent(7, histo.GetBinContent(7)*1.100)
              histoOut.Add(histo)
              fileIn.Close()

            nBins = histoOut.GetNbinsX()
            for iBin in range(1, nBins+1):

                yValue = histoOut.GetBinContent(iBin)
                histoOut.SetBinError(iBin, math.sqrt(abs(yValue)))

            self._outFile.cd(cutDirectory)
            histoOut.Write()


    def _checkBadBins(self, histo):

        histoIntegral = histo.Integral()
        nBins = histo.GetNbinsX()
        for iBin in range(1, nBins+1):
            yValue = histo.GetBinContent(iBin)
            if yValue <= 0. :
                histo.SetBinContent(iBin, 0.001)
        histoIntegralCorrected = histo.Integral()
        if (histoIntegralCorrected!=histoIntegral and histoIntegral != 0) :
            histo.Scale(histoIntegralCorrected/histoIntegral)
        return histo
            

if __name__ == '__main__':
    print 'SHAPE MAKER'

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)

    parser.add_option('--tag'                , dest='tag'               , help='Tag used for the shape file name'           , default=None)
    parser.add_option('--sigset'             , dest='sigset'            , help='Signal samples [SM]'                        , default='SM')
    parser.add_option('--inputDir'           , dest='inputDir'          , help='input directory'                            , default='./')
    parser.add_option('--outputDir'          , dest='outputDir'         , help='output directory'                           , default='./')
    parser.add_option('--outputFileName'     , dest='outputFileName'    , help='output file name'                           , default='01_PseudoDataSmeared')
    parser.add_option('--analysisName'       , dest='analysisName'      , help='analysis name'                              , default='Stop')
    parser.add_option('--structureFile'      , dest='structureFile'     , help='file with datacard configurations'          , default=None )
    parser.add_option('--nuisancesFile'      , dest='nuisancesFile'     , help='file with nuisances configurations'         , default=None )

    # read default parsing options as well
    hwwtools.addOptions(parser)
    hwwtools.loadOptDefaults(parser)
    (opt, args) = parser.parse_args()

    sys.argv.append( '-b' )
    ROOT.gROOT.SetBatch()


    print " configuration file = ", opt.pycfg
    
    print " inputDir       =          ", opt.inputDir
    print " outputDir      =          ", opt.outputDir
    print " outputFileName =          ", opt.outputFileName
 
    if not opt.debug:
        pass
    elif opt.debug == 2:
        print 'Logging level set to DEBUG (%d)' % opt.debug
        logging.basicConfig( level=logging.DEBUG )
    elif opt.debug == 1:
        print 'Logging level set to INFO (%d)' % opt.debug
        logging.basicConfig( level=logging.INFO )
     
    factory = PseudoDataFactory()
    #factory._energy    = opt.energy
    factory._lumi      = opt.lumi
    factory._tag       = opt.tag
    
    # ~~~~
    variables = {}
    if os.path.exists(opt.variablesFile) :
      handle = open(opt.variablesFile,'r')
      exec(handle)
      handle.close()
    
    cuts = {}
    if os.path.exists(opt.cutsFile) :
      handle = open(opt.cutsFile,'r')
      exec(handle)
      handle.close()
    
    samples = {}
    if os.path.exists(opt.samplesFile) :
      handle = open(opt.samplesFile,'r')
      exec(handle)
      handle.close()
   
    # ~~~~
    structure = {}
    if opt.structureFile == None :
       print " Please provide the datacard structure "
       exit ()
       
    if os.path.exists(opt.structureFile) :
      handle = open(opt.structureFile,'r')
      exec(handle)
      handle.close()


    # ~~~~
    nuisances = {}
    if opt.nuisancesFile == None :
       print " Please provide the nuisances structure if you want to add nuisances "
       
    if os.path.exists(opt.nuisancesFile) :
      handle = open(opt.nuisancesFile,'r')
      exec(handle)
      handle.close()
        
    factory.makePseudoData( opt.inputDir ,opt.outputDir, opt.outputFileName, opt.analysisName, opt.lumi, variables, cuts, samples, structure, nuisances)
    
        
        
