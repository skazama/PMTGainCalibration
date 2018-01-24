#!/bin/sh
dir=`pwd`
echo "Your working dir is $dir"
echo ""

### rootfiles ###
rootfile_dir="${dir}/rootfile"
if ! test -d $rootfile_dir
then
	echo "creating a dir for saving histograms: $rootfile_dir"
	mkdir $rootfile_dir
fi 

### logfiles ###
if ! test -d $dir/logfiles
then
	echo "creating a dir for saving batch job logfiles: $dir/logfiles"
	mkdir $dir/logfiles
fi 

### batch job scripts ###
batch_dir="${dir}/batch"
if ! test -d $dir/batch
then
        echo "creating a dir for saving batch job scripts: $batch_dir"
        mkdir $batch_dir
fi
echo ""

### a file which contains RunNumber, bg_flag(BG=1, LED ON=0), dataset_name
run_file="run.txt"

while read line
do
	for end_pos in 175 177 179 181 183 185
	do
	        RunNumber=`echo $line | awk '{print $1}'`
	       	bg_flag=`echo $line | awk '{print $2}'`
		filename="${RunNumber}_${end_pos}"

		today=`date +"%Y%m%d_%I%M" | sed -e "s/2016/16/"`	

		#echo "You are creating a job script for Run${RunNumber}, Integration Length = [130,${end_pos}]"

		### bathc job scripts ###	
		echo "#!/bin/sh"                                                    > ${batch_dir}/sbatch_${filename}.sh
		echo "  "                                                          >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --job-name=gain_calib_${filename}.sh"                >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --output=${dir}/logfiles/${filename}.o"              >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --error=${dir}/logfiles/${filename}.e"               >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --time=24:00:00"                                     >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --ntasks=1"                                          >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --cpus-per-task=1"                                   >> ${batch_dir}/sbatch_${filename}.sh
		echo "#SBATCH --account=pi-lgrandi"                                >> ${batch_dir}/sbatch_${filename}.sh 
	       	echo "#SBATCH --qos=xenon1t"                                       >> ${batch_dir}/sbatch_${filename}.sh
        	echo "#SBATCH --partition=xenon1t"                                 >> ${batch_dir}/sbatch_${filename}.sh

		echo "  "                                                          >> ${batch_dir}/sbatch_${filename}.sh
		echo "srun /bin/sh ${batch_dir}/gain_calib_${filename}.sh"         >> ${batch_dir}/sbatch_${filename}.sh

		### job scripts ###
		echo "cd ${dir}"                                                    > ${batch_dir}/gain_calib_${filename}.sh 
                echo "export PATH=\"/project/lgrandi/anaconda3/bin:\$PATH\""       >> ${batch_dir}/gain_calib_${filename}.sh
                echo "source activate pax_head"                                    >> ${batch_dir}/gain_calib_${filename}.sh

                echo "host=\`hostname | sed -e \"s/-/ /\" | awk '{print \$1}'\`"   >> ${batch_dir}/gain_calib_${filename}.sh
                echo "scratch=\"/scratch/\${host}/${USER}/qjob\""                  >> ${batch_dir}/gain_calib_${filename}.sh

                echo "mkdir -p \${scratch}/${today}"                               >> ${batch_dir}/gain_calib_${filename}.sh
                echo "mkdir -p \${scratch}/${today}/${filename}"                   >> ${batch_dir}/gain_calib_${filename}.sh

		echo "cp MakeHist.py \${scratch}/${today}/${filename}"             >> ${batch_dir}/gain_calib_${filename}.sh
		echo "cd \${scratch}/${today}/${filename}"                         >> ${batch_dir}/gain_calib_${filename}.sh

	       	echo "python MakeHist.py $RunNumber $bg_flag ${end_pos}"           >> ${batch_dir}/gain_calib_${filename}.sh
	        echo "cp *.root ${rootfile_dir}"                                   >> ${batch_dir}/gain_calib_${filename}.sh

		echo "rm -rf \${scratch}/${today}/${filename}"                     >> ${batch_dir}/gain_calib_${filename}.sh 
	done
done  < ${run_file}

for job in `ls ${batch_dir}/sbatch_*sh`
do
	echo "sbatch ${job}"
done > job_submit.sh

echo ""
echo "If there are no problems with batch job scripts, then submit jobs with the following command" 
echo ". job_submit.sh" 
echo ""

