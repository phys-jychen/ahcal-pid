#! /bin/bash
source /home/chenjiyuan/conda.env

filename=/lustre/collider/chenjiyuan/ahcal-pid/run/new_geo/selected/training/sel_pi-_100GeV.root
#tree=Calib_Hit
title='100-GeV $\pi^-$'
event_index=2023
save_dir=/lustre/collider/chenjiyuan/ahcal-pid/figs/
output=EventDisplay_pi-_100GeV.pdf
#show=0

#python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o=$output -s=$show
python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o=$output
