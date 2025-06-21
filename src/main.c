#include <stdio.h>
#include <string.h>
#include "common.h"
#include "uart.h"
#include "gpio_key.h"

__attribute__((optimize("O0")))
int main() {
    UART_init();
    KEY_init();

    for (uint32_t i = 0; i < UINT32_MAX; ++i) {
        printf("(%lu) Button value: %d\n", i, KEY_read_value());
    }
}
