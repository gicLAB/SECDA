//#ifdef __RTL_SIMULATION__
//#include "ACCNAME_rtl_wrapper.h"
//#define ACCNAME ACCNAME_rtl_wrapper
//#else
//#include "../acc.h"
//#endif

#include "dmadriver.h"

#include <iostream>
#include <fstream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

void load_start(DMADriver *dmad, string name){
	string input;
	ifstream wrin0;
	ifstream wrin1;
	ifstream wrin2;
	ifstream wrin3;
	wrin0.open (name+"_0.txt");
	wrin1.open (name+"_1.txt");
	wrin2.open (name+"_2.txt");
	wrin3.open (name+"_3.txt");


	dmad->inl0=0;
	dmad->inl1=0;
	dmad->inl2=0;
	dmad->inl3=0;
	while (getline (wrin0, input)) {
		int d = atoi(input.c_str());
		dmad->in0[dmad->inl0++] = d;
	}
	while (getline (wrin1, input)) {
		int d = atoi(input.c_str());
		dmad->in1[dmad->inl1++] = d;
	}
	while (getline (wrin2, input)) {
		int d = atoi(input.c_str());
		dmad->in2[dmad->inl2++] = d;
	}
	while (getline (wrin3, input)) {
		int d = atoi(input.c_str());
		dmad->in3[dmad->inl3++] = d;
	}
	wrin0.close();
	wrin1.close();
	wrin2.close();
	wrin3.close();
	sc_start();

}

void read_out(DMADriver *dmad, string name){
  int expected_out = (dmad->r_max/4) * (dmad->l_max/4);

  ofstream wrin0;
  ofstream wrin1;
  ofstream wrin2;
  ofstream wrin3;
  wrin0.open (name+"_outputs_0.txt");
  wrin1.open (name+"_outputs_1.txt");
  wrin2.open (name+"_outputs_2.txt");
  wrin3.open (name+"_outputs_3.txt");

  uint8_t* bo0= reinterpret_cast<uint8_t*> (dmad->out0);
  uint8_t* bo1= reinterpret_cast<uint8_t*> (dmad->out1);
  uint8_t* bo2= reinterpret_cast<uint8_t*> (dmad->out2);
  uint8_t* bo3= reinterpret_cast<uint8_t*> (dmad->out3);

  int bc0 =0;
  int bc1 =0;
  int bc2 =0;
  int bc3 =0;

  for(int i=0;i<expected_out+1;i++){
    wrin0 << (int) bo0[bc0++] << "\n";
    wrin0 << (int) bo0[bc0++] << "\n";
    wrin0 << (int) bo0[bc0++] << "\n";
    wrin0 << (int) bo0[bc0++] << "\n";

    wrin1 << (int) bo1[bc1++] << "\n";
    wrin1 << (int) bo1[bc1++] << "\n";
    wrin1 << (int) bo1[bc1++] << "\n";
    wrin1 << (int) bo1[bc1++] << "\n";

    wrin2 << (int) bo2[bc2++] << "\n";
    wrin2 << (int) bo2[bc2++] << "\n";
    wrin2 << (int) bo2[bc2++] << "\n";
    wrin2 << (int) bo2[bc2++] << "\n";

    wrin3 << (int) bo3[bc3++] << "\n";
    wrin3 << (int) bo3[bc3++] << "\n";
    wrin3 << (int) bo3[bc3++] << "\n";
    wrin3 << (int) bo3[bc3++] << "\n";
  }
  wrin0.close();
  wrin1.close();
  wrin2.close();
  wrin3.close();
}


int sc_main(int argc, char* argv[]) {

  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
  sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);
  sc_report_handler::set_actions( SC_ID_OBJECT_EXISTS_, SC_LOG);

  //Signals
  sc_clock clk_fast("ClkFast", 1, SC_NS);
  sc_signal<bool>    sig_reset;

  sc_fifo <DATA>	din1("din1_fifo",163840);
  sc_fifo <DATA>	din2("din2_fifo",163840);
  sc_fifo <DATA>	din3("din3_fifo",163840);
  sc_fifo <DATA>	din4("din4_fifo",163840);

  sc_fifo <DATA>	dout1("dout1_fifo",100000);
  sc_fifo <DATA>	dout2("dout2_fifo",100000);
  sc_fifo <DATA>	dout3("dout3_fifo",100000);
  sc_fifo <DATA>	dout4("dout4_fifo",100000);

  sc_signal<int>    sig_inS;
  sc_signal<int>    sig_outS;
  sc_signal<int>    sig_w1S;
  sc_signal<int>    sig_w2S;
  sc_signal<int>    sig_w3S;
  sc_signal<int>    sig_w4S;
  sc_signal<int>    sig_schS;
  sc_signal<int>    sig_p1S;
  sc_signal<int>    sig_read_cycle_count;
  sc_signal<int>    sig_process_cycle_count;

  sc_signal<int> sig_gemm_1_idle;
  sc_signal<int> sig_gemm_2_idle;
  sc_signal<int> sig_gemm_3_idle;
  sc_signal<int> sig_gemm_4_idle;
  sc_signal<int> sig_gemm_1_write;
  sc_signal<int> sig_gemm_2_write;
  sc_signal<int> sig_gemm_3_write;
  sc_signal<int> sig_gemm_4_write;
  sc_signal<int> sig_gemm_1;
  sc_signal<int> sig_gemm_2;
  sc_signal<int> sig_gemm_3;
  sc_signal<int> sig_gemm_4;
  sc_signal<int> sig_wstall_1;
  sc_signal<int> sig_wstall_2;
  sc_signal<int> sig_wstall_3;
  sc_signal<int> sig_wstall_4;

  sc_signal<int> sig_rmax;
  sc_signal<int> sig_lmax;


  //DUT
  ACCNAME mg("DUT");
  mg.clock(clk_fast);
  mg.reset(sig_reset);


  mg.dout1(dout1);
  mg.dout2(dout2);
  mg.dout3(dout3);
  mg.dout4(dout4);

  mg.din1(din1);
  mg.din2(din2);
  mg.din3(din3);
  mg.din4(din4);

  //States
  mg.inS(sig_inS);
  mg.outS(sig_outS);
  mg.w1SS(sig_w1S);
  mg.w2SS(sig_w2S);
  mg.w3SS(sig_w3S);
  mg.w4SS(sig_w4S);
  mg.schS(sig_schS);
  mg.p1S(sig_p1S);
  mg.read_cycle_count(sig_read_cycle_count);
  mg.process_cycle_count(sig_process_cycle_count);
  mg.gemm_1_idle(sig_gemm_1_idle);
  mg.gemm_2_idle(sig_gemm_2_idle);
  mg.gemm_3_idle(sig_gemm_3_idle);
  mg.gemm_4_idle(sig_gemm_4_idle);
  mg.gemm_1_write(sig_gemm_1_write);
  mg.gemm_2_write(sig_gemm_2_write);
  mg.gemm_3_write(sig_gemm_3_write);
  mg.gemm_4_write(sig_gemm_4_write);
  mg.gemm_1(sig_gemm_1);
  mg.gemm_2(sig_gemm_2);
  mg.gemm_3(sig_gemm_3);
  mg.gemm_4(sig_gemm_4);
  mg.wstall_1(sig_wstall_1);
  mg.wstall_2(sig_wstall_2);
  mg.wstall_3(sig_wstall_3);
  mg.wstall_4(sig_wstall_4);
  mg.rmax(sig_rmax);
  mg.lmax(sig_lmax);

  //Driver
  DMADriver dmad("Driver");
  dmad.clock(clk_fast);
  dmad.reset(sig_reset);

  dmad.dout1(dout1);
  dmad.dout2(dout2);
  dmad.dout3(dout3);
  dmad.dout4(dout4);

  dmad.din1(din1);
  dmad.din2(din2);
  dmad.din3(din3);
  dmad.din4(din4);


  //States
  dmad.inS(sig_inS);
  dmad.outS(sig_outS);
  dmad.w1S(sig_w1S);
  dmad.w2S(sig_w2S);
  dmad.w3S(sig_w3S);
  dmad.w4S(sig_w4S);
  dmad.schS(sig_schS);
  dmad.p1S(sig_p1S);
  dmad.read_cycle_count(sig_read_cycle_count);
  dmad.process_cycle_count(sig_process_cycle_count);
  dmad.gemm_1_idle(sig_gemm_1_idle);
  dmad.gemm_2_idle(sig_gemm_2_idle);
  dmad.gemm_3_idle(sig_gemm_3_idle);
  dmad.gemm_4_idle(sig_gemm_4_idle);
  dmad.gemm_1_write(sig_gemm_1_write);
  dmad.gemm_2_write(sig_gemm_2_write);
  dmad.gemm_3_write(sig_gemm_3_write);
  dmad.gemm_4_write(sig_gemm_4_write);
  dmad.gemm_1(sig_gemm_1);
  dmad.gemm_2(sig_gemm_2);
  dmad.gemm_3(sig_gemm_3);
  dmad.gemm_4(sig_gemm_4);
  dmad.wstall_1(sig_wstall_1);
  dmad.wstall_2(sig_wstall_2);
  dmad.wstall_3(sig_wstall_3);
  dmad.wstall_4(sig_wstall_4);
  dmad.rmax(sig_rmax);
  dmad.lmax(sig_lmax);




  cout << "INFO: Simulating " << endl;
  load_start(&dmad,"input_data/model0_w0");
  load_start(&dmad,"input_data/model0_w0_i0");
  read_out(&dmad,"input_data/");
  cout << "Sim: Done " << endl;

  sc_start();


  int additional_cycles = dmad.r_max*dmad.l_max*2/4;

  float weight_global_usage = (float) mg.weight_max_index/ (float)GWE_BUF_LEN *100;
  float input_global_usage = (float) mg.input_max_index/ (float)IN_BUF_LEN *100;
  float weight_local_usage = (float) (mg.local_weight_max_index+1)/ (float)WE_BUF_LEN *100;


  int total_batch_macs = mg.g_macs[0]+mg.g_macs[1]+mg.g_macs[2]+mg.g_macs[3];
  int total_out_count = mg.g_out_count[0]+mg.g_out_count[1]+mg.g_out_count[2]+mg.g_out_count[3];

  float macs_to_out_ratio = (float) total_batch_macs/ (float) total_out_count;

  float gemm_cycle_percentage = (float) dmad.g1_gemm/ (float) dmad.p_cycles  *100;
  float write_cycle_percentage = (float) dmad.g1_write/ (float) dmad.p_cycles  *100;
  float wstall_cycle_percentage = (float) dmad.wstall1/ (float) dmad.p_cycles  *100;
  float idle_cycle_percentage = (float) dmad.g1_idle/ (float) dmad.p_cycles  *100;

  cout << "RMAX : "<< dmad.r_max << endl;
  cout << "LMAX : "<< dmad.l_max << endl;

  cout << "Loading Cycles: "<< dmad.loading << endl;
  cout << "Processing Cycles: "<< dmad.p_cycles << endl;

  cout << "==================================================" <<  endl;

  cout << "G1 Idle Cycles: "<< dmad.g1_idle << endl;
  cout << "G2 Idle Cycles: "<< dmad.g2_idle << endl;
  cout << "G3 Idle Cycles: "<< dmad.g3_idle << endl;
  cout << "G4 Idle Cycles: "<< dmad.g4_idle << endl;
  cout << "Idle Cycle: "<< idle_cycle_percentage << "%" <<  endl;

  cout << "==================================================" <<  endl;

  cout << "G1 Write Cycles: "<< dmad.g1_write << endl;
  cout << "G2 Write Cycles: "<< dmad.g2_write << endl;
  cout << "G3 Write Cycles: "<< dmad.g3_write << endl;
  cout << "G4 Write Cycles: "<< dmad.g4_write << endl;
  cout << "Write Cycle: "<< write_cycle_percentage << "%" <<  endl;


  cout << "==================================================" <<  endl;

  cout << "G1 WStall Cycles: "<< dmad.wstall1 << endl;
  cout << "G2 WStall Cycles: "<< dmad.wstall2 << endl;
  cout << "G3 WStall Cycles: "<< dmad.wstall3 << endl;
  cout << "G4 WStall Cycles: "<< dmad.wstall4 << endl;
  cout << "WStall Cycle: "<< wstall_cycle_percentage << "%" <<  endl;


  cout << "==================================================" <<  endl;

  cout << "G1 GEMM Cycles: "<< dmad.g1_gemm << endl;
  cout << "G2 GEMM Cycles: "<< dmad.g2_gemm << endl;
  cout << "G3 GEMM Cycles: "<< dmad.g3_gemm << endl;
  cout << "G4 GEMM Cycles: "<< dmad.g4_gemm << endl;
  cout << "GEMM Cycle: "<< gemm_cycle_percentage << "%" <<  endl;

  cout << "==================================================" <<  endl;

  cout << "Global Weight Buffer Usage: "<< weight_global_usage << "%" <<  endl;
  cout << "Global Input Buffer Usage: "<< input_global_usage << "%" <<  endl;
  cout << "Local Weight Buffer Usage: "<< weight_local_usage << "%" <<  endl;

  cout << "==================================================" <<  endl;

  cout << "Total MAC count: "<< total_batch_macs << endl;
  cout << "G1 MAC count: "<< mg.g_macs[0] << endl;
  cout << "G2 MAC count: "<< mg.g_macs[1] << endl;
  cout << "G3 MAC count: "<< mg.g_macs[2] << endl;
  cout << "G4 MAC count: "<< mg.g_macs[3] << endl;

  cout << "==================================================" <<  endl;

  cout << "Total Output count: "<< total_out_count << endl;
  cout << "G1 Output count: "<< mg.g_out_count[0] << endl;
  cout << "G2 Output count: "<< mg.g_out_count[1] << endl;
  cout << "G3 Output count: "<< mg.g_out_count[2] << endl;
  cout << "G4 Output count: "<< mg.g_out_count[3] << endl;
  cout << "MACs per Output: "<< macs_to_out_ratio << endl;

  cout << "==================================================" <<  endl;
  cout << "Sim: Done " << endl;


  return 0;
}
