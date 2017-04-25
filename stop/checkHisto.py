import os
import ROOT as r
import optparse, subprocess



class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'




##Main body of the analysis
if __name__ == '__main__':

    print bcolors.HEADER
    print '#######################################################################'
    print '             Checking all root files                                   '
    print '#######################################################################' + bcolors.ENDC

    parser = optparse.OptionParser(usage='usage: %prog [opts] FilenameWithSamples', version='%prog 1.0')
    parser.add_option('-d', '--directory', action='store', type=str, dest='theDir', default='.', help='Directory where the root files are contained')
    parser.add_option('-l', '--logs', action='store', type=str, dest='theLog', default='.', help='Directory where the logs are contained')
    (opts, args) = parser.parse_args()

    theDir = opts.theDir
    theLog = opts.theLog

    outputA = subprocess.Popen("ls " + theDir + "*.root | grep root", stdout=subprocess.PIPE, shell=True)
    outputB = outputA.communicate()[0]
    output = outputB.split("\n")
 
    output2 = output[0:len(output)-1]
    for i in output2:
        
        f = r.TFile(i)
        if f.IsZombie():
            print bcolors.FAIL, "File ", i, " is broken" + bcolors.ENDC
            nameOfFile = i[len(theDir)+1:len(i)]
            theoutputA = subprocess.Popen("grep -r " + nameOfFile + " " + theLog + "/*", stdout=subprocess.PIPE, shell=True)
            theoutputB = theoutputA.communicate()[0]
            theoutput = theoutputB.split("\n")
            if theoutput == ['']:            
                print bcolors.FAIL, "The LSF directory processing filename ", bcolors.OKBLUE, nameOfFile, bcolors.FAIL, "was not found", bcolors.ENDC
            ####Si lo encuentra, imprimir que directorio es el culpable
            # else 
            #   print bcolors.WARNING, " This is the guilty!!\n" , bcolors.BOLD, theoutput, bcolors.ENDC
        else:          
            print bcolors.OKGREEN, "File ", i, " is nice" + bcolors.ENDC


