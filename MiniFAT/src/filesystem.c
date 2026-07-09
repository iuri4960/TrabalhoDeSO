#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

#include "disk.h"
#include "fat.h"
#include "directory.h"

// Inicialização
int fs_init(const char *disk_name)
{
    printf("Inicializando sistema de arquivos...\n");

    // Tenta abrir o disco
    if (disk_open() != 0) {
        printf("Aviso: Disco não encontrado ou não formatado. Execute 'format'.\n");
        return -1;
    }

    // Carrega a Tabela de Alocação (Bloco 0) e o Diretório (Bloco 1) para a RAM
    fat_load();
    dir_load();

    return 0;
}

// Encerramento
int fs_shutdown(void)
{
    printf("Encerrando sistema...\n");

    // Salva o estado atual da memória para o disco antes de fechar
    fat_save();
    dir_save();
    disk_close();

    return 0;
}
// Formatação
int fs_format(void)
{
    printf("Formatando disco...\n");

    // 1. Cria o arquivo físico do disco vazio
    if (disk_create() != 0) {
        return -1;
    }
    
    // 2. Abre o disco para podermos escrever nele
    disk_open();

    // 3. Preenche todos os 1024 blocos com zeros
    disk_format();

    // 4. Inicializa as estruturas na memória RAM
    fat_init();
    dir_init();

    // 5. Salva essas estruturas limpas nos Blocos 0 e 1 do disco
    fat_save();
    dir_save();

    printf("Formatação concluída com sucesso!\n");
    return 0;
}

// Criar Arquivo
int fs_create(const char *filename)
{
    printf("Criando arquivo: %s\n", filename);

    if (dir_find(filename) != -1) {
        printf("Erro: O arquivo '%s' já existe.\n", filename);
        return -1;
    }

    if (dir_create(filename) == -1) {
        printf("Erro: Não foi possível criar o arquivo (diretório cheio?).\n");
        return -1;
    }

    dir_save();
    printf("Arquivo '%s' criado com sucesso.\n", filename);
    return 0;
}

// Escrever Arquivo
int fs_write(const char *filename,
             const char *data,
             int size)
{
    printf("Escrevendo arquivo: %s\n", filename);

    int index = dir_find(filename);
    if (index == -1) {
        printf("Erro: Arquivo '%s' não encontrado.\n", filename);
        return -1;
    }

    Arquivo *arq = dir_get(index);

    // Se o arquivo já tinha dados, libera os blocos antigos na FAT (sobrescrever)
    if (arq->primeiroBloco != -1) {
        fat_free(arq->primeiroBloco);
    }

    int blocks_needed = (size == 0) ? 0 : ((size - 1) / BLOCK_SIZE) + 1;
    int first_block = -1;
    int previous_block = -1;
    int bytes_written = 0;

    for (int i = 0; i < blocks_needed; i++) {
        int current_block = fat_allocate();
        if (current_block == -1) {
            printf("Erro: Disco cheio!\n");
            return -1;
        }

        if (first_block == -1) {
            first_block = current_block;
        }

        if (previous_block != -1) {
            fat_set_next(previous_block, current_block);
        }

        // Prepara o buffer para o bloco atual
        char buffer[BLOCK_SIZE];
        memset(buffer, 0, BLOCK_SIZE);
        
        int chunk_size = (size - bytes_written < BLOCK_SIZE) ? (size - bytes_written) : BLOCK_SIZE;
        memcpy(buffer, data + bytes_written, chunk_size);
        
        disk_write_block(current_block, buffer);

        bytes_written += chunk_size;
        previous_block = current_block;
    }

    // Atualiza os metadados no diretório e salva tudo no disco
    dir_update(filename, size, first_block);
    fat_save();
    dir_save();

    printf("Escritos %d bytes no arquivo '%s'.\n", size, filename);
    return 0;
}

// Ler Arquivo
int fs_read(const char *filename)
{
    printf("Lendo arquivo: %s\n", filename);

    int index = dir_find(filename);
    if (index == -1) {
        printf("Erro: Arquivo '%s' não encontrado.\n", filename);
        return -1;
    }

    Arquivo *arq = dir_get(index);

    if (arq->tamanho == 0 || arq->primeiroBloco == -1) {
        printf("(Arquivo vazio)\n");
        return 0;
    }

    int current_block = arq->primeiroBloco;
    int bytes_read = 0;
    char buffer[BLOCK_SIZE];

    printf("--- Início do Conteúdo ---\n");
    while (current_block != FAT_EOF && current_block != FAT_FREE && current_block != -1) {
        disk_read_block(current_block, buffer);
        
        int chunk_size = (arq->tamanho - bytes_read < BLOCK_SIZE) ? (arq->tamanho - bytes_read) : BLOCK_SIZE;

        // Imprime exatamente os caracteres do arquivo, sem lixo de memória
        for (int i = 0; i < chunk_size; i++) {
            putchar(buffer[i]);
        }

        bytes_read += chunk_size;
        current_block = fat_next(current_block);
    }
    printf("\n--- Fim do Conteúdo ---\n");

    return 0;
}

// Remover Arquivo
int fs_remove(const char *filename)
{
    printf("Removendo arquivo: %s\n", filename);

    int index = dir_find(filename);
    if (index == -1) {
        printf("Erro: Arquivo '%s' não encontrado.\n", filename);
        return -1;
    }

    Arquivo *arq = dir_get(index);

    // Libera o espaço físico no disco (FAT)
    if (arq->primeiroBloco != -1) {
        fat_free(arq->primeiroBloco);
    }

    // Remove a entrada lógica do diretório
    dir_delete(filename);

    fat_save();
    dir_save();

    printf("Arquivo '%s' removido com sucesso.\n", filename);
    return 0;
}

// Renomear Arquivo
int fs_rename(const char *old_name,
              const char *new_name)
{
    printf("Renomeando %s -> %s\n", old_name, new_name);

    if (dir_rename(old_name, new_name) == -1) {
        printf("Erro: Não foi possível renomear. Verifique se o arquivo existe ou se o novo nome já está em uso.\n");
        return -1;
    }

    dir_save();
    printf("Arquivo renomeado com sucesso.\n");
    return 0;
}

// Listar Arquivos
int fs_list(void)
{
    dir_list();
    return 0;
}