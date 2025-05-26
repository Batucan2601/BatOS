#pragma once
#include <stdint.h>
#include "kheap.h"
typedef enum { FILE_NODE, DIR_NODE } NodeType;

typedef struct Node {
    char name[32];
    NodeType type;
    char* content;                  // For memory files (optional for disk files)
    struct Node* parent;
    struct Node* children[16];     // For directories
    uint32_t child_count;

    // ⬇️ New fields for disk-backed files
    uint32_t is_disk_backed;            // 1 if file is stored on real disk
    uint32_t disk_sector;          // LBA starting sector
    uint32_t size;                 // Size in bytes
} Node;
typedef struct FileSystem {
    Node* root;
    Node* current_dir;
} FileSystem;
void fs_init(); 
FileSystem* fs_get();
void fs_get_current_path(char* buffer);
Node* fs_create_file(const char* name, uint32_t size, uint32_t disk_sector);