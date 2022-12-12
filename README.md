# SECDA: Efficient Hardware/Software Co-Design of FPGA-based DNN Accelerators for Edge Inference
SECDA is a hardware-software co-design methodology for efficiently designing and implementing FPGA-based DNN accelerators. The methodology employs SystemC simulation in parallel with accelerator driver co-design and early system integration, reducing the length of each design iteration cycle, therefore, reducing the time to deliver the final design.

# Paper
Our research paper covers the methodology in detail including our initial case study using SECDA. If you are using the SECDA methodology in research, we kindly request a reference to the following:

```
@inproceedings{Haris2021SBACPAD,
      title={{SECDA: Efficient Hardware/Software Co-Design of FPGA-based DNN Accelerators for Edge Inference}}, 
      author={Jude Haris and Perry Gibson and José Cano and Nicolas Bohm Agostini and David Kaeli},
      booktitle = {SBAC-PAD},
      year={2021}
}
```

# SECDA Case-Study
This repo contains the work for our initial case study using the SECDA methodology. We developed and implemented two GEMM accelerators, the Systolic Array (SA) and the Vector Mac (VM) designs. Both designs can execute CONV2D layers during TFLite inference. Here we publish the source code to reproduce our methodology for the TFLite-GEMM case study that we highlight in our paper, along with the two accelerator designs.

## End-to-end Simulation
TFLite-SECDA End-to-end simulation enables us to perform inference on TFLite CNN models while simulating and profiling our GEMM accelerators. For ease of installation, we provide a fully contained docker image.

**Requirements**
* Docker

**Recommendation**
* VSCode: to connect to the docker container and load up the pre-configured SECDA workspace using "open workspace from file" option in the VSCode File menu.

**Instructions**
* First pull the docker image: 
```
docker pull judeharis97/secda-tflite:v1
```
* Simply create a container of the downloaded image using the following command: 
```
docker run -it -d --name secda-tflite-uint8 judeharis97/secda-tflite:v1
```
* Once the container is created and launched, you can access it through [VSCode's attach to container functionality](https://code.visualstudio.com/docs/remote/attach-container)
* Load VSCode workspace at `/root/workspace/tensorflow/workspace.code-workspace`
*  Once the VSCode workspace is loaded, you are able to run to the launch configurations through the [Run and Debug](https://code.visualstudio.com/docs/editor/debugging) tab to use the End to end Simulation
* The "Run and Debug" tab contains two launch configurations seen in the top left of the tab: `(gbd) Label Image VM (tensorflow)` and `(gbd) Label Image VM (tensorflow)`, these launch the End to end simulation using the VM and SA accelerators respectively
* These configurations are stored within `/root/Workspace/tensorflow/.vscode/launch.json` (to launch) and `/root/Workspace/tensorflow/.vscode/task.json` (to compile), you can edit these to change the parameters to compile and launch the End to end Simulation.
* Input files such as images and tflite models can be added/edited through the launch configurations (checkout `/root/workspace/tensorflow/inputs_models` for more)
* After simulation, a host of profiling data regarding the inference will be stored at `/root/workspace/tensorflow/out`


## Accelerator Designs
* We provide pre-compiled binaries/bitstream for the PYNQ Z1 along with archived Vivado and Vivado HLS project folders ([release](https://github.com/gicLAB/SECDA/releases/tag/v1.0)) to enable synthesis from scratch
* We also provide source code for both accelerators
* For more information please check out the [accelerator source-code](accelerators/) or the section below


## Testbench Simulation & Synthesis for PYNQ-Z1
* To perform Testbench simulation, we provide archived Vivado HLS project folders ([VM](https://github.com/gicLAB/SECDA/releases/download/v1.0/vm_uint8_v2.zip),[SA](https://github.com/gicLAB/SECDA/releases/download/v1.0/sa_uint8_v2.zip)) which contain everything required to perform testbench simulation once opened in Vivado HLS

* To perform logic synthesis, we provide Vivado project folders ([VM](https://github.com/gicLAB/SECDA/releases/download/v1.0/vm_uint8_v2.xpr.zip),[SA](https://github.com/gicLAB/SECDA/releases/download/v1.0/sa_uint8_v3.xpr.zip)). These contain the necessary block diagram configuration including AXI DMA's and the accelerator to ensure correct connectivity to the processing system.


**Requirements**
* Vivado HLS 2019.2
* Vivado 2019.2

**Instructions**
To perform Testbench simulation or create Vivado IP from SystemC source code, do the following:
* Unzip ([VM](https://github.com/gicLAB/SECDA/releases/download/v1.0/vm_uint8_v2.zip),[SA](https://github.com/gicLAB/SECDA/releases/download/v1.0/sa_uint8_v2.zip)) the accelerator project you want to use.
* Load up Vivado HLS and choose the project folder within the unzipped folder to open the pre-configured Vivado project and solution.
* Here, we can ask Vivado HLS to perform SystemC simulation. It can also perform HLS and export RTL using the menu bar.
* For logic synthesis, simply open up Vivado and loaded up ([VM](https://github.com/gicLAB/SECDA/releases/download/v1.0/vm_uint8_v2.xpr.zip),[SA](https://github.com/gicLAB/SECDA/releases/download/v1.0/sa_uint8_v3.xpr.zip)).xpr project within the tool.
* Use the "Generate Bitstream" option to synthesize and export the FPGA mapping.


## PYNQ-Z1 TFLite Inference with Accelerators
The Tensorflow submodule linked within this repo contains the required source code for compiling TFLite-SECDA for the PYNQ-Z1.
We have adapted an example "label_image" provided by Tensorflow to "label_image_secda", which enables our accelerator pipeline for CONV2D layers.

To cross-compile "label_image_secda" binary for PYNQ-Z1, use the following commands:
```
git clone https://github.com/gicLAB/SECDA.git --recurse-submodules
cd tensorflow/
./make_label_image_secda.sh [vm|sa]
cp ./out/label_image_secda_[vm|sa] TO_PYNQ_DEVICE
```

**Usage** 
 ```
 sudo ./label_image_secda_[vm|sa] -m[path/to/tflite/model.tflite] -i[path/to/input/image.bmp] -l[path/to/labels.txt] -t [1|2] -n [number of runs] -o [profile] -x [store_profile] 
 ```
* "-i" path to input image (.bmp required) [path/to/input/image.bmp]
* "-l" path to class labels.txt [path/to/labels.txt]
* "-t" threads used [1|2]
* "-n" number of inference runs to perform [0-100]
* "-o" to profile inference times [0|1]
* "-x" to profile & save profiled information (saves to current directory) [0|1]
* Example: 
```
sudo ./label_image_secda_vm -mtmp/mobilenetv1.tflite -itmp/grace_hopper.bmp -ltmp/labels.txt -t 1 -n 1 -o 1 -x 0
```

**Requirements**
* To cross-compile make sure you have to have the relevant utilities and arm-linux-gnueabihf-gcc compiler
* Cross-compilation was tested/recommended for Ubuntu 18.04
* Get compilers for PYNQ-Z1 by installing the following:

```
sudo apt-get install libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev build-essential bison flex libssl-dev bc
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```
