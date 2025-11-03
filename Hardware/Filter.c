//////由于adc在获取电位器数据时会有微小抖动（10左右），为减缓或消除这一现象，加入滤波函数缓解
#include "stm32f10x.h"                  // Device header
#include "filter.h"




// 全局滤波器数组，每个通道一个
Filter_t filters[CHANNEL_NUM];//参数：通道数


//初始化，全部通道成员置零，程序开始前调用
void Filter_InitAll(void) {
    for (int ch = 0; ch < CHANNEL_NUM; ch++) {
        filters[ch].idx = 0;
        filters[ch].last_val = 0;
        for (int i = 0; i < WINDOW_SIZE; i++) {
            filters[ch].buf[i] = 0;
        }
    }
}

// 中值滤波，取规定数量的数排列取中值作为有效值输出
uint16_t Filter_Median(Filter_t *f, uint16_t new_val) {
    f->buf[f->idx++] = new_val;
    if (f->idx >= WINDOW_SIZE) f->idx = 0;

    // 拷贝临时数组
    uint16_t tmp[WINDOW_SIZE];
    for (int i = 0; i < WINDOW_SIZE; i++) tmp[i] = f->buf[i];

    // 冒泡排序
    for (int i = 0; i < WINDOW_SIZE - 1; i++) {
        for (int j = i + 1; j < WINDOW_SIZE; j++) {
            if (tmp[i] > tmp[j]) {
                uint16_t t = tmp[i]; tmp[i] = tmp[j]; tmp[j] = t;
            }
        }
    }
    return tmp[WINDOW_SIZE / 2];
}



// 限幅滤波，如果前后数据差小于设定值，舍弃最新的数据
uint16_t Filter_Limit(Filter_t *f, uint16_t new_val) {
    const uint16_t limit = 4; // 阈值，可调
    if ((new_val > f->last_val && new_val - f->last_val < limit) ||
        (f->last_val > new_val && f->last_val - new_val < limit)) {
        return f->last_val;
    } else {
        f->last_val = new_val;
        return new_val;
    }
}

// 综合滤波：先中值，再限幅
uint16_t Filter_Process(Filter_t *f, uint16_t new_val) {
    uint16_t mid = Filter_Median(f, new_val);
    return Filter_Limit(f, mid);
}








