void ACCNAME::Post1(){
int yoff[16];
int xoff[16];
ACC_DTYPE<32> ind[256];
sc_uint<8> pram1[256];
sc_uint<8> r1[256];
DATA ot[4][16];

#pragma HLS array_partition variable=yoff cyclic factor=4 dim=0
#pragma HLS array_partition variable=xoff cyclic factor=4 dim=0
#pragma HLS array_partition variable=ind complete dim=0
#pragma HLS array_partition variable=pram1 cyclic factor=4 dim=0
#pragma HLS array_partition variable=r1 cyclic factor=4 dim=0
#pragma HLS array_partition variable=ot complete dim=0


for (int i=0;i<16;i++){
	for (int j=0;j<4;j++){
		ot[j][i].tlast =false;
	}
}
wait();
	while(true){
		while(!write1.read())wait();

		for(int i=0;i<16;i++){
#pragma HLS unroll factor=4
#pragma HLS pipeline II=4
			yoff[i]=WRQ1.read();
			xoff[i]=WRQ2.read();
		}
		DWAIT(16);

		for(int i=0;i<256;i++){
#pragma HLS unroll
			ind[i] = g1[i];
		}
		wait();
		wait();
		DWAIT();
		for(int i=0;i<16;i++){
			for(int j=0;j<16;j+=4){
#pragma HLS pipeline II=1

				int yf1 = yoff[i];
				int xf1 = xoff[j+0];
				int xf2 = xoff[j+1];
				int xf3 = xoff[j+2];
				int xf4 = xoff[j+3];

				ACC_DTYPE<32> in1 = ind[(i*16)+j+0];
				ACC_DTYPE<32> in2 = ind[(i*16)+j+1];
				ACC_DTYPE<32> in3 = ind[(i*16)+j+2];
				ACC_DTYPE<32> in4 = ind[(i*16)+j+3];

				int aa1 = yf1 + xf1 + in1;
				int aa2 = yf1 + xf2 + in2;
				int aa3 = yf1 + xf3 + in3;
				int aa4 = yf1 + xf4 + in4;

				sc_int<32> a1 = (aa1)*l_offset;
				sc_int<32> a2 = (aa2)*l_offset;
				sc_int<32> a3 = (aa3)*l_offset;
				sc_int<32> a4 = (aa4)*l_offset;

				if(a1>MAX)a1 = MAX;
				if(a2>MAX)a2 = MAX;
				if(a3>MAX)a3 = MAX;
				if(a4>MAX)a4 = MAX;

				if(a1<MIN)a1 = MIN;
				if(a2<MIN)a2 = MIN;
				if(a3<MIN)a3 = MIN;
				if(a4<MIN)a4 = MIN;

				sc_int<64> r_a1 = a1 * rf;
				sc_int<64> r_a2 = a2 * rf;
				sc_int<64> r_a3 = a3 * rf;
				sc_int<64> r_a4 = a4 * rf;

				sc_int<32> bf_a1;
				sc_int<32> bf_a2;
				sc_int<32> bf_a3;
				sc_int<32> bf_a4;

				bf_a1 = (r_a1+POS)/DIVMAX;
				bf_a2 = (r_a2+POS)/DIVMAX;
				bf_a3 = (r_a3+POS)/DIVMAX;
				bf_a4 = (r_a4+POS)/DIVMAX;

				if(r_a1<0)bf_a1 = (r_a1+NEG)/DIVMAX;
				if(r_a2<0)bf_a2 = (r_a2+NEG)/DIVMAX;
				if(r_a3<0)bf_a3 = (r_a3+NEG)/DIVMAX;
				if(r_a4<0)bf_a4 = (r_a4+NEG)/DIVMAX;

				sc_int<32> f_a1 =(bf_a1);
				sc_int<32> f_a2 =(bf_a2);
				sc_int<32> f_a3 =(bf_a3);
				sc_int<32> f_a4 =(bf_a4);

				f_a1 = SHR(f_a1,r);
				f_a2 = SHR(f_a2,r);
				f_a3 = SHR(f_a3,r);
				f_a4 = SHR(f_a4,r);

				sc_int<32> rf_a1 = bf_a1 & mask;
				sc_int<32> rf_a2 = bf_a2 & mask;
				sc_int<32> rf_a3 = bf_a3 & mask;
				sc_int<32> rf_a4 = bf_a4 & mask;

				sc_int<32> lf_a1 = (bf_a1 < 0);
				sc_int<32> lf_a2 = (bf_a2 < 0);
				sc_int<32> lf_a3 = (bf_a3 < 0);
				sc_int<32> lf_a4 = (bf_a4 < 0);

				sc_int<32> tf_a1 = sm + lf_a1;
				sc_int<32> tf_a2 = sm + lf_a2;
				sc_int<32> tf_a3 = sm + lf_a3;
				sc_int<32> tf_a4 = sm + lf_a4;

				sc_int<32> af_a1 = (rf_a1 > tf_a1) + ra;
				sc_int<32> af_a2 = (rf_a2 > tf_a2) + ra;
				sc_int<32> af_a3 = (rf_a3 > tf_a3) + ra;
				sc_int<32> af_a4 = (rf_a4 > tf_a4) + ra;

				f_a1+= af_a1;
				f_a2+= af_a2;
				f_a3+= af_a3;
				f_a4+= af_a4;

				if(f_a1>MAX8)f_a1 = MAX8;
				else if (f_a1<0) f_a1 = 0;
				if(f_a2>MAX8)f_a2 = MAX8;
				else if (f_a2<0) f_a2 = 0;
				if(f_a3>MAX8)f_a3 = MAX8;
				else if (f_a3<0) f_a3 = 0;
				if(f_a4>MAX8)f_a4 = MAX8;
				else if (f_a4<0) f_a4 = 0;

				pram1[(i*16)+j+0] = f_a1.range(7,0);
				pram1[(i*16)+j+1] = f_a2.range(7,0);
				pram1[(i*16)+j+2] = f_a3.range(7,0);
				pram1[(i*16)+j+3] = f_a4.range(7,0);
			}
		}
		wait();
		DWAIT(92);


		//Rearrange
		for(int i=0;i<256;i++){
#pragma HLS unroll factor=4
			r1[i] = pram1[i];
		}
		DWAIT(192);
    	wait();

		//Map to douts
		for (int i=0;i<16;i++){
#pragma HLS unroll
			for (int j=0;j<4;j++){
#pragma HLS unroll
				ot[j][i].data.range(7,0)  = r1[i*16 + j*4 + 0];
				ot[j][i].data.range(15,8)  = r1[i*16 + j*4 + 1];
				ot[j][i].data.range(23,16)  = r1[i*16 + j*4 + 2];
				ot[j][i].data.range(31,24)  = r1[i*16 + j*4 + 3];
			}
		}

		int rb=WRQ1.read();
		int lb=WRQ2.read();
		bool lb12f = lb<12;
		bool lb8f = lb<8;
		bool lb4f = lb<4;
		bool rb15 = rb<15;
		bool rb14 = rb<14;
		bool rb13 = rb<13;
		bool rb12 = rb<12;
		bool rb11 = rb<11;
		bool rb10 = rb<10;
		bool rb9 = rb<9;
		bool rb8 = rb<8;
		bool rb7 = rb<7;
		bool rb6 = rb<6;
		bool rb5 = rb<5;
		bool rb4 = rb<4;
		bool rb3 = rb<3;
		bool rb2 = rb<2;
		bool rb1 = rb<1;

		wait();
												dout1.write(ot[0][0]);
		if((lb12f))								dout1.write(ot[1][0]);
		if((lb8f))								dout1.write(ot[2][0]);
		if((lb4f))								dout1.write(ot[3][0]);

		if(rb15)								dout2.write(ot[0][1]);
		if(rb15	&& (lb12f))						dout2.write(ot[1][1]);
		if(rb15 && (lb8f))						dout2.write(ot[2][1]);
		if(rb15	&& (lb4f))						dout2.write(ot[3][1]);

		if(rb14)								dout3.write(ot[0][2]);
		if(rb14 && (lb12f))						dout3.write(ot[1][2]);
		if(rb14 && (lb8f))						dout3.write(ot[2][2]);
		if(rb14 && (lb4f))						dout3.write(ot[3][2]);

		if(rb13)								dout4.write(ot[0][3]);
		if(rb13 && (lb12f))						dout4.write(ot[1][3]);
		if(rb13 && (lb8f))						dout4.write(ot[2][3]);
		if(rb13 && (lb4f))						dout4.write(ot[3][3]);

		wait();

		if(rb12)								dout1.write(ot[0][4]);
		if(rb12 && (lb12f))						dout1.write(ot[1][4]);
		if(rb12 && (lb8f))						dout1.write(ot[2][4]);
		if(rb12 && (lb4f))						dout1.write(ot[3][4]);

		if(rb11)								dout2.write(ot[0][5]);
		if(rb11 && (lb12f))						dout2.write(ot[1][5]);
		if(rb11 && (lb8f))						dout2.write(ot[2][5]);
		if(rb11 && (lb4f))						dout2.write(ot[3][5]);

		if(rb10)								dout3.write(ot[0][6]);
		if(rb10 && (lb12f))						dout3.write(ot[1][6]);
		if(rb10 && (lb8f))						dout3.write(ot[2][6]);
		if(rb10 && (lb4f))						dout3.write(ot[3][6]);

		if(rb9)									dout4.write(ot[0][7]);
		if(rb9 && (lb12f))						dout4.write(ot[1][7]);
		if(rb9 && (lb8f))						dout4.write(ot[2][7]);
		if(rb9 && (lb4f))						dout4.write(ot[3][7]);

		wait();

		if((rb8))								dout1.write(ot[0][8]);
		if((rb8)	&& (lb12f))					dout1.write(ot[1][8]);
		if(rb8 && (lb8f))						dout1.write(ot[2][8]);
		if(rb8 && (lb4f))						dout1.write(ot[3][8]);

		if((rb7))								dout2.write(ot[0][9]);
		if((rb7) && (lb12f))					dout2.write(ot[1][9]);
		if(rb7 && (lb8f))						dout2.write(ot[2][9]);
		if(rb7 && (lb4f))						dout2.write(ot[3][9]);

		if((rb6))								dout3.write(ot[0][10]);
		if((rb6) && (lb12f))					dout3.write(ot[1][10]);
		if(rb6 && (lb8f))						dout3.write(ot[2][10]);
		if(rb6 && (lb4f))						dout3.write(ot[3][10]);

		if((rb5))								dout4.write(ot[0][11]);
		if((rb5) && (lb12f))					dout4.write(ot[1][11]);
		if(rb5 && (lb8f))						dout4.write(ot[2][11]);
		if(rb5 && (lb4f))						dout4.write(ot[3][11]);

		wait();

		if((rb4))								dout1.write(ot[0][12]);
		if((rb4) && (lb12f))					dout1.write(ot[1][12]);
		if(rb4 && (lb8f))						dout1.write(ot[2][12]);
		if(rb4 && (lb4f))						dout1.write(ot[3][12]);

		if((rb3))								dout2.write(ot[0][13]);
		if((rb3) && (lb12f))					dout2.write(ot[1][13]);
		if(rb3 && (lb8f))						dout2.write(ot[2][13]);
		if(rb3 && (lb4f))						dout2.write(ot[3][13]);

		if((rb2))								dout3.write(ot[0][14]);
		if((rb2) && (lb12f))					dout3.write(ot[1][14]);
		if(rb2 && (lb8f))						dout3.write(ot[2][14]);
		if(rb2 && (lb4f))						dout3.write(ot[3][14]);

		if((rb1))								dout4.write(ot[0][15]);
		if((rb1) && (lb12f))					dout4.write(ot[1][15]);
		if(rb1 && (lb8f))						dout4.write(ot[2][15]);
		if(rb1 && (lb4f))						dout4.write(ot[3][15]);
		DWAIT(77);

		write1.write(0);
		wait();
	}
}
