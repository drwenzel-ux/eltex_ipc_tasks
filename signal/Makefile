CFLAGS := -g -Wall
CC := gcc
RM := rm

TARGET := test ignore suspend

all: test ignore suspend receiver sender

.PHONY: clean

receiver: receiver.o utils.o
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

sender: sender.o utils.o
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^


ignore: ignore.o utils.o
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^


test: test.o
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

suspend: suspend.o
	@echo [INFO] Creating Binary Executable [$@]
	@$(CC) -o $@ $^

%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -o $@ -c $<


clean:
	@rm -vf receiver sender suspend test ignore *.o




