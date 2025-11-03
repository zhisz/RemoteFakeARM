#ifndef __FILTER_H
#define __FILTER_H
#include <stdint.h>


#define WINDOW_SIZE   5      // 中值滤波窗口
#define CHANNEL_NUM   6      // ADC通道数，根据需要改

typedef struct {
    uint16_t buf[WINDOW_SIZE];  // 环形缓冲区
    uint8_t  idx;               // 写指针
    uint16_t last_val;          // 限幅滤波的历史值
} Filter_t;

extern Filter_t filters[CHANNEL_NUM];//声明全局数组


// 初始化所有滤波器
void Filter_InitAll(void);

// 单通道中值滤波
uint16_t Filter_Median(Filter_t *f, uint16_t new_val);

// 单通道限幅滤波
uint16_t Filter_Limit(Filter_t *f, uint16_t new_val);

// 综合滤波（先中值后限幅）
uint16_t Filter_Process(Filter_t *f, uint16_t new_val);



#endif


