cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00010_camille_0.70_combined.txt s010.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00020_camille_0.86_combined.txt s020.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00050_camille_0.94_combined.txt s050.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00100_camille_0.96_combined.txt s100.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00200_camille_0.98_combined.txt s200.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00300_camille_0.99_combined.txt s300.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001scalar00500_camille_0.99_combined.txt s500.txt

cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00010_camille_0.99_combined.txt p010.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00020_camille_0.97_combined.txt p020.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00050_camille_0.97_combined.txt p050.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00100_camille_0.99_combined.txt p100.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00200_camille_0.99_combined.txt p200.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00300_camille_0.99_combined.txt p300.txt
cp /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/ttDM0001pseudo00500_camille_0.99_combined.txt p500.txt



text2workspace.py s010.txt
combineTool.py -M Impacts -d s010.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s010.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s010.root -m 1  -o impacts_s010.json
plotImpacts.py -i impacts_s010.json -o impacts_s010

text2workspace.py s020.txt
combineTool.py -M Impacts -d s020.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s020.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s020.root -m 1  -o impacts_s020.json
plotImpacts.py -i impacts_s020.json -o impacts_s020

text2workspace.py s050.txt
combineTool.py -M Impacts -d s050.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s050.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s050.root -m 1  -o impacts_s050.json
plotImpacts.py -i impacts_s050.json -o impacts_s050

text2workspace.py s100.txt
combineTool.py -M Impacts -d s100.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s100.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s100.root -m 1  -o impacts_s100.json
plotImpacts.py -i impacts_s100.json -o impacts_s100

text2workspace.py s200.txt
combineTool.py -M Impacts -d s200.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s200.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s200.root -m 1  -o impacts_s200.json
plotImpacts.py -i impacts_s200.json -o impacts_s200

text2workspace.py s300.txt
combineTool.py -M Impacts -d s300.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s300.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s300.root -m 1  -o impacts_s300.json
plotImpacts.py -i impacts_s300.json -o impacts_s300

text2workspace.py s500.txt
combineTool.py -M Impacts -d s500.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s500.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d s500.root -m 1  -o impacts_s500.json
plotImpacts.py -i impacts_s500.json -o impacts_s500



text2workspace.py p010.txt
combineTool.py -M Impacts -d p010.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p010.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p010.root -m 1  -o impacts_p010.json
plotImpacts.py -i impacts_p010.json -o impacts_p010

text2workspace.py p020.txt
combineTool.py -M Impacts -d p020.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p020.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p020.root -m 1  -o impacts_p020.json
plotImpacts.py -i impacts_p020.json -o impacts_p020

text2workspace.py p050.txt
combineTool.py -M Impacts -d p050.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p050.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p050.root -m 1  -o impacts_p050.json
plotImpacts.py -i impacts_p050.json -o impacts_p050

text2workspace.py p100.txt
combineTool.py -M Impacts -d p100.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p100.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p100.root -m 1  -o impacts_p100.json
plotImpacts.py -i impacts_p100.json -o impacts_p100

text2workspace.py p200.txt
combineTool.py -M Impacts -d p200.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p200.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p200.root -m 1  -o impacts_p200.json
plotImpacts.py -i impacts_p200.json -o impacts_p200

text2workspace.py p300.txt
combineTool.py -M Impacts -d p300.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p300.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p300.root -m 1  -o impacts_p300.json
plotImpacts.py -i impacts_p300.json -o impacts_p300

text2workspace.py p500.txt
combineTool.py -M Impacts -d p500.root -m 1 --doInitialFit --robustFit 1 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p500.root -m 1 --robustFit 1 --doFits --parallel 5 -t -1 --expectSignal=1
combineTool.py -M Impacts -d p500.root -m 1  -o impacts_p500.json
plotImpacts.py -i impacts_p500.json -o impacts_p500



cp impacts_*.pdf /afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/170803/impacts/
