#ifndef FILESYSTEM_H
#define FILESYSTEM_H

/* Inicialização e encerramento */

int fs_init(const char *disk_name);
int fs_shutdown(void);

/* Formatação */

int fs_format(void);

/* Operações com arquivos */

int fs_create(const char *filename);

int fs_write(const char *filename,
             const char *data,
             int size);

int fs_read(const char *filename);

int fs_remove(const char *filename);

int fs_rename(const char *old_name,
              const char *new_name);

int fs_list(void);

#endif