#include "fs_disk.h"
#include "memory.h"
#include "filesystem.h"
#include <stdint.h>
#include "ide.h"  // for ide_read_sector
#include "kheap.h"
typedef struct {
    char name[32];
    uint32_t size;
    uint32_t sector;
} DiskFile;

DiskFile disk_files[16];
int file_count = 0;

void fs_disk_init() 
{
    uint8_t buffer[512];
    ide_read_sector((uint32_t)1, buffer);  // Load file table from sector 1

    file_count = buffer[0];  // First byte: number of files

    uint32_t offset = 1;
    for (int i = 0; i < file_count; i++) {
        DiskFile* file = &disk_files[i];

        // Copy filename
        memcpy(file->name, &buffer[offset], 32);
        offset += 32;

        // Read size (4 bytes)
        file->size = *(uint32_t*)&buffer[offset];
        offset += 4;

        // Read sector (4 bytes)
        file->sector = *(uint32_t*)&buffer[offset];
        offset += 4;
    }
}

void write_fake_disk_table() {
    uint8_t buffer[512];
    memset(buffer, 0, 512);

    buffer[0] = 2; // 2 files

    // File 1
    strcpy((char*)&buffer[1], "readme.txt");
    *(uint32_t*)&buffer[33] = 64;        // size
    *(uint32_t*)&buffer[37] = 10;        // starts at sector 10

    // File 2
    strcpy((char*)&buffer[41], "log.txt");
    *(uint32_t*)&buffer[73] = 128;       // size
    *(uint32_t*)&buffer[77] = 12;        // starts at sector 12

    ide_write_sector(1, buffer);
}

void fs_mount_disk(Node* mount_point) {
    fs_disk_init();  // You implement this to populate disk_files[] and file_count
    for (int i = 0; i < file_count; i++) {
        Node* file = fs_create_file(disk_files[i].name, disk_files[i].size, disk_files[i].sector);
        file->parent = mount_point;
        mount_point->children[mount_point->child_count++] = file;
    }
}