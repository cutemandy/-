#include <REGX52.H>
sbit DA=P2^2;
unsigned char wendu1,wendu2,shidu1,shidu2;
unsigned int wendu,shidu;
unsigned char TM1,TM2,TM3,TM4;
void DMS(unsigned int x_ms)		
{
	unsigned char i, j;
  while(x_ms)
	{	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
  x_ms--;
	}
}


unsigned char  DHT22_init()//��ʼ��
{
	unsigned char i;
	DA=1;
	DA=0;
	

	DMS(20);								
	DA=1;
	while(DA==1);
	i = 27;while (--i);     
	if(DA==1) return 1;
	i = 37;while (--i);			
	if(DA==0) return 1;
	while(DA==1); 			
	
	return 0;
}


unsigned char ReadHH()		//��ʪ�ȸ߰�λ
{
	unsigned char i,n,HH=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			HH=HH|(0X01<<(7-n));
		}
	}
	
	return HH;
}
	
unsigned char ReadHL()			//��ʪ�ȵͰ�λ
{
	unsigned char i,n,HL=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			HL=HL|(0X01<<(7-n));
		}
	}
	
	return HL;
	
}

unsigned char ReadTH()				//���¶ȸ߰�λ
{
	unsigned char i,n,TH=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			TH=TH|(0X01<<(7-n));
		}
	}
	
	return TH;
	
}

unsigned char ReadTL()			//���¶ȵͰ�λ
{
	unsigned char i,n,TL=0x00;
	for(n=0;n<8;n++)
	{
		while(DA==0);
		
		while (DA==1)
		{
			i = 17;while (--i);	
			if(DA==1)
			TL=TL|(0X01<<(7-n));
		}
	}
	
	return TL;
}

void getHT()//������ʪ��ֵ
{
		EA=0;
		DHT22_init();		
		wendu1=ReadHH();
		wendu2=ReadHL();
		shidu1=ReadTH();
		shidu2=ReadTL();
		wendu=wendu1<<8|wendu2;//����8λ ���ϵͰ�λ ���16λ�������ֲ�˵������ֵΪ��ʵֵ��10��
		shidu=shidu1<<8|shidu2;
		TM1=shidu/10;//�õ�ʵ��ֵ
		TM2=shidu%10;
		TM3=wendu/10;
		TM4=wendu%10;
		EA=1;
}





//#include <REGX52.H>
//#include <intrins.h>
//#define uchar unsigned char
//#define uint unsigned int
//uchar rec_dat[9];
//uint shidu,wendu;
//uchar shidu1,shidu2,wendu1,wendu2;
//sbit Data=P1^7;   //����������

//void DHT22_delay_us(uchar n)
//{
//    while(--n);
//}

//void DHT22_delay_ms(uint z)
//{
//   uint i,j;
//   for(i=z;i>0;i--)
//      for(j=110;j>0;j--);
//}

//void DHT22_start()
//{
//   Data=1;
//   DHT22_delay_us(2);
//   Data=0;
//   DHT22_delay_ms(20);   //��ʱ18ms����
//   Data=1;
//   DHT22_delay_us(30);
//}

//uchar DHT22_rec_byte()      //����һ���ֽ�
//{
//   uchar i,dat=0;
//  for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
//   {          
//      while(!Data);  // �ȴ�50us�͵�ƽ��ȥ
//      DHT22_delay_us(8);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0 
//      dat<<=1;           //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
//      if(Data==1)    //����Ϊ1ʱ��ʹdat��1����������1
//         dat+=1;
//      while(Data);  //�ȴ�����������    
//    }  
//    return dat;
//}

//void DHT22_receive()      //����40λ������
//{
//    uchar R_H,R_L,T_H,T_L,revise; 
//		EA=0;
//    DHT22_start();
//    if(Data==0)
//    {
//        while(Data==0);   //�ȴ�����     
//        DHT22_delay_us(40);  //���ߺ���ʱ80us
//        R_H=DHT22_rec_byte();    //����ʪ�ȸ߰�λ  
//        R_L=DHT22_rec_byte();    //����ʪ�ȵͰ�λ  
//        T_H=DHT22_rec_byte();    //�����¶ȸ߰�λ  
//        T_L=DHT22_rec_byte();    //�����¶ȵͰ�λ
//        revise=DHT22_rec_byte(); //����У��λ

//        DHT22_delay_us(25);    //����

////        if((R_H+R_L+T_H+T_L)==revise)      //У��
////        {
//            shidu=R_H*256+R_L;
//						wendu=T_H*256+T_L;
//						shidu1=shidu/10;//����
//						shidu2=shidu%10;//С��
//						wendu1=wendu/10;
//						wendu2=wendu%10;
////        } 
//       
//				EA=1;
//    }
//}



//#include <REGX52.H>
//sbit DA=P1^7;

//void dms(unsigned int x_ms)		
//{
//	unsigned char i, j;
//  while(x_ms)
//	{	i = 2;
//	j = 239;
//	do
//	{
//		while (--j);
//	} while (--i);
//  x_ms--;
//	}
//}


//unsigned char  DHT22_init()
//{
//	unsigned char i;
//	DA=1;
//	DA=0;
//	

//	dms(20);								//delay 1ms
//	DA=1;
//	while(DA==1);
//	i = 27;while (--i);     //delay 60us
//	if(DA==1) return 1;
//	i = 37;while (--i);			//delay 80us
//	if(DA==0) return 1;
//	while(DA==1); 			
//	
//	return 0;
//}


//unsigned char ReadHH()		//��ʪ�ȸ߰�λ
//{
//	unsigned char i,n,HH=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			HH=HH|(0X01<<(7-n));
//		}
//	}
//	
//	return HH;
//}
//	
//unsigned char ReadHL()			//��ʪ�ȵͰ�λ
//{
//	unsigned char i,n,HL=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			HL=HL|(0X01<<(7-n));
//		}
//	}
//	
//	return HL;
//	
//}

//unsigned char ReadTH()				//���¶ȸ߰�λ
//{
//	unsigned char i,n,TH=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			TH=TH|(0X01<<(7-n));
//		}
//	}
//	
//	return TH;
//	
//}

//unsigned char ReadTL()			//���¶ȵͰ�λ
//{
//	unsigned char i,n,TL=0x00;
//	for(n=0;n<8;n++)
//	{
//		while(DA==0);
//		
//		while (DA==1)
//		{
//			i = 17;while (--i);	//delau 40us
//			if(DA==1)
//			TL=TL|(0X01<<(7-n));
//		}
//	}
//	
//	return TL;
//}







