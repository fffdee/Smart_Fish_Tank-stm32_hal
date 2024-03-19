#include "ds18b20.h"

unsigned char wendubuf1[2];//wendubuf[0]������10�����¶ȸ�8λ   wendubuf[0]������10�����¶ȵ�8λ   ����326 Ϊ32.6
typedef unsigned char u8; 


void DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = DS18B20_Port;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//��ͨ���ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;//50MHz
	GPIO_InitStruct.Pull = GPIO_PULLUP;//����
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//��ʼ��
	
  
}
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL=0X00;//��ռ�����
	SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
	do
	{
		temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00; //�رռ�����
	SysTick->VAL =0X00; //��ռ�����
}

void DS18B20_Rst()      //��λDS18B20
{                    
	DS18B20_IO_OUT(); //����Ϊ���ģʽ    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);
	//����DQ    
	delay_us(750);    //����750us������480us)   
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);	
	//DQ=1�����ͷ�����    
	delay_us(15);     //15US    
	//�������ģʽ���ȴ�Ӧ���źš�
}


/* ��λ���ߣ���ȡ�������壬������һ�ζ�д���� */
uint8_t DS18B20_Check(void)       
{      
	uint8_t retry=0;    
	DS18B20_IO_IN();//SET PE8 INPUT        
	while (HAL_GPIO_ReadPin(GPIOA,DS18B20_Port)&&retry<200)    
	{            
		retry++;          
		delay_us(1);    
	};        
	if(retry>=200)return 1;    
	else retry=0;    

	while (!HAL_GPIO_ReadPin(GPIOA,DS18B20_Port)&&retry<240)    
	{            
		retry++;            
		delay_us(1);    
	};    
	if(retry>=240)return 1;            
	return 0;
}

//����ֵ��1/0

u8 DS18B20_Read_Bit(void)              // read one bit

{
	u8 data;    
	DS18B20_IO_OUT();//����Ϊ���    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET); //����͵�ƽ2us    
	delay_us(2);    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);	 //�����ͷ�����    
	DS18B20_IO_IN();//����Ϊ����    
	delay_us(12);//��ʱ12us  
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3))data=1;//��ȡ��������    
	else data=0;        
	delay_us(50);  //��ʱ50us            
	return data;
}

//��DS18B20��ȡһ���ֽ� //����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{            
	u8 i,j,dat;    
	dat=0;  
	for (i=1;i<=8;i++)  
	{        
		j=DS18B20_Read_Bit();        
		dat=(j<<7)|(dat>>1);    
	}                                
	return dat;
}


void DS18B20_Write_Byte(u8 dat)    
{                
u8 j;
u8 testb;    
DS18B20_IO_OUT();//����PA0Ϊ���    
for (j=1;j<=8;j++)    
{        
	testb=dat&0x01;        
	dat=dat>>1;        
	if (testb) //�����      
	{          
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);// ��������͵�ƽ            
		delay_us(2);                  //��ʱ2us            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);//�ͷ�����            
		delay_us(60); //��ʱ60us                    
	}        
	else //�����        
	{            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);//��������͵�ƽ            
		delay_us(60);               //��ʱ60us            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);//�ͷ�����            
		delay_us(2);                  //��ʱ2us              
	}    
}
}



//��ʼ�¶�ת��
void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert
}

//��ʼ�� DS18B20 �� IO �� DQ ͬʱ��� DS �Ĵ���
//���� 1:������
//���� 0:����

//�� ds18b20 �õ��¶�ֵ
//���ȣ� 0.1C
//����ֵ���¶�ֵ �� -550~1250��
short DS18B20_Get_Temp(void)
{
	uint8_t temp;
	uint8_t TL,TH;
	short tem;
	DS18B20_Start();// ds1820 start convert
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0xbe);// convert
	TL=DS18B20_Read_Byte(); // LSB
	TH=DS18B20_Read_Byte(); // MSB
	if(TH>7)
	{
		TH=~TH;
		TL=~TL;
		temp=0; //�¶�Ϊ��
	}
	else 
	{
		temp=1; //�¶�Ϊ��
	}
	tem=TH; //��ø߰�λ
	tem<<=8;
	tem+=TL; //��õװ�λ
	tem=(double)tem*0.625;//ת��*10��
	if(temp)
	{
		return tem; //�����¶�ֵ
	}
	else 
	{
		return -tem;
	}
}



