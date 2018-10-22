CC=gcc
CFLAGS=-fno-stack-protector
TARGET = myserver 
OBJS = server.c

server: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
