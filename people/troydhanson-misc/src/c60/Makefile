all: libc60.a testcli testsrv

libc60.a: c60_cli.o c60_srv.o c60.o
	ar cr $@ $^

testcli: testcli.c libc60.a
	$(CC) -o $@ $< -lzmq -L. -lc60

testsrv: testsrv.c libc60.a
	$(CC) -o $@ $< -lzmq -L. -lc60

.PHONY: clean

clean:
	rm -f *.o *.a testcli
