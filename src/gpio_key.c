#include "gpio_key.h"

/**
 * PA0 is the key GPIO
 */

void KEY_init(void) {
    /* Enable clock to GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA0 to input */
    GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
}

bool KEY_read_value(void) {
    return (GPIOA->IDR & GPIO_IDR_ID0_Msk);
}
