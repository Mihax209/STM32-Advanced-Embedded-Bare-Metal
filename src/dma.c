#include "dma.h"

volatile bool g_transfer_complete;

void DMA_init(void) {
    /* Enable clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    /* Disable DMA stream */
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;

    /* Wait for stream to be disabled */
    while (DMA2_Stream0->CR & DMA_SxCR_EN) { }

    /* Configure DMA parameters */
    /* 16 bit memory size */
    DMA2_Stream0->CR &= ~DMA_SxCR_MSIZE_Msk;
    DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;

    /* PSIZE 16 bit */
    DMA2_Stream0->CR &= ~DMA_SxCR_PSIZE_Msk;
    DMA2_Stream0->CR |= DMA_SxCR_PSIZE_1;

    /* Pointer INC */
    DMA2_Stream0->CR |= DMA_SxCR_PINC;
    DMA2_Stream0->CR |= DMA_SxCR_MINC;

    /* Configure memory-to-memory */
    DMA2_Stream0->CR &= ~DMA_SxCR_DIR_Msk;
    DMA2_Stream0->CR |= DMA_SxCR_DIR_1;

    /* Enable interrupts */
    DMA2_Stream0->CR |= DMA_SxCR_TCIE; // Transfer Complete
    DMA2_Stream0->CR |= DMA_SxCR_TEIE; // Transfer Error

    /* Disable direct mode */
    DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS; // 1 is disabled

    /* Set DMA FIFO threshold */
    DMA2_Stream0->FCR &= ~DMA_SxFCR_FTH_Msk;
    DMA2_Stream0->FCR |= DMA_SxFCR_FTH_1 | DMA_SxFCR_FTH_0;

    /* Enable DMA interrupt in NVIC */
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA_transfer_start(uint16_t *src_buf, uint16_t *dst_buf, uint32_t len) {
    g_transfer_complete = false;

    /* Set peripheral address */
    DMA2_Stream0->PAR = (uint32_t)src_buf;

    /* Set memory address */
    DMA2_Stream0->M0AR = (uint32_t)dst_buf;

    /* Set transfer length */
    DMA2_Stream0->NDTR = len;

    /* Enable dma stream */
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}

bool DMA_is_transfer_complete(void) {
    return g_transfer_complete;
}

void DMA2_Stream0_IRQHandler_yeet(void) {
    if (DMA2->LISR & DMA_LISR_TCIF0_Msk) {
        g_transfer_complete = true;
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
    }

    /* Check if error, and clear it */
    if (DMA2->LISR & DMA_LISR_TEIF0_Msk) {
        DMA2->LIFCR |= DMA_LIFCR_CTEIF0;
    }
}
