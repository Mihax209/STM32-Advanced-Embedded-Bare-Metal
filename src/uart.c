#include "uart.h"
#include <stddef.h>

#define CLK             (16000000u)
#define UART_BUADRATE   (115200)

static void configure_uart_port_pins(void);
static void configure_uart_module(void);
void UART_init(void);

int _write(int file, char *ptr, int len) {
    for (size_t i = 0; i < len; ++i) {
        UART_send_byte(ptr[i]);
    }
    return len;
}

uint16_t compute_uart_baudrate(uint32_t periph_clock, uint32_t baudrate) {
    return ((periph_clock + (baudrate/2u)) / baudrate);
}

static void configure_uart_port_pins(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA11/PA12 to alternate function mode */
    GPIOA->MODER &= (~GPIO_MODER_MODER11_Msk);
    GPIOA->MODER |= GPIO_MODER_MODER11_1;
    
    GPIOA->MODER &= (~GPIO_MODER_MODER12_Msk);
    GPIOA->MODER |= GPIO_MODER_MODER12_1;

    /* Set Alternate function */
    /* Set GPIOA_AFRH11 & GPIOA_AFRH12 = 8 (USART 6) */
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL11_3; // PA11 - TX
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL12_3; // PA12 - RX
}

static void configure_uart_module(void) {
    /* Enable clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

    /* Set Buadrate */
    USART6->BRR = compute_uart_baudrate(CLK, UART_BUADRATE);

    /* Set Transfer direction */
    USART6->CR1 |= USART_CR1_TE;
    USART6->CR1 |= USART_CR1_RE;

    /* Enable UART */
    USART6->CR1 |= USART_CR1_UE;
}

void UART_init() {
    configure_uart_port_pins();
    configure_uart_module();
}

void UART_send_byte(int ch) {
    /* Check data register is clear (while bit is 0) */
    while (!(USART6->SR & USART_SR_TXE_Msk)) { }

    USART6->DR = ch & 0xff;
}
