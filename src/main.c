#include <stdio.h>
#include <string.h>
#include "common.h"
#include "uart.h"
#include "gpio_key.h"
#include "dma.h"

#define BUFFER_SIZE (5)

uint16_t data_arr[BUFFER_SIZE] = {69, 420, 2352, 3423, 3423};
uint16_t temp_data_arr[BUFFER_SIZE];

void init(void) {
    UART_init();
    KEY_init();
    DMA_init();

    printf("\nINIT COMPLETE\n");
}

__attribute__((noinline))
volatile void print_temp_data_arr() {
    printf("Array data: ");
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        printf("%d, ", temp_data_arr[i]);
    }
    printf("\n");
}

__attribute__((optimize("O0")))
int main() {
    init();

    print_temp_data_arr();
    printf("Starting DMA transfer\n");
    DMA_transfer_start(data_arr, temp_data_arr, BUFFER_SIZE);
    while (!DMA_is_transfer_complete()) { }
    print_temp_data_arr();

    while (1) { }
}
