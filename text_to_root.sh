#!/bin/bash





##=====for one file:
PATH=/Users/Administrador/Documents/MAPS/XTB01/testbeams_analysis/SPSnovember/exponential/DUT/Waveform_analysis/txt/2014-11-24_RUN000001c.txt
./tbConverter -c converttexttoroot -f configs/diamong.cfg -i $PATH -o ${PATH%.*}.root


##====for a folder:
#IN=/Users/Administrador/Documents/MAPS/XTB01/testbeams_analysis/SPSnovember/exponential/DUT/Waveform_analysis/txt/*
#for file in $IN
#do
#echo "procesing ${file%.*}.root"
#    ./tbConverter -c converttexttoroot -f configs/diamong.cfg -i $file -o ${file%.*}.root
#done