# Variabili
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# File sorgenti e oggetti per il primo eseguibile
SRC = $(SRC_DIR)/network.c $(SRC_DIR)/utils.c  $(SRC_DIR)/processes.c $(SRC_DIR)/entities.c $(SRC_DIR)/music.c $(SRC_DIR)/display.c $(SRC_DIR)/handler.c $(SRC_DIR)/main.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# File sorgenti e oggetti per il secondo eseguibile
SRC_CLIENT = $(SRC_DIR)/network.c $(SRC_DIR)/client.c
OBJ_CLIENT = $(SRC_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target finali
TARGET = $(BIN_DIR)/frogger
TARGET_CLIENT = $(BIN_DIR)/client

# Regole
all: $(TARGET) $(TARGET_CLIENT)

# Controllo sistema operativo
ifeq ($(shell uname), Darwin)
    LIBS = -lpthread -lncurses 
else
    LIBS = -lpthread -lncursesw 
endif

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -g

$(TARGET_CLIENT): $(OBJ_CLIENT)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -g
	rm -rf $(OBJ_DIR)/*

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run