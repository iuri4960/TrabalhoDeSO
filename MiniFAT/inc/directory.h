#ifndef DIRECTORY_H
#define DIRECTORY_H

#define MAX_FILES 64
#define MAX_FILENAME 32

typedef struct
{
    char nome[MAX_FILENAME];
    int tamanho;
    int primeiroBloco;
    int usado;
} Arquivo;

/* Inicializa o diretório */
void dir_init(void);

/* Cria um novo arquivo */
int dir_create(const char *nome);

/* Remove um arquivo */
int dir_delete(const char *nome);

/* Procura um arquivo pelo nome */
int dir_find(const char *nome);

/* Renomeia um arquivo */
int dir_rename(const char *antigo, const char *novo);

/* Lista todos os arquivos */
void dir_list(void);

/* Retorna um ponteiro para um arquivo */
Arquivo *dir_get(int indice);

/* Atualiza tamanho e primeiro bloco */
int dir_update(const char *nome, int tamanho, int primeiroBloco);

/* Carrega o diretório do disco para a memória (Bloco 1) */
int dir_load(void);

/* Salva o diretório da memória para o disco (Bloco 1) */
int dir_save(void);

#endif