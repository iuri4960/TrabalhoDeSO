# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
BIN_DIR = bin

# Nome do executável final agora dentro da pasta bin
TARGET = $(BIN_DIR)/minifs

# Compilador e flags (corrigido para a sua pasta inc)
CC = gcc
CFLAGS = -Wall -I$(INC_DIR)

# Encontra todos os arquivos .c na pasta src automaticamente
SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Define que para cada .c, existirá um .o na pasta obj
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra padrão (o que roda quando você digita só 'make')
all: $(TARGET)

# Como juntar tudo (linkar) para criar o executável final
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@echo "Montando o executável final: $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Pronto! Para rodar digite: ./$(TARGET)"

# Como compilar cada .c em um .o (cria a pasta obj se não existir)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar a bagunça e resetar o disco
clean:
	@echo "Limpando arquivos compilados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) disco.bin

# Define que 'all' e 'clean' não são arquivos de verdade
.PHONY: all clean