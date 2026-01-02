/*
 * adc_dma.c
 *
 *  Created on: Jan 2, 2026
 *      Author: sowji
 */




#include "adc_dma.h"

/* ================= BASE ================= */
#define PERIPH_BASE     0x40000000UL
#define AHB1_BASE       (PERIPH_BASE + 0x20000)
#define APB2_BASE       (PERIPH_BASE + 0x10000)
#define APB1_BASE       (PERIPH_BASE + 0x00000)

/* ================= RCC ================= */
#define RCC_BASE        (AHB1_BASE + 0x3800)
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))

/* ================= GPIOA ================= */
#define GPIOA_BASE      (AHB1_BASE + 0x0000)
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

/* ================= TIM2 ================= */
#define TIM2_BASE       (APB1_BASE + 0x0000)
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_CR2        (*(volatile uint32_t *)(TIM2_BASE + 0x04))

/* ================= ADC1 ================= */
#define ADC1_BASE       (APB2_BASE + 0x2000)
#define ADC1_CR2        (*(volatile uint32_t *)(ADC1_BASE + 0x08))
#define ADC1_SMPR2      (*(volatile uint32_t *)(ADC1_BASE + 0x10))
#define ADC1_SQR1       (*(volatile uint32_t *)(ADC1_BASE + 0x2C))
#define ADC1_SQR3       (*(volatile uint32_t *)(ADC1_BASE + 0x34))
#define ADC1_DR         (*(volatile uint32_t *)(ADC1_BASE + 0x4C))

/* ================= DMA2 Stream0 ================= */
#define DMA2_BASE       (AHB1_BASE + 0x6400)
#define DMA2_S0CR       (*(volatile uint32_t *)(DMA2_BASE + 0x10))
#define DMA2_S0NDTR     (*(volatile uint32_t *)(DMA2_BASE + 0x14))
#define DMA2_S0PAR      (*(volatile uint32_t *)(DMA2_BASE + 0x18))
#define DMA2_S0M0AR     (*(volatile uint32_t *)(DMA2_BASE + 0x1C))
#define DMA2_LIFCR      (*(volatile uint32_t *)(DMA2_BASE + 0x08))


#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))  // <- ADD THIS
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))

volatile uint16_t adc_buffer[ADC_BUF_LEN];

void adc_dma_init(void)
{
    /* ---------- CLOCK ENABLE ---------- */
    RCC_AHB1ENR |= (1 << 0);   // GPIOA
    RCC_AHB1ENR |= (1 << 22);  // DMA2
    RCC_APB1ENR |= (1 << 0);   // TIM2
    RCC_APB2ENR |= (1 << 8);   // ADC1

    /* ---------- GPIO ---------- */
    GPIOA_MODER &= ~(3 << 0);
    GPIOA_MODER |=  (3 << 0); // PA0 analog

    /* ---------- TIM2 (100 Hz TRGO) ---------- */
    TIM2_PSC = 15999;
    TIM2_ARR = 9;
    TIM2_CR2 &= ~(7 << 4);     // CLEAR MMS
    TIM2_CR2 |=  (2 << 4);     // TRGO = update
    TIM2_CR1 |= 1;             // CEN

    /* ---------- DMA ---------- */
    DMA2_S0CR &= ~1;
    while (DMA2_S0CR & 1);

    DMA2_LIFCR = 0x3D;

    DMA2_S0PAR  = (uint32_t)&ADC1_DR;
    DMA2_S0M0AR = (uint32_t)adc_buffer;
    DMA2_S0NDTR = ADC_BUF_LEN;

    DMA2_S0CR &= ~(7 << 25);   // Channel 0 (ADC1)
    DMA2_S0CR &= ~(3 << 11);
    DMA2_S0CR |=  (1 << 11);   // PSIZE = 16-bit
    DMA2_S0CR &= ~(3 << 13);
    DMA2_S0CR |=  (1 << 13);   // MSIZE = 16-bit
    DMA2_S0CR |= (1 << 10);    // MINC
    DMA2_S0CR |= (1 << 8);     // CIRCULAR
    DMA2_S0CR |= 1;            // ENABLE

    /* ---------- ADC ---------- */
    ADC1_SMPR2 |= (7 << 0);    // Max sample time CH0
    ADC1_SQR1 = 0;
    ADC1_SQR3 = 0;             // Channel 0 (PA0)
    ADC1_CR2 |= (1 << 8);      // DMA
    ADC1_CR2 |= (1 << 9);      // DDS
    ADC1_CR2 &= ~(0xF << 24);
    ADC1_CR2 |=  (6 << 24);    // TIM2 TRGO
    ADC1_CR2 &= ~(3 << 28);
    ADC1_CR2 |=  (1 << 28);    // Rising edge
    ADC1_CR2 |= 1;             // ADC ON
    for (volatile int i = 0; i < 1000; i++);
    ADC1_CR2 |= (1 << 30);     // ONE dummy SWSTART
}
