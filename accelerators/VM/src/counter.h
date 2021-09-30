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
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_2_idle
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_3_idle
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_4_idle

int cycles=0;
int idle1=0;
int idle2=0;
int idle3=0;
int idle4=0;
process_cycle_count.write(0);
gemm_1_idle.write(0);
gemm_2_idle.write(0);
gemm_3_idle.write(0);
gemm_4_idle.write(0);
wait();
  while(1){
	  while(out_check){
		  cycles++;
		  if(w1S.read()==10)idle1++;
		  if(w2S.read()==10)idle2++;
		  if(w3S.read()==10)idle3++;
		  if(w4S.read()==10)idle4++;
		  process_cycle_count.write(cycles);
		  gemm_1_idle.write(idle1);
		  gemm_2_idle.write(idle2);
		  gemm_3_idle.write(idle3);
		  gemm_4_idle.write(idle4);
		  DWAIT();
	  }
	  cycles=0;
	  idle1=0;
	  idle2=0;
	  idle3=0;
	  idle4=0;
	  wait();
  }
}

void ACCNAME::Writer_Cycle_Counter(){
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_1_write
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_2_write
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_3_write
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_4_write
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_1
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_2
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_3
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=gemm_4
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=wstall_1
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=wstall_2
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=wstall_3
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=wstall_4
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=w1SS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=w2SS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=w3SS
#pragma HLS resource core=AXI4LiteS metadata="-bus_bundle slv0" variable=w4SS

int c_write1=0;
int c_write2=0;
int c_write3=0;
int c_write4=0;
int gemm1=0;
int gemm2=0;
int gemm3=0;
int gemm4=0;
int wstall1=0;
int wstall2=0;
int wstall3=0;
int wstall4=0;
gemm_1_write.write(0);
gemm_2_write.write(0);
gemm_3_write.write(0);
gemm_4_write.write(0);
gemm_1.write(0);
gemm_2.write(0);
gemm_3.write(0);
gemm_4.write(0);
wstall_1.write(0);
wstall_2.write(0);
wstall_3.write(0);
wstall_4.write(0);
wait();
  while(1){
	  while(out_check){
		  if(write1.read())c_write1++;
		  if(write2.read())c_write2++;
		  if(write3.read())c_write3++;
		  if(write4.read())c_write4++;
		  int w1=w1S.read();
		  int w2=w2S.read();
		  int w3=w3S.read();
		  int w4=w4S.read();
		  w1SS.write(w1);
		  w2SS.write(w2);
		  w3SS.write(w3);
		  w4SS.write(w4);
		  if(w1==3)gemm1++;
		  if(w2==3)gemm2++;
		  if(w3==3)gemm3++;
		  if(w4==3)gemm4++;
		  if(w1==9)wstall1++;
		  if(w2==9)wstall2++;
		  if(w3==9)wstall3++;
		  if(w4==9)wstall4++;

		  gemm_1_write.write(c_write1);
		  gemm_2_write.write(c_write2);
		  gemm_3_write.write(c_write3);
		  gemm_4_write.write(c_write4);
		  gemm_1.write(gemm1);
		  gemm_2.write(gemm2);
		  gemm_3.write(gemm3);
		  gemm_4.write(gemm4);
		  wstall_1.write(wstall1);
		  wstall_2.write(wstall2);
		  wstall_3.write(wstall3);
		  wstall_4.write(wstall4);
		  DWAIT();
	  }
	  c_write1=0;
	  c_write2=0;
	  c_write3=0;
	  c_write4=0;
	  gemm1=0;
	  gemm2=0;
	  gemm3=0;
	  gemm4=0;
	  wstall1=0;
	  wstall2=0;
	  wstall3=0;
	  wstall4=0;
	  wait();
  }
}
