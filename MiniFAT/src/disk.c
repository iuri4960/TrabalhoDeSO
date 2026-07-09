#include "disk.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// File descriptor
static int disk_fd = -1;

int disk_create() {
    int fd = open(DISK_NAME, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Erro ao criar o disco");
        return -1;
    }

    off_t total_size = (off_t)BLOCK_SIZE * NUM_BLOCKS;
    if (ftruncate(fd, total_size) < 0) {
        perror("Erro no ftruncate");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}


int disk_open() {
    if (disk_fd != -1) {
        return 0; // Se o disco ja esta aberto
    }

    disk_fd = open(DISK_NAME, O_RDWR);
    if (disk_fd < 0) {
        perror("Erro ao abrir o disco");
        return -1;
    }

    return 0;
}

int disk_close() {
    if (disk_fd != -1) {
        if (close(disk_fd) < 0) {
            perror("Erro ao fechar o disco");
            return -1;
        }
        disk_fd = -1;
    }
    return 0;
}


int disk_read_block(int block_num, void *buffer) {
    if (disk_fd == -1) {
        fprintf(stderr, "Erro: Disco nao esta aberto.\n");
        return -1;
    }
    
    if (block_num < 0 || block_num >= NUM_BLOCKS) {
        fprintf(stderr, "Erro: Tentativa de leitura fora dos limites (Bloco %d).\n", block_num);
        return -1;
    }

    // Calcula a posicao da memoria a partir do offset e bloco
    off_t offset = (off_t)block_num * BLOCK_SIZE;
    
    if (lseek(disk_fd, offset, SEEK_SET) == (off_t)-1) {
        perror("Erro ao posicionar ponteiro de leitura");
        return -1;
    }

    ssize_t bytes_read = read(disk_fd, buffer, BLOCK_SIZE);
    if (bytes_read != BLOCK_SIZE) {
        perror("Erro na leitura do bloco");
        return -1;
    }

    return 0;
}

int disk_write_block(int block_num, const void *buffer) {
    if (disk_fd == -1) {
        fprintf(stderr, "Erro: Disco nao esta aberto.\n");
        return -1;
    }
    
    if (block_num < 0 || block_num >= NUM_BLOCKS) {
        fprintf(stderr, "Erro: Tentativa de escrita fora dos limites (Bloco %d).\n", block_num);
        return -1;
    }

    off_t offset = (off_t)block_num * BLOCK_SIZE;

    if (lseek(disk_fd, offset, SEEK_SET) == (off_t)-1) {
        perror("Erro ao posicionar ponteiro de escrita");
        return -1;
    }

    ssize_t bytes_written = write(disk_fd, buffer, BLOCK_SIZE);
    if (bytes_written != BLOCK_SIZE) {
        perror("Erro na escrita do bloco");
        return -1;
    }

    return 0;
}


int disk_format() {
    if (disk_fd == -1) {
        fprintf(stderr, "Erro: Disco nao esta aberto para formatacao.\n");
        return -1;
    }

    // Cria um buffer de zeros
    char zero_buffer[BLOCK_SIZE];
    memset(zero_buffer, 0, BLOCK_SIZE);

    // Escreve o buffer em todos blocos
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (disk_write_block(i, zero_buffer) != 0) {
            fprintf(stderr, "Erro ao formatar no bloco %d.\n", i);
            return -1;
        }
    }

    return 0;
}