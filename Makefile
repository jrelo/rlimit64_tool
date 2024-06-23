CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=gnu99

TARGETS = rlimit64_tool open_files_test

SRCS = rlimit64_tool.c open_files_test.c

OBJS = $(SRCS:.c=.o)

all: $(TARGETS)

rlimit64_tool: rlimit64_tool.o
	$(CC) $(CFLAGS) -o rlimit64_tool rlimit64_tool.o

open_files_test: open_files_test.o
	$(CC) $(CFLAGS) -o open_files_test open_files_test.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: all clean
