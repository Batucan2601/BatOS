#include "filesystem.h"
#include "kheap.h"
#include <stdint.h>
#include "stringcustom.h"
FileSystem* FS;
void init_fs() {
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

