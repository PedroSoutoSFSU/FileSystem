/**************************************************************
* Class: CSC-415-0# Spring 2020
* Group Name: Last Minute
* Name: Pedro Souto
* Student ID: 918412864
* Name: Aaron Schlichting
* Student ID: 917930213
* Name:
* Student ID:
* Name:
* Student ID:
*
* Project: Assignment 3 – File System
* File: fsStructures.h
*
* Description: Header file containing all structures that the file system implements.
* **************************************************************/

#ifndef fsStructures_h
#define fsStructures_h
#include <stdio.h>
#endif /* fsStructures_h */

// Permissions masking values
#define OTHER_EXECUTE 0x0001
#define OTHER_WRITE 0x0002
#define OTHER_READ 0x0004
#define GROUP_EXECUTE 0x0008
#define GROUP_WRITE 0x0010
#define GROUP_READ 0x0012
#define USER_EXECUTE 0x0014
#define USER_WRITE 0x0016
#define USER_READ 0x0018

/*
 Total space used by Volume Control Block: 592 bits - 74 bytes
 Volume control block should be <= 512 bytes, in order to fit into an LBA block
 STORED IN LBA BLOCK 0
*/
struct volumeControlBlock {
    // Total number of bytes a HDD has
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t volumeSize;
    
    // Location of root directory
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t rootDirectory;
    
    // Block of the current directory we are at
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t currentDirectory;
    
    // Number of directories on the partition
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t numberOfDirectories;
    
    // Char array representing the name of the volume
    // Assigned by the user. If not assigned by user, generated by file system
    // 240 bits - 30 bytes
    char volumeName[30];
    
    // Random number representing the HDD
    // Assigned randomly by file system - really large random value. Small chance for collision
    // 32 bits [0, 4,294,967,295] - 4 bytes
    uint32_t volumeID;
    
    // Size of an LBA block in bytes
    // Default will be 512 bytes, should not be changed
    // 64 bits  [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t blockSize;
    
    // Total number of blocks in the partition
    // 64 bits  [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t numberOfBlocks;
};


/*
 Description: Keeps track of the free blocks of the LBA
 STORED IN LBA BLOCKS 1-49
 This is a total of 49 blocks. If you have 512 byte blocks, this can store (49 * 512) = 25,088 bytes = 200,704 bits = 200,704 POSSIBLE BLOCKS
 If each block is 512 bytes, then 200,704 POSSIBLE BLOCKS allows you to have a partition of size 102,760,448 bytes = 102.76 mbs.
*/
struct freeSpaceInformation {
    // Number of bytes a partition has free to be used
    // As files are added or removed, this number changes to represent those changes
    // 64 bits [0, 18,446,744,073,709,551,615]
    uint64_t freeSpace;
    
    // Lowest LBA block accessible for this partition
    // [0, 255]
    uint8_t lowestBlockAccessible;
    
    // Highest LBA block accessible for this partition
    // 32 bits [0, 18,446,744,073,709,551,615]
    uint64_t highestBlockAccessible;
    
    // Bitmap of all used spaces in the partition, where each bit represents a single block in the LBA
    // 0 indicates an used block, 1 indicates a free block
    int freeBlockBitArray[];
};

/*
 Total space used per directory entry: 4096 bits - 512 bytes
 One directory per LBA block
 STORED IN LBA BLOCKS 50 - 99
 This is a total of 49 blocks, which means you can have a total of 49 directories/files in this file sytem. While not many, it is trivial to add more, so we simply left at this number. Adding more would be as simple as increasing the end block from 99 to X, where is X is how many directories/files you want to have.
 */
struct directoryEntry {
    // Char array representing the name of the directory/file
    // 240 bits - 30 bytes
    char name[30];
    
    // Char array represeting the file type (ie: pdf, txt, xcodeproj, ...etc)
    // 128 bits - 16 bytes
    char fileExtension[16];
    
    // Location this entry sits at in the LBA
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t blockLocation;
    
    // A 16 bit bitmap representing the file permissions
    // A bit map of 111100100 is saved as a 484
    // 16 bits [0, 65,535] - 2 bytes
    uint16_t permissions;
    
    // Number of second since January 1st, 1970: Unix time (also known as Epoch time)
    // 32 bits [0, 4,294,967,295] - 4 bytes
    uint32_t dateCreated;
    
    // Number of second since January 1st, 1970: Unix time (also known as Epoch time)
    // 32 bits [0, 4,294,967,295] - 4 bytes
    uint32_t dateModified;
    
    // Number of bytes a file uses
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t fileSize;
    
    // Block of the parent directory
    // 64 bits [0, 18,446,744,073,709,551,615] - 8 bytes
    uint64_t parentDirectory;
    
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    // * An array of block numbers - 432 bytes. Maximum of 54 ELEMENTS    <---- IMPORTANT!!!!                                  *
    // * FOR A DIRECTORY, these blocks will point to other directories ONLY (remember: a directory can be a file OR directory) *
    // * FOR A FILE, these blocks will ONLY point to the blocks that make up the ACTUAL FILE                                   *
    // * Each element is 64 bits [0, 18,446,744,073,709,551,615]                                                               *
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    uint64_t indexLocations[54];
};
