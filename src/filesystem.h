#pragma once
#include <stdint.h>
#include "kheap.h"
#define MAX_FILES 128

#define SIZE_NAME 32
#define SIZE_SIZE 4
#define SIZE_DISK_SECTOR 4
#define SIZE_UID 4
#define SIZE_PID 4
#define SIZE_CHILDID 64
#define SIZE_CHILDCOUNT 4
#define SIZE_CONTENT 4
typedef struct Node {
    char name[32];
    uint32_t size;                 // Size in bytes
    uint32_t disk_sector;          // LBA starting sector

    uint32_t unique_id;             // Unique identifier for the file
    uint32_t parent_id;
    uint32_t child_count;
    uint32_t children_id[16];     // For directories

    // ⬇️ New fields for disk-backed files
   
    char* content;                  // For memory files (optional for disk files)
} Node;
typedef struct FileSystem {
    Node file_nodes[MAX_FILES];
    uint32_t file_count;           // Total number of files
    uint32_t current_dir; //index 
    uint32_t uid; //unique id; 
} FileSystem;
void fs_init(); 
FileSystem* fs_get();
void fs_get_current_path(char* buffer);
Node* fs_create_file(const char* name, uint32_t size, uint32_t disk_sector);
void fs_display_node(const Node* node);