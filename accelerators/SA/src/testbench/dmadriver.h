#ifndef GEMMDRIVER_H
#define GEMMDRIVER_H

#include <sys/time.h> 
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

#include <systemc.h>
#include "../acc.h"

SC_MODULE(DMADriver) {
  sc_in<bool> clock;
  sc_in <bool>  reset;
  sc_fifo_in<DATA> dout1;
  sc_fifo_in<DATA> dout2;
  sc_fifo_in<DATA> dout3;
  sc_fifo_in<DATA> dout4;

  sc_fifo_out<DATA> din1;
  sc_fifo_out<DATA> din2;
  sc_fifo_out<DATA> din3;
  sc_fifo_out<DATA> din4;

  bool sent;
  bool retval;

  //States
  int states[9];
  sc_signal<int> sourceS;
  sc_signal<int> sinkS;
  sc_in<int> inS;
  sc_in<int> outS;
  sc_in<int> w1S;
  sc_in<int> w2S;
  sc_in<int> w3S;
  sc_in<int> w4S;
  sc_in<int> schS;
  sc_in<int> p1S;
  sc_in<int>  read_cycle_count;
  sc_in<int>  process_cycle_count;
  sc_in<int> gemm_1_idle;
  sc_in<int> gemm_1_write;
  sc_in<int> gemm_1;
  sc_in<int> wstall_1;
  sc_in<int> rmax;
  sc_in<int> lmax;

  int p_cycles= 0;
  int waiting= 0;
  int loading= 0;
  int g1_idle= 0;
  int g1_write= 0;
  int g1_gemm= 0;
  int wstall1=0;
  int r_max=0;
  int l_max=0;

  void Source();

  void Sink();

  void Status();

  void read_in();

  void read_out();

  SC_HAS_PROCESS(DMADriver);

  // Parameters for the DUT
  DMADriver(sc_module_name name_) :sc_module(name_) { // @suppress("Class members should be properly initialized")
    SC_CTHREAD(Source, clock.pos());
    reset_signal_is(reset,true);

    SC_CTHREAD(Sink, clock.pos());
    reset_signal_is(reset,true);

    SC_CTHREAD(Status, clock.neg());
    reset_signal_is(reset,true);
  }

  int inl0;
  int inl1;
  int inl2;
  int inl3;

  int in0[563840];
  int in1[563840];
  int in2[563840];
  int in3[563840];

  int out0[554800];
  int out1[554800];
  int out2[554800];
  int out3[554800];
};
#endif /* GEMMDRIVER_H */
