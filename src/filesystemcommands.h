#pragma once 
#include "filesystem.h"

void execute_command(char* command);
void ls();
void cd(char* path);
void mkdir(char* name);
void touch(char* name);
void rm(char* name);