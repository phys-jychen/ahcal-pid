#! /bin/bash
source /home/chenjiyuan/conda.env

energy=100
particle="neutron"
data_type="MC"
data_source="SJTU"

filename="/lustre/collider/chenjiyuan/ahcal-pid/run/new_geo/selected/training/sel_${particle}"
#tree=Calib_Hit
event_index=2024
save_dir=/lustre/collider/chenjiyuan/ahcal-pid/figs/
output="EventDisplay_${particle}"
#show=0

title="${energy}-GeV"
if [ $particle = "e" ]
then
    title+=' $e^-$ ('
    filename+='-'
    output+='-'
elif [ $particle = "mu" ]
then
    title+=' $\mu^-$ ('
    filename+='-'
    output+='-'
elif [ $particle = "pi" ]
then
    title+=' $\pi^-$ ('
    filename+='-'
    output+='-'
elif [ $particle = "neutron" ]
then
    title+=' $n$ ('
elif [ $particle = "proton" ]
then
    title+=' $p$ ('
fi
filename+="_${energy}GeV.root"
output+="_${energy}GeV_"
if [ $data_type = "data" ] || [ $data_type = "Data" ]
then
    title+='Data, 2023)'
    output+='Data2023.pdf'
elif [ $data_type = "MC" ]
then
    title+="MC, $data_source)"
    output+="MC_${data_source}.pdf"
fi

#python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o=$output -s=$show
python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o="$output"
