#include <stdio.h>
#include "adc_dma.h"

/**
 * We'll use PA1/PA2 as the ADC input
 */

#define NUM_OF_CHANNELS (2)
uint16_t g_raw_adc_data[NUM_OF_CHANNELS];

void ADC_init(void) {
    /***************** Configure GPIO *****************/
    /* Enable clock to GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA1/PA2 as analog in */
    GPIOA->MODER |= GPIO_MODER_MODE1_0 | GPIO_MODER_MODE1_1;
    GPIOA->MODER |= GPIO_MODER_MODE2_0 | GPIO_MODER_MODE2_1;

    /***************** Configure ADC *****************/
    /* Enable Clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* Set sequence length */
    ADC1->SQR1 &= ~ADC_SQR1_L_Msk;
    ADC1->SQR1 |= ADC_SQR1_L_0; // 2 conversions

    /* Set sequence */
    // 1st - Channel 1 (PA1)
    ADC1->SQR3 &= ~ADC_SQR3_SQ1_Msk;
    ADC1->SQR3 |= ADC_SQR3_SQ1_0;
    // 2nd - Channel 2 (PA2)
    ADC1->SQR3 &= ~ADC_SQR3_SQ2_Msk;
    ADC1->SQR3 |= ADC_SQR3_SQ2_1;

    /* Enable scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;

    /* Select use DMA */
    ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_DMA | ADC_CR2_DDS;

    /***************** Configure DMA *****************/
    /* Enable DMA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    /* DMA2-Stream4 is the ADC1*/
    /* Disable DMA stream */
    DMA2_Stream4->CR &= ~DMA_SxCR_EN;

    /* Wait for stream to be disabled */
    while (DMA2_Stream4->CR & DMA_SxCR_EN) { }

    /* Circular mode */
    DMA2_Stream4->CR |= DMA_SxCR_CIRC;

    /* Configure DMA parameters */
    /* 16 bit memory size */
    DMA2_Stream4->CR &= ~DMA_SxCR_MSIZE_Msk;
    DMA2_Stream4->CR |= DMA_SxCR_MSIZE_0;

    /* PSIZE 16 bit */
    DMA2_Stream4->CR &= ~DMA_SxCR_PSIZE_Msk;
    DMA2_Stream4->CR |= DMA_SxCR_PSIZE_0;

    /* Pointer INC */
    DMA2_Stream4->CR |= DMA_SxCR_MINC;

    /* Configure peripheral-to-memory */
    DMA2_Stream4->CR &= ~DMA_SxCR_DIR_Msk;
    DMA2_Stream4->PAR = (uint32_t)(&(ADC1->DR));
    DMA2_Stream4->M0AR = (uint32_t)(&g_raw_adc_data);

    /* Configure number of data items */
    DMA2_Stream4->NDTR = (uint16_t)NUM_OF_CHANNELS;

    /* Enable dma stream */
    DMA2_Stream4->CR |= DMA_SxCR_EN;

    /***************** Start ADC *****************/
    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Start ADC */
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

void ADC_print_values(void) {
    for (int i = 0; i < NUM_OF_CHANNELS; ++i) {
        printf("%d, ", g_raw_adc_data[i]);
    }
    printf("\n");
}
