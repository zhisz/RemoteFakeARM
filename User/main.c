#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "AD.h"
#include "pack.h"
#include "filter.h"
#include "stdio.h"
#include "Key.h"
#include "Timer.h"



uint16_t Num1;
uint16_t Num2;

uint16_t ADC_value6[6];

uint8_t SendFlag;								//发送标志位
uint16_t SendSuccessCount, SendFailedCount;		//发送成功计次，发送失败计次

//uint8_t ReceiveFlag;							//接收标志位
//uint8_t ReceiveSuccessCount, ReceiveFailedCount;//接收成功计次，接收失败计次

int fputc(int ch,FILE *p) 
 
{
 
 USART_SendData(USART1,(u8)ch);
 
 while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
 
 return ch;
 
}

int main(void)
{
	/*初始化*/
	OLED_Init();
	NRF24L01_Init();
	AD_Init();
	Filter_InitAll();
	Key_Init();
	Timer_Init();
	
	
	/*OLED显示静态字符串*/
	
	OLED_ShowNum(108, 0, Num1,1,OLED_6X8);
	
	
	OLED_ShowString(0, 0, "T:00000-00000-0",OLED_6X8);		//格式为：T:发送成功计次-发送失败计次-发送标志位
	OLED_ShowString(0, 8, "CH1 HEX:0000 Num:0000",OLED_6X8);
	OLED_ShowString(0, 16, "CH2 HEX:0000 Num:0000",OLED_6X8);
	OLED_ShowString(0, 24, "CH3 HEX:0000 Num:0000",OLED_6X8);
	OLED_ShowString(0, 32, "CH4 HEX:0000 Num:0000",OLED_6X8);
	OLED_ShowString(0, 40, "CH5 HEX:0000 Num:0000",OLED_6X8);
	OLED_ShowString(0, 48, "CH6 HEX:0000 Num:0000",OLED_6X8);

	OLED_Update();
	
	/*初始化测试数据，此处值为任意设定，便于观察实验现象*/
	NRF24L01_TxPacket[0] = 0x00;
	NRF24L01_TxPacket[1] = 0x01;
	NRF24L01_TxPacket[2] = 0x02;
	NRF24L01_TxPacket[3] = 0x03;
	NRF24L01_TxPacket[4] = 0x04;
	NRF24L01_TxPacket[5] = 0x05;
	NRF24L01_TxPacket[6] = 0x06;
	NRF24L01_TxPacket[7] = 0x07;
	NRF24L01_TxPacket[8] = 0x08;
	NRF24L01_TxPacket[9] = 0x09;
	NRF24L01_TxPacket[10] = 0x10;
	NRF24L01_TxPacket[11] = 0x11;
	
	while (1)
	{
		
		
			/*示例3*/
		if (Key_Check(KEY_1, KEY_SINGLE))
		{
			Num1 = Num1 ? 0:1;
		}
		OLED_ShowNum(108, 0, Num1,1,OLED_6X8);
		
//		printf("a");
		 
	    for (int ch = 0; ch < CHANNEL_NUM; ch++) 
		{
        ADC_value6[ch] = Filter_Process(&filters[ch], AD_Value[ch]);
		}
//		ADC_value6[0] = AD_Value[0] ;
//		ADC_value6[1] = AD_Value[1] ;
//		ADC_value6[2] = AD_Value[2] ;
//		ADC_value6[3] = AD_Value[3] ;
//		ADC_value6[4] = AD_Value[4] ;
//		ADC_value6[5] = AD_Value[5] ;
		
		
//		ADC_value6[0] = Filter_Limit(Filter_Median(AD_Value[0]));
//		ADC_value6[1] = Filter_Limit(Filter_Median(AD_Value[1]));
//		ADC_value6[2] = Filter_Limit(Filter_Median(AD_Value[2]));
//		ADC_value6[3] = Filter_Limit(Filter_Median(AD_Value[3]));
//		ADC_value6[4] = Filter_Limit(Filter_Median(AD_Value[4]));
//		ADC_value6[5] = Filter_Limit(Filter_Median(AD_Value[5]));
		
					
			//打包数据并放入发送数组
		packADC6(ADC_value6,NRF24L01_TxPacket);
			
		
			
			
		/*调用NRF24L01_Send函数，发送数据，同时返回发送标志位，方便用户了解发送状态*/
		/*发送标志位与发送状态的对应关系，可以转到此函数定义上方查看*/
		SendFlag = NRF24L01_Send();
			
		if (SendFlag == 1)			//发送标志位为1，表示发送成功
		{
			SendSuccessCount ++;	//发送成功计次变量自增
		}
		else						//发送标志位不为1，即2/3/4，表示发送不成功
		{
			SendFailedCount ++;		//发送失败计次变量自增
		}
			
		OLED_ShowNum(12, 0, SendSuccessCount, 5,OLED_6X8);	//显示发送成功次数
		OLED_ShowNum(48, 0, SendFailedCount, 5,OLED_6X8);		//显示发送失败次数
		OLED_ShowNum(84, 0, SendFlag, 1,OLED_6X8);			//显示最近一次的发送标志位
			
		/*显示发送数据*/
		OLED_ShowHexNum(48, 8, NRF24L01_TxPacket[0], 2,OLED_6X8);
		OLED_ShowHexNum(60, 8, NRF24L01_TxPacket[1], 2,OLED_6X8);
		OLED_ShowHexNum(48, 16, NRF24L01_TxPacket[2], 2,OLED_6X8);
		OLED_ShowHexNum(60, 16, NRF24L01_TxPacket[3], 2,OLED_6X8);
		OLED_ShowHexNum(48, 24, NRF24L01_TxPacket[4], 2,OLED_6X8);
		OLED_ShowHexNum(60, 24, NRF24L01_TxPacket[5], 2,OLED_6X8);
		OLED_ShowHexNum(48, 32, NRF24L01_TxPacket[6], 2,OLED_6X8);
		OLED_ShowHexNum(60, 32, NRF24L01_TxPacket[7], 2,OLED_6X8);
		OLED_ShowHexNum(48, 40, NRF24L01_TxPacket[8], 2,OLED_6X8);
		OLED_ShowHexNum(60, 40, NRF24L01_TxPacket[9], 2,OLED_6X8);
		OLED_ShowHexNum(48, 48, NRF24L01_TxPacket[10], 2,OLED_6X8);
		OLED_ShowHexNum(60, 48, NRF24L01_TxPacket[11], 2,OLED_6X8);		
		
		OLED_ShowNum(102, 8, ADC_value6[0], 4,OLED_6X8);	
		OLED_ShowNum(102, 16, ADC_value6[1], 4,OLED_6X8);	
		OLED_ShowNum(102, 24, ADC_value6[2], 4,OLED_6X8);	
		OLED_ShowNum(102, 32, ADC_value6[3], 4,OLED_6X8);	
		OLED_ShowNum(102, 40, ADC_value6[4], 4,OLED_6X8);	
		OLED_ShowNum(102, 48, ADC_value6[5], 4,OLED_6X8);	

			
		/*TX字符串闪烁一次，表明发送了一次数据*/
		OLED_ShowString(90, 0, "TX",OLED_6X8);
		OLED_Update();
		OLED_ShowString(90, 0, "  ",OLED_6X8);
		OLED_Update();
		
		
//		/*主循环内循环执行NRF24L01_Receive函数，接收数据，同时返回接收标志位，方便用户了解接收状态*/
//		/*接收标志位与接收状态的对应关系，可以转到此函数定义上方查看*/
////		ReceiveFlag = NRF24L01_Receive();
//		
//		if (ReceiveFlag)				//接收标志位不为0，表示收到了一个数据包
//		{
//			if (ReceiveFlag == 1)		//接收标志位为1，表示接收成功
//			{
//				ReceiveSuccessCount ++;	//接收成功计次变量自增
//			}
//			else	//接收标志位不为0也不为1，即2/3，表示此次接收产生了错误，错误接收的数据不应该使用
//			{
//				ReceiveFailedCount ++;	//接收失败计次变量自增
//			}
//			
//			OLED_ShowNum(12, 24, ReceiveSuccessCount, 3,OLED_6X8);	//显示接收成功次数
//			OLED_ShowNum(36, 24, ReceiveFailedCount, 3,OLED_6X8);	//显示接收失败次数
//			OLED_ShowNum(60, 24, ReceiveFlag, 1,OLED_6X8);		//显示最近一次的接收标志位
//			
//			/*显示接收数据*/
//			OLED_ShowHexNum(0, 32, NRF24L01_RxPacket[0], 2,OLED_6X8);
//			OLED_ShowHexNum(12, 32, NRF24L01_RxPacket[1], 2,OLED_6X8);
//			OLED_ShowHexNum(24, 32, NRF24L01_RxPacket[2], 2,OLED_6X8);
//			OLED_ShowHexNum(36, 32, NRF24L01_RxPacket[3], 2,OLED_6X8);
//			
//			/*RX字符串闪烁一次，表明接收到了一次数据*/
//			OLED_ShowString(0, 40, "RX",OLED_6X8);
//			OLED_Update();
////			Delay_ms(100);
//			OLED_ShowString(0, 40, "  ",OLED_6X8);
//			OLED_Update();
//		}
	}
}



void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
	}
}
