#include "ports.h"
#include "vga.h"
#include <stdint.h>

#define ATA_PRIMARY_IO  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_MASTER         0xE0
#define ATA_CMD_WRITE_PIO  0x30
#define ATA_STATUS_BSY     0x80
#define ATA_STATUS_DRQ     0x08


void ide_wait() {
    while (inb(ATA_PRIMARY_IO + 7) & 0x80); // Wait for BSY to clear
    while (!(inb(ATA_PRIMARY_IO + 7) & 0x08)); // Wait for DRQ set
}

void ide_read_sector(uint32_t lba, uint8_t* buffer) {
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + 2, 1); // Number of sectors
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba & 0xFF));
    outb(ATA_PRIMARY_IO + 4, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO + 5, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_PRIMARY_IO + 7, 0x20); // READ SECTOR command

    ide_wait();

    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_PRIMARY_IO);
        buffer[i*2] = data & 0xFF;
        buffer[i*2 + 1] = (data >> 8) & 0xFF;
    }
}
static void ide_wait_bsy() {
    while (inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_BSY);
}

static void ide_wait_drq() {
    while (!(inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_DRQ));
}

void ide_write_sector(uint32_t lba, const uint8_t* buffer) {
    ide_wait_bsy();

    outb(ATA_PRIMARY_IO + 6, (ATA_MASTER | ((lba >> 24) & 0x0F)));
    outb(ATA_PRIMARY_IO + 2, 1);                       // sector count
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba & 0xFF));   // LBA low byte
    outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));     // LBA mid byte
    outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));    // LBA high byte
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_WRITE_PIO);       // send write command

    ide_wait_drq();

    // Write 512 bytes (256 words)
    for (int i = 0; i < 256; i++) {
        uint16_t word = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
        outw(ATA_PRIMARY_IO, word);
    }

    // Flush cache
    outb(ATA_PRIMARY_IO + 7, 0xE7);  // FLUSH CACHE
    ide_wait_bsy();
}