#include "dmadriver.h"

#include <iostream>
#include <fstream>
using namespace std;

void DMADriver::read_in(){
	for(int i=0;i<inl0;i++){
		din1.write({in0[i], 1});
		if(i<inl1)din2.write({in1[i], 1});
		if(i<inl2)din3.write({in2[i], 1});
		if(i<inl3)din4.write({in3[i], 1});
	}
}

void DMADriver::read_out(){
	int res[10];
	bool last = false;
	int i=0;
	do{
		DATA d = dout1.read();
		out0[i++] = d.data;
		last = d.tlast;
	} while (!last);

	last = false;
	i=0;
	do{
		DATA d = dout2.read();
		out1[i++] = d.data;
		last = d.tlast;
	} while (!last);

	last = false;
	i=0;
	do{
		DATA d = dout3.read();
		out2[i++] = d.data;
		last = d.tlast;
	} while (!last);


	last = false;
	i=0;
	do{
		DATA d = dout4.read();
		out3[i++] = d.data;
		last = d.tlast;
	} while (!last);
}

void DMADriver::Source(){
	sourceS.write(0);
	wait();
	while (true){


		read_in();
		sent=true;
		while(sent){
			sourceS.write(2);
			wait();
		}
		sourceS.write(3);
		wait();
		sc_pause();
		wait();
	}
}

void DMADriver::Sink(){
	wait();
	sinkS.write(0);
	while (true){
		while(!sent){
			sinkS.write(1);
			wait();
		}
		sinkS.write(2);
		read_out();

		sent=false;
		sinkS.write(3);
		loading=read_cycle_count.read();
		p_cycles=process_cycle_count.read();
		g1_idle=gemm_1_idle.read();
		g2_idle=gemm_2_idle.read();
		g3_idle=gemm_3_idle.read();
		g4_idle=gemm_4_idle.read();
		g1_write=gemm_1_write.read();
		g2_write=gemm_2_write.read();
		g3_write=gemm_3_write.read();
		g4_write=gemm_4_write.read();
		g1_gemm=gemm_1.read();
		g2_gemm=gemm_2.read();
		g3_gemm=gemm_3.read();
		g4_gemm=gemm_4.read();
		wstall1=wstall_1.read();
		wstall2=wstall_2.read();
		wstall3=wstall_3.read();
		wstall4=wstall_4.read();
		r_max=rmax.read();
		l_max=lmax.read();
		wait();
	}
}

void DMADriver::Status(){
	bool waited;
	wait();
	while(true){
		states[0] = inS.read();
		states[1] = outS.read();
		states[2] = w1S.read();
		states[3] = w2S.read();
		states[4] = w3S.read();
		states[5] = w4S.read();
		states[6] = sourceS.read();
		states[7] = sinkS.read();
		states[8] = schS.read();
		waited= false;
		if(waited)waiting++;
		wait();
	}
}
