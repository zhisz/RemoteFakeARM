///此模块主要用于数据的打包 和 解包
///由于通信数据只能单次发送一个字节，而adc转换得到的数据大于一个字节
///故将数据打包成两个字节数据发出
#include "stm32f10x.h"                  // Device header
#include "pack.h"



// 打包一个 0-4095 的数到两个 uint8_t
void packADC(uint16_t value, uint8_t *buf)
{
    buf[0] = (value >> 8) & 0xFF; // 高 8 位
    buf[1] = value & 0xFF;        // 低 8 位
	
	
	
}

//解包一个adc数据

uint16_t unpackADC(uint8_t *buf)
{
    return ((uint16_t)buf[0] << 8) | buf[1];
}





// adc_vals: 6 个 uint16_t 的数组
// buf: 至少长度 12 的 uint8_t 数组
void packADC6(uint16_t adc_vals[6], uint8_t *buf)
{
	
	adc_vals[2] += adc_vals[1];
    for (int i = 0; i < 6; i++) {
        buf[2*i]     = (adc_vals[i] >> 8) & 0xFF; // 高字节
        buf[2*i + 1] = adc_vals[i] & 0xFF;        // 低字节
    }
	
}

//解包，解出含六个adc数值的数组
void unpackADC6(uint8_t *buf, uint16_t adc_vals[6])
{
    for (int i = 0; i < 6; i++) {
        adc_vals[i] = ((uint16_t)buf[2*i] << 8) | buf[2*i + 1];
    }
}







