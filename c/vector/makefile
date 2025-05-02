CC = gcc

CFLAGS = -Wall -std=c23

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)


test: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c $@

run:
	./test

clean:
	rm -f $(OBJS) test


