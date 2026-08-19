CC ?= gcc
CFLAGS ?= -std=c17 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O0 -g

PROGRAMS := 01_hello 02_hello_wait 03_lifetime 04_fork_count 05_pipe

.PHONY: all clean

all: $(PROGRAMS:%=bin/%)

bin:
	mkdir -p $@

bin/%: src/%.c | bin
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf bin
