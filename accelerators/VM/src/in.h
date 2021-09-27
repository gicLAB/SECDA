#define STOPPER 4294967295

void ACCNAME::Input_Handler(){
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=inS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=rmax
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=lmax
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=outS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=schS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=inS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=p1S

inS.write(0);
int r_max;
int l_max;
DATA last ={5000,1};
wait();
  while(1){

	inS.write(1);
	sc_uint<32> header_lite= din1.read().data;
	if(header_lite!=STOPPER){
		bool rhs_take=header_lite.range(0,0);
		bool lhs_take=header_lite.range(1,1);
		depth=header_lite.range(31,18);

		inS.write(2);
		sc_uint<32> hcounts=din1.read().data;
		int rhs_count = hcounts.range(15, 0);
		int lhs_count = hcounts.range(31, 16);

		inS.write(3);
		sc_uint<32> lengths=din1.read().data;
		int rhs_length = lengths.range(15, 0);
		int lhs_length = lengths.range(31, 16);

		if(rhs_take){
			rf=din1.read().data;
			ra=din1.read().data;
			int re=din1.read().data;
			if(re>0){
				left_s=re;
				right_s=0;
			}else{
				left_s=0;
				right_s=-re;
			}
		}

		d_in1.write(1);
		read_inputs.write(1);
		rtake.write(rhs_take);
		ltake.write(lhs_take);
		rlen.write(rhs_length);
		llen.write(lhs_length);
		DWAIT();

		inS.write(4);
		while(d_in1.read())wait();
		read_inputs.write(0);
		inS.write(5);
		wait();

		inS.write(6);
		if(lhs_take){
			r_max=rhs_count;
			l_max=lhs_count;
		}
#ifndef __SYNTHESIS__
		if(rhs_take){
			out_check.write(1);
		}
#endif
		DWAIT(3);

	}else{
		while(schedule.read())wait();
		rhs_block_max.write(r_max);
		lhs_block_max.write(l_max);
		rmax.write(r_max);
		lmax.write(l_max);
		schedule.write(1);
		out_check.write(1);
		inS.write(7);
		wait();
		while(schedule.read())wait();

	}


  }
}
