/*
 * uart.c
 *
 *  Created on: Jan 1, 2026
 *      Author: sowji
 */

#include "uart.h"

/* ================= USART2 ================= */
#define PERIPH_BASE     0x40000000UL
#define APB1_BASE       (PERIPH_BASE + 0x00000)
#define GPIOA_BASE      (PERIPH_BASE + 0x20000)

#define RCC_BASE        (PERIPH_BASE + 0x20000 + 0x3800)
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define USART2_BASE     (APB1_BASE + 0x4400)
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR       (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0C))

void uart_init(void)
{
    RCC_APB1ENR |= (1 << 17); // Enable USART2 clock
    RCC_AHB1ENR |= (1 << 0);  // Enable GPIOA clock

    // PA2 -> USART2_TX
    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |=  (2 << 4); // AF
    GPIOA_AFRL  |=  (7 << 8); // AF7

    USART2_BRR = 0x8B;        // 115200 @ 16 MHz
    USART2_CR1 |= (1 << 3);   // TE
    USART2_CR1 |= (1 << 13);  // UE
}

void uart_send_char(char c)
{
    while (!(USART2_SR & (1 << 7)));
    USART2_DR = c;
}

void uart_send_string(const char *s)
{
    while (*s) uart_send_char(*s++);
}

void uart_send_dec(uint16_t v)
{
    char buf[5];
    int i = 0;

    if (v == 0)
    {
        uart_send_char('0');
        return;
    }

    while (v)
    {
        buf[i++] = (v % 10) + '0';
        v /= 10;
    }
    while (i--) uart_send_char(buf[i]);
}
