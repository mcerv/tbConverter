#!/bin/bash



IN=/Volumes/WD_Elements/SPS_november/dut_data/tbc/*
#OUT=/Users/Administrador/Documents/MAPS/XTB01/testbeams_data/SPSnovember/osc_converted/


for file in $IN
do


echo "procesing ${file%.*}"
./tbConverter -c convertbintotext -f configs/diamond.cfg -i $file -o ${file%.*}c
done


#IN=/Volumes/WD_Elements/XFAB/all_november/2014-11-24_RUN000022
#OUT=/Volumes/WD_Elements/XFAB/all_november/2014-11-24_RUN000022c

#./tbConverter -c convertbintotext -f configs/diamond.cfg -i $IN -o $OUT