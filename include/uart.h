#pragma once
#include <stdint.h>
#include <stm32f411xe.h>

void UART_init();
void UART_send_byte(int ch);
