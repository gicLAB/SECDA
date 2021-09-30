void ACCNAME::PPU(int* x, int* y,sc_uint<32>* g1,sc_uint<32>* r1){
	sc_int<32> pram[16];
	for(int i=0;i<4;i++){
#pragma HLS unroll
		for(int j=0;j<4;j++){
#pragma HLS unroll
			pram[i*4+j] = g1[j*4+i]+ y[i]+x[j];
		}
	}
	sc_int<32> l_offset = (1 << left_s);
	int mask = (1 << right_s)-1;
	int r =right_s;
	int sm = mask  >> 1;
	wait();
	for(int i=0;i<16;i++){
#pragma HLS unroll factor=2 skip_exit_check
#pragma HLS pipeline II=1
		int aa1 = pram[i];
		sc_int<32> a1 = (aa1)*l_offset;
		if(a1>MAX)a1 = MAX;
		if(a1<MIN)a1 = MIN;

		sc_int<64> r_a1 = a1 * rf;
		sc_int<32> bf_a1;
		bf_a1 = (r_a1+POS)/DIVMAX;
		if(r_a1<0)bf_a1 = (r_a1+NEG)/DIVMAX;

		sc_int<32> f_a1 =(bf_a1);
		f_a1 = SHR(f_a1,r);
		sc_int<32> rf_a1 = bf_a1 & mask;
		sc_int<32> lf_a1 = (bf_a1 < 0);
		sc_int<32> tf_a1 = sm + lf_a1;
		sc_int<32> af_a1 = ((rf_a1 > tf_a1) & 1) + ra;
		f_a1+= af_a1;
		if(f_a1>MAX8)f_a1 = MAX8;
		else if (f_a1<0) f_a1 = 0;
		pram[i] = f_a1;
	}
	wait();

	for(int i=0;i<4;i++){
#pragma HLS unroll
		for(int j=0;j<4;j++){
#pragma HLS unroll
			r1[j*4+i] = pram[i*4+j];
		}
	}
	DWAIT(51);
}

void ACCNAME::Post1(){
int y[4];
int x[4];
#pragma HLS array_partition variable=y complete dim=0
#pragma HLS array_partition variable=x complete dim=0
wait();
	while(true){
		while(!write1.read())wait();
		for(int i=0;i<4;i++){
	#pragma HLS unroll
			y[i]=WRQ1.read();
		}

		for(int i=0;i<4;i++){
	#pragma HLS unroll
			x[i]=WRQ1.read();
		}
		PPU(x,y,g1,r1);
		arrange1.write(1);wait();
		while(arrange1.read())wait();
		write1.write(0);
		wait();
	}
}

void ACCNAME::Post2(){
int y[4];
int x[4];
#pragma HLS array_partition variable=y complete dim=0
#pragma HLS array_partition variable=x complete dim=0
wait();
	while(true){
		while(!write2.read())wait();
		for(int i=0;i<4;i++){
	#pragma HLS unroll
			y[i]=WRQ2.read();
		}

		for(int i=0;i<4;i++){
	#pragma HLS unroll
			x[i]=WRQ2.read();
		}
		PPU(x,y,g2,r2);
		arrange2.write(1);wait();
		while(arrange2.read())wait();
		write2.write(0);
		wait();
	}
}

void ACCNAME::Post3(){
int y[4];
int x[4];
#pragma HLS array_partition variable=y complete dim=0
#pragma HLS array_partition variable=x complete dim=0
wait();
	while(true){
		while(!write3.read())wait();
		for(int i=0;i<4;i++){
	#pragma HLS unroll
			y[i]=WRQ3.read();
		}

		for(int i=0;i<4;i++){
	#pragma HLS unroll
			x[i]=WRQ3.read();
		}
		PPU(x,y,g3,r3);
		arrange3.write(1);wait();
		while(arrange3.read())wait();
		write3.write(0);
		wait();
	}
}

void ACCNAME::Post4(){
int y[4];
int x[4];
#pragma HLS array_partition variable=y complete dim=0
#pragma HLS array_partition variable=x complete dim=0
wait();
	while(true){
		while(!write4.read())wait();
		for(int i=0;i<4;i++){
	#pragma HLS unroll
			y[i]=WRQ4.read();
		}

		for(int i=0;i<4;i++){
	#pragma HLS unroll
			x[i]=WRQ4.read();
		}
		PPU(x,y,g4,r4);
		arrange4.write(1);wait();
		while(arrange4.read())wait();
		write4.write(0);
		wait();
	}
}
