#include <stdio.h>
#include <string.h>

#include "directory.h"
#include "disk.h"

/* Diretório armazenado em memória */
static Arquivo diretorio[MAX_FILES];

/* Inicializa o diretório */
void dir_init(void)
{
    for(int i = 0; i < MAX_FILES; i++)
    {
        diretorio[i].usado = 0;
        diretorio[i].tamanho = 0;
        diretorio[i].primeiroBloco = -1;
        diretorio[i].nome[0] = '\0';
    }
}

/* Procura um arquivo */
int dir_find(const char *nome)
{
    for(int i = 0; i < MAX_FILES; i++)
    {
        if(diretorio[i].usado &&
           strcmp(diretorio[i].nome, nome) == 0)
        {
            return i;
        }
    }

    return -1;
}

/* Cria um novo arquivo */
int dir_create(const char *nome)
{
    if(dir_find(nome) != -1)
    {
        return -1;
    }

    for(int i = 0; i < MAX_FILES; i++)
    {
        if(!diretorio[i].usado)
        {
            diretorio[i].usado = 1;

            strncpy(diretorio[i].nome,
                    nome,
                    MAX_FILENAME - 1);

            diretorio[i].nome[MAX_FILENAME - 1] = '\0';

            diretorio[i].tamanho = 0;
            diretorio[i].primeiroBloco = -1;

            return i;
        }
    }

    return -1;
}

/* Remove um arquivo */
int dir_delete(const char *nome)
{
    int indice = dir_find(nome);

    if(indice == -1)
        return -1;

    diretorio[indice].usado = 0;
    diretorio[indice].nome[0] = '\0';
    diretorio[indice].tamanho = 0;
    diretorio[indice].primeiroBloco = -1;

    return 0;
}

/* Renomeia um arquivo */
int dir_rename(const char *antigo,
               const char *novo)
{
    if(dir_find(novo) != -1)
        return -1;

    int indice = dir_find(antigo);

    if(indice == -1)
        return -1;

    strncpy(diretorio[indice].nome,
            novo,
            MAX_FILENAME - 1);

    diretorio[indice].nome[MAX_FILENAME - 1] = '\0';

    return 0;
}

/* Atualiza os metadados */
int dir_update(const char *nome,
               int tamanho,
               int primeiroBloco)
{
    int indice = dir_find(nome);

    if(indice == -1)
        return -1;

    diretorio[indice].tamanho = tamanho;
    diretorio[indice].primeiroBloco = primeiroBloco;

    return 0;
}

/* Retorna um ponteiro para o arquivo */
Arquivo *dir_get(int indice)
{
    if(indice < 0 || indice >= MAX_FILES)
        return NULL;

    if(!diretorio[indice].usado)
        return NULL;

    return &diretorio[indice];
}

/* Lista os arquivos existentes */
void dir_list(void)
{
    printf("\n-------- DIRETÓRIO --------\n");

    int vazio = 1;

    for(int i = 0; i < MAX_FILES; i++)
    {
        if(diretorio[i].usado)
        {
            vazio = 0;

            printf("Nome: %s\n", diretorio[i].nome);
            printf("Tamanho: %d bytes\n", diretorio[i].tamanho);
            printf("Primeiro bloco: %d\n",
                   diretorio[i].primeiroBloco);
            printf("-------------------------------\n");
        }
    }

    if(vazio)
    {
        printf("Diretório vazio.\n");
    }

    printf("----------------\n");
}

/* Carrega o diretório do Bloco 1 do disco para a RAM */
int dir_load(void)
{
    char buffer[BLOCK_SIZE];
    
    if(disk_read_block(1, buffer) != 0)
        return -1;
        
    memcpy(diretorio, buffer, sizeof(diretorio));
    return 0;
}

/* Salva o diretório da RAM para o Bloco 1 do disco */
int dir_save(void)
{
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE); 
    
    memcpy(buffer, diretorio, sizeof(diretorio));
    
    return disk_write_block(1, buffer);
}