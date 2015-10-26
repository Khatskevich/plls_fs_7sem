#include "fat32_structures.h"

#define DEBUG

#include "log.h"
#include "fs_driver.h"

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fat_reader.h>
#include <string.h>
#include <fat32_structures.h>


BootRecord *bR;

int startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

int performCommand(FSState *fsState, char *next_command);

int main(int argc, char **argv) {
    logChangeLvl(LOG_INFO);
    LOGMESG(LOG_INFO, "FAT READER STARTED!");
    if (argc != 2) {
        LOGMESG(LOG_ERROR, "Wrong arguments! Please run programm with path to img or raw mounted FAT32");
        goto main_error_0;
    }
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        LOGMESG(LOG_ERROR, "Can not open file!");
        goto main_error_1;
    }
    struct stat sb;
    fstat(fd_in, &sb);
    LOGMESG(LOG_INFO, "File size = %ld", sb.st_size);
    if (sb.st_size == 0) {
        sb.st_size = DEFAULT_FILE_SIZE;
        LOGMESG(LOG_WARN, "File size cna not be determined, so, the default file size was set = %ld", sb.st_size);
    }
    char *mmap_start = mmap(NULL, sb.st_size, PROT_READ,
                            MAP_SHARED, fd_in, 0);
    if (mmap_start == NULL) {
        LOGMESG(LOG_WARN, "Can not create mmap");
        goto main_error_2;
    }
    /*
     * reading mbr
     */
    size_t n;
    char *next_command = NULL;
    char *boot_record_start_byte = DEFAULT_OFFSET_TO_BOOT_RECORD + mmap_start;
    bR = (BootRecord *) boot_record_start_byte;
    FSState *fsState = createFSState(mmap_start, sb.st_size, "/", bR);
    while (getline(&next_command, &n, stdin)) {
        if (next_command[0] == 0) {
            break;
        }
        performCommand(fsState, next_command);
    }
    destroyFSState(fsState);
    munmap(mmap_start, sb.st_size);
    close(fd_in);
    return 0;

    main_error_2 :
    close(fd_in);
    main_error_1 :
    main_error_0 :
    return 1;

}

int performCommand(FSState *fsState, char *next_command) {
    if (startsWith("ls", next_command)) {
        next_command += 2;
        while (next_command[0] == ' ') next_command++;
        next_command[ strlen(next_command) == 0? 0 : strlen(next_command) -1  ] = 0;
        DirectoryEntry *dir = getPtrToDirectory(fsState, next_command , NULL);
        if (dir != NULL) {
            DirectoryIterator *dirIter = createDirectoryIterator(fsState, dir);
            DirectoryEntry *nextDir = NULL;
            while ((nextDir = getNextDir(dirIter)) != NULL){
                char* fname = getFileName(nextDir);
                int day_of_month = (nextDir->date & 0x1f);
                int month = ((nextDir->date >> 5) & 0x0f);
                int year = (nextDir->date >> 9) & 0x7f;
                int seconds = (nextDir->time & 0x1f) * 2;
                int minutes = (nextDir->time >> 5) & 0x3f;
                int hours = (nextDir->time >> 11) & 0x1f;
                char date[20];
                sprintf(date, "%d.%02d.%02d %02d:%02d:%02d",year+1980, month, day_of_month, hours,minutes,seconds );
                printf("%-15s%-10u%-23s%-5o\n" ,fname ,nextDir->file_size, date, nextDir->glags);
                free(fname);
            }
            destroyDirectoryIterator(dirIter);
        } else {
            LOGMESG(LOG_ERROR, "Can not find this directory");
        }
    }
    if (startsWith("cat ", next_command)) {
        char* data;
        next_command += 3;
        while (next_command[0] == ' ') next_command++;
        next_command[ strlen(next_command) == 0? 0 : strlen(next_command) -1  ] = 0;
        DirectoryEntry *dir = getPtrToDirectory(fsState, next_command , NULL);
        if (dir != NULL) {
            data = readFile(fsState,dir);
            ssize_t  i = 0;
            for( ; i < dir->file_size; i++){
                //if ( data[i] <= 'Z' && data[i]>= '0') {// need filter
                    printf("%c", data[i]);
                    fflush(stdout); // to avoid some problems
                //}
            }
        } else {
            LOGMESG(LOG_ERROR, "Can not find this directory");
        }

    }
    if (startsWith("cd ", next_command)) {
        char* data;
        next_command += 2;
        while (next_command[0] == ' ') next_command++;
        next_command[ strlen(next_command) == 0? 0 : strlen(next_command) -1  ] = 0;
        DirectoryEntry *dir = changeDirectory(fsState, next_command);
        if (dir!=NULL) {
        }else{
            LOGMESG(LOG_ERROR, "Can not find this directory");
        }

    }
    return 0;
}