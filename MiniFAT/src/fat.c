#include "fat.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

// A tabela FAT na memória. Como temos 1024 blocos, 
// 1024 * sizeof(int) = 4096 bytes (exatamente 1 bloco!)
static int fat_table[NUM_BLOCKS]; 

void fat_init() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        fat_table[i] = FAT_FREE;
    }
    
    // Bloco 0 é da própria FAT, Bloco 1 é do Diretório. Não podem ser usados para dados.
    fat_table[0] = FAT_EOF;
    fat_table[1] = FAT_EOF;
}

int fat_load() {
    // Lê o bloco 0 do disco e joga direto no array fat_table
    return disk_read_block(0, fat_table);
}

int fat_save() {
    // Escreve o array fat_table inteiro de volta no bloco 0
    return disk_write_block(0, fat_table);
}

int fat_allocate() {
    // Procura o primeiro bloco livre (começando do bloco 2, pois 0 e 1 são sistema)
    for (int i = 2; i < NUM_BLOCKS; i++) {
        if (fat_table[i] == FAT_FREE) {
            fat_table[i] = FAT_EOF;
            return i;
        }
    }
    return -1; // Disco cheio
}

void fat_free(int first_block) {
    int current = first_block;
    while (current != FAT_EOF && current != FAT_FREE && current >= 2 && current < NUM_BLOCKS) {
        int next = fat_table[current];
        fat_table[current] = FAT_FREE;
        current = next;
    }
}

int fat_next(int block) {
    if (block < 0 || block >= NUM_BLOCKS) return FAT_FREE;
    return fat_table[block];
}

void fat_set_next(int block, int next) {
    if (block >= 0 && block < NUM_BLOCKS) {
        fat_table[block] = next;
    }
}