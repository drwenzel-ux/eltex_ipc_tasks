.PHONY: clean

SRC := $(wildcard *.c)
TARGET := $(SRC:%.c=%)

all: $(TARGET)

$(TARGET):
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $@.c

clean:
	@$(RM) -rfv $(TARGET)



