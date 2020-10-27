CC := gcc
RM := rm

SRC_PATH := src/
OBJ_PATH := build/
INC_PATH := -Iinclude/
LDFLAGS := -lrt -lpthread


TARGET := writer reader
DEPS := $(addprefix $(OBJ_PATH), co2mon.o)

.PHONY: clean

all: $(TARGET)

writer: build/writer.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^ $(LDFLAGS) 

reader: build/reader.o $(DEPS)
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^ $(LDFLAGS) 

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC_PATH)

clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv $(TARGET)