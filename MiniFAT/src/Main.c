#include <stdio.h>
#include <string.h>

#include "filesystem.h"

#define MAX_CMD 32
#define MAX_NAME 32
#define MAX_DATA 1024

int main(void)
{
    char command[MAX_CMD];
    char filename[MAX_NAME];
    char new_name[MAX_NAME];
    char data[MAX_DATA];

    printf("=====================================\n");
    printf("          MiniFAT File System\n");
    printf("=====================================\n");

    /* Inicializa o sistema */
    fs_init("disco.bin");

    /* Repetidamente esperando comando */
    while (1)
    {
        printf("\nMiniFS> ");

        scanf("%31s", command);
        if (strcmp(command, "format") == 0)
        {
            fs_format();
        }

        else if (strcmp(command, "create") == 0)
        {
            scanf("%31s", filename);

            fs_create(filename);
        }

        else if (strcmp(command, "write") == 0)
        {
            scanf("%31s", filename);

            getchar(); // remove o '\n'

            printf("Digite o conteúdo:\n");

            fgets(data, MAX_DATA, stdin);

            fs_write(filename,
                     data,
                     strlen(data));
        }

        else if (strcmp(command, "read") == 0)
        {
            scanf("%31s", filename);

            fs_read(filename);
        }

        else if (strcmp(command, "rm") == 0)
        {
            scanf("%31s", filename);

            fs_remove(filename);
        }


        else if (strcmp(command, "rename") == 0)
        {
            scanf("%31s", filename);
            scanf("%31s", new_name);

            fs_rename(filename,
                      new_name);
        }

        else if (strcmp(command, "ls") == 0)
        {
            fs_list();
        }

        else if (strcmp(command, "help") == 0)
        {
            printf("\nComandos disponíveis:\n");

            printf("format\n");
            printf("create <arquivo>\n");
            printf("write <arquivo>\n");
            printf("read <arquivo>\n");
            printf("rm <arquivo>\n");
            printf("rename <arquivo> <novo_nome>\n");
            printf("ls\n");
            printf("help\n");
            printf("exit\n");
        }


        else if (strcmp(command, "exit") == 0)
        {
            break;
        }


        else
        {
            printf("Comando inválido.\n");
        }
    }

    fs_shutdown();

    printf("Sistema encerrado.\n");

    return 0;
}