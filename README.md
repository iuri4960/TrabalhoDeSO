![Logo UFC](imagens/logo_ufc.png)

## Integrantes

- **Matheus Reis**- Implementação de `fat.c`, criação do `Makefile` (automatizar e facilitar execução dos códigos), integração dos códigos e documentação/organização do repositório;
- **Vinicius Alexandre** - Implementação de `disk.c` e revisor do projeto;
- **Yuri Fontenele**  - Implementação de `directory.c` criação do Documento Orientativo e Slides para apresentação;
- **Iuri Fernandes** - Implementação de `main.c` e `filesystem.c`, além dos Slides para apresentação.

---


# MiniFAT - Sistema de Arquivos em C

Esse repositório tem como objetivo principal documentar e organizar todos os códigos que compõem o trabalho final da cadeira de Sistemas Operacionais do Prof. Jardel Silveira.

## Objetivo Geral

Desenvolver um sistema de arquivos simplificado utilizando a linguagem C, implementando operações básicas de gerenciamento de arquivos por meio de chamadas de sistema.

## Descrição do Projeto

Na discussão de desenvolvimento do projeto, enfrentamos diversas problemáticas ligadas a falta de conhecimento técnico para criação de um sistema mais complexo e também a chamadas de sistema diretos ao nosso Kernel, o que poderia gerar falhas no nosso desenvolvimento e também problemas de compatibilidade para outros sistemas.

Dito isso, decidimos que o projeto consistiria na implementação de um sistema de arquivos simplificado chamado **MiniFAT** (versão simplificada do FAT), cujo armazenamento é realizado em um arquivo binário denominado `disco.bin`, que representa um disco virtual. Essa abordagem em espaço de usuário garante a segurança do sistema hospedeiro contra travamentos fatais

Toda a estrutura do sistema é organizada em blocos de tamanho fixo, contendo:

- **FAT (File Allocation Table)**: Atua como o mapa principal de alocação do disco. Monitora quais blocos estão livres, gerando uma lista encadeada lógica conectando os fragmentos de um mesmo arquivo, garantindo salvar esses arquivo de maneira não contígua e íntegra;
- **Diretório Raiz**: Funciona como o catálogo de metadados do sistema. É a estrutura responsável por armazenar atributos dos arquivos, como nome e tamanho, e mapeá-los para o seu índice correspondente na tabela de alocação (FAT); 
- **Área de Dados**: Finalmente, essa área corresponde ao espaço útil de armazenamento dos nossos arquivos. Nessa região que as syscalls de escrita depositam efetivamente os dados dos arquivos criados pelo usuário, fatiados em tamanhos fixos.

O usuário interage com o sistema através de um terminal (shell), executando comandos semelhantes aos encontrados em sistemas operacionais convencionais.

Os principais comandos disponíveis são:

| Comando | Função |
|----------|--------|
| format | Formata o disco virtual |
| create | Cria um novo arquivo |
| write | Escreve dados em um arquivo |
| read | Lê o conteúdo de um arquivo |
| ls | Lista os arquivos existentes |
| rm | Remove um arquivo |
| rename | Renomeia um arquivo |
| exit | Encerra o programa |

---

## Chamadas de Sistema Utilizadas

O projeto faz uso das seguintes chamadas de sistema da API POSIX:

- `open()`
- `read()`
- `write()`
- `lseek()`
- `close()`
- `ftruncate()`

Essas chamadas são utilizadas para manipular diretamente o arquivo `disco.bin`, responsável por representar o disco virtual.

---


## Diagrama de Blocos

```
                 Usuário

                    │

                    ▼

              Interface (main.c)

                    │

                    ▼

           Camada de Sistema de Arquivos
               (filesystem.c)

          ┌─────────┼─────────┐
          │         │         │
          ▼         ▼         ▼

     Diretório      FAT      Disco

 (directory.c)   (fat.c)  (disk.c)

          └─────────┼─────────┘
                    │
                    ▼

              disco.bin
          (Disco Virtual)
```

---


## Estrutura do Projeto

```text
TRABALHODESO-MAIN/
├── imagens/                            # Recursos visuais do repositório
│   └── logo_ufc.png                    # Logo e imagens utilizadas na documentação
│
├── MiniFAT/                            # Implementação prática em C do sistema
│   ├── inc/                            # Pasta de Cabeçalhos (.h)
│   │   ├── directory.h                 # Interface dos metadados e catálogo de arquivos
│   │   ├── disk.h                      # Interface de comunicação de baixo nível (syscalls)
│   │   ├── fat.h                       # Interface da Tabela de Alocação de Arquivos
│   │   └── filesystem.h                # API principal do sistema de arquivos
│   │
│   ├── src/                            # Pasta de Código-Fonte (.c)
│   │   ├── directory.c                 # Implementação do gerenciador de diretórios
│   │   ├── disk.c                      # Implementação do disco virtual simulado
│   │   ├── fat.c                       # Implementação do gerenciamento de blocos livres/ocupados
│   │   ├── filesystem.c                # Orquestração de todas as camadas (abstração de alto nível)
│   │   └── Main.c                      # Ponto de entrada (Shell interativo do usuário)
│   │
│   ├── Makefile                        # Script de automação para compilação do MiniFAT
│   └── README.md                       # Documentação orientativa para rodar o código
│
├── ZFS/                                # Estudo teórico e análise do ZFS
│   ├── Sistema de Arquivos ZFS.pdf     # Documento detalhando os conceitos e arquitetura do ZFS
│   └── Sistema-de-Arquivos-ZFS.pptx    # Apresentação de slides sobre o sistema estudado
│
└── README.md                           # Documentação principal do projeto
```

## Uso de Inteligência Artificial

O uso de chat (Large Language Models) foi direcionado principalmente a:
 - Revisão teórica do FAT assim como proposição de um sistema mínimo;
 - Revisão teórica das chamadas de sistemas para implementação das chamadas de sistema;
 - Revisão de erros lógicos eventuais do desenvolvimento do projeto;
 - Criação integral do Makefile após criação da arquitetura do projeto (inc e src folders).

 O uso foi totalmente direcionado para o **suporte teórico e prático** do desenvolvimento do projeto, **dispensando a IA como um desenvolvedor independente do projeto**