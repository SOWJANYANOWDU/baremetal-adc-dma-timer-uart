/*
 * adc_dma.h
 *
 *  Created on: Jan 2, 2026
 *      Author: sowji
 */

#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdint.h>

#define ADC_BUF_LEN 1
extern volatile uint16_t adc_buffer[ADC_BUF_LEN];

void adc_dma_init(void);

#endif
