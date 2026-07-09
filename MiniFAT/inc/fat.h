#ifndef FAT_H
#define FAT_H

#define FAT_FREE -1
#define FAT_EOF -2

// Inicializa a FAT em memória (tudo livre, exceto blocos de sistema)
void fat_init();

// Carrega a FAT do disco (Bloco 0) para a memória
int fat_load();

// Salva a FAT da memória para o disco (Bloco 0)
int fat_save();

// Procura um bloco livre, marca como FAT_EOF e retorna o índice
int fat_allocate();

// Libera uma cadeia de blocos a partir de um bloco inicial
void fat_free(int first_block);

// Retorna o próximo bloco da cadeia
int fat_next(int block);

// Define o próximo bloco de um bloco atual
void fat_set_next(int block, int next);

#endif