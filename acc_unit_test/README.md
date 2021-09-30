# PYNQ Accelerator Unit Test
Accelerator Unit Test is a simple program to run/test the Systolic & Vector MAC accelerators on a simple set of input data.

**Instructions**

* Simply compile by using "make", requires arm-linux-gnueabihf-g++ to cross compile for the PYNQ Z1
* After "make" copy the "acc_unit_test" program to the PYNQ device
* sudo ./acc_unit_test -i {dir/to/input_files}  -s [0|1]
* "-s" flag followed by 0 or 1 indicates the accelerator you are using, 0: SA, 1: VM
* "-i" flag specify the path to the folder containing all input data files
* Example call "sudo ./acc_unit_test -i input_data/model_l1_iw_i0/ -s 0"

**Input Data** 
* For sample input data files download [input_data](https://github.com/gicLAB/SECDA/releases/download/v1.0/input_data.zip)
* Extract the file to get two sample data folders "model_l0_w0_i0" and "model_l5_w0_i0"
* Directing "i" flag to either of these folders will let the accelerators load and perform GEMM on the sample data set
* Input data sets are currently accelerator independant
<!-- * You are able to generate these input data sets using SECDA-TFLite End to end Simulation (https://github.com/gicLAB/SECDA#end-to-end-simulation) -->




