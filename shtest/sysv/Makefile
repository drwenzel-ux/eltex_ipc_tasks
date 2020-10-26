CC := gcc
RM := rm

SRC_PATH := src/
OBJ_PATH := build/
INC_PATH := -Iinclude/


TARGET := writer reader
DEPS := $(addprefix $(OBJ_PATH), common.o binsem.o)

.PHONY: clean

all: $(TARGET)

writer: build/writer.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

reader: build/reader.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC_PATH) 

clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv $(TARGET)