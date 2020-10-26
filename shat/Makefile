CFLAGS := -g -Wall
CC := gcc
RM := rm

SRC_PATH := src/
OBJ_PATH := build/
INC_PATH := -Iinclude/


TARGET := server client
DEPS := $(addprefix $(OBJ_PATH), common.o sem.o)

.PHONY: clean

all: $(TARGET)

server: build/server.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

client: build/client.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC_PATH) 

clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv $(TARGET)

