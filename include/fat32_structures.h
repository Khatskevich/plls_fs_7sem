#include <stdio.h>

typedef struct{
    char executabla_code[446];
    char partition_entry_1[16];
    char partition_entry_2[16];
    char partition_entry_3[16];
    char partition_entry_4[16];
    uint16_t signature; // 55hAAh
}MasterBootRecord;

typedef struct{
    uint8_t current_state_of_partition;
    uint8_t beginning_of_partition_head;
    uint16_t beginning_of_partition_cylinder;
    uint8_t type_of_partition;
    uint8_t end_of_partition_head;
    uint16_t end_of_partition_cylinder;
    uint32_t offset_to_the_first_sector;
    uint32_t number_of_sectors;
}PartitionEntry; // mbr

typedef struct{
    char jump_code[3];
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_copies_of_fat;
    uint16_t max_root_dir_entries;
    uint16_t number_of_sectors_in_partition_smaller_then_3mb;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t number_of_hidden_sectors;
    uint32_t number_of_sectors;
    uint32_t number_of_sectors_per_fat;
   /*28h*/ uint16_t flags;
    uint16_t version_of_fat32;
    uint32_t cluster_number_of_the_root_directory;
    uint16_t sector_number_of_fs_information_sector;
    uint16_t sector_number_of_the_backup_boot_sector;
    char reserved[12];
    uint8_t logical_drive_number_of_partition;
    uint8_t unused;
    uint8_t extended_signature; // 29h
    uint32_t serial_number_of_partition;
    char voluma_name_of_partition[11];
    char fat_name[8];
    char executable_code[420];
    uint16_t signature; // 55hAAh
}BootRecord;

typedef struct{
    uint8_t first_signature; // 52h 52h 61h 41h
    char unknown[480]; 
    uint32_t signature; // 72h 72h 41h 61h
    uint32_t number_of_free_clusters;
    uint32_t recently_allocated_cluster_number;
    char reserver[12];
    char unknown[2];
    uint16_t signature; // 55hAAh
}FSInfoSector;