#include<iostream>
#include<fstream>

using namespace std;
	 char txfreq[6];
     char rxfreq[6];
	 char slove[256];
    float fr;
    char regset[]={0x00,0x7C,0x04,0x02,0x40,0xF1, 0x00 ,0xED ,0xE0 ,0x17 ,0xE0 ,0xE0 ,0x43 ,0x85 ,0x00 ,0x07 ,
0x66 ,0xA0 ,0xFF ,0xFF,0x00 ,0x03 ,0x04 ,0x0E ,0x40 ,0xF1 ,0x00 ,0xED ,0xE0 ,0x3F ,0xE0 ,0xE0 ,
0x43 ,0x85 ,0x00 ,0x07 ,0x66 ,0xA0 ,0xFF ,0xFF ,0xE0 ,0xFF ,0xE0 ,0xFF ,0x00 ,0x03 ,0x3C ,0x9E,
0x00 ,0x1F ,0xc1 ,0xD1 ,0x0F ,0x20 ,0xFF ,0x01 ,0x00 ,0xE0 ,0x35 ,0x02 ,0xE0 ,0x88 ,0xFF ,0xFF };
	
 char HIGHBYTE(short int bi)//Get higer 8 bit
	 {
		 unsigned char highER ;
		 highER=bi>>8;
		 return highER;
	 }
 char 	LOWBYTE(short int bi)//Get lower 8 bit
	 {
		 unsigned char lowER;
		 lowER=bi&0xff;
		 return lowER;
	 }
 short int HIGHBYTE16( int bi)//Get higer 16 bit
	 {
		 short int highER;
		 highER=bi>>16;
		 return highER;
	 }
 short int 	LOWBYTE16( int bi)//Get lower 16 bit
	 {
		 short int lowER;
		 lowER=bi&0xffff;
		 return lowER;
	 }
	 
void calc(float freq)
{
	int rx,tx;
	short int rxh,rxl,txh,txl;
	char ndiv;
	if(freq<=512&&freq>=384)
	{
	 txfreq[5]=0x00;
	 rxfreq[5]=0x00;
	 txfreq[4]=0x00;
	 rxfreq[4]=0x00;
	 ndiv=4;
    }
	if(freq<=170&&freq>=128)
	{
	 txfreq[5]=0x20;
	 rxfreq[5]=0x20;
	 txfreq[4]=0x02;
	 rxfreq[4]=0x02;
	 ndiv=12;
    }
	if(freq<=57&&freq>=43)
	{
	 txfreq[5]=0x80;
	 rxfreq[5]=0x80;
	 txfreq[4]=0x08;
	 rxfreq[4]=0x08;
	 ndiv=36;
	}
	if(freq<=46&&freq>=35)
	{
	 txfreq[5]=0xa0;
	 rxfreq[5]=0xa0;
	 txfreq[4]=0x0a;
	 rxfreq[4]=0x0a;
	 ndiv=44;
    }
	if(freq<=32&&freq>=24)
	{
	 txfreq[5]=0xc0;
	 rxfreq[5]=0xc0;
	 txfreq[4]=0x0f;
	 rxfreq[4]=0x0f;
	 ndiv=64;
    }
	else if(freq<24||freq>512)
	cout<<"error: Frequency is out of range! 超出允许频率范围"<<endl;

	rx=((freq-0.137)*ndiv*16777216)/21.25;
	tx=(freq*ndiv*16777216)/21.25;
	cout<<hex<<tx<<endl;
	cout<<hex<<rx<<endl;
	txh=HIGHBYTE16(tx);
	txl=LOWBYTE16(tx);
	
	rxh=HIGHBYTE16(rx);
	rxl=LOWBYTE16(rx);
	
	txfreq[1]=HIGHBYTE(txh);
	txfreq[0]=LOWBYTE(txh);
	txfreq[3]=HIGHBYTE(txl);
	txfreq[2]=LOWBYTE(txl);
	
	rxfreq[1]=HIGHBYTE(rxh);
	rxfreq[0]=LOWBYTE(rxh);
	rxfreq[3]=HIGHBYTE(rxl);
	rxfreq[2]=LOWBYTE(rxl);

}
int main()
{
    int s=0;

	ofstream ofs;
	cout << "4802N FREQ CALC BIN_GEN" << endl;
	ofs.open("C:\\BK4802NE2PROM.BIN",ios::out|ios::binary);
	cout << "默认收发频率一致，即直频。对于只有发射功能的也适用" << endl;
	cout<<"strat. please key in 16 channels frequency(MHz)"<<endl;
	cout << "请以次输入16个信道的频率，单位是(MHz)，输入完一个信道频率后按enter，计算下一个信道频率，直到16个都输入完毕" << endl;
	if(ofs)
	{
		for(int i=0;i<16;i++)
		{
		    cout<<dec<<"Key in channel "<<i+1<<":"<<endl;
		  	cin>>fr;
		  	calc(fr);
		  	cout<<"write this channel freq is:"<<fr<<endl;

		  for(int k=1;k<=6;k++)
		  {
		  	slove[s]=txfreq[k-1];
		  	s++;
		  }
		slove[s]=0x00;
		s++;
		slove[s]=0x00;
		s++;
		for(int j=1;j<=6;j++)
		  {
		  	slove[s]=rxfreq[j-1];
		  	s++;
		  }
		  slove[s]=0x00;
		s++;
		slove[s]=0x00;
        s++;
	    }
	    
	    
         ofs.write(slove,256);
          ofs.write(regset,64);
          cout<<dec<<s<<endl;
   } 
   ofs.close();
   cout<<"Have write finish!,please go to D:\\ to see BK4802NE2PROM.BIN to write to 24C04!"<<endl;
   cout << "文件已经创建，请到C盘根目录下查看BK4802NE2PROM.BIN，" << endl;
   cout << "确认存在此文件后就可用CH341编程器24c04 EEPROM进行烧录写频。" << endl;
   system("pause");
   return 0;
}

//00 7C 04 02 40 F1 00 ED E0 17 E0 E0 43 85 00 07 
//66 A0 FF FF 00 03 04 0E 40 F1 00 ED E0 3F E0 E0 
//43 85 00 07 66 A0 FF FF E0 FF E0 FF 00 03 3C 9E 
//00 1F c1 D1 0F 20 FF 01 00 E0 35 02 E0 88 FF FF 

