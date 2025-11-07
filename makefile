CC = gcc

FlAGS = -I ./NtyCo/core/ -L ./NtyCo/ -lntyco

SRCS = KVstore.c Reactor.c KVstore_array.c nty_server.c
TARGET = main

OBJS = $(SRCS:.c=.o)
SUBDIR = ./NtyCo/

all:$(SUBDIR) $(TARGET)

$(SUBDIR):
	make -C $@
	
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(FlAGS)

%.o: %.c 
	$(CC) $(FlAGS) -c $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)