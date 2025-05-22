#pragma once
#include <stdint.h>
#include <string.h>
#include "kheap.h"
typedef enum { FILE_NODE, DIR_NODE } NodeType;

typedef struct Node {
    char name[32];
    NodeType type;
    char* content;                  // For files
    struct Node* parent;
    struct Node* children[16];     // For directories
    int child_count;
} Node;
typedef struct FileSystem {
    Node* root;
    Node* current_dir;
} FileSystem;
void init_fs(); 
FileSystem* fs_get();
void fs_get_current_path(char* buffer);