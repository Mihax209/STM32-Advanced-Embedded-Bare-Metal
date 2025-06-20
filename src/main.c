#include <stm32f411xe.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"

__attribute__((optimize("O0")))
int main() {
    UART_init();

    for (uint32_t i = 0; i < UINT32_MAX; ++i) {
        printf("Testing...%lu\n", i);
    }
}
