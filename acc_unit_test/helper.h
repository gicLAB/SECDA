#ifndef ACCEL
#define ACCEL

#include <chrono>
#include <typeinfo>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>


using namespace std;
using namespace std::chrono;

#define MM2S_CONTROL_REGISTER 0x00
#define MM2S_STATUS_REGISTER 0x04
#define MM2S_START_ADDRESS 0x18
#define MM2S_LENGTH 0x28

#define S2MM_CONTROL_REGISTER 0x30
#define S2MM_STATUS_REGISTER 0x34
#define S2MM_DESTINATION_ADDRESS 0x48
#define S2MM_LENGTH 0x58

#define acc_address 0x43C00000
#define page_size 65536

#define dma_buffer_len 4194304
#define dma_addr0 0x40400000
#define dma_addr1 0x40410000
#define dma_addr2 0x40420000
#define dma_addr3 0x40430000

#define dma_addr_in0 0x16000000
#define dma_addr_in1 0x18000000
#define dma_addr_in2 0x1a000000
#define dma_addr_in3 0x1c000000
#define dma_addr_out0 0x16400000
#define dma_addr_out1 0x18400000
#define dma_addr_out2 0x1a400000
#define dma_addr_out3 0x1c400000

template <typename Integer>
void dma_set(unsigned int* dma_virtual_address, int offset, unsigned int value) {
    dma_virtual_address[offset>>2] = value;
}

template <typename Integer>
unsigned int dma_get(unsigned int* dma_virtual_address, int offset) {
    return dma_virtual_address[offset>>2];
}


template <typename Integer>
void dma_s2mm_status(unsigned int* dma_virtual_address) {
    msync(dma_virtual_address,88,MS_SYNC);
    unsigned int status = dma_get<int>(dma_virtual_address, S2MM_STATUS_REGISTER);
}

template <typename Integer>
void dma_mm2s_status(unsigned int* dma_virtual_address) {
    msync(dma_virtual_address,88,MS_SYNC);
    unsigned int status = dma_get<int>(dma_virtual_address, MM2S_STATUS_REGISTER);
}

template <typename Integer>
void dma_mm2s_sync(unsigned int* dma_virtual_address) {
    msync(dma_virtual_address,88,MS_SYNC);
    unsigned int mm2s_status =  dma_get<int>(dma_virtual_address, MM2S_STATUS_REGISTER);
    while(!(mm2s_status & 1<<12) || !(mm2s_status & 1<<1) ){
        dma_s2mm_status<int>(dma_virtual_address);
        dma_mm2s_status<int>(dma_virtual_address);
        mm2s_status =  dma_get<int>(dma_virtual_address, MM2S_STATUS_REGISTER);

    }
}

template <typename Integer>
void dma_s2mm_sync(unsigned int* dma_virtual_address) {
    msync(dma_virtual_address,88,MS_SYNC);
    unsigned int s2mm_status = dma_get<int>(dma_virtual_address, S2MM_STATUS_REGISTER);
    while(!(s2mm_status & 1<<12) || !(s2mm_status & 1<<1)){
        dma_s2mm_status<int>(dma_virtual_address);
        dma_mm2s_status<int>(dma_virtual_address);      
        s2mm_status = dma_get<int>(dma_virtual_address, S2MM_STATUS_REGISTER);

    }
}

struct accelerator { 
    //Accelerator MM pointer
    int* gemm;

    //DMA MM Pointers
    unsigned int* dma0;
    unsigned int* dma1;
    unsigned int* dma2;
    unsigned int* dma3;

    //DMA Input MMapped Buffers
    unsigned int* in0;
    unsigned int* in1;
    unsigned int* in2;
    unsigned int* in3;

    //DMA Output MMapped Buffers
    int* out0;
    int* out1;
    int* out2;
    int* out3;
};

void read_in(string name, accelerator &acc){
	string input;
	ifstream wrin0;
	ifstream wrin1;
	ifstream wrin2;
	ifstream wrin3;

	wrin0.open (name+"_0.txt");
	wrin1.open (name+"_1.txt");
	wrin2.open (name+"_2.txt");
	wrin3.open (name+"_3.txt");

    int il0=0;
    int il1=0;
    int il2=0;
    int il3=0;
	while (getline (wrin0, input)) {
    unsigned int d = stoul(input.c_str());
		acc.in0[il0++]=d;
	}

	while (getline (wrin1, input)) {
    unsigned int d = stoul(input.c_str());
		acc.in1[il1++]=d;
	}
	while (getline (wrin2, input)) {
    unsigned int d = stoul(input.c_str());
		acc.in2[il2++]=d;
	}
	while (getline (wrin3, input)) {
    unsigned int d = stoul(input.c_str());
		acc.in3[il3++]=d;
	}
	wrin0.close();
	wrin1.close();
	wrin2.close();
	wrin3.close();

  dma_set<int>(acc.dma0, MM2S_LENGTH, (il0)*4);
  dma_set<int>(acc.dma1, MM2S_LENGTH, (il1)*4);
  dma_set<int>(acc.dma2, MM2S_LENGTH, (il2)*4);
  dma_set<int>(acc.dma3, MM2S_LENGTH, (il3)*4);
  dma_mm2s_sync<int>(acc.dma0);
  dma_mm2s_sync<int>(acc.dma1);
  dma_mm2s_sync<int>(acc.dma2);
  dma_mm2s_sync<int>(acc.dma3);
}

void rec_out(accelerator &acc) {
    dma_set<int>(acc.dma0, S2MM_LENGTH,dma_buffer_len);
    dma_set<int>(acc.dma1, S2MM_LENGTH,dma_buffer_len);
    dma_set<int>(acc.dma2, S2MM_LENGTH,dma_buffer_len);
    dma_set<int>(acc.dma3, S2MM_LENGTH,dma_buffer_len);
}


void initDMA(unsigned int* dma, int src, int dst){
    dma_set<int>(dma, S2MM_CONTROL_REGISTER, 4);
    dma_set<int>(dma, MM2S_CONTROL_REGISTER, 4);
    dma_set<int>(dma, S2MM_CONTROL_REGISTER, 0);
    dma_set<int>(dma, MM2S_CONTROL_REGISTER, 0);
    dma_set<int>(dma, S2MM_DESTINATION_ADDRESS, dst); // Write destination address
    dma_set<int>(dma, MM2S_START_ADDRESS, src); // Write source address
    dma_set<int>(dma, S2MM_CONTROL_REGISTER, 0xf001);
    dma_set<int>(dma, MM2S_CONTROL_REGISTER, 0xf001);
}


int* getArray(size_t base_addr,size_t length ){
    fstream myfile;
    size_t virt_base = base_addr & ~(getpagesize()- 1);
    size_t virt_offset = base_addr - virt_base;
    int fd = open ("/dev/mem", O_RDWR | O_SYNC);
    void *addr =mmap(NULL,length+virt_offset,PROT_READ | PROT_WRITE,MAP_SHARED,fd,virt_base);
    close(fd);
    if (addr == (void*) -1 ) exit (EXIT_FAILURE);
    int* array =reinterpret_cast<int*> (addr);
    return array;
}
#endif
