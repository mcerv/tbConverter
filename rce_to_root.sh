#!/bin/bash



#PATH=/Users/Administrador/Desktop/dut_october/method1_m100_testbeam/files/*


#for file in $PATH
#do
#echo "procesing ${file%.*}.root"
#   ./tbConverter -c convertrcetoroot -f configs/RCE.cfg -i $file -o ${file%.*}.root
#done


IN=/Volumes/Elements_1/SPS_october/cosmicData/2014-10-30/cosmic_000003/cosmic_000003_000000.dat
OUT=/Users/Administrador/Documents/MAPS/XTB01/testbeams_data/SPSoctober/cosmic_data/lasttbconverter/2014-10-30_run3

./tbConverter -c convertrcetoroot -f configs/RCE.cfg -i $IN -o ${OUT%.*}.root