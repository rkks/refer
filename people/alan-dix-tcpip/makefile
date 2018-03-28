PD=/n/Numbers/usr/roger/research/presenter/prespub3/presenter
SDIR=.
CFLAGS=$(CCFLAGS) -I$(SDIR)/include -I$(PD)
CC=cc
LINK=cc
INF=$(SDIR)/lib/inform.o
MYLIBS=$(SDIR)/lib/sock.a
LIBS=

server:	server.o $(MYLIBS)
	$(LINK) $(CFLAGS) -o server server.o $(INF) $(MYLIBS)
client:	client.o $(MYLIBS)
	$(LINK) $(CFLAGS) -o client client.o $(INF) $(MYLIBS)
