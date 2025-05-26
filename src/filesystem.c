#include "filesystem.h"
#include "kheap.h"
#include "ide.h"
#include "memory.h"
#include <stdint.h>
#include "stringcustom.h"
FileSystem* FS;
void fs_init() {
    FS->root = (Node*)(sizeof(Node));
    strcpy(FS->root->name, "/");
    FS->root->type = DIR_NODE;
    FS->root->parent = NULL;
    FS->root->child_count = 0;
    FS->current_dir = FS->root;
}

FileSystem* fs_get()
{
    return FS;
}

static void fs_get_full_path(char* buffer, Node* node)
{
    if (node->parent != NULL) {
        fs_get_full_path(buffer, node->parent);
    }
    strcat(buffer, "/");
    strcat(buffer, node->name);
}
void fs_get_current_path(char* buffer)
{
    fs_get_full_path(buffer, FS->current_dir);
}


char* fs_read_file(Node* node) {
    if (node->is_disk_backed) {
        char* buf = kmalloc(node->size);
        //ide_read_sectors(node->disk_sector, buf, node->size / 512);
        return buf;
    } else {
        return node->content;
    }
}

Node* fs_create_file(const char* name, uint32_t size, uint32_t disk_sector) 
{
    Node* file = (Node*)kmalloc(sizeof(Node));
    strcpy(file->name, name);
    file->type = FILE_NODE;
    file->is_disk_backed = 1; // Set to 1 for disk-backed files
    file->disk_sector = disk_sector;
    file->size = size;
    file->content = NULL; // No content for disk-backed files
    return file;
}