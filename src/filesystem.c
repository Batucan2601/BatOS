#include "filesystem.h"
#include "kheap.h"
#include "ide.h"
#include "memory.h"
#include <stdint.h>
#include "stringcustom.h"
#include "vga.h"
FileSystem FS;
void fs_init() {
    print("fs_init\n");
    FS.current_dir = 0;
}

FileSystem* fs_get()
{
    return &FS;
}

static void fs_get_full_path(char* buffer, uint32_t current_dir_id) 
{
    Node* node = &(FS.file_nodes[current_dir_id]);
    buffer[0] = '\0'; // Initialize the buffer

    if (node->unique_id == 0) {
        strcpy(buffer, "/"); // Root directory
        return;
    }
    fs_get_full_path(buffer, node->parent_id); // Recursive call to get parent path
    strcat(buffer, node->name); // Append current node name
    strcat(buffer, "/"); // Append a slash

}

void fs_get_current_path(char* buffer)
{
    fs_get_full_path(buffer, FS.current_dir);
}


// THIS FUNCTION CANNOT READ FILES THAT ARE BIGGER THAN ONE SECTOR
char* fs_read_file(Node* node) {
    if (node->size != 0 ) {
        char* buf = kmalloc(node->size);
        ide_read_sector(node->disk_sector, buf); 
        return buf;
    } else {
        return node->content;
    }
}

Node* fs_create_file(const char* name, uint32_t size, uint32_t disk_sector) 
{
    Node* file = (Node*)kmalloc(sizeof(Node));
    strcpy(file->name, name);
    file->size = size;
    file->disk_sector = disk_sector;
    file->unique_id = FS.uid++; // Unique ID can be set later
    file->parent_id = FS.current_dir; // Unique ID can be set later
    file->child_count = 0; // Unique ID can be set later
    file->content = NULL; // No content for disk-backed files
    return file;
}

void fs_display_node(const Node* node)
{
    print("Node Name: ");
    print(node->name);
    print("\nSize: ");
    print_hex(node->size);
    print("\nDisk Sector: ");
    print_hex(node->disk_sector);
    print("\nUnique ID: ");
    print_hex(node->unique_id);
    print("\nParent ID: ");
    print_hex(node->parent_id);
    print("\nChild Count: ");
    print_hex(node->child_count);
    print("\n");
}