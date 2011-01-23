CFLAGS = -Wall -g
LDFLAGS = -lrt
HEADERS = tcp.h common.h
OBJS = tcp.o common.o

TARGETS = tcp_server tcp_client

ALL = $(OBJS) $(TARGETS)

all : $(ALL)

tcp.o : tcp.h

common.o : common.h

tcp_server.o : $(HEADERS)

tcp_server : tcp_server.o $(OBJS)

tcp_client : tcp_client.o $(OBJS)

