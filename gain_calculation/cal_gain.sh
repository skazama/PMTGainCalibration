for run in `cat runs.list`
do
        bg_file=`ls -d ../make_hist/rootfile/${run}* | head -n 1`
        mode1=`ls -d ../make_hist/rootfile/${run}* | head -n 2 | tail -n 1`
        mode2=`ls -d ../make_hist/rootfile/${run}* | head -n 3 | tail -n 1`
        mode3=`ls -d ../make_hist/rootfile/${run}* | head -n 4 | tail -n 1`

        output1=`echo $mode1 | sed -e "s/\.\.\/make_hist\/rootfile\///"`
        output2=`echo $mode2 | sed -e "s/\.\.\/make_hist\/rootfile\///"`
        output3=`echo $mode3 | sed -e "s/\.\.\/make_hist\/rootfile\///"`

        bg_name=`echo $bg_file | sed -e "s/\.\.\/make_hist\/rootfile\///" | sed -e "s/\.root//"`
	
        ./cal_gain $bg_file ${mode1} rootfile/$output1 0 $bg_name
        ./cal_gain $bg_file ${mode2} rootfile/$output2 1 $bg_name
        ./cal_gain $bg_file ${mode3} rootfile/$output3 2 $bg_name

        hadd -f rootfile/${run}.root rootfile/$output3 rootfile/$output2 rootfile/$output1
	rm rootfile/$output1 rootfile/$output2 rootfile/$output3

	gain_file=`ls txt/gain*_2_*${bg_name}.txt | sed -e "s/_2_//" | sed -e "s/${bg_name}//"`
	cat txt/gain*_2_*${bg_name}.txt txt/gain*_1_*${bg_name}.txt txt/gain*_0_*${bg_name}.txt > ${gain_file}
	rm txt/gain*_*_*${bg_name}.txt
done
