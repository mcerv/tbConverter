#!/bin/bash



#IN=/Volumes/WD_Elements/SPS_november/dut_data/tbc/*
#OUT=/Users/Administrador/Documents/MAPS/XTB01/testbeams_data/SPSnovember/osc_converted/


#for file in $IN
#do


#echo "procesing ${file%.*}"
#./tbConverter -c convertDrsToText -f configs/diamond.cfg -i $file -o ${file%.*}c
#done


IN=/Volumes/Sonias_Backup/Testbeam_may2015/DUT/20150522_diamond_500v_30deg_run34.dat
OUT=/Volumes/Sonias_Backup/Testbeam_may2015/DUT/run34/
#20150522_diamond_500v_30deg_run34.txt

./tbConverter -c convertdrstotext -f configs/diamond.cfg -i $IN -o $OUT