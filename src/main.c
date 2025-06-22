#include <stdio.h>
#include <string.h>
#include "common.h"
#include "uart.h"
#include "gpio_key.h"
#include "dma.h"

void init(void) {
    UART_init();
    ADC_init();

    printf("\nINIT COMPLETE\n");
}

__attribute__((optimize("O0")))
int main() {
    init();

    while (1) { 
        ADC_print_values();
        for (int i = 0; i < 0xB0000; ++i) { }
    }
}
