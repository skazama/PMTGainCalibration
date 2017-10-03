
#!/bin/bash

if [ "$1" != "" ]; then
    DIR=$1
else
    echo "Usage: ./`basename "$0"` <step0 run name> <step1 run name> <step2 run name> <step3 run name> <step4 run name>"
    exit 1
fi



DIRNAME=(${1//_/ })

mkdir -p "led_raw_data_${DIRNAME}"
export PATH="/project/lgrandi/anaconda3/bin:$PATH"
source activate pax_head
source /cvmfs/xenon.opensciencegrid.org/software/rucio-py26/setup_rucio_1_8_3.sh
export RUCIO_ACCOUNT=xenon-analysis
export X509_USER_PROXY=/project/lgrandi/xenon1t/grid_proxy/xenon_service_proxy
source /cvmfs/xenon.opensciencegrid.org/software/rucio-py27/setup_rucio_1_8_3.sh
source /cvmfs/oasis.opensciencegrid.org/osg-software/osg-wn-client/3.3/current/el7-x86_64/setup.sh


cd "led_raw_data_${DIRNAME}"
for run_name in "$@"
do
    mkdir -p $run_name
    cd $run_name
    if [ -d "$DIRECTORY" ]; then
        continue
    fi
    rucio download "x1t_SR001_${run_name}_tpc:raw"
    mv raw/* .
    cd ..
done
