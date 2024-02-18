#ifndef DS18B20_H__
#define DS18B20_H__
#include "main.h"



//IO����                          
#define    DS18B20_Port GPIO_PIN_3 //���ݶ˿�PE8

//GPIO mode ѡ��
#define DS18B20_IO_IN()  {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=0<<3*2;}    //PA3ģ������
#define DS18B20_IO_OUT() {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=1<<3*2;}     //PA3ͨ�����ģʽ



extern unsigned char wendubuf1[2];//wendubuf[0]������10�����¶ȸ�8λ   wendubuf[1]������10�����¶ȵ�8λ   ����326 Ϊ32.6




void DS18B20_Start(void);// ����һ��18B20�¶�ת��������ֵ-��ʾ�Ƿ������ɹ� */  ��ʼ��ֵ����1��
short DS18B20_Get_Temp(void);//�¶ȴ��������������¶�ֵ��������    ��ȡ�¶�ֵ����
void DS18B20_Init(void);//�¶ȴ�����
void DS18B20_Rst(void);
void delay_us(uint32_t nus);
uint8_t DS18B20_Check(void);

#endif



