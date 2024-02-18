#ifndef DS18B20_H__
#define DS18B20_H__
#include "main.h"



//IO操作                          
#define    DS18B20_Port GPIO_PIN_3 //数据端口PE8

//GPIO mode 选择
#define DS18B20_IO_IN()  {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=0<<3*2;}    //PA3模拟输入
#define DS18B20_IO_OUT() {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=1<<3*2;}     //PA3通常输出模式



extern unsigned char wendubuf1[2];//wendubuf[0]存扩大10倍的温度高8位   wendubuf[1]存扩大10倍的温度低8位   比如326 为32.6




void DS18B20_Start(void);// 启动一次18B20温度转换，返回值-表示是否启动成功 */  初始化值调用1次
short DS18B20_Get_Temp(void);//温度处理函数，包括把温度值存入数组    获取温度值调用
void DS18B20_Init(void);//温度传感器
void DS18B20_Rst(void);
void delay_us(uint32_t nus);
uint8_t DS18B20_Check(void);

#endif



