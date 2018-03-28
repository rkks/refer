
PRODUCTS = dlinfo dlunitdatareq dlrcv dlmdata

all: $(PRODUCTS)

clean:
	rm -f *.o 

clobber: clean
	rm -f $(PRODUCTS)

dlcommon.o:	dlcommon.c
	$(CC) -c dlcommon.c

dlinfo: dlinfo.c dlcommon.o
	$(CC) dlinfo.c dlcommon.o -o dlinfo

dlunitdatareq: dlunitdatareq.c dlcommon.o
	$(CC) dlunitdatareq.c dlcommon.o -o dlunitdatareq

dlrcv: dlrcv.c dlcommon.o
	$(CC) dlrcv.c dlcommon.o -o dlrcv

dlmdata: dlmdata.c dlcommon.o
	$(CC) dlmdata.c dlcommon.o -o dlmdata

dlcommon.o:	dlcommon.c
