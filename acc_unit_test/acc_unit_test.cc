
#include <fcntl.h> 
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <sys/mman.h>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>

#include "helper.h"


bool file_check(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool check_data(const std::string& name) {
  bool i0 = file_check(name+"/input_0.txt");
  bool i1 = file_check(name+"/input_1.txt");
  bool i2 = file_check(name+"/input_2.txt");
  bool i3 = file_check(name+"/input_3.txt");
  bool w0 = file_check(name+"/weight_0.txt");
  bool w1 = file_check(name+"/weight_1.txt");
  bool w2 = file_check(name+"/weight_2.txt");
  bool w3 = file_check(name+"/weight_3.txt");
  return (i0 && i1 && i2 && i3 && w0 && w1 && w2 && w3);
}

struct Settings {
  string input_base = "aData/model_l0_w0_i0";
  string weight_base = "aData/model0_w0_i0";
  string output_base = "aData/out";
  bool vm = false;
};

void display_usage() {
  cout<< "Nothing to see here\n"<< "\n";
}


int main(int argc, char *argv[]){
  Settings s;
  int c;
    while (1) {
      static struct option long_options[] = {
          {"input", required_argument, nullptr, 'i'},
          {"systolic", required_argument, nullptr, 's'},
          {nullptr, 0, nullptr, 0}};

      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long(argc, argv,
                      "i:s:", long_options,
                      &option_index);

      /* Detect the end of the options. */
      if (c == -1) break;

      switch (c) {
        case 'i':
          s.input_base = optarg;
          break;
        case 's':
          s.vm = strtol(optarg, nullptr, 10);
          break;
        case 'h':
        case '?':
          /* getopt_long already printed an error message. */
          display_usage();
          exit(-1);
        default:
          exit(-1);
      }
    }

    int* gemm = getArray(acc_address,page_size);
    int dh = open("/dev/mem", O_RDWR | O_SYNC); // Open /dev/mem which represents the whole physical memory
    gemm[0]=1;
    void *dma_mm0 = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr0); // Memory map AXI Lite register block
    void *dma_mm1 = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr1); // Memory map AXI Lite register block
    void *dma_mm2 = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr2); // Memory map AXI Lite register block
    void *dma_mm3 = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr3); // Memory map AXI Lite register block
    void *dma_in_mm0  = mmap(NULL, dma_buffer_len, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr_in0); // Memory map source address
    void *dma_in_mm1  = mmap(NULL, dma_buffer_len, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr_in1); // Memory map source address
    void *dma_in_mm2  = mmap(NULL, dma_buffer_len, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr_in2); // Memory map source address
    void *dma_in_mm3  = mmap(NULL, dma_buffer_len, PROT_READ | PROT_WRITE, MAP_SHARED, dh, dma_addr_in3); // Memory map source address  
    void *dma_out_mm0 = mmap(NULL, dma_buffer_len, PROT_READ, MAP_SHARED, dh, dma_addr_out0); // Memory map destination address
    void *dma_out_mm1 = mmap(NULL, dma_buffer_len, PROT_READ, MAP_SHARED, dh, dma_addr_out1); // Memory map destination address
    void *dma_out_mm2 = mmap(NULL, dma_buffer_len, PROT_READ, MAP_SHARED, dh, dma_addr_out2); // Memory map destination address
    void *dma_out_mm3 = mmap(NULL, dma_buffer_len, PROT_READ, MAP_SHARED, dh, dma_addr_out3); // Memory map destination address
    close(dh);

    unsigned int* dma0 =reinterpret_cast<unsigned int*> (dma_mm0);
    unsigned int* dma1 =reinterpret_cast<unsigned int*> (dma_mm1);
    unsigned int* dma2 =reinterpret_cast<unsigned int*> (dma_mm2);
    unsigned int* dma3 =reinterpret_cast<unsigned int*> (dma_mm3);
    unsigned int* dma_in0 =reinterpret_cast<unsigned int*> (dma_in_mm0);
    unsigned int* dma_in1 =reinterpret_cast<unsigned int*> (dma_in_mm1);
    unsigned int* dma_in2 =reinterpret_cast<unsigned int*> (dma_in_mm2);
    unsigned int* dma_in3 =reinterpret_cast<unsigned int*> (dma_in_mm3);
    int* dma_out0 =reinterpret_cast<int*> (dma_out_mm0);
    int* dma_out1 =reinterpret_cast<int*> (dma_out_mm1);
    int* dma_out2 =reinterpret_cast<int*> (dma_out_mm2);
    int* dma_out3 =reinterpret_cast<int*> (dma_out_mm3);
    initDMA(dma0,dma_addr_in0,dma_addr_out0);
    initDMA(dma1,dma_addr_in1,dma_addr_out1);
    initDMA(dma2,dma_addr_in2,dma_addr_out2);
    initDMA(dma3,dma_addr_in3,dma_addr_out3);

    struct accelerator acc = {
      gemm,
      dma0,dma1,dma2,dma3,
      dma_in0,dma_in1,dma_in2,dma_in3,
      dma_out0,dma_out1,dma_out2,dma_out3,
    };

    if(!check_data(s.input_base)){
      cout << "Data files missing or not specified" << endl;
      return 0;
    }

    cout  << s.input_base+"/weight" << endl;
    cout << "Running" << endl;
    rec_out(acc);
    read_in(s.input_base+"/weight",acc);
    read_in(s.input_base+"/input",acc);
    dma_s2mm_sync<int>(acc.dma0);
    dma_s2mm_sync<int>(acc.dma1);
    dma_s2mm_sync<int>(acc.dma2);
    dma_s2mm_sync<int>(acc.dma3);
    cout << "Done" << endl;

    if (s.vm){
      cout << "====================VECTOR_MAC========================" <<  endl;
      cout << "RMax: " << gemm[43] << endl;
      cout << "LMax: " << gemm[45] << endl;
      cout << "Loading Cycles: " << gemm[7] << endl;
      cout << "Processing Cycles: " << gemm[9] << endl;
      cout << "==================================================" <<  endl;
      cout << "G1 Idle Cycles: " << gemm[11] << endl;
      cout << "G2 Idle Cycles: " << gemm[13] << endl;
      cout << "G3 Idle Cycles: " << gemm[15] << endl;
      cout << "G4 Idle Cycles: " << gemm[17] << endl;
      cout << "==================================================" <<  endl;
      cout << "G1 Write Cycles: " << gemm[19] << endl;
      cout << "G2 Write Cycles: " << gemm[21] << endl;
      cout << "G3 Write Cycles: " << gemm[23] << endl;
      cout << "G4 Write Cycles: " << gemm[25] << endl;
      cout << "==================================================" <<  endl;
      cout << "G1 WStall Cycles: " << gemm[35] << endl;
      cout << "G2 WStall Cycles: " << gemm[37] << endl;
      cout << "G3 WStall Cycles: " << gemm[39] << endl;
      cout << "G4 WStall Cycles: " << gemm[41] << endl;
      cout << "==================================================" <<  endl;
      cout << "G1 GEMM Cycles: " << gemm[27] << endl;
      cout << "G2 GEMM Cycles: " << gemm[29] << endl;
      cout << "G3 GEMM Cycles: " << gemm[31] << endl;
      cout << "G4 GEMM Cycles: " << gemm[33] << endl;
      cout << "==================================================" <<  endl;
    }else{
      cout << "====================SYSTOLIC_ARRAY========================" <<  endl;
      cout << "RMax: " << gemm[19] << endl;
      cout << "LMax: " << gemm[21] << endl;
      cout << "Loading Cycles: " << gemm[7] << endl;
      cout << "Processing Cycles: " << gemm[9] << endl;
      cout << "==================================================" <<  endl;
      cout << "G1 Idle Cycles: " << gemm[11] << endl;
      cout << "G1 Write Cycles: " << gemm[13] << endl;
      cout << "G1 WStall Cycles: " << gemm[17] << endl;
      cout << "G1 GEMM Cycles: " << gemm[15] << endl;
      cout << "==================================================" <<  endl;
    }
    munmap(dma_in_mm0,dma_buffer_len);
    munmap(dma_in_mm1,dma_buffer_len);
    munmap(dma_in_mm2,dma_buffer_len);
    munmap(dma_in_mm3,dma_buffer_len);
    
    munmap(dma_out_mm0,dma_buffer_len);
    munmap(dma_out_mm1,dma_buffer_len);
    munmap(dma_out_mm2,dma_buffer_len);
    munmap(dma_out_mm3,dma_buffer_len);
    return 0;
}