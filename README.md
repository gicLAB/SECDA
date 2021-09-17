
# SECDA: Efficient Hardware/Software Co-Design of FPGA-based DNN Accelerators for Edge Inference
SECDA is a hardware-software co-design methodology for efficiently designing and implementing FPGA-based DNN accelerators. The methodology employs SystemC simulation in parallel with accelerator driver co-design and early system integration, reducing the length of each design iteration cycle, therefore, reducing the time to deliver the final design.

# Paper
Our research paper covers the methodology in detail including our initial case study using SECDA. If you are using the SECDA methodology in research, we kindly request a reference to the following:

J. Haris, P. Gibson, J. Cano, N. Bohm Agostini, D. Kaeli. SECDA: Efficient Hardware/Software Co-Design of FPGA-based DNN Accelerators for Edge Inference. 33rd IEEE International Symposium on Computer Architecture and High Performance Computing (SBAC-PAD), Belo Horizonte, Brazil, October 2021.

# SECDA-TFLite
This repo contains the work for our initial case study using the SECDA methodology where we developed and implemented two GEMM accelerators, the Systolic Array (SA) and the Vector Mac (VM) designs. Both designs can be deployed for CONV2D layers during TFLite inference. Here we publish the source code to reproduce our methodology for the  TFLite-GEMM case study that we highlight in our paper, along with the two accelerator designs. 

## PYNQ-Z1 TFLite Inference with Accelerators

## Accelerator Designs & Synthesis 

## Testbench Simulation

## End-to-end Simulation

