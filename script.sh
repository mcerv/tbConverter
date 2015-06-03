#!/bin/bash






#PATH_IN=/Users/Administrador/Desktop/dut_october/method1_m100_testbeam/method1_251014_RUN000016c.txt
#PATH_OUT=/Users/Administrador/Desktop/dut_october/method1_m100_testbeam/method1_251014_RUN000016c.root

PATH=/Users/Administrador/Desktop/dut_october/method1_m100_testbeam/*


#./tbConverter -c converttexttoroot -f configs/diamong.cfg -i $PATH_IN -o $PATH_OUT

for file in $PATH
do
    echo "procesing $file"
done