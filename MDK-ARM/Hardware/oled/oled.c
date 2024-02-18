#include "oled.h"
#include "oledfont.h"

//几个变量声明
//uint8_t **Hzk;
I2C_HandleTypeDef hi2c1;
//初始化命令
uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0x81, 0xCF, 0xA1, 0xC8, 0xA6, 0xA8, 0x3f,
					
0xD3, 0x00, 0xD5, 0x80, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x40, 0x20,
					
0x02, 0x8D, 0x14, 0xA4, 0xA6, 0xAF};
/**********************************************
//IIC Start
**********************************************/

void OLED_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊ��������ģʽ
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//�ж��Ƿ���յ�OLED Ӧ���ź�
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊͨ���������ģʽ
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void OLED_WR_CMD(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLED_WR_DATA(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_Init()
{
	/*uint8_t i = 0;
	for(i=0; i<28; i++){
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,2,0x100);
	}*/
	OLED_WR_CMD(0xAE);//--turn off oled panel
	OLED_WR_CMD(0x00);//---set low column address
	OLED_WR_CMD(0x10);//---set high column address
	OLED_WR_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_CMD(0x81);//--set contrast control register
	OLED_WR_CMD(0xCF);// Set SEG Output Current Brightness
	OLED_WR_CMD(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_CMD(0xA6);//--set normal display
	OLED_WR_CMD(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WR_CMD(0x3f);//--1/64 duty
	OLED_WR_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_CMD(0x00);//-not offset
	OLED_WR_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WR_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_CMD(0xD9);//--set pre-charge period
	OLED_WR_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_CMD(0xDA);//--set com pins hardware configuration
	OLED_WR_CMD(0x12);
	OLED_WR_CMD(0xDB);//--set vcomh
	OLED_WR_CMD(0x40);//Set VCOM Deselect Level
	OLED_WR_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_CMD(0x02);//
	OLED_WR_CMD(0x8D);//--set Charge Pump enable/disable
	OLED_WR_CMD(0x14);//--set(0x10) disable
	OLED_WR_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Clear();
	OLED_WR_CMD(0xAF);
}
//水平滚动
void OLED_ver_scroll()
{
  OLED_WR_CMD(0x2e);//关滚动
  OLED_WR_CMD(0x27);//29向右，2a向左
  OLED_WR_CMD(0x00);//A:空字节
  OLED_WR_CMD(0x02);//B:水平起始页
  OLED_WR_CMD(0x07);//C:水平滚动速度
  OLED_WR_CMD(0x03);//D:水平结束页
  OLED_WR_CMD(0x00);//E:每次垂直滚动位移
  OLED_WR_CMD(0xFF);//E:每次垂直滚动位移
  OLED_WR_CMD(0x2f);//开滚动
  
}
////向设备写控制命令
//void OLED_WR_CMD(uint8_t cmd)
//{
//	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
//}
//向设备写数据
//void OLED_WR_DATA(uint8_t data)
//{
//	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
//}
//初始化oled屏幕

//清屏size12 size16要清两行，其他函数有类似情况
void OLED_Clear()
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0x00);
	} 
}
void OLED_Clear_H(unsigned char H)
{
	uint8_t i,n;		    
	for(i=H*2-2;i<H*2;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0x00);
	} 
}
//清行
void OLED_Clearrow(uint8_t i)
{
	uint8_t n;
	OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x20); 
		OLED_WR_CMD (0x00); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0x00);
}
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
}		   			 
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 	
	OLED_WR_CMD(0xb0+y);
	OLED_WR_CMD(((x&0xf0)>>4)|0x10);
	OLED_WR_CMD(x&0x0f);
}
//画竖线
void Draw_Vline(unsigned char y0,unsigned char y,unsigned char x0)
{	
	unsigned char y1,y2,pos,pos1,mod0,mod1,i;
		if(y0%8==0){
		 y1=y0/8;
		 pos = 0xff;
	}
	else{
		y1 = y0/8;
		mod0 = y0%8;
		switch(mod0)
		{
			case 1:
			pos = 0xfe;
			break;
			case 2:
			pos = 0xfc;
			break;
			case 3:
			pos = 0xf8;
			break;
			case 4:
			pos = 0x10;
			break;
			case 5:
			pos = 0xe0;
			break;
			case 6:
			pos = 0xc0;
			break;
			case 7:
			pos = 0x80;
			break;
			default:
			break;

		}
	}
	if(y%8==0){
		 y2=y/8;
		 pos1 = 0x01;
	}
	else{
		y2 = y/8;
		mod1 = y%8;
		switch(mod1)
		{
			case 1:
			pos1 = 0x03;
			break;
			case 2:
			pos1 = 0x07;
			break;
			case 3:
			pos1 = 0x0f;
			break;
			case 4:
			pos1 = 0x1f;
			break;
			case 5:
			pos1 = 0x3f;
			break;
			case 6:
			pos1 = 0x7f;
			break;
			case 7:
			pos1 = 0xff;
			break;
			default:
			break;

		}
	}
	OLED_Set_Pos(x0,y1);
	OLED_WR_DATA(pos);

	OLED_Set_Pos(x0,y1+1);
	for(i=y1+1;i<y2;i++)
	{

		OLED_Set_Pos(x0,i);
		OLED_WR_DATA(0xff);	
	} 
   	OLED_Set_Pos(x0,y2);

	OLED_WR_DATA(pos1);


}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_DATA(0x00); 
	} //更新显示
}
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size <=16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8x16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8x16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_DATA(F6x8[c][i]);
				
			}
}
 //显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 


void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}


//z_len为整数显示位数，f_len为小数显示位数，size2为字体大小
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow;
	int z_temp,f_temp;      
	z_temp=(int)num;
	//整数部分
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2); 
	
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2); 
	}
}

unsigned char Hzk[][16]=
{

{0x10,0x60,0x02,0x8C,0x20,0x18,0x08,0x08,0x88,0x7F,0x88,0x08,0x28,0x18,0x08,0x00},
{0x04,0x04,0x7E,0x01,0x80,0x40,0x30,0x0C,0x03,0x00,0x3F,0x40,0x40,0x40,0x70,0x00},/*"沈",0*/
{0x00,0xFE,0x02,0x22,0xDA,0x06,0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0xFC,0x00,0x00},
{0x00,0xFF,0x08,0x10,0x08,0x07,0x00,0xFF,0x41,0x41,0x41,0x41,0x41,0xFF,0x00,0x00},/*"阳",1*/
{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00},
{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00},/*"工",2*/
{0x40,0x30,0x11,0x96,0x90,0x90,0x91,0x96,0x90,0x90,0x98,0x14,0x13,0x50,0x30,0x00},
{0x04,0x04,0x04,0x04,0x04,0x44,0x84,0x7E,0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x00},/*"学",3*/
{0x00,0xFE,0x22,0x5A,0x86,0x10,0x0C,0x24,0x24,0x25,0x26,0x24,0x24,0x14,0x0C,0x00},
{0x00,0xFF,0x04,0x08,0x07,0x80,0x41,0x31,0x0F,0x01,0x01,0x3F,0x41,0x41,0x71,0x00},/*"院",4*/	
	
	
{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"温",5*/

{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",6*/

{0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00},/*"湿",7*/
{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",8*/

{0x00,0xFC,0x84,0x84,0xFC,0x40,0x44,0x54,0x65,0x46,0x44,0x64,0x54,0x44,0x40,0x00},
{0x00,0x3F,0x10,0x10,0x3F,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00},/*"暗",9*/

{0x00,0x3E,0x22,0xE2,0x22,0x3E,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0xE2,0x02,0x00},
{0x20,0x3F,0x20,0x1F,0x11,0x11,0x00,0x7F,0x44,0x44,0x44,0x44,0x44,0x47,0x40,0x00},/*"距",10*/
{0x04,0x04,0x04,0xF4,0x84,0xD4,0xA5,0xA6,0xA4,0xD4,0x84,0xF4,0x04,0x04,0x04,0x00},
{0x00,0xFE,0x02,0x02,0x12,0x3A,0x16,0x13,0x12,0x1A,0x32,0x42,0x82,0x7E,0x00,0x00},/*"离",11*/

{0x80,0x70,0x00,0xFF,0x10,0x08,0xFE,0x42,0x42,0x42,0xFA,0x42,0x42,0x42,0xFE,0x00},
{0x80,0x60,0x18,0x07,0x08,0x10,0xFF,0x50,0x48,0x46,0x41,0x42,0x4C,0x40,0xFF,0x00},/*"烟",12*/
{0x10,0x0C,0x05,0x55,0x55,0xD5,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00},
{0x10,0x10,0x10,0x8A,0xA9,0x6B,0x35,0x25,0x25,0xAB,0xE9,0x08,0x10,0x10,0x10,0x00},/*"雾",13*/


{0x00,0x00,0x0C,0x12,0x12,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"°",6*/
{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},/*"c",7*/






{0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00},
{0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00},/*"光",0*/
{0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x42,0xA2,0x9E,0x82,0xA2,0xC2,0xBE,0x00,0x00},
{0x80,0x6F,0x08,0x08,0x28,0xCF,0x00,0x00,0x2F,0xC8,0x08,0x08,0x28,0xCF,0x00,0x00},/*"照",1*/














};
//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_DATA(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_DATA(Hzk[2*no+1][t]);
				adder+=1;
      }					
}
