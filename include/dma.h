#pragma once

#include "common.h"

void DMA_init(void);
void DMA_transfer_start(uint16_t *src_buf, uint16_t *dst_buf, uint32_t len);
bool DMA_is_transfer_complete(void);
