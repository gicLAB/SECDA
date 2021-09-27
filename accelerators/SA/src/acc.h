#ifndef ACCNAME_H
#define ACCNAME_H

#include <systemc.h>
#ifndef __SYNTHESIS__
#define DWAIT(x) wait(x)
#else
#define DWAIT(x)
#endif

#define ACCNAME SA_UINT8_V2_0
#define ACC_DTYPE sc_uint
#define ACC_C_DTYPE unsigned int
#define STOPPER 4294967295

#define IN_BUF_LEN 4096
#define WE_BUF_LEN 8192
#define SUMS_BUF_LEN 1024

#define MAX 2147483647
#define MIN -2147483648
#define POS 1073741824
#define NEG -1073741823
#define DIVMAX 2147483648
#define MAX8 255

typedef struct _DATA{
  sc_uint<32> data;
  bool        tlast;

  inline friend ostream& operator << (ostream& os, const _DATA &v){
    cout << "data&colon; " << v.data << " tlast: " << v.tlast;
    return os;
  }
} DATA;

SC_MODULE(ACCNAME) {
	sc_uint<14> depth;
	sc_in<bool> clock;
	sc_in <bool>  reset;

	sc_fifo_in<DATA> din1;
	sc_fifo_in<DATA> din2;
	sc_fifo_in<DATA> din3;
	sc_fifo_in<DATA> din4;

	sc_fifo_out<DATA> dout1;
	sc_fifo_out<DATA> dout2;
	sc_fifo_out<DATA> dout3;
	sc_fifo_out<DATA> dout4;

	sc_signal<bool> read_inputs;
	sc_signal<bool> rtake;
	sc_signal<bool> ltake;
	sc_signal<int> llen;
	sc_signal<int> rlen;
    sc_signal<int> lhs_block_max;
    sc_signal<int> rhs_block_max;


#ifndef __SYNTHESIS__
	sc_signal<bool,SC_MANY_WRITERS> d_in1;
	sc_signal<bool,SC_MANY_WRITERS> schedule;
	sc_signal<bool,SC_MANY_WRITERS> out_check;
	sc_signal<bool,SC_MANY_WRITERS> gemm_unit_1_ready;
	sc_signal<bool,SC_MANY_WRITERS> write1;
	sc_signal<bool,SC_MANY_WRITERS> arrange1;
#else
	sc_signal<bool> d_in1;
    sc_signal<bool> schedule;
    sc_signal<bool> out_check;
	sc_signal<bool> gemm_unit_1_ready;
    sc_signal<bool> write1;
	sc_signal<bool> arrange1;
#endif

    sc_signal<int> gemm_unit_1_l_pointer;
    sc_signal<bool> gemm_unit_1_iwuse;

    sc_uint<32> g1 [256];
    sc_uint<8> r1 [256];

	//GEMM 1 Inputs
	sc_uint<32> lhsdata1a[IN_BUF_LEN];
	sc_uint<32> lhsdata2a[IN_BUF_LEN];
	sc_uint<32> lhsdata3a[IN_BUF_LEN];
	sc_uint<32> lhsdata4a[IN_BUF_LEN];

	//Global Weights
	sc_uint<32> rhsdata1[WE_BUF_LEN];
	sc_uint<32> rhsdata2[WE_BUF_LEN];
	sc_uint<32> rhsdata3[WE_BUF_LEN];
	sc_uint<32> rhsdata4[WE_BUF_LEN];

	//new sums bram
	sc_int<32> lhs_sum1[SUMS_BUF_LEN];
	sc_int<32> lhs_sum2[SUMS_BUF_LEN];
	sc_int<32> lhs_sum3[SUMS_BUF_LEN];
	sc_int<32> lhs_sum4[SUMS_BUF_LEN];

	sc_int<32> rhs_sum1[SUMS_BUF_LEN];
	sc_int<32> rhs_sum2[SUMS_BUF_LEN];
	sc_int<32> rhs_sum3[SUMS_BUF_LEN];
	sc_int<32> rhs_sum4[SUMS_BUF_LEN];

    sc_fifo<int> WRQ1;
    sc_fifo<int> WRQ2;
    sc_fifo<char> sIs1;
    sc_fifo<char> sIs2;
    sc_fifo<char> sIs3;
    sc_fifo<char> sIs4;
    sc_fifo<char> sIs5;
    sc_fifo<char> sIs6;
    sc_fifo<char> sIs7;
    sc_fifo<char> sIs8;
    sc_fifo<char> sIs9;
    sc_fifo<char> sIs10;
    sc_fifo<char> sIs11;
    sc_fifo<char> sIs12;
    sc_fifo<char> sIs13;
    sc_fifo<char> sIs14;
    sc_fifo<char> sIs15;
    sc_fifo<char> sIs16;

    sc_fifo<char> sWs1;
    sc_fifo<char> sWs2;
    sc_fifo<char> sWs3;
    sc_fifo<char> sWs4;
    sc_fifo<char> sWs5;
    sc_fifo<char> sWs6;
    sc_fifo<char> sWs7;
    sc_fifo<char> sWs8;
    sc_fifo<char> sWs9;
    sc_fifo<char> sWs10;
    sc_fifo<char> sWs11;
    sc_fifo<char> sWs12;
    sc_fifo<char> sWs13;
    sc_fifo<char> sWs14;
    sc_fifo<char> sWs15;
    sc_fifo<char> sWs16;

	sc_int<64> rf;
	int ra=0;
	sc_int<32> l_offset = 0;
	int mask = 0;
	int r = 0;
	int sm = 0;

    sc_signal<int>  w1S;
    sc_signal<int>  w2S;
    sc_signal<int>  w3S;
    sc_signal<int>  w4S;

#ifndef __SYNTHESIS__
    int weight_max_index=0;
    int input_max_index=0;

    int g1_macs=0;
    int g1_out_count=0;
#endif

	sc_out<int>  inS;
	sc_out<int>  read_cycle_count;
	sc_out<int> process_cycle_count;
	sc_out<int> gemm_1_idle;
	sc_out<int> gemm_1_write;
	sc_out<int> gemm_1;
	sc_out<int> wstall_1;
	sc_out<int> rmax;
	sc_out<int> lmax;
	sc_out<int>  outS;
	sc_out<int>  w1SS;
	sc_out<int>  w2SS;
	sc_out<int>  w3SS;
	sc_out<int>  w4SS;
	sc_out<int>  schS;
	sc_out<int>  p1S;

	void Input_Handler();

	void Output_Handler();

	void Worker1();

	void Data_In();

	void Tracker();

	void Scheduler();

	void Post1();

	void schedule_gemm_unit(int, int, int, int, int,int,int);

	int SHR(int,int);

	void Read_Cycle_Counter();

	void Process_Cycle_Counter();

	void Writer_Cycle_Counter();

	sc_uint<32> mul_u8(sc_uint<8>,sc_uint<8>);

	SC_HAS_PROCESS(ACCNAME);

  ACCNAME(sc_module_name name_) :sc_module(name_),WRQ1(512),sIs1(2048),sIs2(2048),sIs3(2048),sIs4(2048),sIs5(2048),sIs6(2048),sIs7(2048),sIs8(2048),
											    sIs9(2048),sIs10(2048),sIs11(2048),sIs12(2048),sIs13(2048),sIs14(2048),sIs15(2048),sIs16(2048),
											    WRQ2(512),sWs1(2048),sWs2(2048),sWs3(2048),sWs4(2048),sWs5(2048),sWs6(2048),sWs7(2048),sWs8(2048),
												sWs9(2048),sWs10(2048),sWs11(2048),sWs12(2048),sWs13(2048),sWs14(2048),sWs15(2048),sWs16(2048){

    SC_CTHREAD(Input_Handler,clock.pos());
    reset_signal_is(reset,true);

    SC_CTHREAD(Output_Handler,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Worker1,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Data_In,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Scheduler,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Post1,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Read_Cycle_Counter,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Process_Cycle_Counter,clock);
    reset_signal_is(reset,true);

    SC_CTHREAD(Writer_Cycle_Counter,clock);
    reset_signal_is(reset,true);



#pragma HLS RESOURCE variable=din1 core=AXI4Stream metadata="-bus_bundle S_AXIS_DATA1" port_map={{din1_0 TDATA} {din1_1 TLAST}}
#pragma HLS RESOURCE variable=din2 core=AXI4Stream metadata="-bus_bundle S_AXIS_DATA2" port_map={{din2_0 TDATA} {din2_1 TLAST}}
#pragma HLS RESOURCE variable=din3 core=AXI4Stream metadata="-bus_bundle S_AXIS_DATA3" port_map={{din3_0 TDATA} {din3_1 TLAST}}
#pragma HLS RESOURCE variable=din4 core=AXI4Stream metadata="-bus_bundle S_AXIS_DATA4" port_map={{din4_0 TDATA} {din4_1 TLAST}}
#pragma HLS RESOURCE variable=dout1 core=AXI4Stream metadata="-bus_bundle M_AXIS_DATA1" port_map={{dout1_0 TDATA} {dout1_1 TLAST}}
#pragma HLS RESOURCE variable=dout2 core=AXI4Stream metadata="-bus_bundle M_AXIS_DATA2" port_map={{dout2_0 TDATA} {dout2_1 TLAST}}
#pragma HLS RESOURCE variable=dout3 core=AXI4Stream metadata="-bus_bundle M_AXIS_DATA3" port_map={{dout3_0 TDATA} {dout3_1 TLAST}}
#pragma HLS RESOURCE variable=dout4 core=AXI4Stream metadata="-bus_bundle M_AXIS_DATA4" port_map={{dout4_0 TDATA} {dout4_1 TLAST}}

#pragma HLS array_partition variable=g1 complete dim=0
#pragma HLS RESET variable=reset

#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=inS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=rmax
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=lmax
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=outS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=schS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=inS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=p1S
  }
};
#endif /* ACCNAME_H */
