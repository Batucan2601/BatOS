#pragma once
#include <stdint.h>
void ide_wait();
void ide_read_sector(uint32_t lba, uint8_t* buffer);
void ide_write_sector(uint32_t lba, uint8_t* buffer);