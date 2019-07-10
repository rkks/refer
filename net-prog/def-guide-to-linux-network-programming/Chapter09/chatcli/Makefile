CC = g++
INCDIR = -I $(QTDIR)/include
LDFLAGS	= -L $(QTDIR)/lib -lqt-mt

all: chatcli

chatcli: chatcli.o chatwin.o logindlg.o	moc_chatwin.o moc_logindlg.o
	$(CC) -o chatcli $(LDFLAGS) chatcli.o chatwin.o logindlg.o moc_chatwin.o moc_logindlg.o

chatcli.o: chatcli.cpp
	$(CC) -c $(INCDIR) chatcli.cpp

chatwin.o: chatwin.cpp chatwin.h moc_chatwin.cpp
	$(CC) -c $(INCDIR) chatwin.cpp

moc_chatwin.o: moc_chatwin.cpp
	$(CC) -c $(INCDIR) moc_chatwin.cpp

moc_chatwin.cpp: chatwin.h
	$(QTDIR)/bin/moc chatwin.h -o moc_chatwin.cpp

logindlg.o: logindlg.cpp logindlg.h moc_logindlg.cpp
	$(CC) -c $(INCDIR) logindlg.cpp

moc_logindlg.o: moc_logindlg.cpp
	$(CC) -c $(INCDIR) moc_logindlg.cpp

moc_logindlg.cpp: logindlg.h
	$(QTDIR)/bin/moc logindlg.h -o moc_logindlg.cpp

clean: FORCE
	rm -f *.o
	rm -f moc_*

FORCE:
