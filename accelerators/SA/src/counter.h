void ACCNAME::Read_Cycle_Counter(){
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=read_cycle_count
int cycles=0;
read_cycle_count.write(0);
  while(1){

	  while(read_inputs){
		  cycles++;
		  read_cycle_count.write(cycles);
          DWAIT();
	  }
	  cycles=0;
	  wait();
  }
}

void ACCNAME::Process_Cycle_Counter(){
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=process_cycle_count
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_1_idle

int cycles=0;
int idle1=0;

process_cycle_count.write(0);
gemm_1_idle.write(0);
wait();
  while(1){

	  while(out_check){
		  cycles++;
		  if(w1S.read()==10)idle1++;
		  process_cycle_count.write(cycles);
		  gemm_1_idle.write(idle1);
          DWAIT();
	  }
	  cycles=0;
	  idle1=0;
	  wait();
  }
}

void ACCNAME::Writer_Cycle_Counter(){
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_1_write
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_1
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=wstall_1
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=w1SS

int c_write1=0;
int gemm1=0;
int wstall1=0;
gemm_1_write.write(0);
gemm_1.write(0);
wstall_1.write(0);
wait();
  while(1){
	  while(out_check){
		  if(write1.read())c_write1++;
		  int w1=w1S.read();
		  w1SS.write(w1);
		  if(w1==3)gemm1++;
		  if(w1==9)wstall1++;
		  gemm_1_write.write(c_write1);
		  gemm_1.write(gemm1);
		  wstall_1.write(wstall1);
          DWAIT();
	  }
	  c_write1=0;
	  gemm1=0;
	  wstall1=0;
	  wait();
  }
}
