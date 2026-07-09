#ifndef DISK_H
#define DISK_H

#include <stdint.h>

// Config do Disco
#define BLOCK_SIZE 4096 // Tamanho dos blocos em bytes
#define NUM_BLOCKS 1024 // Numero de blocos
#define DISK_NAME "disco.bin"

// Interface
int disk_create();
int disk_open();
int disk_close();
int disk_read_block(int block_num, void *buffer);
int disk_write_block(int block_num, const void *buffer);
int disk_format();

#endif