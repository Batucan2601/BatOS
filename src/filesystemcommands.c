#include "filesystemcommands.h"
#include "filesystem.h"
#include "stringcustom.h"
#include "vga.h"

#define COMMANDS_COUNT 5
#define MAX_COMMAND_LENGTH 32
char* commands[] = {
    "ls",
    "cd",
    "mkdir",
    "touch",
    "rm"
};
void ls()
{
    FileSystem* fs = fs_get();
    Node* current_dir = &fs->file_nodes[fs->current_dir];
    for (size_t i = 0; i < current_dir->child_count; i++)
    {
        uint32_t child_id = current_dir->children_id[i];
        print(fs->file_nodes[child_id].name);
    }
}

void cd(char* path)
{
    FileSystem* fs = fs_get();
    Node* current_dir = &fs->file_nodes[fs->current_dir];

    if (strcmp(path, "..") == 0) 
    {
        if (current_dir->parent_id != NULL) 
        {
            fs->current_dir = current_dir->parent_id;
        }
    } 
    else 
    {
        print_hex(current_dir->child_count);
        for (int i = 0; i < current_dir->child_count; i++) {
            if (strcmp(fs->file_nodes[current_dir->children_id[i]].name, path) == 0) {
                fs->current_dir = current_dir->children_id[i];
                print("Changed directory to: ");
                print(fs->current_dir);
                return;
            }
        }
    }
}
void mkdir(char* name)
{
    FileSystem* fs = fs_get();
    uint32_t current_dir = fs->current_dir;
    if (fs->file_nodes[current_dir].child_count < 16) 
    {
        Node new_dir; 
        strcpy(new_dir.name, name);
        new_dir.size = 0;
        new_dir.disk_sector = 1; //THIS WILL CHANGE LATER 
        new_dir.unique_id = fs->uid++;
        new_dir.parent_id = current_dir;
        new_dir.child_count = 0;
        fs->file_nodes[current_dir].children_id[fs->file_nodes[current_dir].child_count++] = new_dir.unique_id;
        fs->file_nodes[fs->file_count++] = new_dir; // Add the new directory to the file system
    }
}
void touch(char* name)
{
    /*FileSystem* fs = fs_get();
    Node* current_dir = fs->current_dir;

    if (current_dir->child_count < 16) {
        Node* new_file = (Node*)kmalloc(sizeof(Node));
        strcpy(new_file->name, name);
        new_file->type = FILE_NODE;
        new_file->content = NULL;
        new_file->parent = current_dir;
        current_dir->children[current_dir->child_count++] = new_file;
    } */
}
void rm(char* name)
{
    /*FileSystem* fs = fs_get();
    Node* current_dir = fs->current_dir;

    for (int i = 0; i < current_dir->child_count; i++) {
        if (strcmp(current_dir->children[i]->name, name) == 0) {
            kfree(current_dir->children[i]);
            for (int j = i; j < current_dir->child_count - 1; j++) {
                current_dir->children[j] = current_dir->children[j + 1];
            }
            current_dir->child_count--;
            return;
        }
    } */
}

uint32_t int_to_string(uint32_t num)
{
    char buffer[12]; // Enough to hold 32-bit integer in decimal
    char* ptr = buffer + sizeof(buffer) - 1; // Point to the end of the buffer
    *ptr = '\0'; // Null-terminate the string

    do {
        *--ptr = (num % 10) + '0'; // Convert digit to character
        num /= 10; // Divide by 10
    } while (num != 0);

    return (uint32_t)ptr; // Return pointer to the start of the string
}
void show_directory()
{
    char buffer[128];
    buffer[0] = '\0';
    fs_get_current_path(buffer);
    print(buffer);
    vga_write_directory(buffer);
}

void execute_command(char* command)
{
    uint8_t is_command = 0;
    // Remove leading and trailing whitespace
    while (*command == ' ') command++;    
    for (size_t i = 0; i < COMMANDS_COUNT; i++)
    {
        size_t cmd_len = strlen(commands[i]);
        if (strncmp(command, commands[i], cmd_len) == 0 && 
            (command[cmd_len] == ' ' || command[cmd_len] == '\0'))
        {
            is_command = 1; 
            // Pointer to argument after command
            char* arg = NULL;
            if (command[cmd_len] == ' ')
            {
                arg = command + cmd_len + 1;
            }

            switch(i)
            {
                case 0:
                    ls();
                    break;
                case 1:
                    cd(arg);
                    break;
                case 2:
                    mkdir(arg);
                    break;
                case 3:
                    touch(arg);
                    break;
                case 4:
                    rm(arg);
                    break;
            }
            break;
        }
    }
    if( is_command == 0 )
    {
        print("Command not found\n");
    }
    show_directory();
}