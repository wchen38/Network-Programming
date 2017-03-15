C = gcc
CFLAGS = -g -Wall -ggdb
PROGS = server client
SRCS = server.c client.c
OBJS = $(SRCS:.c=.o)

images : server.c client.c
	$(CC) $(CFLAGS) -o ../bin/server server.c
	$(CC) $(CFLAGS) -o ../bin/client client.c

clean:
		rm -f ../bin/${PROGS} ./bin/${CLEANFILES}
#		killall ../bin/server
