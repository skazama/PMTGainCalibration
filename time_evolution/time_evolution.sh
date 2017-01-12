pre_file=`cat input.txt | tail -2 | head -1`
current_file=`cat input.txt | tail -1`
pre_time=`cat input.txt | tail -2 | head -1 | cut -c 30-100 | sed -e "s/\.root//"`
current_time=`cat input.txt | tail -1 | cut -c 30-100 | sed -e "s/\.root//"` 

./time_evolution input.txt ${pre_file} ${current_file} test.root ${pre_time} ${current_time}
