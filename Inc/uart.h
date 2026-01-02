/*
 * uart.h
 *
 *  Created on: Jan 1, 2026
 *      Author: sowji
 */
#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char *s);
void uart_send_dec(uint16_t v);

#endif

