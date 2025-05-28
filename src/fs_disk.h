#pragma once
struct DiskHeader {
    char     signature[20];       // e.g., "BATFS1.0"
    uint32_t total_sectors;      // total number of sectors in use
    uint32_t fat_start;          // sector where FAT begins
    uint32_t fat_sectors;        // number of sectors used by FAT
    uint32_t data_start;         // first sector for file data
};

void fs_disk_init();
void write_fake_disk_table();