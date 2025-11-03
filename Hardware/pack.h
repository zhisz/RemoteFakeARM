#ifndef __PACK_H
#define __PACK_H
#include <stdint.h>



void packADC(uint16_t value, uint8_t *buf);
uint16_t unpackADC(uint8_t *buf);

void packADC6(uint16_t adc_vals[6], uint8_t *buf);
void unpackADC6(uint8_t *buf, uint16_t adc_vals[6]);




#endif

