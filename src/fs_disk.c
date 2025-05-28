#include <stdint.h>
#include "fs_disk.h"
#include "memory.h"
#include "filesystem.h"
#include "ide.h"  // for ide_read_sector
#include "kheap.h"
#include "vga.h"

#define TOTAL_SECTORS 128  // Total sectors in the disk (for simplicity)
#define SECTOR_SIZE 512    // Size of each sector in bytes
#define MAX_FILES 128      // Maximum number of files in the disk
typedef struct  {
    char     signature[20];       // e.g., "BATFS1.0"
    uint32_t total_sectors;      // total number of sectors in use
    uint32_t fat_start;          // sector where FAT begins
    uint32_t fat_sectors;        // number of sectors used by FAT
    uint32_t data_start;         // first sector for file data
}DiskHeader;
DiskHeader disk_header;  // Global disk header
int file_count = 0;

static void fs_init_disk_header()
{
    print("fs_init_disk_header\n");
    memcpy(disk_header.signature, "BATFS1.0", 8);
    disk_header.total_sectors = TOTAL_SECTORS;  // To be set later
    disk_header.fat_start = 1;       // FAT starts at sector 1
    disk_header.fat_sectors = 16;     // FAT occupies 1 sector
    disk_header.data_start = 17;      // Data starts at sector 2
}
static void fs_write_disk_header() 
{
    uint8_t buffer[SECTOR_SIZE];
    memset(buffer, 0, SECTOR_SIZE);
    // Copy the disk header into the buffer
    memcpy(buffer, &disk_header, sizeof(DiskHeader));
    // Write the disk header to sector 0
    ide_write_sector(0, buffer);
}

static void fs_write_initial_files() 
{
    uint32_t id = 0;
    uint32_t current_byte = 0;
    // Write a fake file table to sector 1
    uint8_t buffer[SECTOR_SIZE];
    memset(buffer, 0, SECTOR_SIZE);
    buffer[0] = 3; // only one directory
    // root directory;
    current_byte = current_byte + 1;
    strcpy((char*)&buffer[current_byte], "/");
    current_byte += SIZE_NAME;
    *(uint32_t*)&buffer[current_byte] = 0;        // size is 0 it is a directory
    current_byte += SIZE_SIZE;
    *(uint32_t*)&buffer[current_byte] = 0;        // starts at sector 10
    current_byte += SIZE_DISK_SECTOR;
    *(uint32_t*)&buffer[current_byte] = id++ ; //id 
    current_byte += SIZE_UID;
    *(uint32_t*)&buffer[current_byte] = 0; //no parent, it is the root directory
    current_byte += SIZE_PID;
    *(uint32_t*)&buffer[current_byte] = 1; //no of children 
    current_byte += SIZE_CHILDCOUNT;
    *(uint32_t*)&buffer[current_byte] = id; //children id
    current_byte += SIZE_CHILDID;
    current_byte += SIZE_CONTENT;

    // Write the file table to sector 1
    //ide_write_sector(1, buffer);
    //memset(buffer, 0, SECTOR_SIZE);

    strcpy((char*)&buffer[current_byte], "bin");
    current_byte += SIZE_NAME;
    *(uint32_t*)&buffer[current_byte] = 0;        // size is 0 it is a directory
    current_byte += SIZE_SIZE;
    *(uint32_t*)&buffer[current_byte] = 0;        // starts at sector 10
    current_byte += SIZE_DISK_SECTOR;
    *(uint32_t*)&buffer[current_byte] = id++; //id 
    current_byte += SIZE_UID;
    *(uint32_t*)&buffer[current_byte] = 0; // parent, it is the root directory
    current_byte += SIZE_PID;
    *(uint32_t*)&buffer[current_byte] = 1;  //child
    current_byte += SIZE_CHILDCOUNT;
    *(uint32_t*)&buffer[current_byte] = id;  //child
    current_byte += SIZE_CHILDID;
    current_byte += SIZE_CONTENT; 

    //ide_write_sector(1, buffer);
    //memset(buffer, 0, SECTOR_SIZE);

    strcpy((char*)&buffer[current_byte], "README.TXT");
    current_byte += SIZE_NAME;
    *(uint32_t*)&buffer[current_byte] = 128;        // size is 128 bytes
    current_byte += SIZE_SIZE;
    *(uint32_t*)&buffer[current_byte] = 17;        // size is 0 it is a directory
    current_byte += SIZE_DISK_SECTOR;
    *(uint32_t*)&buffer[current_byte] = id++;        // starts at sector 10
    current_byte += SIZE_UID;
    *(uint32_t*)&buffer[current_byte] = 1; //id 
    current_byte += SIZE_PID;
    *(uint32_t*)&buffer[current_byte] = 0;  //child
    current_byte += SIZE_CHILDCOUNT;
    *(uint32_t*)&buffer[current_byte] = 0; //child id 
    current_byte += SIZE_CHILDID;
    current_byte += SIZE_CONTENT;

    ide_write_sector(1, buffer);
    memset(buffer, 0, SECTOR_SIZE);

    strcpy((char*)&buffer[0], "If this file can be read, the disk is working!");
    ide_write_sector(17, buffer);

}
static void build_file_tree()
{
    FileSystem* fs = fs_get();
    memset(fs->file_nodes, 0, sizeof(fs->file_nodes));
    uint32_t file_count = 0;
    uint32_t max_uid = 0;
    uint8_t buffer[SECTOR_SIZE];
    for (uint8_t i = 1; i < 17; i++)
    {
        ide_read_sector((uint32_t)i, buffer);  // Load file table from sector 1
        uint32_t file_count_in_sector = buffer[0];  // First byte: number of files
        uint32_t offset = 1;
        uint32_t child_count = 0;
        for (int j = 0; j < file_count_in_sector; j++) {
            Node* file = &fs->file_nodes[file_count++];
            // Copy filename
            memcpy(file->name, &buffer[offset], 32);
            offset += 32;
            // Read size (4 bytes)
            file->size = *(uint32_t*)&buffer[offset];
            offset += 4;
            // Read sector (4 bytes)
            file->disk_sector = *(uint32_t*)&buffer[offset];
            offset += 4;
            // Read unique ID (4 bytes)
            file->unique_id = *(uint32_t*)&buffer[offset];
            offset += 4;
            if( max_uid < file->unique_id) {
                max_uid = file->unique_id;
            }
            // Read parent ID (4 bytes)
            file->parent_id = *(uint32_t*)&buffer[offset];
            offset += 4;
            // Read child count  (4 bytes)
            file->child_count = *(uint32_t*)&buffer[offset];

            offset += 4;
            uint32_t end_offset = offset + 16 * 4; // Maximum children per directory
            // Read child IDs (8 bytes each, up to 8 children)
            for (int k = 0; k < file->child_count; k++) {
                file->children_id[k] = *(uint32_t*)&buffer[offset];
                offset += 4;
            }
            offset = end_offset; // Move to the next file entry
            offset += 4; 
            //fs_display_node(file); // Display the file node
        }
            
    }
    fs->uid = max_uid + 1; // Set the next unique ID
    fs->current_dir = 0; // Set the current directory to root
    print("succesfuly built file tree.\n");

    
}
void fs_disk_init() 
{
    fs_init_disk_header();
    if (memcmp(disk_header.signature, "BATFS1.0", 8) != 0) {
        fs_write_disk_header();
        fs_write_initial_files();
        print("Invalid disk format.");
        return;
    }
    print("successfully mounted disk.\n");
    build_file_tree();
}
void fs_mount_disk(Node* mount_point) {
    fs_disk_init();  // You implement this to populate disk_files[] and file_count
    for (int i = 0; i < file_count; i++) {
        Node* file = &fs_get()->file_nodes[i];
        if (file->parent_id == mount_point->unique_id) {
            // Add the file to the mount point's children
            mount_point->children_id[mount_point->child_count++] = file->unique_id;
        }
    }
}