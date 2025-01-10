# Variabili
CC = gcc
CFLAGS = -std=c11
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

VERBOSE = true

# File sorgenti e oggetti
# SRC = $(wildcard $(SRC_DIR)/*.c)
SRC = $(SRC_DIR)/processes.c $(SRC_DIR)/entities.c $(SRC_DIR)/display.c $(SRC_DIR)/handler.c $(SRC_DIR)/main.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target finale
TARGET = $(BIN_DIR)/main

# Regole
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lncurses -g
	rm -rf $(OBJ_DIR)/*

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compilazione $<"
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run