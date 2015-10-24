//
// Created by lesaha on 22.10.15.
//

#ifndef MY_LOG_LIBRARY_FS_DRIVER_H
#define MY_LOG_LIBRARY_FS_DRIVER_H

#include <fat32_structures.h>

#define FILE_PATH_MAX_LEN 256
#define FILE_NAME_MAX_LEN 8
#define FILE_EXTENSIO_MAX_LEN 3

typedef struct{
    char *fs_mmap;
    ssize_t mmap_size;
    uint32_t* FAT;
    char *currPath;
    BootRecord *bR;
    DirectoryEntry *currDir;
    DirectoryEntry virtualRootDir;
    ssize_t cluster_size;
} FSState;

FSState * createFSState(char *fs_mmap, ssize_t mmap_size, char *path, BootRecord *bR);
void destroyFSState(FSState* fsState);

DirectoryEntry *changeDirectory(FSState* fsState, char *path);
DirectoryEntry *getPtrToDirectory(FSState* fsState, char *path, DirectoryEntry*directory);
DirectoryEntry *getPtrToRootDirectory(FSState* fsState);
DirectoryEntry *getInnerDirectories(FSState* fsState, DirectoryEntry *directories);
DirectoryEntry *getFileWithNameInDirectory(FSState* fsState, DirectoryEntry* dir, char *name);
char *getPtrToFile( FSState* fsState, uint32_t cluster_number);
int compareFileAndDirecrtoryName(DirectoryEntry *dir, char *name);
char* getFileName( DirectoryEntry* dir);
char *readFile(FSState* fsState, DirectoryEntry *dir);
void listDirectory( FSState* fsState, DirectoryEntry* dir);

#endif //MY_LOG_LIBRARY_FS_DRIVER_H
