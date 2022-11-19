#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "DHT22.h"
#include "AT24C02.h"
#include "Timer0.h"
#include "Key.h"
#include "BH1750.h"

sbit motor=P2^3;
sbit hot=P2^4;
sbit jiashi=P3^6;
sbit chushi=P3^7;
sbit buzzer=P3^5;
unsigned char keynum,slectflag,flash;
unsigned char RepublicOfChina;
unsigned char mode,lflag;
unsigned char TH_newflag,LX_newflag;
unsigned int Lx,i,j,compare;
unsigned char set[]={0,0,0,0};//���δ�RL RH TL TH
unsigned int LXset[]={0,0};//���δ�LL LH
unsigned int  dianjif[]={0x01,0x02,0x04,0x08};//���������ת����
unsigned int  dianjiz[]={0x08,0x04,0x02,0x01};//���������ת����
unsigned char dakai=1,guanbi=0;//Ĭ���������
unsigned char temp1,temp2,temp3,temp4;



void setTH()//������ʪ�ȱ�����ֵ
{
	if(keynum==2)//���¡��л���ǰλ������
	{
		slectflag++;
		if(slectflag>=4){slectflag=0;}
	}
	if(keynum==3)//��ǰλ+1
	{
		set[slectflag]++;
		TH_newflag=1;//��ֵ���º� ��ʪ�ȴ洢��־λ��1
	}
	if(keynum==4)//��ǰλ-1
	{
		set[slectflag]--;
		TH_newflag=1;//��ֵ���º� ��ʪ�ȴ洢��־λ��1
	}
	if(set[0]>=set[1]){set[0]=set[1]-1;}//Խ���ж�
	if(set[2]>=set[3]){set[2]=set[3]-1;}
	
	if(TH_newflag==1)//��ֵ���º�Ŵ���EEPROM ���򲻴�
	{
		EA=0;//��ֵ��ʱ��رն�ʱ�ж����EA �������
		AT24C02_WriteByte(0,set[0]);//��RL
		delay(5); 
		AT24C02_WriteByte(1,set[1]);//��RH
		delay(5);
		AT24C02_WriteByte(2,set[2]);//��TL
		delay(5);
		AT24C02_WriteByte(3,set[3]);//��TH
		delay(5);
		EA=1;
		TH_newflag=0;
	}

	
	//����Ϊ������ֵ��LCD��ʾ���ִ���
	LCD_ShowString(1,1,"H:");
	LCD_ShowString(1,5,"%-");
	if(slectflag==0 && flash==1){LCD_ShowString(1,3,"  ");}//��˸
	else {LCD_ShowNum(1,3,set[0],2);}//H����
	if(slectflag==1 && flash==1){LCD_ShowString(1,7,"  ");}
	else {LCD_ShowNum(1,7,set[1],2);}//H����
	LCD_ShowString(1,9,"%");
	LCD_ShowString(1,10,"       ");
	
	
	LCD_ShowString(2,1,"T:");
	LCD_ShowChar(2,5,0xDF);
	LCD_ShowString(2,6,"C-");
	if(slectflag==2 && flash==1){LCD_ShowString(2,3,"  ");}
	else {LCD_ShowNum(2,3,set[2],2);}//T����
	LCD_ShowChar(2,5,0xDF);
	if(slectflag==3 && flash==1){LCD_ShowString(2,8,"  ");}
	else {LCD_ShowNum(2,8,set[3],2);}//T����
	LCD_ShowChar(2,10,0xDF);
	LCD_ShowString(2,11,"C");
	LCD_ShowString(2,12,"     ");
}




void setLX()//���ù�ǿ������ֵ
{
	if(keynum==2)//���¡��л���ǰλ������
	{
		lflag++;
		if(lflag>=2){lflag=0;}
	}
	if(keynum==3)//��ǰλ+1
	{
		LXset[lflag]++;
		LX_newflag=1;//��ֵ���º� ���մ洢��־λ��1
	}
	if(keynum==4)//��ǰλ-1
	{
		LXset[lflag]--;
		LX_newflag=1;//��ֵ���º� ���մ洢��־λ��1
	}
	if(LXset[0]>=LXset[1]){LXset[0]=LXset[1]--;}//Խֵ�ж�
	temp1=LXset[0]/256;//��16λ������ֵ ��Ϊ����8λ ��EEPROM
	temp2=LXset[0]%256;
	temp3=LXset[1]/256;
	temp4=LXset[1]%256;
	if(LX_newflag==1)//������ֵ��Ŵ����� ���򲻴�
	{
		EA=0;//��ֵʱ�ڹرն�ʱ�ж���� �������
		AT24C02_WriteByte(4,temp1);
		delay(5); 
		AT24C02_WriteByte(5,temp2);//��LL
		delay(5);
		AT24C02_WriteByte(6,temp3);
		delay(5); 
		AT24C02_WriteByte(7,temp4);//��LH
		delay(5);
		EA=1;
		LX_newflag=0;
	}
	//����Ϊ��ǿ��ֵ��LCD��ʾ���ִ���
	
	LCD_ShowString(1,1,"light limit is:");
	
	if(lflag==0 && flash==1){LCD_ShowString(2,1,"     ");}
	else {LCD_ShowNum(2,1,LXset[0],5);}//LX����
	if(lflag==1 && flash==1){LCD_ShowString(2,9,"     ");}
	else {LCD_ShowNum(2,9,LXset[1],5);}//LX����
	LCD_ShowString(2,6,"Lx-");
	LCD_ShowString(2,14,"Lx");
	
}




void THwarn()
{
	
		if(TM1>=set[3] || TM1<set[2])//�¶��쳣
			{
				
				if(TM1>=set[3])//�¶ȹ��� ����
					{
						if(TM1<(set[3]/2)+set[3])//������1.5��
						{
							compare=1500;//ռ�ձȵ�Ϊ50% ��������
						}
						else
						{
							compare=3000;//ռ�ձ�100% ȫ������
						}
					}
				else
					{
						compare=0;//�ر�����
					}
				if(TM1<set[2])//�¶ȹ��� ��������
					{
						hot=0;//��������
						//compare=0;
					}
				else
					{
						//compare=0;
						//motor=1;
						hot=1;//�رռ���
					}
			}
			else//�¶����� �ر��¿��豸
			{
				compare=0;
				hot=1;
			}
		
		if(TM3>=set[1] || TM3<set[0])//ʪ���쳣
		{
			
			if(TM3>=set[1]){chushi=0;}//���� ������ʪ��
			if(TM3<set[0]){jiashi=0;}//���� ������ʪ��
		}
		else//ʪ������ �ر�ʪ���豸
		{
			
			jiashi=1;
			chushi=1;
		}
		
}



void LXwarn()
{
	if(Lx>=LXset[1] || Lx<LXset[0])
	{
		if(Lx<LXset[0] && dakai==1 && guanbi==0)//���� ���������ת
		{
			EA=0;//�رն�ʱ�ж����
			for(i=0;i<2150;i++)
				{
					j=i;j=j%4;
					P1=dianjiz[j];
					delay(2);
				}
				dakai=0;guanbi=1;//��־λ��ת �������Ѵ� ����������� ���Ҳ����ת����
			EA=1;
		}
		if(Lx>=LXset[1] && guanbi==1 && dakai==0 )//��ǿ ���������ת
		{
			EA=0;
			for(i=0;i<2150;i++)
				{
					j=i;j=j%4;
					P1=dianjif[j];
					delay(2);
				}
				dakai=1;guanbi=0;//��־λ��ת
			EA=1;
		}
		
	}
	else//�������� �����������
	{
		P1=0x00;
	}
}



void buzzerwarn()//�쳣״̬����������
{
	if(TM1>=set[3] || TM1<set[2] || TM3>=set[1] || TM3<set[0] || Lx>=LXset[1] || Lx<LXset[0])
	{
		buzzer=0;
	}
	else
	{
		buzzer=1;
	}
}



void showTH()
{
		
		LCD_ShowString(1,1,"H:");//������������ʾ
		LCD_ShowNum(1,3,TM3,2);	
		LCD_ShowString(1,5,".");
		LCD_ShowNum(1,6,TM4,1);
		LCD_ShowString(1,7,"%");
		LCD_ShowString(1,8," ");
	

	
	
		LCD_ShowString(1,9,"T:"); 
		LCD_ShowNum(1,11,TM1,2);
		LCD_ShowString(1,13,".");
		LCD_ShowNum(1,14,TM2,1);
		LCD_ShowChar(1,15,0xDF);
		LCD_ShowString(1,16,"C");
		
	
	
	
		LCD_ShowString(2,1,"L:");
		LCD_ShowNum(2,3,Lx,5);
		LCD_ShowString(2,8,"Lx");
		LCD_ShowString(2,10,"        ");
		
		
		
}




void main()
{
	LCD_Init();//LCD��ʼ��
	set[0]=AT24C02_ReadByte(0);//����������ֵ
	set[1]=AT24C02_ReadByte(1);
	set[2]=AT24C02_ReadByte(2);
	set[3]=AT24C02_ReadByte(3);
	temp1=AT24C02_ReadByte(4);
	temp2=AT24C02_ReadByte(5);
	temp3=AT24C02_ReadByte(6);
	temp4=AT24C02_ReadByte(7);
	if(set[0]>=set[1] || set[2]>=set[3] || set[0]>99 || set[1]>99 || set[2]>99 || set[3]>99
	  	|| set[0]<0 || set[1]<0 || set[2]<0 || set[3]<0)//�������ı�����ֵ�쳣 ���ó�ֵ
	{
		EA=0;
		set[0]=40;
		
		set[1]=60;
		set[2]=21;
		set[3]=28;
		AT24C02_WriteByte(0,set[0]);//��RL
		delay(5);
		AT24C02_WriteByte(1,set[1]);//��RH
		delay(5);
		AT24C02_WriteByte(2,set[2]);//��TL
		delay(5);
		AT24C02_WriteByte(3,set[3]);//��TH
		delay(5);
		EA=1;
	}
//	if(temp2<=0 || temp4<=0 || temp1<=0 || temp3<=0 || temp1>5 || temp3>5 )
//	{
//		temp1=0;
//		temp2=20;
//		temp3=0;
//		temp4=50;
//		EA=0;
//		AT24C02_WriteByte(4,temp1);
//		delay(5); 
//		AT24C02_WriteByte(5,temp2);//��LL
//		delay(5);
//		AT24C02_WriteByte(6,temp3);
//		delay(5); 
//		AT24C02_WriteByte(7,temp3);//��LH
//		delay(5);
//		EA=1;
//	}
	LXset[0]=(temp1*256)+temp2;//������ǿ��ֵ�� ���ݴ���õ�16λ
	LXset[1]=(temp3*256)+temp4;
	Timer0Init();//��ʱ����ʼ�� 1ms����һ�ζ�ʱ�ж�
	motor=1;
	hot=1;
	buzzer=1;
	chushi=1;
	jiashi=1;
	P1=0x00;//���ϼ���Ϊ��ʼ��
	delay(2000);
	while(1)
	{
		keynum=Key();//��ȡ����ɨ��
		getHT();//�ɼ���ʪ�Ȳ�����
		delay(2000);//2s�ɼ�һ����ʪ��
		BH1750_Update();
		delay(500);//500ms�ɼ�һ�ι�ǿ
		Lx=BH1750_Read();//�ɼ���ǿ������
		if(keynum==1)//�������ü� �ı�ģʽ��־λ0��1��2ѭ��
		{
			
			if(mode==0){mode=1;slectflag=0;}
			else if(mode==1){mode=2;lflag=0;}
			else {mode=0;}
		}
		switch(mode)
		{
			case 0:showTH();break;//������������
			case 1:setTH();break;//������ʪ�ȱ�����ֵ
			case 2:setLX();break;//���ù�ǿ������ֵ
			default: break;
		}
		buzzerwarn();//������ģ��
		THwarn();//�¡�ʪ����ģ��
		LXwarn();//���ģ��
	
	}
}



void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count,count2,count3;
		
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	T0Count++;
	if(T0Count>=20)
	{
		T0Count=0;
		Key_Loop();//20msɨ��һ�μ���
	}
	count2++;
	if(count2>=500)
	{
		count2=0;
		flash=!flash;//������ֵ���� ��˸�� 500ms��־λ��ת
	}
	
		count3++;
		count3%=3000;//PWM����Ϊ3��
	
		if(count3<compare)
		{
			motor=0;
		}

		else
		{
			motor=1;//�رս���
		}
	
	
	
}