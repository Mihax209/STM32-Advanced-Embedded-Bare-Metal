#include <stm32f411xe.h>

__attribute__((noreturn, optimize("O0")))
int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER |=  GPIO_MODER_MODE13_0;
    GPIOC->MODER &= ~GPIO_MODER_MODE13_1;

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_OD13;
        for (int i = 0; i < 100000; i++) { } 
    }
}
