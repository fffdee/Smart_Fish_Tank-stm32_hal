#include "ds18b20.h"

unsigned char wendubuf1[2];//wendubuf[0]存扩大10倍的温度高8位   wendubuf[0]存扩大10倍的温度低8位   比如326 为32.6
typedef unsigned char u8; 


void DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = DS18B20_Port;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//普通输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;//50MHz
	GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//初始化
	
  
}
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL=0X00;//清空计数器
	SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
	do
	{
		temp=SysTick->CTRL;//读取当前倒计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
	SysTick->CTRL=0x00; //关闭计数器
	SysTick->VAL =0X00; //清空计数器
}

void DS18B20_Rst()      //复位DS18B20
{                    
	DS18B20_IO_OUT(); //设置为输出模式    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);
	//拉低DQ    
	delay_us(750);    //拉低750us（至少480us)   
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);	
	//DQ=1拉高释放总线    
	delay_us(15);     //15US    
	//进入接受模式，等待应答信号。
}


/* 复位总线，获取存在脉冲，以启动一次读写操作 */
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

//返回值：1/0

u8 DS18B20_Read_Bit(void)              // read one bit

{
	u8 data;    
	DS18B20_IO_OUT();//设置为输出    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET); //输出低电平2us    
	delay_us(2);    
	HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);	 //拉高释放总线    
	DS18B20_IO_IN();//设置为输入    
	delay_us(12);//延时12us  
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3))data=1;//读取总线数据    
	else data=0;        
	delay_us(50);  //延时50us            
	return data;
}

//从DS18B20读取一个字节 //返回值：读到的数据
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
DS18B20_IO_OUT();//设置PA0为输出    
for (j=1;j<=8;j++)    
{        
	testb=dat&0x01;        
	dat=dat>>1;        
	if (testb) //输出高      
	{          
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);// 主机输出低电平            
		delay_us(2);                  //延时2us            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);//释放总线            
		delay_us(60); //延时60us                    
	}        
	else //输出低        
	{            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_RESET);//主机输出低电平            
		delay_us(60);               //延时60us            
		HAL_GPIO_WritePin(GPIOA, DS18B20_Port, GPIO_PIN_SET);//释放总线            
		delay_us(2);                  //延时2us              
	}    
}
}



//开始温度转换
void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert
}

//初始化 DS18B20 的 IO 口 DQ 同时检测 DS 的存在
//返回 1:不存在
//返回 0:存在

//从 ds18b20 得到温度值
//精度： 0.1C
//返回值：温度值 （ -550~1250）
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
		temp=0; //温度为负
	}
	else 
	{
		temp=1; //温度为正
	}
	tem=TH; //获得高八位
	tem<<=8;
	tem+=TL; //获得底八位
	tem=(double)tem*0.625;//转换*10倍
	if(temp)
	{
		return tem; //返回温度值
	}
	else 
	{
		return -tem;
	}
}



