import numpy as np
import ROOT

### hax ###
import hax
hax.init(raw_data_access_mode = 'local',
         raw_data_local_path = ['/project/lgrandi/xenon1t/raw/',
                                '/project2/lgrandi/xenon1t/raw/', 
                                '/home/kazama/scratch-midway/raw/',
                                '/home/kazama/scratch-midway2/raw/'])
import sys
argvs = sys.argv

my_dataset = int(argvs[1])
bg_flag = int(argvs[2])

start_sample = 130
end_sample = int(argvs[3])

subdataset = hax.runs.datasets[hax.runs.datasets.number == my_dataset]

info = hax.runs.get_run_info(my_dataset)
time = info['trigger']['start_timestamp']

#n_events = int(subdataset.iloc[0]['trigger__events_built'])
n_events = 190000

output = subdataset.iloc[0]['name']
outputfile = output + "_" + argvs[3] + ".root"

### Saving timestamp as TBranch in TFile ###
from ROOT import gROOT, AddressOf

gROOT.ProcessLine(
"struct MyStruct {\
   Long64_t   ftimestamp;\
};" );

from ROOT import MyStruct
mystruct = MyStruct()

f = ROOT.TFile(outputfile, "recreate")
t = ROOT.TTree("tree", "tree")
t.Branch( 'timestamp', mystruct, 'timestamp/L' )
mystruct.ftimestamp = info['trigger']['start_timestamp']
t.Fill()

histos  = []

for ipmt in range(0,254):
    
    if bg_flag == 1:

        name = "BG_Area_of_PMT%d_EndPos%d" % (ipmt, end_sample)
        histo = ROOT.TH1F(name, name, 25000, -10000, 15000)
        histos.append(histo)   

    if bg_flag == 0:
    
        name = "Signal_Area_of_PMT%d_EndPos%d" % (ipmt, end_sample)
        histo = ROOT.TH1F(name, name, 25000, -10000, 15000)
        histos.append(histo)

### Create histograms ###
for ievt, event in enumerate(hax.raw_data.raw_events(my_dataset, event_numbers=np.arange(0,n_events))):

    if(ievt % 1000 == 0):
        print ("Event No", ievt)
    
    for p in event.pulses:
        
        Area = 0.0
        Baseline = 0.0
        Baseline_ini = 20
        Baseline_end = 120
        Baseline_samples = Baseline_end - Baseline_ini + 1
        Reference_baseline = 16000.0

        raw_data_adc = p.raw_data.astype(np.float64)
        raw_data_adc = Reference_baseline - raw_data_adc

        for adc in raw_data_adc[Baseline_ini:Baseline_end+1]:

            Baseline += adc

        Baseline /= Baseline_samples

        for adc in raw_data_adc[start_sample:end_sample+1]:

            Area += adc - Baseline

        histos[p.channel].Fill(Area)


f.Write()
f.Close()
