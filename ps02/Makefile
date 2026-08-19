CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O0 -g

.PHONY: all clean

all: bin/01_hello bin/02_hello_wait bin/03_lifetime bin/04_fork_count bin/05_pipe

bin:
	mkdir -p bin

bin/01_hello: bin src/01_hello.c
	$(CC) $(CFLAGS) src/01_hello.c -o $@

bin/02_hello_wait: bin src/02_hello_wait.c
	$(CC) $(CFLAGS) src/02_hello_wait.c -o $@

bin/03_lifetime: bin src/03_lifetime.c
	$(CC) $(CFLAGS) src/03_lifetime.c -o $@

bin/04_fork_count: bin src/04_fork_count.c
	$(CC) $(CFLAGS) src/04_fork_count.c -o $@

bin/05_pipe: bin src/05_pipe.c
	$(CC) $(CFLAGS) src/05_pipe.c -o $@

clean:
	rm -rf bin
